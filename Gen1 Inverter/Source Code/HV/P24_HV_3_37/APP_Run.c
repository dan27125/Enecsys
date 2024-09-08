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

void APP_Stop_Mode()
{
    
//  __builtin_disi(1000);         //disable interrupts for 1000 cycles     
    
	PWM_BUCK_LATCH_OFF();
    PWM_SINE_DUTY=0;
    
//  HAL_H_BRIDGE_Set(H_BRIDGE_DISABLED);
    IO_PIN_UFB_LIVE_LOW_LATCH=0;
    IO_PIN_UFB_LIVE_HIGH_LATCH=0;
    ufb_state=H_BRIDGE_DISABLED;
    
	HAL_ISR_TMR4_PWM_SINE_IE=0;
    HAL_ISR_ADC1_IE=0;

    phase.bit.value = 0;
	pwm.buck.enable = 0;
    pwm.buck.delay = 0;
	pwm.amp.watts = 0;
	pwm.amp.scale = 0;
    pwm.bit.value=0;    //reset
    pwm.watts=0;
    #if defined(Uses_PWM_Ripple_Fault)
    pwm.rip.inhibit_counter=0;
    #endif
	fault.alarm.delay = 0;
	fault.alarm.value |= (ALARM_DISC );
	fault.alarm.diagnostic = 0;
	
	#if defined(Uses_Diagnostics)
	diag.bit.start = 0;
	#endif

	#define	STOP_MODE_TIMER_NORM				21
	#define	STOP_MODE_TIMER_AC_HIGH				21
	#define	STOP_MODE_TIMER_DC_LOW				21
	
	timer.stop_delay_ms100 = STOP_MODE_TIMER_NORM;		// DGA set timer to lock stop mode state
	
	rcb.bit.stop_task=0;
	rcb.bit.fault_history_reset=0;
    
    GEO_freq.bit.lock=0;
    GEO_freq.zc.start.lock = 0;
    
    APP_State_Set(INVERTER_STOP);
    
    HAL_ISR_ADC1_IE=1;
}    

void APP_Inverter_Start_Reset()
{
	GEO_Set();  
	timer.ac_connect_delay=0;
    fault.alarm.phase=0;    // Reset PHASE fault bit
    
    #if defined(Uses_Debug_UFB)
    debug_ufb_reset();
    #endif
    
}

