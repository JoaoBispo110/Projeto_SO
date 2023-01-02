#ifndef PROC_ADM
#define PROC_ADM	

	int RequestNewProc();
	int InitNewProc(char prog[]);
	void KillProc(int pid);
	void SignalProc(int pid, int signal);

#endif