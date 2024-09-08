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


#if defined(NOTES)
       


#endif


void HAL_PWM_Init()
{
	PWM_SINE_PRX = PWM_SINE_PRX_46K;		// Set Sine PWM freq
	PWM_SINE_DUTY  = 0;					// Set duty to zero 

    OC2CON = OCM2 | OCM1;
    T2CON=DEF_TON;
        
    OC3CON = OCTSEL | OCM2 | OCM1;
    T3CON=DEF_TON;
    
    #if defined(Uses_Debug_DCV)
    pwm.dcf_state = 175;
    pwm.dcf_pcent = 50;
    #endif
   
    pwm.start_amp.watts = 5;
    pwm.start_amp.scale = PWM_Amplitude_Calc(5);
    
}

#if defined(SMI240_60)
	Uint16 pwm_watt_scale[PWM_WATT_SCALE_MAX]=
    {
        922,    // 01 500 362v 92.2/100	.922     
        926,    // 02 510 369v 92.6/100    .926    
        933,    // 03 520 377v 93.3/100    .933
        940,    // 04 530 384v 94/100      .94
        945,    // 05 540 390  94.5/100
        949,    // 06 550 398v 949
        955,    // 07 560 405v 95.5
        961,    // 08 570 411v 96.1
        966,    // 09 580 420v 96.6
        972,    // 10 590 427v 97.2
        978,    // 11 600 435v 97.7
        980,    // 12 610 442v 98.0 
       986,     // 13 620 448v 98.6
       990,    // 14 630 456v 99.0
       993 ,   // 15 640 464v 99.3
       998,    // 16 650 470v 99.8
       1000,    // 17 660 477 100
       1000,    // 18 670 485v 100
       1009,    // 19 680 493v 100.9/100   1.009
       1017,    // 20 690 500v 101.7/100   1.017
       1014,    // 21 700 506v 101.4/100   1.014
       1018,    // 22 710 514v 101.8/100      1.018
       1020,    // 23 720 521v 102/100
       1020,    // 24 730 528v 102
       1020,    // 25 740  102
       1020,    // 26 750 541v  102
       1020,    // 27 760 550v 102.3
       1020,    // 28 770 557v 102
       1020,    // 29 780
       1020     // 30 790 572 102.2
    };

Uint16 PWM_Get_Wattage_Scale(Uint16 dca)
{
    Uint8 ndx;
     
     if (dca < PWM_WATT_SCALE_BEG_ADC)
         dca = PWM_WATT_SCALE_BEG_ADC;
     
     ndx = (dca-PWM_WATT_SCALE_BEG_ADC) / 10;
     
     if (ndx > (PWM_WATT_SCALE_MAX-1))
         ndx = PWM_WATT_SCALE_MAX-1;
         
    return pwm_watt_scale[ndx];
}
#endif


#if defined(SMI360_72)
	static Uint16 pwm_watt_scale[PWM_WATT_SCALE_MAX]=
    {

        790,    // 01 
        800,    // 02 
        810,    // 03 
        820,    // 04  326v
        840,    // 05 
        900,    // 06 470 
        915,    // 07 480 
        930,    // 08 490
        960,    // 09 500
        980,    // 10 510
        1000,   // 11 520 
        1004,  // 12 530 
        1010,   // 13 540 390v
        
        1018,   // 14 550 398v
        1024,   // 15 560 405v 
        
        1033,   // 16 570 412v
        1037,   // 17 580 419v
        1043,   // 18 590 427v
        1049,   // 19 600 435v
        1054,   // 20 610 442v
        
        1061,   // 21 620 448v
        1067,   // 22 630 456v
        1071,   // 23 640 464v
        1077,   // 24 650 470v
        1083,   // 25 660 477v
        1088,   // 26 670 485v
        1090,   // 27 680 492v        
        1091,   // 28 690 500v
        1103 ,   // 29 700 506v
        1104,   // 30 710 514v
        1106,    // 31 720 520v
        1106     // 32 730 530v
    };
    

