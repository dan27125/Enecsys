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

#if defined(Uses_Inverter)

void HAL_Fault_Send_Report(Uint8 force)
{
	// If force or the alarm state has changed, send the report.
	if ((force!=0) || (fault.telem.value != fault.alarm.value))
	{
		// If force or the alarm state has changed, send the report.
		ZNet_SendNormalReport(REPORT_TYPE_ALARM);
		fault.telem.value = fault.alarm.value;
	}
}

#define	CALC_FAULT_DELAY_SECONDS(x)	(x*10)


void HAL_Fault_Clear(AlarmState mask)
{
	fault.alarm.value &= (~(Uint16) mask);
}

//    __builtin_disi(0x3FFF); /* disable interrupts for 16383 cycles */
//    __builtin_disi(0x0000); /* enable interrupts */


void HAL_Fault_Set(AlarmState mask, Uint16 error_code, Uint16 value1, Uint16 value2)
{
    #if defined(DOCUMENTATION) 
    if ((fault.alarm.grid_low==0) && (fault.alarm.freq_high==0) && (fault.alarm.freq_low==0))    // 04/24/23 DGA - Change to mutually exclusive            
    #endif

    __builtin_disi(1000);         //disable interrupts for 1000 cycles 
    PWM_BUCK_LATCH_OFF();
    PWM_SINE_DUTY=0;
    IO_PIN_UFB_LIVE_LOW_LATCH=0;
    IO_PIN_UFB_LIVE_HIGH_LATCH=0;
    ufb_state=H_BRIDGE_DISABLED;

	fault.alarm.value |= (Uint16) mask;
    fault.info.code1 = error_code;
    fault.info.code2 = error_code;
    fault.info.param1 = value1;
    fault.info.param2 = value2;
    
    #if defined(Uses_Debug_Phase)
//    trace.phase.fault_code = error_code;
//    trace.phase.fault_timer1 = timer1;
//    trace.phase.fault_timer2 = timer2;
    #endif

    APP_Stop_Mode();

    #if defined(Uses_PWM_Ripple_Fault)
    if ((error_code==FC_DCV_RIPPLE1) || (error_code==FC_DCV_RIPPLE2) || (fault.alarm.dclink_high))
    {
        fault.info.ripple.code = error_code;
        fault.info.ripple.run.sample = value1;
        fault.info.ripple.limit.sample = value2;
        fault.info.ripple.error_count = pwm.sine_error_count;
        fault.info.ripple.error_limit = pwm.sine_error_limit;
        fault.info.ripple.sample_counter = pwm.sample_counter;
        fault.info.ripple.watts = pwm.watts;
        fault.info.ripple.rip_calc = pwm.rip.calc;
    }
    #endif
    
    __builtin_disi(0x0000);     // enable interrupts
}


#if defined(Uses_Console_Fault)

char * console_fault_text(FAULTCODE code)
{
    switch(code)
    {
        case FC_TRIG_REV_POS:
        {
            return "TRIG_REV_POS";
        }
        case FC_TRIG_REV_NEG:
        {
            return "TRIG_REV_NEG";
        }
        case FC_TRIG_LO_POS:
        {
            return "TRIG_LO_POS";
        }
        case FC_TRIG_LO_NEG:
        {
            return "TRIG_MIN_NEG";
        }
        case FC_TRIG_MAX_POS:
        {
            return "TRIG_MAX_POS";
        }
        case FC_TRIG_MAX_NEG:
        {
            return "TRIG_MAX_NEG";
        }
        case FC_RANGE_POS:
        {
            return "RANGE_POS";
        }
        case FC_RANGE_NEG:
        {
            return "RANGE_NEG";
        }
        case FC_ADC_SURGE:
        {
            return "ADC_SURGE";
        }
        case FC_GEO_ZC_FREQ_LO:
        {
            return "GEO_FREQ_LO";
        }
        case FC_GEO_ZC_FREQ_HI:
        {
            return "GEO_FREQ_HI";
        }
        case FC_ACV_HI:
        {
            return "AC+HI";
        }
        case FC_ACV_LO:
        {
            return "AC-LO";
        }
        case FC_TEMP_HOT:
        {
            return "TEMP-HOT";
        }
        case FC_TMR5_POS:
        {
            return "TMR5-POS";
        }
        case FC_TMR5_NEG:
        {
            return "TMR5-NEG";
        }
        case FC_DCV_LO:
        {
            return "DCL";
        }
        
        case FC_DCV_RIPPLE1:
       {
            return "DC-RIP1";
        }
        
        case FC_DCV_RIPPLE2:
        {
            return "DC-RIP2";
        }
        
        
    }
    
    return TEXT_ERROR;
}

