#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(int arc, char *argv[]){
    if(mkfifo(argv[1], 0666) < 0);
        /* return 0; */
    

    int fd = open("fifo", O_WRONLY);
    write(fd, argv[1], sizeof(argv[1]));

    int fd2 = open(argv[1], O_WRONLY);
    int rb;
    char buff[50];
    while((rb = read(fd2, buff, 50)) > 0){
        buff[rb] = '\0';
        printf("%s", buff);
    }
    close(fd);
    close(fd2);

    unlink("fifo");
    return 0;
}

