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


#include "APP_Inverter.h"






#if defined(DOCUNMENTATION)

#endif



void APP_Bootloader_Reset()
{
//	inverter_off_flag = 0;		// DGA clear flag 
	app.gRemoteCommand = OFF;		// DGA clear flag 
	APP_Stop_Mode();		// DGA Bootloader Reset
	asm("RESET");				// DGA trigger a reset instruction 	
}


void APP_Bootloader_Task()
{
	if (IO_PIN_BOOTLOADER_PORT)
	{
		APP_Bootloader_Reset();
	}
	
}

void __attribute__((interrupt,auto_psv)) _INT2Interrupt(void)
{
//	HAL_IoPortConfiguration.c
//	CONFIG_RB11_AS_DIG_INPUT();		// P24 p09 RB11 input from  EM250 I/O-10 (ETRX p30) 
//	CONFIG_INT2_TO_RP(11);			// CONFIG_INT2_TO_RP(pin) _INT2R = pin
	
	if (IO_PIN_BOOTLOADER_PORT)
	{
		APP_Bootloader_Reset();
	}
	
	HAL_ISR2_BOOT_LOAD_IF = 0;	// DGA reset interrupt
}



//	#endif