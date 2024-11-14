#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

int n = 5;
int chld_pid[5];

void sig_handl(int sig_num){
    printf("time's up!\n");
    for(int i = 0; i < n; i++){
        kill(chld_pid[i], SIGINT);
    }
    exit(0);
}

int main(int argc, char *argv[])
{
    signal(SIGCLD, sig_handl);
    for(int i = 0; i < n; i++){
        printf("%d\n", i);
        int c;
        if((c = fork())){
            int time = rand()%10;
            printf("waiting for %ds\n", time);
            sleep(time);
            exit(0);
        }
        else{
            chld_pid[i] = c;
            // setpgid(c, chld_pid[0]);
        }
    }
    while(1);
    return 0;
}
