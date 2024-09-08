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


#define _PUBLIC

#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <libpic30.h> 
#include "Pic24_Bootloader.h"
#include "pic24_ports.h"
#include "HAL_Uart.h"
#include "HAL_IO_Port.h"
#include "Pic24_LED_PCB.h"



/*
 *   ****************** DGA SAFE MODE ADDED 6/17/24 ****************** 
*/
void P24_Safe_Mode()
{
    PWM_BUCK_LATCH_OFF();               // Disable buck mosfet
    PWM_SINE_DUTY=0;                    // Disable sine pwm
    IO_PIN_UFB_LIVE_LOW_LATCH=0;        // Disable UFB LO
    IO_PIN_UFB_LIVE_HIGH_LATCH=0;       // Disable UFB HI
    HAL_ISR_ADC1_IE=0;                  // Disable ADC ISR
    HAL_ISR0_AC_ZERO_CROSS_IE  = 0;     // Disable zero crossing ISR
}

void P24_IO_port_map()
{
    Pic24_IO_Unock();

//  ****************** DGA SAFE MODE ADDED 6/17/24 ****************** 
    CONFIG_RA10_AS_DIG_OUTPUT();		// p12 RA10 UFB_ENABLE_LIVE_LOW
	CONFIG_RB4_AS_DIG_OUTPUT();		// p33 RB4 UFB_ENABLE_LIVE_HI
	CONFIG_RB5_AS_DIG_OUTPUT();		// P41 RB5  IO_PIN_BUCK_ENABLE
    
	CONFIG_RB2_AS_DIG_OUTPUT();		// PIC P23 RB2 SINE_TEMPLATE   PWM 
	CONFIG_OC2_TO_RP(2);			// PIC P23 RP2 to Sine PWM output OC1 
	CONFIG_RB3_AS_DIG_OUTPUT();		// PIC P24 RP3 to PR3/OC3 SQR PWM clock 
	CONFIG_OC3_TO_RP(3);			// p24 RP3 to OC3 PWM SQR clock     
    CONFIG_RB7_AS_DIG_INPUT();					// P43 RB7  GRID_INTERRUPT
//  ****************** DGA SAFE MODE ADDED 6/17/24 ****************** 
   
    
//	Map Uart-1 to Zigbee 	
    CONFIG_RP23_AS_DIG_PIN();       // RX RP pin must be digital
    CONFIG_RP24_AS_DIG_PIN();       // TX RP pin must be digital
	CONFIG_RC7_AS_DIG_INPUT();
	CONFIG_RC8_AS_DIG_OUTPUT();
    CONFIG_U1RX_TO_RP(23);          // U1RX <- RP23 pin 3
    CONFIG_U1TX_TO_RP(24);          // U1TX -> RP24 pin 4
	Uart1_Config(BAUDRATE);
	
	#if defined ( Uses_Debug )
	Uart2_Config(BAUDRATE);
	#endif

	CONFIG_RP11_AS_DIG_PIN();        // RX RP pin must be digital	
	CONFIG_RB11_AS_DIG_INPUT();		// 09	PGEC2/RP11/CN15/PMD1/RB11	EM250 Pic24 Update

	#if defined(Uses_JST_Diag_PCB_LED)
	P24_JST_PCB_MapPins();
	#endif

	Pic24_IO_Lock();
} 

