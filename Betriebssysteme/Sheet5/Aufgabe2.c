#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
	int fd[2];
	pid_t child_1;
	pid_t child_2;
	
	//char arrays with parameters
	char* ls_args[] = {"ls", "-l", 0};
	char* grep_args[] = {"grep", argv[1], 0};
	
	//pipe
	if(pipe(fd) == -1)
	{
		perror("Creating pipe failed");
		exit(EXIT_FAILURE);
	}
	
	if((child_1 = fork()) == 0)
	{
		//closing stdout
		close(STDOUT_FILENO);
		//stdout -> pipe
		dup(fd[1]);
		
		//closing pipe
		close(fd[0]);
		close(fd[1]);
		
		execvp(ls_args[0], ls_args);
		perror("execvp failed");
		exit(EXIT_FAILURE);
	}
	
	if((child_2 = fork()) == 0)
	{
		//closing stdin
		close(STDIN_FILENO);
		//stdin -> pipe
		dup(fd[0]);
		
		//closing pipe
		close(fd[1]);
		close(fd[0]);
		
		execvp(grep_args[0], grep_args);
		perror("execvp failed");
		exit(EXIT_FAILURE);
	}
	
	close(fd[0]);
	close(fd[1]);
	
	wait(NULL);
	wait(NULL);
	return EXIT_SUCCESS;
}
