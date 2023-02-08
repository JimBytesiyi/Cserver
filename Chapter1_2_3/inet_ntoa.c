#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
// convert 32-bit integer IP address into string

int main(int argc, char* argv[]){

    struct sockaddr_in addr1, addr2;
    char* str_ptr;
    char str_arr[20];

    addr1.sin_addr.s_addr = htonl(0x10203041);
    addr2.sin_addr.s_addr = htonl(0x10101010);

    str_ptr = inet_ntoa(addr1.sin_addr);
    strcpy(str_arr, str_ptr);
    printf("dotted decimal notation: %s\n", str_ptr);

    inet_ntoa(addr2.sin_addr);
    printf("dotted decimal notation: %s\n", str_ptr);
    printf("dotted decimal notation: %s\n", str_arr);

    return 0;
}