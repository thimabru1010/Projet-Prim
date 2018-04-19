#include "brick.h"
#include <stdlib.h>
#include <stdio.h>
#define Sleep(ms)  usleep(( ms ) * 1000 )


int main(void)
{
	POOL_T sock_uart_sensor;
	float value0;
	printf("Waiting the EV3 brick online...\n");
	if( !brick_init() )
		return( 1 );
	printf("*** (EV3) Hello! ***\n");
	ev3_sensor_init();
	sock_uart_sensor = sensor_search( LEGO_EV3_US );
	if( sock_uart_sensor )
	{
		if( us_set_mode_us_dist_cm( sock_uart_sensor ) == 1 )
			printf("Continous measurement distance mode set (cm) \n");
	}
	else
		printf("Uart sensor not found \n");
	while(true)
	{
		value0 = sensor_get_value0( sock_uart_sensor, US_US_DIST_CM );
		printf("Value obtained: %f \n", value0);

		if( value0 <= 50 )
			return 0;
		Sleep(4000);
	}

	return 0;
}
