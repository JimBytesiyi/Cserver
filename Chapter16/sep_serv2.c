#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024
void error_handling(char* message);

int main(int argc, char* argv[]){

    int serv_sock;
    int clnt_sock;
    FILE* readfp; FILE* writefp;
    char buf[BUF_SIZE] = {0,};

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    if(argc != 2){ // pass port number as the second parameter
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
        error_handling("socket() error");
    
    memset(&serv_addr, 0, sizeof(serv_addr)); 
    serv_addr.sin_family = AF_INET; // IPv4 
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
    serv_addr.sin_port = htons(atoi(argv[1])); 

    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");

    if(listen(serv_sock, 5) == -1) 
        error_handling("listen() error");
    
    clnt_addr_size = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size); // 如果没有connection请求会阻塞
    if(clnt_sock == -1)
        error_handling("accept() error");

    readfp = fdopen(clnt_sock, "r"); // 创建读模式指针
    writefp = fdopen(dup(clnt_sock), "w"); // 创建写模式指针

    fputs("From server: Hi~ Client?\n", writefp);
    fputs("I love all of the world\n", writefp);
    fputs("You are awesome\n", writefp);
    fflush(writefp); // 清空输出缓冲发送数据, 结束发送过程

    shutdown(fileno(writefp), SHUT_WR); // 进入半关闭状态, 会向client发送EOF
    fclose(writefp);

    fgets(buf, sizeof(buf), readfp); 
    fputs(buf, stdout);
    fclose(readfp);
    return 0;
}

void error_handling(char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
