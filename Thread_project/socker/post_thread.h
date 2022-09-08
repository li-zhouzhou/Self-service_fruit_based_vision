#ifndef     _POST_THREAD_H
#define     _POST_THREAD_H

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>

#include "../main/main_a.h"
 
#define IPSTR "192.168.40.8"
#define PORT 8081
#define BUFSIZE 1024


void* post(void* thread_arg);

#endif