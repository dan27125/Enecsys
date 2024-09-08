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

#define MPPT_START_SAMPLE_FILTER        50
#define MPPT_START_SAMPLE_CYCLES        50

    void MPPT_Print_Up();
    void MPPT_Print_Dn();
    void MPPT_Print_Auto();
    
    #if defined(Uses_MPPT_Print_Track)
    void MPPT_Print_Tracking();
    #endif

    #if defined(Uses_MPPT_Print_Ramp)
    void MPPT_Print_Ramp();
    #endif

    #if defined(Uses_MPPT_Print_Flutter)    
    void MPPT_Print_Flutter();
    #endif
    
    void MPPT_Print_Ramp_Pcent();
    void MPPT_Print_Ramp_Time();

void MPPT_Init()
{
	memset((void*) &mppt,0,sizeof(mppt));
	mppt.watts = MPPT_MIN_WATTS;
//  rcb.bit.console_mppt=1;

}

Uint16 MPPT_Get_Percent(Uint16 value, unsigned char pcent)
{
    Uint16 rc;
    if ((value < 100) || pcent < 10)
        rc = 0;
    else
    {
        rc = (Uint16) (((Uint32) (value * pcent)) / 100);
    }
    return rc;
}

void MPPT_Calc_Ramp_Up_Percent(Uint16 src_hvdc, Uint8 percent)
{
    Uint16 hvdc, pvdc;
    
//  Must perform the following steps:
//  1) Calc HVDC percentage
//  2) Calc PV-10X from HVDC percentage
//  3) Bounds check and adjust PVDC
//  4) Calc/set HVDC from bounds adjusted PV10X
    
//  1) Calc HVDC percentage
    hvdc = MPPT_Get_Percent(src_hvdc, percent);   // Calc HVDC percentage
    
//  2) Calc PV-10X from HVDC percentage
    pvdc = ADC_DCL_Calc_PV10X(hvdc);                // Calc PV-10X from HVDC percentage

// 3) Bounds check and adjust PVDC
    tune.mppt.calc.up.pvdc = Bounds_Check_Uint16(pvdc, TUNE_MPPT_RAMP_UP_MIN_PV10X,TUNE_MPPT_RAMP_UP_MAX_PV10X);  

// 4) Calc HVDC from bounds adjusted PV10X
    tune.mppt.calc.up.hvdc = ADC_PV10X_Calc_DCL(tune.mppt.calc.up.pvdc);   // Calc HVDC from PV10X
    
}

void MPPT_Calc_Ramp_Dn_Percent(Uint16 src_hvdc, Uint8 percent)
{
    Uint16 hvdc, pvdc;
    
//  Must perform the following steps:
//  1) Calc HVDC percentage
//  2) Calc PV-10X from HVDC percentage
//  3) Bounds check and adjust PVDC
//  4) Calc HVDC from bounds adjusted PV10X
    
//  1) Calc HVDC percentage
    hvdc = MPPT_Get_Percent(src_hvdc, percent);   // Calc HVDC percentage
    
//  2) Calc PV-10X from HVDC percentage
    pvdc = ADC_DCL_Calc_PV10X(hvdc);                // Calc PV-10X from HVDC percentage

// 3) Bounds check and adjust PVDC
    tune.mppt.calc.dn.pvdc = Bounds_Check_Uint16(pvdc, TUNE_MPPT_RAMP_DN_MIN_PV10X,TUNE_MPPT_RAMP_DN_MAX_PV10X);  

// 4) Calc HVDC from bounds adjusted PV10X
    tune.mppt.calc.dn.hvdc = ADC_PV10X_Calc_DCL(tune.mppt.calc.dn.pvdc);    // Calc HVDC from PV 10X    
}

Uint16 MPPT_Get_Limit()
{
    switch(mppt.flutter.state)
    {
        case FLUTTER_LOW:
        {
            return (tune.mppt.ramp_trigger-20);
            break;
        }
        case FLUTTER_MEDIUM:
        {
            return (tune.mppt.ramp_trigger-40);
            break;
        }
        case FLUTTER_HIGH:
        {
            return (tune.mppt.ramp_trigger-60);
            break;
        }
        
        case FLUTTER_NONE:
        default:
        {
            if (mppt.bit.debug)
                return (tune.mppt.ramp_trigger-6);
            else
                return (tune.mppt.ramp_trigger);
            break;
        }
    }
}

void MPPT_Calc_Power_Limit()
{
    tune.run_power_limit = tune.nvs_power_limit;
    if (fault.alarm.value==0)
    {
        if (analog.ac.avg.volts < 180)
        {
            tune.run_power_limit = tune.nvs_power_limit / 2;
        }
    }
}

void MPPT_Get_Ramp_Percent()
{
    Uint16 dcv;
    //  2024/30/19 DGA change if connected to grid then to use analog.pwm.avg.volts 
    dcv = analog.dc.voltage;
    
    #if defined(Uses_MPPT_Ramp_PWM_Avg)
    if ((APP_State_Get() == INVERTER_RUN) && (fault.alarm.value==0))
    {
        dcv = analog.pwm.avg.volts;
    }
    #endif
    
    mppt.up.pcent = CALC_RAMP_UP_PERCENT(dcv,tune.mppt.run.up.hvdc);
    mppt.dn.pcent = CALC_RAMP_DN_PERCENT(dcv,tune.mppt.run.dn.hvdc);
    
    mppt.limit_pcent = MPPT_Get_Limit();
}

