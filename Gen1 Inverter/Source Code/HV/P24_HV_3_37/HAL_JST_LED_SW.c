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
#include "HAL_JST_LED_SW.h"
#include "HAL_Pic24_ports.h"

#if defined (Uses_JST_LED_SW)

void HAL_JST_Initialize()
{
	Pic24_IO_Unock();
	SW2_INPUT();
	LED_3_OUTPUT();
	Pic24_IO_Lock();
	HAL_JST_LED_2_ON();
	HAL_JST_LED_3_OFF();
}

void HAL_JST_LED_2_ON()
{
	jst.bit.led2=1;
	LED_2_LATCH = 1;		
	LED_2_OUTPUT();
}
void HAL_JST_LED_2_OFF()
{
	jst.bit.led2=0;
	LED_2_LATCH = 0;		
	LED_2_INPUT();
}

void HAL_JST_LED_2_Toggle()
{
	if (jst.bit.led2==0)
	{
		HAL_JST_LED_2_ON();
	}
	else
	{
		HAL_JST_LED_2_OFF();
	}
}

void HAL_JST_LED_3_ON()
{
	jst.bit.led3=1;
	LED_3_LATCH = 1;		
	LED_3_OUTPUT();
}
void HAL_JST_LED_3_OFF()
{
	jst.bit.led3=0;
	LED_3_LATCH = 0;
	LED_3_INPUT();
}

void HAL_JST_LED_3_Toggle()
{
	if (jst.bit.led3==0)
	{
		HAL_JST_LED_3_ON();
	}
	else
	{
		HAL_JST_LED_3_OFF();
	}
}

void HAL_JST_SW2_Task()
{
	Uint8 c;

	Pic24_IO_Unock();
	SW2_INPUT();
	c = SW2_PORT;
	SW2_OUTPUT();
	Pic24_IO_Lock();
	
	if (c==0)
	{
		jst.sw2.ms_off=0;
		
		if (jst.sw2.ms_on < 999)
			jst.sw2.ms_on++;
		
		if ((jst.sw2.ms_on >= 5) && (jst.sw2.bit.state==0))
		{
			jst.sw2.bit.state = 1;
			
			HAL_JST_LED_2_Toggle();
			
			if (++jst.sw2.cnt >= 3)
			{
				jst.sw2.bit.lock = !jst.sw2.bit.lock;
				
				if (jst.sw2.bit.lock)
					HAL_JST_LED_2_ON();
	//			else
	//				HAL_JST_LED_2_OFF();
				
				jst.sw2.cnt=0;
			}
		}
	}
	else
	{
		jst.sw2.ms_on=0;
		
		if (jst.sw2.ms_off < 999)
			jst.sw2.ms_off++;		
		
		if ((jst.sw2.ms_off >= 2) && (jst.sw2.bit.state==1))
		{
			jst.sw2.bit.state = 0;
		}
			
		if (jst.sw2.ms_off >= 200)
		{
			jst.sw2.cnt=0;
		}
	}
}

void HAL_JST_Task()
{
	if (jst.bit.ms10)
	{
		jst.bit.ms10 = 0;
		HAL_JST_SW2_Task();
	}
	
	if (jst.bit.ms500)
	{
		jst.bit.ms500 = 0;
		
//		if (!jst.sw2.bit.lock)
//			HAL_JST_LED_2_Toggle();
//		HAL_JST_LED_3_Toggle();
	}	

	if (jst.bit.sec)
	{
		jst.bit.sec = 0;
		
//		if (jst.sw2.bit.lock==0)
//			HAL_JST_LED_2_Toggle();
		
		HAL_JST_LED_3_Toggle();
	}	

}

#endif // #if defined (Uses_JST_LED_SW)