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

typedef struct sockaddr Sockaddr;
typedef struct in_addr In_addr;
typedef struct sockaddr_in Sockaddr_in;

int main(void)
{
	int sock_server, status, valread;
	int opt = 1;
	char buffer[1024] = {0};
	char *hello = "Hello from server";
	int data_message[] = {10, 4444};
	Sockaddr_in address;

	// Socket creation
	if( (sock_server = socket( AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}

	// Avoid bind error: address already in use
	if( setsockopt( sock_server, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt) ) )
	{
		perror("setsockspot");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("192.168.0.105"); //INADDR_ANY before
	//Default port for ssh
	address.sin_port = htons(PORT) ;

	// Bind: Assign adress to the socket
	// Why can't I use my typedef?
	status = bind(sock_server, (const struct sockaddr *)&address, sizeof(address));
	if( status < 0 )
	{
		perror("bind");
		exit(EXIT_FAILURE);
	}

	status = listen(sock_server, 3);
	if( status < 0 )
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	else
		printf("Waiting connection from a client...\n");	

	// Accept Connection
	Sockaddr foreignAddr;
	// why don't need to cast with socklen_t* ?
	int cli_len = sizeof(address);
	status = accept(sock_server, (Sockaddr *)&address, &cli_len);
	if( status < 0 )
	{
	  printf("Acceptation failed with status %s\n", strerror(errno));
	  exit(EXIT_FAILURE);
	}
	
	// Receive data from client
	valread = read(status, buffer, 1024);
	printf("%s\n", buffer );

	// Send data to client
	send(status, data_message, 2*sizeof(int), 0);
	
	
/*	// Exchange data with stream Socket
	const void[] msg;
	void[] recvBuf;
	int flag1, flag2;
	int count1, count2; 
	count1 = sendto(sock_id, msg, sizeof(msg), flag1);
	count2 = recvfrom(sock_id, recvBuf, sizeof(recvBuf), flag2);
*/	 

	// CLosing the socket
	status = close(sock_server);

	return 0;
}
