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


#if defined(Uses_Debug) || defined(Uses_AMP_Lock)

#if defined ( Uses_Debug )
void Debug_trace_cnt_clear()
{
	memset((void*)&trace,0,sizeof(trace) );	// clear all params
}
#endif


Uint8 Debug_console_parse_args()
{
//  printf("parse_args_debug %s %s %s\n",arg1,arg2,arg3);
    
    if ( strcmp(TEXT_DEBUG, (char*) arg1))
        return 0;
    
    #if defined(Uses_Debug_Phase)
    else if (strcmp(TEXT_PHASE, (char*)arg2)==0)
    {
        if ( strcmp(TEXT_TRIG, (char*)arg3)==0)
        {
            dcb1.phase_trig = !dcb1.phase_trig;
        }
        else if ( strcmp(TEXT_RANGE, (char*)arg3)==0)
        {
            dcb1.phase_range = !dcb1.phase_range;
        }
        else if ( strcmp("ISR", (char*)arg3)==0)
        {
            dcb1.phase_isr_error = !dcb1.phase_isr_error;
        }
    }
    #endif

    else if (strcmp(TEXT_RUN, (char*)arg2)==0)
    {
        dcb1.run_state=!dcb1.run_state;
    }
    
	else if ( strcmp("ACV", (char*) arg2)==0)
	{
		dcb3.acv = !dcb3.acv;
//		printf("%s %d\n",arg1,dcb2.acv);
	}
    
	else if ( strcmp("ACV2", (char*) arg2)==0)
	{
		dcb3.acv2 = !dcb3.acv2;
//		printf("%s %d\n",arg1,dcb2.acv);
	}
    

	else if ( strcmp("ACW", (char*) arg2)==0)
	{
		dcb2.acw = !dcb2.acw;
	}

	else if ( strcmp("EFF", (char*) arg2)==0)
	{
		dcb2.eff = !dcb2.eff;
	}
   
    
//	else if ( strcmp("SCALE", arg1)==0)
//	{
//		Uint16 amp = csib.value[1];
//	}

    #if defined(Uses_Debug_PWM)
	else if ( strcmp("PWM1", (char*)arg2)==0)
	{
		dcb1.pwm1 = ~dcb1.pwm1;
//      printf("FUZZY %s %d\n",arg2,dcb1.pwm1);
	}

	else if ( strcmp("PWM2", (char*)arg2)==0)
	{
		dcb1.pwm2 = ~dcb1.pwm2;
//		printf("FUZZY %s %d\n",arg2,dcb1.pwm2);
	}
    #endif
    
	else if ( strcmp("RPC", (char*)arg2)==0)
	{
		dcb1.rcp = !dcb1.rcp;
		printf("%s %d\n",arg1,dcb1.rcp);
	}

    
    
/*	
	#if defined(Uses_Debug_ADC)
	else if ( strcmp("ADC", (char*)arg2)==0)
	{
        if ( strcmp("ALL", (char*)arg3)==0)
        {
            dcb2.adc_all = !dcb2.adc_all;
        }
        else if ( strcmp("AC", (char*)arg3)==0)
        {
            dcb2.adc_ac = !dcb2.adc_ac;
        }
    }
    #endif
*/
 
    #if defined(Uses_Debug_ADC_PWM)
    else if (strcmp("SINE", (char*)arg2)==0)
    {
        if (strcmp("ALL", (char*)arg3)==0)
        {
            printf("ADC SINE REPORT ADCS=%d\n",AD1CON3bits.ADCS);
            debug_print_adc_sine = 1;
            dcb2.adc_ac = 0;
        }
        else
        {
            printf("ADC SINE %d\n",csib.value[3]);
            console_print_adc_sine_item( csib.value[3] );
        }
    }
    #endif
        
    
    #if defined(Uses_Debug_ADC_PWM)
    else if ((strcmp("AC", (char*)arg2)==0) && (strcmp("SINE", (char*)arg3)==0))
    {
        printf("SHOW AC SINE %d\n",csib.value[3]);
        console_print_adc_sine_item( csib.value[3] );
    }
    #endif

	#if defined(Uses_Debug_Start)
	else if ( strcmp("STATUS", (char*)arg1)==0)
	{
		dcb2.start_logic = console_scan_command(&csib, 0, dcb2.start_logic);
	}
	#endif

	#if defined(Uses_Debug_KWH)	
	else if ( ( strcmp(TEXT_KWH, (char*)arg2)==0) && ( strcmp(TEXT_DEBUG, (char*)arg3)==0))
	{
		dcb2.kwh = !dcb2.kwh;
		console_print_bit(TEXT_KWH,dcb2.kwh);
	}
	#endif

	
	#if defined(Uses_Debug_Freq)
	else if (strcmp("FREQ", (char*)arg2)==0)
	{
		dcb2.freq = !dcb2.freq;
	}
	#endif
	
	#if defined(Uses_Debug_NVS_SPI)
	else if (strcmp("SPI", (char*)arg2)==0)
	{
		if (strcmp("DEBUG", (char*)arg3)==0)
		{
			dcb2.spi_debug = !dcb2.spi_debug;
		}
		else if (strcmp("STATUS", (char*)arg3)==0)
		{
			dcb2.spi_status = !dcb2.spi_status;
			printf("%s=%d\n",arg2,dcb2.spi_status);
		}
		else if (strcmp("WRITE", (char*)arg3)==0)
		{
			NVS_Format(0, csib.value[2] );
		}
		
	
	}
    #endif
	

    #if defined(Uses_Debug_UFB)
	else if ( strcmp("UFB", (char*)arg2)==0)
	{
        debug_ufb_toggle();
	}

    #endif

    #if defined(Uses_Debug_DCV)
	else if ( strcmp("DC1", (char*)arg2)==0)
	{
        dcb3.dc1 = !dcb3.dc1;
	}
	else if ( strcmp("DC2", (char*)arg2)==0)
	{
        dcb3.dc2 = !dcb3.dc2;
	}
	else if ( strcmp("DC3", (char*)arg2)==0)
	{
        dcb3.dc3 = !dcb3.dc3;
	}
  
	else if ( strcmp("DCF", (char*)arg2)==0)
	{
        dcb3.fault = !dcb3.fault;
	}
    
    #endif
	else
		return 0;
	
	return 1;
}