Uint16 MPPT_Calc_Start_Power()
{
    Uint16 value;

/*    
    #define	PV_DIVISOR_RUN_V240			1591
	#define	PV_DIVISOR_RUN_V360			1177	
	#define	PV_DIVISOR_RUN_V480			1591
*/
    
    #define START_POWER_WATT_MIN           5
    #define START_POWER_WATT_MAX           20

    #define START_POWER_HISTORY_WATT_MIN   50

    #if defined(SMI360_72)  
    // PV_DIVISOR_RUN_V360                 1177	
    #define START_POWER_HVDC_MIN           400     // 34 x 11.77 = 400
    #define START_POWER_HVDC_DIV           10

    #endif
    
    #if defined(SMI240_60)
//  #define	PV_DIVISOR_RUN_V240            1591
    #define START_POWER_HVDC_MIN           445     // 28 * 15.91 = 445
    #define START_POWER_HVDC_DIV           7
    #endif

    #if defined(SMI480_60)
    #define	PV_DIVISOR_RUN_V480            1591
    #define START_POWER_HVDC_MIN           445     // 28 * 15.91 = 445
    #define START_POWER_HVDC_DIV           7
    #endif
    
    // Check following conditions 
    if ( (analog.dc.voltage < START_POWER_HVDC_MIN) || 
         (mppt.history.watts < START_POWER_HISTORY_WATT_MIN) ||
         (tune.mppt.auto_mode==0)
       )
        return START_POWER_WATT_MIN;    // Abort power increase based on voltage
    
    
     value = ((analog.dc.voltage-START_POWER_HVDC_MIN) / START_POWER_HVDC_DIV) + 5;
     if (value > START_POWER_WATT_MAX)
         value = START_POWER_WATT_MAX;
     
    return value;
}

void MPPT_Calc_ADC_Samples()
{
    // 6/11/23 calculate sample for the ADC ISR 
    mppt.dn_sample = DCLINK_GET_ADC(tune.mppt.run.dn.hvdc);    // Calc ramp dn
    mppt.up_sample = DCLINK_GET_ADC(tune.mppt.run.up.hvdc);    // Calc ramp up
}

void MPPT_Start(Uint8 opts)
{
	if (opts & MPPT_READ_NVS)
		NVS_VID_Read_All();		// DGA get tuning parameters 
	
	mppt.watts = 5;	
    
	timer.mppt_up_delay=0;
	timer.mppt_dn_delay=0;
    timer.mppt_ramp_up = 0;
    timer.mppt_ramp_dn = 0;
    timer.mppt_adc_up = 0;
    timer.mppt_adc_dn = 0;
    
    tune.mppt.run.up = tune.mppt.lock.up;
    tune.mppt.run.dn = tune.mppt.lock.dn;
    
    memset( (void*) &mppt.start,0,sizeof(mppt.start));
    memset( (void*) &mppt.flutter,0,sizeof(mppt.flutter));  
    
    mppt.bit.start = 1;         // set bit 
    mppt.bit.run = 0;            // clear bit 
    
    mppt.start.time_limit = MPPT_RUN_TIMER_LIMIT;
    
    #if defined(Uses_MPPT_Run_Test_Sec)
    #if defined(Uses_AMP_Lock)
    if (dcb.pwm.amp_lock==0)
    #endif
        mppt.start.time_limit = Uses_MPPT_Run_Test_Sec;
    #endif
    
    mppt.watts = MPPT_Calc_Start_Power();
    
    MPPT_Calc_Power_Limit();
    
	
	if (opts & MPPT_PRINT_PARAMS)
		MPPT_console_print_config();
}

void MPPT_Ramp_Up_Set_Delay()
{
    timer.mppt_up_delay = tune.mppt.ramp_up_delay;
    
//    //  Was flutter was detected
//    if (mppt.flutter.count)
//    {
//        // Yes then use predetermined delay
//        timer.mppt_up_delay =  mppt.flutter.up_delay;   
//        return;
//    }
    
    if (mppt.flutter.count)
        timer.mppt_up_delay = 100;

}

void MPPT_Ramp_Up()
{
	if (timer.mppt_up_delay==0)
	{
//		if (mppt.watts < tune.power_limit)
        if (pwm.watts < tune.run_power_limit)
		{
			mppt.watts++;
            timer.mppt_ramp_up = 0;  // Reset stop watch
			mppt.up.count++;
			mppt.up.telem++;
            mppt.bit.track_refresh=1;
        
            #if defined(Uses_MPPT_Print_Track)
            if ((tune.mppt.rcb.print_track) && (mppt.up.count<=2))
            {
                printf("%s+ ",TEXT_RAMP);
                MPPT_Print_Up();
                MPPT_Print_Ramp_Pcent();
                MPPT_Print_Ramp_Time();
                printf("\n");
                mppt.print_cnt++;
            }
            #endif
            
            #if defined(Uses_MPPT_Print_Flutter)
            if ((mppt.dn.count) || (mppt.flutter.count))
            {
                MPPT_Print_Flutter();
            }
            #endif

            MPPT_Ramp_Up_Set_Delay();
		}
	}
}

