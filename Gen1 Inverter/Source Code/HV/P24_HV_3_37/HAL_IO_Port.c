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

/*
//	PIC24FJXXGA004 
-----------------------
01	SDA1/RP9/CN21/PMD3/RB9
02	RP22/CN18/PMA1/RC6
03	RP23/CN17/PMA0/RC
04	RP24/CN20/PMA5/RC8
05  RP25/CN19/PMA6/RC9
06	DISVREG
07	VCAP/VDDCORE
08	PGED2/RP10/CN16/PMD2/RB10
09	PGEC2/RP11/CN15/PMD1/RB11		EM250 Pic24 Update
10	AN12/RP12/CN14/PMD0/RB12
11	AN11/RP13/CN13/PMRD/RB13
12	TMS/PMA10/RA10
13	TCK/PMA7/RA7		// zigbee reset
14	AN10/CVREF/RTCC/RP14/CN12/PMWR/RB14
15	AN9/RP15/CN11/PMCS1/RB15
16	AVSS
17	
18	MCLR
19	AN0/VREF+/CN2/RA0
20	AN1/VREF-/CN3/RA1
21	PGED1/AN2/C2IN-/RP0/CN4/RB0
22	PGEC1/AN3/C2IN+/RP1/CN5/RB1
23	AN4/C1IN-/SDA2/RP2/CN6/RB2
24	AN5/C1IN+/SCL2/RP3/CN7/RB3
25	AN6/RP16/CN8/RC0
26	AN7/RP17/CN9/RC1
27	AN8/RP18/CN10/PMA2/RC2
28	VDD
29	VSS
30	OSCI/CLKI/CN30/RA2
31	OSCO/CLKO/CN29/RA3
32	TDO/PMA8/RA8
 34	SOSCO/T1CK/CN0/RA4
35	TDI/PMA9/RA9
36	RP19/CN28/PMBE/RC3
37	RP20/CN25/PMA4/RC4
38	RP21/CN26/PMA3/RC5
39	VSS
40	VDD
41	PGED3/RP5/ASDA1/CN27/PMD7/RB5
42	PGEC3/RP6/ASCL1/CN24/PMD6/RB6
43	RP7/INT0/CN23/PMD5/RB7
44	SCL1/RP8/CN22/PMD4/RB8
 */


void MapPins_Zigbee_UART1()
{
    CONFIG_RP23_AS_DIG_PIN();              // RX RP pin must be digital
    CONFIG_RP24_AS_DIG_PIN();              // TX RP pin must be digital

	CONFIG_RC7_AS_DIG_INPUT();
	CONFIG_RC8_AS_DIG_OUTPUT();
 
    CONFIG_U1RX_TO_RP(23);                 // U1RX <- RP23 pin 3
    CONFIG_U1TX_TO_RP(24);                 // U1TX -> RP24 pin 4
}

#if defined(Uses_UART2)
void MapPins_JST_UART2()
{
	
// JST Plug   JST1->Red      to USB->White
// JST Plug   JST2->Green    to USB->Green
// JST Plug   JST3->Yellow   to USB->Black

	CONFIG_RP1_AS_DIG_PIN();              // RX RP pin must be digital
    CONFIG_RP0_AS_DIG_PIN();              // TX RP pin must be digital
	
	CONFIG_RB1_AS_DIG_INPUT();				//RX RB1 pin must be input to JST 1
	CONFIG_RB0_AS_DIG_OUTPUT();				//RX RB0 pin must be input to JST 2
	
	CONFIG_U2RX_TO_RP(1);                 //U2RX <- RP1 / RB1 Pin 22 to JST 1 (USB Green)	
	CONFIG_U2TX_TO_RP(0);                 //U2TX -> RP0 / RB0 Pin 21 to JST 2 (USB white)
	
//	#if defined( Uses_Uart2_Opto_Board )
//	U2MODEbits.RXINV = 0;	
//	U2STAbits.UTXINV = 0;
//	#endif


}
#endif

/*******************************************************************/
/* Function:    HAL_ConfigurePorts                                 */
/* Description: Perform port configuration.                        */
/* Side effects:changes port control registers.                    */
/*******************************************************************/