Uint16 PWM_Get_Wattage_Scale(Uint16 dca)
{
    Uint8 ndx;
     
     if (dca < PWM_WATT_SCALE_BEG_ADC)
         dca = PWM_WATT_SCALE_BEG_ADC;
     
     ndx = (dca-PWM_WATT_SCALE_BEG_ADC) / 10;
     
     if (ndx > (PWM_WATT_SCALE_MAX-1))
         ndx = PWM_WATT_SCALE_MAX-1;
         
    return pwm_watt_scale[ndx];

}
#endif

Uint16 PWM_Scale_Wattage(Uint16 watts, Uint16 dca)
{
	pwm.watts_scale = PWM_Get_Wattage_Scale(dca);
    return  (Uint16) (((Uint32) watts * pwm.watts_scale) / 1000);
}

Uint16 _Scale_Value_Uin16(Uint16 value, Uint8 pcent)
{
    return ((value * pcent) / 100);
}

#if defined(Uses_PWM_Ripple_Fault)

void PWM_Buck_Ripple_Calc()
{
    //*************** DGA RIPPLE FAULT ALGORITHM ************************************************************

        #if defined(SMI360_72)
//        #define RFD_TABLE_DEFAULT_OFFSET        4
//        #define RFD_TABLE_RUN_OFFSET1           5
//        #define RFD_TABLE_RUN_OFFSET2           5
        #endif

        #if defined(SMI240_60)

//        #define RFD_TABLE_DEFAULT_OFFSET        4
//        #define RFD_TABLE_RUN_OFFSET1           4
//        #define RFD_TABLE_RUN_OFFSET2           4

        #define RFD_TABLE_DEFAULT_OFFSET        4
        #define RFD_TABLE_RUN_OFFSET1           5
        #define RFD_TABLE_RUN_OFFSET2           5
        
        #endif

    
    pwm.sine_error_limit = 8;       // DGA 03/01/2024 Set default error limit

    if (pwm.rip.inhibit_counter)
    {
        pwm.sine_error_limit = 15;
    }

    if (analog.pwm.avg.volts > 420)
    {
        pwm.bit.dc_over_420v = 1;
        if (pwm.watts > 100)
            pwm.sine_error_limit -= 2;
        else
            pwm.sine_error_limit -= 1;
    }
    else
        pwm.bit.dc_over_420v = 0;

    if (pwm.milliamps)
    {
        Uint16 i;

        Uint16 hi_sample1;
        Uint16 hi_sample2;

        pwm.rip.div = tune.fault.ripple_div;

        #if defined(SMI240_60)
        // As voltage approaches and goes over 570 the ripple is no longer linear.  
        if (analog.pwm.avg.volts >= 570)
        {
            pwm.rip.div = tune.fault.ripple_div + (analog.pwm.avg.volts-569);
            if (pwm.rip.div > 32)
                pwm.rip.div = 32;
        }
        #endif

        #define Scale_Ripple(value,pcent)   ( (Uint16) (value * pcent) / 100)

        pwm.rip.calc = pwm.milliamps / pwm.rip.div;
        pwm.rip.calc50 = Scale_Ripple(pwm.rip.calc,50);

        #if defined(Uses_Debug_DCV)
        pwm.dcf_ripple = Scale_Ripple(pwm.rip.calc,pwm.dcf_pcent);
        pwm.dcf_fault_sample = analog.dc.ripple.hi.pwm_sample - pwm.dcf_ripple;
       
        pwm.avg2.sum1 += pwm.milliamps;
        pwm.avg2.sum2 += pwm.rip.calc;
        if (++pwm.avg2.cnt >= 50)
        {
            pwm.avg2.milliamps = pwm.avg2.sum1 / pwm.avg2.cnt;
            pwm.avg2.calc = pwm.avg2.sum2 / pwm.avg2.cnt;
            pwm.avg2.cnt=0;
            pwm.avg2.sum1=0;
            pwm.avg2.sum2=0;
        }
        
        #endif

        hi_sample1 = analog.dc.ripple.hi.pwm_sample + tune.fault.ripple_dc2;
        hi_sample2 = analog.dc.ripple.hi.pwm_sample + tune.fault.ripple_dc3;

        if (pwm.rip.inhibit_counter==0)
        {
            if (pwm.fault_error_count > 10)
            {
                pwm.sine_error_limit += pwm.fault_error_count / 10;
            }
        }

        #if defined(DOCUMENTATION)
        090=10% 100=20% 110=30% 119=39%
        #endif

        if (pwm.rip.calc)
        {
            Sint8 pcent;        
            Sint8 pcent_adjust = 0;

            #if defined(SMI360_72)
            if (analog.pwm.avg.volts < 350)
                pcent_adjust = -20;
            else if (analog.pwm.avg.volts < 400)
                pcent_adjust = -10;
            #endif

            for (i = 0; i < 90; i++)
            {
                pwm.rip.dclink_fault_table[i] = hi_sample1;
            }

            for (i = 90; i < 129; i++)
            {
                pcent = i-70;     
                pcent += pcent_adjust;
                if (pcent > 0)
                {
                    pwm.rip.dclink_fault_table[i] = hi_sample2 - Scale_Ripple(pwm.rip.calc,pcent);
                }
            }

            for (i = 130; i <= 149; i++)
            {
                pcent = 60 + ((i - 130)*2);
                pcent += pcent_adjust;
                if (pcent > 0)
                {
                    pwm.rip.dclink_fault_table[i] = hi_sample2 - Scale_Ripple(pwm.rip.calc,pcent);
                }
            }
            for (i = 150; i < 169; i++)
            {
                pcent = 99 - (i-150);
                pcent -= pcent_adjust;
                if (pcent > 0)
                {
                    pwm.rip.dclink_fault_table[i] = hi_sample2 - Scale_Ripple(pwm.rip.calc,pcent);
                }
            }
            for (i = 170; i < 199; i++)
            {
                pcent = 59 - (i-170);
                pcent -= pcent_adjust;
                if (pcent > 0)
                {
                    pwm.rip.dclink_fault_table[i] = hi_sample2 - Scale_Ripple(pwm.rip.calc,pcent);
                }
            }
        }
        else
        {
            Uint16 fault_sample = analog.dc.ripple.hi.pwm_sample + tune.fault.ripple_dc1;
            for (i = 0; i < 199; i++)
            {
                pwm.rip.dclink_fault_table[i] = fault_sample;
            }
        }

    }
    else
    {
        Uint16 i;
        Uint16 fault_sample = analog.dc.ripple.hi.pwm_sample + tune.fault.ripple_dc1;
        for (i = 0; i < 199; i++)
        {
            pwm.rip.dclink_fault_table[i] = fault_sample;
        }
    }

//*************** DGA FAULT ALGORITHM ************************************************************************                


    if ((tune.fault.bit.fault_ripple) && (pwm.rip.calc))
    {
        pwm.bit.dc_fault_check=1;
    }


/*        
        #if defined(Uses_Debug_DCV)
        if (pwm.bit.wattage_drop_10)
        {
            debug_dcf.trigger_wattage_drop=pwm.wattage_drop;
        }
        if (pwm.bit.dc_fault_check==0)
        {
            debug_dcf.trigger_no_dcf = 1;
        }
        #endif
*/
    
}
#endif

