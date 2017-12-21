
/*

   The process_hierarchy C file contains source code that will recursively create a process hierarchy tree that H levels 
   deep, print out the process tree information as the tree is being created and then systamatically terminates all the 
   processes in the tree. 

*/

#include <stdio.h>   										// standard input output header file	- IO functions
#include <stdlib.h>										// standard library header file		- Macro and functions
#include <unistd.h>										// standard header file			- POSIX operating system API	
#include <sys/types.h>										// standard header file			- POSIX operating system API
#include <sys/wait.h>										// standard header file			- POSIX operating system API

typedef int H;											// alias for an int to showcase height of process hierarchy
typedef int C;											// alias for an int to showcase number of child process to each node 


int main(int argc, char *argv[])								// main entry point		        - begin program execution
{

	if(argc == 0 || argc == 1 || argc == 2)
	{
		fprintf(stderr, "Two arguments expected.\n");
		exit(EXIT_FAILURE);
	}
	else if(argc > 3)
	{
		fprintf(stderr, "Too many arguments.\n");
		exit(EXIT_FAILURE);
	} 
	else
	{
		
		H height_of_process_hierarchy  = atoi(argv[1]);
		C number_of_child_to_each_node = atoi(argv[2]);

		pid_t process_ids[number_of_child_to_each_node];
                int counter_for_child_process = number_of_child_to_each_node;
                int loop_counter = 0;


		if(height_of_process_hierarchy < 0)
		{
			fprintf(stderr, "Process Hierarchy Height should not be \"negative.\" \n");
			exit(EXIT_FAILURE);
		}
		else
		{

			/* Display Status Information*/

                        fprintf(stdout, "(%d): Process starting\n", (int) getpid());
                        fprintf(stdout, "(%d): Parent\'s id = (%d)\n", (int) getpid(), (int) getppid());
                        fprintf(stdout, "(%d): Height in the tree = (%d)\n", (int) getpid(), height_of_process_hierarchy);
                        fprintf(stdout, "(%d): Creating (%d) children at height (%d)\n", 
						(int) getpid(), number_of_child_to_each_node, 
						(height_of_process_hierarchy - 1));                     
			if(height_of_process_hierarchy > 1) 
			{
				for(loop_counter = 0; loop_counter < counter_for_child_process; loop_counter++)
				{
					if((process_ids[loop_counter] = fork()) < 0)
					{
						fprintf(stderr, "fork failed.\n");
						exit(EXIT_FAILURE);
					}
					if(process_ids[loop_counter] == 0)
					{
						char h_buffer[20];
						char c_buffer[20];
						height_of_process_hierarchy = height_of_process_hierarchy - 1;
						snprintf(h_buffer, 10, "%d", height_of_process_hierarchy);
						snprintf(c_buffer, 10, "%d", number_of_child_to_each_node);
						char *argv[] = {"./process_hierarchy_main", h_buffer, c_buffer, NULL};
						execvp("./process_hierarchy_main", argv); 
						exit(EXIT_SUCCESS);
					}
				}
			}
		  }
	

				int status;
                                pid_t process_id;
                                while(counter_for_child_process > 0)
                                {
                                        process_id = wait(&status);
                                        --counter_for_child_process;
                                }
				printf("\n");
                                fprintf(stdout, "(%d): Terminating at height (%d).\n", (int) getpid(), height_of_process_hierarchy);
                                exit(EXIT_SUCCESS);
	}

	return 0;										// exit main function		      - deactivate from the stack  
}

// end of main function

