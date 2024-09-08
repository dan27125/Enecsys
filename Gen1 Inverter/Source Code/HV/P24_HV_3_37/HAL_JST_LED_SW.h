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


#ifndef HAL_LED_H
#define HAL_LED_H

	typedef union {
		struct 
		{
			unsigned led2:1;
			unsigned led3:1;			
			unsigned sec:1;	
			unsigned ms500:1;
			unsigned ms100:1;
			unsigned ms10:1;
		};
		struct 
		{
			unsigned char value;
	   };
	} JST_Bits;

	typedef union {
		struct 
		{
			unsigned state:1;
			unsigned lock:1;
		};
		struct 
		{
			unsigned char value;
	   };
	} JST_SW_Bits;

	typedef struct 
	{
		Uint16 ms_on;
		Uint16 ms_off;
		JST_SW_Bits bit;
		Uint8 cnt;
	} SWITCH_CONTROL;

	typedef struct 
	{
		SWITCH_CONTROL sw2;
		JST_Bits bit;
	} JST_CONTROL_BLOCK;
	
	public volatile JST_CONTROL_BLOCK jst;
	
	#define JST_1_PORT		PORTBbits.RB1
	#define JST_1_LATCH		LATBbits.LATB1
	#define JST_1_INPUT()	CONFIG_RB1_AS_DIG_INPUT()
	#define JST_1_OUTPUT()	CONFIG_RB1_AS_DIG_OUTPUT()

	#define JST_2_PORT		PORTBbits.RB0
	#define JST_2_LATCH		LATBbits.LATB0
	#define JST_2_INPUT()	CONFIG_RB0_AS_DIG_INPUT()
	#define JST_2_OUTPUT()	CONFIG_RB0_AS_DIG_OUTPUT()

//	#define	SW1_LATCH		JST_5_LATCH
//	#define	SW1_PORT		JST_5_PORT

	#define	SW2_LATCH		JST_1_LATCH
	#define	SW2_PORT		JST_1_PORT	
	#define SW2_OUTPUT()	JST_1_OUTPUT()
	#define SW2_INPUT()		JST_1_INPUT()
	
	#define LED_2_LATCH		JST_1_LATCH
	#define LED_2_OUTPUT()	JST_1_OUTPUT();
	#define LED_2_INPUT()	JST_1_INPUT()
		
	#define LED_3_LATCH		JST_2_LATCH
	#define LED_3_INPUT()	JST_2_INPUT()
	#define LED_3_OUTPUT()	JST_2_OUTPUT()

	void HAL_JST_Initialize();
	void HAL_JST_Task();
	void HAL_JST_LED_2_ON();
	void HAL_JST_LED_2_OFF();
	void HAL_JST_LED_2_Toggle();
	void HAL_JST_LED_3_ON();
	void HAL_JST_LED_3_OFF();
	void HAL_JST_LED_3_Toggle();
	

#endif 