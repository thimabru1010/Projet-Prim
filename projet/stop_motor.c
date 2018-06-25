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
	tacho_stop(MOTOR_BOTH);
	return 0;
}
