#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

// using signal()

void keycontrol(int sig){
    char c;
    if(sig == SIGINT){
        printf("CTRL+C is pressed\n");
        printf("Do you wanna exit? (Y/n)\n");
        scanf("%c", &c);
        if(c == 'Y' || c == 'y')
            exit(0);
    }
}

void out_string(int sig){
    if(sig == SIGALRM)
        puts("Time out!\n");
    alarm(2);
}

int main(int argc, char* argv[]){

    int i;
    signal(SIGALRM, out_string);
    signal(SIGINT, keycontrol);
    alarm(2);
    for(i = 0; i < 5; i++){
        puts("Wait......\n");
        sleep(30);
    }

    return 0;
}