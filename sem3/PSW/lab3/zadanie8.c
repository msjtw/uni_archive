#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    if(!fork()){
        sleep(1);
        printf("koniec");
    }
    return 0;
}
