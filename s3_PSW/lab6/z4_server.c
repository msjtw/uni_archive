#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
    if(mkfifo("fifo", 0666) < 0);
        /* return 0; */

    int fd = open("fifo", O_RDONLY);
    int rb;
    char buff[50];
    while((rb = read(fd, buff, 50)) > 0){
        buff[rb] = '\0';
        int fd2 = open(buff, O_RDONLY);
        char messg[50];
        rb = read(fd2, messg, 50);
        messg[rb] = '\0';
        close(fd2);

        fd2 = open(buff, O_WRONLY);
        printf("opend fd: %s", buff);
        if(!fork()){
            close(1);
            dup(fd2);
            execlp(messg, messg, NULL);
        }
        close(fd2);
    }
    close(fd);

    unlink("fifo");
    return 0;
}

