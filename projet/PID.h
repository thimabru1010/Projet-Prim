#include <stdio.h>
#include<stdlib.h>
#include "brick.h"
#include "ev3.h"
#include "ev3_light.h"
#define Sleep(ms) usleep(( ms ) * 1000 )
#define MOTOR_RIGHT OUTD
#define MOTOR_LEFT OUTA
#define MOTOR_BOTH ( MOTOR_LEFT | MOTOR_RIGHT )
// Percentage of max speed
#define SPEED_LINEAR 50
#define RED_MIN 65
#define RED_MAX 75

// Initialize the brick. Returns 1 if could'nt initialize 
int init_brick(void);

// Initialize and search for the color sensor. Set the sensor to the color mode. Returns the correspondent socket
POOL_T search_set_color_sensor(void);

// Search the large motor. Set the duty cycle sp mode for the motor.
void search_set_wheels(void);

// Makes de calculus of PID
// Receives the socket of color sensor.
// Returns true if the sensor finds the red color, otherwise if not.
bool calculate_pid(POOL_T sock_col_sensor);




