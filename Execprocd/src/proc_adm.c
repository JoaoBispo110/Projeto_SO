#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "proc_adm.h"

int InitNewProc(char** argv){
	int pid;

	pid = fork();

	if(pid < 0){
		printf("Couldn't fork!\n");
		exit(-1);
	}
	else if(pid == 0){
		execv(argv[0], argv);
		printf("Execv returned!\n");
		exit(-2);
	}

	return pid;
}

void KillProc(int pid){
	SignalProc(pid, SIGTERM);
}

void StopProc(int pid){
	SignalProc(pid, SIGSTOP);
}

void ContProc(int pid){
	SignalProc(pid, SIGCONT);
}

int CheckProc(int pid){
	return (SignalProc(pid, 0) == 0);
}

int SignalProc(int pid, int signal){
	return kill(pid, signal);
}