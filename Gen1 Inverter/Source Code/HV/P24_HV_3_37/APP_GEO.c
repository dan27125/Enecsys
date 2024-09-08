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

	const GEO_DATA_BLOCK ConfigData_50Hz = 
	{
		50,
        100,
	    NO_AC_TIMEOUT_MS_50HZ,
		DC_LINK_SAMPLE_TIME_50HZ,
	    SINE_TIMER_PR4_50HZ,
		0
	};    
	
	const GEO_DATA_BLOCK ConfigData_60Hz = 
	{
		60,
        120,
		NO_AC_TIMEOUT_MS_60HZ,
		DC_LINK_SAMPLE_TIME_60HZ,		
	    SINE_TIMER_PR4_60HZ,
		0
	};    
	
    const GEO_LIMITS_DATA GEO_Limits_Default_50HZ = 
    {
		DEFAULT_50HZ_START_OVER_VOLTAGE_LIMIT,
		DEFAULT_50HZ_START_UNDER_VOLTAGE_LIMIT,
		DEFAULT_50HZ_RUN_OVER_VOLTAGE_LIMIT,
        DEFAULT_50HZ_RUN_UNDER_VOLTAGE_LIMIT,
		GET_CONNECTION_DELAY_SECCONDS(120)
    };    
	
    const GEO_LIMITS_DATA GEO_Limits_Default_60HZ = 
    {
        DEFAULT_60HZ_START_OVER_VOLTAGE_LIMIT,
        DEFAULT_60HZ_START_UNDER_VOLTAGE_LIMIT,
        DEFAULT_60HZ_RUN_OVER_VOLTAGE_LIMIT,
        DEFAULT_60HZ_RUN_UNDER_VOLTAGE_LIMIT,
		GET_CONNECTION_DELAY_SECCONDS(120)
    };

	#if defined(Uses_GEO_USA)
    const GEO_LIMITS_DATA GEO_Limits_USA = 
    {
        USA_STARTING_OVER_VOLTAGE_LIMIT,
        USA_STARTING_UNDER_VOLTAGE_LIMIT,
        USA_RUNNING_OVER_VOLTAGE_LIMIT,
        USA_RUNNING_UNDER_VOLTAGE_LIMIT,
		USA_CONNECTION_TIME,
    };
	#endif

	#if defined(Uses_GEO_USA2)
    const GEO_LIMITS_DATA GEO_Limits_USA2 = 
    {
        USA2_STARTING_OVER_VOLTAGE_LIMIT,
        USA2_STARTING_UNDER_VOLTAGE_LIMIT,
        USA2_RUNNING_OVER_VOLTAGE_LIMIT,
        USA2_RUNNING_UNDER_VOLTAGE_LIMIT,
		USA2_CONNECTION_TIME
    };
	#endif

	
	#if defined(Uses_GEO_UK)
    const GEO_LIMITS_DATA GEO_Limits_UK =
    {
        UK_STARTING_OVER_VOLTAGE_LIMIT,
        UK_STARTING_UNDER_VOLTAGE_LIMIT,
        UK_RUNNING_OVER_VOLTAGE_LIMIT,
        UK_RUNNING_UNDER_VOLTAGE_LIMIT,
        UK_CONNECTION_TIME
    };    
	#endif
    


	
#if defined(Uses_Debug_NVS)	
void GEO_Print(void)
{
	printf("GEO=%d ",tune.geo_code);
}
#endif


