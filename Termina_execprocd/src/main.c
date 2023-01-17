#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

int main()
{
    int idFila;

    idFila = msgget(170067793, 0x124);

    long type = 5;
    
    msgsnd(idFila, &type, 0, 0);
    if(errno != 0){
        printf("errno diferent from 0\n");
        printf("errno = %d\n", errno);
    }

    return 0;
}