void print_timer1_freq(Uint16 timer, Uint8 hz)
{
//   printf("%s: %s:%u,%u,%u,%u  %s:%u,%u,%u,%u \n", 
//   TEXT_50HZ, GEO_Nvs.freq.F50.lower.herz,GEO_Nvs.freq.F50.lower.tmr1,GEO_Nvs.freq.F50.upper.herz,GEO_Nvs.freq.F50.upper.tmr1,
  
}


Uint8 is_fault_type_freq(Uint8 type)
{
    switch(type)
    {
        case FC_GEO_ZC_FREQ_LO:
        case FC_GEO_ZC_FREQ_HI:
        {
            return TRUE;
        }
    }
    return FALSE;
}

Uint8 is_fault_type_dc_ripple(Uint8 type)
{
    switch(type)
    {
        case FC_DCV_RIPPLE1:
        case FC_DCV_RIPPLE2:
        {
            return TRUE;
        }
    }
    return FALSE;
}


Uint16 console_fault_print_item(Uint8 item)
{
    NVS_Fault_Hist x;
    Uint8 sz;
    sz = 0;
    
    if (NVS_Fault_Hist_Read(item,&x))
    {
        if (is_fault_type_freq(item)==TRUE)
        {
            Uint16 fr1,fr2;
        
            fr1  = TIMER1_CALC_10X(x.item1);
            fr2  = TIMER1_CALC_10X(x.item2);
            
            sz = printf("F%d:%d,%d=%u.%u,%d=%u.%u ",item,x.count,
                x.item1, fr1/10,fr1%10,
                x.item2, fr2/10,fr2%10);
        }
        else if (is_fault_type_dc_ripple(item)==TRUE)
        {
            //  RIP 1,567,422
            sz = printf("DC+ %u,%u ",x.item1,x.item2);
        }
        else
            sz = printf("F%d:%d,%d,%d ",item,x.count,x.item1 ,x.item2);
        
    }
    return sz;
}

void console_fault_print()
{
    Uint8 bytes, count,i;
    count = 0;
    
    bytes = printf("%s ",TEXT_FAULT);
    
    for (i = 1; i <= NVS_FAULT_COUNTER_TOTAL; i++)
    {
        Uint8 sz = console_fault_print_item(i);
        if (sz)
        {
            bytes += sz;
            count++;

            if (bytes >= 55)
            {
                printf("\n");
                bytes=0;
            }
        }
    }
    if (count==0)
        printf("%s,0,CLEAR\n",TEXT_NONE);
    else
        printf("\n");
    
//  printf("console_fault_print()\n");    
    
}


