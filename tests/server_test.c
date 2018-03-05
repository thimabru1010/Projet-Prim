#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <
#define PORT 22

/*
typedef struct sockaddr {
	unsigned short sa_family;
	char sa_data[14];
} Sockaddr;

 typedef struct in_addr {
	unsigned long s_addr;
} In_addr;

typedef struct sockaddr_in {
	unsigned short sin_family;
	unsigned short sin_port;
	struct in_addr sin_addr;
	char sin_zero[8];
} Sockaddr_in;
*/

typedef struct sockaddr Sockaddr;
typedef struct in_addr In_addr;
typedef struct sockaddr_in Sockaddr_in;

int main(void)
{
	int sock_server, status, valread;
	char buffer[1024] = {0};
	char *hello = "Hello from server";
	Sockaddr_in address;

	// Socket creation
	if( (sock_server = socket( AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("Socket server failed \n");
		exit(1);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("192.168.0.106"); //INADDR_ANY before
	//Default port for ssh
	address.sin_port = htons(PORT) ;

	// Assign adress to the socket
	status = bind(sock_server, (Sockaddr *)&address, sizeof(address));
	if( status < 0 )
	{
		printf("Bind failed \n");
		exit(1);
	}

	status = listen(sock_server, 2);
	if( status < 0 )
	{
		printf("Listen failed \n");
		exit(1);
	}	

	// Accept Connection
	Sockaddr foreignAddr;
	status = accept(sock_server, (Sockaddr *)&address, (socklen_t*)sizeof(address));
	if( status < 0 )
	{
		printf("Accpetion failed \n");
		exit(1);
	}

	valread = read(status, buffer, 1024);
	printf("%s\n", buffer );
	send(status, hello, strlen(hello), 0);
	
	
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
