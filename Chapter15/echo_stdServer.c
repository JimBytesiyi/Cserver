#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024
void error_handling(char* message);

// 使用standard input/output实现的echo server

int main(int argc, char* argv[]){

    int serv_sock;
    int clnt_sock;
    char message[BUF_SIZE];
    int str_len, i;

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    FILE* readfp;
    FILE* writefp;

    if(argc != 2){ // pass port number as the second parameter
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

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
    for(int i = 0; i < 5; i++){
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
        if(clnt_sock == -1)
            error_handling("accept() error");
        else 
            printf("Connected client %d\n", i+1);
        
        readfp = fdopen(clnt_sock, "r");
        writefp = fdopen(clnt_sock, "w");
        while(!feof(readfp)){
            fgets(message, BUF_SIZE, readfp);
            fputs(message, writefp);
            fflush(writefp);
        }
        fclose(readfp);
        fclose(writefp);
    }
    close(serv_sock);
    return 0;
}

void error_handling(char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
