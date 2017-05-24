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
#include <sys/sem.h>

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
	
	int semId;
	semId = semget(key, sizeof(int*), IPC_CREAT | 0666);			// gets semaphore set
	if(semId == -1)
	{
		perror("semget failed");
		exit(1);
	}
	
	union semun arg;
	arg.val = 1;
	if(semctl(semId, 0, SETVAL, arg) == -1)
	{
		perror("semctl failed");
		exit(1);
	}
	
	struct sembuf sb = {0, -1, 0};						// index of first / operation(non 0 makes add/sub - 0 makes block until sem = 0) / flags
	
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
		
			if(semop(semId,&sb,1) == -1)				// gets semaphore to work with - id / operation / sem amount
			{
				perror("semop failed");
				exit(1);
			}
		
			for(int iteration = 0; iteration < 100; iteration++)
			{
				*data = *data + 1;
			}
					
			sb.sem_op = 1;
			
			if(semop(semId, &sb, 1) == -1)
			{
				perror("semop failed");
				exit(1);
			}
		
			exit(0);
		}

		wait(NULL);

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

	return EXIT_SUCCESS;
}
