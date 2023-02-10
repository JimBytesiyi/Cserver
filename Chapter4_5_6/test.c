#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void){
    char msg[] = "Hi";
    size_t i = 0;
    printf("%ld\n", sizeof(msg));
    printf("%ld\n", strlen(msg));
    printf("short: %ld\n", sizeof(short));
    printf("long: %ld\n", sizeof(long));
    printf("int: %ld\n", sizeof(int));
    printf("size_t: %ld\n", sizeof(i));
    return 0;
}