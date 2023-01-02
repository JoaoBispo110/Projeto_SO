#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "proc_adm.h"

int RequestNewProc(){
	char prog[1000];

	printf("Qual programa executar?\n");

	scanf("%s", prog);

	return InitNewProc(prog);
}

int InitNewProc(char prog[]){
	int pid;

	pid = fork();

	if(pid < 0){
		printf("Couldn't fork!\n");
		exit(-1);
	}
	else if(pid == 0){
		execl(prog, prog, (char *)NULL);
		printf("Execl returned!\n");
		exit(-2);
	}

	return pid;
}

void KillProc(int pid){
	SignalProc(pid, -9);
}

void SignalProc(int pid, int signal){
	kill(pid, signal);
}