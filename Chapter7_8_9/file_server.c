#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 30
void error_handling(char* message);

// iteration server

int main(int argc, char* argv[]){

    int serv_sock;
    int clnt_sock;
    char buf[BUF_SIZE];
    int read_cnt;
    FILE* fp;

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    if(argc != 2){ // pass port number as the second parameter
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    fp = fopen("file_server.txt", "r");

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
        error_handling("socket() error");
    
    memset(&serv_addr, 0, sizeof(serv_addr)); // initializing struct sockaddr_in
    serv_addr.sin_family = AF_INET; // IPv4 
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); // use local host address
    serv_addr.sin_port = htons(atoi(argv[1])); // we need to input port number

    // bind the struct sockaddr to server's socket
    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");

    if(listen(serv_sock, 5) == -1) // length of waiting queue is 5
        error_handling("listen() error");
    
    clnt_addr_size = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
    if(clnt_sock == -1)
        error_handling("accept() error");
    
    if(fp != NULL){
        printf("File exists!\n");
        while(1){
            read_cnt = fread((void*)buf, 1, BUF_SIZE, fp);
            if(read_cnt < BUF_SIZE){ // It's possible that this file is large
                write(clnt_sock, buf, read_cnt);
                break;
            }
            write(clnt_sock, buf, BUF_SIZE); // keep sending data if file is large
        }
    }
    else printf("File don't exist!\n");

    shutdown(clnt_sock, SHUT_WR); // close the write end
    read(clnt_sock, buf, BUF_SIZE); // read msg sent from client
    printf("Message from client: %s\n", buf);

    fclose(fp);
    close(clnt_sock);
    close(serv_sock);
    return 0;
}

void error_handling(char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}