#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[]){

    pid_t pid = fork();

    if(pid == 0) // if child process
        puts("Hi, I'm a chile process\n");
    else{
        printf("Child proc ID: %d\n", pid);
        sleep(30);
    }

    if(pid == 0)
        puts("End child proc\n");
    else 
        puts("End parent proc\n");

    return 0;
}