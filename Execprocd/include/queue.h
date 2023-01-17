#ifndef QUEUE
#define QUEUE	

	typedef struct Process{
		int id;
		int pid;
		char** argv;
		int argc;
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
	void Enqueue(Queue* queue, int id, int pid, char** argv, int argc, int flag, int status, int startTime);
	Proc* Dequeue(Queue* queue);
	void EndRuningProc(Proc** proc);
	void EndProc(Proc** proc, int finished);
	void FreeProc(Proc** proc);
	int RemoveProcFromQueue(Queue* queue, int id);


#endif