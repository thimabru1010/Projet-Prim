#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>

struct in_addr {
	unsigned long s_addr;
}
struct sockaddr_in {
	unsigned short sin_family;
	unsigned short sin_port;
	struct in_addr sin_addr;
	char sin_zero[8];
}

int main(void)
{
	int sock_id, status;
	struct sockaddr_in address;

	// Why use PF_INET and not AF_INET ?
	if( (sock_id = socket( AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("Socket failed \n");
		exit(1);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = ;
	if( (status = bind(sock_id, &address, sizeof(address)) < 0)
	{
		printf("Bind failed");
		exit(1);
	}
		

	status = close(sock_id);
	return 0;
}
