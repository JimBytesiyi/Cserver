#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void timeout(int sig){
    if(sig == SIGALRM)
        puts("Time out!\n");
    alarm(2);
}

void keycontrol(int sig){
    if(sig == SIGINT)
        puts("Ctrl + c pressed\n");
}

int main(int argc, char* argv[]){

    int i;
    signal(SIGALRM, timeout);
    signal(SIGINT, keycontrol);
    alarm(2);

    for(i = 0; i < 3; i++){
        puts("Wait...\n");
        sleep(100);
    }

    return 0;
}