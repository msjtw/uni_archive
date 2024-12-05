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

    struct msgbuf buff1 = {1, 1};
    msgsnd(msqid, &buff1, sizeof(buff1.num), 0);
    buff1 = (struct msgbuf){2, 1};
    msgsnd(msqid, &buff1, sizeof(buff1.num), 0);
    
    struct msgbuf r_buff;
    msgrcv(msqid, &r_buff, sizeof(r_buff.num), 3, 0);
    printf("%d", r_buff.num);


    return 0;
}
