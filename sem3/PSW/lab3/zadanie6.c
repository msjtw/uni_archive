#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    fork();
    fork();
    fork();

    sleep(10);

    int rv;
    wait(&rv);
    wait(&rv);
    wait(&rv);

    return 0;
}
