#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MSGQSND 0x160000
#define MSGQRCV 0x160001

int main(int argc, char** argv)
{
    int msqid_snd;
    int msgid;


    msgid = atoi(argv[1]); // id do processo a ser cancelado
    msqid_snd = msgget(MSGQSND, IPC_CREAT | 0600 );
    printf("id = %d\n", msgid);
    msgsnd(msqid_snd, &msgid, sizeof(int), 0);

    return 0;
}