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
#include <stdbool.h>
#include <time.h>

#include "queue.h"
#include "proc_adm.h"

void InitQ(Queue* queue){
	queue->head = NULL;
	queue->tail = NULL;
}

void Enqueue(Queue* queue, int id, int pid, char** argv, int argc, int contextSwitch, int prioridade, int startTime){
	Proc* process;

	process = malloc( sizeof(Proc) );
	process->id = id;
	process->pid = pid;
	process->argc = argc;
	process->argv = malloc(argc*sizeof(char*));
	for(int i = 0; i < argc; i++){
		process->argv[i] = malloc(strlen(argv[i]));
		strcpy(process->argv[i], argv[i]);
	}
	process->contextSwitch=contextSwitch;
	process->prioridade = prioridade;
	process->next = NULL;
	//process->prev = NULL;
	process->startTime = startTime;

	if(queue->head != NULL){
		queue->tail->next = process;
	}
	else{
		queue->head = process;
	}
	queue->tail = process;
}

Proc* Dequeue(Queue* queue){
	Proc* process = queue->head;

	if(queue->head != NULL){
		queue->head = queue->head->next;
		process->next = NULL;
		//queue->head->prev = NULL;
		if(queue->head == NULL){
			queue->tail = NULL;
		}
	}

	return process;
}

void EndRuningProc(Proc** proc){
	int finished = false;

	if((*proc)->pid != 0){
		if(CheckProc((*proc)->pid)){
			KillProc((*proc)->pid);
			wait(NULL);
		}
		else{
			finished = true;
		}
	}

	EndProc(proc, finished);
}

void EndProc(Proc** proc, int finished){ 
	printf("\nProcess: %d, ended\n", (*proc)->id);
	if((*proc)->startTime != 0){
		printf("turnaround time = %f seconds\n", difftime(time(NULL), (*proc)->startTime));		
		printf("Context Switch = %d\n", (*proc)->contextSwitch);
		if(finished){
			printf("Process finished\n");
		}
		else{
			printf("Process was interrupted\n");
		}
	}else{
		printf("Process was cancelled before running\n");
		printf("Context Switch = 0\n");
	}

	FreeProc(proc);
}

void FreeProc(Proc** proc){
	for(int i = 0; i < (*proc)->argc; i++){
		free((*proc)->argv[i]);
	}
	free((*proc)->argv);
	free((*proc));
	(*proc) = NULL;
}

int RemoveProcFromQueue(Queue* queue, int id){
	if(queue->head == NULL){
		return -1;
	}

	Proc* proc;

	if(queue->head->id == id){
		proc = Dequeue(queue);

		EndRuningProc(&proc);

		return 0;
	}

	Proc* runner;

	for(runner = queue->head; runner->next != NULL; runner = runner->next){
		proc = runner->next;
		if(proc->id == id){
			runner->next = proc->next;
			if(queue->tail == proc){
				queue->tail = runner;
			}
			else{
				proc->next = NULL;
			}

			EndRuningProc(&proc);

			return 0;
		}
	}

	return -1;
}

int sizeofQueue(Queue *queue)
{
	int size = 0;
	Proc* runner = queue->head;
	if (runner == NULL)
	{
		return 0;
	}

	while (runner->next != NULL)
	{
		runner = runner->next;
		size++;
	}
	
	return size;
}

int randomscheduler()// add all queues (possible to generalize)
{
	return (rand() % QUEUE_SIZE);
}

int dinamicscheduler(Proc* proc)
{
	
}

int escalonador(Proc* proc, char t_escalonador)
{
	switch (t_escalonador)
	{
	case 'd':
		//implementação escalonador dinamico
		return 0;
		break;
	case 'e':
		return proc->prioridade; // mantem na mesma fila de prioridade
		break;
	case 'r':
		return randomscheduler();
		break;
	default:
		return -1;
		break;
	}
}