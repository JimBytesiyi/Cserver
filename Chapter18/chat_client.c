#define _REENTRANT 1 // 定义该宏，启用线程安全版本的函数
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#define BUF_SIZE 100
#define NAME_SIZE 20

void* send_msg(void* arg);
void* recv_msg(void* arg);
void error_handling(char* message);

char name[NAME_SIZE] = "[DEFAULT]";
char msg[BUF_SIZE];

int main(int argc, char* argv[]){

    int sock;
    struct sockaddr_in serv_addr;
    pthread_t snd_thread, rcv_thread;
    void* thread_return; // 接受线程结束后返回的信息

    if(argc != 4){ 
        printf("Usage: %s <IP> <port> <Name>\n", argv[0]);
        exit(1);
    }

    sprintf(name, "[%s]", argv[3]);
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)
        error_handling("socket() error");
    
    memset(&serv_addr, 0, sizeof(serv_addr)); 
    serv_addr.sin_family = AF_INET; // IPv4 
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]); 
    serv_addr.sin_port = htons(atoi(argv[2])); 

    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error");
    // else 
        //printf("Connected......\n");

    pthread_create(&snd_thread, NULL, send_msg, (void*)&sock); // 创建接受和发出消息的两个线程
    pthread_create(&rcv_thread, NULL, recv_msg, (void*)&sock);
    pthread_join(snd_thread, &thread_return);
    pthread_join(rcv_thread, &thread_return);
    close(sock);
    return 0;
}

void error_handling(char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

void* send_msg(void* arg){ // send thread main 发送线程主函数
    int sock = *((int*)arg); // 注意指针的转换
    char name_msg[NAME_SIZE + BUF_SIZE]; // 将client名字连同输入信息一起发送

    while(1){
        fgets(msg, BUF_SIZE, stdin);
        if(!strcmp(msg, "q\n") || !strcmp(msg, "Q\n")){
            close(sock);
            exit(0);
        }
        sprintf(name_msg, "%s %s", name, msg); // 将格式化后的字符串放入name_msg数组
        write(sock, name_msg, strlen(name_msg));
    }
    return NULL;
}
void* recv_msg(void* arg){ // read thread main 读取线程主函数
    int sock = *((int*)arg); 
    char name_msg[NAME_SIZE + BUF_SIZE]; 
    int str_len;

    while(1){
        str_len = read(sock, name_msg, NAME_SIZE + BUF_SIZE -1);
        if(str_len == -1)
            return (void*) -1; // 返回一个非零值，表示没有数据
        name_msg[str_len] = 0;
        fputs(name_msg, stdout);
    }
    return NULL;
}