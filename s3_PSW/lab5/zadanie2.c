#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){

    int pfds[2];
    char buff[30];

    pipe(pfds);

    if(!fork()){
        printf("child 1 writig...\n");
        write(pfds[1],"czesc!",6);
        printf("child 1 wrote\n");
        exit(0);
    }
    else{
        if(!fork()){
            printf("child 2, waiting to read...\n");
            read(pfds[0],buff,6);
            printf("child 2 read: %s\n", buff);
            exit(0);
        }
        exit(0);
    }
    return 0;
}
