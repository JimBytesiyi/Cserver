#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
// convert Dotted Decimal Notation into 32-bit integer IP address
// we don't need to save the converted result
void error_handling(char* message);

int main(int argc, char* argv[]){

    char* addr = "127.0.0.23";
    struct sockaddr_in addr_inet;

    if(!inet_aton(addr, &addr_inet.sin_addr))
        error_handling("error in conversion\n");
    else 
        printf("Network integer addr: %#x\n", addr_inet.sin_addr.s_addr);

    return 0;
}

void error_handling(char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}