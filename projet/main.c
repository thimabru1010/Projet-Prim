#include "PID.h"
#include "server.h"
#include <pthread.h>

int main(void)
{
	pthread_t car1, car2;
	int status, valread, sock_server;
	char buffer[1024] = {0};
	char *hello = "Hello from server";
	bool red_flag;
	POOL_T sock_col_sensor;

	// Initializing the brick
	init_brick();

	// Searching for color sensor
	sock_col_sensor = search_set_color_sensor();

	// Making connection with client
	status = estabiish_connection();
	if( status > 0 )
	{
		while(true)
		{
			// Calculating PID
			red_flag = calculate_pid(sock_col_sensor);

			// Receive data from client
			valread = read(status, buffer, 1024);
			printf("%s\n", buffer );

			// Send data to client
			send(status, data_message, 2*sizeof(int), 0);
			
			close(sock_server);
		}
	}


	return 0;
}
