#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    int pid1, pid2;
    pid1 = fork();
    if(pid1 == 0){
        int x = 0;
        printf("proces 1\n");
        sleep(1);
        return 0;
    }
    if(!(pid2 = fork())){
        int x = 0;
        printf("proces 2\n");
        sleep(2);
        exit(x);
    }
        int rv = 90;
        waitpid(pid1, &rv, 0);
        printf("%d, %d\n", rv, pid1);
        waitpid(pid2, &rv, 0);
        printf("%d, %d\n", rv, pid2);
        printf("koniec wszytskiego.\n");
    return 0;
}
