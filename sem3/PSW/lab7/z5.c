#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>

struct msgbuf{
    long type;
    char messg[100];
};


int main(int argc, char *argv[]){
    int key = ftok("/home/msjtw/Documents/uni/s3_PSW/lab7/z4_server.c", 'a');
    int msqid = msgget(key, 0666 | IPC_CREAT);

    if(!fork()){
        while(1){
            struct msgbuf buff;
            scanf("%d", &buff.type);
            scanf("%99s", &buff.messg);
            msgsnd(msqid, &buff, sizeof(buff.messg), 0);
            if(strcmp(buff.messg, "exit") == 0)
                break;
        }
        exit(0);
    }
    if(!fork()){
        while(1){
            struct msgbuf buff;
            msgrcv(msqid, &buff,  sizeof(buff.messg), atoi(argv[1]), 0);
            if(strcmp(buff.messg, "exit") == 0)
                break;
            printf("recieved: %s\n", buff.messg);
        }
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
