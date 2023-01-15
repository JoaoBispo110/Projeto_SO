#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <stdlib.h>

#include "queue.h"

void InitQ(Queue queue){
	queue.head = NULL;
	queue.tail = NULL;
}

void Enqueue(Queue queue, int idfila, int pid,char name[],int ppid,int retpid,int flag,int status){
	Proc* process;

	process = malloc( sizeof(Proc) );
	process->pid = pid;
	process->idfila = idfila;
	process->name = name;
	process->ppid=ppid;
	process->retpid=retpid;
	process->flag=flag;
	process->status=status;
	process->next = NULL;
	//process->prev = NULL;

	ppid=getpid();

	idfila=msgget(IPC_PRIVATE, IPC_CREAT | 0600);	

	struct msqid_ds aux;
	msgctl(idfila, IPC_STAT, &aux);
	
	retpid=fork();
	if(ppid==getpid()){//processo 1
		int mensagem;

		

		for(int i=0; i<10; i++){
			mensagem=i+1;
			//envia 1 mensagem
			msgsnd(idfila, &mensagem, sizeof(int), 0); 
		}

		flag=0;
		//espera o fim do processo 2
		while(flag==0){
			if(retpid==waitpid(retpid,&status,WNOHANG)){
				flag=1;
			}
		}
		//remove fila de mensagens
		msgctl(idfila, IPC_RMID, NULL); 

	}
	//processo 2
	else{
		int msg;

		for(int i=0; i<10; i++){
			msgrcv(idfila, &msg, sizeof(int), 0, 0);
			printf("Número da mensagem recebida pelo processo P2: %d\n", msg);
			sleep(5);
		}
	}
	
	exit(0);
}

Proc* Dequeue(Queue queue){
	Proc* process = queue.head;

	if(queue.head != NULL){
		queue.head = queue.head->next;
		process->next = NULL;
		//queue.head->prev = NULL;
	}

	return process;
}