void GEO_freq_calc_timers(void)
{
    #if defined(DOCUMENTATION)
    GEO: 50HZ:490,15306,510,14705  60HZ:576,13020,612,12254 
    ADCS:3,11887   0:5973,85,24997   1:17,104,12776   3+:5910,198,12695
    50 HZ STATE, 000, ACV, 0.0,  TMR, 109, 153
    60 HZ STATE, 000, ACV, 0.0,  TMR, 109, 153
    STATE, 000, ACV, 0.0,  TMR, 109, 153
    STATE, 001, ACV, 2.4,  TMR, 203, 218
    STATE, 002, ACV, 1.8,  TMR, 236, 278
    STATE, 003, ACV, 1.2,  TMR, 236, 340
    STATE, 004, ACV, 12.3,  TMR, 327, 403
    
    #endif

//  F60.upper      60HZ:612,12254   50HZ:510,14705 
//  F60.lower      60HZ:576,13020   50HZ:490,15306
//                 60HZ:588,12755,612,12254 
//  phase2     12500 + 240 = 12740
    
    GEO_Nvs.freq.F60.lower.tmr1 = TIMER1_CALC_10X( GEO_Nvs.freq.F60.lower.herz );
    GEO_Nvs.freq.F60.lower.tmr2 = GEO_Nvs.freq.F60.lower.tmr1 * 2;    
    GEO_Nvs.freq.F60.upper.tmr1 = TIMER1_CALC_10X( GEO_Nvs.freq.F60.upper.herz );
    GEO_Nvs.freq.F60.trig.pos.lower = TIMER1_ADC_TRIG_POS_LOWER_60HZ;
    GEO_Nvs.freq.F60.trig.pos.upper = TIMER1_ADC_TRIG_POS_UPPER_60HZ;
    GEO_Nvs.freq.F60.trig.neg.lower = TIMER1_ADC_TRIG_NEG_LOWER_60HZ;
    GEO_Nvs.freq.F60.trig.neg.upper = TIMER1_ADC_TRIG_NEG_UPPER_60HZ;
    
    GEO_Nvs.freq.F50.lower.tmr1 = TIMER1_CALC_10X( GEO_Nvs.freq.F50.lower.herz );
    GEO_Nvs.freq.F50.lower.tmr2 = GEO_Nvs.freq.F50.lower.tmr1 * 2;    
    GEO_Nvs.freq.F50.upper.tmr1 = TIMER1_CALC_10X( GEO_Nvs.freq.F50.upper.herz );
    GEO_Nvs.freq.F50.trig.pos.lower = TIMER1_ADC_TRIG_POS_LOWER_50HZ;
    GEO_Nvs.freq.F50.trig.pos.upper = TIMER1_ADC_TRIG_POS_UPPER_50HZ;
    GEO_Nvs.freq.F50.trig.neg.lower = TIMER1_ADC_TRIG_NEG_LOWER_50HZ;
    GEO_Nvs.freq.F50.trig.neg.upper = TIMER1_ADC_TRIG_NEG_UPPER_50HZ;
    
    if (GEO_Config.grid_freq==50)
    {
        GEO_Nvs.freq.limit = GEO_Nvs.freq.F50;   // copy 50HZ values from above to runtime 'limit'
        
    }
    else
    {
        GEO_Nvs.freq.limit = GEO_Nvs.freq.F60;   // copy 60HZ values from above to runtime 'limit'
    }
    
}

