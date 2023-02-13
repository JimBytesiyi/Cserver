#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[]){

    int cfd1; int cfd2;
    char str1[] = "Hi~\n";
    char str2[] = "It's a nice day!\n";

    cfd1 = dup(1); // 复制stdout文件描述符
    cfd2 = dup2(cfd1, 7); // 复制cfd1这个文件描述符

    printf("fd1 = %d, fd2 = %d\n", cfd1, cfd2);
    write(cfd1, str1, sizeof(str1)); // 分别向复制的文件描述符中写入字符串
    write(cfd2, str2, sizeof(str2));

    close(cfd1);
    close(cfd2);
    write(1, str1, sizeof(str1));
    close(1); // 终止最后的文件描述符, 值为1(即stdout)
    write(1, str2, sizeof(str2)); // 这一行无法输出

    return 0;
}