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



#if !defined(_Pic24_LED_PCB1_C)
#define _Pic24_LED_PCB1_C


	#include "Pic24_Bootloader.h"
	#include "pic24_ports.h"
	#include "Pic24_LED_PCB.h"




#if defined( Uses_JST_Diag_PCB_LED )


void P24_JST_PCB_MapPins()
{
//	CONFIG_RP1_AS_DIG_PIN();        // RX RP pin must be digital
	CONFIG_RB1_AS_DIG_INPUT();		// RX RB1 pin must be input to JST 1
//	P24_JST_PCB_LED2_OFF();
	
	CONFIG_RP0_AS_DIG_PIN();        // TX RP pin must be digital
	CONFIG_RB0_AS_DIG_OUTPUT();		// RX RB0 pin must be input to JST 2
	P24_JST_PCB_LED3_OFF();
}

void P24_JST_PCB_LED2_ON()
{
	P24_JST_LED2_LATCH = 1;
	P24_JST_LED2_TRIS = 0;
}

void P24_JST_PCB_LED2_OFF()
{
	P24_JST_LED2_LATCH = 0;
	P24_JST_LED2_TRIS = 1;
}

void P24_JST_PCB_LED2_SET(char set)
{
	if (set)
		P24_JST_PCB_LED2_ON();
	else
		P24_JST_PCB_LED2_OFF();
}

void P24_JST_PCB_LED2_Toggle()
{
//	if (P24_JST_LED2_LATCH==0)
	if (P24_JST_LED2_IS_ON()==0)
	{
		P24_JST_PCB_LED2_ON();
	}
	else 
	{
		P24_JST_PCB_LED2_OFF();
	}
}

void P24_JST_PCB_LED3_ON()
{
	P24_JST_LED3_LATCH = 1;
	P24_JST_LED3_TRIS = 0;
}

void P24_JST_PCB_LED3_OFF()
{
	P24_JST_LED3_LATCH = 0;
	P24_JST_LED3_TRIS = 1;
}

void P24_JST_PCB_LED3_SET(char set)
{
	if (set)
		P24_JST_PCB_LED3_ON();
	else
		P24_JST_PCB_LED3_OFF();
}

void P24_JST_PCB_LED3_Toggle()
{
	if (P24_JST_LED3_IS_ON()==0)
	{
		P24_JST_PCB_LED3_ON();
	}
	else 
	{
		P24_JST_PCB_LED3_OFF();
	}
}

#endif


#if defined(Uses_JST_Diag_PCB_SW2)
void P24_JST_PCB_SW2_Task()
{
	static WORD onCnt = 0;
	static WORD offCnt = 0;	
	
        
	if (P24_JST_SW2_PORT)
	{
		if (onCnt==0)
		{
			if (offCnt >20)
				onCnt++;
			return;
		}
		onCnt++;
		if (onCnt==100)
		{
			P24_JST_PCB_SW2_ON();
			onCnt++;
		}
	}
	else
	{
		onCnt=0;
		if (offCnt < 0xffff)
		{
			offCnt++;
		}
	}

}
#endif


#endif	// Pic24_LED_PCB1_C