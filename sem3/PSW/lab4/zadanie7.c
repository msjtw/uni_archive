#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

int stop_ps = 1;

void hand_chld(){
    alarm(0);
}
void hand_int(){
    stop_ps *= -1;
}

int main(int argc, char *argv[])
{
    signal(SIGCLD, hand_chld);
    signal(SIGINT, hand_int);
    int odd = 1;
    while(1){
        if(odd < 0){
            if(!fork()){
                execlp("ls",NULL);
            }
        }
        else{
            if(!fork()){
                execlp("ps",NULL);
            }
        }
        pause();
        sleep(1);
        if(stop_ps > 0)
            odd *= -1;
        else
            odd = -1;
    }
    return 0;
}
