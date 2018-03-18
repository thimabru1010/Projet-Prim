#include <stdio.h>
#include <stdlib.h>
#include "brick.h"
#include "ev3.h"
#include "ev3_light.h"
#define Sleep(ms) usleep(( ms ) * 1000 )
#define MOTOR_RIGHT OUTD
#define MOTOR_LEFT OUTA
#define MOTOR_BOTH ( MOTOR_LEFT | MOTOR_RIGHT )
// Percentage of max speed
#define SPEED_LINEAR 50


int main(void)
{
	float value0, target, error, prop, integral, derivate, PID_value;
	float last_error = 0, kp = 1, ki = 1, kd = 1;
	int max_speed, speed_linear, duty_cycle, duty_cycle_sp;
	bool duty_cycle_flag1, duty_cycle_flag2;
	POOL_T sock_col_sensor, sock_tacho_motor;
	target = 1000; // Still need to get an target value

	// Initialize the brick
    printf( "Waiting the EV3 brick online...\n" );
    if ( !brick_init() ) 
	   return ( 1 );
    printf( "*** ( EV3 ) Hello! ***\n" );
    ev3_sensor_init();

	// Find and set color sensor
    sock_col_sensor = sensor_search( LEGO_EV3_COLOR );
    if( sock_col_sensor )
    {
		if( color_set_mode_col_reflect( sock_col_sensor ) == 1 )
			printf( "Reflect mode set \n" );
    }
    else
	   printf("COLOR SENSOR NOT FOUND\n");

	// Find and set wheels
	sock_tacho_motor = tacho_search( LEGO_EV3_L_MOTOR );
	if( sock_tacho_motor )
	{
		max_speed = tacho_get_max_speed( MOTOR_LEFT, 0 );
		speed_linear = max_speed * SPEED_LINEAR / 100;
		duty_cycle_flag1 = tacho_set_duty_cycle_sp( MOTOR_BOTH, speed_linear );
		if( duty_cycle_flag == true )
		{
			tacho_run_direct(MOTOR_BOTH);
		}
	}
	
	while(true)
	{
		value0 = sensor_get_value0( sock_col_sensor, COLOR_COL_REFLECT);
		printf("Value obtained: %f \n", value0);

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
		int left_motor_speed = speed_linear - PID_value;
		int right_motor_speed = speed_linear + PID_value;
		duty_cycle_flag1 = tacho_set_duty_cycle_sp( MOTOR_LEFT,
		left_motor_speed);
		duty_cycle_flag2 = tacho_set_duty_cycle_sp( MOTOR_RIGHT,
		right_motor_speed);
		tacho_run_direct(MOTOR_BOTH);

		return 0;
 	}
}