void HAL_ConfigurePorts()
{
	Pic24_IO_Unock();
	
// OLD CODE THAT CLEARS EVERYTHING	
	LATA			  = 0x0000;
	LATB              = 0x0000;
	LATC              = 0x0000;
	PORTA             = 0x0000; 
	PORTB             = 0x0000;
	PORTC             = 0x0000; 
//	AD1PCFG           = 0x00BF;	// AN9..AN12, AN8, AN6 analogue, the rest digital
//	AD1PCFG           = 0x00BF;	// AN9..AN12, AN8, AN6 analogue, the rest digital	
	
//	TRISAbits.TRISA0  = IO_DIRECTION_INPUT;
	CONFIG_RA0_AS_DIG_INPUT();   // p19  VEE
	
//	TRISAbits.TRISA1  = IO_DIRECTION_INPUT;
	CONFIG_RA1_AS_DIG_INPUT();   // p20  VEE
	
//	TRISAbits.TRISA10 = IO_DIRECTION_OUTPUT;
	CONFIG_RA10_AS_DIG_OUTPUT();				// p12 RA10 UFB_ENABLE_LIVE_LOW

//	TRISBbits.TRISB4  = IO_DIRECTION_OUTPUT;
	CONFIG_RB4_AS_DIG_OUTPUT();					// p33 RB4 UFB_ENABLE_LIVE_HI
	
//	TRISBbits.TRISB1  = IO_DIRECTION_OUTPUT;	// p22 RB1 JST p1	

//	TRISBbits.TRISB2  = IO_DIRECTION_OUTPUT;	// 
	CONFIG_RB2_AS_DIG_OUTPUT();					// PIC P23 RB2 SINE_TEMPLATE   PWM 
//	RPOR1bits.RP2R    = 18;						// sine PWM output 
//	CONFIG_OC1_TO_RP(2);						// PIC P23 RP2 to Sine PWM output OC1 
  	CONFIG_OC2_TO_RP(2);						// PIC P23 RP2 to Sine PWM output OC1 
	
//	TRISBbits.TRISB3  = IO_DIRECTION_OUTPUT;	//  
	CONFIG_RB3_AS_DIG_OUTPUT();					// PIC P24 RP3 to PR3/OC3 SQR PWM clock 
//	CONFIG_OC3_TO_RP(3);						// p24 RP3 to OC3 PWM SQR clock 
	CONFIG_OC3_TO_RP(3);						// p24 RP3 to OC3 PWM SQR clock     
	
//	TRISBbits.TRISB5  = IO_DIRECTION_OUTPUT;	// p41 RB5 
	CONFIG_RB5_AS_DIG_OUTPUT();					// P41 RB5  IO_PIN_BUCK_ENABLE
	
//	TRISBbits.TRISB6  = IO_DIRECTION_INPUT; // SPI data in
//	TRISBbits.TRISB7  = IO_DIRECTION_INPUT;
	CONFIG_RB7_AS_DIG_INPUT();					// P43 RB7  GRID_INTERRUPT
	
//	TRISBbits.TRISB8  = IO_DIRECTION_INPUT;
	CONFIG_RB8_AS_DIG_INPUT();					// p44 RB8   GRID_PHASE
	
//	#if defined(Uses_Console) && defined(Uses_Diagnostics)
	CONFIG_INT1_TO_RP(8);
//	#endif
	
//	TRISBbits.TRISB9  = IO_DIRECTION_OUTPUT;	// 
	CONFIG_RB9_AS_DIG_OUTPUT();					// p01 RB9   GRID_PHASE
	
//	TRISBbits.TRISB10 = IO_DIRECTION_OUTPUT;
//	CONFIG_RB10_AS_DIG_OUTPUT();				// p08 RB10   NC
	
//	TRISBbits.TRISB12 = IO_DIRECTION_INPUT;  // dc link sensing, analog
	CONFIG_AN12_AS_ANALOG();				 // p10 AN12  dc link sensing, analog (voltage only))
	
//	TRISBbits.TRISB13 = IO_DIRECTION_INPUT; // mains grid voltage sensing, analog
	CONFIG_AN11_AS_ANALOG();				 // p11 AN11/RB13 grid voltage sensing, analog 
	
//	TRISBbits.TRISB14 = IO_DIRECTION_INPUT; // temperature sensor, analog  not fitted
//	CONFIG_AN10_AS_ANALOG();				// p14 AN10/RB14 temperature sensor, analog

//************************************************************************************	
//	TRISBbits.TRISB15 = IO_DIRECTION_INPUT; // second temperature sensor, analogue
	CONFIG_AN9_AS_ANALOG();					// p15 AN9/RB15		5v ps2
//************************************************************************************		


//	TRISCbits.TRISC0  = IO_DIRECTION_INPUT;  // (Pin 25) 3V3 supply monitor. 	SVC_SelfCheck.c		CONFIG_RC0_AS_DIG_OUTPUT()
	CONFIG_RC0_AS_DIG_INPUT();				// p25    RC0/AN6   3V3 supply monitor. 	SVC_SelfCheck.c	
			
//	TRISCbits.TRISC1  = IO_DIRECTION_OUTPUT;
//	CONFIG_RC1_AS_DIG_OUTPUT();				// p26 RC1	Not used
	
//	TRISCbits.TRISC2  = IO_DIRECTION_INPUT;  // (Pin 27) 5V supply monitor. 		CONFIG_RC2_AS_DIG_OUTPUT		CONFIG_RC2_AS_DIG_OUTPUT
//	CONFIG_RC2_AS_DIG_INPUT();				// p27 RC2/AN8
	
 

// *********************************************************************************	
//  BOOT  LOADER
//	DGA - Jan 26, 2019  EM250 drives RB11 as P24 input for bootloader 	
	CONFIG_RP11_AS_DIG_PIN();      // RX RP pin must be digital	
	CONFIG_RB11_AS_DIG_INPUT();		// P24 p09 RB11 input from  EM250 I/O-10 (ETRX p30) 
	CONFIG_INT2_TO_RP(11);			// CONFIG_INT2_TO_RP(pin) _INT2R = pin
// *********************************************************************************	
	
	MapPins_Zigbee_UART1();
	
//	#if defined(Uses_LLC_Bridge)
//	MapPins_JST_UART2();
//	__C30_UART = 2;				
//	#endif
	
	#if defined (Uses_JST_LED_SW)
	HAL_JST_Initialize();
	#endif

	Pic24_IO_Lock();
}

