// ********************************************************************************************************
//	Enecsys Gen 1 Micro Inverter Firmware 
//
//	MIT License
//
//  Copyright 2016-2024 Daniel G. Ambrose
//
//	Daniel G. Ambrose
//	12614 Bradford Woods Dr
//	Sunset Hills, Missouri, USA 63127
//	Email: dan27125@gmail.com
//	Facetime: dambrose@amcominc.net
//	Mobile:   3145044858
//	Whatsapp: 3145044858
//
// 	Permission is hereby granted, free of charge, to any person obtaining a copy of this software and 
//	associated documentation files (the "Software"), to deal in the Software without restriction, 
//	including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
//	and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
//	subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included in all copies or substantial 
//	portions of the Software.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//	SOFTWARE.
//
// ********************************************************************************************************


#if !defined(  _HAL_P24_Task2_ )
#define _HAL_P24_Task2_

#include "APP_Inverter.h"

void APP_delay_miliseconds(int ms)
{
	Uint16 timer5_delay_ticks = ms * 6;
	timer.delay = 0;
		
	while(timer.delay <  timer5_delay_ticks)
	{
//		void HAL_EEPROM_spi_isr();
//		HAL_EEPROM_spi_isr();
        #if defined (Uses_P24_WDT)
        WDT_Reset();
        #endif
	}
}

Uint16 CALC_TMR1_US(Uint16 tmr)
{
//	25000 / 1.5 = 16666	
//	25000 / 6 * 4		
	return (tmr / 6) * 4;
}



#if defined(RFU_SAVED_CODE)
void hal_delay_us(Uint16 us)
{
	Uint16 timer5_delay_ticks;

//	Timer 5 is programmed for 6k ticks per second. 
//	One timer5 tick equals 16.6666 US 
	
	if (us < 16)
		timer5_delay_ticks = 1;
	else
		timer5_delay_ticks = us / 16;
	timer.delay = 0;
	while(timer.delay <  timer5_delay_ticks)
	{
		
	}
}
Uint16 CALC_TMR1_US(Uint16 tmr)
{
//	25000 / 1.5 = 16666	
//	25000 / 6 * 4		
	return (tmr / 6) * 4;
}


Uint8 P24_IS_CRITICAL_TIMING()
{
	// ARE WE ABOUT TO MEASURE AC VOLTAGE
//    if (0 == IO_PIN_GRID_PHASE)
//	{
//		if ( (grid_sample_flag) || (grid_flag))
//			return 1;
//	}
	
	return 0;
}
Uint16 CALC_TMR1_MSX10(Uint16 tmr)
{
	Uint16 us;
	us = CALC_TMR1_US(tmr);
	return us / 100;
}

Uint16 CALC_TMR1_MS(Uint16 tmr)
{
	Uint16 us;
	us = CALC_TMR1_US(tmr);
	return us / 1000;
}

Uint8 CALC_TMR1_MSD1(Uint16 tmr)
{
	Uint16 ms;
	ms = CALC_TMR1_MSX10(tmr);
	return ms % 10;
}

#endif
		
void APP_State_Set(InverterState newState)
{
	if (system_state != newState)
	{
		system_state = newState;
		rcb1.state_set=1;
	}
}



#endif