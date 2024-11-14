#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

void my_sleep(int time){
    alarm(time);
    pause();
}

int main(int argc, char *argv[])
{
    while(1){
        printf("working...\n");
        my_sleep(1);
    }
    return 0;
}
