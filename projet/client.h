#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <stdbool.h>
#define PORT 4444

struct message
{
	bool flag;
	char car_ip[14];
	int status;
};


int establish_client_connection(char *car_ip);


