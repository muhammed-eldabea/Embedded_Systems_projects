/*-------------------------------------------------------------------------------------
[FILE NAME]  : timer1.c

[AUTHOR(S)]  : Muhammed Eldabea Hashem

[DATE CREATED] 22 April 2020


[DESCRIPTION] :this file contain the implementation of functions related to the LCD module

-----------------------------------------------------------------------------------------*/
#ifndef TIMER1_H_
#define TIMER1_H_


/*=============================================> INCLUDES <=====================================================*/
#include "avr/io.h"
#include "avr/interrupt.h"
#include "avr/delay.h"
/*=====================================> Definitions and configuration <=========================================*/

#define InputCaptureMode  1
#define Input_Capture_Noise_Canceler 0
#define Timer1_interrupt_enable      1

/*=================================================enums=========================================================*/

typedef enum
{
	InputCapture_ICF1,Output_Compare_A_Match_OCF1A,Output_Compare_B_Match_OCF1B,OverFlow_TOV1
}Timer1_interrupt;


typedef enum
{ /*configure the rising by which the IC module will capture */
	falling_IC_edge,Rissing_IC_edge
}ICedgeSelect;
typedef enum
{/*operation  of the OC1A/OC1B with every toggle or interrupt */
	Normal_port_operation_OC1A_OC1Bdisconnected,Toggle_OC1A_OC1B_on_compare_match,Clear_OC1A_OC1B_on_compare_match,Set_OC1A_OC1B_on_compare_match
}Compare_Output_Mode;


typedef enum
{ /*configure the mode by which the timer will mode */
	Normal_mode,PWM_timer1, Phase_Correct_8_bit_timer1,Phase_Correct_9_bit_timer1,Phase_Correct_10_bit_timer1,
	CTC_OCR1,Fast_PWM_Correct_8_bit_timer1,Fast_PWM_Correct_9_bit,Fast_PWM_Correct_10_bit ,PWM,Phase_and_Frequency_Correct_ICR1_bottomTobottom,
	Phase_and_Frequency_Correct_OCR1A_TopTobottom,CTC_ICR1_,Timer1_reserved_WGM,Fast_PWM_ICR1_ToptoTop,Fast_PWM_OCR1A_ToptoTop
}Waveform_Generation_Mode;

typedef enum {
	/*define the clock source  as we can scale the frequency pf the timer module */
	Timer1_NoClockSource,Timer1_clockby1,Timer1_clockby8,Timer1_clockby64,Timer1_clockby256,Timer1_clockby1024,
	Timer1_externalClockSource_fallingEdge,Timer1_externalClockSource_risingEdge
}ClockSelect;

typedef enum {
	Timer1_ICU_fallingEdge ,Timer1_ICU_RaissingEdge
}Timer1_ICU_edge;



/*===========================================> Structure and Unions <================================================*/


typedef struct
{
	/*----------------------------------------------------------------------------------------
	[Structure Name]  :  Timer1_configuration
	[Description]     :  contain all possible configuration of timer1
	                     like Compare output mode ,waveform generation mode and clock source select


	-----------------------------------------------------------------------------------------*/

	unsigned char Compare_Output_Mode ;  /*configure the operation of OC pin when an interrupt or match event is happen  */
	unsigned char Waveform_Generation_Mode ; /*configure the mode by which the timer will run */
	unsigned char ClockSelect ; /*pre-scaler value of frequency that  timer will run */
	unsigned char Timer1_interrupt_handler ; /*configure the interrupt source */
}Timer1_configuration;



/*================================================> Macros <========================================================*/


/*=========================================> Function Declarations <===================================================*/

void timer1_init(Timer1_configuration *ptrToStruct) ;
void Timer1_set_initalvalue(unsigned char initial_value) ;
void Timer1_SetOutCompare_A_Register(unsigned short OC_value) ;
void Timer1_SetOutCompare_B_Register(unsigned short OC_value) ;
unsigned short  Timer1_ICU_ReadICR(void) ;
void Timer1_polling(unsigned char interrupt_flag) ;
void Timer1_callBack(void(*ptrFun)(void)) ;
void Timer1_ICU_selectheClock(unsigned char edge)  ;
void Timer1_ICU_clearICR()  ;
void Icu_clearTimerValue(void) ;
void Timer1_Deinit(void) ;
#endif /* TIMER1_H_ */
