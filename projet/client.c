#include "client.h"

typedef struct sockaddr Sockaddr;
typedef struct in_addr In_addr;
typedef struct sockaddr_in Sockaddr_in;

int establish_client_connection(void)
{
	int sock_client;
	Sockaddr_in serv_addr;

	// Creates client socket
	sock_client = socket(AF_INET, SOCK_STREAM, 0);
	if( sock_client < 0 )
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}

	memset(&serv_addr, '0', sizeof(serv_addr));

	// Put the right Port 
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 to addresses from text to binary
	if(  inet_pton(AF_INET, "192.168.0.105", &serv_addr.sin_addr) <= 0 )
	{
		printf("\nInvalid address/ Address not supported");
		return -1;
	}

	// Make the connection with the server
	if( ( connect(sock_client,  (struct sockaddr *)&serv_addr, sizeof(serv_addr)) ) < 0 )
	{
		perror("connect");
		exit(EXIT_FAILURE);
	}

	return sock_client;
}
	

