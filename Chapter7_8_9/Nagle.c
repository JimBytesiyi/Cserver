#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/tcp.h>

void error_handling(char* message);

int main(int argc, char* argv[]){

    int sock;
    int state;
    int opt_val;
    socklen_t opt_len;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    opt_len = sizeof(opt_val);
    state = getsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (void*)&opt_val, &opt_len);
    if(state)
        error_handling("getsockopt() error");

    if(!opt_val)
        printf("Nagle algorithm is on\n");
    else printf("Nagle algorithm is off\n");

    return 0;
}

void error_handling(char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}