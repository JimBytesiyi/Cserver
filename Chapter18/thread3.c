#define _REENTRANT 1
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
// 最后运行结果虽然正确，但存在线程安全问题
void* thread_sum(void* arg);
int sum = 0; // 保存计算结果的全局变量

int main(int argc, char* argv[]){

    pthread_t id_t1, id_t2;
    int range1[] = {1, 5};
    int range2[] = {6, 10};

    pthread_create(&id_t1, NULL, thread_sum, (void*)range1);
    pthread_create(&id_t2, NULL, thread_sum, (void*)range2);

    pthread_join(id_t1, NULL);
    pthread_join(id_t2, NULL);
    printf("result: %d\n", sum); // 2个线程直接访问全局变量

    return 0;
}

void* thread_sum(void* arg){
    int start = ((int*)arg)[0]; // 将void*转换为int*
    int end = ((int*)arg)[1];

    while(start <= end){
        sum += start;
        start++;
    }
    return NULL;
}