#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/unistd.h>

int main(int argc, char *argv[])
{
    pid_t pid;
    int rv;

    if((pid = fork()) > 0){
        // child
        printf("child: PID %d\n", getpid());
        printf("child: parent PID %d\n", getppid());
        if((pid = fork()) > 0){
            // child
            printf("child: PID %d\n", getpid());
            printf("child: parent PID %d\n", getppid());
        }
        else if(pid == 0){
            //parent
        }
        else{
            printf("error");
        }
    }
    else if(pid == 0){
        //parent
        if((pid = fork()) > 0){
            // child
            printf("child: PID %d\n", getpid());
            printf("child: parent PID %d\n", getppid());
            if((pid = fork()) > 0){
                // child
                printf("child: PID %d\n", getpid());
                printf("child: parent PID %d\n", getppid());
            }
            else if(pid == 0){
                //parent
            }
            else{
                printf("error");
            }
        }
        else if(pid == 0){
            //parent
        }
        else{
            printf("error");
        }
    }
    else{
        printf("error");
    }


    return 0;
}