void GEO_Set(void)
{
	GEO_CODE code;
    
	code = tune.geo_code;
	if (code==GEO_FACTORY)
	{
		if (GEO_Config.grid_freq==50)
		{
			code = GEO_UK;
		}
		else
		{
			code = GEO_USA2;
		}
	}

/*    
	if (GEO_Config.grid_freq==50)
    {
        GEO_Config.grid_freq2x = 100;
    }
    else
    {
        GEO_Config.grid_freq2x = 120;
    }
*/
    
    switch(code)
    {
       case GEO_FACTORY:
			GEO_Limits = GEO_Limits_Default_60HZ;
			GEO_Config = ConfigData_60Hz;
			break;
	
		case GEO_USA:
            GEO_Config = ConfigData_60Hz;
			#if defined(Uses_GEO_USA)
            GEO_Limits = GEO_Limits_USA;
			#else
			GEO_Limits = GEO_Limits_Default_60HZ;
			#endif
        break;
		
		case GEO_USA2:			
            GEO_Config = ConfigData_60Hz;
			#if defined(Uses_GEO_USA2)
            GEO_Limits = GEO_Limits_USA2;
			#else
			GEO_Limits = GEO_Limits_Default_60HZ;
			#endif
        break;

        case GEO_HAWAII:
            GEO_Config = ConfigData_60Hz;
			#if defined(Uses_GEO_Hawaii)
            GEO_Limits = GEO_Limits_Hawaii;
			#else
			GEO_Limits = GEO_Limits_Default_60HZ;
			#endif			
        break;

	    case GEO_UK:
            GEO_Config = ConfigData_50Hz;
			#if defined(Uses_GEO_UK)
			GEO_Limits = GEO_Limits_UK;
			#else
			GEO_Limits = GEO_Limits_Default_50HZ;
			#endif
        break;

        case GEO_GERMANY:
            GEO_Config = ConfigData_50Hz;
			#if defined(Uses_GEO_GERMANY)
            GEO_Limits = GEO_Limits_GERMANY;
			#else
			GEO_Limits = GEO_Limits_Default_50HZ;
			#endif						
        break;
		
        case GEO_FRANCE:
            GEO_Config = ConfigData_50Hz;
			#if defined(Uses_GEO_FRANCE)
            GEO_Limits = GEO_Limits_FRANCE;
			#else
			GEO_Limits = GEO_Limits_Default_50HZ;
			#endif			
        break;

        case GEO_ITALY:
            GEO_Config = ConfigData_50Hz;
			#if defined(Uses_GEO_ITALY)
            GEO_Limits = GEO_Limits_ITALY;
			#else
			GEO_Limits = GEO_Limits_Default_50HZ;
			#endif
			break;
        
		case GEO_GREECE:
            GEO_Config = ConfigData_50Hz;
			#if defined(Uses_GEO_GREECE) 
            GEO_Limits = GEO_Limits_GREECE;
			#else
			GEO_Limits = GEO_Limits_Default_50HZ;
			#endif
        break;
        
        case GEO_SPAIN:
            GEO_Config = ConfigData_50Hz;
			#if defined(Uses_GEO_SPAIN)
            GEO_Limits = GEO_Limits_SPAIN;
			#else
			GEO_Limits = GEO_Limits_Default_50HZ;
			#endif			
        break;
      
        case GEO_INDIA:
            GEO_Config = ConfigData_50Hz;
			#if defined(Uses_GEO_INDIA)
            GEO_Limits = GEO_Limits_INDIA;
			#else
			GEO_Limits = GEO_Limits_Default_50HZ;
			#endif
        break;
        
        default:
            GEO_Config = ConfigData_60Hz;
            GEO_Limits = GEO_Limits_Default_60HZ;
        break;
    }

    GEO_freq.zc.start.lock = GEO_Config.grid_freq;
    GEO_freq_calc_timers();
}


void GEO_freq_set_default(void)
{
    GEO_Nvs.freq.F50.lower.herz = TUNE_GEO_FREQ_50HZ_LOWER_DEF;
	NVS_Write_Word(NVS_GEO_FREQ_50HZ_LOWER ,GEO_Nvs.freq.F50.lower.herz,NVS_VALID_DEFAULT);
    
    GEO_Nvs.freq.F50.upper.herz = TUNE_GEO_FREQ_50HZ_UPPER_DEF;
	NVS_Write_Word(NVS_GEO_FREQ_50HZ_UPPER ,GEO_Nvs.freq.F50.upper.herz,NVS_VALID_DEFAULT);

    GEO_Nvs.freq.F60.lower.herz = TUNE_GEO_FREQ_60HZ_LOWER_DEF;
	NVS_Write_Word(NVS_GEO_FREQ_60HZ_LOWER ,GEO_Nvs.freq.F60.lower.herz,NVS_VALID_DEFAULT);

    GEO_Nvs.freq.F60.upper.herz = TUNE_GEO_FREQ_60HZ_UPPER_DEF;
	NVS_Write_Word(NVS_GEO_FREQ_60HZ_UPPER ,GEO_Nvs.freq.F60.upper.herz,NVS_VALID_DEFAULT);
}



#if defined(Uses_GEO_Console)




void GEO_console_print_freq(void)
{
    printf("%s: %s:%u,%u,%u,%u  %s:%u,%u,%u,%u \n", 
            TEXT_GEO, 
            TEXT_50HZ, GEO_Nvs.freq.F50.lower.herz,GEO_Nvs.freq.F50.lower.tmr1,GEO_Nvs.freq.F50.upper.herz,GEO_Nvs.freq.F50.upper.tmr1,
            TEXT_60HZ, GEO_Nvs.freq.F60.lower.herz,GEO_Nvs.freq.F60.lower.tmr1,GEO_Nvs.freq.F60.upper.herz,GEO_Nvs.freq.F60.upper.tmr1
    );
}

void GEO_console_freq_set(Uint16 vid, Uint16 value)
{
    NVS_VID_Write_Item(vid,value);
    printf("%s %s set %u\n",arg3,arg4,value);
    
    {
        Uint16 tmr;
        tmr = TIMER1_CALC_10X(value);
        printf("Calc %u Timer %u \n",value, tmr );
    }
    
    GEO_console_print_freq();
}

