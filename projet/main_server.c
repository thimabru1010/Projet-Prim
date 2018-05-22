#include "server.h"
#include <pthread.h>
#include <stdbool.h>

int main(void)
{
	pthread_t car1, car2;
	int valread, sock1_server, sock2_server, car1_id, car2_id;
	char *hello = "Hello from server";
	bool *red_flag, *message1, *message2;
	char car1_ip[] = "192.168.0.105";
	char car2_ip[] = " ";

	// Make connection with car 1
	sock1_server = establish_server_connection();

	// Make connection with car 2
	sock2_server = establish_server_connection();

	// Creating the threads
	message1 = false;
	car1_id = pthread_create( &car1, NULL, send_data_server, (void *) message1 );
	if( car1_id )
	{
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}

	message2 = false;
	car2_id = pthread_create( &car2, NULL, send_data_server, (void *) message2 );
	if( car2_id )
	{
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}

	while(true)
	{
		// Receive data from client
		// Receber também qual carro parou na luz vermelha

		receive_data_server(&red_flag);

		if( *red_flag == true )
		{
			// Enter in critical section
			
		
		}

	}
	
	close(sock2_server);
	close(sock1_server);
	return 0;
}
