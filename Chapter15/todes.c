#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define BUF_SIZE 3

int main(int argc, char* argv[]){

    FILE* fp;
    int fd = open("data.txt", O_WRONLY | O_CREAT | O_TRUNC);

    if(fd == -1){
        fputs("File open error", stdout);
        return -1;
    }

    printf("First file descriptor: %d\n", fd);
    fp = fdopen(fd, "w");
    fputs("TCP/IP socket programming\n", fp);
    printf("Second file descriptor: %d\n", fileno(fp)); // 将文件指针再次转换为文件描述符
    fclose(fp);
    return 0;
}