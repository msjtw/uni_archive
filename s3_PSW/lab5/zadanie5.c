#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(){
    int fds[2];
    pipe(fds);
    if(!fork()){
        close(fds[0]);
        close(1);
        dup(fds[1]);
        execlp("ls","ls",NULL);
    }
    else{
        close(fds[1]);
        close(0);
        dup(fds[0]);
        char buff[1];
        while(read(fds[0], buff, 1)){
            if(buff[0] >= 'a' && buff[0] <= 'z'){
                buff[0] += 'A' - 'a';
            }
            printf("%s", buff);
        }
    }
}
