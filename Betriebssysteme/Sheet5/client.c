#include "ServerClientHeader.h"

int main(int argc, char** argv)
{
	int database, middleware, web;
	
	database = open(DATABASE_SERVER, O_RDONLY);
	
	if(database == -1)
	{
		perror("open database");
		exit(EXIT_FAILURE);
	}
	
	middleware = open(MIDDLEWARE_SERVER, O_RDONLY);
	
	if(middleware == -1)
	{
		perror("open database");
		exit(EXIT_FAILURE);
	}
	
	web = open(WEB_SERVER, O_RDONLY);
	
	if(web == -1)
	{
		perror("open database");
		exit(EXIT_FAILURE);
	}
	
	char buff[2056];
	fd_set set;
	int maxfd;

	while(true)
	{
		FD_ZERO(&set);
		FD_SET(database, &set);
		FD_SET(middleware, &set);
		FD_SET(web, &set);
	
		maxfd = database;
		if(maxfd < middleware)
			maxfd = middleware;
		if(maxfd < web)
			maxfd = web;
			
		select(maxfd + 1, &set, NULL, NULL, NULL);
	
		if(FD_ISSET(database, &set))
		{
			if(read(database, buff, sizeof(buff)) > 0)
			{
				printf("%s", buff);
			}
		}
		if(FD_ISSET(middleware, &set))
		{
			if(read(middleware, buff, sizeof(buff)) > 0)
			{
				printf("%s", buff);
			}
		}
		if(FD_ISSET(web, &set))
		{
			if(read(web, buff, sizeof(buff)) > 0)
			{
				printf("%s", buff);
			}
		}
		
		
	}
	return EXIT_SUCCESS;
}
