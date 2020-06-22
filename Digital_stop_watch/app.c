

/*--------------------------------------------------------------------------------------------------------------

 &[FILE NAME]    :adc.h

 &[AUTHOR(S)]    : Muhammed Eldabea Hashem

 &[DATE CREATED] : 20 June 2020

 &[DESCRIPTION]  :  definitions and configuration used foe ADC module in Atmega16

------------------------------------------------------------------------------------------------------------------*/

/* ===================================================================== *
          ----------------------->includes<---------------------------
 * ===================================================================== */



#include "app.h"







/* ===================================================================== *
       ---------------------->Global Variable<------------------------
 * ===================================================================== */


volatile unsigned char  Timerinterrupt_Flag = 0 ;
volatile unsigned char  Second_counter0 = 0 ;
volatile unsigned char  Second_counter1 = 0 ;
volatile unsigned char  Minuit_counter0 = 0 ;
volatile unsigned char  Minuit_counter1 = 0 ;
volatile unsigned char  Hours_counter0 = 0 ;
volatile unsigned char  Hours_counter1 = 0 ;
volatile unsigned char  stop_Flag = 0 ;
volatile unsigned char  resume_Flag = 0 ;



/* ===================================================================== *
       ----------------------> Configuration <------------------------
 * ===================================================================== */



/*Timer2 configuration*/
Timer2_Configuration Timer_config  = {Normal_mode,
		Normal_Port_OC2_Diconnected_FastPWM,FrqBY_1024_Prescaler
		,synch_clockSource} ;




/* ===================================================================== *
    ----------------------->Functions Definitions<---------------------
 * ===================================================================== */

void Flag_increeament(void)
{


	Timerinterrupt_Flag++ ;

}




void App_init()
{

	/*--------------------------------------------------------------------
	[FUNCTION NAME] :App_init
	[DESRIPTION]  : configuration needed modules like timer 2 and external
					interrupt that will be used
	[ARGUMENT(S)]  :
	   [IN]  : VOID
	   [OUT] : VOID
	[Return]      : VOID

	----------------------------------------------------------------------*/

	/*
	 * we have a a FCPU = 8000000
	 * and a pre-scaler = 1024
	 * that mean to make a 1 second we need a nearly  30 OVERFLOW
	 */



	/*initialize the timer with a specific configuration */
	Timer2_init(&Timer_config) ;


	/*
	 * now we will set the PORTA as output we
	 * we will need a 4 pins for decoder and a three pins for multiplexer
	 *so From PINA0 to PINA3 >> For Decoder
	 *   From PINA4 TO PINA6 >> For Multiplexer
	 */
	DDRA |=0x7f ;

	/*the next part in responsible for initialization
	  the external interrupts int0 and int1*/

	/*interrupt 0 */
	DDRD  &= ~(1<<2) ; /*set PIND2 as input  */
	/*configuration as rising edge of INT0
	 * so we a rising edge detected a interrupt
	 * request will be generated */

	MCUCR |=(1<<ISC01) ;
	MCUCR |=(1<<ISC00) ;

	/*interrupt 1 */
	DDRD  &= ~(1<<3) ; /*set PIND3 as input  */
	/*configuration as rising edge of INT1
	 * so we a rising edge detected a interrupt
	 * request will be generated */

	MCUCR |=(1<<ISC11) ;
	MCUCR |=(1<<ISC10) ;

	/*call the Flag_increeament with every timer OVF interrupt */
	Timer2_callBack(Flag_increeament) ;

}



void App_run(void )
{

	/*--------------------------------------------------------------------
		[FUNCTION NAME] :App_run
		[DESRIPTION]  : run the  main application code
		[ARGUMENT(S)]  :
		   [IN]  : VOID
		   [OUT] : VOID
		[Return]      : VOID

		----------------------------------------------------------------------*/

	if(stop_Flag==1) {
		Timer2_stop() ;
	}

	if(resume_Flag==1) {
		Timer2_count(&Timer_config) ;
	}

	if(Timerinterrupt_Flag==31)
	{
		Second_counter0++ ;
		Timerinterrupt_Flag=0 ;
	}

	if(Second_counter0==9){
		Second_counter1++;
		Second_counter0=0 ;
	}

	if(Second_counter1==6){
		Minuit_counter0++ ;
		Second_counter1=0 ;
	}

	if(Minuit_counter0==9){
		Minuit_counter1++;
		Minuit_counter0=0 ;
	}

	if(Minuit_counter1==6){
		Hours_counter0++ ;
		Minuit_counter1=0 ;
	}

	if(Hours_counter0==9){
		Hours_counter1++ ;
		Hours_counter0=0 ;
	}
	if(Hours_counter1==2){
		Hours_counter1++ ;
		Hours_counter0=0 ;
	}



	PORTA = (PORTA & 0x0f) | 0 ;
	PORTA = (PORTA & 0xf0) | (Second_counter0 & 0x0f) ;
	PORTA = (PORTA & 0x0f) | 1 ;
	PORTA = (PORTA & 0xf0) | (Second_counter1 & 0x0f) ;
	PORTA = (PORTA & 0x0f) | 2 ;
	PORTA = (PORTA & 0xf0) | (Minuit_counter0 & 0x0f) ;
	PORTA = (PORTA & 0x0f) | 3 ;
	PORTA = (PORTA & 0xf0) | (Minuit_counter1 & 0x0f) ;
	PORTA = (PORTA & 0x0f) | 4 ;
	PORTA = (PORTA & 0xf0) | (Hours_counter0 & 0x0f) ;
	PORTA = (PORTA & 0x0f) | 5 ;
	PORTA = (PORTA & 0xf0) | (Hours_counter1 & 0x0f) ;





}









