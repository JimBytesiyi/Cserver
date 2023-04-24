#include <stdio.h>
#include <unistd.h>
#define BUF_SIZE 30

int main(int argc, char* argv[]){

    int fds[2];
    char str1[] = "Who are you?";
    char str2[] = "Thanks for your message!";
    char buf[BUF_SIZE];
    pid_t pid;

    pipe(fds);
    pid = fork();
    if(pid == 0){
        write(fds[1], str1, sizeof(str1)); // write msg first
        sleep(2);
        read(fds[0], buf, BUF_SIZE); // read msg sent from parent
        printf("Child proc output: %s\n", buf);
    }
    else{
        read(fds[0], buf, BUF_SIZE); // read msg sent from child
        printf("Parent proc output: %s\n", buf);
        write(fds[1], str2, sizeof(str2));
        sleep(3);
    }

    return 0;
}