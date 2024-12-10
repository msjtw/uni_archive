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
    int num;
};

int main(){
    int key = ftok("/home/msjtw/Documents/uni/s3_PSW/lab7/z4_server.c", 'a');
    int msqid = msgget(key, 0666 | IPC_CREAT);

    struct msgbuf buff;

    int sum = 0;

    while(1){
        msgrcv(msqid, &buff, sizeof buff.num, 0, 0);
        sum += buff.num;
        if(buff.type == 2)
            break;
    }

    struct msgbuf s_buff = {3, sum};
    msgsnd(msqid, &s_buff, sizeof(int), 0);

    if(msgctl(msqid, IPC_RMID, NULL) == -1){
        perror("msgctl");
        exit(1);
    }

    return 0;
}
