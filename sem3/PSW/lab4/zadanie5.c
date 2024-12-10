#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

void sig_handl(int sig_num){
    printf("time's up!");
}

int main(int argc, char *argv[])
{
    signal(SIGCLD, SIG_IGN);
    if(fork()){
        //child
        printf("CHILD\n");
        exit(0);
    }
    signal(SIGCLD, sig_handl);
    if(fork()){
        //child
        printf("CHILD\n");
        exit(0);
    }
    return 0;
}