void MPPT_Ramp_Dn()
{
	if ((mppt.watts > MPPT_MIN_WATTS) && (timer.mppt_dn_delay==0))
	{
		mppt.watts -= 1;
        
		if (mppt.watts < MPPT_MIN_WATTS)
			mppt.watts = MPPT_MIN_WATTS;
        
        mppt.bit.track_refresh=1;
        timer.mppt_ramp_dn = 0;  // Reset stop watch
		mppt.dn.count++;
		mppt.dn.telem++;
        
        #if defined(Uses_MPPT_Print_Track)
        if ((tune.mppt.rcb.print_track) && (mppt.dn.count<=2))
        {
            printf("%s- ",TEXT_RAMP);
            MPPT_Print_Dn();
            MPPT_Print_Ramp_Pcent();
            MPPT_Print_Ramp_Time();
            printf("DS:%u\n",mppt.dn_sample_cnt);
            mppt.print_cnt++;
            
        }
        #endif
        
        #if defined(Uses_MPPT_Print_Flutter)
        if ((mppt.up.count) || (mppt.flutter.count))
        {
            MPPT_Print_Flutter();
        }
        #endif

		timer.mppt_dn_delay = tune.mppt.ramp_dn_delay;
		timer.mppt_up_delay = 1000;	
        
	}
}

void MPPT_Flutter_Ms1000()
{
    
    // If Ramp down less than 10% ... disable flutter detection 
    if (tune.mppt.ramp_dn_percent < 10)
    {
        mppt.flutter.count = 0;
        mppt.flutter.state = FLUTTER_NONE;
        return;
    }
    
    // Look for new flutter
    if ((mppt.dn.count) && (mppt.up.count))   // Was both dn and up adjusted 
    {
        mppt.flutter.count++;   // bump counter 
        mppt.flutter.time_off=0; 
                
        if (mppt.flutter.count == 2)
        {
            mppt.flutter.state = FLUTTER_LOW;
            
            #if defined(Uses_MPPT_Print_Flutter)
            if (tune.mppt.rcb.print_track)
                printf("%s=%s\n",TEXT_FLUT,TEXT_LOW);
            #endif
        }
        else if (mppt.flutter.count == 4)
        {
            mppt.flutter.state = FLUTTER_MEDIUM;
            #if defined(Uses_MPPT_Print_Flutter)
            if (tune.mppt.rcb.print_track)
                printf("%s=%s\n",TEXT_FLUT,TEXT_MED);
            #endif
        }
        else if (mppt.flutter.count >= 6)
        {
            mppt.flutter.state = FLUTTER_HIGH;
            #if defined(Uses_MPPT_Print_Flutter)
            if (tune.mppt.rcb.print_track)
                printf("%s=%s\n",TEXT_FLUT,TEXT_HIGH);
            #endif

        }
    }
    else
    {
        if (mppt.flutter.count)
        {
            ++mppt.flutter.time_off;
            
            if (++mppt.flutter.time_off >= 6)
            {
                mppt.flutter.time_off = 0;
                mppt.flutter.count--;
                if (mppt.flutter.count==0)
                {
                    mppt.flutter.count = 0;
                    mppt.flutter.state = FLUTTER_NONE;
                    #if defined(Uses_MPPT_Debug)
                    if (tune.mppt.rcb.print_track)
                        printf("%s=%s\n",TEXT_FLUT,TEXT_RESET);
                    #endif
                }
            }
        }
    }

}


void MPPT_Run_Task()
{
    // Here at the peak of the AC Sine wave, 2x the AC frequency
    // On 50HZ AC we are here 100 HZ / times per second
    // On 60HZ AC we are here 120 HZ / times per second
/*
  ********** CRITICAL NOTES **********
 * 
   *  This function assumes the following logic was applied by calling function 
  if( (HAL_Fault_Get()==FALSE) && (APP_State_Get() == INVERTER_RUN) && (pwm.buck.enable) )
 * &&  if (mppt.bit.dclink_sample)  i.e. there is a new DC Link sample 
   The frequency of calls is 2 X GRID, i.e. 50HZ=100 60HZ=120 
*/
	

//  ********** GET RAMP PERCENTAGE *********    
    // Get both ramp percentages (up and down) and store them in local variables
    MPPT_Get_Ramp_Percent();  // Refresh mppt struct elements mppt.up.pcent and mppt.dn.pcent
//  ********** GET RAMP PERCENTAGE *********        
    
	mppt.hvdc = analog.dc.avg.volts;
//	ramp = tune.mppt.ramp_run.hvdc;
	
    // analog.dc.voltage represents the instantaneous HVDC voltage 
    // mppt.hvdc = analog.dc.voltage_avg1 represents a slow average of HVDC voltage 
    //  Look at both the average and instantaneous HVDC, wait for both to be TRUE before ramping  
    // If HVDC voltage greater then RAMP then ramp up


//    #define ACV_ADC_TEMPERATURE_MPPT_UP     55  // 131F 55C
//    #define ACV_ADC_TEMPERATURE_MPPT_DN     61  // 142F 61C
    
    #if defined(Uses_MPPT_AC_Ramp)
	if((mppt.up.pcent <  mppt.limit_pcent) && (analog.ac.avg.volts < ACV_ADC_VOLT_MPPT_UP) && (temperature.celsius < tune.temperature.mppt_up))
	{
		MPPT_Ramp_Up(); // If HVDC voltage > RAMP then call Ramp function which steps UP one watt of power
	}
    else if ((mppt.dn.pcent <  mppt.limit_pcent) || (analog.ac.avg.volts >= ACV_ADC_VOLT_MPPT_DN) || (temperature.celsius >= tune.temperature.mppt_dn))
    {
        MPPT_Ramp_Dn(); 
    }
    #else
	if(mppt.up.pcent <  (mppt.limit_pcent))
	{
		MPPT_Ramp_Up(); // If HVDC voltage > RAMP then call Ramp function which steps UP one watt of power
	}
    else if (mppt.dn.pcent <  mppt.limit_pcent)
    {
        MPPT_Ramp_Dn(); 
    }
    #endif
    
//else if ((timer.mppt_adc_dn > 10) &&  (mppt.dn.pcent <  mppt.limit_pcent))
 // if ((mppt.dn_sample_cnt > tune.mppt.ramp_dn_sample_cnt) &&  (mppt.dn.pcent <  mppt.limit_pcent))    
    
}