void APP_Stop_Mode_Task()
{
	#if defined(Uses_Diagnostics)
	if (diag.bit.fault_stop)		// Was AC connected while in diagnostic mode?
	{
		APP_Inverter_Diag_Mode_Stop();	// DGA this function will set diag.bit.fault_stop=0;
		printf("<DIAG FAULT> AC VOLTAGE DETECTED\n");
		ecb1.run = 1;
		NVS_RCB_Write();
		NVS_Console_Hours_Write(0);
	}
	#endif
	
	if (rcb.bit.stop_task==0)
	{
		rcb.bit.stop_task=1;
    
    //  if (app.run_sec >= 2)
/*      if (fault.info.code)
        {
            NVS_Fault_Hist_Increment(fault.info.counter,fault.info.param1,fault.info.param2);    // DGA increment nvs fault counter 
            fault.info.counter = 0;                             // DGA reset
        }
*/        
        if ((fault.alarm.freq_low) || (fault.alarm.freq_high))
        {
            // Calc the frequency from the saved timer fault 
            fault.info.hertz_10x = TIMER1_CALC_10X(fault.info.param1);
            GEO_freq.zc.instant = fault.info.hertz_10x;
            app.fault_history.ac_freq++;
        }
        else
            fault.info.hertz_10x = 0;
        

        if (fault.alarm.dclink_high)
        {
            // Calc the voltage from the saved sample values
            fault.info.ripple.run.volts = DCLINK_GET_VDC(fault.info.ripple.run.sample);
            fault.info.ripple.limit.volts = DCLINK_GET_VDC(fault.info.ripple.limit.sample);
            app.fault_history.dcv_ripple++;
        }

        // Check and set fault history counters 
        if (fault.alarm.freq_high)
            app.fault_history.ac_freq_high++;
        if (fault.alarm.freq_low)
            app.fault_history.ac_freq_low++;
        if (fault.alarm.grid_high)
            app.fault_history.ac_high++;
        
        if (fault.alarm.dclink_low)
            app.fault_history.dc_low++;
        
        if (fault.alarm.phase)
        {
            app.fault_history.phase++;
            app.fault_history.ac_freq++;
        }

        if (fault.info.code1)
        {
            NVS_Fault_Hist_Increment(fault.info.code1,fault.info.param1,fault.info.param2);    // DGA increment nvs fault counter 
            
            #if defined(Uses_Console_Fault)        

//          if (rcb.bit.console)
            {
                sprintf((char*)fault.info.text,"RUN-FAULT %d",fault.info.code1);
                
//              if ((fault.info.code1==FC_DCV_RIPPLE) || (fault.alarm.dclink_high))
                if (fault.alarm.dclink_high)   
                {
                    #if defined(Uses_Debug_DCV1)
                    printf("FAULT: "); 
                    printf("%uw ",debug_rip.watts); 
                    printf("V:%u,%u ",debug_rip.volts,debug_rip.sample); 
                    printf("RIP:%u,%u ",debug_rip.ripple_run,debug_rip.ripple_calc);
                    printf("DIV:%u,%u ",debug_rip.ripple_div,debug_rip.ripple_ndx);
                    printf("\n");
                    
                    #endif
                    
                    
//                  [RUN-FAULT 1] DC RIPPLE 1 Run:4 Calc:8  00:00:45                    
                    printf("[%s] DC RIPPLE %d, ST:%u,%d RL:%u,%u RC:%d ",
                            fault.info.text,
                            fault.info.code1,
                            fault.info.ripple.sample_counter,
                            fault.info.ripple.error_count,
//                            fault.info.ripple.error_limit,
                            fault.info.ripple.limit.sample,
                            fault.info.ripple.run.sample,
                            fault.info.ripple.rip_calc
                    );
                    
                    
               }
               else
               {
                
                    printf("[%s] %s T1:%u T2:%u",
                            fault.info.text,
                            console_fault_text(fault.info.code1),
                            fault.info.param1,
                            fault.info.param2
                    );

                    if (fault.info.hertz_10x)
                        printf(" HZ:%u.%u",fault.info.hertz_10x/10,fault.info.hertz_10x%10);
               }

                printf(" %.2u:%.2u:%.2u \n",(Uint16)app.run_clock.hours,app.run_clock.minutes,app.run_clock.seconds);
           
                console_print_alarms((char*)fault.info.text,fault.alarm);
                
        //      printf("(1) AC:%u,%u\n",analog.grid.voltage, ADC_CALC_ACV(analog.grid.sample_max) );
                
            }
            #endif
            
            if (app.run_sec)
            {
                HAL_Fault_Send_Report(1);       // Report 
            }
            
            fault.info.code1=0;
        }

	}
	
	#if defined(Uses_Console)

	if (ecb1.run==0)
	{
//		printf("STOP RUN=%d  UFB=%d  %s,%d\n",ecb1.run, diag.ufb_sec,__FILE__,__LINE__);
		
		#if defined(Uses_Diagnostics)
		if ((diag.ufb_sec) && (diag.bit.start==0))
		{
			APP_Inverter_Diag_Mode_Start();
			diag.bit.start=1;
		}
		#endif

		return;
	}
	#endif

	if (timer.stop_delay_ms100==0)
	{
		HAL_Fault_Send_Report(1);
		APP_Inverter_Start_Reset();
//        printf("(2) AC:%u,%u\n",analog.grid.voltage, ADC_CALC_ACV(analog.grid.sample_max) );
		APP_State_Set(INVERTER_START);
	}
}

