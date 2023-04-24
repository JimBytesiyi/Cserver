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
    char file_buf[BUF_SIZE];
    char file_name[BUF_SIZE];
    int file_len;
    FILE* fp; 

    if(argc != 3){ 
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    printf("Input file name: ");
    scanf("%s", file_name); 
    // fp = fopen(file_name, "w"); // create a new file 

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

    // send file name to server
    write(sock, file_name, strlen(file_name) + 1);

    fp = fopen("output.txt", "w");
    while((file_len = read(sock, file_buf, BUF_SIZE)) != 0)
        fwrite((void*)file_buf, 1, file_len, fp);

    fclose(fp);
    close(sock); 
    return 0;
}

void error_handling(char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}