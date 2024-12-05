#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct msgbuf {
    long type;
    char messg[20];
};

int main(){
    int key = ftok("/home/msjtw/Documents/uni/s3_PSW/lab7/z1a.c", 'a');
    int msqid = msgget(key, 0666 | IPC_CREAT);

    struct msgbuf buff = {1, "Hello"};
    int size = sizeof buff.messg;

    msgsnd(msqid, &buff, size, 0);

    if(msgctl(msqid, IPC_RMID, NULL) == -1){
        perror("msgctl");
        exit(1);
    }

    return 0;
}
