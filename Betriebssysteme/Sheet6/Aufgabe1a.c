#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int shmId;					// the id of the shared segment
	key_t key = 1000;				// key
	size_t size = sizeof(int);			// size of memory
	char buf[4096];
	
	shmId = shmget(key, size, IPC_CREAT | 0666);	// gets segment -> Creates new segment
							// with read, write rights
	
	if(shmId == -1)
	{
		perror("shmget failed");
		exit(1);
	}
	else
	{
		int* data = shmat(shmId, (void*)0,0);		// attach pointer to segment
		*data = 0;					// set data to 0
		
		if(shmdt(data) == -1)				// detach pointer
		{
			perror("shmdt(data) failed");
			exit(1);
		}
		
		if(mkfifo("RESULT_FIFO", 0666) == -1)		// creates fifo
		{
			//perror("mkfifo failed");
			//exit(1);
		}
		
		FILE* myFifo = fopen("RESULT_FIFO", "r");	// filereader opens fifo
		
		if(myFifo == NULL)
		{
			perror("fopen failed");
			exit(1);
		}
		
		while(fscanf(myFifo, "%s", buf) > 0)		// filereader reads fifo
		{
			printf("%s\n", buf);
		}
		
		if(fclose(myFifo) == -1)			// close fifo
		{
			perror("fclose failed");
			exit(1);
		}
	}
	return EXIT_SUCCESS;
}
