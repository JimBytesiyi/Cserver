#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 100
void read_childproc(int sig);
void error_handling(char* message);

int main(int argc, char* argv[]){

    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    int fds[2];

    pid_t pid;
    struct sigaction act;
    char buf[BUF_SIZE];
    socklen_t addr_size;
    int str_len, state;

    if(argc != 2){ 
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    state = sigaction(SIGCHLD, &act, 0);

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
        error_handling("socket() error");
    
    memset(&serv_addr, 0, sizeof(serv_addr)); 
    serv_addr.sin_family = AF_INET; // IPv4 
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
    serv_addr.sin_port = htons(atoi(argv[1])); 

    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");

    if(listen(serv_sock, 5) == -1) 
        error_handling("listen() error");

    pipe(fds);
    pid = fork();
    if(pid == 0){
        FILE* fp = fopen("echoMsg.txt", "w");
        char msg_buf[BUF_SIZE];
        int i, len;

        for(i = 0; i < 10; i++){
            len = read(fds[0], msg_buf, BUF_SIZE);
            fwrite((void*)msg_buf, i, len, fp);
        }
        fclose(fp);
        return 0;
    }
    
    while(1){
        addr_size = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &addr_size);
        if(clnt_sock == -1)
            continue;
        else
            puts("New client connected...\n");
        
        pid = fork();
        if(pid == 0){
            close(serv_sock);
            while((str_len = read(clnt_sock, buf, BUF_SIZE)) != 0){
                write(clnt_sock, buf, str_len);
                write(fds[1], buf, str_len);
            }
            
            close(clnt_sock);
            puts("Client disconnected...\n");
            return 0;
        }
        else{
            close(clnt_sock);
        }
    }
    close(serv_sock);
    return 0;
}

void error_handling(char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

void read_childproc(int sig){
    int status;
    pid_t id = waitpid(-1, &status, WNOHANG);
    if(WIFEXITED(status)){ // if child exits normally
        printf("Removed proc id: %d\n", id);
        printf("Child send: %d\n", WEXITSTATUS(status));
    }
}