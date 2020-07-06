/*========================================================================
 *  File name  : duty_cycle.c
 *  Created on: Jul 6, 2020
 *  Author: Muhammed eldabea Hashem
 *========================================================================*/



/* ========================================================================================= *
          ----------------------------------->includes<------------------------------
 * ========================================================================================= */
#include "timer1.h"
#include  "lcd.h"

/* ========================================================================================= *
       ----------------------------------->Global Variable<------------------------------
 * ========================================================================================= */
#define InputCaptureMode  1
#define Input_Capture_Noise_Canceler 0
#define Timer1_interrupt_enable      1

volatile unsigned short Timer3 = 0 ;
volatile unsigned short Timer2 = 0 ;
volatile unsigned short Timer1 = 0 ;
//volatile unsigned short Duty_Cycle = 0 ;
volatile unsigned char g_counter = 0 ;

/* ========================================================================================= *
    ----------------------------------->Functions Definitions<------------------------------
 * ========================================================================================= */








void App_proccecing(void)
{

	g_counter++ ;
	if(g_counter==1)
	{
		Timer1_ICU_clearICR() ;
		Timer1_ICU_selectheClock(falling_IC_edge) ;

	}
	else if (g_counter==2)
	{
		Timer1 = Timer1_ICU_ReadICR() ;
		Timer1_ICU_selectheClock(Rissing_IC_edge) ;
	}
	else if (g_counter==3)
	{
		Timer2 = Timer1_ICU_ReadICR() ;
		Timer1_ICU_selectheClock(falling_IC_edge) ;
	}
	else if (g_counter==4)
	{
		Timer3 = Timer1_ICU_ReadICR() ;
		Icu_clearTimerValue() ;
		Timer1_ICU_selectheClock(Rissing_IC_edge) ;
	}



}



int  main ()
{
	unsigned short Duty = 0 ;
	DDRD &= ~(1<<PD6);

	/*set the needed configuration*/
	Timer1_configuration config = {Normal_port_operation_OC1A_OC1Bdisconnected,
			Normal_mode,Timer1_clockby1,InputCapture_ICF1} ;

	Timer1_ICU_selectheClock(Rissing_IC_edge) ;
Timer1_callBack(&App_proccecing) ;
	LCD_init() ;
	timer1_init(&config) ;


 while (1)
 {
	 if (g_counter==4)
	 {
		 Timer1_Deinit() ;
		 g_counter = 0  ;
		 Duty = ((float)(Timer3-Timer2)/(Timer3-Timer1)) *100 ;
		 lcd_GotoRowANDcoulm(1, 5) ;
		 LCD_DisplayInteger(Duty) ;
		 _delay_ms(5000) ;
		 LCD_clearSreen() ;

	 }

 }

}
