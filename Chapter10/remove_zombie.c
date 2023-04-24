#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>

void read_childproc(int sig){
    int status;
    pid_t id = waitpid(-1, &status, WNOHANG);
    if(WIFEXITED(status)){ // if child exits normally
        printf("Removed proc id: %d\n", id);
        printf("Child send: %d\n", WEXITSTATUS(status));
    }
}

int main(int argc, char* argv[]){

    pid_t pid;
    struct sigaction act;
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    sigaction(SIGCHLD, &act, 0);

    pid = fork();
    if(pid == 0){ // if child
        puts("Hi, I'm child proc\n");
        sleep(10);
        return 12;
    }
    else{ // if parent
        printf("Child proc id: %d\n", pid);
        pid = fork();
        if(pid == 0){
            puts("Hi, I'm child proc\n");
            sleep(10);
            exit(24);
        }
        else{
            int i;
            printf("Child proc id: %d\n", pid);
            for(i = 0; i < 5; i++){
                puts("Wait......\n");
                sleep(5);
            }
        }
    }

    return 0;
}