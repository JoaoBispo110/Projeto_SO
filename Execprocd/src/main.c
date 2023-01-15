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

#include "queue.h"
#include "proc_adm.h"

int main()
{
	int id = 0;
	char* name;
	char command = 0;
	Queue highPQ;
	int	startTime;
	Proc* currentProc;

	InitQ(highPQ);

	while(command != 27){
		printf("ESC to terminate program\np to start another program\n");

		scanf("%c", &command);

		if(command == 'p'){
			name = RequestNewProcName();
			Enqueue(highPQ, ++id, 0, name, 0, 0, 0, 0);
		}

		if( (currentProc != NULL) && ( (currentProc->pid = 0) || ( !CheckProc(currentProc->pid) ) ) ){
			if( !CheckProc(currentProc->pid) ){
				wait(NULL);
			}
			currentProc = Dequeue(highPQ);
			if(currentProc != NULL){
				if(currentProc->pid <= 0){
					currentProc->pid = InitNewProc(currentProc->name);
				}
				else{
					ContProc(currentProc->pid);
				}
				startTime = time(NULL);
			}
		}

		if( (currentProc != NULL) && (difftime(time(NULL), startTime) >= 10) ){
			StopProc(currentProc->pid);
			Enqueue(highPQ, currentProc->id, currentProc->pid, currentProc->name,currentProc->ppid,currentProc->retpid,currentProc->flag,currentProc->status);
			free(currentProc);
			currentProc == NULL;
		}
	}

	if(currentProc != NULL){
		KillProc(currentProc->pid);
		free(currentProc);
	}

	while(( currentProc = Dequeue(highPQ) ) != NULL){
		if(currentProc->pid != 0){
			KillProc(currentProc->pid);
		}
		free(currentProc);
	}

	return 0;
}