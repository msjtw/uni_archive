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

    struct msgbuf buff;

    while(1){
        msgrcv(msqid, &buff, sizeof buff.messg, 1, 0);
        if(strcmp(buff.messg, "exit") == 0)
            break;
        for(int i = 0; i < 20; i++){
            buff.messg[i] = toupper(buff.messg[i]);
        }
        buff.type = 2;
        msgsnd(msqid, &buff, sizeof(buff.messg), 0);
    }

    if(msgctl(msqid, IPC_RMID, NULL) == -1){
        perror("msgctl");
        exit(1);
    }

    return 0;
}
