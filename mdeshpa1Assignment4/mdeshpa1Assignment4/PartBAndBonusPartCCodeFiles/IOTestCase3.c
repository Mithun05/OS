/**
 * Test Application
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

// constants
#define INPUT_FILE_NAME "fileio/file_io_operations.txt"
#define OUTPUT_FILE_NAME1 "fileio/file_output_io_operations1.txt"
#define OUTPUT_FILE_NAME2 "fileio/file_output_io_operations2.txt"
#define OUTPUT_FILE_NAME3 "fileio/file_output_io_operations3.txt"
#define OUTPUT_FILE_NAME4 "fileio/file_output_io_operations4.txt"
#define BUFFER_SIZE 1024

size_t nRead;
char buf[BUFFER_SIZE];

int main()
{

        pid_t processId = getpid();
        fprintf(stdout, "PID = %d\n", processId);
        fprintf(stdout, "Process will wait for 15 sec.\n");
        fprintf(stdout, "Pass (%d) PID as a module parameter while insertion of the LKM\n", processId);
	
        sleep(15);

	FILE *filp1 = NULL, *filp2 = NULL, *filp3 = NULL, *filp4 = NULL, *filp5 = NULL;

	for(int iterNum = 0; iterNum  < 2000; iterNum++)
	{
		filp1 = fopen(INPUT_FILE_NAME, "r");
		filp2 = fopen(OUTPUT_FILE_NAME1, "w");
		filp3 = fopen(OUTPUT_FILE_NAME2, "w");
		filp4 = fopen(OUTPUT_FILE_NAME3, "w");
		filp5 = fopen(OUTPUT_FILE_NAME4, "w");
		
		while((nRead = fread(buf, 1, sizeof(buf), filp1)) > 0) {
			fwrite(buf, 1, nRead, filp2);
			fwrite(buf, 1, nRead, filp3);
			fwrite(buf, 1, nRead, filp4);
			fwrite(buf, 1, nRead, filp5);
		}
		
		char buffer[100];
		strcpy(buffer, "Writing Finished\n");
		fwrite(buffer, 1, strlen(buffer), filp2);
		fwrite(buffer, 1, strlen(buffer), filp3);
		fwrite(buffer, 1, strlen(buffer), filp4);
		fwrite(buffer, 1, strlen(buffer), filp5);
		
		fclose(filp1);
		fclose(filp2);
		fclose(filp3);
		fclose(filp4);
		fclose(filp5);
		
	}
	
	fprintf(stdout, "Please see the system log for all page faults\n");
	return 0;
}
