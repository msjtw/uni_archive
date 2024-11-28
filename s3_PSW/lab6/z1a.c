#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
    if(mkfifo("fifo", 0666) < 0)
        return 0;

    int fd = open("fifo", O_RDONLY);
    char buff[10];
    int rb;
    while((rb = read(fd, buff, 10)) > 0){
        buff[rb] = '\0';
        printf("%s", buff);
    }
    close(fd);

    unlink("fifo");
    return 0;
}
