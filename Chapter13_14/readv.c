#include <stdio.h>
#include <sys/uio.h>
#define BUF_SIZE 100

int main(int argc, char* argv[]){

    struct iovec vec[2];
    char buf1[BUF_SIZE] = {0,};
    char buf2[BUF_SIZE] = {0,};
    int str_len;

    vec[0].iov_base = buf1;
    vec[0].iov_len = 5; // 只读取长度为5的字符
    vec[1].iov_base = buf2;
    vec[1].iov_len = BUF_SIZE; // 剩余字符保存到vec[1]注册的缓冲区

    str_len = readv(0, vec, 2); // 从stdin(控制台)读取输入
    printf("Write bytes: %d\n", str_len);
    printf("First message: %s\n", buf1);
    printf("Second message: %s\n", buf2);
    
    return 0;
}