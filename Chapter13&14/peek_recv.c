#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUF_SIZE 30
void error_handling(char* message);

int main(int argc, char* argv[]){

    int recv_sock, acpt_sock;
    struct sockaddr_in recv_addr, acpt_addr;
    int str_len;
    socklen_t recv_addr_size;
    char buf[BUF_SIZE];

    if(argc != 2){
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    acpt_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&acpt_addr, 0, sizeof(acpt_addr));
    acpt_addr.sin_family = AF_INET;
    acpt_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    acpt_addr.sin_port = htons(atoi(argv[1]));

    if(bind(acpt_sock, (struct sockaddr*)&recv_addr, sizeof(recv_addr)) == -1)
        error_handling("bind() error");
    if((listen(acpt_sock, 5)) == -1)
        error_handling("listen() error");

    recv_addr_size = sizeof(recv_addr);
    recv_sock = accept(acpt_sock, (struct sockaddr*)&recv_addr, &recv_addr_size);

    while(1){
        str_len = recv(recv_sock, buf, sizeof(buf) -1, MSG_PEEK | MSG_DONTWAIT);
        if(str_len > 0)
            break;
    }

    buf[str_len] = 0; // 第一次读取数据, 读取缓冲区数据并未被删除
    printf("Buffering %d bytes: %s\n", str_len, buf);

    str_len = recv(recv_sock, buf, sizeof(buf) -1, 0);
    buf[str_len] = 0;
    printf("Read again: %s\n", buf);

    close(recv_sock);
    close(acpt_sock);
    return 0;
}

void error_handling(char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