void MPPT_Start_Task()
{
/*
 * MPPT_Calc_Start()
  ********** CRITICAL NOTES **********
  1 This function assumes the following logic was applied by calling function 
  if( (HAL_Fault_Get()==FALSE) && (APP_State_Get() == INVERTER_RUN) && (pwm.buck.enable) )
 * 
 * 2 The frequency of calls is 2 X GRID, i.e. 50HZ=100 60HZ=120
*/
    
    mppt.start.sample_cnt++;                            // INCREMENT sample counter
    
    if (mppt.start.sample_cnt > MPPT_START_SAMPLE_FILTER)
    {
        mppt.start.hvdc_sum += analog.dc.voltage;     // Add to sum
        mppt.start.sample_sum += analog.dc.sample;
    }
   
    if (mppt.start.sample_cnt >= (MPPT_START_SAMPLE_FILTER+MPPT_START_SAMPLE_CYCLES)) 
    {
        mppt.start.hvdc = (Uint16)  ( ((Uint32)  mppt.start.hvdc_sum) / MPPT_START_SAMPLE_CYCLES);
        mppt.start.sample = (Uint16)  ( ((Uint32)  mppt.start.sample_sum) / MPPT_START_SAMPLE_CYCLES);
        mppt.start.pvdc = ADC_DCL_Calc_PV10X(mppt.start.hvdc);
        
//      printf("mppt.start.sample=%u,%u\n", DCLINK_GET_VDC(mppt.start.sample),mppt.start.sample);
        
        
        MPPT_Calc_Ramp_Up_Percent(mppt.start.hvdc, tune.mppt.ramp_up_percent);          // Auto Calc Ramp using desired percent of start HVDC
        MPPT_Calc_Ramp_Dn_Percent(tune.mppt.calc.up.hvdc,tune.mppt.ramp_dn_percent);    // Auto Calc down using 85% of ramp HVDC
        
        if (tune.mppt.auto_mode)
        {
            tune.mppt.run.up = tune.mppt.calc.up;
            tune.mppt.run.dn = tune.mppt.calc.dn;
        }
        else
        {
            tune.mppt.run.up = tune.mppt.lock.up;
            tune.mppt.run.dn = tune.mppt.lock.dn;
        }

        MPPT_Calc_ADC_Samples();
        
        mppt.bit.start = 0;     // clear bit
        mppt.bit.run = 1;       // set bit 
        
        #if defined(Uses_MPPT_Debug)
        MPPT_Print_Auto();
        #endif
        
    }
}

#if defined(Uses_MPPT_PV_Slope_Reset)
Uint8 MPPT_pv_slope_bounds_check(Uint8 pcent)
{
    
//  #define Uses_Slope_Bounds_Debug     
    
    Uint16 hvdc_pcent;
    Uint16 hvdc_min;
    Uint16 hvdc_max;
            
    hvdc_pcent = MPPT_Get_Percent(mppt.start.hvdc,pcent);
    hvdc_min = mppt.start.hvdc - hvdc_pcent;
    hvdc_max = mppt.start.hvdc + hvdc_pcent;
    
    #if defined(Uses_Slope_Bounds_Debug)
    printf("ST:%u,%u ",mppt.start.pvdc,mppt.start.hvdc);
    printf("RUN:%u,%u ",ADC_DCL_Calc_PV10X(analog.dc.voltage),analog.dc.voltage);
    printf("%u 25:%u\n",500,MPPT_Get_Percent(500,25));
    #endif
    
        
    if (analog.dc.avg1s.voltage > hvdc_max)
    {
        #if defined(Uses_Slope_Bounds_Debug)
        printf("+++ HI +++\n");
        #endif
        return TRUE;
    }
    else if (analog.dc.avg1s.voltage < hvdc_min)
    {
        #if defined(Uses_Slope_Bounds_Debug)
        printf("--- LO ---\n");
        #endif

        return TRUE;
    }
    
    return FALSE;
}

void MPPT_PV_Slope_Reset_Task()
{
    if (mppt.start.timer > 10)
    {
        if (MPPT_pv_slope_bounds_check(15))
        {
            #if defined(Uses_Debug)
            printf("MPPT SLOPE RESET\n");
            #endif
            MPPT_Start(0);
        }
    }
}
#endif

