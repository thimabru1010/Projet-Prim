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
#define PORT1 4444
#define PORT2 8080

struct message
{
	bool flag;
	char *car_ip;
};

// Creates the socket. Makes de Bind. Accept connection from client.
// Returns the socket server
int establish_server_connection(void);

// Sends data from server to client
void *send_data_server(void* message);

// Receive data from client
void receive_data_server(void* buffer);
