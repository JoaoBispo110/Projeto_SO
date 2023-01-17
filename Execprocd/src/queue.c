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

void InitQ(Queue* queue){
	queue->head = NULL;
	queue->tail = NULL;
}

void Enqueue(Queue* queue, int id, int pid, char** argv, int flag, int status, int startTime){
	Proc* process;

	process = malloc( sizeof(Proc) );
	process->id = id;
	process->pid = pid;
	process->argv = argv;
	process->flag=flag;
	process->status=status;
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