#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 30

void error_handling(char* message);

int main(int argc, char* argv[]){

    int sock;
    struct sockaddr_in serv_addr, from_addr;
    char message_from[BUF_SIZE];
    char message_to[BUF_SIZE];
    int str_len;
    socklen_t addr_size;

    if(argc != 3){ 
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(sock == -1)
        error_handling("socket() error");
    
    memset(&serv_addr, 0, sizeof(serv_addr)); // initializing struct sockaddr_in
    serv_addr.sin_family = AF_INET; // IPv4 
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]); 
    serv_addr.sin_port = htons(atoi(argv[2])); // we need to input port number and IP address

    // write and receive bytes from server
    while(1){
        fputs("Input message (Q to quit): \n", stdout);
        fgets(message_to, sizeof(message_to), stdin);
        if(!strcmp(message_to, "q\n") || !strcmp(message_to, "Q\n"))
            break;
        sendto(sock, message_to, strlen(message_to), 0, // send message first
        (struct sockaddr*)&serv_addr, sizeof(serv_addr));
        
        addr_size = sizeof(from_addr);
        str_len = recvfrom(sock, message_from, BUF_SIZE, 0,
        (struct sockaddr*)&from_addr, &addr_size);
        message_from[str_len] = 0; // add an EOF 
        printf("Message from server: %s\n", message_from);
    }
    
    close(sock); 
    return 0;
}

void error_handling(char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}