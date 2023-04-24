#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024
#define OPSZ 4
void error_handling(char* message);
int calculator(int op_num, int op[], char operator);

int main(int argc, char* argv[]){

    int serv_sock;
    int clnt_sock;
    char message[BUF_SIZE];
    int recv_len, recv_cnt;
    int op_cnt; int result; int i;

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
    
    memset(&serv_addr, 0, sizeof(serv_addr)); // initializing struct sockaddr_in
    serv_addr.sin_family = AF_INET; // IPv4 
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); // use local host address
    serv_addr.sin_port = htons(atoi(argv[1])); // we need to input port number

    // bind the struct sockaddr to server's socket
    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error\n");

    if(listen(serv_sock, 5) == -1) // length of waiting queue is 5
        error_handling("listen() error\n");
    
    clnt_addr_size = sizeof(clnt_addr);

    // read input numbers
    for(i = 0; i < 5; i++){
        op_cnt = 0;
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
        read(clnt_sock, &op_cnt, 1); // read 1 byte first

        // need to wait for client to finish input
        recv_len = 0;
        while(recv_len < (op_cnt * OPSZ + 1)){
            // record the received bytes automaticlly
            recv_cnt = read(clnt_sock, &message[recv_len], BUF_SIZE - 1); 
            recv_len += recv_cnt;
        }
        result = calculator(op_cnt, (int*)message, message[recv_len - 1]);
        write(clnt_sock, (char*)&result, sizeof(result)); // write result to client socket
        close(clnt_sock);
    }

    close(serv_sock);
    return 0;
}

void error_handling(char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int calculator(int op_num, int op[], char operator){
    int result = op[0];
    int i;
    switch (operator)
    {
    case '+':
        for(i = 1; i < op_num; i++)
            result += op[i];
        break;
    case '-':
        for(i = 1; i < op_num; i++)
            result -= op[i];
        break;
    case '*':
        for(i = 1; i < op_num; i++)
            result *= op[i];
        break;
    default:
        printf("invalid\n");
        break;
    }
    return result;
}