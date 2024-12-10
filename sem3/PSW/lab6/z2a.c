#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
    /* if(mkfifo("fifo", 0666) < 0) */
    /*     return 0; */

    int fd = open("fifo", O_RDONLY);
    close(0);
    dup(fd);
    execlp("wc", "wc", NULL);
    close(fd);

    unlink("fifo");
    return 0;
}

