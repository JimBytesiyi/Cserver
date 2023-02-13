#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#define BUF_SIZE 4
#define EPOLL_SIZE 50
void error_handling(char* message);
// 采用epoll方式实现的回声服务器
// epoll默认采用条件触发

int main(int argc, char* argv[]){

    int serv_sock;
    int clnt_sock;
    char buf[BUF_SIZE];
    int str_len, i;

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t addr_size;

    struct epoll_event* ep_events;
    struct epoll_event event;
    int epfd, event_count;

    if(argc != 2){ // pass port number as the second parameter
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
        error_handling("socket() error");
    
    memset(&serv_addr, 0, sizeof(serv_addr)); // initializing struct sockaddr_in
    serv_addr.sin_family = AF_INET; // IPv4 
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); // use local host address
    serv_addr.sin_port = htons(atoi(argv[1])); // we need to input port number

    // bind the struct sockaddr to server's socket
    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");

    if(listen(serv_sock, 5) == -1) // length of waiting queue is 5
        error_handling("listen() error");

    epfd = epoll_create(EPOLL_SIZE);
    ep_events = malloc(sizeof(struct epoll_event) * EPOLL_SIZE);

    event.events = EPOLLIN;
    event.data.fd = serv_sock;
    epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &event); // 注册新事件
    
    while(1){
        event_count = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);
        if(event_count == -1){
            puts("epoll_wait() error");
            break;
        }

        puts("return epoll_wait()");
        for(i = 0; i < event_count; i++){
            if(ep_events[i].data.fd == serv_sock){ // 如果事件描述符与serv_sock相同
                addr_size = sizeof(clnt_addr);
                clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &addr_size);
                event.events = EPOLLIN;
                event.data.fd = clnt_sock;
                epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock, &event); // 注册clnt_sock事件
                printf("Connected client: %d\n", clnt_sock);
            }
            else{
                str_len = read(ep_events[i].data.fd, buf, BUF_SIZE);
                if(str_len == 0){ // 如果没有需要读取的字符串，关闭连接
                    epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);
                    close(ep_events[i].data.fd);
                    printf("Closed client: %d\n", ep_events[i].data.fd);
                }
                else
                    write(ep_events[i].data.fd, buf, str_len); // echo
            }
        }
    }
    close(serv_sock);
    close(epfd);
    return 0;
}

void error_handling(char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}