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

#ifndef HAL_PERIPHERAL_CONFIGURATION_H
#define HAL_PERIPHERAL_CONFIGURATION_H

	// Timer control register bits. 
	#define DEF_TON         BIT_15
//	#define TSIDL           BIT_13
//	#define TGATE           BIT_6
//	#define TCKPS1          BIT_5
	#define TCKPS0          BIT_4
//	#define T32             BIT_3
//	#define TSYNC           BIT_2
//	#define TCS             BIT_1

	// Output compare control register bits.
//	#define OCSIDL          BIT_13
//	#define OCFLT           BIT_4
	#define OCTSEL          BIT_3
	#define OCM2            BIT_2
	#define OCM1            BIT_1
//	#define OCM0            BIT_0

//	#define INTER_PROCESSOR_BAUD_RATE   (115200)


	void HAL_ConfigureTimers(void);
	void HAL_ConfigureSpi(void);
	void HAL_ConfigureUart_1(void);

	#if defined( Uses_Uart2 )
	void HAL_ConfigureUart_2(void);
	#endif

	void HAL_ConfigureInterrupts(void);
	void HAL_P24_Bootloader_ISR_Enable();
	void HAL_P24_Bootloader_ISR_Disable();
	void APP_Bootloader_Task();

#endif 
