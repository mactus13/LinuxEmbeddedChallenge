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
**       **     Functions needed to control the motor
**
** ###################################################################*/

#include "MyCode.h"
#include "PWM_dreapta.h"
#include "PWM_stanga.h"
#include "sens_stanga.h"
#include "sens_dreapta.h"
#include "CountTimer.h"
#include "A1.h"
#include "A3.h"
#include "D11.h"
#include "A0.h"
#include "A2.h"
#include "D5.h"
#include "IR_LED.h"
#include "WAIT1.h"


/* definition and mapping of the sensors done below in the following mapping:
 * 
 *  | LS1 | LS2 | LS3 | RS3 | RS2 | RS1 |
 *  
 *  LS1 - Left Sensor position 1 (most exterior left)
 *  
 *  ....
 *  
 *  RS1 - Right Sensor position 1 (most exterior right)
 *  
 */


#define LS1_SetDir(input) A1_SetDir(input)
#define LS1_GetVal() A1_GetVal()
#define LS1_SetVal() A1_SetVal()

#define LS2_SetDir(input) A3_SetDir(input)
#define LS2_GetVal() A3_GetVal()
#define LS2_SetVal() A3_SetVal()

#define LS3_SetDir(input) D11_SetDir(input)
#define LS3_GetVal() D11_GetVal()
#define LS3_SetVal() D11_SetVal()

#define RS3_SetDir(input) A0_SetDir(input)
#define RS3_GetVal() A0_GetVal()
#define RS3_SetVal() A0_SetVal()

#define RS2_SetDir(input) A2_SetDir(input)
#define RS2_GetVal() A2_GetVal()
#define RS2_SetVal() A2_SetVal()

#define RS1_SetDir(input) D5_SetDir(input)
#define RS1_GetVal() D5_GetVal()
#define RS1_SetVal() D5_SetVal()

/* definition of the vector containing values corresponding to the sensors
 * sensorsValue[n] = 0 if LSn/RSn reads black
 * sensorsValue[n] = 1 if LSn/RSn reads white
 *   
 */
uint16 sensorsValue[NUMBER_OF_SENSORS];

void initializePlatform(){
	
/* Initialize the PWMs that will be used to driver the motors*/
	PWM_dreapta_SetRatio16(0xFFFF); // 0xFFFF = duty cycle 0%
	PWM_stanga_SetRatio16(0xFFFF);
	PWM_dreapta_Enable();
	PWM_stanga_Enable();
	
}

/*
** ===================================================================
** 
**     Component   :  Private
**     Description :
**         read sensors
**     Parameters  : boolean string with 
**     				1 - white
**     				0 - black
**     Returns     : Nothing
** ===================================================================
*/

void readSensors(uint16_t sensorsValue[NUMBER_OF_SENSORS]){
	
	/*used for reading how long it take for the line to fall to 0
	 *and this way detecting the black or white level
	 */	
	uint16_t counterReadValue; 
	uint16_t i;
	
	
/* start the infrared LED */
	IR_LED_SetVal();
	
//	WAIT1_Waitus(USEC_WAITING_INFRARED_LED_TO_START);
		
/* init the vector to reflecting by default
 * it will be populated with values in case reflection is discovered
 * - just a convention 0 means black and 1 means white */
	
	for (i=0;i<NUMBER_OF_SENSORS;i++){
			sensorsValue[i]=0;
	}
		
/* set sensors as output and put them into 1 */
	LS1_SetDir(TRUE); 
	LS1_SetVal();
	
	A1_SetDir(TRUE);
	A1_SetVal();
	
	LS2_SetDir(TRUE); 
	LS2_SetVal();
	
	LS3_SetDir(TRUE); 
	LS3_SetVal();

	RS3_SetDir(TRUE); 
	RS3_SetVal();

	RS2_SetDir(TRUE); 
	RS2_SetVal();
	
	RS1_SetDir(TRUE); 
	RS1_SetVal();
	
/* wait until sensors capacitor is fully up */
	WAIT1_Waitus(USEC_WAITING_FOR_CAPACITOR_TO_CHARGE);

/* set all sensors as inputs to read them */
	
	LS1_SetDir(FALSE); 
	LS2_SetDir(FALSE); 
	LS3_SetDir(FALSE); 
	RS3_SetDir(FALSE); 
	RS2_SetDir(FALSE); 
	RS1_SetDir(FALSE); 

/* read the time needed for the value of the sensor line to reach 0
 * the longer it takes the less light it is reflected
 */

/* reset of the counter (using periods of 6.1 usec) used to count time
 * sensor needs till it line is falling to 0
 */

	CountTimer_ResetCounter((LDD_TDeviceData *)NULL);
	
/* effective reading of the value of the line + decision if it is black or white
 * the sensor is seeing
 */
	counterReadValue = CountTimer_GetCounterValue((LDD_TDeviceData *)NULL);
	
	while (counterReadValue < BLACK_OR_WHITE_BOUNDRY){
	/* do nothing wait till enough time has passed
	 * after this time has passed the sensors should be
	 * in correct shape	 */
		counterReadValue = CountTimer_GetCounterValue((LDD_TDeviceData *)NULL);
		
	}
	
	/* basically if level did not reach 0 which means it is black 
	 * and that is recorded in the sensors vector with a value of 1 
	 * a bit tricky and maybe confusing here are they are opposite */
	
	
	if(LS1_GetVal()==0){ 
		sensorsValue[0] = 1;  
	}

	if(LS2_GetVal()==0){ 
		sensorsValue[1] = 1;  
	}
	
	if(LS3_GetVal()==0){ 
		sensorsValue[2] = 1;  
	}
	if(RS3_GetVal()==0){ 
		sensorsValue[3] = 1;
	}
	if(RS2_GetVal()==0){ 
		sensorsValue[4] = 1; 
	}
	if(RS1_GetVal()==0){  
		sensorsValue[5] = 1; 
	}
				
	/* shut down the LED emitting infrared - need to see if this step is needed 
	 * here decision taken not to shut down the sensor 
	 * */
//	IR_LED_ClrVal();
	
}


/*
** ===================================================================
** 
**     Component   : Private
**     Description : Simple function basically gets a percentage of 
**     				 max speed from 1 to 100
**     
**     Parameters  : speed
**     Returns     : Nothing
** ===================================================================
*/

void setLeftMotorSpeed(uint16 speed){
	
	uint16_t value;
	
	if (speed > 100)
		value = 100;
	else
		value = speed;
	
	value = 0xFFFF - (value*0xFFFF)/100;
	
	sens_stanga_PutVal(0);
	PWM_stanga_SetRatio16(value);
		
}


/*
** ===================================================================
** 
**     Component   : Private
**     Description : Simple function basically gets a percentage of 
**     				 max speed from 1 to 100
**     
**     Parameters  : speed
**     Returns     : Nothing
** ===================================================================
*/

void setRightMotorSpeed(uint16 speed){
	
	uint16_t value;
	
	if (speed > 100)
		value = 100;
	else
		value = speed;
	
	value = 0xFFFF - (value*0xFFFF)/100;
	
	sens_dreapta_PutVal(0);
	PWM_dreapta_SetRatio16(value);
		
}



