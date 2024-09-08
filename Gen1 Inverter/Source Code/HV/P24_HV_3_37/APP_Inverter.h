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

#if !defined(_APP_INVERTER_H)
#define _APP_INVERTER_H

//	PIC24FJ64GA004

	#define	FIRMWARE_VERSION_NUM_MAJOR		3
	#define	FIRMWARE_VERSION_NUM_MINOR		37

#if defined(SMI240_60)
	#if defined(SMI360_72) || defined(SMI480_60)
	#error Multiple variants defined
	#endif
	
	#define VARIANT_TEXT		"SMI240_60"
	#define	Uses_Inverter		1
#endif

#if defined(SMI360_72)
	#if defined(SMI240_60) || defined(SMI480_60)
	#error Multiple inverter models defined
	#endif
	#define VARIANT_TEXT			"SMI360_72"
#endif

#if defined(SMI480_60)
	#if defined(SMI240_60) || defined(SMI360_72)
	#error Multiple inverter models defined
	#endif	
	#define VARIANT_TEXT        "SMI480_60"
#endif


// ******* BEG CONDITIONAL COMPILE FEATURES ******* 
	#define		Uses_Inverter						1
    #define		Uses_JST_LED_SW						1
	#define		Uses_MPPT_Console
	#define		Uses_PWM_Fault                      1
	#define		Uses_Console_Run
	#define		Uses_Console_Temperature
    #define		Uses_P24_WDT
    #define		Uses_Console_Fault
    #define     Uses_MPPT_AC_Ramp                   1
    #define		Uses_Fault_Freq_HI                  1
    #define		Uses_Fault_Freq_LO                  1
    #define		Uses_MPPT_Ramp_Up_Delay_Lock        1
    #define		Uses_MPPT_Ramp_PWM_Avg              1
    #define		Uses_Install_Mode                   1

//  #define		Uses_PWM_Ripple_Fault               1


//    #define		Uses_Fault_Phase_REV_POS            1
//    #define		Uses_Fault_Phase_LO_POS             1
//    #define		Uses_Fault_Phase_MAX_POS            1
//    #define       Uses_Fault_Phase_RANGE_POS          1
//    #define       Uses_Fault_Phase_TRIG_REV_POS       1
//    #define		Uses_Fault_Phase_RANGE_NEG          1
//    #define		Uses_Fault_Phase_TRIG_LO_NEG        1
//    #define		Uses_Fault_Phase_TRIG_MAX_NEG       1
//    #define		Uses_Fault_Phase_TRIG_REV_NEG       1


//    #define Debug_2024  1

    #if defined(Debug_2024)
	#define Uses_Debug                  1
    #define Uses_Debug_Start            1
    #define Uses_Debug_UFB              1
    #define Uses_Debug_DCV              1
    #define Uses_Debug_ACV              1
//	#define	Uses_Debug_Temperature		1
//  #define	Uses_PWM_Ripple_Fault       1
// #define Uses_MPPT_Run_Test_Sec      30
    #define	Uses_AMP_Lock               1
    #endif

//  #define		Uses_P24_WDT_Test                   1
//  #define Uses_Lockup_Test
//	#define	Uses_Debug_Fault			1
//	#define	Uses_Debug_Freq             1
//  #define	Uses_Debug_Phase            1
//	#define	Uses_Debug_ADC				1
//	#define	Uses_Debug_ADC_Peak_AC  	1
//  #define	Uses_Debug_ADC_PWM          1
//  #define Uses_Debug_GEO              1
//  #define Uses_Debug_PWM2             1
//  #define Uses_Debug_DCV_Report       1
//  #define	Uses_Debug_PWM              1
//  #define Uses_Debug_NVS_Write        1
//	#define	Uses_Clock_Lock
//  #define	Uses_Debug_Alarms		1
//	#define	Uses_Debug_DcLimits			1

//	#define	Uses_Debug_PWM_Fault		1
//	#define	Uses_Debug_UFB_Test		1
//	#define	Uses_Debug_SPI
//	#define	Uses_DCV_Test_Mode	501
//	#define	Uses_Debug_Alarms		1
//	#define	Uses_Debug_Power
//	#define	Uses_Debug_DC_Link
//	#define	Uses_Debug_Tune				
//	#define	Uses_Debug_Tuning
//	#define	Uses_Debug_MPPT				1
//	#define Uses_Debug
//  #define Uses_Debug_KWH
//	#define	Uses_Debug_GEO
//   #define Uses_Console_Fault_Test      1    


//  *** 09/14/2022 SPI FLASH MEM DEBUG
	#define	Uses_NVS_Console            1
    #define	Uses_GEO_Console            1
	#define	Uses_Diagnostics            1
//  #define	Uses_Debug_NVS_SPI          1
//  #define	Uses_Debug_NVS_Fault        1

//	#define	Uses_Debug_NVS_Clock        1
    #if defined(Uses_Debug_NVS_Clock)
    #define	NVS_Clock_Reset_Sec         20
    #endif

//  *** 09/14/2022 SPI FLASH MEM DEBUG

//	#define	Uses_Debug_MPPT_Auto        1
//  #define	Uses_MPPT_Print_Tracking

//	#define	Uses_Debug_Temperature		1

