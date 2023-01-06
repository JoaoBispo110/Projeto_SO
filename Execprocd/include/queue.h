#ifndef QUEUE
#define QUEUE	

	typedef struct Process{
		int id;
		int pid;
		char* name;
		struct Process* next;
		//Process* prev;
	} Proc;

	typedef struct Queue{
		Proc* head;
		Proc* tail;
	} Queue;

	void InitQ(Queue queue);
	void Enqueue(Queue queue, int id, int pid, char name[]);
	Proc* Dequeue(Queue queue);

#endif