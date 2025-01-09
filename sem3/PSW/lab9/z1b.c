#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 1

int main(){
    int key;
    int shmid;
    char *data;
    int mode;
    key = 10034;
    shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT);
    data = shmat(shmid, (void *)0 , 0);
    for(;;){
        printf("%s\n", data);
    }
    shmdt(data);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
