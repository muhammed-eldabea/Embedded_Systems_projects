/******************************************************************************
 *[Author]     	: Muhammed eldabea Hashem
 *
 *[filename]   	: timer_8_bit.c
 *
 *[Date]       	: 9 June 2020
 *
 *[Description] : contain all function implementation that used in 8 bit timer
 *					in  ATmega 16
 *
 ******************************************************************************/



/*=============================================================================
 * 									Include
 *=============================================================================*/

#include "timer_8_bit.h"

/*=============================================================================
 * 								Global Variable
 *=============================================================================*/

/*a global pointer to function used in timer2 ISR callBack*/
static volatile void (*ptrFunc)(Void) = Null ;

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/




ISR(TIMER2_COMP_vect)
{
	/*=========================================================================
		[Function Name]  : ISR(TIMER2_COMP_vect)
		[Description]    : Interrupt service routine for compare match
	==========================================================================*/


	/*check if the pointer hold no address */
	if(ptrFunc != Null) {
		(*ptrFunc)() ;
	}
}




ISR (TIMER2_OVF_vect)
{
	/*=========================================================================
		[Function Name]  : ISR(TIMER2_OVF_vect)
		[Description]    : Interrupt service routine for overFlow
	==========================================================================*/


	/*check if the pointer hold no address */
	if(ptrFunc != Null) {
		(*ptrFunc)() ;
	}

}





/*=============================================================================
 * 							Function Definition
 *=============================================================================*/


void Timer2_init(Timer2_Configuration *ptrStruct )
{

	/*==========================================================================
	[FUNCTION NAME] : Timer2_init

	[DESRIPTION]    : initialize the timer2 module be a given configuration
					  send in a structure

	[ARGUMENT(S)]  :
	   [IN]  : pointer to Timer2_Configuration Structure

	   [Return] : void


	============================================================================*/

	if (ptrStruct->Async_enable == Asynch_clockSource ){

		ASSR |= (1<<3) ; /*set Asynchronous Timer/Counter2 */
		/* To switch to asynchronous operation: Wait for TCN2UB, OCR2UB, and
	   TCR2UB */
		while((BIT_IS_CLEAR(ASSR,2)) &&
				(BIT_IS_CLEAR(ASSR,1)) &&
				(BIT_IS_CLEAR(ASSR,0)) ) {}


	}

	TCCR2 = (TCCR2 & 0xf8) | (ptrStruct->Frequency_prescaler  & 0x07) ; /*CLK*/
	TCCR2 = (TCCR2 & 0xBf) | ((ptrStruct->Wave_generation_mode & 0x01)<<6) ;/*WGM20*/
	TCCR2 = (TCCR2 & 0xf7) | ((ptrStruct->Wave_generation_mode & 0x02)<<2) ;/*WGM21*/
	TCCR2 = (TCCR2 & 0xCf) | ((ptrStruct->Compare_output_mode & 0x03)<<4) ; /*COM*/
	
	if(ptrStruct->Wave_generation_mode != PWM_PhaseCoorect_mode)
	{
		TCCR2 |= (1<<7) ; /*enable FOC2*/
	}


#if Interrupt_enable ==1
	SREG |= (1<<7) ;
	switch (ptrStruct->Wave_generation_mode)
	{
	case Normal_mode :
		TIMSK |= (1<<6) ; /*Timer/Counter2 Overflow Interrupt Enable*/
		break ;
	case CTC_mode :
		TIMSK |= (1<<7) ; /*Timer/Counter2 CTC Interrupt Enable*/
		break ;


	}

#endif

	TCNT2 = 0 ;
	OCR2 = 0 ;

}




void Timer2_OVF_Polling()
{
	/*=========================================================================
		[Function Name]   : Timer2_OVF_Polling
		[Description]     : polling to the overflow flag
	 ==========================================================================*/


	while(BIT_IS_CLEAR(TIFR,6)){}
	TIFR |= (1<<6) ; /*clear the flag */
}

void Timer2_CTC_Polling()
{
	/*=========================================================================
		[Function Name]   : Timer2_CTC_Polling
		[Description]     : polling to the CTC flag
     ==========================================================================*/

	while(BIT_IS_CLEAR(TIFR,7)){}
	TIFR |= (1<<7) ; /*clear the flag */
}


void Timer2Clear ()
{
	/*=========================================================================
			[Function Name]   : Timer2clear
			[Description]     : clear timer2 register
     ==========================================================================*/

	TCNT2 = 0 ;
	OCR2  = 0 ;

}



void Timer2_SetCompareValue(uint8 value)
{
	/*=========================================================================
			[Function Name]   : Timer2_SetCompareValue
			[Description]     : set the compare value
	 ==========================================================================*/

	OCR2 = value ;
}

void Timer2_Setinitail_Value(uint8 value)
{
	/*=========================================================================
			[Function Name]   : Timer2_Setinitail_Value
			[Description]     : set the initial  value
	 ==========================================================================*/

	TCNT2 = value ;
}



void Timer2_callBack(void(*ptr)(void))
{

	/*=========================================================================
			[Function Name]   : Timer2_callBach
			[Description]     : function will be called in ISR
			[Arguments]       : a pointer to void function
     ==========================================================================*/


	ptrFunc =ptr ;
}


void Timer2_stop()
{
	/*used to stop the timer by prevent the clock to reach */

	TCCR2 = (TCCR2 & 0xf8) | (0x00 & 0x07) ; /*CLK*/
}

void Timer2_count(Timer2_Configuration*ptr)
{
	/*resume counting*/
	TCCR2 = (TCCR2 & 0xf8) | (ptr->Frequency_prescaler  & 0x07) ; /*CLK*/
}
