
/** 
 * The source file contains the user helper code for the LKM
 * in which it is able to retrieve the time and address at which it 
 * generates fault
 */

/** 
 * Standard Header files
 */
#include <stdio.h>		// std - IO header file
#include <stdlib.h>		// std - Lib header file
#include <errno.h>		// std - Error handling
#include <fcntl.h>		// std - header file
#include <unistd.h>		// std - header file
#include <string.h>		// std - header file

// custom header file
#include "ConfigDetailsUser.h"

/**
 *  global error handling variable declaration	 
 */
extern int errno;

// main entry function
int main(int argc, char *argv[])
{
	
	int  fd; 					// file descriptor 
	int nBytes = 0; 				// return value of read function 
	int errorCode;					// error code 
	ConfigData configDetails[BUFFER_CAPACITY];	// initialize buffer capacity

	fd = open(PROC_SPEC_FILE, O_RDONLY);		// open proc device file
	if(fd == -1)					// error handling
	{
		errorCode = errno;
		fprintf(stderr, "Error : %s\n", strerror(errorCode));
		fprintf(stdout, "ConfigDetailsUserModule.c : File does not exist or locked by another process\n");
		exit(EXIT_FAILURE);			// exit failure
	}

	fprintf(stdout, "Counter\tTime\t\t\t\tVirtual Address\n");
	for(int counter = 0; counter < BUFFER_CAPACITY; counter++)
	{
		nBytes = read(fd, &configDetails[counter], sizeof(configDetails[counter]));    // read from proc device file

		if(nBytes == 0)
		{
			fprintf(stdout, "%d\t%ld\t\t\t0x%lx\n", counter, configDetails[counter].disTime, 
									configDetails[counter].virAddress);
		}
	}

	close(fd);
	return 0;
}

// end of main function