#if defined(Uses_Install_Mode)
void APP_Inverter_Install_Mode_Refresh()
{
	Uint16 run_limit;
	
	run_limit = CONSOLE_RUN_LIMIT_NORMAL;
	if(mEngineeringMode)
	{
		run_limit = CONSOLE_RUN_LIMIT_ENGINEERING;
	}
	
	if(nvs.hours_console <= run_limit) 
		rcb.bit.install_mode = 1;
	else
		rcb.bit.install_mode = 0;

	#if defined(Uses_Console)
	if( (rcb.bit.install_mode) || (rcb.bit.console_cmd))
		rcb.bit.console = 1;
	else
		rcb.bit.console = 0;
	#endif
	
}
#endif


void APP_Inverter_NVS_Stop_Refresh()
{
	if (app.gRemoteCommand==0)
	{
		fault.alarm.stop=1;	// 02.28.2022 DGA 
	}
	else
	{
		fault.alarm.stop=0;	// 02.28.2022 DGA 
	}
}

void APP_Inverter_Start_Connect()
{
    pwm.buck.delay = 0;
	timer.buck_sec=0;
	timer.buck_ms=0;
	app.run_sec = 0;
	timer.hours_runtime = 0;
    
    HAL_Fault_Clear(ALARM_DISC|ALARM_DIAGNOSTIC|ALARM_DC_LINK_HIGH);
	
	MPPT_Start(0);		// DGA MPPT 
    
    
//    #if defined(Uses_Debug)
//    printf("APP_Inverter_Start_Connect\n");
//    #endif
	
	#if defined(Uses_Debug_NVS)
	NVS_Tune_Print_All();
	#if defined(Uses_Debug_NVS_Fault)
	if (nvs.read_error)
	{
		fault.alarm.diagnostic=1;
	}
	#endif
	#endif

	APP_State_Set(INVERTER_CONNECT);
}

Uint16 calc_max(Uint16 v1, Uint16 v2)
{
    if (v1 > v2)
        return v1;
    
    return v2;
}


Uint16 APP_Inverter_Calc_Connection_Delay()
{
	Uint16 connection_time;
	Uint16 fault_time;

	#if defined(Uses_Debug_Start)
	#define	FAULT_CONNECTION_TIME_AC_HIGH1		50
	#define	FAULT_CONNECTION_TIME_AC_HIGH2		100
	#else
	#define	FAULT_CONNECTION_TIME_AC_HIGH1		300
	#define	FAULT_CONNECTION_TIME_AC_HIGH2		600
	#endif

	#if defined(Uses_Debug_Start)
	#define	FAULT_CONNECTION_TIME_DC_LOW1		50
	#define	FAULT_CONNECTION_TIME_DC_LOW2		100
	#else
	#define	FAULT_CONNECTION_TIME_DC_LOW1		100
	#define	FAULT_CONNECTION_TIME_DC_LOW2		300
	#endif

    
    #if defined(Uses_Debug_GEO)
	#define	FAULT_CONNECTION_TIME_FREQ1         40
	#define	FAULT_CONNECTION_TIME_FREQ2 		40
    #else
	#define	FAULT_CONNECTION_TIME_FREQ1         10
	#define	FAULT_CONNECTION_TIME_FREQ2 		300
    #endif

	
	fault_time = 0;
	if (app.fault_history.ac_high)
	{
		if (app.fault_history.ac_high <= 3)
		{
			fault_time = FAULT_CONNECTION_TIME_AC_HIGH1;
			#if defined(Uses_Debug_Start)
			if (rcb.bit.console)
				printf("AC+1 %u  %s,%d\n",fault_time, __FILE__,__LINE__);
			#endif
		}
		else
		{
			fault_time = FAULT_CONNECTION_TIME_AC_HIGH2;
			#if defined(Uses_Debug_Start)
			if (rcb.bit.console)
				printf("AC+2 %u  %s,%d\n",fault_time, __FILE__,__LINE__);
			#endif
		}
	}
	
	if (app.fault_history.dc_low)
	{
		if (app.fault_history.dc_low <= 10)
		{
			fault_time = FAULT_CONNECTION_TIME_DC_LOW1;
			#if defined(Uses_Debug_Start)
			if (rcb.bit.console)
				printf("DC+1 %u  %s,%d\n",fault_time, __FILE__,__LINE__);
			#endif
		}
		else
		{
			fault_time = FAULT_CONNECTION_TIME_DC_LOW2;
			#if defined(Uses_Debug_Start)
			if (rcb.bit.console)
				printf("DC+2 %u  %s,%d\n",fault_time, __FILE__,__LINE__);
			#endif
		}
	}

	if (app.fault_history.ac_freq)
	{
		if (app.fault_history.ac_freq <= 1)
		{
			fault_time = FAULT_CONNECTION_TIME_FREQ1;
		}
		else
		{
			fault_time = FAULT_CONNECTION_TIME_FREQ2;
		}
	}
  
	else if (app.fault_history.phase)
	{
		if (app.fault_history.phase <= 1)
		{
			fault_time = FAULT_CONNECTION_TIME_FREQ1;
		}
		else
		{
			fault_time = FAULT_CONNECTION_TIME_FREQ2;
		}
	}
	
	connection_time = calc_max(GEO_Limits.connection_time,fault_time);
		
	if( (mEngineeringMode) || (nvs.hours_console < CONSOLE_RUN_LIMIT_NORMAL))
	{
		// make connection time short for engineering work.
		connection_time = CONNECTION_TIME_ENGINEERING;
	}
	
	return connection_time;
}