void initialize()
{
    AD1PCFG = 0xFFFF;
	WDT_SW_DISABLE1();
	P24_IO_port_map();
    P24_Safe_Mode();
	Set_Timer_32();
	is_valid_app = 0;
	app_erase = 0;
	HAL_clear_OTA_Buffers();
	sourceAddr.Val = APP_PROG_RESET;		//Setup user reset vector
	app_Reset.Val = ReadLatch(sourceAddr.word.HW, sourceAddr.word.LW);
	
	//Prevent bootloader lockout - if no user reset vector, reset to BL start
	if(app_Reset.Val == 0xFFFFFF)
	{
		is_valid_app = 0;
		app_Reset.Val = BOOT_ADDR_LOW;
	}
	else
	{
		is_valid_app = 1;
		
//		#if defined( Uses_Debug	)
//		Put_CR();
//		PutsCR("************************");
//		PutsCR("FOUND VALID APP FIRMWARE");
//		PutsCR("************************");
//		Put_CR();
//		#endif
	}
	
	app_ResetRead = 0;

	ticks = 0;
	secs = 0;
	ticks_ms = 0;
	
	WDT_SW_DISABLE1();
    
    P24_Safe_Mode();
    
	#if defined(Uses_JST_Diag_PCB_LED)
    P24_JST_PCB_LED2_ON();
    P24_JST_PCB_LED3_ON();
	#endif
		
	Put_Keep_Alive(0);

	#if defined(Uses_Debug_Write)
	Uart2_Print_Name_And_Copyright();
	#endif

} 

void Boot_App()
{
	#if defined( Uses_Debug	)
	JST_PutsCR("Reset");
	Uart2_TX_process(1);		// Wait here until done transmitting 	
	#endif
	
	T2CONbits.T32 = 0;
	T2CONbits.TON  = 0;
	U1MODEbits.UARTEN = 0;
	U2MODEbits.UARTEN = 0;
	ResetDevice(APP_PROG_START);
}

void task_quit()
{
	if (blBusy)
		return;
	
	ZIG_pole_P24_bootload_mode_input();
	
	if (pic24_update==0)
	{
		if (secs < 6)
			return;
		
		if (is_valid_app)
		{
			Boot_App();
		}
	}	
	
}

void task_run()
{
    
	Uart_process();
	
	if(IFS0bits.T3IF)
	{
		IFS0bits.T3IF = 0;
		task_1_ms();
	}	
}

int main()
{
	initialize();
	task_quit();
	
	Main_Loop:
	{
		task_run();
		handle_SerialData_task();
		goto Main_Loop;
	}	
	
	return 0;
}



#if defined(USE_BOOT_PROTECT) || defined(USE_RESET_SAVE)
/*********************************************************************
* Function:     void replaceBLReset(DWORD_VAL sourceAddr)
* Input:		sourceAddr - the address to begin writing reset vector
* Overview:		Writes bootloader reset vector to input memory location
********************************************************************/
void replaceBLReset(DWORD_VAL sourceAddr)
{
	DWORD_VAL data;	
	//get BL reset vector low word and write
	data.Val = 0x040000 + (0xFFFF & BOOT_ADDR_LOW);
	WriteLatch(sourceAddr.word.HW, sourceAddr.word.LW, data.word.HW, data.word.LW);
	//Write low word back to memory on word write capable devices
	#ifdef DEV_HAS_WORD_WRITE
	WriteMem(PM_WORD_WRITE);	//perform BL reset vector word write bypassing flow protect
	#endif
	//get BL reset vector high byte and write
	data.Val = ((DWORD)(BOOT_ADDR_LOW & 0xFF0000))>>16;
	WriteLatch(sourceAddr.word.HW,sourceAddr.word.LW+2,data.word.HW,data.word.LW);
	WriteMem(PM_WORD_WRITE); //perform BL reset vector word write
}
#endif

/********************************************************************
* Function:     void Verify_Write_Timeout()
* Overview:		This function writes the stored value of the bootloader
*				timeout delay to memory.  This function should only
*				be called after successful verification of the programmed
*				data to prevent possible bootloader lockout
********************************************************************/
void Verify_Write_Timeout()
{
	app_Timeout.Val = 10;
	//Write timeout value to memory write data into latches
	WriteLatch((DELAY_TIME_ADDR & 0xFF0000)>>16, (DELAY_TIME_ADDR & 0x00FFFF), 
	app_Timeout.word.HW, app_Timeout.word.LW);
	//Perform write to enable BL timeout - execute write sequence
	WriteMem(PM_WORD_WRITE);	
}
