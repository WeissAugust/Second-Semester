#define _POSIX_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>

void handler(int sig)
{
	if(sig == SIGUSR1)
	{
		printf("Recieved SIGUSR1\n");
	}
	else if(sig == SIGUSR2)
	{
		printf("Recieved SIGUSR2\n");
		exit(0);
	}
	else
	{
		printf("Wrong signal\n");
	}
}

int main(void)
{
	pid_t pid;
	pid = fork();
	
	if(pid < 0)
	{
		printf("An error occoured\n");
		exit(1);
	}	
	else if(pid == 0)
	{
		printf("Child process with pid: %d\n", getpid());
		
		signal(SIGUSR1, handler);
		pause();
		
		printf("Exiting...\n");
		
		exit(EXIT_SUCCESS);
	}
	else{}
	
	kill(pid, SIGUSR1);
	wait(NULL);
}


