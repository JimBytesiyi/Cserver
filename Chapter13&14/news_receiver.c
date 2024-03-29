#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024
void error_handling(char* message);

int main(int argc, char* argv[]){

    int recv_sock;
    int str_len;
    char buf[BUF_SIZE];
    struct sockaddr_in addr;
    struct ip_mreq join_addr;

    if(argc != 3){
        printf("Usage: %s <GroupIP> <port>\n", argv[0]);
        exit(1);
    }

    recv_sock = socket(PF_INET, SOCK_DGRAM, 0);
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(atoi(argv[2]));

    if(bind(recv_sock, (struct sockaddr*)&addr, sizeof(addr)) == -1)
        error_handling("bind() error");

    // 指定多播组
    join_addr.imr_multiaddr.s_addr = inet_addr(argv[1]);
    join_addr.imr_interface.s_addr = htonl(INADDR_ANY);

    // 加入多播组
    setsockopt(recv_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void*)&join_addr, sizeof(join_addr));

    while(1){
        str_len = recvfrom(recv_sock, buf, BUF_SIZE -1, 0, NULL, 0); // 调用recvfrom()接收数据
        if(str_len < 0) // 最后一句话没有收到, 为什么?
            break;
        buf[str_len] = 0;
        fputs(buf, stdout); // 接收到的数据输出到控制台
    }
    close(recv_sock);
    return 0;
}

void error_handling(char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}