Uint8 GEO_console_parse_args(void)
{
//  printf("ARGS %s %s %s %s %u\n",arg1,arg2,arg3,arg4, csib.value[4] );
    
    if ( strcmp(TEXT_GEO, (char*)arg1)==0)
    {
        if (strcmp(TEXT_FREQ, (char*)arg2)==0)
        {
            if ((strcmp(TEXT_50HZ, (char*)arg3)==0) && (strcmp(TEXT_MIN, (char*)arg4)==0))
            {
                GEO_console_freq_set(VID_GEO_FREQ_50HZ_MIN, csib.value[4]);
            }
            else if ((strcmp(TEXT_50HZ, (char*)arg3)==0) && (strcmp(TEXT_MAX, (char*)arg4)==0))
            {
                GEO_console_freq_set(VID_GEO_FREQ_50HZ_MAX, csib.value[4]);
            }
            else if ((strcmp(TEXT_60HZ, (char*)arg3)==0) && (strcmp(TEXT_MIN, (char*)arg4)==0))
            {
                GEO_console_freq_set(VID_GEO_FREQ_60HZ_MIN, csib.value[4]);
            }
            else if ((strcmp(TEXT_60HZ, (char*)arg3)==0) && (strcmp(TEXT_MAX, (char*)arg4)==0))
            {
                GEO_console_freq_set(VID_GEO_FREQ_60HZ_MAX, csib.value[4]);
            }
            else if (strcmp(TEXT_DEFAULT, (char*)arg3)==0)
            {
                GEO_freq_set_default();
                GEO_console_print_freq();
            }
            else
                GEO_console_print_freq();
            
        }
        else if ( strcmp(TEXT_SHOW, (char*)arg2)==0)
        {
            GEO_Nvs.bit.print_timer = !GEO_Nvs.bit.print_timer;
        }

		else if ( strcmp(TEXT_SET, (char*)arg2)==0)
		{
			Uint16 value = csib.value[2];
			printf("%s %s %u\n",arg1,arg2,value);
			NVS_GEO_Write(value);
            printf("GEO=%d\n", NVS_GEO_Read() );
		}
      
        #if !defined( Uses_Debug_GEO_FREQ_Timer)
        else if ( strcmp("CALC", (char*)arg2)==0)
        {
            Uint16 freq10x;  
            Uint16 tmr;
            freq10x = csib.value[2];
            tmr = TIMER1_CALC_10X(freq10x);
            printf("Calc TIMER:%u from %u  (%d.%d HZ) \n",tmr, freq10x, freq10x/10, freq10x%10);
            printf("Calc FREQ:%u from TIMER:%u  \n", TIMER1_CALC_10X(tmr), tmr);
        }
        #endif

    }
    
	else
		return 0;
	
	return 1;
}