void APP_Inverter_Fault_History_Reset()
{
	memset( (void*) &app.fault_history,0,sizeof(app.fault_history));
    pwm.fault_error_count=0;
}


Uint8 APP_Inverter_Fault_Other()
{
	
	if (app.gRemoteCommand==0)
	{
		fault.alarm.stop=1;	// 02.28.2022 DGA 
	}
	else
	{
		fault.alarm.stop=0;	// 02.28.2022 DGA 
	}
	
	if ( (ecb1.run) && (app.gRemoteCommand))
	{
		return FALSE;
	}
	return TRUE;
}

Uint8 APP_Inverter_is_Start_OK()
{
	if (GEO_freq.bit.lock)
	{
		if ( APP_Inverter_Fault_Other()==FALSE)
		{
			if (APP_Inverter_is_Start_Fault()==FALSE)
				return TRUE;
		}
	}
	return FALSE;
}

Uint8 APP_Inverter_is_Start_Fault()
{
	if  (HAL_Fault_Start()==0)
		return FALSE;
	return TRUE;
}

void APP_Inverter_Start_Task()
{
	#if defined(Uses_Debug_NVS_Fault)
	if (nvs.read_error)
	{
		fault.alarm.diagnostic=1;
		return;
	}
	else
		fault.alarm.diagnostic=0;
		return;
	#endif
	
	if ( APP_Inverter_is_Start_OK()==TRUE )
	{
        
//		#if defined(Uses_Debug_Start)
//		if (rcb.bit.console)
//			printf("Start_Task OK %s,%d\n",__FILE__,__LINE__);
//		#endif 

		APP_State_Set(INVERTER_DELAY);	
		timer.ac_connect_delay=0;							// Reset Timer

		app.connection_time = APP_Inverter_Calc_Connection_Delay();
	}
}

void APP_Inverter_Start_Delay_Ms1000()
{
    #if defined(Uses_Debug) || defined(Uses_Debug_Start)
	APP_Inverter_console_print_status();
	#else
	if (rcb.bit.console)
		APP_Inverter_console_print_status();
	#endif
}

