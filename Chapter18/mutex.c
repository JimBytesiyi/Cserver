#define _REENTRANT 1 // 定义该宏，启用线程安全版本的函数
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#define NUM_THREAD 100 // 线程个数
void* thread_inc(void* arg);
void* thread_des(void* arg);

long long num = 0; // 保存计算结果的全局变量
pthread_mutex_t mutex; // 全局互斥量变量

int main(int argc, char* argv[]){

    pthread_t thread_id[NUM_THREAD];
    int i;

    pthread_mutex_init(&mutex, NULL); // 初始化互斥锁变量

    for(i = 0; i < NUM_THREAD; i++){
        if(i % 2) // 一半线程用于加运算，另一半线程用于减运算
            pthread_create(&(thread_id[i]), NULL, thread_inc, NULL);
        else
            pthread_create(&(thread_id[i]), NULL, thread_des, NULL);
    }
    for(i = 0; i < NUM_THREAD; i++){
        pthread_join(thread_id[i], NULL); 
    }

    printf("result: %lld\n", num); // 使用互斥锁保护全局变量的读取和写入，结果应该是0
    pthread_mutex_destroy(&mutex); // 释放互斥锁变量
    return 0;
}

void* thread_inc(void* arg){
    int i;
    pthread_mutex_lock(&mutex);
    for(i = 0; i < 500000; i++)
        num += 1; // critical section只是这一行，但我们对整个循环过程都上锁
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void* thread_des(void* arg){
    int i;
    pthread_mutex_lock(&mutex);
    for(i = 0; i < 500000; i++)
        num -= 1; // 分析同上
    pthread_mutex_unlock(&mutex);
    return NULL;
}