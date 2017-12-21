

/**
 * Test Application
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>


int main()
{

	pid_t processId = getpid();
	fprintf(stdout, "PID = %d\n", processId);
	fprintf(stdout, "Process will wait for 15 sec.\n");
	fprintf(stdout, "Pass (%d) PID as a module parameter while insertion of the LKM\n", processId);   	
	sleep(15);

	int iterNum = 0;
	while(iterNum < 2) 
	{
		char* array = malloc(1 * 512 * 512);
		if(array == NULL)
		{
			fprintf(stderr, "Malloc Error\n");
			exit(EXIT_FAILURE);
		}

		for(int i = 0; i < 512; ++i) 
		{
			array[i * 512] = 0;    
		}
		iterNum++;
	}

	fprintf(stdout, "Please see the system log for all page faults\n");
	return 0;
}