void APP_Inverter_Start_Delay_Task()
{
	if ( APP_Inverter_is_Start_OK()==FALSE )
	{
		APP_State_Set(INVERTER_START);
	}
	else
	{
		if (timer.ac_connect_delay >= app.connection_time)
		{
			fault.alarm.delay = 0;
			APP_Inverter_Start_Connect();
		}
		else if (fault.alarm.delay==0)
		{
			fault.alarm.delay = 1;
		}
	}
}

Uint8 APP_Inverter_is_Run_Fault()
{
	if ((HAL_Fault_Get()) || (APP_Inverter_Fault_Other()==TRUE))
	{
   		APP_Stop_Mode();	// shutdown

		return TRUE;
	}

	return FALSE;
}

void APP_Inverter_Connecting_Task()
{
	if(APP_Inverter_is_Start_Fault() )
	{
		#if defined( Uses_Console_Run )
		if (rcb.bit.console)
			console_print_alarms("FAULT",fault.alarm);
		#endif

		APP_Stop_Mode();	// shutdown
	}

}

void APP_INVERTER_RUN_Task()
{
	if(APP_Inverter_is_Run_Fault()==FALSE)
	{
		MPPT_Task();

		if (timer.bit.run_sec)
		{
			timer.bit.run_sec=0;
        }
        
        PWM_Run_Task();
        
	}
}

#if defined( Uses_Console_Run )
char *console_get_run_state()
{
	if (app.gRemoteCommand==0)
	{
		return "INVERTER-OFF";
	}
	
	switch(APP_State_Get())
	{
		case INVERTER_STOP:
			return "STOP";
        #if defined(Uses_Diagnostics)
		case INVERTER_DIAG_MODE:
			return "DIAG";
        #endif
		case INVERTER_START:
			return "START";
		case INVERTER_DELAY:
			return "DELAY";
		case INVERTER_CONNECT:
			return "CONNECT";
		case INVERTER_RUN:
			return "RUN";
	}

	return "????";
}

void APP_Inverter_console_print_status()
{
//	[DELAY]:300
	printf("[%s]", console_get_run_state() );
		
	if (APP_State_Get()==INVERTER_DELAY)
	{
		Uint16 sec,delay;
		timer.bit.start_sec=0;
		sec = timer.ac_connect_delay/10;
		delay = app.connection_time / 10;
		printf(":%u",(delay-sec));
	}
	printf(" ");

	if ( (APP_State_Get()==INVERTER_START) && (app.gRemoteCommand))
	{
		printf("GEO:%u.%uHZ ",GEO_freq.zc.instant/10,GEO_freq.zc.instant%10);
		printf("SF:%d%d ", APP_Inverter_is_Start_OK(), APP_Inverter_is_Start_Fault() );
		
//		printf("(%u,%u,%u", GEO_freq.start.cnt, APP_AC_Freq_Query(GEO_freq.instant),GEO_freq.start.lock);
//		printf(",%d%d) ", APP_Inverter_is_Start_OK(), APP_Inverter_is_Start_Fault() );
		
	}

//	printf("HZ:%u ",GEO_freq.zc.average );
		
	printf("AC:%u ", analog.ac.avg.volts);

	printf("HR:%u ", nvs.hours_console);

	if (fault.alarm.value)
	{
//		printf("FAULT:");
		console_print_alarm_bits(fault.alarm);
		printf(" ");
	}
	else
	{
		if (APP_State_Get()==INVERTER_RUN)
		{
			printf("%uw ",pwm.watts);
            
            printf("GEO:%d L%u,%u U%u,%u",
                GEO_Config.grid_freq,
                GEO_Nvs.freq.limit.lower.herz,GEO_Nvs.freq.limit.lower.tmr1,
                GEO_Nvs.freq.limit.upper.herz,GEO_Nvs.freq.limit.upper.tmr1
            );
		}
	}
	
	printf("\n");
	
	
}
#endif

