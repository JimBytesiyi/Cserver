#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 30
void error_handling(char* message);
// UDP host1

int main(int argc, char* argv[]){

    int sock; // note that there's only one socket, different from TCP
    char message[BUF_SIZE];
    int str_len, i;

    struct sockaddr_in my_addr;
    struct sockaddr_in your_addr;
    socklen_t addr_size;

    if(argc != 2){ // pass port number as the second parameter
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_DGRAM, 0); // using UDP
    if(sock == -1)
        error_handling("socket() error");
    
    memset(&my_addr, 0, sizeof(my_addr)); // initializing struct sockaddr_in
    my_addr.sin_family = AF_INET; // IPv4 
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY); // use local host address
    my_addr.sin_port = htons(atoi(argv[1])); // we need to input port number

    // bind the struct sockaddr to server's socket
    if(bind(sock, (struct sockaddr*)&my_addr, sizeof(my_addr)) == -1)
        error_handling("bind() error");
    
    for(i = 0; i < 3; i++){ // run recvfrom() 3 times
        sleep(5); // delay 5 sec.
        addr_size = sizeof(your_addr);
        str_len = recvfrom(sock, message, BUF_SIZE, 0, 
        (struct sockaddr*)&your_addr, &addr_size); // receive msg from another host

        printf("Message %d : %s\n", i + 1, message);
    }
    
    close(sock); 
    return 0;
}

void error_handling(char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}