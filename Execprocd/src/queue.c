#include <stdlib.h>
#include "queue.h"

void InitQ(Queue queue){
	queue.head = NULL;
	queue.tail = NULL;
}

void Enqueue(Queue queue, int id, int pid, char name[], int flag,int status,int ppid){
	Proc* process;

	process = malloc( sizeof(Proc) );
	process->pid = pid;
	process->id = id;
	process->name = name;
	process->flag = flag;
	process->ppid = ppid;
	process->status = status;
	process->next = NULL;
	//process->prev = NULL;

	if(queue.tail == NULL){
		queue.head = process;
	}
	else{
		queue.tail->next = process;
		//process->prev = queue.tail;
	}
	queue.tail = process;
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