void MPPT_Run_Ms1000()
{
/*
 This function is called once a second when the following logic is TRUE
 * 
 *  PSUDO CODE: if ONE_SECCOND_HAS ELAPSED and 
 *  if( (HAL_Fault_Get()==FALSE) && (APP_State_Get() == INVERTER_RUN) && (pwm.buck.enable) )
 *  then call function and software state machine causes entry here 
 *  once a second when inverter is running 
 */    

    MPPT_Calc_ADC_Samples();
    MPPT_Get_Ramp_Percent();  // Refresh mppt struct elements mppt.up.pcent and mppt.dn.pcent
    MPPT_Calc_Power_Limit();
    
    mppt.start.timer++;     // Increment run seconds timer 
    
/*    
    #if defined(Uses_MPPT_Run_Test)
    {
        Uint16 secs = mppt.start.time_limit-mppt.start.timer;
        printf("Test mode %d of %u\n",secs,mppt.start.time_limit);
    }
    #endif
*/
    
    if (mppt.start.timer >= mppt.start.time_limit)
    {
        #if defined(Uses_MPPT_Run_Test_Sec)
        printf("Test mode %ds reset\n",mppt.start.time_limit);
        #endif
        mppt.history.watts = mppt.watts;
        mppt.history.count++;
        MPPT_Start(0);
    }
    else
    {
        #if defined( Uses_MPPT_Debug )
        if (mppt.start.timer < 2)
        {
            if (tune.mppt.auto_mode)
                MPPT_Print_Auto();
        }
        #endif
    }

    MPPT_Flutter_Ms1000();

    #if defined(Uses_MPPT_Print_Track)
    if (tune.mppt.rcb.print_track) 
    {
        if ((mppt.dn.count) || (mppt.up.count))
        {
            MPPT_Print_Tracking();
        }
        else if ((timer.mppt_ramp_dn > 2000) && (timer.mppt_ramp_up > 2000) && (mppt.bit.track_refresh))
        {
            mppt.bit.track_refresh=0;
            MPPT_Print_Tracking();
        }
    }
    #endif
    
    #if defined(Uses_MPPT_Print_Ramp)
    if (tune.mppt.rcb.print_ramp) 
    {
        MPPT_Print_Ramp();
    }
    #endif
   
    
    mppt.up.count=0;
    mppt.dn.count=0;
    mppt.print_cnt=0;

/*    if (mppt.bit.debug)
    {
        MPPT_Print_Dn();
    }
*/

    #if defined(Uses_MPPT_PV_Slope_Reset)
    MPPT_PV_Slope_Reset_Task();
    #endif
    

}

void MPPT_Task()
{

    #if defined(Uses_AMP_Lock)    
    if (dcb.pwm.amp_lock)
        return;
    #endif
    
//	if( (HAL_Fault_Get()==FALSE) && (APP_State_Get() == INVERTER_RUN) && (pwm.buck.enable) )
    if( (APP_State_Get() == INVERTER_RUN) && (pwm.buck.enable) )
	{
        
/*        
		if (mppt.bit.dclink_min)
		{
//			MPPT_Ramp_Down();
			mppt.bit.dclink_min=0;
		}
*/		
		if (mppt.bit.dclink_sample)
		{
			mppt.bit.dclink_sample=0;
            
            if (mppt.bit.start)
                MPPT_Start_Task();
            else
                MPPT_Run_Task();
		}

  		if (mppt.bit.sec)
       	{
           	mppt.bit.sec=0;
            if (mppt.bit.run)
            {
                MPPT_Run_Ms1000();
            }
        }
	}
}

#if defined(Uses_MPPT_Console)

void MPPT_Set_Default()
{
    printf("%s %s %s\n",TEXT_MPPT,TEXT_SET,TEXT_DEFAULT);
    
    tune.mppt.auto_mode = TUNE_MPPT_MODE_DEF;
    NVS_Write_Word(NVS_MPPT_AUTO_MODE,tune.mppt.auto_mode,NVS_VALID_DEFAULT);

    tune.mppt.ramp_up_percent = TUNE_MPPT_RAMP_UP_PERCENT_DEF;
    NVS_Write_Word(NVS_MPPT_RAMP_UP_PERCENT,tune.mppt.ramp_up_percent,NVS_VALID_DEFAULT);

    #if defined(Old_Code)
    tune.mppt.ramp_up_timer = TUNE_MPPT_RAMP_UP_TIMER_DEF;
    NVS_Write_Word(NVS_MPPT_RAMP_UP_TIMER,tune.mppt.ramp_up_timer,NVS_VALID_DEFAULT);
    #endif

    tune.mppt.ramp_up_delay = TUNE_MPPT_RAMP_UP_DELAY_DEF;
    NVS_Write_Word(NVS_MPPT_DELAY_RAMP,tune.mppt.ramp_up_delay,NVS_VALID_DEFAULT);

    tune.mppt.ramp_dn_percent = TUNE_MPPT_RAMP_DN_PERCENT_DEF;
    NVS_Write_Word(NVS_MPPT_RAMP_DN_PERCENT,tune.mppt.ramp_dn_percent,NVS_VALID_DEFAULT);

    tune.mppt.ramp_dn_delay = TUNE_MPPT_RAMP_DN_DELAY_DEF;
    NVS_Write_Word(NVS_MPPT_DELAY_DOWN,tune.mppt.ramp_dn_delay,NVS_VALID_DEFAULT);

    tune.mppt.lock.up.hvdc = ADC_PV10X_Calc_DCL( TUNE_MPPT_RAMP_UP_DEF_PV10X );
    NVS_Write_Word(NVS_MPPT_RAMP_UP,tune.mppt.lock.up.hvdc,NVS_VALID_DEFAULT);
    tune.mppt.lock.dn.hvdc = ADC_PV10X_Calc_DCL( TUNE_MPPT_RAMP_DN_DEF_PV10X );
    NVS_Write_Word(NVS_MPPT_RAMP_DN,tune.mppt.lock.dn.hvdc,NVS_VALID_DEFAULT);

    tune.mppt.ramp_trigger = TUNE_MPPT_RAMP_TRIG_DEF;
    NVS_Write_Word(NVS_MPPT_RAMP_TRIG,tune.mppt.ramp_trigger,NVS_VALID_DEFAULT);

//  tune.mppt.rcb.value = TUNE_MPPT_BITS_DEF;
    tune.mppt.rcb.flutter_mode=1;
    tune.mppt.rcb.print_track=1;
    NVS_Write_Word(NVS_MPPT_RCB,tune.mppt.rcb.value,NVS_VALID_DEFAULT);
    
    MPPT_Start(MPPT_READ_NVS|MPPT_PRINT_PARAMS);
}

