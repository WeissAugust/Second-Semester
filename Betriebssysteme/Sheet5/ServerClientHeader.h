#ifndef HEADER_H
#define HEADER_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <sys/select.h>

char* DATABASE_SERVER = "DatabaseServer";
char* MIDDLEWARE_SERVER = "MiddleWareServer";
char* WEB_SERVER = "WebServer";

char* DATABASE_MESSAGE = "Message from database\n";
char* MIDDLEWARE_MESSAGE = "Message from middleware\n";
char* WEB_MESSAGE = "Message from web\n";
#endif
