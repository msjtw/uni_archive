#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <errno.h>
#include <time.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>

struct req{
    int a, b ,c;
};

struct request_buf {
    long mtype;
    struct req data;
};

struct mag_ret_buff{
    long mtype;
    int val;
};

uint get_key(char *);
int get_queue(int);
int get_mem(int, int);
int get_sem(int);
void kurier(int, int, int, int *);
void read_conf(char *, char *, int *, int *, int *, int *, int *, int *);

int main(int argc, char **argv){
    if(argc != 3){
        fprintf(stderr, "Zła liczba argumentów.\n");
        fprintf(stderr, "./mag <plik konfiguracyjny> <klucz>\n");
        exit(1);
    }

    char mag_name[50];
    memset(mag_name, '\0', 20);
    int amount_a = 0, amount_b = 0, amount_c = 0;
    int price_a = 0, price_b = 0, price_c = 0;
    int total_gold = 0;

    char *path = argv[1];
    read_conf(path, mag_name, &amount_a, &amount_b, &amount_c, &price_a, &price_b, &price_c);

    char *key_string = argv[2];
    uint key = get_key(key_string);
    printf("key: %d\n", key);

    int reqqid = get_queue(key);

    char *key_res_string;
    key_res_string = malloc(strlen(key_string) + strlen(mag_name));
    key_res_string[0] = '\0';
    strcat(key_res_string, key_string);
    strcat(key_res_string, mag_name);
    int res_key = get_key(key_res_string);
    free(key_res_string);

    int res_shmid = get_mem(res_key, 16);
    int *resources;
    resources = shmat(res_shmid, 0, 0);
    resources[0] = 0;

    int res_semid = get_sem(res_key);

    for(int i = 0; i < 3; i++){
        kurier(i, reqqid, res_semid, resources);
    }

    int kurier_left = 3;
    while(kurier_left > 0){
        struct sembuf sb1 = {.sem_num = 1, .sem_op = -1, .sem_flg = 0};
        struct sembuf sb2 = {.sem_num = 2, .sem_op = 1, .sem_flg = 0};

        semop(res_semid, &sb1, 1);

        if(resources[0] == -1){
            kurier_left--;
            printf("Pozostało %d kurierów.\n", kurier_left);
        }
        else if(resources[0] <= amount_a && resources[1] <= amount_b && resources[2] <= amount_c){
            amount_a -= resources[0];
            amount_b -= resources[1];
            amount_c -= resources[2];

            resources[3] = 0;
            resources[3] += resources[0]*price_a;
            resources[3] += resources[1]*price_b;
            resources[3] += resources[2]*price_c;
            total_gold += resources[3];
            printf("Kurier obługa... pozostało %d %d %d.\n", amount_a, amount_b, amount_c);
        }
        else{
            resources[3] = -1;
            printf("Kurier obługa... brak zasobów.\n");
        }

        semop(res_semid, &sb2, 1);
    }

    struct mag_ret_buff buff = {3, 0};
    msgsnd(reqqid, &buff, sizeof(int), 0);
    printf("Magazyn został zamknięty.\n");
    printf("Pozostało %d %d %d.\n", amount_a, amount_b, amount_c);
    printf("Wysłano zasobów za %d złota.\n", total_gold);

    semctl(res_semid, 0, IPC_RMID);
    shmctl(res_shmid, IPC_RMID, NULL);

    return 0;
}

uint get_key(char *pattern){
    int MOD = 1e9+9;
    int P = 9973;
    int MX = 1e9+7;

    uint key = 0;
    int n = strlen(pattern);
    for(int i = 0; i < n; i++){
        key += key + (pattern[i]-'a'+1)*pow(P,i);
        key %= MOD;
    }
    return key;
}

int get_queue(int key){
    int reqqid;
    if((reqqid = msgget(key, 0644 | IPC_CREAT)) == -1){
        perror("msgget");
        exit(1);
    }
    return reqqid;
}

int get_mem(int key, int ssize){
    int shmid;
    if((shmid = shmget(key, ssize, 0644 | IPC_CREAT)) == -1){
        perror("shmget");
        exit(1);
    }
    return shmid;
}

