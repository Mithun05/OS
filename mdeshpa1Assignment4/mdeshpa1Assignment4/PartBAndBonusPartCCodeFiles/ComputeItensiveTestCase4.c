 /*
 	Test Application
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

#define ROWS 1024

int main()
{
        pid_t processId = getpid();
        fprintf(stdout, "PID = %d\n", processId);
        fprintf(stdout, "Process will wait for 15 sec.\n");
        fprintf(stdout, "Pass (%d) PID as a module parameter while insertion of the LKM\n", processId);
        sleep(15);
	
	int pageSize = (int) getpagesize() / sizeof(int);

	int array[ROWS][pageSize];

	int average = 0;

	/**
	 * Column major access
	 */
	for(int i = 0; i < pageSize; i++)
		for(int j = 0; j < ROWS; j++)
			average += array[j][i];	

	
	fprintf(stdout, "Final Average(Can be anything) =  %d\n", average);
	fprintf(stdout, "Please see the system log for all page faults\n");	

	return 0;
}
