#include "server.h"
#include <pthread.h>
#include <stdbool.h>

int main(void)
{
	pthread_t car1, car2;
	int valread, sock_server, car1_id, car2_id;
	char *hello = "Hello from server";
	bool *red_flag;
	// Creating the threads
	
	// Making connection with client
	sock_server = establish_server_connection();

	// Receive data from client
	receive_data_server(&red_flag);

/*	if( *buffer == true )
	{
		// Enter in critical section
		
	}
*/	
	if( *red_flag == true )
		printf("Red flag is: %d \n", 1);
	else
		printf("%d \n", 0);

	close(sock_server);

	return 0;
}
