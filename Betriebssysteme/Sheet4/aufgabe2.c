#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
	int childs_max = 16;
	pid_t pids[childs_max];

	for (int i = 0; i < childs_max; i++)
	{
		pids[i] = fork();
			
		if(pids[i] < 0)
		{
			printf("An error occoured\n");
			exit(1);
		}	
		else if(pids[i] == 0)
		{
			printf("Child process with pid: %d\n", getpid());
			exit(0);
			
		}
		else
		{
			printf("Waiting for childs to execute\n");
			wait(NULL);
		}
	}
	
	/*
		It is consistent.
		The order can be predicted: It's always first a child then waiting -> parent
		because of exit(0); (depends on OS)
	*/
	return EXIT_SUCCESS;
}
