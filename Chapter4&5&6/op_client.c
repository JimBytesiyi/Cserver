#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024
#define RESULT_SZ 4
#define OPSZ 4 

void error_handling(char* message);

int main(int argc, char* argv[]){

    int sock;
    struct sockaddr_in serv_addr;
    char message[BUF_SIZE];
    // int str_len = 0;
    // int recv_len, recv_cnt;
    int op_cnt; int result; int i;

    if(argc != 3){ 
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)
        error_handling("socket() error");
    
    memset(&serv_addr, 0, sizeof(serv_addr)); // initializing struct sockaddr_in
    serv_addr.sin_family = AF_INET; // IPv4 
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]); 
    serv_addr.sin_port = htons(atoi(argv[2])); // we need to input port number and IP address

    // client connects to server
    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error");
    else 
        puts("Connected......\n");

    fputs("Operand count: ", stdout);
    scanf("%d", &op_cnt); // read operand count
    message[0] = (char)op_cnt;

    for(i = 0; i < op_cnt; i++){ // read operands
        printf("Operand %d: ", i + 1);
        // fgets(&message[1 + 4 * i], 4, stdin); // failed 
        scanf("%d", (int*)&message[1 + OPSZ * i]);
    } 
    fgetc(stdin);
    fputs("Operator: ", stdout); // read operator
    // fgets(&message[strlen(message)], 1, stdin); 
    scanf("%c", &message[op_cnt * OPSZ + 1]);
    write(sock, message, op_cnt * OPSZ + 2);

    read(sock, &result, RESULT_SZ); // read and print the result
    printf("Operation result: %d\n", result);

    close(sock); 
    return 0;
}

void error_handling(char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}