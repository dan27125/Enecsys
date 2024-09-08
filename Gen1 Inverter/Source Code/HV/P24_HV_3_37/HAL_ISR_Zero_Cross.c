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

	#define FREQUENCY_SCALE         (600)

/*******************************************************************/
/* Function:    INT0Interrupt                                      */
/* Description: Zero crossing point interrupt handler.             */
/*******************************************************************/

void __attribute__((interrupt,auto_psv)) _INT0Interrupt(void)
{
    
    #if defined(Uses_ISR_Nesting)
    HAL_ISR_ADC1_IE=0;
    HAL_ISR1_AC_PHASE_IE=0;
//  __write_to_IEC(HAL_ISR1_AC_PHASE_IE=0);
    #endif
    
    if (INTCON2bits.INT0EP)
    {
        #if defined(Uses_Debug)
       	if (phase.dir.flag==0)
        {
            trace.zc.neg.cnt++;
            trace.zc.neg.timer_sum += (TMR1-timer.tmr1_reg);
        }
        #endif

        
		HAL_ISR0_AC_ZERO_CROSS_IF=0;
        INTCON2bits.INT0EP = 0;     // Alternate to 0=Positive edge
        
        #if defined(Uses_ISR_Nesting)
        HAL_ISR_ADC1_IE = 1;		// DGA 2023-09-01 ADC ISR ENABLE
        #endif
		return;
    }
    
    timer.tmr1_reg = TMR1;      // Save TMR0 value for critical grid frequency measurements 
  	
	#if defined(Uses_Diagnostics)
	if (APP_State_Get() == INVERTER_DIAG_MODE)
	{
		diag.isr_zc++;
		PWM_BUCK_LATCH_OFF();
		HAL_ISR_TMR4_PWM_SINE_IE = 0;	// Disable PWM ISR enable	
       	HAL_ISR_ADC1_IE = 1;		// DGA 2023-09-01 ADC ISR ENABLE
		HAL_ISR0_AC_ZERO_CROSS_IF=0;
		return;
	}
	#endif
	
// ----------------------------------------------------------------

	timer.zc_ms=0;		// Reset ZC timer
    
    PWM_BUCK_LATCH_OFF();		// Disable BUCK enable
	PWM_SINE_DUTY = 0;
	OC1R = 0;
	OC3R = 0;
    HAL_ISR_TMR4_PWM_SINE_IE=0;
//  __write_to_IEC(HAL_ISR_TMR4_PWM_SINE_IE=0);
            
    HAL_H_BRIDGE_Set(H_BRIDGE_DISABLED);    // Disable H-Bridge
//	timer.dclink=0;                         // Reset Timer
//	analog.bit.dclink_timer = 0;
	
	GEO_ZC_ISR();                           // 2022-02-16 DGA new PLL phase 
    ADC_ZC_ISR();
    
//  PWM_ZC_ISR();
    
    PWM_Buck_Drive_Control();
    
	timer.zc_isr=0;
	timer.zc_freq=0;			// DGA reset frequency count timer 
    timer.tmr1_reg2 = TMR1;      // Save TMR0 value for critical grid frequency measurements 
    
	#if defined(Uses_Debug)
	trace.cnt_zc++;
//  dcb.int1_phase.trig_isr++;
	#endif
  
	HAL_ISR0_AC_ZERO_CROSS_IF=0;
    #if defined(Uses_ISR_Nesting)
    HAL_ISR1_AC_PHASE_IE=1;
//  __write_to_IEC(HAL_ISR1_AC_PHASE_IE=1);
 	HAL_ISR_ADC1_IE = 1;		// DGA 2023-09-01 ADC ISR ENABLE    
//  __write_to_IEC(HAL_ISR_ADC1_IE=1);
    #endif
}
