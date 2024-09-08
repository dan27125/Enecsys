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


void APP_Set_Default_Bits()
{
	ecb1.diag_ufb = 0;
	
	#if defined ( Uses_Console )	
	rcb.bit.console_clock = 0;
	
	#if defined(Uses_MPPT_Print_Tracking)
	rcb.bit.console_mppt=1;
	#endif
	
	#endif

	#if defined ( Uses_Debug )	
	dcb1.pic24_jst_debug = 0;
	#endif

	rcb.em250_wdt_time_out = 0;
	rcb.em250_wdt_timer = 0;

//	rcb.p24_clock_cnt = 0;
//	rcb.p24_clock_div = 1;
	
	#if defined(Uses_Diagnostics)
	diag.ufb_sec = 0;
	#endif

	ecb1.telemetry = 1;
	ecb1.run = 1;
//	ecb1.debug = 0;
//	ecb1.console = 0;
	
    fault.alarm.value |= ALARM_DISC|ALARM_AC_GRID_LOW;
		
	#if defined(Uses_Debug_NVS_Clock)
	rcb.bit.console_clock = 1;
	#endif
	
	
}



#if defined(DOC)
#define RCON RCON
extern volatile uint16_t  RCON __attribute__((__sfr__));
__extension__ typedef struct tagRCONBITS {
  union {
    struct {
      uint16_t POR:1;
      uint16_t BOR:1;
      uint16_t IDLE:1;
      uint16_t SLEEP:1;
      uint16_t WDTO:1;
      uint16_t SWDTEN:1;
      uint16_t SWR:1;
      uint16_t EXTR:1;
      uint16_t VREGS:1;
      uint16_t CM:1;
      uint16_t :4;
      uint16_t IOPUWR:1;
      uint16_t TRAPR:1;
    };
    struct {
      uint16_t :8;
      uint16_t PMSLP:1;
    };
  };
} RCONBITS;
extern volatile RCONBITS RCONbits __attribute__((__sfr__));

#endif


void APP_Task_Init()
{
    #if defined (Uses_P24_WDT)
    WDT_SW_Enable();
    if (RCON==0x20) 
        RCONbits.WDTO = 1;
    #endif
    
    boot_rcon = RCON;   // DGA save RCON bits
//  RCONbits.POR = 0;
//  RCONbits.BOR = 0;
//  RCONbits.SWR = 0;
//  RCONbits.EXTR = 0;
//  RCONbits.TRAPR = 0;
    
    RCON = 0;           // DGA clear RCON BITS

    #if defined (Uses_P24_WDT)
    WDT_SW_Enable();    // Enable WDT 
    #endif

	rcb.bit.initializing=1;

// ********************************************************		
// 2021-09-20 UPDATE FIRMWARE VERSION TEXT 
// ********************************************************		
//	"HV_G_1_7a"	"HV_G_2.13"	
	sprintf( (char*) app.text_firmare_ver, "HV %d.%.2d", FIRMWARE_VERSION_NUM_MAJOR, FIRMWARE_VERSION_NUM_MINOR );
	sprintf( (char*) app.text_variant_ver, "%s %s",VARIANT_TEXT, app.text_firmare_ver);
	sprintf( (char*) app.text_copyright, "(c) Dan Ambrose enecsysparts.com St Louis,MO USA");
// ********************************************************	
	
	HAL_ConfigurePorts();			// DGA set I/O ports.
	HAL_ConfigureTimers();			// DGA set timers

    #if defined (Uses_P24_WDT)
    WDT_Reset();
    #endif
	
	HAL_EEPROM_init();				// DGA set SPI EPPROM
	
	HAL_ConfigureUart_1();			// DGA set UART 1 
	
	#if defined( Uses_Uart2 )
	HAL_ConfigureUart_2();
	#endif

	HAL_ConfigureInterrupts();		// DGA set some ISR stuff 
	
//	******* CRITICAL ********	
	APP_Set_Default_Bits();
//  ******* CRITICAL ********		

    #if defined (Uses_P24_WDT)
    WDT_Reset();
    #endif

    
    _boot_sec = 0;
    
	ZNet_Send_P24_Boot_Info();			// DGA send special boot up message to zigbee wifi 
	
	P24_Flash_Init();				// DGA Initialize flash app 
	APP_delay_miliseconds(50);		// DGA delay 
	
	HAL_EEPROM_spi_read();			// DGA 04-11,2020 read entire SPI flash eprom into ram cache 

    #if defined (Uses_P24_WDT)
    WDT_Reset();
    #endif
    
	#if defined(Uses_Console)
	if (rcb.bit.console)
	{
		APP_delay_miliseconds(10);			// DGA short delay 
		console_print_version();
		APP_delay_miliseconds(10);			// DGA short delay 
		console_print_copyright();
		APP_delay_miliseconds(10);			// DGA short delay 
	}
	#endif
	
	NVS_Set_Valid(NVS_VALID_DEFAULT);

	#if defined(Uses_Debug_NVS_Clock)
	NVS_Read_All(NVS_MODE_IGNORE);		// DGA read NVS config 
	#else
	NVS_Read_All(NVS_MODE_NORMAL);		// DGA read NVS config 
	#endif
	
	KWH_Init();						// DGA init KWH wattmeter app 
	HAL_PWM_Init();				// DGA config PWM 
	
	MPPT_Init();					// DGA start MPPT module	
	
	APP_Stop_Mode();				// DGA set boot state to stop mode

//	********* CRITICAL LINE OF CODE *********
//	ADC module uses high speed ISR which runs at a high rate 
//	Do not activate until all other initialization 	is done 
	ADC_Init();				// DGA ADC causes heavy ISR traffic - wait till end of init
//	********* CRITICAL LINE OF CODE *********	
	
	HAL_ISR1_AC_PHASE_IF=0;			// DGA clear flag 
//	HAL_ISR1_AC_PHASE_IE=1;			// DGA enable ISR 
    __write_to_IEC(HAL_ISR1_AC_PHASE_IE=1);
	
	HAL_ISR0_AC_ZERO_CROSS_IF  = 0;     // DGA Clear zero crossing ISR
//	HAL_ISR0_AC_ZERO_CROSS_IE  = 1;     // DGA Enable zero crossing ISR
    __write_to_IEC(HAL_ISR0_AC_ZERO_CROSS_IE=1);
    

	HAL_JST_LED_2_OFF();				// DGA LED OFF 
	
	Temperature_Start();

	#if defined(Uses_Debug)
	Debug_Initialize();			// DGA init debug module 
	#endif

    #if defined (Uses_P24_WDT)
    WDT_Reset();
    #endif

	APP_delay_miliseconds(50);			// DGA short delay 
	rcb.bit.initializing=0;
    
//   rcb.bit.console_clock = 1;
//  _boot_sec = 197890UL;
    
}

int main(void)
{
	APP_Task_Init();

    while(TRUE)
    {	
		APP_Task_Scheduler();	// Main thread, event driven polling  
    }	
}