#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    printf("poczatek\n");
    if(!fork()){
        execlp("ls","ls","-la",NULL);
    }
    int rv;
    wait(&rv);
    printf("koniec\n");
    return 0;
}
