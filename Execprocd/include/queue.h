#ifndef QUEUE
#define QUEUE	

	typedef struct Process{
		int id;
		int pid;
		char** argv;
        int flag;
        int status;
		int startTime;
		struct Process* next;
		//Process* prev;
	} Proc;

	typedef struct Queue{
		Proc* head;
		Proc* tail;
	} Queue;

	void InitQ(Queue* queue);
	void Enqueue(Queue* queue, int id, int pid, char** argv, int flag, int status, int startTime);
	Proc* Dequeue(Queue* queue);

#endif