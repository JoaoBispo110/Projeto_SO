#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <stdlib.h>
#include <stdbool.h>

#include "queue.h"
#include "proc_adm.h"

int terminate = false;
int idfila = -1;

void HandleSignal(int signal){
	if(signal == SIGINT){
		terminate = true;
		return;
	}
}

int main()
{
	int id = 0;
	char* name;
	char command = 0;
	Queue highPQ;
	int	startTime;
	Proc* currentProc = NULL;
	int argc;
	int maior;
	struct MsgInt{ long m_type; int m_text; };
	struct MsgInt messageInt;

	InitQ(&highPQ);

	idfila = msgget( 170067793, IPC_CREAT | 0666 );

	struct msqid_ds aux;
	//msgctl(idfila, IPC_STAT, &aux);

	signal(SIGINT, HandleSignal);

	while(!terminate){
		errno = 0;
		msgrcv(idfila, &messageInt, sizeof(int), 1, IPC_NOWAIT);
		if( errno != ENOMSG ){
			argc = messageInt.m_text;

			char** argv = malloc(argc*sizeof(char*));

			msgrcv(idfila, &messageInt, sizeof(int), 1, 0);
			maior = messageInt.m_text;

			struct MsgsChar{ long m_type; char msgs[argc*maior]; };
			struct MsgsChar messageChar;
			
			msgrcv(idfila, &messageChar, sizeof(messageChar.msgs), 1, 0);

			for(int i = 0; i < argc; i++){
				argv[i] = malloc(strlen(&((messageChar.msgs)[i*maior])));
				strcpy( argv[i], &((messageChar.msgs)[i*maior]) );
			}

			Enqueue(&highPQ, ++id, 0, argv, argc, 0, 0, 0);

			for(int i = 0; i < argc; i++){
				free(argv[i]);
			}
			free(argv);

			messageInt.m_type = 10;
			messageInt.m_text = id;
			msgsnd(idfila, &messageInt, sizeof(int), 0);
		}

		if( (currentProc == NULL) || ( (currentProc->pid == 0) || ( !CheckProc(currentProc->pid) ) ) ){
			if( (currentProc != NULL) && ( !CheckProc(currentProc->pid) ) ){
				wait(NULL);
				FreeProc(&currentProc);
			}
			if(currentProc == NULL){
				currentProc = Dequeue(&highPQ);
			}
			if(currentProc != NULL){
				if(currentProc->pid <= 0){
					currentProc->pid = InitNewProc(currentProc->argv);
					currentProc->startTime = time(NULL);
				}
				else{
					ContProc(currentProc->pid);
				}
				startTime = time(NULL);
			}
		}

		if( (currentProc != NULL) && (difftime(time(NULL), startTime) >= 10) ){
			if(CheckProc(currentProc->pid)){
				StopProc(currentProc->pid);
			}
			else{
				wait(NULL);
			}
			Enqueue(&highPQ, currentProc->id, currentProc->pid, currentProc->argv, currentProc->argc, currentProc->flag, currentProc->status, currentProc->startTime);
			FreeProc(&currentProc);
		}

		errno = 0;
		msgrcv(idfila, &messageInt, sizeof(int), 2, IPC_NOWAIT);
		if(errno != ENOMSG){
			if(currentProc != NULL && currentProc->id == messageInt.m_text){
				EndRuningProc(&currentProc);
			}
			else{
				if(RemoveProcFromQueue(&highPQ, messageInt.m_text) != 0){
					printf("Couldnt find process with id = %d\n", messageInt.m_text);
				}
			}
		}
	}

	if(idfila > -1){
		msgctl(idfila, IPC_RMID, NULL);
	}

	if(currentProc != NULL){
		EndRuningProc(&currentProc);
	}

	while(( currentProc = Dequeue(&highPQ) ) != NULL){
		EndRuningProc(&currentProc);
	}

	return 0;
}