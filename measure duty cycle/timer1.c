/*====================================================================================================
[FILE NAME]  : timer1.c

[AUTHOR(S)]  : Muhammed Eldabea Hashem

[DATE CREATED] 22 April 2020


[DESCRIPTION] :this file contain the implementation of functions related to the Timer1 module

=====================================================================================================*/



/* ========================================================================================= *
          ----------------------------------->includes<------------------------------
 * ========================================================================================= */

#include "timer1.h"


/* ========================================================================================= *
       ----------------------------------->Global Variable<------------------------------
 * ========================================================================================= */

static volatile void (*GptrToFunc)(void) = (void*)0 ;

/* ========================================================================================= *
    ----------------------------------->Functions Definitions<------------------------------
 * ========================================================================================= */

#if Timer1_interrupt_enable==1
ISR(TIMER1_CAPT_vect)
{
	if(GptrToFunc != (void*)0)
	{
		(*GptrToFunc)() ;
	}
}


ISR(TIMER1_COMPA_vect)
{
	if(GptrToFunc != (void*)0)
	{
		(*GptrToFunc)() ;
	}

}
ISR(TIMER1_COMPB_vect)
{
	if(GptrToFunc != (void*)0)
	{
		(*GptrToFunc)() ;
	}
}

ISR(TIMER1_OVF_vect)
{
	if(GptrToFunc != (void*)0)
	{
		(*GptrToFunc)() ;
	}
}

#endif


void timer1_init(Timer1_configuration *ptrToStruct)
{

	/*--------------------------------------------------------------------------------------
	[FUNCTION NAME] :timer1_init

	[DESRIPTION]    :used to initialize the timer1 module

	[ARGUMENT(S)]  :
	   [IN]  : a pointer to structure with Timer1_configuration type
	   [OUT] :  void
	[Return]      : void

	---------------------------------------------------------------------------------------*/

	TCCR1A = (TCCR1A & 0xEf) | ((ptrToStruct->Compare_Output_Mode &0x01)<<4) ; /*COM1B0*/
	TCCR1A = (TCCR1A & 0xBf) | ((ptrToStruct->Compare_Output_Mode &0x01)<<6) ; /*COM1A0*/
	TCCR1A = (TCCR1A & 0xDf) | ((ptrToStruct->Compare_Output_Mode &0x02)<<4) ; /*COM1B1*/
	TCCR1A = (TCCR1A & 0x7f) | ((ptrToStruct->Compare_Output_Mode &0x02)<<6) ; /*COM1A1*/
	TCCR1A = (TCCR1A & 0xfc) | (ptrToStruct->Waveform_Generation_Mode &0x03) ; /*WGM11 / WGM10*/
	TCCR1B = (TCCR1B & 0xE7) | ((ptrToStruct->Waveform_Generation_Mode&0x18)<<1); /*WGM12/WGM13*/
	TCCR1B = (TCCR1B & 0xf7) | (ptrToStruct->ClockSelect&0x07) ;        /*CS 10:12 Clock select  */
#if Input_Capture_Noise_Canceler==1

	TCCR1B|=(1<<7) ; /*enable Input Capture Noise Canceler*/
#endif


	if((ptrToStruct->Waveform_Generation_Mode==CTC_ICR1_) || (ptrToStruct->Waveform_Generation_Mode==CTC_OCR1)||(ptrToStruct->Waveform_Generation_Mode==Normal_mode) )
	{

		/*The FOC1A/FOC1B bits are only active when the WGM13:0 bits specifies a non-PWM mode*/
		TCCR1A |=(1<<3) ; /*FOC1A*/
		TCCR1A |=(1<<2) ; /*FOC1B*/

	}

#if Timer1_interrupt_enable ==1
	SREG|=(1<<7) ; /*enable the Global  interrupt */
	switch (ptrToStruct->Timer1_interrupt_handler)
	{
	case (InputCapture_ICF1) :
																		TIMSK|=(1<<TICIE1) ; /*enable the input capture interrupt */
	break ;

	case (Output_Compare_A_Match_OCF1A) :
																	TIMSK |=(1<<OCIE1A) ; /*enable the output compare A interrupt */
	break ;
	case (Output_Compare_B_Match_OCF1B) :
																TIMSK |=(1<<OCIE1B) ; /*enable the output compare B interrupt */
	break ;
	case (OverFlow_TOV1) :
															TIMSK |=(1<<TOIE1) ; /*enable the timer1 overflow interrupt*/
	}


#endif




}