void PWM_Buck_Drive_Control()
{
    PWM_BUCK_LATCH_OFF();                        // Make sure buck is disabled
	pwm.sample_counter = 0;
    pwm.sine_error_count = 0;
    
    HAL_ISR_ADC1_IE=0;
    
    #if defined(Uses_ISR_Nesting)
    HAL_ISR_ADC1_IE=0;
    #endif
    
    analog.dc.ripple.lo.pwm_sample = analog.dc.ripple.lo.adc_sample;      // save lo sample
    analog.dc.ripple.hi.pwm_sample = analog.dc.ripple.hi.adc_sample;      // save hi sample
    analog.dc.ripple.dif.sample = (analog.dc.ripple.hi.adc_sample - analog.dc.ripple.lo.adc_sample);
            
    //  Reset the peak and dip samples
    analog.dc.ripple.lo.adc_sample = analog.dc.sample;   // reset
    analog.dc.ripple.hi.adc_sample = analog.dc.sample;   // reset
        
    analog.pwm.avg.sample = (Uint16) ((Uint32) (analog.pwm.avg.sum / analog.pwm.avg.cnt));
    analog.pwm.avg.cnt=0;
    analog.pwm.avg.sum=0;
    
    #if defined(Uses_ISR_Nesting)
    HAL_ISR_ADC1_IE=1;
    #endif
    
    HAL_ISR_ADC1_IE=1;

    #if defined(Uses_PWM_Ripple_Fault)
    if (pwm.rip.inhibit_counter)
    {
        pwm.rip.inhibit_counter--;
    }
    #endif
    
    if (analog.pwm.avg.sample )
    {
        analog.pwm.avg.volts = DCLINK_GET_VDC(analog.pwm.avg.sample);
    }
    else
    {
        analog.pwm.avg.volts = 0;
    }
    
    if ((APP_State_Get() == INVERTER_CONNECT) && (fault.alarm.value == 0))
    {
        pwm.buck.enable = 1;
        pwm.bit.value = 0;  // reset 

        #if defined(Uses_PWM_Ripple_Fault)
        pwm.rip.calc=0;     // reset 
        pwm.rip.calc50=0;   // reset 
        #endif
        
    }
    else if ((APP_State_Get() == INVERTER_RUN) && (fault.alarm.value==0))
    {
        
        pwm.bit.value=0;      // reset 
        pwm.buck.enable = 1;
        
        #if defined(Uses_PWM_Ripple_Fault)
        pwm.rip.calc=0;         // reset 
        pwm.rip.calc50=0;       // reset 

        if ((pwm.rip.calc50) && (!pwm.bit.wattage_drop_10))
        {
            if (analog.dc.ripple.dif.sample < pwm.rip.calc50)
            {
//              debug_dcv_fault_assert(__FILE__,__LINE__, analog.dc.ripple.lo.adc_sample,pwm.rip.calc);
                HAL_Fault_Set(ALARM_DISC|ALARM_DC_LINK_HIGH,FC_DCV_RIPPLE2,analog.dc.ripple.dif.sample,pwm.rip.calc50);
            }
        }
        #endif
			
		#if defined(Uses_PWM_Telem)
		pwm.avg.ocx2.sum += pwm.clock.ocx;
		pwm.avg.ocx2.cnt++;
		#endif

//*************** DGA MPPT ALGORITHM ************************************************************************
        
        pwm.wattage_drop=0;
        
        #if defined(Uses_AMP_Lock)
        if ((dcb.pwm.amp_lock==0) && (pwm.amp.watts < mppt.watts))			// DGA is PWM wattage less then suggested MPPT?
        #else
        if (pwm.amp.watts < mppt.watts)			// DGA is PWM wattage less then suggested MPPT?
        #endif  
		{    
			pwm.amp.watts++;					// DGA If Yes then slow ramp current towards suggested MPPT
		}
        #if defined(Uses_AMP_Lock)
        else if ((dcb.pwm.amp_lock==0) && (pwm.amp.watts > mppt.watts))
        #else
		else if (pwm.amp.watts > mppt.watts)
        #endif
		{   
            #if defined(Uses_Debug_DCV)
            debug_dcf.wattage_drop_cnt++;
            #endif

            if (pwm.wattage_drop >= 10)
            {
                pwm.bit.wattage_drop_10=1;

                #if defined(Uses_PWM_Ripple_Fault)
                pwm.rip.inhibit_counter= pwm.wattage_drop / 10;
                #endif
                
                #if defined(Uses_Debug_DCV)
                debug_dcf.trigger_wattage_drop = pwm.wattage_drop;
                #endif
            }
            
            if (pwm.wattage_drop >= 50)
            {
                pwm.bit.wattage_drop_50=1;
                #if defined(Uses_Debug_DCV)
                debug_dcf.trigger_wattage_drop = pwm.wattage_drop;
                #endif
            }
            
			pwm.amp.watts = mppt.watts;			// DGA If No then FAST drop current at suggested MPPT
		}
//		********** WATTAGE TO AMPLITUDE ********** 
        
		#if defined(Uses_AMP_Lock)
		if (dcb.pwm.amp_lock)
		{
			pwm.amp.scale = dcb.pwm.amp_lock;
            pwm.amp.watts = PWM_Get_Amplitude_Watts(dcb.pwm.amp_lock);
		}
        else
        {
    		pwm.amp.scale = PWM_Amplitude_Calc(pwm.amp.watts);	// DGA use lookup table to calc amplitude
        }
        #else
   		pwm.amp.scale = PWM_Amplitude_Calc(pwm.amp.watts);	// DGA use lookup table to calc amplitude
		#endif
        
        pwm.watts = PWM_Scale_Wattage(pwm.amp.watts, analog.pwm.avg.sample);
        pwm.milliamps = ( (Uint32) pwm.watts*1000) / analog.pwm.avg.volts;  // calc milliamps of current 
    //*************** DGA MPPT ALGORITHM ************************************************************************

        #if defined(Uses_PWM_Ripple_Fault)
        PWM_Buck_Ripple_Calc();
        #endif  
        
    }
    
    PWM_SINE_DUTY=0;
    
    if (pwm.buck.enable)
    {
        OC2CON = OCM2 | OCM1;
        T2CON=DEF_TON;
        
        OC3CON = OCTSEL | OCM2 | OCM1;
        T3CON=DEF_TON;

        //	pwm.clock.prx = PWM_CLOCK_PRX_200K;		// DGA 2022-02-15 200 khz 
        //  pwm.clock.ocx = PWM_CLOCK_DUTY95;       // DGA 2024-03-01 lower duty down to 95%
        
        pwm.clock.prx = PWM_CLOCK_PRX_200K;		// DGA 2022-02-15 200 khz 
        
        PWM_SINE_PRX = PWM_SINE_PRX_46K;		// Set Sine PWM freq
        
        #if defined(Uses_AMP_Lock)
        if (dcb.pwm.clock_duty_lock)
        {
           pwm.clock.ocx = PWM_CALC_OCX_DUTY(PWM_CLOCK_PRX,dcb.pwm.clock_duty_lock);
        }
        else
        {
            pwm.clock.ocx = PWM_CLOCK_DUTY99;       // DGA 2024-03-01 lower duty down to 95%    
        }
        #else
        pwm.clock.ocx = PWM_CLOCK_DUTY99;       // DGA 2024-03-01 lower duty down to 95%
        #endif
       
        PWM_CLOCK_PRX = pwm.clock.prx;
		PWM_CLOCK_DUTY = pwm.clock.ocx;
        
        PR4=GEO_Config.sine_pr4;
        TIMER2=0;
        TIMER3=0;
        TIMER4=0;
        
        T4CON = DEF_TON; // Timer4 on and operates as 16bit timer.
        HAL_ISR_TMR4_PWM_SINE_IF=0;
        HAL_ISR_TMR4_PWM_SINE_IE=1;
	}
    else
    {
        pwm.buck.enable=0;
        T3CONbits.TON = 0;
        T4CONbits.TON = 0; 
    }
	
}

