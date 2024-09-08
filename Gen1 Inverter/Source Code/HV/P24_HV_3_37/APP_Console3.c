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

#if defined( Uses_Console )


Uint8 console_scan_cmd_on(Uint8 *str1, Uint8 *str2)
{
   
    if (
            (strcmp("ON", (char*)str1)==0) || (strcmp("=ON", (char*)str1)==0)
            || (strcmp("1", (char*)str1)==0) || (strcmp("=1", (char*)str1)==0)
       )
    {

        printf("%s%s\n",str2,"=ON");
        return TRUE;
    }
  
    return FALSE;
}

Uint8 console_scan_cmd_off(Uint8 *str, Uint8 *desc)
{
    if ((strcmp("OFF", (char*)str)==0) || (strcmp("=OFF", (char*)str)==0) || 
       (strcmp("0", (char*)str)==0) || (strcmp("=0", (char*)str)==0) )
    {
        printf("%s%s\n",desc,"=OFF");
        return TRUE;
    }
    return FALSE;
}

char console_scan_command(TString_Info_Block *sip, char mode, Uint8 set)
{
	Uint8 *arg = sip->str[1];
	
	if (sip->cnt == 2)
	{
		if ( (strcmp(TEXT_ON, (char*)arg)==0) ||
			(strcmp("1", (char*)arg)==0) )
		{
			set = 1;
		}
		else if ( (strcmp(TEXT_OFF, (char*)arg)==0) ||
			(strcmp("0", (char*)arg)==0) )
		{
			set = 0;
		}

	}
	else if (sip->cnt == 1)
	{
		if (set)
			set = 0;
		else 
			set = 1;
	}
	
	console_print_bit( (char*) sip->str[0], set);
	return set;
}


#if defined(Uses_Console_Run)
void console_print_alarm_bits(Alarm_Fault_Bits bit)
{
	#if defined(DOCUMENTATION)
	#endif

//	printf("ALARM:%x",fault.alarm.value);	
	printf("%04x",fault.alarm.value);	
	
	if (bit.disc)
		printf(",%s",TEXT_DISC);
	
	if (bit.grid_high)
		printf(",%s+",TEXT_AC);
	
	if (bit.grid_low)
		printf(",%s-",TEXT_AC);
	
	if (bit.freq_high)
		printf(",%s+",TEXT_FREQ);
	
	if (bit.freq_low)
		printf(",%s-",TEXT_FREQ);
	
	if (bit.island)
		printf(",%s",TEXT_ISLAND);
	
	if (bit.temp_hot)
		printf(",%s",TEXT_HOT);
	
	if (bit.temp_cold)
		printf(",%s",TEXT_COLD);

	if (bit.temp_range)
		printf(",%s",TEXT_TEMP);

	if (bit.delay)
		printf(",%s","DELAY");
	
	if (bit.dclink_low)
		printf(",%s",TEXT_HVDC);
	
	if (bit.dclink_high)
		printf(",%s+",TEXT_HVDC);
	
	if (bit.phase)
		printf(",%s",TEXT_PHASE);
	
	if (bit.surge)
		printf(",%s ",TEXT_SURGE);	
	
	if (bit.diagnostic)
		printf(",%s",TEXT_DIAG);
	
	if (bit.stop)
		printf(",%s",TEXT_STOP);
	
}

void console_print_alarms(char *desc, Alarm_Fault_Bits bit)
{
	if (desc)
		printf("[%s] ",desc);
	console_print_alarm_bits(bit);

//  #if defined(Uses_Debug_GEO_FREQ_Fault)    
    if ((bit.freq_low) || (bit.freq_high))
    {
        printf(" FR:%u.%u TMR:%u,%u CNT:%u",
            fault.info.hertz_10x/10,
            fault.info.hertz_10x%10,
            fault.info.param1,
            fault.info.param2, 
            app.fault_history.ac_freq
        );
    }
//  #endif
    
	printf("\n");
}

#endif	// Uses_Console_Alarms



#endif		//	Uses_Console