void console_print_list(Uint16* list, Uint8 size, char *desc)
{
	Uint8 i,cnt,lf;
	cnt=0;
	lf=0;
	
	for(i=0; i < size; i++)
	{
		Uint16 value = list[i];
		if (value)
		{
			if ((desc) && (cnt==0))
				printf("%s ", desc);
			
			printf("%.2d=%u ",i,value);  // 4=22
			cnt++;
			lf++;
			
			if (lf >= 12)
			{
				printf("\n");
				lf = 0;
				
				if (desc)
					printf("%s ", desc);
			}
		}
	}

	if ( (lf) || (desc))
	{
		printf("\n");
	}
}

void console_print_list1(Uint16* list, Uint8 size)
{
	console_print_list(list,size,0);
}

void console_print_faults()
{
    
	#if defined(Uses_Debug_ZC)
	if (trace.zc.error1.cnt )
	{
		printf("ZC TRAP-1 ISR:%u TMR:%u\n",
			trace.zc.error1.cnt, trace.zc.error1.timer
		);
		trace.zc.error1.cnt=0;
	}
	
	if ( trace.zc.error2.cnt )
	{
		printf("ZC TRAP-2 ISR:%u TMR:%u\n",
			trace.zc.error2.cnt, trace.zc.error2.timer 
		);
		trace.zc.error2.cnt=0;
	}
	#endif

	#if defined(Uses_Debug_Phase)

	if (phase.error_cnt)
	{
		printf("PHASE-2 ERR:%u POS:%u NEG:%u LOCK:%u \n",
			phase.error_cnt, phase.error_pos, phase.error_neg, phase.lock_cnt
		);
		phase.error_cnt=0;
		phase.error_neg=0;
		phase.error_pos=0;
	}
	#endif
}

