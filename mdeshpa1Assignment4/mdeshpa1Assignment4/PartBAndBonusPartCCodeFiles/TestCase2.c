
/**
 * Test Case Application
 */


/**
 * Standard Header files
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>

// Buffer size
#define BUFFER_SIZE 50


// main driver entry
int main()
{

	pid_t processId = getpid();
	fprintf(stdout, "PID = %d\n", processId);
	fprintf(stdout, "Process will wait for 15 sec.\n");
	fprintf(stdout, "Pass (%d) PID as a module parameter while insertion of the LKM\n", processId);

	sleep(15);

	double* array = (double*) malloc(sizeof(double) * 5467837);

	if(array == NULL)
	{
		fprintf(stderr, "Malloc Error\n");
		exit(EXIT_FAILURE);
	}

	srand(0);

	double sum = 0.0;
	int iterNum = 1;

	while(iterNum < 10000)
	{
		int index = rand() % BUFFER_SIZE;
		sum += array[index];
		iterNum++;
	}

	fprintf(stdout, "\nPlease see the System Log for all page faults\n");
	return 0;
}	

// end of main	
