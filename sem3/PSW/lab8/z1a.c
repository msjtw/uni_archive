#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 1024

int main(){
    int key;
    int shmid;
    char *data;
    int mode;
    key = 10034;
    shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT);
    data = shmat(shmid, (void *)0 , 0);
    int i = 1;
    for(;;){
        if(i > 0){
            strcpy(data, "haaaa");
        }
        else{
            strcpy(data, "hoooo");
        }
        i *= -1;
    }
    shmdt(data);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
