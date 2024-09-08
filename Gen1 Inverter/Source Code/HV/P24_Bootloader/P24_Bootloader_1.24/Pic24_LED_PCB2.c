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




#if !defined(_Pic24_LED_PCB2_C)
#define _Pic24_LED_PCB2_C

#include "Pic24_Bootloader.h"
#include "pic24_ports.h"
#include "Pic24_LED_PCB.h"

#if defined( Uses_JST_Diag_PCB_SW2 )

void P24_JST_PCB_SW2_ON()
{
//	Put_CONSOLE("SW2");
//	cfg1.debug = !cfg1.debug;
	
//	Put_Str("SW2:");
//	Put_Dec(255);
//	Put_Str("TEST\n");
//	Put_Flush();
	
	if (cfg1.debug==0)
	{
		cfg1.debug = 1;
		Put_CONSOLE("DEBUG ON");
	}
	else
	{
		cfg1.debug = 0;
		Put_CONSOLE("DEBUG OFF");
	}
	
	P24_JST_PCB_LED3_Toggle();
}
#endif


#endif	// Pic24_LED_PCB2_C