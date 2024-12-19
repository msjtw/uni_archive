#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 1024

int main(){
    int key;
    int shmid;
    int *honey;
    int mode;
    key = 10034;
    shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT);
    honey = shmat(shmid, (void *)0 , 0);
    for(int i = 0; i < 2; i++){
        if(!fork()){
            for(int k = 0; k < 5; k++){
                printf("bee %d, %d\n", i, *honey);
                *honey += 1;
                sleep(1);
            }
            exit(0);
        }
    }
    return 0;
}
