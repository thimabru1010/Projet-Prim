#include "server.h"
#include <stdbool.h>
#include <pthread.h>

int main(void)
{
	pthread_t car1_id, car2_id;
	int sock1_server, sock2_server, thread_status, *status1, *status2;
	struct message *message_red1, *message_red2;
	message_red1 = (struct message *) malloc(sizeof(struct message));
	message_red2 = (struct message *) malloc(sizeof(struct message));
//	char *message_red1->car_ip = (char *) malloc(sizeof("192.168.0.105"));
//	char *message_red2->car_ip = (char *) malloc(sizeof(" "));
	strcpy(message_red1->car_ip, "192.168.0.104");
	strcpy(message_red2->car_ip, "000.000.0.000");
	printf("%s \n", message_red1->car_ip);
	// Make connection with car 1
	sock1_server = establish_server_connection(message_red1->car_ip, PORT1, status1);

	// Make connection with car 2
//	sock2_server = establish_server_connection(message_red2->car_ip, PORT2, status2);
	
	message_red1->status = *status1;
	// Creating the threads
	thread_status = pthread_create( &car1_id, NULL, thread_send_recv, (void *) message_red1 );
	if( thread_status != 0 )
	{
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}

/*	thread_status = pthread_create( &car2_id, NULL, thread_send_recv, (void *) message_red2 );
	if( thread_status != 0)
	{
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}
*/	
	free(message_red1);
	free(message_red2);
	close(sock2_server);
	close(sock1_server);
	return 0;
}
