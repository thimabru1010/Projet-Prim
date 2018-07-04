#include "server.h"
#include <stdbool.h>
#include <pthread.h>

int main(void)
{
	pthread_t car1_id, car2_id;

	int sock1_server, sock2_server, thread_status, *status1, *status2;
	status1 = (int *) malloc(sizeof(int));
	status2 = (int *) malloc(sizeof(int));
	
	struct message *message_red1, *message_red2;
	message_red1 = (struct message *) malloc(sizeof(struct message));
	message_red2 = (struct message *) malloc(sizeof(struct message));

	message_red1->port = PORT1;
	message_red2->port = PORT2;
	
	// Same ip for both cars
	char car_ip[14] = "192.168.0.104";	
		
	// Make connection with car 1
	sock1_server = establish_server_connection(car_ip, message_red1->port, status1);

	// Make connection with car 2
	sock2_server = establish_server_connection(car_ip, message_red2->port, status2);
	
	message_red1->status = *status1;
	// Creating the threads
	thread_status = pthread_create( &car1_id, NULL, thread_send_recv, (void *) message_red1 );
	if( thread_status != 0 )
	{
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}
	
	message_red2->status = *status2;
	thread_status = pthread_create( &car2_id, NULL, thread_send_recv, (void *) message_red2 );
	if( thread_status != 0)
	{
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}
	
	pthread_join(car1_id, NULL);
	pthread_join(car2_id, NULL);

	printf("Ending \n");
	free(message_red1);
	free(message_red2);
//	close(sock2_server);
	close(sock1_server);
	return 0;
}