void Timer1_set_initalvalue(unsigned char initial_value)
{

	/*--------------------------------------------------------------------------------------
	[FUNCTION NAME] :Timer1_set_initalvalue

	[DESRIPTION]    :used to set the initial value for Timer1

	[ARGUMENT(S)]  :
	   [IN]  : unsigned char initial_value >> the value by which the counter will start
	   [OUT] :  void
	[Return]      : void

	---------------------------------------------------------------------------------------*/

	TCNT1=initial_value ; /*set the initial value */

}

void Timer1_SetOutCompare_A_Register(unsigned short OC_value)
{

	/*--------------------------------------------------------------------------------------
	[FUNCTION NAME] :Timer1_SetOutCompare_A_Register

	[DESRIPTION]    :used to set the compared value  work with mode output compare A

	[ARGUMENT(S)]  :
	   [IN]  : unsigned short OC_value >> the compared  value
	   [OUT] :  void
	[Return]      : void

	---------------------------------------------------------------------------------------*/

	OCR1A=OC_value ;
}

void Timer1_SetOutCompare_B_Register(unsigned short OC_value)
{

	/*--------------------------------------------------------------------------------------
	[FUNCTION NAME] :Timer1_SetOutCompare_B_Register

	[DESRIPTION]    :used to set the compared value  work with mode output compare B

	[ARGUMENT(S)]  :
	   [IN]  : unsigned short OC_value >> the compared  value
	   [OUT] :  void
	[Return]      : void

	---------------------------------------------------------------------------------------*/


	OCR1B=OC_value ;
}

unsigned short  Timer1_ICU_ReadICR(void)
{

	/*--------------------------------------------------------------------------------------
	[FUNCTION NAME] :Timer1_ReadICR

	[DESRIPTION]    :read the value of the ICR register

	[ARGUMENT(S)]  :
	   [IN]  : void

	[Return]      : unsigned short >>the value of ICR

	---------------------------------------------------------------------------------------*/


	return ICR1 ; /*return the value of the ICR*/
}

void Timer1_polling(unsigned char interrupt_flag)
{

	/*--------------------------------------------------------------------------------------
	[FUNCTION NAME] :Timer1_callBack

	[DESRIPTION]    :used to make a callback to ISR function using the address of function

	[ARGUMENT(S)]  :
	   [IN]  : a pointer to function that will be called with interrupt happened
	   [OUT] :  void
	[Return]      : void

	---------------------------------------------------------------------------------------*/

	while(!(TIFR &(1<<interrupt_flag))) {}
	TIFR|=(1<<interrupt_flag) ; /*be sure  that the flag bit is clear */

}

void Timer1_callBack(void(*ptrFun)(void))
{

	/*--------------------------------------------------------------------------------------
	[FUNCTION NAME] :Timer1_callBack

	[DESRIPTION]    :used to make a callback to ISR function using the address of function

	[ARGUMENT(S)]  :
	   [IN]  : a pointer to function that will be called with interrupt happened
	   [OUT] :  void
	[Return]      : void

	---------------------------------------------------------------------------------------*/
	GptrToFunc=ptrFun ;
}

/*
 * Description : configuration for the selected edge
 * */
void Timer1_ICU_selectheClock(unsigned char edge)
{
	TCCR1B = (TCCR1B & 0xBF)  | ((edge &0x01)<<6) ;
}


void Timer1_ICU_clearICR()
{
	/*
	 * used to clear the ICR register value
	 * */
	ICR1 = 0 ;
}


void Icu_clearTimerValue(void)
{
	TCNT1 = 0;
}

void Timer1_Deinit(void)
{
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;
	ICR1 = 0;
}




