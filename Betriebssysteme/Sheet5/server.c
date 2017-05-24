#include "ServerClientHeader.h"

int main(int argc, char** argv)
{
	srand(time(NULL));
	int database, middleware, web;
	
	// create server-fifos
	if(mkfifo(DATABASE_SERVER, O_CREAT | 0666) == -1)
	{
		perror("mkfifo database");
		exit(EXIT_FAILURE);
	}
	
	if(mkfifo(MIDDLEWARE_SERVER, O_CREAT | 0666) == -1)
	{
		perror("mkfifo middleware");
		exit(EXIT_FAILURE);
	}
	
	if(mkfifo(WEB_SERVER, O_CREAT | 0666) == -1)
	{
		perror("mkfifo web");
		exit(EXIT_FAILURE);
	}
	
	database = open(DATABASE_SERVER, O_WRONLY);
	middleware = open(MIDDLEWARE_SERVER, O_WRONLY);
	web = open(WEB_SERVER, O_WRONLY);
	
	while(true)
	{
		// sending messages
		int seconds = rand() % 5 + 2;
		int server = rand() % 3 + 1;
		
		switch(server)
		{
			case 1:
				if(write(database, DATABASE_MESSAGE, strlen(DATABASE_MESSAGE) + 1) == -1)
				{
					perror("write database");
					exit(EXIT_FAILURE);
				}
				break;
			case 2:
				if(write(middleware, MIDDLEWARE_MESSAGE, strlen(MIDDLEWARE_MESSAGE) + 1) == -1)
				{
					perror("write middleware");
					exit(EXIT_FAILURE);
				}
				break;
			case 3:
				if(write(web, WEB_MESSAGE, strlen(WEB_MESSAGE) + 1) == -1)
				{
					perror("write web");
					exit(EXIT_FAILURE);
				}
				break;
			default:
				perror("switch unexpected case");
				exit(EXIT_FAILURE);
		}
		
		
		sleep(seconds);
	}
	
	if(close(database))
	{
		perror("close database");
		exit(EXIT_FAILURE);
	}
	if(close(middleware))
	{
		perror("close middleware");
		exit(EXIT_FAILURE);
	}
	if(close(web))
	{
		perror("close web");
		exit(EXIT_FAILURE);
	}
	
	if(unlink(DATABASE_SERVER) == -1)
	{
		perror("unlink database");
		exit(EXIT_FAILURE);
	}
	if(unlink(MIDDLEWARE_SERVER) == -1)
	{
		perror("unlink middleware");
		exit(EXIT_FAILURE);
	}
	if(unlink(WEB_SERVER) == -1)
	{
		perror("unlink web");
		exit(EXIT_FAILURE);
	}
	
	return EXIT_SUCCESS;
}
