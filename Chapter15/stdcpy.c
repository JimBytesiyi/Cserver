#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define BUF_SIZE 3

int main(int argc, char* argv[]){

    FILE* fp1;
    FILE* fp2; // FILE结构体指针
    int len;
    char buf[BUF_SIZE];

    fp1 = fopen("news.txt", "r");
    fp2 = fopen("cpy.txt", "w"); // 如果没有cpy.txt，创建一个新文件

    while(fgets(buf, BUF_SIZE, fp1) != NULL)
        fputs(buf, fp2);
    
    fclose(fp1);
    fclose(fp2);
    return 0;
}