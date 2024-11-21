#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){

    int pfds[2];
    char buff[30];

    pipe(pfds);

    if(!fork()){
        close(pfds[0]);;
        printf("child 1 writig...\n");
        write(pfds[1],"czesc!",7);
        printf("child 1 wrote\n");
        exit(0);
    }
    if(!fork()){
        close(pfds[0]);;
        printf("child 2 writig...\n");
        write(pfds[1],"jest czwartek",13);
        printf("child 2 wrote\n");
        exit(0);
    }
    if(!fork()){
        close(pfds[1]);;
        printf("child 3, waiting to read...\n");
        int ms;
        while((ms = read(pfds[0],buff,30))){
            buff[ms] = '\0';
            printf("child 3 read: %s\n", buff);
        }
        exit(0);
    }
    return 0;
}