/*
void MPPT_Set(Uint16 down_pvdc, Uint16 ramp_pvdc)
{
	NVS_Tune_Write_PV_Ramp_Down(down_pvdc);
	NVS_Tune_Write_PV_Ramp_Up(ramp_pvdc);
	NVS_VID_Write_Item(VID_MPPT_DELAY_RAMP_UP, TUNE_MPPT_RAMP_UP_DELAY_DEF);
	NVS_VID_Write_Item(VID_MPPT_DELAY_RAMP_DN, TUNE_MPPT_RAMP_DN_DELAY_DEF);
	NVS_VID_Write_Item(VID_POWER_UPPER_LIMIT, TUNE_POWER_UPPER_LIMIT_DEF);
	MPPT_Start(MPPT_READ_NVS|MPPT_PRINT_PARAMS);
}
*/

#if defined(SMI240_60) || defined(SMI480_60)

#if defined(DOCUMENTATION)
BEHAVIOR OF ORIGINAL 60 CELL MPPT
23.5 13w
24.0 70w	
25.0 114w	
25.5 117w
25.5 117w AC RESET FULL POWER 
#endif

#endif

void MPPT_print_1k_dp1(Uint16 num)
{
	printf("%u.%u", (num/10), (num %10));
}

void MPPT_print_1k_dp2(Uint16 num, Uint8*s)
{
    MPPT_print_1k_dp1(num);
    printf("%s",s);
}

void MPPT_print_param_1k(char *text, Uint16 value, unsigned char end_char)
{
	if (text!= NULL)
		printf("%s:",text);
    MPPT_print_1k_dp1(value);
    printf("%c",end_char);
}

void MPPT_print_ramp(char *s1, Uint16 pvdc, Uint16 pvpc)
{
	if (s1!= NULL)
		printf("%s:",s1);
  
    if (tune.mppt.auto_mode)
    {
        MPPT_print_1k_dp1(pvpc*10);
    }
    else
    {
        MPPT_print_1k_dp1(pvdc);
    }
    
	printf(" ");
}

void MPPT_console_print_config()
{
    
    printf("%s:%d ",TEXT_MPPT,tune.mppt.auto_mode);
    
    MPPT_print_ramp(TEXT_UP,tune.mppt.lock.up.pvdc,tune.mppt.ramp_up_percent);
	MPPT_print_ramp(TEXT_DN,tune.mppt.lock.dn.pvdc,tune.mppt.ramp_dn_percent);
    
    MPPT_print_param_1k(TEXT_TRIG,tune.mppt.ramp_trigger,' ');

    #if defined(Old_Code)
   	printf("%s:%d ",TEXT_TIMER,tune.mppt.ramp_up_timer);
    #endif
    
	printf("%s:R%u,D%u ",TEXT_DELAY,tune.mppt.ramp_up_delay,tune.mppt.ramp_dn_delay);    
    
    printf("\n");

}

// if pvdc less then 100 then x10 it 
Uint16 MPPT_parse_PV(Uint16 pvdc)
{
	// if pvdc less then 100 then x10 it 	
	if (pvdc < 100)		// less then 100 
		pvdc *= 10;		// x10 
	return pvdc;
}

Uint16 MPPT_parse_PVS(char *str)
{
//	32.2	
	Uint8 len = strlen(str);
	if ((len==4) && (str[2] == '.'))
	{
		str[2]=0;
		return (atoi(str) * 10) + atoi( (char*) &str[3] );
	}
	return MPPT_parse_PV( atoi(str) );
}