void PWM_Buck_Drive_Task()
{
    if (pwm.bit.drive_trigger)
    {
  //    PWM_Buck_Drive_Control();
        pwm.bit.drive_trigger=0;
    }
}

Uint16 PWM_GET_OCX_DUTY10X(Uint16 prx, Uint16 ocx)
{
	Uint32 ocx1k;
	Uint16 resolution, duty;
	ocx1k = (ocx * 1000);		// shift number by 100 
	resolution = prx;
	duty = (ocx1k / resolution) + 1;
	return duty;	
}

Uint8 PWM_GET_OCX_DUTY(Uint16 prx, Uint16 ocx)
{
	Uint32 ocx100;
	Uint16 resolution, duty;
	ocx100 = (ocx * 100);		// shift number by 100 
	resolution = prx;
	duty = (ocx100 / resolution) + 1;
	return duty;	
}

void PWM_Clock_Duty_Set(Uint8 duty)
{
    pwm.clock.ocx = PWM_CALC_OCX_DUTY(PWM_CLOCK_PRX,duty);
    PWM_CLOCK_DUTY = pwm.clock.ocx;
}


#if defined(Uses_PWM_Telem)
void PWM_Telem_Calc_Avg()
{
	if ( HAL_Fault_Get()==0)
	{
		if (pwm.avg.ocx1.cnt)
		{
			pwm.avg.ocx1.reg = pwm.avg.ocx1.sum / pwm.avg.ocx1.cnt;
		}
		
		pwm.avg.ocx1.num = PWM_GET_OCX_DUTY(PWM_SINE_PRX, pwm.avg.ocx1.reg);
		
		if (pwm.avg.ocx2.cnt)
			pwm.avg.ocx2.reg = pwm.avg.ocx2.sum / pwm.avg.ocx2.cnt;
		
		
		pwm.avg.cnt_rpt = pwm.avg.ocx1.cnt;
		pwm.avg.ocx2.num = PWM_GET_OCX_DUTY(PWM_SINE_PRX, pwm.avg.ocx2.reg);
                
//		printf("PR1:%u OC1:%u,%u%c ", PRX_SINE, pwm.avg.ocx1.reg, pwm.avg.ocx1.num,'%');
//		printf("PR2:%u,%u OC2:%u,%u%c\n", pwm.avg.prx2.reg, pwm.avg.prx2.num, pwm.avg.ocx2.reg, pwm.avg.ocx2.num,'%');
		
		pwm.avg.ocx1.sum=0;
		pwm.avg.ocx1.cnt=0;
		pwm.avg.ocx2.sum=0;
		pwm.avg.ocx2.cnt=0;
		
	}
	else
	{
		pwm.avg.ocx2.num = 0;
		pwm.avg.cnt_rpt = 0;
	}
}
#endif


