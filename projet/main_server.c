#include "server.h"
#include <stdbool.h>
#include <pthread.h>

int main(void)
{
	pthread_t car1_id, car2_id;
	int sock1_server, sock2_server, status;
	char *hello = "Hello from server";
	struct message *message_red1, *message_red2;
	strcpy(message_red1->car_ip,  "192.168.0.105");
	strcpy(message_red2->car_ip, " ");

	// Make connection with car 1
	sock1_server = establish_server_connection(message_red1->car_ip, PORT1);

	// Make connection with car 2
	sock2_server = establish_server_connection(message_red2->car_ip, PORT2);

	// Creating the threads
	status = pthread_create( &car1_id, NULL, thread_send_recv, (void *) message_red1 );
	if( status != 0 )
	{
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}

	status = pthread_create( &car2_id, NULL, thread_send_recv, (void *) message_red2 );
	if( status != 0)
	{
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}

	close(sock2_server);
	close(sock1_server);
	return 0;
}
