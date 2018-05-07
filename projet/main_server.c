#include "server.h"
#include <pthread.h>

int main(void)
{
	pthread_t car1, car2;
	int valread, sock_server;
	char *hello = "Hello from server";
	bool red_flag, buffer;

	// Making connection with client
	sock_server = establish_server_connection();

	// Receive data from client
	valread = read(status, buffer, sizeof(bool));			
	if( buffer == true )
	{
		// Enter in critical section
	}

	// Send data to client
	send(status, data_message, 2*sizeof(int), 0);
			
	close(sock_server);

	return 0;
}