Uint8 MPPT_console_parse_args()
{
	if ( strcmp(TEXT_MPPT, (char*)arg1)==0)
	{

//      DEBUG *********************************************
		if ( strcmp(TEXT_DEBUG, (char*)arg2)==0)
		{
			mppt.bit.debug = !mppt.bit.debug;
			printf("DEBUG=%d \n",mppt.bit.debug);
		}
//      DEBUG *********************************************		
        
//      DN      *********************************************        
		else if ( strcmp(TEXT_DN, (char*)arg2)==0)
		{
            
            if ( strcmp(TEXT_DELAY, (char*)arg3)==0)
            {
                tune.mppt.ramp_dn_delay = Bounds_Check_Uint16( csib.value[3], TUNE_MPPT_RAMP_DN_DELAY_MIN, TUNE_MPPT_RAMP_DN_DELAY_MAX);
                NVS_VID_Write_Item(VID_MPPT_DELAY_RAMP_DN, tune.mppt.ramp_dn_delay);
            }
			else if (strcmp(TEXT_TIMER, (char*)arg3)==0)
			{
                Uint16 dn_cnt = csib.value[3];
                tune.mppt.ramp_dn_sample_cnt = dn_cnt;
                printf("DN SC:%d %d\n",tune.mppt.ramp_dn_sample_cnt, csib.value[3]);
//              NVS_Tune_Write_PV_Ramp_Down( pvdc );
			}
            
            else
            {
                Uint16 down = Bounds_Check_Uint16(csib.value[2], TUNE_MPPT_RAMP_DN_PERCENT_MIN, TUNE_MPPT_RAMP_DN_PERCENT_MAX);
                NVS_VID_Write_Item(VID_MPPT_RAMP_DN_PERCENT, down);
                printf("%s %s=%d%% %d %d\n",arg2,arg3, tune.mppt.ramp_dn_percent, down, csib.value[2] );
            }

            MPPT_Start(MPPT_READ_NVS|MPPT_PRINT_PARAMS);
		}
//      DN      *********************************************        

        
//      FLUT ************************************************        
		else if ( strcmp(TEXT_FLUT, (char*)arg2)==0)
		{
            tune.mppt.rcb.flutter_mode = !tune.mppt.rcb.flutter_mode;
			NVS_VID_Write_Item(VID_MPPT_RCB, tune.mppt.rcb.value);
			printf("%s=%d\n",arg2,tune.mppt.rcb.flutter_mode);
		}
//      FLUT ************************************************                
        
//      MODE *********************************************
		else if ( strcmp(TEXT_MODE, (char*)arg2)==0)
		{
            // MPPT MODE 
            Uint16 mode = Bounds_Check_Uint16(csib.value[2], TUNE_MPPT_MODE_MIN , TUNE_MPPT_MODE_MAX);
//          printf("MODE=%d %d %s\n",csib.value[2], mode, arg3);
            NVS_VID_Write_Item(VID_MPPT_MODE, mode);
            MPPT_console_print_config();
        }
//      MODE *********************************************
        
        
//      SET *********************************************                
		else if ( strcmp(TEXT_SET, (char*)arg2)==0)
		{
          	if (strcmp(TEXT_DEFAULT, (char*)arg3)==0)
            {
                MPPT_Set_Default();
            }
		}
//      SET *********************************************                
        
//      SHOW ************************************************
		else if ( strcmp(TEXT_SHOW, (char*)arg2)==0)
		{
            #if defined(Uses_MPPT_Print_Track)
          	if (strcmp(TEXT_TRACK, (char*)arg3)==0)
            {
                tune.mppt.rcb.print_track = !tune.mppt.rcb.print_track;
                NVS_VID_Write_Item(VID_MPPT_RCB, tune.mppt.rcb.value);
            	printf("%s=%d\n",arg3,tune.mppt.rcb.print_track);
            }
            #endif

            #if defined(Uses_MPPT_Print_Ramp)
          	if (strcmp(TEXT_RAMP, (char*)arg3)==0)
            {
                tune.mppt.rcb.print_ramp = !tune.mppt.rcb.print_ramp;
                NVS_VID_Write_Item(VID_MPPT_RCB, tune.mppt.rcb.value);
            	printf("%s=%d\n",arg3,tune.mppt.rcb.print_ramp);
            }
            #endif
//          else MPPT_Print_Tracking();
		}
//      SHOW ************************************************

//      START ***********************************************
		else if ( strcmp(TEXT_START, (char*)arg2)==0)
		{
			MPPT_Start(MPPT_READ_NVS|MPPT_PRINT_PARAMS);
		}
//      START ***********************************************        

/*        
//      TEST *********************************************                
		else if ( strcmp("TEST", (char*)arg2)==0)
		{
            Uint16 up = Bounds_Check_Uint16(csib.value[2], 1, 40);
            printf("RAMP %s %d %d\n",arg2,up, csib.value[2]);
  			mppt.watts += up;
		}
//      TEST *********************************************                        
*/
        
//      TRIGGER *********************************************                
		else if ( strcmp(TEXT_TRIG, (char*)arg2)==0)
		{
            Uint16 limit = Bounds_Check_Uint16(csib.value[2], TUNE_MPPT_RAMP_TRIG_MIN  , TUNE_MPPT_RAMP_TRIG_MAX);
            printf("%s=%d,%d\n",arg2, csib.value[2], limit);
            NVS_VID_Write_Item(VID_MPPT_RAMP_TRIG, limit);
            MPPT_Start(MPPT_READ_NVS|MPPT_PRINT_PARAMS);
		}
//      TRIGGER *********************************************                        
        
//      UP      *********************************************
		else if ( strcmp(TEXT_UP, (char*)arg2)==0)
		{
            if ( strcmp(TEXT_DELAY, (char*)arg3)==0)
            {
                tune.mppt.ramp_up_delay = Bounds_Check_Uint16( csib.value[3], TUNE_MPPT_RAMP_UP_DELAY_MIN, TUNE_MPPT_RAMP_UP_DELAY_MAX);
                NVS_VID_Write_Item(VID_MPPT_DELAY_RAMP_UP, tune.mppt.ramp_up_delay);
                MPPT_Start(MPPT_READ_NVS|MPPT_PRINT_PARAMS);
            }
			else if (strcmp(TEXT_PV, (char*)arg3)==0)
			{
                Uint16 pvdc = MPPT_parse_PVS((char*)arg4);
                NVS_Tune_Write_PV_Ramp_Up( pvdc );
                printf("%s %s %d\n",arg2,arg3, pvdc);
			}

            #if defined(Old_Code)
			else if (strcmp(TEXT_TIMER, (char*)arg3)==0)
			{
                Uint16 up_timer = Bounds_Check_Uint16(csib.value[3], TUNE_MPPT_RAMP_UP_TIMER_MIN,TUNE_MPPT_RAMP_UP_TIMER_MAX);
                printf("%s=%d,%d\n",arg3, csib.value[3], up_timer);
                NVS_VID_Write_Item(VID_MPPT_RAMP_UP_TIMER, up_timer);
			}
            #endif
            
            else
            {
                Uint16 up = Bounds_Check_Uint16(csib.value[2], TUNE_MPPT_RAMP_UP_PERCENT_MIN, TUNE_MPPT_RAMP_UP_PERCENT_MAX);
                NVS_VID_Write_Item(VID_MPPT_RAMP_UP_PERCENT, up);
                
                printf("%s %s=%d%% %d %d\n",arg2,arg3, tune.mppt.ramp_up_percent, up, csib.value[2] );
                
            }
            
            MPPT_Start(MPPT_READ_NVS|MPPT_PRINT_PARAMS);
		}
//      UP      *********************************************
        
		else 
		{
			MPPT_console_print_config();
		}
		
		return 1;
	}

	return 0;
}
#endif

