#include <stdio.h>
#include <unistd.h>
#define BUF_SIZE 30

int main(int argc, char* argv[]){

    int i;
    int fds1[2], fds2[2];
    char str_ch1[] = "Who are you?";
    char str_ch2[] = "Hello, I'm child!";
    char str_ch3[] = "Nice to meet you, parent!";
    // char* childMsg[] = {str_ch1, str_ch2, str_ch3};

    char str_pr1[] = "Waiting...";
    char str_pr2[] = "Hello, I'm parent!";
    char str_pr3[] = "Nice to meet you, child!";
    // char* parentMsg[] = {str_pr1, str_pr2, str_pr3};

    char buf[BUF_SIZE];
    pid_t pid;

    pipe(fds1), pipe(fds2);
    pid = fork();
    if(pid == 0){
        write(fds1[1], str_ch1, sizeof(str_ch1));
        read(fds2[0], buf, BUF_SIZE); 
        printf("Child proc output: %s\n", buf);

        write(fds1[1], str_ch2, sizeof(str_ch2));
        read(fds2[0], buf, BUF_SIZE); 
        printf("Child proc output: %s\n", buf);

        write(fds1[1], str_ch3, sizeof(str_ch3));
        read(fds2[0], buf, BUF_SIZE); 
        printf("Child proc output: %s\n", buf);
    }
    else{
        read(fds1[0], buf, BUF_SIZE); 
        printf("Parent proc output: %s\n", buf);
        write(fds2[1], str_pr1, sizeof(str_pr1));
        sleep(3);

        read(fds1[0], buf, BUF_SIZE); 
        printf("Parent proc output: %s\n", buf);
        write(fds2[1], str_pr2, sizeof(str_pr2));
        sleep(3);

        read(fds1[0], buf, BUF_SIZE); 
        printf("Parent proc output: %s\n", buf);
        write(fds2[1], str_pr3, sizeof(str_pr3));
        sleep(3);
    }

    return 0;
}