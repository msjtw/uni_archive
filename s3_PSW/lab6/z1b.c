#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
    int fd = open("fifo", O_WRONLY);
    char buff[] = "yo. mr white";
    write(fd, buff, sizeof(buff));
    close(fd);
    return 0;
}
