#include "client.h"
#include "PID.h"

int main(void)
{
	char buffer[1024] = {0};
	int int_buffer[2];
	int status, valread, sock_client;
	POOL_T sock_col_sensor;
	bool red_flag;

	// Initializing the brick
	init_brick();

	// Searching for color sensor
	sock_col_sensor = search_set_color_sensor();

	// Making connection with server
	sock_client = establish_client_connection();
	
	while(true)
	{
		red_flag = calculate_pid(sock_col_sensor);

		// Send message for server enter in critical section
		if( red_flag == true )
		{
			send(sock_client, red_flag, sizeof(bool));
			printf("Car is on the red mark \n");
		}
	}

	// Receive data from the server
	valread = read( sock_client, int_buffer, 1024);
	if( valread < 0 )
	{
		perror("read");
		exit(EXIT_FAILURE);
	}
	printf("%d \t %d\n", int_buffer[0], int_buffer[1]);

	close(sock_client);
	return 0;
}
