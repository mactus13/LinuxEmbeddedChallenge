/* ###################################################################
**     Filename    : MyCode.h
**     Project     : Line Follower
**     Processor   : MKL25Z128VLK4
**     Component   : Private
**     Version     : 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2014-04-29
**     Abstract    :
**         This is a quick implementation of the motor control.
**       **     Settings    :
**     Contents    :
**       **     Functions needed to control the motor and read the sensors
**
** ###################################################################*/
/*!
** @file MyCode.h
** @version 01.00
** @brief
**         This file contains all the functions needed to control the
**         motor and read the sensors.
**         
*/         

#ifndef __MyCode_H
#define __MyCode_H

#include <stdint.h>

#define NUMBER_OF_SENSORS 6 
//#define TIMEOUT_READING_SENSORS 100 /* 1 means 6.1us - CountTimer=163.84kHz */

#define BLACK_OR_WHITE_BOUNDRY 25 /* number of units of 6.1us to wait till deciding */

#define USEC_WAITING_FOR_CAPACITOR_TO_CHARGE 20

#define USEC_WAITING_INFRARED_LED_TO_START 200




/* some values for tolerance of error on sensors
 * measured in cycles of code execution, it is variable 
 * and depending on time of the main while loop
 */

/* golden values 

#define TOLERANCE_RS1 20
#define CORRECTION_STEP_RS1 10
#define SPEED_RIGHT_MOTOR_REFERENCE_RS1 75
#define SPEED_LEFT_MOTOR_REFERENCE_RS1 20

#define TOLERANCE_RS2 9
#define CORRECTION_STEP_RS2 3
#define SPEED_RIGHT_MOTOR_REFERENCE_RS2 90
#define SPEED_LEFT_MOTOR_REFERENCE_RS2 60

#define TOLERANCE_LS1 20
#define CORRECTION_STEP_LS1 10
#define SPEED_RIGHT_MOTOR_REFERENCE_LS1 20
#define SPEED_LEFT_MOTOR_REFERENCE_LS1 75

#define TOLERANCE_LS2 9
#define CORRECTION_STEP_LS2 3
#define SPEED_RIGHT_MOTOR_REFERENCE_LS2 60
#define SPEED_LEFT_MOTOR_REFERENCE_LS2 90

#define FULL_SPEED 90

*/


void initializePlatform();
void readSensors(uint16_t sensorsValue[NUMBER_OF_SENSORS]);
void setLeftMotorSpeed(uint16_t speed);
void setRightMotorSpeed(uint16_t speed);


#endif /* __MyCode_H */



















/* MODULE Private */

