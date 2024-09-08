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



#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <libpic30.h> 
#include "Pic24_Bootloader.h"
#include "pic24_ports.h"
#include "Pic24_LED_PCB.h"

#include "HAL_Uart.h"

void task_sec()
{
	int seconds;
	
	secFlag++;
	secs++;
	
	seconds = secs % 60;
	mins = secs / 60;
	
	if (verify_app_boot_flag)	// Did bootloader update run and finish OK
	{
		Boot_App();				// Yes then boot application firmware
	}

	task_quit();
	task_Keep_Alive();
    

	#if defined(Uses_OTA_CONSOLE)
    if (cfg1.debug)
    {
    	Put_Dec(secs);
        Put_Str(" SECS\n");
    }
    
    #elif defined(Uses_JST_Diag_PCB_LED)

    if (seconds % 2)
    {   
        P24_JST_PCB_LED2_ON();
        P24_JST_PCB_LED3_ON();
    }
    else
    {
        P24_JST_PCB_LED2_OFF();
        P24_JST_PCB_LED3_OFF();
    }
    
//    P24_JST_PCB_LED3_Toggle();
	#endif
	
}

void task_1_ms()
{
	ticks_ms++;
	
	asm("clrwdt");		// clear WDT
    
    P24_Safe_Mode();    // Force safe mode

	#if defined(Uses_JST_Diag_PCB_SW2)
	if ( (ticks_ms%10)==0)
	{
		P24_JST_PCB_SW2_Task();	
	}	
	#endif

	if ( (ticks_ms%1000)==0)
	{
		ticks_ms = 0;
		task_sec();
	}	
	
}

void Set_Timer_32()
{
	//	24FJ64GA004 
	T2CON = 0x00; //Stops any 16/32-bit Timer2 operation
	T3CON = 0x00; //Stops any 16-bit Timer3 operation
	TMR3 = 0x00; //Clear contents of the timer3 register
	TMR2 = 0x00; //Clear contents of the timer2 register
	PR3 = 0;
	PR2 = 0;
	
	PR3	 = TIMER_PR3_1_MS;
	PR2	= TIMER_PR2_1_MS;
	
//	IPC2bits.T3IP = 0B111;
//	PR3	 = TIMER_PR3_1000_MS;
//	PR2	= TIMER_PR2_1000_MS;
	
	IFS0bits.T3IF = 0;
	T2CONbits.T32 = 1;
	T2CONbits.TON  = 1;
}
