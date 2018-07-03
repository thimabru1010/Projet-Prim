#include "server.h"

typedef struct sockaddr Sockaddr;
typedef struct in_addr In_addr;
typedef struct sockaddr_in Sockaddr_in;

int establish_server_connection(char *client_ip, int port, int *status)
{
	int sock_server, opt =1;
	Sockaddr_in address;

	// Socket creation
	if( (sock_server = socket( AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}

	// Avoid bind error: address already in use
	if( setsockopt( sock_server, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt) ) )
	{
		perror("setsockspot");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(client_ip);
	address.sin_port = htons(port);

	// Bind: Assign adress to the socket
	*status = bind(sock_server, (const struct sockaddr *)&address, sizeof(address));
	printf("%d \n", *status);
	if( *status < 0 )
	{
		perror("bind");
		exit(EXIT_FAILURE);
	}

	*status = listen(sock_server, 3);
	if( *status < 0 )
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	else
		printf("Waiting connection from a client...\n");	

	// Accept Connection
	Sockaddr foreignAddr;
	int cli_len = sizeof(address);

	*status = accept(sock_server, (Sockaddr *)&address, &cli_len);
	if( *status < 0 )
	{
	  printf("Acceptation failed with status %s\n", strerror(errno));
	  exit(EXIT_FAILURE);
	}

	printf("connection made \n");	
	return sock_server;
}

void *thread_send_recv(void *message_arg)
{
	// variables flag's indicates if it's possible to enter on the critical
	// section
	struct message *message_red = (struct message*) message_arg;
	struct message  *message_cs;
	message_cs = (struct message *) malloc(sizeof(struct message));
	message_cs->flag = true;
	strcpy(message_cs->car_ip, "000.000.0.000");
	bool *buffer;
	buffer = (bool *) malloc(sizeof(bool));
	
	pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
	
	while(true)
	{
		// red_flag = true if the car entered in the cs and false if exited
		// read too from which car the signals cames from
		read(message_red->status, (void *) buffer, sizeof(bool));
		message_red->flag = (bool *) buffer;

		pthread_mutex_lock(&lock);
		if( message_red->flag == true && message_cs->flag == true )
		{
			message_cs->flag = !message_cs->flag;
			strcpy(message_cs->car_ip, message_red->car_ip);
		}
		if( message_red->flag == false && message_cs->flag == false )
		{
			printf("false \n");
			message_cs->flag = !message_cs->flag;
			strcpy(message_cs->car_ip, "000.000.0.000");
		}
		pthread_mutex_unlock(&lock);
		
		send(message_red->status, (const void *) message_cs, sizeof(struct message), 0);
		
	}
	free(buffer);
}

