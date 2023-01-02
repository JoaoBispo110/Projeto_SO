#include <stdio.h>
#include "proc_adm.h"

int main()
{
	int pid;
	char command = 0;

	pid = RequestNewProc();

	while(command != 27){
		printf("ESC to terminate program\np to start another program\n");

		scanf("%c", &command);

		if(command == 'p'){
			KillProc(pid);
			pid = RequestNewProc();
		}
	}

	KillProc(pid);

	return 0;
}