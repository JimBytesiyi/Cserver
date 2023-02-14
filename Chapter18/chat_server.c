#define _REENTRANT 1 // 定义该宏，启用线程安全版本的函数
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#define BUF_SIZE 100
#define MAX_CLIENT 256 // 可同时连接的最大client个数
void* handle_clnt(void* arg);
void send_msg(char* msg, int len);
void error_handling(char* message);
static int clnt_cnt = 0;
int clnt_socks[MAX_CLIENT]; // client套接字数组
pthread_mutex_t mutx;

int main(int argc, char* argv[]){

    int serv_sock; int clnt_sock;
    char buf[BUF_SIZE];
    int clnt_addr_size;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    pthread_t t_id; // 线程号

    if(argc != 2){ 
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    pthread_mutex_init(&mutx, NULL); // 初始化互斥锁全局变量
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
        error_handling("socket() error");
    
    memset(&serv_addr, 0, sizeof(serv_addr)); 
    serv_addr.sin_family = AF_INET; // IPv4 
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
    serv_addr.sin_port = htons(atoi(argv[1])); 

    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");

    if(listen(serv_sock, 5) == -1) // 等待队列长度为5
        error_handling("listen() error");
    
    while(1){
        clnt_addr_size = sizeof(clnt_addr);
        // 接受client连接请求
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);

        pthread_mutex_lock(&mutx);
        clnt_socks[clnt_cnt++] = clnt_sock; // 套接字数组中增加新的客户端套接字
        pthread_mutex_unlock(&mutx);

        pthread_create(&t_id, NULL, handle_clnt, (void*)&clnt_sock); // 将新的client套接字作为指针传递给线程处理函数
        pthread_detach(t_id); // 回收该线程
        printf("Connected client IP: %s\n", inet_ntoa(clnt_addr.sin_addr));
    }
    close(serv_sock);
    return 0;
}

void error_handling(char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

void* handle_clnt(void* arg){
    int clnt_sock = *((int*)arg);
    int str_len = 0, i;
    char msg[BUF_SIZE];

    while((str_len = read(clnt_sock, msg, sizeof(msg))) != 0)
        send_msg(msg, str_len); // 如果数组中仍有待发送的数据，则发送
    pthread_mutex_lock(&mutx);
    for(i = 0; i < clnt_cnt; i++){ // remove disconnected client
        if(clnt_sock == clnt_socks[i]){
            while(i++ < (clnt_cnt - 1))
                clnt_socks[i] = clnt_socks[i + 1];
            break;
        }
    }
    clnt_cnt--; // 发送数据一次就断开连接
    pthread_mutex_unlock(&mutx);
    close(clnt_sock);
    return NULL;
}
void send_msg(char* msg, int len){ // send to all
    int i;
    pthread_mutex_lock(&mutx);
    for(i = 0; i < clnt_cnt; i++) // 将消息发送给所有连接上server的客户端
        write(clnt_socks[i], msg, len);
    pthread_mutex_unlock(&mutx);
}