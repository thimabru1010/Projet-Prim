#include <stdio.h>
#include "brick.h"
#include "stdlib.h"
#include "ev3_light.h"
#include <unistd.h>
#include<string.h>
#define Sleep(ms)  usleep(( ms ) * 1000 )
/* enum {SOCKET__NONE_ = 0, IN1 = 0x1L, IN2 = 0x2L, IN3 = 0x4L, 
  IN4 = 0x8L, OUTA = 0x10L, OUTB = 0x20L, OUTC = 0x40L, 
  OUTD = 0x80L 
}

enum { LIT_LEFT, LIT_RIGHT, LIT__LOC__ }

enum { 
      LIT_OFF, LIT_GREEN, LIT_RED, LIT_AMBER, 
        LIT__COL__ 
}

EV3_LIGHT_VAR_INIT (const char *const LIT_COLOR[],{"OFF","GREEN","RED","AMBER", STR_unknown_})
*/

int main(void)
{
    uint8_t l_color, trigger;
    char *color_name;
    ev3_init();
    brick_init();
    set_light( LIT_LEFT, LIT_OFF);
    Sleep( 500 );
    set_light_blink( LIT_LEFT, LIT_AMBER, 1000, 500);
    Sleep( 5000 );
    set_light( 0, LIT_AMBER );
    l_color = get_light( 0 );
    printf("Unknow number: %u \n", l_color);
    color_name = (char*)malloc( strlen(light_color(l_color)) * sizeof(char) ); 
    strcpy( color_name, light_color(l_color) );
    printf(" Actual Color: %s \n", color_name);
    Sleep( 1000 );
    set_light( LIT_LEFT, LIT_RED);
    Sleep( 500 );
    if( "RED" == light_color(LIT_LEFT) );
        set_light( LIT_LEFT, LIT_GREEN );
    trigger = get_light_trigger( LIT_RIGHT, 3 );
    printf( "Trigger: %u \n", trigger );
    set_led_trigger( 1,"MAX" );
   // set_light_trigger( LIT_RIGHT, 3, trigger); 
    free( color_name);
    ev3_uninit();
    brick_uninit();
    return 0;
}

/* OBSERVATIONS:

the function get_light returns an interger of 8 bits, which value is referenced
to the light color agreeing with the ENUM order, 0 for OFF, 1 for GREEN and
etc..

Also, can be used on the array LIT_COLOR[], listed above, that has all the light
colors. Besides, the function "light_color" returns one of the names of this
array and not a LIT_X name 

For the locations the integers works the same way
*/

 