void GEO_ZC_ISR(void)
{
    Uint16 tmr1_reg;
    
	#if defined(DOCUMENTATION)
    DGA 9/5/2022  Frequency count Notes.  Here at Zero crossing.  60 HZ = 120
	TMR1 is running at 1.5MHZ (1,5000,000). TMR1 is saved in timer.tmr1_reg at begining of ZC ISR. 
	#endif
            
    phase.bit.trig_alternate = 0;
			
    if (phase.dir.flag==0)	// Negative cycle. 
    {
        GEO_freq.zc.tmr1 = timer.tmr1_reg;  // TMR1 is saved in timer.tmr1_reg at begining of ZC ISR. 
        GEO_freq.zc.tmr2 = 0;   // Make sure tmr2 is reset.
        tmr1_reg = GEO_freq.zc.tmr1;
    }
    else
    {
        tmr1_reg = (timer.tmr1_reg - GEO_freq.zc.tmr1);   // Subtract 1st reading 
        GEO_freq.zc.tmr2 = tmr1_reg; 
    }

    GEO_freq.bit.zc_sample=1;	// DGA Set bit for main task to trigger further processing;

    if (GEO_freq.bit.lock)
    {
        #if defined(Uses_Console_Fault_Test)
        if (fault.test.freq_value)
        {
            tmr1_reg = fault.test.freq_value;
            fault.test.freq_value = 0;
        }
        #endif
        
        if (fault.alarm.grid_low==0)
        {
            
             #if defined(DOC)
            GEO: 50HZ:490,15306,510,14705  60HZ:576,13020,612,12254 
            lower.tmr1  50HZ:490,15306	60HZ:576,13020
            upper.tmr1  50HZ,510,14705	60HZ:612,12254 
            0-POS PHASE RANGE 00400-15000     
            1-NEG PHASE RANGE 15400-30000
             #endif
             
            // lower.tmr1  50HZ:490,15306	60HZ:576,13020
            if (tmr1_reg > GEO_Nvs.freq.limit.lower.tmr1)   // Is FREQ low ?
            {
                #if defined(Uses_Fault_Freq_LO)
                if ((fault.alarm.freq_high==0) && (fault.alarm.freq_low==0) && (pwm.bit.wattage_drop_50==0) )    // 04/24/23 DGA - Change to mutually exclusive            
                {
                    HAL_Fault_Set(ALARM_DISC|ALARM_UNDER_FREQUENCY,FC_GEO_ZC_FREQ_LO,tmr1_reg,GEO_Nvs.freq.limit.lower.tmr1);
                    GEO_freq.zc.cycle_cnt_low=0;	// DGA reset 
                }
                #endif

            }
            
            //  upper.tmr1  50HZ,510,14705	60HZ:612,12254 
            else if  (tmr1_reg < GEO_Nvs.freq.limit.upper.tmr1)
            {
                #if defined(DOC)

                GEO: 50HZ:490,15306,510,14705  60HZ:576,13020,612,12254 
                lower.tmr1  50HZ:490,15306	60HZ:576,13020
                upper.tmr1  50HZ,510,14705	60HZ:612,12254 
                0-POS PHASE RANGE 00400-15000     
                1-NEG PHASE RANGE 15400-30000
                
                FC_GEO_ZC_FREQ_HI =11
                9/28 60 HZ F11:1,11411,12254 
                9/30 60 HZ F11:1,11922=62.9,12254=61.2
                #endif

                #if defined(Uses_Fault_Freq_HI)
                if ((fault.alarm.freq_high==0) && (fault.alarm.freq_low==0)&& (pwm.bit.wattage_drop_50==0))    // 04/24/23 DGA - Change to mutually exclusive
                {
                    HAL_Fault_Set(ALARM_DISC|ALARM_OVER_FREQUENCY,FC_GEO_ZC_FREQ_HI, tmr1_reg,GEO_Nvs.freq.limit.upper.tmr1);
                    GEO_freq.zc.cycle_cnt_high=0;		// reset 
                }
                #endif

            }
        }

    }
    
    if ((phase.dir.flag) || (TMR1 > TIMER1_MAX))
        TMR1  = 0;          // Reset the frequency monitoring timer.
        
    phase.bit.trig_positive1 = 0;    
    phase.bit.trig_negative1 = 0;    
    
	if (phase.new.positive_cnt >= GEO_PHASE_SAMPLE_MIN)
	{
		phase.new.flag=1;
        phase.bit.trig_negative0 = 0;    // Reset the neg phase trigger;
		
		if (phase.new.negative_cnt <= GEO_PHASE_ERROR_MAX)
		{
			if (phase.lock_cnt < 0xffff)
				phase.lock_cnt++;
		}
		else
		{
			if (phase.lock_cnt)
			{
				phase.error_cnt++;
				phase.error_pos++;
				phase.lock_cnt = 0;
			}
		}

	}
	else if (phase.new.negative_cnt >= GEO_PHASE_SAMPLE_MIN)
	{
		phase.new.flag=0;
        phase.bit.trig_positive0 = 0;    // Reset the pos phase trigger;
		if (phase.new.positive_cnt <= GEO_PHASE_ERROR_MAX)
		{
			if (phase.lock_cnt < 0xffff)
				phase.lock_cnt++;
		}
		else
		{
			if (phase.lock_cnt)
			{
				phase.error_cnt++;
				phase.error_neg++;
				phase.lock_cnt = 0;
			}
		}
	}
    
    
	phase.old = phase.dir;
	phase.dir = phase.new;
	memset( (void*) &phase.new,0,sizeof(phase.new));
}

//	Called from APP_Inverter_Start_Reset 
void GEO_ZC_Reset()
{
	memset( (void*) &phase,0,sizeof(phase));
	memset( (void*) &GEO_freq,0,sizeof(GEO_freq));
}




#endif