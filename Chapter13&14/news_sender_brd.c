#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024
#define TTL 64 // 最大延时时长
void error_handling(char* message);

int main(int argc, char* argv[]){

    int send_sock;
    struct sockaddr_in board_addr;
    FILE* fp;
    char buf[BUF_SIZE];
    int so_brd = 1;

    if(argc != 3){
        printf("Usage: %s <Boardcast IP> <port>\n", argv[0]);
        exit(1);
    }

    send_sock = socket(PF_INET, SOCK_DGRAM, 0);
    memset(&board_addr, 0, sizeof(board_addr));
    board_addr.sin_family = AF_INET;
    board_addr.sin_addr.s_addr = inet_addr(argv[1]); // Multicast IP
    board_addr.sin_port = htons(atoi(argv[2])); // Multicast port

    // 设置广播
    setsockopt(send_sock, SOL_SOCKET, SO_BROADCAST, (void*)&so_brd, sizeof(so_brd));
    if((fp = fopen("news.txt", "r")) == NULL)
        error_handling("fopen() error");
    
    while(!feof(fp)){ // 多播过程(boardcasting)
        fgets(buf, BUF_SIZE, fp);
        sendto(send_sock, buf, strlen(buf), 0, 
        (struct sockaddr*)&board_addr, sizeof(board_addr));
        sleep(2);
    }
    fclose(fp);
    close(send_sock);
    return 0;
}

void error_handling(char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}