Uint8 PWM_Get_Grid_Voltage_Scale()
{
	Uint16 scale;
	scale = (240 * 100) / analog.ac.avg.volts;
	return scale;
}

/*
Uint16 PWM_Grid_LO_Scale_UP(Uint16 value)
{
	Uint8 scale;
	scale = PWM_Get_Grid_Voltage_Scale();
	// 240 = 100  120 = 200 
	if (scale > 100)
	{
		// 120v, 200 scale, value X 2	
		return ((Uint32) value * scale) / 100; 
	}
	else
	{
		// 250, 195, amp 100 
		return ((Uint32) value * scale) / 100;
	}
}
*/

/*
Uint16 PWM_Grid_LO_Scale_DN(Uint16 value)
{
	Uint8 scale;
	scale = PWM_Get_Grid_Voltage_Scale();

	if (scale > 100)
	{
		// 120v, 200 scale, value X 2	
		return ((Uint32) value * 100) / scale; 
	}
	else
	{
		// 250, 195, amp 100 
		return ((Uint32) value * 100) / scale;
	}	
}
*/

Uint16 PWM_Get_Amplitude_Watts(Uint16 amp)
{
    Uint16 i;
    
    for (i =0; i < WATT_TABLE_MAX; i++)
    {
        if (pwm_watt_table[i] >= amp)
            return i;
    }
	return 	0;
}

