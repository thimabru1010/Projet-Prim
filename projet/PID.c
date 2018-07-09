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


