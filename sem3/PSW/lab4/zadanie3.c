#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

void sig_alarm(int sig_num){
    printf("time's up!");
}

int main(int argc, char *argv[])
{
    signal(SIGALRM, sig_alarm);
    alarm(atoi(argv[1]));
    while(1){
        printf("working...\n");
        sleep(1);
    }
    return 0;
}
