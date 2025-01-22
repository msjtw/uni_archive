#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/sem.h>

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

void send_request(int, int, int, int);
uint get_key(char *);
int get_queue(int);

int main(int argc, char **argv){
    if(argc != 6){
        fprintf(stderr, "Zła liczba argumentów!\n");
        fprintf(stderr, "./dysp <klucz> <n_zamowien> <max_A> <max_B> <max_C>\n");
        exit(1);
    }

    srand(time(NULL));

    char *key_string = argv[1];
    uint key = get_key(key_string);
    int n_zamowien = strtol(argv[2],NULL,10);
    int max_A = strtol(argv[3],NULL,10);
    int max_B = strtol(argv[4],NULL,10);
    int max_C = strtol(argv[5],NULL,10);
    if(n_zamowien == 0 || max_A == 0 || max_B == 0 || max_C == 0){
        fprintf(stderr, "Parametry muszą być liczbami większymi od 0!");
        exit(1);
    }

    printf("max A: %d, max B: %d, max C: %d\n", max_A, max_B, max_C);

    printf("key: %d\n", key);
    int reqqid = get_queue(key);

    int mag_left = 3;
    while(n_zamowien --> 0){
        struct mag_ret_buff mg_buff;
        while(msgrcv(reqqid, &mg_buff, sizeof(int), 3, IPC_NOWAIT) != -1){
            mag_left--;
            printf("Magazyn został zamknięty. Pozostały %d magazyny.\n", mag_left);
        }
        if(errno != ENOMSG && errno != EAGAIN){
            perror("msgrcv");
            exit(1);
        }
        if(mag_left == 0){
            break;
        }
        usleep(500 * 1000);
        int get_A = rand() % max_A+1;
        int get_B = rand() % max_B+1;
        int get_C = rand() % max_C+1;

        send_request(reqqid, get_A, get_B, get_C);
    }
    while(mag_left > 0){
        struct mag_ret_buff mg_buff;
        msgrcv(reqqid, &mg_buff, sizeof(int), 3, 0);
        mag_left--;
        printf("Magazyn został zamknięty. Pozostały %d magazyny.\n", mag_left);
        if(errno != ENOMSG && errno != EAGAIN){
            perror("msgrcv");
            exit(1);
        }
    }

    int gold = 0;
    int zrealizowano = 0;
    struct mag_ret_buff mg_buff;
    while(msgrcv(reqqid, &mg_buff, sizeof(int), 2, IPC_NOWAIT) != -1){
        gold += mg_buff.val;
        zrealizowano++;
    }
    if(errno != ENOMSG && errno != EAGAIN){
        perror("msgrcv");
        exit(1);
    }

    printf("Zapłacono %d złota.\n", gold);
    printf("Odebrano %d zamówień.\n", zrealizowano);

    if(msgctl(reqqid, IPC_RMID, NULL) == -1){
        perror("msgctl IPC_RMID");
        exit(1);
    }

    return 0;
}

uint get_key(char *pattern){
    int MOD = 1e9+9;
    int P = 9973;
    int MX = 1e9+7;

    uint key = 0;
    int n = strlen(pattern);
    for(int i =0; i < n; i++){
        key += key + (pattern[i]-'a'+1)*pow(P,i);
        key %= MOD;
    }
    return key;
}

void send_request(int qid, int a, int b, int c){
    printf("Złożono zlecenie na A(%d) B(%d) C(%d).\n", a, b, c);

    struct request_buf buff = {1, a, b, c};
    if(msgsnd(qid, &buff, sizeof(struct req), 0) == -1){
        exit(1);
    }
}

int get_queue(int key){
    int reqqid;
    if((reqqid = msgget(key, 0644 | IPC_CREAT)) == -1){
        perror("msgget");
        exit(1);
    }
    return reqqid;
}
