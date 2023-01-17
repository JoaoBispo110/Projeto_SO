#ifndef PROC_ADM
#define PROC_ADM	

	int InitNewProc(char** argv);
	void KillProc(int pid);
	void StopProc(int pid);
	void ContProc(int pid);
	int CheckProc(int pid);
	int SignalProc(int pid, int signal);

#endif