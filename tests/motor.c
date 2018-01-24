#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "brick.h"
#define Sleep(ms)  usleep(( ms ) * 1000 )
#define MOTOR_RIGHT OUTD
#define MOTOR_LEFT OUTA
#define MOTOR_BOTH ( MOTOR_LEFT | MOTOR_RIGHT )
#define SPEED_LINEAR 75

int main(void)
{
	int max_speed_left, max_speed_right, speed_linear;
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
		printf(" Motor left max speed: %d	Motor right max speed: %d \n",
		max_speed_left, max_speed_right);
		speed_linear = max_speed_left * SPEED_LINEAR / 100;
		tacho_set_speed_sp( MOTOR_BOTH, speed_linear);
		tacho_set_command( MOTOR_BOTH, TACHO_RUN_FOREVER );
		tacho_run_forever( MOTOR_BOTH );
		Sleep(4000);
	}
	return 0;
}
