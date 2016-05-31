/* ###################################################################
**     Filename    : ProcessorExpert.c
**     Project     : ProcessorExpert
**     Processor   : MKL25Z128VLK4
**     Version     : Driver 01.01
**     Compiler    : GNU C Compiler
**     Date/Time   : 20xx-xx-xx, xx:xx, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file ProcessorExpert.c
** @version 01.01
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup ProcessorExpert_module ProcessorExpert module documentation
**  @{
*/         
/* MODULE ProcessorExpert */

/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "LED_Orange.h"
#include "LEDpin1.h"
#include "BitIoLdd1.h"
#include "WAIT1.h"
#include "sens_dreapta.h"
#include "BitIoLdd2.h"
#include "sens_stanga.h"
#include "BitIoLdd3.h"
#include "PWM_dreapta.h"
#include "PwmLdd1.h"
#include "TU1.h"
#include "PWM_stanga.h"
#include "PwmLdd2.h"
#include "Periodic_timer.h"
#include "TimerIntLdd1.h"
#include "TU2.h"
#include "CountTimer.h"
#include "A1.h"
#include "BitIoLdd4.h"
#include "A3.h"
#include "BitIoLdd5.h"
#include "D11.h"
#include "BitIoLdd6.h"
#include "A0.h"
#include "BitIoLdd7.h"
#include "A2.h"
#include "BitIoLdd8.h"
#include "D5.h"
#include "BitIoLdd9.h"
#include "Term1.h"
#include "Inhr1.h"
#include "ASerialLdd2.h"
#include "IR_LED.h"
#include "BitIoLdd10.h"
#include "Button.h"
#include "BitIoLdd11.h"
#include "PullupButton.h"
#include "Term2.h"
#include "Inhr2.h"
#include "ASerialLdd1.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

#include "MyCode.h"

#define TOLERANCE_S1 25
#define CORRECTION_STEP_S1 5
#define SPEED_MOTOR_S1_HIGH_REFERENCE 25
#define SPEED_MOTOR_S1_LOW_REFERENCE 50

#define TOLERANCE_S2 25
#define CORRECTION_STEP_S2 5
#define SPEED_MOTOR_S2_HIGH_REFERENCE 50
#define SPEED_MOTOR_S2_LOW_REFERENCE 25

#define FULL_SPEED 50

#define WAIT_SENSORS_1 200
#define WAIT_SENSORS_2 500 // us
#define WAIT_NORMAL 6000 // us

#define TIME_TO_STOP_ON_WHITE 150 /* cycles to wait till it stops */


