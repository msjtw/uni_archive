#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){

    int pfds[2];
    char buff[30];

    pipe(pfds);

    if(!fork()){
        printf("child writig...\n");
        write(pfds[1],"czesc!",6);
        printf("child wrote\n");
        exit(0);
    }
    else{
        printf("parent, waiting to read...\n");
        read(pfds[0],buff,6);
        printf("parent read: %s\n", buff);
        exit(0);
    }
    return 0;
}
