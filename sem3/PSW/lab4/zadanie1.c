#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void sig_hand(int sig_num){
    printf("signal: %d\n", sig_num);
}

void sig_int(int sig_num){
    printf("signal: %d", sig_num);
    exit(0);
}

int main(int argc, char *argv[])
{
    for(int i = 1; i <= 20; i++){
        signal(i,sig_hand);
    }
    signal(SIGINT, sig_int);

    while(1);
    return 0;
}
