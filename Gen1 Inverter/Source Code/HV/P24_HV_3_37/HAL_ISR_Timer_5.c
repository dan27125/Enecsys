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

//******************************************************************
// Function:    _T5Interrupt   
// Description: software timer
//******************************************************************

void __attribute__((__interrupt__, auto_psv)) _T5Interrupt(void)
{		
	timer.ticks++;
	timer.timer++;	
	timer.delay++;
	
	#define	TIMER5_COUNT	6000

	#if defined(Uses_LLC_Bridge)		
	timer5_llc_tx++;
	#endif
	
	timer.zc_isr++;
	timer.zc_freq++;

//	**** PHASE SAMPLE CODE ****	
	if (IO_PIN_GRID_ZC_PORT==0)	// If NOT Zero crossing 
	{
		if (IO_PIN_GRID_PHASE_PORT)	// If negative phase 
		{
			phase.new.negative_cnt++;
			#if defined(Uses_Debug_Phase)
			trace.phase.timer5_neg_cnt++;
			#endif
		}
		else					// Else positive phase 
		{
			phase.new.positive_cnt++;
			#if defined(Uses_Debug_Phase)
			trace.phase.timer5_pos_cnt++;
			#endif
		}
	}
	
/*    
//	**********************************************************	
	if (++timer.dclink >= GEO_Config.dclink_sample_time)
	{
		timer.dclink=0;
		timer.bit.buck_start=1;
		
	    if (analog.bit.dclink_timer==0)
		{
			analog.bit.dclink_timer=1;
			analog.bit.dclink_sample=1;		// Notify ADC module 
		}
	}
//	**********************************************************		
*/
    
	if (++timer.div10 >= 10)	// 6000/10=600
	{
		timer.div10 = 0;
		
		if (++timer.div120 >= 5)
		{
			timer.div120 = 0;
			timer.bit.diag_120_hz=1;
		}
		
		if (++timer.div60 >= 10)
		{
			timer.div60 = 0;
			timer.bit.kwh=1;
            
		}
        if (++timer.dclink >= 10)
        {
            timer.dclink=0;
            analog.bit.dclink_calc_avg=1;   // trigger 
        }
	}
		
	
	if (++timer.div1 >= 6)	// 6000/6=1000 
	{
		timer.div1=0;
				
		if ((pwm.buck.enable) && (APP_State_Get() == INVERTER_RUN))
		{
			if (timer.buck_ms < 0xffff)
				timer.buck_ms++;
			
			if (++timer.buck_div >= 1000)
			{
				timer.buck_div=0;
				timer.bit.run_sec=1;
			}
		}

        
    //  ********* MPPT Timers  ********* 
        if (timer.mppt_ramp_up < 0xFFFF)
            timer.mppt_ramp_up++;
        
        if (timer.mppt_ramp_dn < 0xFFFF)
            timer.mppt_ramp_dn++;
        
        if (timer.mppt_adc_dn < 10000)
            timer.mppt_adc_dn++;
        
        if (timer.mppt_adc_up < 10000)
            timer.mppt_adc_up++;
        
		if (timer.mppt_up_delay)
			timer.mppt_up_delay--;    
		
		if (timer.mppt_dn_delay)
			timer.mppt_dn_delay--;
        
        //  ********* MPPT Timers  ********* 



        #if defined(Uses_Timer5_Fault)
        
        //  *****************************************************************************        
        //  *****************************************************************************
        //  ********** 2023-09-12 LOOK FOR ZERO CROSSING TIME OUT USING NEW TIMER1 PARAMS
//      if ((APP_State_Get() == INVERTER_RUN) && (fault.alarm.value==0))
//      if ((fault.alarm.value <= ALARM_DISC) && (GEO_freq.bit.lock))
        if (fault.alarm.value <= ALARM_DISC)
        {
            if (phase.dir.flag==0)
            {
                if (TMR1 > GEO_Nvs.freq.limit.lower.tmr1)
                {
                    HAL_Fault_Set(ALARM_DISC|ALARM_UNDER_FREQUENCY,FC_TMR5_POS,TMR1,GEO_Nvs.freq.limit.lower.tmr1);
                }
            }
            else
            {
                if (TMR1 > GEO_Nvs.freq.limit.lower.tmr1_phase2)
                {
                    HAL_Fault_Set(ALARM_DISC|ALARM_UNDER_FREQUENCY,FC_TMR5_NEG,TMR1,GEO_Nvs.freq.limit.lower.tmr1_phase2);
                }
            }
        }
        //  ********** 2023-09-12 LOOK FOR ZERO CROSSING TIME OUT USING NEW TIMER1 PARAMS        
        //  *****************************************************************************
        //  *****************************************************************************        
        #endif
        
        
        if (++timer.zc_ms >= GEO_Config.no_ac_timeout)
        {
            timer.zc_ms=0;
            analog.bit.grid_timer=1;
			
        }

		if (++timer.div2 >= 10)	// 1000/10=100
		{
			timer.div2=0;
			timer.bit.ms10=1;
			
			timer.bit.diag_100_hz=1;
           
            if (timer.temperature++ >= 50)
            {
                timer.temperature=0;	// Reset timer
				analog.bit.temp_timer=1;	// Trigger temperature calculations 
            }
			
			if (IO_PIN_BOOTLOADER_PORT)
			{
				APP_Bootloader_Reset();
			}
			
			if (++timer.div3 == 10) // 100/10=10
			{
				timer.div3=0;
				timer.bit.ms100=1;			// Scheduler timer 

				if (timer.stop_delay_ms100)
					timer.stop_delay_ms100--;
					
				timer.ac_connect_delay++;
				if ((timer.ac_connect_delay%10)==0)
					timer.bit.start_sec=1;
				
				if (++timer.div4 >= 5)
				{
					timer.div4=0;
					timer.bit.ms500=1;
					
					#if defined (Uses_LED) && !defined(Uses_LED_Dimmer)
//					HAL_LED_2_toggle();
					#endif
						
				}
				if (++timer.div5 >= 10)
				{
					timer.div5=0;
					timer.bit.sec=1;
					mppt.bit.sec=1;
					_boot_sec++;
					
					if (pwm.buck.enable)
					{
						if (timer.buck_sec < 0xffff)
							timer.buck_sec++;
					}
					
					timer.spi_sec++;
				}
			}
		}
	}
	
	timer.ms_cnt = timer.ticks / 60;	// Calc elapsed Ms 
	
	HAL_ISR_TMR5_IF = 0;	// DGA clear ISR flag
}