#if defined(Uses_Debug_Phase)

#define TIMER1_CALC_MICROSEC(x)         (Uint16)(((Uint32) x * 10) / 15)

void console_print_phase_trig()
{
	Uint16 avgp;
    Uint16 avgnz,avgnp,avgn;
    
    avgp=0;
    avgnz=0;
    avgnp=0;
    avgn=0;
    
    if (trace.phase.pos.cnt)
    {
        avgp = trace.phase.pos.tmr / trace.phase.pos.cnt;
//      pus = TIMER1_CALC_MICROSEC(avgp);
    }
    
    if (trace.phase.neg.cnt)
    {
        avgnz = trace.phase.neg.tmr0 / trace.phase.neg.cnt;
        avgnp = trace.phase.neg.tmr / trace.phase.neg.cnt;
        avgn = avgnp-avgnz;
//      nus = TIMER1_CALC_MICROSEC(avgn);
    }
    
//  printf("TRIG: ");
    
    printf("POS:%u %u,%u,A:%u ",            
       trace.phase.pos.cnt,
       trace.phase.pos.tmin,trace.phase.pos.tmax, avgp
    );
    
   	printf("NEG:%u %u,%u,A:%u,%u",
            trace.phase.neg.cnt,
            trace.phase.neg.tmin-avgnz,trace.phase.neg.tmax-avgnz,avgn,avgnz
    );

	printf("\n");
}

void console_print_phase_range()
{
    printf("RANGE");
    printf(" POS:%u,%u ", trace.phase.pos.range_min,trace.phase.pos.range_max);
    printf(" NEG:%u,%u ", trace.phase.neg.range_min,trace.phase.neg.range_max);
	printf("\n");
}

void console_print_phase_int1()
{
    if (dcb.int1_phase.count)
    {
        printf("**** ISR-1 **** CNT:%u TMR:%u,%u TRG:%d,%d Z:%d \n",
            dcb.int1_phase.count,
            dcb.int1_phase.timer1,
            dcb.int1_phase.timer2,
            dcb.int1_phase.trig_neg,
            dcb.int1_phase.trig_pos,
            dcb.int1_phase.trig_isr
        );
        memset((void*)&dcb.int1_phase.count,0,sizeof(dcb.int1_phase.count));
    }

}

void console_print_phase()
{
    static Uint8 div = 0;
    static Uint8 fault_code = 0;

//    console_print_phase_int1();
    
    if (++div < 0)
        return;
    div = 0;

    if ((trace.phase.fault_code) || (fault_code))
    {
        fault_code = trace.phase.fault_code;
        printf("Debug Fault:%u TMR:%u,%u\n",trace.phase.fault_code,trace.phase.fault_timer1,trace.phase.fault_timer2);
        return;
    }
    console_print_phase_trig();
    
    if (dcb1.phase_range)
        console_print_phase_range();
    
    memset((void*)&trace.phase,0,0);
}
#endif

#if defined(Uses_Debug_DcLimits)
void console_print_dclimits()
{
	if (buck_pwm_enable)
		console_print_list( (Uint16*) &trace.dcl_limits[0], 20, "DCL");
}
#endif


#if defined(Uses_Debug_Temperature)
void console_print_temperature()
{
	printf("TEMPERATURE:%u", temperature.dcnt);
	printf(" F%d C%d", temperature.fahrenheit,temperature.celsius);
	printf(" VDROP:%d",temperature.ac_vdrop);
    
	printf(" HOT:%d ", temperature.hot.count.on );	
		
	printf("\n");
	
	temperature.dcnt = 0;
	
}	
#endif