void APP_Inverter_State_Machine_Task()
{
	switch( APP_State_Get() )
	{
		case INVERTER_STOP:
		{
			APP_Stop_Mode_Task();
			break;
		}
		
		#if defined(Uses_Diagnostics)
		case INVERTER_DIAG_MODE:
		{
			APP_Inverter_Diag_Mode_Task();
			break;
		}
		#endif		

		case INVERTER_START:
		{
			APP_Inverter_Start_Task();
			break;
		}
		case INVERTER_DELAY:
		{
			APP_Inverter_Start_Delay_Task();
			break;
		}
		case INVERTER_CONNECT:
		{
			APP_Inverter_Connecting_Task();
			break;
		}
		case INVERTER_RUN:
		{
			APP_INVERTER_RUN_Task();
			break;
		}
	}
	
	if (rcb1.state_set)
	{
		#if defined( Uses_Console_Run )
		if (rcb.bit.console)
			APP_Inverter_console_print_status();
		#endif

		rcb1.state_set=0;
	}
	
}

void APP_Inverter_Run_Ms1000()
{
	app.run_sec++;
    
    app.run_clock.hours = (app.run_sec / 3600);
    app.run_clock.minutes = (app.run_sec / 60);
    app.run_clock.seconds = (app.run_sec % 60);

//	***** RUNTIME HOURS ***** 
	#define	NVS_HOURS_DIVISOR	3600
	if (++timer.hours_runtime >= NVS_HOURS_DIVISOR)
	{
		timer.hours_runtime=0;
		nvs.hours_runtime++;
		nvs.hours_console++;
		NVS_Runtime_Hours_Write(nvs.hours_runtime);
		NVS_Console_Hours_Write(nvs.hours_console);
	}
//	***** RUNTIME HOURS ***** 	
	
	#if defined(Uses_Debug_Start)
	#define FAULT_HISTORY_RESET_TIME		180
	#else
	#define FAULT_HISTORY_RESET_TIME		600
	#endif
	
	// DGA Inverter running OK for timer length ?
	if ((rcb.bit.fault_history_reset==0) && (app.run_sec >= FAULT_HISTORY_RESET_TIME))	
	{
		rcb.bit.fault_history_reset=1;		// DGA set bit 
		APP_Inverter_Fault_History_Reset();
	}
    
//  ***** RUN TEST CODE     
    #if defined(Uses_Debug)
    if (dcb.test_phase_error_sec)
    {   
        Uint16 sec_timer;
        sec_timer = dcb.test_phase_error_sec - app.run_sec;
        printf("PHASE FAULT TEST %us\n",sec_timer);
        if (app.run_sec >= dcb.test_phase_error_sec)
        {
            HAL_Fault_Set(ALARM_DISC|ALARM_PHASE,FC_TRIG_LO_POS,1234,5678);
        }
        
            
    }
    #endif
    
}

void APP_Inverter_State_Machine_Ms1000()
{
    
    #if defined(Uses_Debug)
    if (dcb1.run_state)
    {
        printf("STATE=%d,%s GEO:%d,%d ",APP_State_Get(), console_get_run_state(APP_State_Get()),GEO_freq.bit.lock,GEO_freq.zc.start.lock);
        console_print_alarm_bits(fault.alarm);
        printf("\n");
    }
    #endif
	
	switch( APP_State_Get() )
	{
		case INVERTER_STOP:
		{
//			printf("STOP RUN=%d  UFB=%d  %s,%d\n",ecb1.run, diag.ufb_sec,__FILE__,__LINE__);
//			APP_Stop_Mode_Ms1000();
			break;
		}
		
		#if defined(Uses_Diagnostics)
		case INVERTER_DIAG_MODE:
		{
			break;
		}
		#endif		

		case INVERTER_START:
		{
			break;
		}
		case INVERTER_DELAY:
		{
			APP_Inverter_Start_Delay_Ms1000();
			break;
		}
		case INVERTER_CONNECT:
		{
			break;
		}
		case INVERTER_RUN:
		{
			APP_Inverter_Run_Ms1000();
			break;
		}
	}
}
