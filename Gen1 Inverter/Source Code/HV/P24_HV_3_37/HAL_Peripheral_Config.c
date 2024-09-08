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

/*******************************************************************/
/* Function:    HAL_ConfigureTimers                                */
/* Description: Configure and initialise on-chip timers.           */
/*******************************************************************/
void HAL_ConfigureTimers(void)
{
    
#if defined(DOCUMENTATION_TIMERS)    
 Timer-0    8 bit Watchdog 
 Timer-1    16 bit free wheel running at 1,500,000  (12,000,000 1:8 = 1.5mhz)
 Timer-2    Sine pwm output on RB2 using Timer2 and Timer 4.
 Timer-3 	Drive waveform for Buck converter driver PWM Output on RB2 
 Timer-4 	See Timer-2
 Timer-5 	6000 TPS
#endif	
    
    
    
    
    // Initialise timer registers.
    TMR1 = 0;
    TIMER2 = 0;
    TIMER3 = 0;
    TIMER4 = 0;
    TMR5 = 0;
	
// Pic24 Timer calculations - 
// *****************************************************************************
// The Pic24 only has one 8 bit timer. Timer0 the watchdog timer.
// *****************************************************************************
// TIMER-0 8-Bit
// *****************************************************************************
// 8 BIT TIMER CALCULATIONS ARE AS FOLLOWS:
// FOSC = 24,000,000
// (FOSC / 4) / (256-PRX) 
//
// 24 mhz w/PLL enabled  - To make timer calculations simple just use 6 mhz 
//
// 8 bit timer-0 example 
// 
// PR0 253	2000000.00 Hz (0.00000050 sec) 
// 
// 256 minus 253 = 3
// 6,000,000 / 3 = 2,000,000 

// *****************************************************************************
// The Pic24 has FIVE 16 bit timerS. Timer 1-5
// *****************************************************************************
// 16 BIT TIMER CALCULATIONS ARE AS FOLLOWS:
// 
// (12Mhz FOSC/2) / PRX
//
// 65536 minus 5536 = 60,000
// 6,000,000 / 60,000 = 100 ticks
// Ticks 
//
//******************************************************************************	
	
	
#if defined(DOCUMENTATION_TIMER1)
On June 15, 2019 and on 09/06/2023  DGA - verified Timer1 is running at 1.5 MHZ 
    
TMR1	Non ISR - TCKPS0 / 8    Full clock frequency monitoring timer reset in every other zero crossing.
The clock freq with PLL is 24mhz.   The FOSC frequency is PLL / 2 = 12,000,000 
Timer1 has BIT 0 (zero) enabled which is 1:8   12,000,000 / 8 = 1,500,000 
TMR1 is free wheel count up from zero.  TMR1 is reset every other ZC 
      The clock freq with PLL is 24mhz.   The FOSC frequency is PLL / 2 which is 12 mhzAll the timers have FOSC (12 mhz) going into them.  Timer1 has BIT 0 (zero) enabled which is 1:8 
12,000,000 / 8 = 1,500,000 

#define TIMER1_FREQ_DIVISOR     ((Uint32)7500000)             
7,500,000  is the magic divisor to calculate to and from grid freq x10Here is how it is calculated.   
Take 1,500,000  and divide 2:1  then multiply x 10.   i.e.  1,500,000  / 2=750,000   * 10=7,500,000      
    //  7500000 can now be used to calculate the timer value from Grid frequency x10 
    // Example: Grid Freq 50HZ is expressed as 500 and 60HZ is 600
    // 7500000 / 500 = 15,000
    // 7500000 / 600 = 12,500        
   // 7500000 can also be used to calculate the Grid frequency x10 from the timer value    // Example:  7500000 / 15,000  = 500 (50.0HZ) 
#endif
    
    // Initialise timer controls.
    T1CON = DEF_TON | TCKPS0; 
			
    // T2CON set elsewhere.
    // T3CON set elsewhere.
    T4CON = DEF_TON; // Timer4 is on and operates as 16bit timer.

	PR5 = 2000;		// 6000 TPS   (12Mhz FOSC/2) / 12000=6000
	T5CON = DEF_TON;

	HAL_ISR_TMR5_IF = 0;
//	HAL_ISR_TMR5_IE = 1;
    __write_to_IEC(HAL_ISR_TMR5_IE=1); 
}

#if defined(DOCUMENTATION)
Timing for ZC
---------------------------------------
60hz  /60=16.66  /2=8.333 ms    1000us
1000  /50=20.00  /2=10.00 ms    1000us
1500/ms 
1.5us *2
---------------------------------------
pos cycle to fall 

6000/6= 166 us per tick  (166,332,498,654,830,1000)

#endif

void HAL_ConfigureUart_1(void)
{
	Uart1_Config(115200);	
}	


#if defined(Uses_Uart2)
void HAL_ConfigureUart_2(void)
{
	#if defined(Uses_LLC_Bridge)

	#if defined(SMI480_60)
	Uart2_Config(LLC_Bridge_Duo_Baud);
	#else
	Uart2_Config(LLC_Bridge_12F_Baud);
	#endif
	
	#else
	Uart2_Config(115200);
	#endif
}	
#endif

