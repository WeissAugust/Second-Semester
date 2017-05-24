#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
	int childs_max = 9;
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
			exit(0);
		}
		else
		{
			wait(NULL);
		}
	}
	
	return EXIT_SUCCESS;
}
