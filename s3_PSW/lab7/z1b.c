#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <string.h>

struct msgbuf {
    long type;
    char messg[20];
};

int main(){
    int key = ftok("/home/msjtw/Documents/uni/s3_PSW/lab7/z1a.c", 'a');
    int msqid = msgget(key, 0666 | IPC_CREAT);

    struct msgbuf buff;

    msgrcv(msqid, &buff, sizeof(char)*20, 1, 0);

    printf("%s", buff.messg);

    return 0;
}
