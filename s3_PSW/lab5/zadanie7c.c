#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(){
    int pfd1[2];
    pipe(pfd1);

    if(!fork()){
        close(pfd1[0]);
        close(1);
        dup(pfd1[1]);
        execlp("ps", "ps", "-ef", NULL);
    }
    else{
        close(pfd1[1]);
        int pfd2[2];
        pipe(pfd2);
        if(!fork()){
            close(0);
            dup(pfd1[0]);
            close(1);
            dup(pfd2[1]);
            execlp("tr", "tr", "-s", "\' \'", ":", NULL);
        }
        else{
            close(pfd1[1]);
            int pfd3[2];
            pipe(pfd3);
            if(!fork()){
                close(pfd3[0]);
                close(0);
                dup(pfd2[0]);
                close(1);
                dup(pfd3[1]);
                execlp("cut", "cut", "-d:", "-f1", NULL);
            }
            else{
                close(pfd2[0]);
                close(pfd2[1]);
                int pfd4[2];
                pipe(pfd3);
                if(!fork()){
                    close(pfd4[0]);
                    close(0);
                    dup(pfd3[0]);
                    close(1);
                    dup(pfd4[1]);
                    execlp("sort", "sort", NULL);
                }
                else{
                    close(pfd3[0]);
                    close(pfd3[1]);
                    int pfd5[2];
                    pipe(pfd3);
                    if(!fork()){
                        close(pfd5[0]);
                        close(0);
                        dup(pfd4[0]);
                        close(1);
                        dup(pfd5[1]);
                        execlp("uniq", "uniq", "-c", NULL);
                    }
                    else{
                        close(pfd4[0]);
                        close(pfd4[1]);
                        close(0);
                        dup(pfd5[0]);
                        execlp("sort", "sort", "n", NULL);
                    }
                }
            }
        }
    }

    return 0;
}
