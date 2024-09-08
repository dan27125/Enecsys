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

/*******************************************************************/
/* Function:    INT1Interrupt                                      */
/* Description: Handler for the phase detection .			       */
/*******************************************************************/
void __attribute__((interrupt,auto_psv)) _INT1Interrupt(void)
{
    Uint16 timer_reg;
//	March 5, 2023 DGA - New Phase LO fault detection 
//	Here at neg phase ISR.  60 HZ = 60	

 #if defined(DOC)
    **** ISR-1 **** CNT:1 TMR:95,24964 TRG:1,0 Z:0     
    On rare occasion this ISR fires right after negative zero crossing
    at start of positive phase.  Debug profile looks like this:
    **** ISR-1 **** CNT:1 TMR:95,24964 TRG:1,0 Z:0         
    To filter test that (IO_PIN_GRID_ZC_PORT==0)
#endif    
// 	HAL_ISR_ADC1_IE = 0;			// DGA 2023-09-01 ADC ISR DISABLE 
    __write_to_IEC(HAL_ISR_ADC1_IE = 0);
    timer_reg = TMR1;
    if (INTCON2bits.INT1EP)
    {
        //  Here at start of Positive phase.  This is an error 
        INTCON2bits.INT1EP = 0;         // Alternate to 0=Positive edge
    }
//  On rare occasion this ISR fires during start of POS phase from ZC noise 
//  To filter test that (IO_PIN_GRID_ZC_PORT==0)
    
//    TIMER1_INT1_FILTER

    
    else if (timer_reg > TIMER1_INT1_FILTER)
    {
        if (phase.dir.flag==0)
        {
            #if defined(Uses_Fault_Phase_TRIG_REV_NEG)
            if (GEO_freq.bit.lock)
                HAL_Fault_Set(ALARM_DISC|ALARM_PHASE,FC_TRIG_REV_NEG,TMR1,0);
            #endif
        }
        else
        {
            if (phase.bit.trig_negative0==0)
            {
                phase.bit.trig_negative0=1;      // Trigger start of negative
                phase.bit.trig_negative1=1;      // Trigger start of negative
                phase.bit.trig_alternate=1;     // Trigger alternate 
                
                #if defined(Uses_Debug_Phase)
                trace.phase.neg.cnt++;
                trace.phase.neg.tmr += timer_reg;
                trace.phase.neg.tmr0 += timer.tmr1_reg;
                if (trace.phase.neg.tmin==0)
                    trace.phase.neg.tmin = timer_reg;
                else if (trace.phase.neg.tmin > timer_reg)
                    trace.phase.neg.tmin = timer_reg;
                if (trace.phase.neg.tmax < timer_reg)
                    trace.phase.neg.tmax = timer_reg;
                #endif

                //  Here at start of Negative phase
                if (GEO_freq.bit.lock)
                {
                    #if defined(DOC)
                    FAULT 4 TRIG_MIN_NEG  T1:12549 T2:12577
                    #define TIMER1_ADC_TRIG_NEG_LOWER_60HZ        25    // 50 Neg phase 
                    12577-25=12552
                    #endif
                    
                    if (timer_reg < timer.tmr1_reg+GEO_Nvs.freq.limit.trig.neg.lower) 
                    {
                        #if defined(Uses_Fault_Phase_TRIG_LO_NEG)
                        HAL_Fault_Set(ALARM_DISC|ALARM_PHASE,FC_TRIG_LO_NEG, timer_reg, timer.tmr1_reg+GEO_Nvs.freq.limit.trig.neg.lower);
                        #endif
                    }

                    // Check TRIG NEG TIMER max
                    else if (timer_reg > timer.tmr1_reg+GEO_Nvs.freq.limit.trig.neg.upper)
                    {
                        #if defined(Uses_Fault_Phase_TRIG_MAX_NEG)
                        HAL_Fault_Set(ALARM_DISC|ALARM_PHASE,FC_TRIG_MAX_NEG, timer_reg,timer.tmr1_reg+GEO_Nvs.freq.limit.trig.neg.upper);
                        #endif
                    }
                    
                }
                
            }
        }
    }

/*    
    #if defined(Uses_Debug)
    if (timer_reg < TIMER1_INT1_FILTER)
    {
        dcb.int1_phase.timer1 = TMR1;
        dcb.int1_phase.trig_neg = phase.bit.trig_negative0;
        dcb.int1_phase.trig_pos = phase.bit.trig_positive0;
        dcb.int1_phase.count++;
        dcb.int1_phase.timer2 = timer.tmr1_reg;
    }
    #endif
*/
    
    HAL_ISR1_AC_PHASE_IF = 0;       // Clear ISR flag 	
//  HAL_ISR_ADC1_IE = 1;			// DGA 2023-09-01 ADC ISR ENABLE
    __write_to_IEC(HAL_ISR_ADC1_IE=1);
}
