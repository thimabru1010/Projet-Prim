#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "brick.h"
#define Sleep(ms)  usleep(( ms ) * 1000 )
#define MOTOR_RIGHT OUTD
#define MOTOR_LEFT OUTA
#define MOTOR_BOTH ( MOTOR_LEFT | MOTOR_RIGHT )
#define SPEED_LINEAR 20

int main(void)
{
	int max_speed_left, max_speed_right, speed_linear, duty_cycle;
	int duty_cycle_sp, meters;
	bool duty_cycle_flag, time_flag, speed_flag, abs_flag;
	POOL_T sock_tacho_motor;
	if( !brick_init() )
		return (1);
	else
		printf( "Brick initialized \n");
	sock_tacho_motor = tacho_search( LEGO_EV3_L_MOTOR );
	// Output sockets OUTA = 16, OUTB = 32, ..., OUTD = 128. When more than one
	// is plugged the result is the sum. Ex: OUTA + OUTD = 144
	printf(" Motor socket number: %u \n", sock_tacho_motor);
	if( sock_tacho_motor )
	{
		max_speed_left = tacho_get_max_speed( MOTOR_LEFT, 0);
		max_speed_right = tacho_get_max_speed( MOTOR_RIGHT, 0);
		printf(" Motor left max speed: %d	Motor right max speed: %d \n", max_speed_left, max_speed_right);
		//The maximum speed is the same fot both motors. It's impossible the
		//motor overpass the maximum speed. So we multiply the maximum speed by
		//a small number to make the car runs slower.
		speed_linear = max_speed_left * SPEED_LINEAR / 100;
		tacho_set_speed_sp( MOTOR_BOTH, speed_linear);
		// No need to set a command
//		tacho_set_command( MOTOR_BOTH, TACHO_RUN_FOREVER );
		//Makes the robot runs forever
		printf("Run forever running...\n");
		tacho_run_forever( MOTOR_BOTH );
		Sleep(2000);
		// The speed of the motor don't changes while he is running
		speed_linear = max_speed_left * 70 / 100;
		tacho_stop( MOTOR_BOTH );
		printf("Waiting... \n");
		Sleep(2000);
		// But with duty_cycle it's possible to change the speed
		duty_cycle_flag = tacho_set_duty_cycle_sp( MOTOR_BOTH, 20);
		if( duty_cycle_flag == true )
		{
			tacho_run_direct(MOTOR_BOTH);
			Sleep(2000);
			tacho_set_duty_cycle_sp(MOTOR_BOTH, 70);
			printf("Duty cycle running...\n");
			Sleep(2000);
			tacho_stop(MOTOR_BOTH);
		}
		// Makes the car run during a time with specific speed
		time_flag = tacho_set_time_sp( MOTOR_BOTH, 4000);
		speed_flag = tacho_set_speed_sp(MOTOR_BOTH, max_speed_left);
		if( time_flag == true && speed_flag == true)
		{
			printf("Run timed running... \n");
			tacho_run_timed(MOTOR_BOTH);
		}
		// Makes the car run determinated meters 
		meters = tacho_get_count_per_rot(MOTOR_RIGHT, 1);
		printf("Meters: %d \n", meters);
		tacho_set_position_sp(MOTOR_BOTH, meters);
		Sleep(2000);
		abs_flag = tacho_run_to_abs_pos(MOTOR_BOTH);
		if( abs_flag == true )
			printf("Abs pos running... \n");
		Sleep(2000);
		tacho_set_stop_action_coast(MOTOR_BOTH);
		tacho_stop(MOTOR_BOTH);
	}
	return 0;
}
