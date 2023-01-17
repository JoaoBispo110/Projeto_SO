#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

int main(int argc, char** argv)
{
    int msqid_snd;
    int msgid;


    msgid = atoi(argv[1]); // id do processo a ser cancelado
    msqid_snd = msgget(170067793, 0x124);

    printf("id = %d\n", msgid);
    
    struct Msg { long m_type; int m_content; };
    struct Msg msg;
    
    msg.m_type = 2;
    msg.m_content = msgid;
    
    msgsnd(msqid_snd, &msg, sizeof(int), 0);
    if(errno != 0){
        printf("errno diferent from 0\n");
        printf("errno = %d\n", errno);
    }

    return 0;
}