#if defined(Uses_Debug_ADC)
void console_print_adc_all()
{
//	printf("ADC:%u ",trace.analog.cnt_isr1);
//	printf("ZC:%u ",trace.cnt_zc);
//	analog.grid.phase_cnt2
//	printf("AC:%uv,%u,%u,%u,%u ",analog.grid.avg_voltage,trace.analog.ac.zc, trace.analog.ac.samples,trace.analog.ac.average,trace.analog.ac.flag);
    
	printf("AC:%uv,%u,%u,P%u,%u ",analog.grid.avg_voltage,
            analog.grid.avg_sample, 
            trace.analog.ac.zc,
            analog.grid.phase_cnt2,
            analog.grid.phase_max2
   );
	
	if (trace.analog.ac.timeout1)
		printf("TO:%u,%u ",trace.analog.ac.timeout1,trace.analog.ac.timeout2);
	
	printf("DC:%u,%u,%u ",analog.dc.voltage, analog.dc.voltage_avg2,analog.dc.sample_avg1);
	
	printf("PV:%u,%u ",DCLINK_CALC_PVX10(analog.dc.voltage), DCLINK_CALC_PVX10_IDLE(analog.dc.voltage) );	
	
	printf("TEMP:%df,%d,%d ",temperature.fahrenheit,trace.analog.temp.sample_cnt1,trace.analog.temp.sample_cnt2);	

	printf("\n");
	
}


#if defined(Uses_Debug_ADC_PWM)
void console_print_adc_ac()
{
//  ISR:8691 LOW:4348 MAX:0     
//        trace.analog.ac.phase_neg_cnt,
//        ADC_Get_ACV(trace.analog.ac.phase_low_adc_max)
  
//    ADC:8527 0:4300,89,24997 1:28,257,22762 3:4190,269,12638
//    ADC:8526 0:4298,86,25026 1:17,156,20244 3:4195,267,12618
//    ADC:8540 0:4311,93,24994 1:18,12609,21517 3:4197,264,12611
    
    printf("ADCS:%d,%u",AD1CON3bits.ADCS, trace.analog.cnt_isr1);
    
//    printf(" NEG:%u,%u,%u",trace.analog.ac.phase_neg_cnt, trace.analog.ac.phase_neg_tmr_min,trace.analog.ac.phase_neg_tmr_max);
//    printf(" MAX:%u",trace.analog.ac.phase_neg_adc_max);
//    printf(" POS:%u,%u,%u",trace.analog.ac.pos.cnt, trace.analog.ac.pos.tmr_min,trace.analog.ac.pos.tmr_max);
    
    printf(" 0:%u,%u,%u",trace.analog.ac.adc0.cnt, trace.analog.ac.adc0.tmr_min,trace.analog.ac.adc0.tmr_max);
    printf(" 1:%u,%u,%u",trace.analog.ac.adc1.cnt, trace.analog.ac.adc1.tmr_min,trace.analog.ac.adc1.tmr_max);
    printf(" 3+:%u,%u,%u",trace.analog.ac.adc3.cnt, trace.analog.ac.adc3.tmr_min,trace.analog.ac.adc3.tmr_max);    
    
    
    memset((void*) &trace.analog.ac,0,sizeof(trace.analog.ac));
//  trace.analog.ac.phase_pos_cnt, trace.analog.ac.phase_pos_tmr_min,trace.analog.ac.phase_pos_tmr_max
    
    
    printf("\n");
}
#endif

#if defined(Uses_Debug_ADC_PWM)
void console_print_adc_sine_item(Uint8 num)
{
    
#if defined(DOCUMENTATION)
00:2.4 01:4.8 02:4.8 
123456789 123456789 123456789 123456789 123456789 123456789
101:240.1 102:240.1 103:240.1 104:240.1 105:240.1 106:240.1
101:240.1 102:240.1 103:240.1 104:240.1 105:240.1 106:240.1
#endif
    Uint16 adc;
    Uint16 acv;

    adc = debug_pwm_adc[num];
    acv = ADC_CALC_ACV10x(adc);
    
    printf("STATE, %.3d, ACV, %u.%u,  TMR, %u, %u",
        num,acv/10,acv%10,
        debug_pwm_time1[num],debug_pwm_time2[num]
    );
}

