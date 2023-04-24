#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define BUF_SIZE 3
// 将文件描述符转换为文件指针
int main(int argc, char* argv[]){

    FILE* fp;
    int fd = open("data.txt", O_WRONLY | O_CREAT | O_TRUNC);

    if(fd == -1){
        fputs("File open error", stdout);
        return -1;
    }
    fp = fdopen(fd, "w");
    fputs("Network C programming\n", fp);
    fclose(fp);
    return 0;
}