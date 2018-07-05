#include "PID.h"
#define RED_MIN 65
#define RED_MAX 75

int init_brick(void)
{
	printf( "Waiting the EV3 brick online...\n" );
    if ( !brick_init() ) 
	   return ( 1 );
    printf( "*** ( EV3 ) Hello! ***\n" );
}

POOL_T search_set_color_sensor(void)
{
	ev3_sensor_init();
	POOL_T sock_col_sensor;
	sock_col_sensor = sensor_search( LEGO_EV3_COLOR );
    if( sock_col_sensor )
    {
		if( color_set_mode_rgb_raw( sock_col_sensor ) == 1 )
			printf( "RGB mode set \n" );
		return sock_col_sensor;
    }
    else
	   printf("COLOR SENSOR NOT FOUND\n");
}

bool calculate_run_pid(POOL_T sock_col_sensor)
{
	float value0, target, error = 0, prop, integral = 0, derivate = 0, PID_value;
	float last_error = 0, kp = 0.45, ki = 0.0008, kd = 0.9;
	int duty_cycle, duty_cycle_sp, cont = 0;
	float left_motor_speed = 0, right_motor_speed = 0;

// Old Variables ------------------------------------------------

/*
	float value0, target, error = 0, prop, integral = 0, derivate = 0, PID_value;
	float last_error = 0, kp = 0.45, ki = 0.0008, kd = 0.9;
	float left_motor_speed = 0, right_motor_speed = 0;
	int cont = 0;
*/
// ---------------------------------------------------------------


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




// Old code
/*
	value0 = sensor_get_value0( sock_col_sensor, COLOR_COL_REFLECT);

	if( value0 >= RED_MIN && value0 <= RED_MAX )
		return true;

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
*/
	return false;
}
