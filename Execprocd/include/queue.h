#ifndef QUEUE
#define QUEUE	

	typedef struct Process{
		int id;
		int pid;
		char* name;
		int ppid;
		int status;
		int flag;
		struct Process* next;
		//Process* prev;
	} Proc;

	typedef struct Queue{
		Proc* head;
		Proc* tail;
	} Queue;

	void InitQ(Queue queue);
	void Enqueue(Queue queue, int id, int pid, char name[],int ppid,int status,int flag);
	Proc* Dequeue(Queue queue);

#endif