#include "client.h"
#include "PID.h"
#include <unistd.h>
#define RED_MIN 65
#define RED_MAX 75

int main(void)
{

// PID variables --------------------------------------------------------

	float value0, target, error = 0, prop, integral = 0, derivate = 0, PID_value;
	float last_error = 0, kp = 0.45, ki = 0.0008, kd = 0.9;
	int duty_cycle, duty_cycle_sp;
	POOL_T sock_col_sensor;
// ----------------------------------------------------------------------

// TCP variables -------------------------------------------------------
	
	int status, valread, sock_client;
	struct message *message_cs;
	message_cs = (struct message *) malloc(sizeof(struct message));
	strcpy(message_cs->car_ip, "000.000.0.000");
	bool *red_flag;
	red_flag = (bool *) malloc(sizeof(bool));

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
//		calculate_run_pid(sock_col_sensor);	

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
			*red_flag = true;
		else
			*red_flag = false;
		
		// Warn (or not) server if red mark was found (entered on critical section)
		send( sock_client, (const void *) red_flag, sizeof(red_flag), 0 );
		
		// Receive if it's possible to enter on critical section
		read( sock_client, (void *) message_cs, sizeof(message_cs) );
		printf("%s \n", message_cs->car_ip);	
		// the car will only stop if there is someone on the critcal section
		//  AND it's not him (check the adress) AND  just if he arrives 
		//  on red mark, otherwise he can continue to run
		if( message_cs->flag == false && *red_flag == true && strcmp(car_ip, message_cs->car_ip) == 0 )
			tacho_stop(MOTOR_BOTH);

	
	}
	
	free(red_flag);
	free(message_cs);
	close(sock_client);
	return 0;

}
