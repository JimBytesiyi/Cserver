#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]){

    unsigned short host_port = 0x1234; // 2 Bytes short
    unsigned short net_port;
    unsigned long host_addr = 0x12345678; // 4 Bytes long
    unsigned long net_addr;

    net_port = htons(host_port);
    net_addr = htonl(host_addr);

    printf("Host port: %#x\n", host_port); // little endian
    printf("Network port: %#x\n", net_port); // sorted in big endian
    printf("Host port: %#lx\n", host_addr);
    printf("Host port: %#lx\n", net_addr);

    return 0;
}