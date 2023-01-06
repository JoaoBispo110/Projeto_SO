#ifndef PROC_ADM
#define PROC_ADM	

	char* RequestNewProcName();
	int InitNewProc(char prog[]);
	void KillProc(int pid);
	void StopProc(int pid);
	void ContProc(int pid);
	int CheckProc(int pid);
	int SignalProc(int pid, int signal);

#endif