#endif



#endif

#if defined(Uses_Debug_Power)

void console_print_power()
{
	Uint16 amplitude;
//	Uint16 mamp;
//	Uint16 watts;
	
	if (HAL_Fault_Get())
		return;

	amplitude = PWM_Get_Amplitude();
//	mamp = PWM_Get_Miliamp();
//	watts = ((Uint32) mamp * analog.grid.avg_voltage) / 1000;
	
	printf("AMP:%u", amplitude );

	printf(" WAT:%u LIM:%u,%u", pwm.watts,pwm.amp.limit, tune.power_limit );

//	printf(" MA:%u", mamp );
	
	printf("\n");
	
//	printf("PCENT 50-100:%u  200-50:%u 105-100:%u \n", CALC_PERCENTAGE(100,50), CALC_PERCENTAGE(50,200), CALC_PERCENTAGE(100,105) );
	
	
}
#endif

#if defined(Uses_Debug_Scale)
void console_print_scale()
{
	printf("SCALE:%u ", PWM_Get_Grid_Voltage_Scale());	
//	printf("100:%u ",	PWM_Grid_Scale(100)  );	
//	printf("200:%u ", PWM_Grid_Scale(200) );
	
	printf("LIMIT:%u,%u ", PWM_Get_Amplitude(), pwm.amp.limit);	
		
	printf("\n");
	
//	printf("PCENT 50:%u 200:%u 105:%u 90:%u", CALC_PERCENTAGE(100,50), CALC_PERCENTAGE(100,200), CALC_PERCENTAGE(100,105),CALC_PERCENTAGE(100,90));
	printf("\n");
}
#endif



#if defined(Uses_Debug_Freq)
void console_print_debug_freq()
{
	printf("HZ:%u,%u ", GEO_freq.instant,GEO_freq.avg20);
	printf("TMR:%u,%u ", GEO_freq.tmr1, GEO_freq.tmr2 );
	printf("\n");
}
#endif

/*
void console_print_pwm_reg_info()
{
    //  65535
    printf("CLOCK PR3:%u FRQ:%lu OCX:%u DUTY:%u\n", PRX_CLOCK, PWM_CALC_PRX_MHZ(PRX_CLOCK),OCX_CLOCK,PWM_GET_OCX_DUTY(PRX_CLOCK,OCX_CLOCK));
    printf("SINE PR2:%u FRQ:%lu\n", PRX_SINE, PWM_CALC_PRX_MHZ(PRX_SINE));
}
*/



void console_print_geo_connect()
{
    printf("ST:%u,%s ALARM:%x BD:%d SEC:%d\n",
        APP_State_Get(),
        console_get_run_state(),
        fault.alarm.value,
        pwm.buck.delay,
        timer.buck_sec

    );
}


