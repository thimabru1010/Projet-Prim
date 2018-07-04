#include "client.h"
#include "PID.h"
#include <unistd.h>
#define RED_MIN 35
#define RED_MAX 60
#define YEL_MIN 65
#define YEL_MAX 85

int main(void)
{

// PID variables --------------------------------------------------------

	float value0, target, error = 0, prop, integral = 0, derivate = 0, PID_value;
	float last_error = 0, kp = 0.45, ki = 0.0008, kd = 0.9;
	int duty_cycle, duty_cycle_sp;
	POOL_T sock_col_sensor;
// ----------------------------------------------------------------------

// TCP variables -------------------------------------------------------
	
	int status, valread, sock_client, port;
	port = PORT;
	struct message *message_cs;
	message_cs = (struct message *) malloc(sizeof(struct message));
	message_cs->port = 0;
	bool *red_flag;
	red_flag = (bool *) malloc(sizeof(bool));
	*red_flag = false;

// ---------------------------------------------------------------------

	//Change this depending on the client
	char car_ip[14] = "192.168.0.104";

	// Setting a vlue for the target
	target = 40;

	// Initialize the brick
	init_brick();


	// Find and set color sensor
	sock_col_sensor = search_set_color_sensor();
   
	// Making connection with server
	sock_client = establish_client_connection(car_ip);
	
	int cont = 0;
	float left_motor_speed = 0, right_motor_speed = 0;

	while(true)
	{

// PID code -------------------------------------------------------------
		value0 = sensor_get_value0( sock_col_sensor, COLOR_COL_REFLECT);

		// Error
		error = target - value0;

		// Proportional
		prop = error;

		// Integral
		integral += error;

		// Derivate
		derivate = error - last_error;

		PID_value = prop*kp + integral*ki + derivate*kd;
	
		last_error = error;

		// Makes the car come back to the line
		left_motor_speed = SPEED_LINEAR - PID_value;
		right_motor_speed = SPEED_LINEAR + PID_value;
		tacho_set_duty_cycle_sp( MOTOR_LEFT, left_motor_speed);
		tacho_set_duty_cycle_sp( MOTOR_RIGHT, right_motor_speed);
		tacho_run_direct(MOTOR_BOTH);
		
		// Report values
		if( cont % 4 == 0 )
		{
			printf("Value obtained: %.2f \n", value0);
			printf("cont: %d\n", cont);
			printf("error: %.2f\n", error);
			printf("integral*ki: %.2f\n", integral*ki);
			printf("derivate*kd: %.2f\n", derivate*kd);
			printf("PID value: %.2f\n", PID_value);
			printf("Motor left speed: %.2f \t right speed: %.2f \n\n", left_motor_speed, right_motor_speed);
		}


// PID code ending -------------------------------------------------------
		
// Transfer data code -----------------------------------------------

		if( value0 >= RED_MIN && value0 <= RED_MAX )
		{
			*red_flag = true;
			printf("RED \n");
		}
		if( value0 >= YEL_MIN && value0 <= YEL_MAX )
			*red_flag = false;
		
		// Warn (or not) server if red mark was found (entered on critical section)
		send( sock_client, (const void *) red_flag, sizeof(red_flag), 0 );
		
		// Receive if it's possible to enter on critical section
		read( sock_client, (void *) message_cs, sizeof(struct message) );

		printf("%d %d %d\n", message_cs->port, *red_flag, message_cs->flag);	
		// the car will only stop if there is someone on the critcal section
		//  AND it's not him (check the adress) AND  just if he arrives 
		//  on red mark, otherwise he can continue to run
		if( message_cs->flag == false && *red_flag == true && port == message_cs->port )
			tacho_stop(MOTOR_BOTH);
// Transfer data code ending -------------------------------------------------------------
	
	}
	
	free(red_flag);
	free(message_cs);
	close(sock_client);
	return 0;

}
