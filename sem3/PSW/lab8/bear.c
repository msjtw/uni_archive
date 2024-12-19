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
    while(1){
        if(*honey > 2){
           *honey -= 2; 
            printf("pozostało: %d moidu\n", *honey);
        }
    }
    return 0;
}
