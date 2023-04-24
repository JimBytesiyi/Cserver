#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024
#define TIME 3

void error_handling(char* message);

int main(int argc, char* argv[]){

    int sock;
    struct sockaddr_in serv_addr;
    char msg1[] = "Hello server!";
    char msg2[] = "I'm client!";
    char msg3[] = "Nice to meet you too!";
    char* msg_arr[] = {msg1, msg2, msg3};
    char read_buf[BUF_SIZE];

    int i, str_len;

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

    for(i = 0; i < TIME; i++){
        read(sock, (char*)&str_len, 4);
        read(sock, read_buf, str_len); // read msg from server
        puts(read_buf);

        str_len = strlen(msg_arr[i]) + 1;
        write(sock, (char*)&str_len, 4);
        write(sock, msg_arr[i], str_len); // write msg to server
    }

    close(sock); 
    return 0;
}

void error_handling(char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}