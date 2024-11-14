#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    signal(SIGINT, SIG_IGN);

    while(1);
    return 0;
}
