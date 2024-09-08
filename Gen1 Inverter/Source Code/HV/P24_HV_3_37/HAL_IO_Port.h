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

#ifndef HAL_IO_PORT_DEFINITIONS_H
#define HAL_IO_PORT_DEFINITIONS_H

//	WRITE TO LATCH
//	READ FROM PORT

	#define IO_PIN_BOOTLOADER_PORT			(PORTBbits.RB11)		//DGA ZIGBEE ETRX ETRX_IO_10
	#define IO_PIN_BOOTLOADER_LATCH			(PORTBbits.RB11)		//DGA ZIGBEE ETRX ETRX_IO_10

	#define IO_PIN_UFB_LIVE_LOW_PORT     (PORTAbits.RA10)	//	p12 
	#define IO_PIN_UFB_LIVE_LOW_LATCH	(LATAbits.LATA10)	

	#define IO_PIN_UFB_LIVE_HIGH_PORT    (PORTBbits.RB4)		//  p33
	#define IO_PIN_UFB_LIVE_HIGH_LATCH	(LATBbits.LATB4)

	#define _IO_PIN_BUCK_PORT               (PORTBbits.RB5)		// p41
	#define _IO_PIN_BUCK_LATCH               (LATBbits.LATB5)	// p41
    #define PWM_BUCK_LATCH_ON()             _IO_PIN_BUCK_LATCH=1
    #define PWM_BUCK_LATCH_OFF()            _IO_PIN_BUCK_LATCH=0


	#define IO_PIN_GRID_ZC_PORT           (PORTBbits.RB7)		// p43
//  #define IO_PIN_GRID_ZC_LATCH          (LATBbits.RB7)		// p43

	#define IO_PIN_GRID_PHASE_PORT        (PORTBbits.RB8)		// p44

	#define IO_PIN_EM250_RESET				(PORTAbits.RA7)		// p13

	#define IO_PIN_EM250_RESET_TRIS				(TRISAbits.TRISA7)
	#define IO_PIN_ZIGBEE_PIC24_UPDATE_TRIS		(TRISBbits.TRISB11)		//DGA DEC 15, 2017 - PIC24 UPDATE SIGNAL FROM ZIGBEE ETRX 

	#define	PWM_SINE_PRX        PR2
    #define	PWM_SINE_DUTY       OC2RS
    #define	PWM_SINE_OCR        OC2R

	#define	PWM_CLOCK_PRX       PR3
	#define	PWM_CLOCK_DUTY      OC3RS
//  #define	PWM_CLOCK_OCRZ       OC3R

	#define	HAL_ISR_TMR4_PWM_SINE_IF	IFS1bits.T4IF
	#define	HAL_ISR_TMR4_PWM_SINE_IE	IEC1bits.T4IE

	#define	HAL_ISR0_AC_ZERO_CROSS_IF	IFS0bits.INT0IF
	#define	HAL_ISR0_AC_ZERO_CROSS_IE	IEC0bits.INT0IE

//	#if defined(Uses_Console) && defined(Uses_Diagnostics)
	#define	HAL_ISR1_AC_PHASE_IF		IFS1bits.INT1IF
	#define	HAL_ISR1_AC_PHASE_IE		IEC1bits.INT1IE
//	#endif

	#define	HAL_ISR1_AC_PHASE_IPOL		INTCON2bits.INT1EP

	#define	HAL_ISR2_BOOT_LOAD_IF		IFS1bits.INT2IF
	#define	HAL_ISR2_BOOT_LOAD_IE		IEC1bits.INT2IE

	#define	HAL_ISR_ADC1_IF				IFS0bits.AD1IF
	#define	HAL_ISR_ADC1_IE				IEC0bits.AD1IE

	#define	HAL_ISR_TMR5_IF				IFS1bits.T5IF
	#define	HAL_ISR_TMR5_IE				IEC1bits.T5IE

	#define	HAL_ISR_SPI2_IF				IFS2bits.SPI2IF
	#define	HAL_ISR_SPI2_IE				IEC2bits.SPI2IE

	#define WDT_SW_Enable()     RCONbits.SWDTEN = 1
	#define WDT_SW_Disable()    RCONbits.SWDTEN = 0
	#define WDT_Reset()         ClrWdt()

	#define Pic24_IO_Unock()    __builtin_write_OSCCONL(OSCCON & 0xBF)
	#define Pic24_IO_Lock()     __builtin_write_OSCCONL(OSCCON | 0x40)

//	#define IO_DIRECTION_OUTPUT     (0)
//	#define IO_DIRECTION_INPUT      (1)

	void HAL_ConfigurePorts();
	
	typedef enum
	{
		H_BRIDGE_DISABLED = 0,
		H_BRIDGE_FORWARD_LIVE,
		H_BRIDGE_REVERSE_NEUTRAL,
		H_BRIDGE_PHASE_FLAG,
	} H_BRIDGE_STATE; 
	
	void HAL_H_BRIDGE_Set(H_BRIDGE_STATE set);
    #define HAL_H_BRIDGE_Get()  ufb_state
    
    

	

#endif 
