#include "server.h"
#include <pthread.h>
#include <stdbool.h>

int main(void)
{
	pthread_t car1, car2;
	int valread, sock1_server, sock2_server, car1_id, car2_id;
	char *hello = "Hello from server";
	struct message message_red1, message_red2;
	message_red1->car_ip[] = "192.168.0.105";
	message_red2->car_ip[] = " ";

	// Make connection with car 1
	sock1_server = establish_server_connection(car1_ip, PORT1);

	// Make connection with car 2
	sock2_server = establish_server_connection(car2_ip, PORT2);

	// Creating the threads
	car1_id = pthread_create( &car1, NULL, thread_send_recv, (void *) message_red1 );
	if( car1_id )
	{
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}

	car2_id = pthread_create( &car2, NULL, thread_send_recv, (void *) message_red2 );
	if( car2_id )
	{
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}

	close(sock2_server);
	close(sock1_server);
	return 0;
}
