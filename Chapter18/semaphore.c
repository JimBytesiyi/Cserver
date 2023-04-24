#define _REENTRANT 1 // 定义该宏，启用线程安全版本的函数
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

void* read_num(void* arg);
void* accu(void* arg);
static sem_t sem_one; // 二进制信号量需要两个信号量
static sem_t sem_two;
static int num; // 负责保存每次输入数字的全局变量

int main(int argc, char* argv[]){

    int i;
    pthread_t id_t1, id_t2;
    sem_init(&sem_one, 0, 0); // 创建在同一进程内部使用且初始值为0的信号量
    sem_init(&sem_two, 0, 1); // 创建在同一进程内部使用且初始值为1的信号量

    pthread_create(&id_t1, NULL, read_num, NULL);
    pthread_create(&id_t2, NULL, accu, NULL);

    pthread_join(id_t1, NULL); 
    pthread_join(id_t2, NULL); 

    sem_destroy(&sem_one); // 释放两个信号量
    sem_destroy(&sem_two);
    return 0;
}

void* read_num(void* arg){
    int i;
    for(i = 0; i < 5; i++){
        fputs("Input num: ", stdout);
        sem_wait(&sem_two); // sem_two信号量值-1后变为0，线程阻塞，等待用户输入数字
        scanf("%d", &num);
        sem_post(&sem_one); // sem_one信号量值+1变为1，线程可以跳出阻塞
    }
    return NULL;
}

void* accu(void* arg){
    int sum = 0, i;
    for(i = 0; i < 5; i++){
        sem_wait(&sem_one); // sem_one信号量值-1变为0，线程阻塞，完成计算
        sum += num;
        sem_post(&sem_two); // sem_two信号量值+1变为1, 线程跳出阻塞
    }
    printf("Result: %d\n", sum); // 打印运算结果
    return NULL;
}