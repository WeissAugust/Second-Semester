#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int pid[99];
	int shmId;
	key_t key = 1000;
	size_t size = sizeof(int);
	
	shmId = shmget(key, size, IPC_CREAT | 0666);
	if(shmId == -1)
	{
		perror("shmget failed");
		exit(1);
	}
	{
		for(int i = 0; i < 100; i++)
		{
			pid[i] = fork();
		
			if(pid[i] < 0)
			{
				printf("fork() failed\n");
				exit(1);
			}	
			else if(pid[i] == 0)
			{
				int* data = shmat(shmId, (void*)0,0);		
			
				for(int iteration = 0; iteration < 100; iteration++)
				{
					*data = *data + 1;
				}
						
				if(shmdt(data) == -1)				
				{
					perror("shmdt(data) failed");
					exit(1);
				}
			
				exit(0);
			}
			else
			{
				wait(NULL);
				
				
			}
		}

		int* data = shmat(shmId, (void*)0,0);
		
		FILE* fifo = fopen("RESULT_FIFO", "w");
		
		if(fifo == NULL)
		{
			perror("fopen failed");
			exit(1);
		}
				
		fprintf(fifo, "%d", *data);
				
		printf("Data in shared memory: %d\n", *data);
				
		if(shmdt(data) == -1)				
		{
			perror("shmdt(data) failed");
			exit(1);
		}
		
		if(fclose(fifo) == -1)
		{
			perror("fclose failed");
			exit(1);
		}
	}
	


	return EXIT_SUCCESS;
}