#if defined(Uses_MPPT_Debug)

void MPPT_Print_VDC(Uint16 pv, Uint16 hv)
{
    printf("%d.%d,%d", (pv/10),(pv%10),hv);
}

void MPPT_Print_ADC(Uint16 sample)
{
    printf(",A%d ",DCLINK_GET_VDC(sample));
}

void MPPT_Print_Up()
{
    printf("UP:%d%%,",tune.mppt.ramp_up_percent);
    MPPT_Print_VDC(tune.mppt.run.up.pvdc,tune.mppt.run.up.hvdc);
    MPPT_Print_ADC(mppt.up_sample);
//  printf(",A%d ",DCLINK_GET_VDC(mppt.up_sample));
}

void MPPT_Print_Dn()
{
    printf("DN:%d%%,",tune.mppt.ramp_dn_percent);
    MPPT_Print_VDC(tune.mppt.run.dn.pvdc,tune.mppt.run.dn.hvdc);
    MPPT_Print_ADC(mppt.dn_sample);
}

void MPPT_Print_Ramp_Pcent()
{
//  +903%  -903%
    printf("RP:%d,%d ",  mppt.up.pcent,mppt.dn.pcent);
}

void MPPT_Print_Ramp_Time()
{
    printf("RT:%d,%d ",  timer.mppt_ramp_up,timer.mppt_ramp_dn);
}

void MPPT_Print_Auto()
{
    if (tune.mppt.rcb.print_track)
    {
        printf("ST:");
        MPPT_Print_VDC(mppt.start.pvdc,mppt.start.hvdc);
        printf(" ");

        MPPT_Print_Up();
        MPPT_Print_Dn();
    
        printf("T%d\n", mppt.start.timer );
    }
    
}


#endif


#if defined(Uses_MPPT_Print_Flutter)
void MPPT_Print_Flutter()
{
    if ((mppt.print_cnt >=4) || (!tune.mppt.rcb.print_track))
        return;
    
    printf("FLUTTER:%d ", mppt.flutter.count);
    
    printf("UP:%d,%d%%,A:%d ", 
            mppt.up.count, mppt.up.pcent, timer.mppt_adc_up
    );

    printf("DN:%d,%d%%,A:%d ", 
            mppt.dn.count,  mppt.dn.pcent,timer.mppt_adc_dn
    );
    
    printf("\n");
    
    mppt.print_cnt++;
}
#endif

#if defined(Uses_MPPT_Print_Track)
void MPPT_Print_Tracking()
{
    if (tune.mppt.rcb.print_track)
    {
    
        #if !defined(Uses_Print_Track)
        if (mppt.print_cnt > 4)
            return;
    
        printf("%s %s:%d,%d,%d%% DN:%d,%d%% ",
                TEXT_TRACK, TEXT_UP,
                mppt.up.count, tune.mppt.ramp_trigger, mppt.up.pcent, 
                mppt.dn.count, mppt.dn.pcent
        ); 

        if (mppt.dn.count)
        {
            MPPT_Print_Dn();
            printf(" ");
        }

        printf("T%d\n", mppt.start.timer );
        #endif
    }
}
#endif


#if defined(Uses_MPPT_Print_Ramp)
void MPPT_Print_Ramp()
{
    printf("TRIG:");
    MPPT_print_1k_dp1(tune.mppt.ramp_trigger);
    
    printf(" UP:");
    MPPT_print_1k_dp1(mppt.up.pcent);
//  printf(",A%u,%u ", timer.mppt_adc_up, tune.mppt.ramp_up_timer);    
//  printf(",A%u ", timer.mppt_adc_up);
    
    printf(" DN:");
    MPPT_print_1k_dp1(mppt.dn.pcent);
//  printf(",A%u ",timer.mppt_adc_dn);
    
    printf(" T%u\n", mppt.start.timer );

}
#endif


#endif