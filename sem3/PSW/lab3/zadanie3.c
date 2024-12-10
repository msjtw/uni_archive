#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    printf("poczatek\n");
    execlp("ls","-la",NULL);
    printf("koniec\n");
    return 0;
}