/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
  /* Write your local variable definition here */
	
  uint16_t sensorsVector[NUMBER_OF_SENSORS];
  
  /* variables needed for deciding which direction it should turn more */
  uint16_t robotTurnsRightMotor;
  uint16_t robotTurnsLeftMotor;
  /* in case of a white detection - it may be that simply the robot got completely
   * out of the track - but it should get back to the truck in case it was
   * just loosing a bit the track
   */
    uint16_t flagStopWhite;
  
   /* correction algorith will relay on counting how bad
    * the curve (error), figuring out how many times the 
    * sensor is activated
    */ 
    
    uint16_t correctionS1;
    uint16_t correctionS2;
    
   
  
  uint16_t i; // for serial debug only
  
    flagStopWhite=0;
  
  
  /* initialize the correction counters 
   * they follow how often in a loop cycle the error appears
   */
  correctionS1 = 0;
  correctionS2 = 0;
  
  
  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */

  initializePlatform();
  
  /* Wait for a button press before doing anything*/
  while(Button_GetVal()) {}
  
  while (1)
  {

  	  /* Read the input sensors */
  	  readSensors(sensorsVector);

	  robotTurnsLeftMotor = sensorsVector[0] + sensorsVector[1] + sensorsVector[2];
	  robotTurnsRightMotor = sensorsVector[3] + sensorsVector [4] + sensorsVector[5];

	  
	  
#if 1 	  
  	  for (i = 0; i < NUMBER_OF_SENSORS; i++)
  	  {
  		  Term2_SendNum(sensorsVector[i]);
  		  Term2_SendChar(' ');
  	  }
  		  Term2_SendChar('R');
  		  Term2_SendChar('=');		  
  		  Term2_SendNum(robotTurnsRightMotor);
  		  Term2_SendChar(' ');
  		  Term2_SendChar('L');
  		  Term2_SendChar('=');		  
  		  Term2_SendNum(robotTurnsLeftMotor);
  		  Term2_CRLF();  	  
  	  

  	  
  		//setRightMotorSpeed(FULL_SPEED);
  		//setLeftMotorSpeed(FULL_SPEED);
  		  
  		  
  	  /* motor right turns faster case robot turns left as issue is on the left side  */
  	  if (robotTurnsRightMotor > robotTurnsLeftMotor){
  		  
  		/* if last sensor is seeing the black line 
  		 * is pretty bad so speed has to be pretty high
  		 * also if situation repeats correction must be more dramatic
  		 * */
  		  if (sensorsVector[0]==0 && sensorsVector[1]!= 0){ 
  			
  			  if(correctionS1 < TOLERANCE_S1)
  				correctionS1= correctionS1 + CORRECTION_STEP_S1;
  			   			  
  			  setRightMotorSpeed(FULL_SPEED);
  			  setRightMotorSpeed(SPEED_MOTOR_S1_HIGH_REFERENCE + correctionS1);
  			  setLeftMotorSpeed(SPEED_MOTOR_S1_LOW_REFERENCE - correctionS1);
  		      
  			  /* clean up correction flag for RS2*/
  			  WAIT1_Waitus(WAIT_SENSORS_1);
  			  
  			  correctionS2 = 0;
  		  }
  		     
  		  /* the case in which second sensor sees black line
  		   * less critical but also with potential of becoming an 
  		   * issue
  		   */
  		  else if( sensorsVector[1]==0){ 
  			
  			      if(correctionS2 < TOLERANCE_S2)
  				     correctionS2 = correctionS2 + CORRECTION_STEP_S2;
  			   			  
  			      setRightMotorSpeed(SPEED_MOTOR_S2_HIGH_REFERENCE + correctionS2);
  			      setLeftMotorSpeed(SPEED_MOTOR_S2_LOW_REFERENCE - correctionS2);
  			  
  			  
  			      /* clean up the correction flag for RS1 */
  			      correctionS1 = 0;
  			  
  			      WAIT1_Waitus(WAIT_SENSORS_2);
  		  
  		          }
  		  
  		  flagStopWhite=0;
  		    	  
  	  }
  	  
  	  /* motor left turns robot goes to right as issue is on the right side  */ 
  	  if ( robotTurnsLeftMotor > robotTurnsRightMotor){
   
  		/* if last sensor is seeing the black line 
  		 * is pretty bad so speed has to be pretty high
  		 * also if situation repeats correction must be more dramatic
  		*/
  		  if (sensorsVector[5]==0 && sensorsVector[4]!=0){ 
  		  			
  		  	  if(correctionS1 < TOLERANCE_S1)
  		  			correctionS1= correctionS1 + CORRECTION_STEP_S1;
  		  			   			  
  		  	  setRightMotorSpeed(SPEED_MOTOR_S1_LOW_REFERENCE - correctionS1);
  		  	  setLeftMotorSpeed(SPEED_MOTOR_S1_HIGH_REFERENCE + correctionS1);
  		  		  
  		      /* clean up correction flag for S2*/
  		  			  
  		  	  WAIT1_Waitus(WAIT_SENSORS_1); 
  		  	  correctionS2 = 0;
  		  	}
  		  
  		/* the case in which second sensor sees black line
  		  		  		   * less critical but also with potential of becoming an 
  		  		  		   * issue
  		  		  		   */
  		  
  		  else if (sensorsVector[4]==0){ 
  		  			
  		  			  if(correctionS2 < TOLERANCE_S2)
  		  				correctionS2 = correctionS2 + CORRECTION_STEP_S2;
  		  			   			  
  		  			  setRightMotorSpeed(SPEED_MOTOR_S2_LOW_REFERENCE - correctionS2);
  		  			  setLeftMotorSpeed(SPEED_MOTOR_S2_HIGH_REFERENCE + correctionS2);
  		  		    
  		  			  /* clean up the correction flag for RS1 */
  		  			  correctionS1 = 0;
  		  			
  		  			  WAIT1_Waitus(WAIT_SENSORS_2);
  		  	 
  		      }
  		  		  
  		   flagStopWhite=0;
  		  		    		  	  
  	  }
  	    	  
  	  
  	  //case line is correctly read and it should move full speed 
  	  if(robotTurnsRightMotor!=0 && robotTurnsLeftMotor != 0 && sensorsVector[2]==0 && sensorsVector[3]==0){
  		  
  		if(robotTurnsRightMotor==robotTurnsLeftMotor && robotTurnsRightMotor== 2){  
  			setRightMotorSpeed(FULL_SPEED);
  			setLeftMotorSpeed (FULL_SPEED);
  			
  		    WAIT1_Waitus(WAIT_NORMAL);
  		
  		    correctionS1 = 0;
  		    correctionS2 = 0;
  		    flagStopWhite=0;
  		}
	  }
  	  
  	  //case when it is on the black it also stops
  	  if (robotTurnsRightMotor==0 && robotTurnsLeftMotor == 0 ){
  		  
    		setRightMotorSpeed(0);
    		setLeftMotorSpeed (0);
    		
    		correctionS1 = 0;
   	        correctionS2 = 0;
  	        
  	        flagStopWhite=0;
  	  }
  	  
  	  // case when it is on white board not sensor read black it stops
  	  
  	  if (robotTurnsRightMotor==3 && robotTurnsLeftMotor == 3 ){
  		  
  		  if(flagStopWhite==TIME_TO_STOP_ON_WHITE){
  		   		setRightMotorSpeed(0);
    		    setLeftMotorSpeed (0);
  		  }
  		  else{
  			    flagStopWhite++;
  			    WAIT1_Waitus(WAIT_NORMAL);
  		  }

  	  }
#endif
  	    	   	  
  }

   
  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END ProcessorExpert */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.3 [05.08]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