Uint8 console_fault_parse_args()
{
    if ( strcmp(TEXT_FAULT, (char*)arg1)==0)
    {
        if ( strcmp(TEXT_SHOW, (char*)arg2)==0)
        {
//            printf("DAN %s %s\n",arg1,arg2);
            console_fault_print();
        }

        else if (strcmp("RIP", (char*)arg2)==0)
        {
            if (console_scan_cmd_on(arg3,arg2)==TRUE)
            {
                tune.fault.bit.fault_ripple = 1;
                NVS_fault_bits_write(tune.fault.bit.value);
            }
            else if (console_scan_cmd_off(arg3,arg2)==TRUE)
            {
                tune.fault.bit.fault_ripple = 0;
                NVS_fault_bits_write(tune.fault.bit.value);
            }
            
            else if (strcmp("DIV", (char*)arg3)==0)
            {
                if (csib.value[3])
                {
                    NVS_ripple_divsor_write(csib.value[3]);
                }
                printf("%s %s %s %u\n",arg1,arg2,arg3,tune.fault.ripple_div);
            }
            else if (strcmp("DC1", (char*)arg3)==0)
            {
                if (csib.value[3])
                {
                    tune.fault.ripple_dc1 = csib.value[3];
                }
                printf("%s %s %s %u\n",arg1,arg2,arg3,tune.fault.ripple_dc1);
            }
            else if (strcmp("DC2", (char*)arg3)==0)
            {
                if (csib.value[3])
                {
                    tune.fault.ripple_dc2 = csib.value[3];
                }
                printf("%s %s %s %u\n",arg1,arg2,arg3,tune.fault.ripple_dc2);
            }
            else if (strcmp("DC3", (char*)arg3)==0)
            {
                if (csib.value[3])
                {
                    tune.fault.ripple_dc3 = csib.value[3];
                }
                printf("%s %s %s %u\n",arg1,arg2,arg3,tune.fault.ripple_dc3);
            }
            

        }
        
        else if ( strcmp(TEXT_RESET, (char*)arg2)==0)
        {
            NVS_Fault_Hist_Reset_All();
            console_fault_print();
        }
  
/*        
        else if ( strcmp(TEXT_SET, (char*)arg2)==0)
        {
  //        printf("%s %d %d\n",arg2,csib.value[2], csib.value[3]);
            
            if ( strcmp("PAT", (char*)arg2)==0)
            {
                Uint8 i;
                for (i = 1; i <= NVS_FAULT_COUNTER_TOTAL; i++)
                {
//                  NVS_Fault_Counter_Write(i,i);
                    console_fault_print();
                }
            }
            else if ((csib.value[2] > 0) && (csib.value[2] <= NVS_FAULT_COUNTER_TOTAL))
            {
//              NVS_Fault_Counter_Write(csib.value[2],csib.value[3]);
                console_fault_print();
            }
           
        }
*/
        
        else if ( strcmp("INC", (char*)arg2)==0)
        {
//          printf("%s %d %d %d\n",arg2, csib.value[2],csib.value[3],csib.value[4]);
            NVS_Fault_Hist_Increment(csib.value[2],csib.value[3],csib.value[4]);
            console_fault_print();
        }
        
        #if defined(Uses_Console_Fault_Test)
        else if ( strcmp(TEXT_TEST, (char*)arg2)==0)
        {
            if ( strcmp(TEXT_FREQ, (char*)arg3)==0)
            {
                fault.test.alarm.freq_high=1;
                fault.test.freq_value = TIMER1_CALC_10X(csib.value[3]);
                printf("%s %s %s %u @ %u\n",arg1,arg2,arg3,csib.value[3], fault.test.freq_value);
            }
            if ( strcmp(TEXT_PHASE, (char*)arg3)==0)
            {
                #if defined(Uses_Debug)
                if (csib.value[3])
                {
                    dcb.test_phase_error_sec = csib.value[3];
                    printf("test_phase_error=%u\n",csib.value[3]);
                }
                else
                {
                    // 12500  12345  67890
                    dcb.test_phase_error_sec = 0;
                    HAL_Fault_Set(ALARM_DISC|ALARM_PHASE,FC_TRIG_LO_POS,1234,5678);
                }
                #else
                HAL_Fault_Set(ALARM_DISC|ALARM_PHASE,FC_TRIG_REV_POS,1234,5678);
                #endif
            }
            
        }
        #endif
        
        
        
        
        else
            console_fault_print();
        
    }
    
	else
		return 0;
	
	return 1;
    
}
#endif



#endif