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


#define public			// DGA public defined causes static memory allocation, one copy
#include "APP_Inverter.h"


void APP_Task_Ms1000()
{
    Uint32  boot_sec;
    Uint16  boot_hrs;
    
	#if defined(Uses_Debug_Reset_P24_Sec)
	if (p24_reset_timer++ >= Uses_Debug_Reset_P24_Sec)
	{
		APP_Bootloader_Reset();
	}
	#endif

//****************** SECONDS *********************
    boot_sec = _boot_sec;   // Copy volatile _boot_sec to local variable
	cpu_time.sec = (int16u) ((Uint32) (boot_sec % 60));     
	cpu_time.min = (int8u)  ((Uint32) ((boot_sec / 60) % 60));
    boot_hrs = (int16u)  (Uint32) (boot_sec / 3600);
	cpu_time.hr = (int16u)  (boot_hrs % 24);
    cpu_time.day = (int16u)  (boot_hrs / 24);
//****************** SECONDS *********************	

// *********************************************************************************
// DGA - P24 Bootloader ISR Enable - Fault tolerant code, waits 
// after boot then periodically enables hardware ISR - 
//	Done this way just in case something disables ISRs 
	#define P24_MIN_BOOT_SECS_ISR_ENABLE	4
	if(boot_sec > P24_MIN_BOOT_SECS_ISR_ENABLE)	
	{
		HAL_P24_Bootloader_ISR_Enable();
		APP_Bootloader_Task();	// Pole input just in case ISR not working 
	}
// P24 Bootloader ISR Enable - Fault tolerant code
// *********************************************************************************	

	#if defined(Uses_PWM_Telem)
	PWM_Telem_Calc_Avg();
	#endif
	
	ZNet_EM250_WDT_Task_Sec();
	
//	#if defined(Uses_Lockup_Test)
//	dcb1.pic24_clock = 1;		// DGA force ON
//	#endif
	
	#if defined(Uses_LLC_Bridge)
	LLC_Bridge_seconds_task();
	#endif
	
	HAL_Fault_Send_Report(0);
	
	#if defined ( Uses_Console )
	console_Task_Ms1000();
	#endif

	
	if (rcb.em250_bootloader)
	{
		if (rcb.em250_bootloader++ >= 60)
		{
			rcb.em250_bootloader = 0;
		}
	}
	
	ZNet_telemetry_ota_write();	// Call new telemetry function once per second 
	
	#if defined( Uses_EM250_WDT )
	pcb1.em250_wdt_timer++;
	#endif

	APP_Inverter_State_Machine_Ms1000();
	
	#if defined(Uses_Debug_NVS_Clock)
	nvs.run_sec++;
	NVS_Clock_Write();
	
	if (nvs.run_sec > NVS_Clock_Reset_Sec)
	{
		nvs.test_reset++;
		APP_Bootloader_Reset();
	}
    #endif

    #if defined (Uses_P24_WDT) && defined (Uses_P24_WDT_Test)
    #define WDT_Test_Seconds    15

	if(app.boot_sec < WDT_Test_Seconds)
	{
        WDT_Reset();
        printf("WDT TEST %ld  RCON:%x %x\n",(WDT_Test_Seconds-app.boot_sec)+1, boot_rcon,RCON);        
    }
    #endif



    #if defined(Uses_Debug_NVS_MPPT)
    {
        Uint16 intData = 56;
        NVS_Read_Word(NVS_MPPT_AUTO_PERCENT, (Uint16 *) &intData,NVS_VALID_DEFAULT);
        printf("TASK AUTO_PERCENT %d %d\n", intData, tune.mppt.auto_percent);
    }
    #endif
    
	
}

void APP_Task_Ms100()
{
	#if defined(Uses_NVS_Console) || defined(Uses_Debug_NVS)
	NVS_Task();
	#endif	
    
    #if defined(Uses_Install_Mode)
	APP_Inverter_Install_Mode_Refresh();
    #endif
    
	APP_Inverter_NVS_Stop_Refresh();
    
    #if defined(Uses_Debug)
    Debug_Task_Ms100();
    #endif
    
    
    
}

void APP_Task_Ms10()
{
    #if defined(Uses_Debug)
    if (++debug_print_tick_div >= 20)
    {
        debug_print_tick_div=0;
        Debug_Task_Ms10();
        debug_print_tick_div = 0;
    }
    #endif
    
	
    
}

void APP_Task_Ms500()
{
//    console_print_debug_tick();
}

void APP_Task_Low_Priority()
{
	if (timer.bit.sec)
	{

		rcb1.sec=1;
		#if defined (Uses_JST_LED_SW)
		jst.bit.sec=1;
		#endif
		APP_Task_Ms1000();
		timer.bit.sec = 0;
		return;
	}

	if (timer.bit.ms500)
	{
		timer.bit.ms500=0;
		#if defined (Uses_JST_LED_SW)
		jst.bit.ms500=1;
		#endif		
		APP_Task_Ms500();
		return;
	}

	if (timer.bit.ms10)
	{
		timer.bit.ms10=0;
		
		#if defined (Uses_JST_LED_SW)
		jst.bit.ms10=1;
		#endif

		APP_Task_Ms10();

		if (timer.bit.ms100)
		{
			timer.bit.ms100=0;
			#if defined (Uses_JST_LED_SW)
			jst.bit.ms100=1;
			#endif			
			rcb1.ms100=1;
			APP_Task_Ms100();
		}

	}
	
	HAL_EEPROM_task();
	KWH_Task();	

	if (timer.bit.tps60)
	{
		timer.bit.tps60=0;
		timer.bit.kwh = 1;
	}

	#if defined(Uses_LLC_Bridge)
	LLC_Bridge_task();
	#endif


	HAL_JST_Task();
	
}

void APP_Task_Scheduler()
{

    #if defined (Uses_P24_WDT) && !defined(Uses_P24_WDT_Test)
    WDT_Reset();
    #endif
    
	#if defined( Uses_Debug_States )
	if (state_line)
	{
		printf("HAL_P24_Task() %s,%d  STATE:%d\n", 
				state_file, state_line, APP_State_Get() );
		state_line = 0;
	}
	#endif

    PWM_Buck_Drive_Task();
    
	ZNet_Task();
    
	ADC_Task();
    
	APP_AC_Freq_Task();	
    
    APP_Inverter_State_Machine_Task();
		
	#if defined (Uses_Uart2 ) && !defined(Uses_LLC_Bridge)
	console_task();
	#endif

	APP_Task_Low_Priority();
    
    #if defined(Uses_Debug)
    Debug_Task();
    #endif

}