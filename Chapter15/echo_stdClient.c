#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024

void error_handling(char* message);

int main(int argc, char* argv[]){

    int sock;
    struct sockaddr_in serv_addr;
    char message[BUF_SIZE];
    int str_len;
    FILE* readfp;
    FILE* writefp;

    if(argc != 3){ 
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)
        error_handling("socket() error");
    
    memset(&serv_addr, 0, sizeof(serv_addr)); // initializing struct sockaddr_in
    serv_addr.sin_family = AF_INET; // IPv4 
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]); 
    serv_addr.sin_port = htons(atoi(argv[2])); // we need to input port number and IP address

    // client connects to server
    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error");
    else 
        printf("Connected......\n");

    readfp = fdopen(sock, "r");
    writefp = fdopen(sock, "w");
    
    while(1){
        fputs("Input message (Q to quit): \n", stdout);
        fgets(message, BUF_SIZE, stdin);
        if(!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
            break;

        fputs(message, writefp); // 将待发送字符串写入输出缓冲
        fflush(writefp); // 清空输出缓冲发送字符串
        fgets(message, BUF_SIZE, readfp);
        printf("Message from server: %s\n", message);
    }
    fclose(writefp);
    fclose(readfp);
    return 0;
}

void error_handling(char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
