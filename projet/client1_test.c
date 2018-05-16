#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#define PORT 4444
#include <stdbool.h>

typedef struct sockaddr Sockaddr;
typedef struct in_addr In_addr;
typedef struct sockaddr_in Sockaddr_in;

int main(void)
{
	char *hello = "Hello from client 1";
	char buffer[1024] = {0};
	int int_buffer[2];
	int sock_client, valread;
	Sockaddr_in serv_addr;

	// Creates the socket
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
	
	printf("Connection with server established \n");
	
	int button;
	char *hello2 = "Hello 2 from client 1";
		
	printf("Press a number\n");
	scanf("%d", &button);
	if( button == 1 )
	{
		// Send data to the server
		send(sock_client, hello, strlen(hello), 0);
		printf("Hello Message sent \n");
	}



/*	// Receive data from the server
	valread = read( sock_client, int_buffer, 1024);
	if( valread < 0 )
	{
		perror("read");
		exit(EXIT_FAILURE);
	}
	printf("%d \t %d\n", int_buffer[0], int_buffer[1]);
	close(sock_client);
*/

	return 0;
}
