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
    struct sockaddr_in mul_addr;
    int timelive = TTL;
    FILE* fp;
    char buf[BUF_SIZE];

    if(argc != 3){
        printf("Usage: %s <GroupIP> <port>\n", argv[0]);
        exit(1);
    }

    send_sock = socket(PF_INET, SOCK_DGRAM, 0);
    memset(&mul_addr, 0, sizeof(mul_addr));
    mul_addr.sin_family = AF_INET;
    mul_addr.sin_addr.s_addr = inet_addr(argv[1]); // Multicast IP
    mul_addr.sin_port = htons(atoi(argv[2])); // Multicast port

    // 设置超时值 
    setsockopt(send_sock, IPPROTO_IP, IP_MULTICAST_TTL, (void*)&timelive, sizeof(timelive));
    if((fp = fopen("news.txt", "r")) == NULL)
        error_handling("fopen() error");
    
    while(!feof(fp)){ // 多播过程(boardcasting)
        fgets(buf, BUF_SIZE, fp);
        sendto(send_sock, buf, strlen(buf), 0, 
        (struct sockaddr*)&mul_addr, sizeof(mul_addr));
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