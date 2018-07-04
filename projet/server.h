#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#define PORT1 4444
#define PORT2 8080

struct message
{
	bool flag;
	int port;
	int status;
};

// Creates the socket. Makes de Bind. Accept connection from client.
// Returns the socket server
int establish_server_connection(char *client_ip, int port, int *status);

// Function to be used for the threads. Change data with client
void *thread_send_recv(void *message_arg);
