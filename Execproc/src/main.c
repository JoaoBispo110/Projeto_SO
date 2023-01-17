#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

int main(int argc, char const *argv[])
{
	int i;
	int idFila;
	
	if(argc < 3){
		printf("Não tem argumentos suficientes para chamar funcao\n");
	}
	else{
		int maior = 0;
		for(i = 1; i < argc; i++){

			if(sizeof(argv[i]) > maior){
				maior = sizeof(argv[i]);
			}
		}

		struct MsgChar{ long mtype; char mcontent[maior*(argc - 1)]; };
		struct MsgChar messageChar;
		messageChar.mtype = 1;
		for(i = 0; i < (argc - 1); i++){
			strcpy(&(messageChar.mcontent[i*maior]), argv[i+1]);
		}

		idFila = msgget( 170067793, 0x124 );

		int amount = argc - 1;

		struct MsgInt{ long mtype; int mcontent; };
		struct MsgInt messageInt;
		messageInt.mtype = 1;
		messageInt.mcontent = amount;

		msgsnd(idFila, &messageInt, sizeof(int), 0);
		if(errno != 0){
			printf("errno diferent from 0\n");
			printf("errno = %d\n", errno);
		}

		messageInt.mcontent = maior;
		msgsnd(idFila, &messageInt, sizeof(int), 0);
		if(errno != 0){
			printf("errno diferent from 0\n");
			printf("errno = %d\n", errno);
		}

		msgsnd(idFila, &messageChar, sizeof(messageChar.mcontent), 0);
		if(errno != 0){
			printf("errno diferent from 0\n");
			printf("errno = %d\n", errno);
		}
	}

	return 0;
}