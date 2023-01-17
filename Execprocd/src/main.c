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

void PrintHelp(){
	printf("./execprocd [option]\n");
	printf("Options:\n");
	printf("\th :\tPrints this help.\n");
	printf("\te :\tExecutes with scheduler in static mode.\n");
	printf("\td :\tExecutes with scheduler in dinamic mode.\n");
	printf("\tr :\tExecutes with scheduler in random mode.\n");
}

int main(int argc, char const *argv[])
{
	char which_scheduler;
	int id = 0;
	char* name;
	char command = 0;
	int prioridade;
	Queue priorityQ[QUEUE_SIZE];
	int	startTime;
	Proc* currentProc = NULL;
	int m_argc;
	int maior;
	int checkProc;
	struct MsgInt{ long m_type; int m_text; };
	struct MsgInt messageInt;

	if(argc < 2){
		printf("Too few arguments for call\n");
		PrintHelp();
		return 1;
	}
	switch (argv[1][0]){
	case 'h':
	case 'H':
		PrintHelp();
		return 0;
	case 'e':
	case 'E':
	case 'd':
	case 'D':
	case 'r':
	case 'R':
		which_scheduler = argv[1][0];
		break;
	default:
		printf("Invalid Argument\n");
		PrintHelp();
		return 1;
	}

	for(int i = 0; i < QUEUE_SIZE; i++){
		InitQ(&(priorityQ[i]));
	}

	idfila = msgget( 170067793, IPC_CREAT | 0666 );

	struct msqid_ds aux;
	//msgctl(idfila, IPC_STAT, &aux);

	signal(SIGINT, HandleSignal);

	while(!terminate){
		errno = 0;
		msgrcv(idfila, &messageInt, sizeof(int), 1, IPC_NOWAIT);
		if( errno != ENOMSG ){
			m_argc = messageInt.m_text;

			char** m_argv = malloc(m_argc*sizeof(char*));

			msgrcv(idfila, &messageInt, sizeof(int), 1, 0);
			maior = messageInt.m_text;

			struct MsgsChar{ long m_type; char msgs[m_argc*maior]; };
			struct MsgsChar messageChar;
			
			msgrcv(idfila, &messageChar, sizeof(messageChar.msgs), 1, 0);

			prioridade = atoi(&(messageChar.msgs[0])) - 1;
			if(prioridade < 0 || prioridade > (QUEUE_SIZE - 1)){
				prioridade = (QUEUE_SIZE - 1);
			}

			for(int i = 0; i < (m_argc-1); i++){
				m_argv[i] = malloc(strlen(&((messageChar.msgs)[(i+1)*maior])));
				strcpy( m_argv[i], &((messageChar.msgs)[(i+1)*maior]) );
			}

			Enqueue(&(priorityQ[prioridade]), ++id, 0, m_argv, m_argc-1, 0, 0, prioridade, 0);

			for(int i = 0; i < m_argc; i++){
				free(m_argv[i]);
			}
			free(m_argv);

			messageInt.m_type = 10;
			messageInt.m_text = id;
			msgsnd(idfila, &messageInt, sizeof(int), 0);
		}
		if(currentProc != NULL){
			checkProc = CheckProc(currentProc->pid);
		}
		if( (currentProc == NULL) || ( (currentProc->pid == 0) || ( !checkProc ) ) ){
			if( (currentProc != NULL) && ( !checkProc ) ){
				EndProc(&currentProc, true);
			}
			if(currentProc == NULL){
				for(int i = 0; i < QUEUE_SIZE; i++){
					if((currentProc = Dequeue(&(priorityQ[i]))) != NULL){
						break;
					}
				}
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
				prioridade = escalonador(currentProc, which_scheduler);
				Enqueue(&(priorityQ[prioridade]), currentProc->id, currentProc->pid, currentProc->argv, currentProc->argc, currentProc->flag, currentProc->status, prioridade, currentProc->startTime);
				FreeProc(&currentProc);
			}
			else{
				EndProc(&currentProc, true);
			}
		}

		errno = 0;
		msgrcv(idfila, &messageInt, sizeof(int), 2, IPC_NOWAIT);
		if(errno != ENOMSG){
			if(currentProc != NULL && currentProc->id == messageInt.m_text){
				EndRuningProc(&currentProc);
			}
			else{
				int i;
				for(i = 0; i < QUEUE_SIZE; i++){
					if(RemoveProcFromQueue(&(priorityQ[i]), messageInt.m_text) == 0){
						break;
					}
				}
				if(i == QUEUE_SIZE){
					printf("Couldnt find process with id = %d\n", messageInt.m_text);
				}
			}
		}

		errno = 0;
		msgrcv(idfila, &messageInt, 0, 5, IPC_NOWAIT);
		if(errno != ENOMSG){
			break;
		}
	}

	if(idfila > -1){
		msgctl(idfila, IPC_RMID, NULL);
	}

	if(currentProc != NULL){
		EndRuningProc(&currentProc);
	}

	for(int i = 0; i < QUEUE_SIZE; i++){
		while(( currentProc = Dequeue(&(priorityQ[i])) ) != NULL){
			EndRuningProc(&currentProc);
		}
	}

	return 0;
}