void HAL_H_BRIDGE_Set(H_BRIDGE_STATE set)
{
	if (set==H_BRIDGE_PHASE_FLAG)
	{
		if (phase.dir.flag==0)	// Coming from Negative going to Positive 
		{
			set = H_BRIDGE_FORWARD_LIVE;
		}
		else
		{
			set = H_BRIDGE_REVERSE_NEUTRAL;
		}
	}
    
    ufb_state = set; 
    
	#if defined(Uses_Debug_UFB)
    if (debug_ufb.bits.active)                   // if ufb debug mode on then 
    {
//        if (analog.dc.voltage < DCL_UFB_TEST_LIMIT_DCV)
//        {
            set = H_BRIDGE_DISABLED;
//        }
//        else
//            dcb1.ufb=0; // Disable test mode due
    }
    #endif
    
	if (set==H_BRIDGE_REVERSE_NEUTRAL)
	{
		IO_PIN_UFB_LIVE_HIGH_LATCH = 0;
        
		asm("NOP");
		asm("NOP");	

        IO_PIN_UFB_LIVE_LOW_LATCH = 1;		
        
		#if defined(Uses_Debug_UFB)
		trace.ufb_neg++;
        #endif
		
	}
	else if (set==H_BRIDGE_FORWARD_LIVE)
	{
		IO_PIN_UFB_LIVE_LOW_LATCH = 0;
        
		asm("NOP");
		asm("NOP");	
        
        IO_PIN_UFB_LIVE_HIGH_LATCH = 1;
        
		#if defined(Uses_Debug_UFB)
		trace.ufb_pos++;
		#endif
	}
	else
	{
		IO_PIN_UFB_LIVE_LOW_LATCH = 0;
		asm("NOP");
		asm("NOP");	
		IO_PIN_UFB_LIVE_HIGH_LATCH = 0;
		
		#if defined(Uses_Debug_UFB)
		trace.ufb_off++;
		#endif
	}
    
}
