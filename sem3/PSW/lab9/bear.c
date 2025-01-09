#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define SHM_SIZE 1024
#define MAX_RETRIES 10
#define NEED_SEMUN

#ifdef NEED_SEMUN
/* Defined in sys/sem.h as required by POSIX now */
union semun {
	int val;
	struct semid_ds *buf;
	ushort *array;
};
#endif

int initsem(key_t key, int nsems)  /* key from ftok() */
{
	int i;
	union semun arg;
	struct semid_ds buf;
	struct sembuf sb = { 0 };  /* best to always init structs */
	int semid;

	semid = semget(key, nsems, IPC_CREAT | IPC_EXCL | 0666);

	if (semid >= 0) { /* we got it first */
		sb.sem_op = 1; sb.sem_flg = 0;
		arg.val = 1;

		printf("press return\n"); getchar();

		for(sb.sem_num = 0; sb.sem_num < nsems; sb.sem_num++) { 
			/* do a semop() to "free" the semaphores. */
			/* this sets the sem_otime field, as needed below. */
			if (semop(semid, &sb, 1) == -1) {
				int e = errno;
				semctl(semid, 0, IPC_RMID); /* clean up */
				errno = e;
				return -1; /* error, check errno */
			}
		}
	} else if (errno == EEXIST) { /* someone else got it first */
		int ready = 0;

		semid = semget(key, nsems, 0); /* get the id */
		if (semid < 0) return semid; /* error, check errno */

		/* wait for other process to initialize the semaphore: */
		arg.buf = &buf;
		for(i = 0; i < MAX_RETRIES && !ready; i++) {
			semctl(semid, nsems-1, IPC_STAT, arg);
			if (arg.buf->sem_otime != 0) {
				ready = 1;
			} else {
				sleep(1);
			}
		}
		if (!ready) {
			errno = ETIME;
			return -1;
		}
	} else {
		return semid; /* error, check errno */
	}

	return semid;
}

int main(){
    int key;
    int shmid;
    int *honey;
    int mode;
    key = 10034;
    shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT);
    honey = shmat(shmid, (void *)0 , 0);

    int semid;
    struct sembuf sb;
    
    sb.sem_num = 0;
    sb.sem_op = -1;  /* set to allocate resource */
    sb.sem_flg = SEM_UNDO;

    if ((semid = initsem(key, 1)) == -1) {
        perror("initsem");
        exit(1);
    }

    while(1){

        sb.sem_op = -1;  /* set to allocate resource */
		if (semop(semid, &sb, 1) == -1) {
		    perror("semop");
			exit(1);
		}

        if(*honey > 2){
           *honey -= 2; 
            printf("pozostało: %d moidu\n", *honey);
        }

        sb.sem_op = 1;  /* set to allocate resource */
		if (semop(semid, &sb, 1) == -1) {
		    perror("semop");
			exit(1);
		}
    }
    return 0;
}