Uint16 PWM_Get_Amplitude_Scale(Uint16 watts)
{
	if (watts > WATT_TABLE_MAX)		// DGA bounds check 
		watts = WATT_TABLE_MAX;		// Limit to table max size 
	return 	pwm_watt_table[watts-1];	// DGA lookup amplitude for desired wattage
}

Uint16 PWM_Amplitude_Calc(Uint16 watts)
{
	Uint16 amplitude;
  
//  2024-02-08 - disabled    
//	watts = PWM_Grid_LO_Scale_UP(watts);    //  2024-02-08 - disabled    
//  2024-02-08 - disabled        
	
	if (watts > TUNE_POWER_UPPER_LIMIT_MAX)
		watts = TUNE_POWER_UPPER_LIMIT_MAX;
	
	amplitude = PWM_Get_Amplitude_Scale(watts);
	return amplitude;
}


void PWM_Power_Limit_Set(Uint16 value)
{
	#if defined(Uses_Console) 
	printf("LIMIT=%u\n",value);
	#endif
	NVS_VID_Write_Item(VID_POWER_UPPER_LIMIT, value);
	MPPT_Start(0);
}

void PWM_Run_Task()
{

/*    
    if (pwm.bit.calc_wattage)
    {
        pwm.bit.calc_wattage=0;
//        pwm.watts = PWM_Scale_Wattage(pwm.amp.watts, analog.pwm.avg.sample);
        
//      pwm.watts = pwm.amp.watts;
        
    }
 */   
    
}


#endif		