void HAL_P24_Bootloader_ISR_Enable()
{
    
//  Bootloader uses PIC pin 09, RB11 which is mapped to INT2 
    
	if (HAL_ISR2_BOOT_LOAD_IE)
		return;
	HAL_ISR2_BOOT_LOAD_IF = 0;	// Clear ISR flag
//	HAL_ISR2_BOOT_LOAD_IE  = 1;	// Set ISR enable
    __write_to_IEC(HAL_ISR2_BOOT_LOAD_IE=1);
}

void HAL_P24_Bootloader_ISR_Disable()
{
//	HAL_ISR2_BOOT_LOAD_IE  = 0; 	//	ISR input FROM EM250 Zigbee 
    __write_to_IEC(HAL_ISR2_BOOT_LOAD_IE=0);
}


/*******************************************************************/
/* Function:    HAL_ConfigureInterrupts                            */
/* Description: Allocate the interrupts to the relevant devices.   */
/*******************************************************************/

void HAL_ConfigureInterrupts(void)
{

    #define Uses_2023_IPC   1
    
/*
 * DGA Notes (from pic24 data sheet) on IRQ priority 
External Interrupt 0		IPC0<2:0>
External Interrupt 1		IPC5<2:0>
Timer1			IPC0<14:12>
Timer2			IPC1<14:12>
Timer3			IPC2<2:0>
Timer4			IPC6<14:12>
Timer5			IPC7<2:0>
UART1 Receiver		IPC2<14:12>
UART1 Transmitter		IPC3<2:0>
UART2 Receiver 		IPC7<10:8>
UART2 Transmitter		IPC7<14:12>
 */	
	
	INTCON1 = 0x0000;
	INTCON2 = 0x0000;
	
//	IPC0    = 0x0007; // INT0 external interrupt 
//	IPC6    = 0x5000; // timer4 interrupt 
//	IPC5    = 0x0006; // Surge protection interrupt.
//	IPC7    = 0x0004;
//	IPC3    = 0x0002; // UART transmit interrupt
//	IPC2    = 0x3000; // UART receive interrupt 

// INT1EP: External Interrupt 1 Edge Detect Polarity Select bit
//	1 = Interrupt on negative edge
//	0 = Interrupt on positive edge
	    
//    ZC ZERO CROSS	PIC P43 RP7/INT0/CN23/PMD5/RB7	INT0    This pin always INT0
//    LOW PHASE	PIC P44 RP8/SCL1/CN22/PMD4/RB8		INT1    CONFIG_INT1_TO_RP(8);   HAL_IO_Port.c ,ln 157
//    BOOTLOADER	PIC P09 RB11                    INT2    CONFIG_INT2_TO_RP(11)   HAL_IO_Port.c ,ln 202
    
	INTCON2bits.INT0EP = 0;     // 0=Positive edge
    
    INTCON2bits.INT1EP = 0;     // 0=Positive edge
//  INTCON1bits.NSTDIS = 0;     //  Interrupt nesting enable (Active Low)
    INTCON1bits.NSTDIS = 1;     //  02/25/2024 DGA - disable nested interrupts 

    HAL_ISR1_AC_PHASE_IE=1;

    #if defined(Uses_2023_IPC)

    IPC7bits.INT2IP = 7;		// bootloader gets highest priority - it is non maskable and MUST stop everything else 
	IPC0bits.INT0IP = 6;		// ZERO Cross - very important 
    IPC5bits.INT1IP = 5;		// Phase 
   	IPC6bits.T4IP = 4;			// sine pwm duty cycle 20khz
    IPC7bits.T5IP = 3;			// Timer 5 , 6khz - very important to measure elapsed time 
	IPC3bits.AD1IP = 2;			// ADC analog AC measurement 
    
	IPC2bits.U1RXIP = 1;
	IPC7bits.U2RXIP = 1;
	IPC3bits.U1TXIP = 1;
	IPC7bits.U2TXIP = 1;
    
	IPC8bits.SPI2IP = 0;		// SPI2 EEPROM flash memory
    
    #else
	IPC0bits.INT0IP = 7;		// bootloader gets highest priority - it is non maskable and MUST stop everything else 
	IPC3bits.AD1IP = 6;			// ADC analog AC measurement 2nd highest priority 
	IPC6bits.T4IP = 5;			// sine pwm duty cycle 
	IPC7bits.T5IP = 4;			// Timer 5 next highest priority 
	IPC2bits.U1RXIP = 3;
	IPC7bits.U2RXIP = 3;
	IPC5bits.INT1IP = 2;		// zero crossing only happens 120 HZ max - that is .015 % as fast as ADC at 8khz 	
	IPC8bits.SPI2IP = 2;		// SPI2 EEPROM flash memory
	IPC3bits.U1TXIP = 1;
	IPC7bits.U2TXIP = 1;
    #endif
    

   	HAL_P24_Bootloader_ISR_Enable();
	
}