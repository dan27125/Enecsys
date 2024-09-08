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



#if !defined(_Pic24_LED_PCB_H)
#define _Pic24_LED_PCB_H


#define P24_JST_LED2_TRIS	TRISBbits.TRISB1	// P24 RB1 p22 to LED2
#define P24_JST_LED2_LATCH	LATBbits.LATB1		// P24 RB1 p22 to LED2
#define P24_JST_SW2_PORT	PORTBbits.RB1		// P24 RB1 p22 to SW2

#define P24_JST_LED3_TRIS	TRISBbits.TRISB0	// P24 RB0 p21 to LED3
#define P24_JST_LED3_LATCH	LATBbits.LATB0		// P24 RB0 p21 to LED3


void P24_JST_PCB_MapPins();
void P24_JST_PCB_LED2_ON();
void P24_JST_PCB_LED2_OFF();
void P24_JST_PCB_LED2_Toggle();
void P24_JST_PCB_LED2_SET(char set);
#define	P24_JST_LED2_IS_ON()	 P24_JST_LED2_LATCH

void P24_JST_PCB_LED3_ON();
void P24_JST_PCB_LED3_OFF();
void P24_JST_PCB_LED3_SET(char set);
void P24_JST_PCB_LED3_Toggle();
void P24_JST_PCB_SW2_Task();
#define	P24_JST_LED3_IS_ON()	 P24_JST_LED3_LATCH

void P24_JST_PCB_SW2_ON();


#endif	// _Pic24_LED_PCB_H