// ******* END CONDITIONAL COMPILE ******* 

	#if !defined(FCY)
	#define FCY  24000000L
	#endif

	#define CPU_CLOCK_FREQUENCY    (12000000)
	#define CPU_CLOCK_PRESCALER    (8)

	#if !defined( public )
    #define public extern
	#endif

	#include <xc.h>
	#include <stdio.h>
	#include <string.h>
	#include <libpic30.h>		// DGA required for __C30_UART
	#include <stdlib.h>

	#include "TYPE_DEFS.h"
	#include "HAL_IO_Port.h"
	#include "HAL_Pic_24fj64ga004_ports.h"
	#include "HAL_Pic24_ports.h"
	#include "APP_Task.h"
	#include "HAL_LLC_Bridge.h"
	#include "APP_ZNet.h"
	#include "HAL_PWM.h"
	#include "HAL_WiMesh.h"
	#include "APP_Base64.h"
	#include "HAL_Peripheral.h"
	#include "APP_NVS.h"
	#include "HAL_Uart.h"
	#include "APP_KWH.h"
    #include "APP_AC_FREQ.h"
	#include "HAL_SPI_EEPROM.h"
	#include "HAL_Faults.h"
	#include "HAL_Temperature.h"
	#include "APP_GEO.h"
	#include "HAL_SPI_EEPROM.h"
	#include "APP_DC_LINK.h"
	#include "APP_AC_GRID.h"
	#include "APP_MPPT.h"
	#include "HAL_ADC.h"
	#include "HAL_JST_LED_SW.h"
	#include "APP_NVS_Tune.h"
	#include "APP_Console.h"
	#include "Debug.h"
	#include "APP_Diagnostics.h"
	#include "APP_P24_Flash.h"

	typedef union 
	{
		struct 
		{
			Uint16 a;
			Uint16 b;
		};

		struct 
		{
			Uint32 value;
		};

	} Clock_Control;

	typedef union 
    {
		struct 
		{

			unsigned telemetry :1;	
			unsigned run  :1;
			unsigned diag_ufb	:1;
			unsigned diag_buck :1;
            unsigned fault_ripple :1;
		};

		struct {
			unsigned int value;
		};

	} Pic24_Eprom_Control_Bits1;
    	
	typedef union {
		struct 
		{
			unsigned initializing:1;
            #if defined(Uses_Install_Mode)
            unsigned install_mode:1;
            #endif
            unsigned stop_task:1;
            
			#if defined(Uses_Console)
			unsigned console:1;
   			unsigned console_cmd:1;
			unsigned console_clock:1;
			unsigned console_mppt:1;
			#endif
			
            unsigned fault_history_reset:1;
            
			unsigned rfu :5;	
		};

		struct {
			unsigned int value;
		};

	} Run_Control_Bits;
	
	typedef struct 
	{
		Uint16	em250_wdt_timer;
		Uint16	em250_wdt_time_out;
		Uint8	em250_bootloader;
		volatile Run_Control_Bits bit;

		#if defined(Uses_Debug_Reset_P24_Sec)
		Uint8	p24_reset;
		#endif
		
	} Run_Control_Block;

	typedef union {
		struct 
		{
			unsigned ms10:1;
			unsigned tps60:1;
			unsigned ms100:1;
			unsigned ms500:1;
			unsigned sec:1;
			unsigned start_sec:1;
			unsigned run_sec:1;
			unsigned kwh:1;
			unsigned diag_120_hz:1;
            unsigned diag_100_hz:1;
			unsigned rfu :2;
		};

		struct {
			unsigned int value;
		};

	} Timer_Control_Bits;
		
	typedef struct 
	{
        Uint16 tmr1_reg;
        Uint16 tmr1_reg2;
		Uint16 ticks;
		Uint16 ms_cnt;
		Uint16 timer;
		Uint16 delay;
		Uint16 zc_ms;
		Uint16 zc_isr;
        Uint16 zc_freq; 
		Uint8 div1;		// 6000/6=1000
		Uint8 div2;		
		Uint8 div3;		
		Uint8 div4;		
		Uint8 div5;		
		Uint8 div10;
		Uint8 div120;
		Uint8 div60;
		
        Uint8 temperature;
 		Uint16 dclink;
		Uint16 buck_div;
 		Uint16 buck_ms;
   		Uint16 buck_sec;
   		Uint16 ac_connect_delay;
   		Uint16 spi_sec;
        
 		Uint16 mppt_up_delay;
 		Uint16 mppt_dn_delay;
        
        Uint16 mppt_adc_up;
        Uint16 mppt_adc_dn;
        
        Uint16 mppt_ramp_up;
        Uint16 mppt_ramp_dn;

   		Uint16 stop_delay_ms100;
        Uint16 hours_runtime;

		Timer_Control_Bits bit;
        

	} Timer_Control_Block;

	public volatile Timer_Control_Block timer;
	public volatile Run_Control_Block rcb;
	public volatile Pic24_Eprom_Control_Bits1 ecb1;

	typedef struct 
	{
        Uint16 ac_high; 
        Uint16 ac_low; 
        Uint16 dc_low;
        Uint16 ac_freq;
        Uint16 ac_freq_high;
        Uint16 ac_freq_low;
        Uint16 dcv_ripple;
        Uint16 phase;
	} APP_Fault_History;
    
    	typedef struct 
	{
        Uint16 hours; 
        Uint16 minutes; 
        Uint16 seconds;
	} APP_Clock;
    
		
	typedef struct 
	{
		Uint8	text_firmare_ver[20];
		Uint8	text_variant_ver[32];
		Uint8	text_copyright[64];
		Uint8	gRemoteCommand; 
        
        Uint32          run_sec;
        
        APP_Clock       run_clock;
        
        Uint16 connection_time;         // DGA 
        Uint16 connection_fault_delay;  // DGA 
        volatile APP_Fault_History fault_history;  // DGA 
        
	} APP_Control_Block;
    
	public APP_Control_Block app;
        
	public volatile CPU_Runtime cpu_time;
	public volatile Uint32 _boot_sec;
    public volatile Uint16 boot_rcon;

#endif	// _APP_INVERTER_H