#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define WAIT_TIME 300000

void error_handling(char* message);

int main(int argc, char* argv[]){

    int sock;
    struct sockaddr_in serv_addr;
    char message[30];
    int str_len = 0;
    int idx = 0, read_len = 0;

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

    // busy waiting until server finish writing 
    for(int i = 0; i < WAIT_TIME; i++){
        if(i == WAIT_TIME - 1)
            printf("Wait time: %d\n", i);
    }

    // read bytes from server(for tcp_server)
    str_len = read(sock, message, sizeof(message) - 1);
    if(str_len == -1)
        error_handling("listen() error");

    // read 1 bit each time 
    /*while(read_len = read(sock, &message[idx++], 1)){
        if(read_len == -1)
            error_handling("read() error");
        str_len += read_len;
    }*/
    
    printf("Message from server: %s\n", message);
    printf("Function read call count: %d\n", str_len); // the call count in my mechine is 17, don't know why
    close(sock); // don't forget to close the socket after reading

    return 0;
}

void error_handling(char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}