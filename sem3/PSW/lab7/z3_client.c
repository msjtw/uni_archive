#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>

struct msgbuf {
    long type;
    char messg[20];
};

int main(){
    int key = ftok("/home/msjtw/Documents/uni/s3_PSW/lab7/z3_server.c", 'a');
    int msqid = msgget(key, 0666 | IPC_CREAT);

    struct msgbuf buff1 = {1, "hello"};
    msgsnd(msqid, &buff1, sizeof(buff1.messg), 0);
    struct msgbuf r_buff;
    msgrcv(msqid, &r_buff, sizeof(r_buff.messg), 2, 0);
    printf("%s", r_buff.messg);

    struct msgbuf buff2 = {1, "exit"};
    msgsnd(msqid, &buff2, sizeof(buff2.messg), 0);


    return 0;
}
