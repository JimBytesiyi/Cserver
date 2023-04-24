#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

void* thread_main(void* arg);

int main(int argc, char* argv[]){

    pthread_t t_id;
    int thread_param = 5;
    void* thr_ret; // 线程主函数的返回值将会保存到这个变量

    if(pthread_create(&t_id, NULL, thread_main, (void*)&thread_param) != 0){
        puts("pthread_create() error");
        return -1;
    }
    
    if(pthread_join(t_id, &thr_ret) != 0){
        puts("pthread_join() error");
        return -1;
    }

    printf("Thread return message: %s\n", (char*)thr_ret);
    free(thr_ret); // 释放线程主函数中动态分配的内存空间
    return 0;
}

void* thread_main(void* arg){
    int i;
    int cnt = *((int*)arg);
    char* msg = (char*)malloc(sizeof(char) * 50); // 动态分配50个字节的空间
    strcpy(msg, "Hello~ I'm thread~\n"); // 将该字符串复制进内存空间
    for(i = 0; i < cnt; i++){
        sleep(1);
        puts("Running thread"); 
    }
    return (void*)msg; // 线程运行完毕时将会返回此条信息
}