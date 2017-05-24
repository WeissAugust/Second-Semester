#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
	pid_t pid = fork();
			
	if(pid < 0)
	{
		printf("An error occoured\n");
		exit(1);
	}	
	else if(pid == 0)
	{
		printf("Child process with pid: %d\n", getpid());
		exit(0);
	}
	else
	{
		waitpid(pid, NULL, 2);
		printf("Waited for child (%d) to execute\n", pid);
	}
	
	return EXIT_SUCCESS;
}