int get_sem(int key){
    int semid;
    semid = semget(key, 3, 0666 | IPC_CREAT);
    semctl(semid, 0, SETVAL, 1);
    semctl(semid, 1, SETVAL, 0);
    semctl(semid, 2, SETVAL, 0);
    return semid;
}

void read_conf(char *path, char *name, int *aa, int *ab, int *ac, int *pa, int *pb, int *pc){
    int conf_fd = open(path, O_RDONLY);
    int n = 0;
    int ni = 0;
    char buff;
    while(read(conf_fd, &buff, 1)){
        if(buff == '\n'){
            n++;
        }
        else if(n == 0){
            name[ni] = buff;
            ni ++;
            if(ni == 20){
                fprintf(stderr, "Za długa nazwa magazynu! Max 50 znaków.");
                exit(1);
            }
        }
        else if(!isdigit(buff)){
            fprintf(stderr, "Paramater musi być liczbą!");
            exit(1);
        }
        else if(n == 1){
            
            *aa *= 10;
            *aa += buff - '0';
        }
        else if(n == 2){
            *ab *= 10;
            *ab += buff - '0';
        }
        else if(n == 3){
            *ac *= 10;
            *ac += buff - '0';
        }
        else if(n == 4){
            *pa *= 10;
            *pa += buff - '0';
        }
        else if(n == 5){
            *pb *= 10;
            *pb += buff - '0';
        }
        else if(n == 6){
            *pc *= 10;
            *pc += buff - '0';
        }
    }
}

void kurier(int id,int msq_id, int sem_id, int *resources){
    if(!fork()){
        time_t last_req = time(NULL);
        int end = 0;
        while(1){
            if(time(NULL) - last_req > 15){
                printf("\tKurier %d, brak zmównień.\n", id);
                break;
            }
            struct request_buf r_buff;
            int rv;
            if((rv = msgrcv(msq_id, &r_buff, sizeof(struct req), 1, IPC_NOWAIT)) != -1){
                last_req = time(NULL);

                struct sembuf sb0 = {.sem_num = 0, .sem_op = -1, .sem_flg = 0};
                semop(sem_id, &sb0, 1);
                    
                    resources[0] = r_buff.data.a;
                    resources[1] = r_buff.data.b;
                    resources[2] = r_buff.data.c;

                    printf("\tkurier %d pobrał zamówienie na %d %d %d\n", id, resources[0], resources[1], resources[2]);

                    struct sembuf sb1 = {.sem_num = 1, .sem_op = 1, .sem_flg = 0};
                    struct sembuf sb2 = {.sem_num = 2, .sem_op = -1, .sem_flg = 0};
                    semop(sem_id, &sb1, 1);
                    semop(sem_id, &sb2, 1);

                    if(resources[3] == -1){
                        printf("\tKurier %d nie zrealizował zamówienia.\n", id);
                        end = 1;
                    }
                    else{
                        printf("\tKurier %d zrealizował zamówienie, cena: %d\n", id, resources[3]);
                        struct mag_ret_buff buff = {2, resources[3]};
                        msgsnd(msq_id, &buff, sizeof(int), 0);
                    }

                sb0.sem_op = 1;
                semop(sem_id, &sb0, 1);
                if(end){
                    break;
                }
            }
            else if(errno != ENOMSG && errno != EAGAIN){
                perror("msgrcv");
                break;
            }
            usleep(1000);
        }

        struct sembuf sb0 = {.sem_num = 0, .sem_op = -1, .sem_flg = 0};
        semop(sem_id, &sb0, 1);
            printf("\tKurier %d się wyłącza.\n", id);
            
            resources[0] = -1;

            struct sembuf sb1 = {.sem_num = 1, .sem_op = 1, .sem_flg = 0};
            struct sembuf sb2 = {.sem_num = 2, .sem_op = -1, .sem_flg = 0};
            semop(sem_id, &sb1, 1);
            semop(sem_id, &sb2, 1);

        sb0.sem_op = 1;
        semop(sem_id, &sb0, 1);

        exit(0);
    }
}
