#include "server.h"

typedef struct sockaddr Sockaddr;
typedef struct in_addr In_addr;
typedef struct sockaddr_in Sockaddr_in;

extern int status;

int establish_server_connection(char *client_ip, int port)
{
	int status, sock_server, opt =1;
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
	status = bind(sock_server, (const struct sockaddr *)&address, sizeof(address));
	if( status < 0 )
	{
		perror("bind");
		exit(EXIT_FAILURE);
	}

	status = listen(sock_server, 3);
	if( status < 0 )
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	else
		printf("Waiting connection from a client...\n");	

	// Accept Connection
	Sockaddr foreignAddr;
	int cli_len = sizeof(address);

	status = accept(sock_server, (Sockaddr *)&address, &cli_len);
	if( status < 0 )
	{
	  printf("Acceptation failed with status %s\n", strerror(errno));
	  exit(EXIT_FAILURE);
	}
	
	return sock_server;
}

void *thread_send_recv(void *message_arg)
{
	// variables flag's indicates if it's possible to enter on the critical
	// section
	struct message *message_red = (struct message*) message_arg;
	struct message  *message_cs;
	message_cs->flag = true;
	message_cs->car_ip = "no address";
	bool buffer;

	int status;
	pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
	
	while(true)
	{
		// red_flag = true if the car entered in the cs and false if exited
		// read too from which car the signals cames from
		read(status, (void *) buffer, sizeof(buffer));
		message_red->flag = buffer;

		pthread_mutex_lock(&lock);
		if( message_red->flag == true && message_cs->flag == true )
		{
			message_cs->flag = !message_cs->flag;
			strcpy(message_cs->car_ip, message_red->car_ip);
		}
		if( message_red->flag == false && message_cs->flag == false )
		{
			message_cs->flag = !message_cs->flag;
			strcpy(message_cs->car_ip, "no address");
		}
		pthread_mutex_unlock(&lock);

		send(status, (const void *) message_cs, sizeof(message_cs), 0);
	}
}

