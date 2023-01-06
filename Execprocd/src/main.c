#include <time.h>
#include <stdio.h>
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
			Enqueue(highPQ, ++id, 0, name);
		}

		if( (currentProc != NULL) && ( (currentProc->pid = 0) || ( !CheckProc(currentProc->pid) ) ) ){
			if( !CheckProc(currentProc->pid) ){
				wait();
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
			Enqueue(highPQ, currentProc->id, currentProc->pid, currentProc->name);
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