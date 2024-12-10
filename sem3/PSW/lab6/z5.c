#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main(int arc, char *argv[]){
    if(mkfifo("fifo", 0666) < 0);
    
    int n = atoi(argv[1]);
    int fd = open("fifo", O_WRONLY);
    

    unlink("fifo");
    return 0;
}

