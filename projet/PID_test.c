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
	float value0, target, error = 0, prop, integral = 0, derivate = 0, PID_value;
	float last_error = 0, kp = 0.45, ki = 0.0008, kd = 0.9;
	int duty_cycle, duty_cycle_sp;
	bool duty_cycle_flag1, duty_cycle_flag2;
	POOL_T sock_col_sensor, sock_tacho_motor;
	
	// Setting a vlue for the target
	target = 40;

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
		printf("Wheels motors found\n");
		duty_cycle_flag1 = tacho_set_duty_cycle_sp( MOTOR_BOTH, SPEED_LINEAR );
		if( duty_cycle_flag1 == true )
		{
			printf("Motor running...\n\n");
			tacho_run_direct(MOTOR_BOTH);
		}
	}
	else
		perror("tacho_search");

	int cont = 0;
	float left_motor_speed = 0, right_motor_speed = 0;

	while(true)
	{
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
		duty_cycle_flag1 = tacho_set_duty_cycle_sp( MOTOR_LEFT, left_motor_speed);
		duty_cycle_flag2 = tacho_set_duty_cycle_sp( MOTOR_RIGHT, right_motor_speed);
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

		cont++;
		
		// Makes the car stops, because he doesn't stops when the programs
		// breaks up 
		if( cont == 400 )
		{
			tacho_stop( MOTOR_BOTH );
			return 0;
		}
 	}
}
