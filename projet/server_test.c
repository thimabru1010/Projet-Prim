#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#define PORT1 4444
#define PORT2 8080
#include <stdbool.h>

typedef struct sockaddr Sockaddr;
typedef struct in_addr In_addr;
typedef struct sockaddr_in Sockaddr_in;

int main(void)
{
	int sock1_server, status1, valread, sock2_server, status2;
	int opt = 1;
	char buffer1[30] = {0}, buffer2[30] = {0};
	char *hello = "Hello from server";
	int data_message[] = {10, 4444};
	Sockaddr_in address1, address2;
	char *client1_ip = "192.168.0.105";
	char *client2_ip = "";

	// Client 1 connection ----------------------------------------------


	// Socket creation
	if( (sock1_server = socket( AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}

	// Avoid bind error: address1 already in use
	if( setsockopt( sock1_server, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt) ) )
	{
		perror("setsockspot");
		exit(EXIT_FAILURE);
	}

	address1.sin_family = AF_INET;
	address1.sin_addr.s_addr = inet_addr(client1_ip); 
	address1.sin_port = htons(PORT1) ;

	// Bind: Assign adress to the socket
	status1 = bind(sock1_server, (const struct sockaddr *)&address1, sizeof(address1));
	if( status1 < 0 )
	{
		perror("bind");
		exit(EXIT_FAILURE);
	}

	status1 = listen(sock1_server, 3);
	if( status1 < 0 )
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	else
		printf("Waiting connection from a client1...\n");	

	// Client 2 connection ----------------------------------
	
	// Accept Connection from client 1 ----------------------------------------
	
	Sockaddr foreignAddr;
	int cli_len = sizeof(address1);
	status1 = accept(sock1_server, (Sockaddr *)&address1, &cli_len);
	if( status1 < 0 )
	{
	  printf("Acceptation failed with status1 %s\n", strerror(errno));
	  exit(EXIT_FAILURE);
	}
	printf("Connection with client 1 made\n");


	// Socket 2 creation
	if( (sock2_server = socket( AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}

	// Avoid bind error: address1 already in use
	if( setsockopt( sock2_server, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt) ) )
	{
		perror("setsockspot");
		exit(EXIT_FAILURE);
	}


	address2.sin_family = AF_INET;
	address2.sin_addr.s_addr = INADDR_ANY;
	address2.sin_port = htons(PORT2) ;

	// Bind: Assign adress to the socket
	status2 = bind(sock2_server, (const struct sockaddr *)&address2, sizeof(address2));
	if( status2 < 0 )
	{
		perror("bind");
		exit(EXIT_FAILURE);
	}

	status2 = listen(sock2_server, 3);
	if( status2 < 0 )
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	else
		printf("Waiting connection from a client2...\n");	



	// Accept Connection from client 1 ----------------------------------------
	
/*	Sockaddr foreignAddr;
	int cli_len = sizeof(address1);
	status1 = accept(sock1_server, (Sockaddr *)&address1, &cli_len);
	if( status1 < 0 )
	{
	  printf("Acceptation failed with status1 %s\n", strerror(errno));
	  exit(EXIT_FAILURE);
	}
	printf("Connection with client 1 made\n");
*/
	
	// Client 1 connection made ----------------------------------------------

		// Accept Connection from client 2 ----------------------------------------
	
	int cli_len2 = sizeof(address2);
	status2 = accept(sock2_server, (Sockaddr *)&address2, &cli_len2);
	if( status2 < 0 )
	{
	  printf("Acceptation failed with status2 %s\n", strerror(errno));
	  exit(EXIT_FAILURE);
	}
	printf("Connection with client 2 made\n");

	// Client 2 connection made ----------------------------------------------


		// Receive data from client 1
		valread = read(status1, buffer1, 1024);
		printf("%s\n", buffer1 );

		// Receive data from client 2
		valread = read(status2, buffer2, 1024);
		printf("%s\n", buffer2);

	close(sock1_server);
	close(sock2_server);
	return 0;
}