void console_print_debug()
{
	#if defined(Uses_Inverter)

//    console_print_zc_debug();
//    console_print_pwm_reg_info();
	
	#if defined(Uses_Debug_Alarms) 
	console_print_faults();
	#endif
				
	#if defined(Uses_Debug_Power)
	if (dcb2.power)	
	{
//		console_print_power();
	}
	#endif

	#if defined(Uses_Debug_Scale)
	if (dcb2.scale)
		console_print_scale();
	#endif
	

	#if defined(Uses_Debug_Phase)
	if (dcb1.phase_trig)
	{
		console_print_phase();
	}
	#endif
	
    
	#if defined(Uses_Debug_PWM)
	if (dcb1.pwm1)
		console_print_buck_pwm1();
	if (dcb1.pwm2)
		console_print_buck_pwm2();
	#endif
	            
	#if defined(Uses_Debug_ADC)
	if (dcb2.adc_all)
	{
		console_print_adc_all();
	}
    #endif
    
    #if defined(Uses_Debug_ADC_PWM)
	if (dcb2.adc_ac)
	{
		console_print_adc_ac();
	}
	#endif

	#if defined(Uses_Debug_Start)
	if (dcb2.start_logic)
		APP_Inverter_console_print_status();
	#endif


	#if defined(Uses_Debug_DcLimits)
	if (dcb2.dclimits)
	{
		console_print_dclimits();
	}
	#endif

	#if defined(Uses_Debug_Temperature)
	if (dcb2.temperature)
		console_print_temperature();	
	#endif

//	if (timer.stop_delay_ms100)
//	{
//		console_print_alarms(TEXT_STOP,fault.alarm);
//	}
		
	#endif

	#if defined(Uses_Debug_Freq)
	if (dcb2.freq)
	{
		console_print_debug_freq();
	}
	#endif

//  console_print_geo_connect();
//	console_print_float_test();

    #if defined(Uses_Debug_Phase)
    if (dcb1.phase_isr_error)
        console_print_phase_int1();
    #endif
    
    #if defined(Uses_Debug_DCV)
    debug_dclink_print();
    #endif

    #if defined(Uses_Debug_ACV)
    if (dcb3.acv)
        debug_acv_print1();
    #endif
	
}


void Debug_Task()
{
    #if defined(Uses_Debug_DCV)
    debug_dcv_task();
    #endif

    #if defined(Uses_Debug_UFB)
    debug_ufb_task();
    #endif

}
    
void Debug_Task_Ms100()
{
    #if defined(Uses_Debug_DCV)
    debug_dcv_task_ms100();
    #endif
}


void Debug_Task_Ms10()
{
    #if defined(Uses_Debug_ADC_PWM)
    if (debug_print_adc_sine)
    {
        console_print_adc_sine_item(debug_print_adc_sine-1);
        printf("\n");
        debug_print_adc_sine += 1;
        if (debug_print_adc_sine >= 201)
            debug_print_adc_sine = 0;
    }
    #endif
}


void Debug_Initialize()
{
	#if defined(Uses_Inverter)	
	dcb.pwm.prx =  140;
	dcb1.pwm1=0;
	dcb1.pwm2=0;
	dcb1.pwm3=0;
	dcb2.mppt = 0;
	dcb3.acv=0;
	dcb2.temperature=0;
	dcb2.adc_all=0;
    
    #if defined(Uses_Debug_ADC_PWM)
    dcb2.adc_ac=0;
    #endif
    
	#if defined(Uses_Debug_KWH)
	dcb2.kwh=0;
	#endif
	
	#if defined(Uses_Debug_Phase)
	dcb1.phase_trig = 0;
    dcb1.phase_range = 0;
    dcb1.phase_isr_error = 0;
    #endif
	
	dcb2.power=0;
	dcb2.scale=0;
	dcb.pwm.ufb_lock=0;
	
	#if defined( Uses_Debug_NVS_SPI )
	dcb2.spi_debug=1;
	dcb2.spi_status=0;
	#endif
	
	#if defined(Uses_Debug_Start)
	dcb2.start_logic=0;
	#endif
	
	#if defined(Uses_Debug_DcLimits)
//	dcb2.dclimits=1;
	#endif

//	#if defined(Uses_Debug_KWH)
//	dcb2.kwh=0;
//	#endif
	#endif

    #if defined(Uses_Debug_GEO)
    GEO_Nvs.bit.print_timer = 0;
    #endif
    
    #if defined(Uses_Debug_DCV)
    dcb3.dc1 = 1;
    dcb3.dc2 = 0;
    dcb3.dc3 = 0;
    dcb3.fault=1;
    #endif

    #if defined(Uses_Debug_ACV)
    dcb3.acv = 0;
    #endif

    
    #if defined(Uses_Debug_PWM)
    dcb1.pwm1 = 0;
    dcb1.pwm2 = 0;
    
    #endif
   
	
}

#endif
