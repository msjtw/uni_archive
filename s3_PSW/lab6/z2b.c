#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
    int fd = open("fifo", O_WRONLY);
    close(1);
    dup(fd);
    execlp("ls", "ls", "-l", NULL);
    close(fd);
    return 0;
}
