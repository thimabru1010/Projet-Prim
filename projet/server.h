#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#define PORT 4444

// Creates the socket. Makes de Bind. Accept connection from client.
// Returns a positive number if the connection was accpeted.
int create_server(void);

