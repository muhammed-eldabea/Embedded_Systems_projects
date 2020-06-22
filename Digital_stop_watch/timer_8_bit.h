
/*--------------------------------------------------------------------------------------------------------------

 &[FILE NAME]    : timer_8_bit.h

 &[AUTHOR(S)]    : Muhammed Eldabea Hashem

 &[DATE CREATED] : 20 April 2020

 &[DESCRIPTION]  :  definitions and configuration used foe Timer2 module in Atmega16

------------------------------------------------------------------------------------------------------------------*/




#ifndef TIMER_8_BIT_H_
#define TIMER_8_BIT_H_





/*---------------------------------------> INCLUDES <---------------------------------------*/

#include  <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>

/*-------------------------------> Definitions and configuration <-------------------------*/

typedef unsigned char uint8   ;

#define Interrupt_enable    1
#define Null  ((void*)0)



typedef enum {
	/*wave generation mode of the timer
	 * the mode by which the timer will work */

	Normal_mode , PWM_PhaseCoorect_mode , CTC_mode , Fast_PWM_mode
}Wave_Generation_mode;

typedef enum {
	/* the action that will be taken when output compare action
	   happen when non PWM is selected  */
	Normal_Port_OC2_Diconnected_NON_PWM , Toggle_OC2_on_compare_match_NON_PWM,
	Clear_OC2_on_Compare_match_Non_PWM,Set_OC2_match_Non_PWM
}Compare_output_mode_non_PWM_mode;

typedef enum {
	/*the action that will be taken when output compare action
	   happen when Fast PWM is selected  */
	Normal_Port_OC2_Diconnected_FastPWM , Toggle_OC2_on_compare_match_FastPWM,
	Clear_OC2_on_Compare_match_FastPWM,Set_OC2_match_FastPWM
}Compare_output_mode_Fast_PWM_mode;

typedef enum {
	/*select the frequency prescaler */
	NO_Clock_source,FrqBY_0_NoPrescaler,FrqBY_8_Prescaler,FrqBY_32_Prescaler,
	FrqBY_64_Prescaler,FrqBY_128_Prescaler,FrqBY_256_Prescaler,
	FrqBY_1024_Prescaler


}Clock_presclaer;

typedef enum {
	/*select the clock source by which the timer will work */
	synch_clockSource,Asynch_clockSource

}Clock_Source;



/*--------------------------> Structure and Unions <--------------------------*/
typedef struct
{

	/*=========================================================================
	[Structure Name]  : Timer2_Configuration
	[Description]     : Select the configuration of the timer2
	 ==========================================================================*/

	uint8 Wave_generation_mode:2 ; /*select the mode by which the timer will work */
	uint8 Compare_output_mode :2 ; /* the operation of the oc2 when output compare */
	uint8 Frequency_prescaler :3 ; /*control the frequency that will be used */
	uint8 Async_enable        :1 ; /*Asynchronous Timer/Counter2*/


}Timer2_Configuration;

/*-------------------------------> Macros <------------------------------------*/


#define BIT_IS_CLEAR(REG,BIT) ( !(REG & (1<<BIT)) )

/*-------------------------> Function Declarations <----------------------------*/


void Timer2_init(Timer2_Configuration *ptrStruct ) ;
void Timer2_OVF_Polling() ;
void Timer2_CTC_Polling() ;
void Timer2Clear () ;
void Timer2_SetCompareValue(uint8 value) ;
void Timer2_Setinitail_Value(uint8 value) ;
void Timer2_callBack(void(*ptr)(void)) ;
void Timer2_stop() ;
void Timer2_count(Timer2_Configuration*ptr) ;

#endif /* TIMER_8_BIT_H_ */
