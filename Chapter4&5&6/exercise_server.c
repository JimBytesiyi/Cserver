#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 30
void error_handling(char* message);
// UDP 'server'

int main(int argc, char* argv[]){

    int serv_sock; // note that there's only one socket, different from TCP
    char message_from[BUF_SIZE];
    char message_to[BUF_SIZE];
    int str_len;

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    if(argc != 2){ // pass port number as the second parameter
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_DGRAM, 0); // using UDP
    if(serv_sock == -1)
        error_handling("socket() error");
    
    memset(&serv_addr, 0, sizeof(serv_addr)); // initializing struct sockaddr_in
    serv_addr.sin_family = AF_INET; // IPv4 
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); // use local host address
    serv_addr.sin_port = htons(atoi(argv[1])); // we need to input port number

    // bind the struct sockaddr to server's socket
    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");
    
    while(1){ // a infinite while loop
        clnt_addr_size = sizeof(clnt_addr);
        str_len = recvfrom(serv_sock, message_from, BUF_SIZE, 0, 
        (struct sockaddr*)&clnt_addr, &clnt_addr_size);
        message_from[str_len] = 0;
        printf("Message from client: %s", message_from);

        fputs("Input message (Q to quit): \n", stdout);
        fgets(message_to, sizeof(message_to), stdin);
        if(!strcmp(message_to, "q\n") || !strcmp(message_to, "Q\n"))
            break;
        sendto(serv_sock, message_to, strlen(message_to), 0, 
        (struct sockaddr*)&clnt_addr, clnt_addr_size); 
    }
    
    close(serv_sock); // close() will not be called.
    return 0;
}

void error_handling(char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}