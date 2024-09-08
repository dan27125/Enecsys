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


#if !defined(  _HAL_CONSOLE_C )
#define	_HAL_CONSOLE_C

#include "APP_Inverter.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#if defined(Uses_Console)

Uint8 console_parse_args()
{
	if (strcmp((char*)"RUN", (char*)arg1)==0)
	{
		#if defined(Uses_Diagnostics)
		APP_Inverter_Diag_Mode_Stop();
		#endif
		
		if (ecb1.run==0)
		{
			APP_Stop_Mode();	// DGA WIFI ZIGBEE cmd
			ecb1.run = 1;
			NVS_RCB_Write();
		}
		console_print_bit((char*) arg1,ecb1.run);
	}
	else if (strcmp((char*)"STOP", (char*)arg1)==0)
	{
		#if defined(Uses_Diagnostics)
		APP_Inverter_Diag_Mode_Stop();
		#endif

		ecb1.run = 0;
        NVS_RCB_Write();
		APP_Stop_Mode();	
	}
	
	else if (strcmp((char*)TEXT_CLOCK, (char*)arg1)==0)
	{
		rcb.bit.console_clock = ! rcb.bit.console_clock;
	}
	
	else if ( strcmp((char*)TEXT_REPORT, (char*)arg1)==0)
//	{
//		printf("%s\n",arg1);
		ZNet_SendNormalReport(REPORT_TYPE_NORMAL);
//	}
	
	else if ( strcmp((char*)TEXT_VER, (char*)arg1)==0)
	{
		console_print_version();
	}
	else if ( strcmp((char*)TEXT_RESET, (char*)arg1)==0)
	{
		printf("%s\n",arg1);
		APP_Stop_Mode();
		APP_delay_miliseconds(100);
		asm("RESET");		// DGA trigger a reset instruction 
	}
	else if ( strcmp(TEXT_LIMIT, (char*)arg1)==0)
	{
		Uint16 value = csib.value[1];
		printf("%s %u\n",arg1,value);
		if (value)
		{
			PWM_Power_Limit_Set(value);
		}
	}
	#if defined(Uses_Clock_Lock)
	else if ( strcmp("FREQ", (char*)arg2)==0)
	{
		PWM_Console_Clock_Freq_Set( csib.value[2] );
	}
		
	else if ( strcmp("DUTY", (char*)arg2)==0)
	{
		PWM_Console_Clock_Duty_Set(csib.value[2]);
	}
	#endif

	#if defined(Uses_AMP_Lock)
	else if ( strcmp("AMP", (char*)arg1)==0)
	{
		PWM_Console_Amp_Lock_Set( csib.value[1] );
	}
    
    
  	else if ( strcmp("SQW", (char*)arg1)==0)
	{
         if ((csib.value[1] > 1) && (csib.value[1] < 99))
         {
            dcb.pwm.clock_duty_lock = csib.value[1];
            PWM_CLOCK_PRX = PWM_CLOCK_PRX_200K;
            PWM_Clock_Duty_Set(dcb.pwm.clock_duty_lock);
            
/*            
            printf("LCK:%d PRX:%u OCX,%d,%d\n",
                    dcb.pwm.clock_duty_lock, 
                    PWM_CLOCK_PRX,
                    PWM_CLOCK_DUTY,
                    PWM_GET_OCX_DUTY(PWM_CLOCK_PRX,PWM_CLOCK_DUTY)
            );	
*/            
         }
         else
         {
             dcb.pwm.clock_duty_lock = 0;
             printf("%s PWM lock OFF\n",arg1);	
         }
         printf("%s CLOCK DUTY:%d%% \n",arg1,PWM_GET_OCX_DUTY(PWM_CLOCK_PRX,PWM_CLOCK_DUTY));	
    }
	#endif
	
	#if defined(Uses_Console_Temperature)
	else if ( strcmp(TEXT_TEMP, (char*)arg1)==0)
	{
		if ( strcmp(TEXT_HOT, (char*)arg2)==0)
		{
			Uint16 temp;
			temp = Bounds_Check_Uint16(csib.value[2],TUNE_TEMP_HOT_MIN,TUNE_TEMP_HOT_MAX);
			NVS_VID_Write_Item(VID_TEMPERATURE_HOT, temp);
			printf("%s %s %u\n",TEXT_TEMP,TEXT_HOT,tune.temperature.hot_on);
		}
		
		else if ( strcmp(TEXT_AC, (char*)arg2)==0)
		{
			temperature.bit.ac_adjust = !temperature.bit.ac_adjust;
			printf("%s=%d\n",arg2,temperature.bit.ac_adjust);
		}
       
		#if defined(Uses_Debug_Temperature)
		else if ( strcmp(TEXT_DEBUG, (char*)arg2)==0)
		{
			dcb2.temperature = !dcb2.temperature;
		}
		#endif
		else
		{
			printf("%s %s %u\n",TEXT_TEMP,TEXT_HOT,tune.temperature.hot_on);
		}
	}
	#endif
	
    else if ( strcmp("CONSOLE", (char*)arg1)==0)
    {
		rcb.bit.console_cmd = ! rcb.bit.console_cmd;
    }
    
	else
		return 0;
	
	return 1;
}

void console_handle_input(Uint8 *argv)
{
	Uint8 rc;
	memset(&csib,0,sizeof(csib));
	
	if (console_scan_string_raw(argv, &csib))
	{
		return;
	}
	
//	printf("%s,%d  Sz=%d [%s]\n", __FILE__, __LINE__, sib.size, arg1);
	
	rc = console_parse_args();
	
	#if defined(Uses_MPPT_Console)
	rc += MPPT_console_parse_args();
	#endif

	#if defined(Uses_NVS_Console) || defined(Uses_Debug_NVS)
	rc += NVS_console_parse_args();
	#endif
	
	#if defined(Uses_Diagnostics)
	rc += console_parse_args_diagnostics();
	#endif
	
	#if defined(Uses_ACV_Console)
	rc += ACV_console_parse_args();
	#endif
	
    
    #if defined(Uses_GEO_Console)
    rc += GEO_console_parse_args();
    #endif

    #if defined(Uses_Console_Fault)
    rc += console_fault_parse_args();
    #endif

	#if defined(Uses_Debug) 
	rc += Debug_console_parse_args();
	#endif
	
	if (rc==0)
	{
		printf("%s %s\n",TEXT_ERROR,arg1);
	}
	
}

#endif	// Uses_Console
	
#endif		//	#if !defined(  _HAL_CONSOLE_C )