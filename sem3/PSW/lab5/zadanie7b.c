#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>

int main(){
    int fds1[2];
    pipe(fds1);
    int fds2[2];
    pipe(fds2);

    if(!fork()){
        close(fds1[0]);
        close(fds2[0]);
        close(fds2[1]);
        close(1);
        dup(fds1[1]);
        execlp("ls", "ls", "-l", NULL);
    }
    else{
        if(!fork()){
            close(fds1[1]);
            close(fds2[0]);
            close(0);
            dup(fds1[0]);
            close(1);
            dup(fds2[1]);
            execlp("grep", "grep", "^d", NULL);
        }
        else{
            close(fds1[0]);
            close(fds1[1]);
            close(fds2[1]);
            close(0);
            dup(fds2[0]);
            execlp("more", "more", NULL);
        }
    }
    return 0;
}
