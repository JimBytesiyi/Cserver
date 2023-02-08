#include <stdio.h>
#include <arpa/inet.h>
// convert Dotted Decimal Notation into 32-bit integer

int main(int argc, char* argv[]){

    char* addr1 = "1.2.3.4";
    char* addr2 = "1.2.3.256"; // invalid IP address

    unsigned long conv_addr = inet_addr(addr1);
    if(conv_addr == INADDR_NONE)
        printf("error in conversion\n");
    else 
        printf("Network integer address: %#lx\n", conv_addr);

    conv_addr = inet_addr(addr2);
    if(conv_addr == INADDR_NONE)
        printf("error in convertion\n");
    else 
        printf("Network integer address: %#lx\n", conv_addr);

    return 0;
}