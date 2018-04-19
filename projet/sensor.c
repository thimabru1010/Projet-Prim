#include "brick.h"
#include<stdio.h>
#include<stdlib.h>
#include "ev3_light.h"
#include "ev3.h"
#include <unistd.h>
#include<string.h>
#define Sleep(ms)  usleep(( ms ) * 1000 )
#define RGB COLOR_RGB_RAW


int main(void)
{
    uint8_t col_sensor;
    bool flag;
    char buf[256], *addr, sens_mode[4];
    byte buf2[256], *bin_data;
    float value0, value1, value2;
    uint8_t port_inx;
    POOL_T sock_col_sensor;
    printf( "Waiting the EV3 brick online...\n" );
    if ( !brick_init() ) 
	   return ( 1 );
    printf( "*** ( EV3 ) Hello! ***\n" );
    ev3_sensor_init();
    // sensor_search returns the number of the socket that is connected to the
    // sensor. The enumerations can be seen on the brick library page. Ex: IN4 =
    // 8, IN3 = 4, etc...
    sock_col_sensor = sensor_search( LEGO_EV3_COLOR );
    printf("Number: %u \n", sock_col_sensor );
    if( sock_col_sensor )
    {
		printf("Type the mode you wish \n");
		printf(" ->ref<- for reflect mode or ->rgb<- for rgb-raw mode \n"); 
		scanf("%s", sens_mode);
		printf("String: %s \n", sens_mode);
		if( strcmp(sens_mode, "ref") == 0 )
		{
			if( color_set_mode_col_reflect( sock_col_sensor ) == 1 )
				printf( "Reflect mode set \n" );
		}
		if( strcmp(sens_mode, "rgb") == 0 )
		{
			if( color_set_mode_rgb_raw( sock_col_sensor ) == 1 )
				printf( "RGB-RAW mode set \n");
		}
    }
    else
	   printf("COLOR SENSOR NOT FOUND\n");
	
	// Shows the adress of the sensor. Ex: in4, in3, in2, ....   
    addr = sensor_get_address( sock_col_sensor, buf, sizeof(buf));
    printf(" Sensor adress: %s \n", addr);

	// Don't know yet
    bin_data = sensor_get_bin_data( sock_col_sensor, buf2, sizeof(buf2) );
    printf("Bin data Value: %hu \n", bin_data);

	// Shows the port of the socket
    port_inx = socket_to_port( sock_col_sensor );
    printf("Port of socket: %u \n", port_inx );
    
	// Loop to make the sensor don't stop
    while(true)
    {
		if( strcmp( sens_mode, "ref" ) == 0 )
		{
			value0 = sensor_get_value0( sock_col_sensor, COLOR_COL_REFLECT);
			printf(" Value obtained: %f \n", value0);
		}
		if( strcmp( sens_mode, "rgb" ) == 0 )
		{
			value0 = sensor_get_value0( sock_col_sensor, RGB );
			value1 = sensor_get_value1( sock_col_sensor, RGB );
			value2 = sensor_get_value2( sock_col_sensor, RGB );
			printf(" Red: %f	Green: %f	Blue: %f \n", value0, value1,
			value2);
		}

		if( strcmp( sens_mode, "ref" ) == 0 )
		{
			 if( value0 < 10 )
			 {
				 set_light_blink( LIT_LEFT, LIT_RED, 1000, 500 );
				 Sleep( 3000 );
			 }
			 else
			 {
				 set_light( LIT_LEFT, LIT_RED );
				 Sleep( 2000 );
			 }
			 if( value0 >= 80 )
			 {
				 printf(" Program finished \n");
				 set_light(LIT_LEFT, LIT_GREEN );
				 brick_uninit();
				 return 0;
			 }
			 Sleep( 4000 );
		}
		if( strcmp( sens_mode, "rgb" ) == 0 )
		{
			if ( value0 >= 200 & value1 < 10 & value2 < 10 )
				printf("Color: Red \n");
			if( value0 <= 10 & value1 >= 200 & value2 <= 10 )
				printf("Color: green\n");
			if( value0 <= 10 & value1 <= 10 & value2 >= 200 )
				printf("Color: blue \n");
			if( value0 <= 10 & value1 <= 10 & value2 <= 10 )
				printf("Color: black \n");
			if( value0 >= 200 & value1 >= 200 & value2 >= 200 )
			{
				printf("Color: white \n");
				printf("Exiting... \n");
				return 0;
			}
			Sleep( 4000 );
		}
	}
}
			
