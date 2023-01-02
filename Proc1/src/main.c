#include <time.h>

int main()
{
	int begin_time = time(NULL);

	while( difftime(time(NULL), begin_time) < 30. ){}

	return 0;
}