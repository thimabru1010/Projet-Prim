#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#define PORT 22

/*
struct sockaddr {
	unsigned short sa_family;
	char sa_data[14];
} Sockaddr;

struct in_addr {
	unsigned long s_addr;
} In_addr;

struct sockaddr_in {
	unsigned short sin_family;
	unsigned short sin_port;
	struct in_addr sin_addr;
	char sin_zero[8];
} Sockaddr_in
*/

typedef struct sockaddr Sockaddr;
typedef struct in_addr In_addr;
typedef struct sockaddr_in Sockaddr_in;

int main(void)
{
	char *hello = "Hello from client";
	char buffer[1024] = {0};
	int sock_client, valread;
	Sockaddr_in serv_addr; 
	sock_client = socket(AF_INET, SOCK_STREAM, 0);
	if( sock_client < 0 )
	{
		printf("Socket client failed\n");
		exit(1);
	}

	memset(&serv_addr, '0', sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 to addresses from text to binary from ???? Didn't
	// understand
	if(  inet_pton(AF_INET, "192.168.0.106", &serv_addr.sin_addr) <= 0 )
	{
		printf("\nInvalid address/ Address not supported");
		return -1;
	}

	if( ( connect(sock_client, &serv_addr, sizeof(serv_addr)) ) < 0 )
	{
		printf("\nConnection failed \n");
		exit(1);
	}

	send(sock_client, hello, strlen(hello), 0);
	printf("Hello Message sent \n");
	valread = read( sock_client, buffer, 1024);
	printf("%s\n", buffer);


	return 0;
}
