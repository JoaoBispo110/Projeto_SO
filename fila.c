#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <stdlib.h>

int main(){

	int ppid;//pid pai
	int retpid;
	int flag;
	int idfila;
	
	int status;

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
			printf("número da mensagem recebida pelo processo p2: %d\n", msg);
			sleep(5);
		}
	}
	
	exit(0);

}