

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include "data.h"

int main()
{
	
	int i = 0, fd, ret, size, nbytes = 0;
	Data data;

	fd = open("/dev/process_list", O_RDONLY);
	if(fd == -1)
	{
		printf("file does not exist or locked by another process\n");
		exit(-1);
	}

	while(nbytes != -1)
	{
		nbytes = read(fd, &data, sizeof(Data));
		printf("\nbytes %d", nbytes);
		printf("\nPID %d PPID %d CPU %d state %ld", data.pid, data.ppid, data.cpu, data.state);		
	}

	close(fd);
	return 0;
}
