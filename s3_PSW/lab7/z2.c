#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

struct msgbuf {
    long type;
    char messg[20];
};

int main(){
    int key = ftok("/home/msjtw/Documents/uni/s3_PSW/lab7/z1.c", 'a');
    int msqid = msgget(key, 0666 | IPC_CREAT);

    if(!fork()){
        struct msgbuf buff_s = {2, "Hello_1"};
        msgsnd(msqid, &buff_s, sizeof(char) *20, 0);
        struct msgbuf buff_r;
        msgrcv(msqid, &buff_r, sizeof(char) *20, 1, 0);
        printf("process 1: %s\n", buff_r.messg);
        exit(0);
    }
    if(!fork()){
        struct msgbuf buff_s = {1, "Hello_2"};
        msgsnd(msqid, &buff_s, sizeof(char) *20, 0);
        struct msgbuf buff_r;
        msgrcv(msqid, &buff_r, sizeof(char) *20, 2, 0);
        printf("process 2: %s\n", buff_r.messg);
        exit(0);
    }

    int wpid;
    while((wpid = wait(0)) > 0);

    if(msgctl(msqid, IPC_RMID, NULL) == -1){
        perror("msgctl");
        exit(1);
    }

    return 0;
}
