#include "client.h"
#include "PID.h"

int main(void)
{
	int status, valread, sock_client;
	POOL_T sock_col_sensor;
	struct message message_cs;
	bool red_flag;

	//Change this depending on the client
	char car_ip[] = "192.168.0.105";

	// Initializing the brick
	init_brick();

	// Searching for color sensor
	sock_col_sensor = search_set_color_sensor();

	// Making connection with server
	sock_client = establish_client_connection();
	
	while(true)
	{
		// Calculates the PID and makes the car run
		red_flag = calculate_run_pid(sock_col_sensor);

		// Warn (or not) server if red mark was found (entered on critical section)
		send( sock_client, red_flag, sizeof(red_flag), 0 );
		
		// Receive if it's possible to enter on critical section
		read( sock_client, message_cs, sizeof(message_cs) );
		
		// the car will only stop if there is someone on the critcal section
		//  AND it's not him (check the adress) AND  just if he arrives 
		//  on red mark, otherwise he can continue to run
		if( message_cs.flag == false && red_flag == true && strcmp(car_ip, message_cs->car_ip) == 0 )
			tacho_stop(MOTOR_BOTH);	
	} 

	close(sock_client);
	return 0;
}
