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

void __attribute__((interrupt, auto_psv)) _T4Interrupt(void)
{

    #if defined(Uses_Debug_PWM2)
    if (dcb.pwm.pwm1_lock)
    {
        HAL_ISR_TMR4_PWM_SINE_IF = 0;	// DGA clear ISR flag 
        return;
    }
    #endif
    
    HAL_ISR_ADC1_IE = 0;                // DGA 2023-09-01 ADC ISR DISABLE 
    
	if (    
            (fault.alarm.value == 0)
            && (pwm.sample_counter < SINE_TABLE_LENGTH)
        )
    {
		#define	DIV_2048_SHIFT		11
		
        PWM_SINE_DUTY = ((Uint32) pwm.amp.scale * (pwm_sine_table[pwm.sample_counter]) >> DIV_2048_SHIFT);
		
		// DGA Bounds check duty cycle - don't allow at or above Period PRX_SINE frequency 
		if (PWM_SINE_DUTY > (PWM_SINE_PRX-1))
		{
			PWM_SINE_DUTY = PWM_SINE_PRX-1;	// DGA Maximum 99%
		}
        
    if (APP_State_Get() == INVERTER_CONNECT)
    {
        if (fault.alarm.value == 0)
        {
            if ((phase.dir.flag==0) && (pwm.sample_counter==99) && (phase.bit.trig_positive1))
            {
                if (++pwm.buck.delay >= 4) 
                {
                    fault.alarm.disc=0;

//****************************************************************************************
//                  NEW BETA CODE 02-07-2024 Capture open circuit right before buck enable
//                  mppt.open.sample=analog.dc.sample;
                    mppt.open.sample=analog.dc.avg.sample;
                    mppt.open.hvdc=analog.dc.avg.volts;
//****************************************************************************************
                    APP_State_Set(INVERTER_RUN);
                    HAL_H_BRIDGE_Set(H_BRIDGE_FORWARD_LIVE);	// DGA Positive phase
                    PWM_BUCK_LATCH_ON();	// DGA actually turn on the buck drive
                    pwm.bit.buck_active=1;
                    timer.buck_ms = 0;	// Reset timer
                    timer.buck_sec=0;
                }
            }
        }
    }
    else if (APP_State_Get() == INVERTER_RUN)
    {
        if (fault.alarm.value == 0)
        {
            #if defined(Uses_Debug_UFB)
            
            if (debug_ufb.bits.pending)
            {
                if (pwm.sample_counter==debug_ufb.start)
                {
                    debug_ufb.bits.pending=0;

                    #if defined(Uses_Debug_DCV_Report)
                    if (debug_ufb.bits.sample)
                    {
                        debug_dcv.bits.sample=1;
                        debug_ufb.bits.sample=0;
                        debug_dcv.bits.ufb_disabled=1;
                    }
                    #endif
                    
                    debug_ufb.bits.active=1;
                    debug_ufb.bits.notify=1;
                    debug_ufb.count=0;
                    
                    HAL_H_BRIDGE_Set(H_BRIDGE_DISABLED);
                }
            }
            
            if (debug_ufb.bits.active)
            {
                debug_ufb.count++;
                if (debug_ufb.count >= 800)
                {
                    debug_ufb.bits.active=0;
                    debug_ufb.bits.limit=1;
//                  debug_ufb.count=0;
                    if (HAL_H_BRIDGE_Get()!= H_BRIDGE_DISABLED) 
                        HAL_H_BRIDGE_Set(H_BRIDGE_PHASE_FLAG);
                    debug_ufb.bits.notify=1;
                }
            }
            
/*            
            if (debug_dcv.bits.pending)
            {
                if (pwm.sample_counter==debug_dcv.start)
                {
                    debug_dcv.bits.pending=0;
                    debug_dcv.bits.sample=1;
                    
                    if (debug_ufb.bits.pending)
                    {
                        debug_ufb.bits.pending=0;
                        debug_ufb.bits.active=1;
                        debug_dcv.bits.ufb_disabled=1;
                        HAL_H_BRIDGE_Set(H_BRIDGE_DISABLED);
                    }
                }
            }
 */
            
/*            
            if (debug_dcv.bits.sample)
            {
                debug_dcv.sample[debug_dcv.count] = analog.dc.sample;
                debug_dcv.count++;
                if (debug_dcv.count >= debug_dcv.limit)
                {
                    debug_dcv.bits.sample=0;
                    debug_dcv.bits.sample_done=1;
                    if (debug_ufb.bits.active)
                    {
                        debug_ufb.bits.active=0;
                        if (HAL_H_BRIDGE_Get()!= H_BRIDGE_DISABLED) 
                            HAL_H_BRIDGE_Set(H_BRIDGE_PHASE_FLAG);
                        debug_ufb.bits.notify=1;
                        
                    }
                }
            }
 */ 
            #endif
            
            if ((HAL_H_BRIDGE_Get()== H_BRIDGE_DISABLED) && (phase.bit.trig_alternate))
            {
                {
                    HAL_H_BRIDGE_Set(H_BRIDGE_PHASE_FLAG);
                    PWM_BUCK_LATCH_ON();              // DGA ENABLE buck drive
                    pwm.bit.buck_active=1;
                    pwm.bit.full_cycle=1;             // signal to later fault check 
                }
            }
            

            if (pwm.sample_counter==99)    
            {
                analog.bit.dclink_calc_avg=1;
                pwm.avg.ocx1.sum += PWM_SINE_DUTY;
                pwm.avg.ocx1.cnt++;
                
                #if defined (Uses_P24_WDT)
                WDT_Reset();
                #endif

            }

  
/*            
            if (pwm.sample_counter==pwm.dcf_state)
            {
                if (pwm.rip.calc >= 2)
                {
                    if (analog.dc.sample > pwm.dcf_fault_sample)
                    {
//                      IO_PIN_BUCK_LATCH=0;
//                      OCX_SINE = 0;
  //                    HAL_Fault_Set(ALARM_DISC|ALARM_DC_LINK_HIGH,FC_DCV_RIPPLE2,analog.dc.sample,pwm.dcf_fault_sample);
                        
                        debug_dcv_fault_assert(__FILE__,__LINE__, analog.dc.sample,pwm.dcf_fault_sample);
                    }
                }
            }
*/

            #if defined(Uses_PWM_Ripple_Fault)
            if ((pwm.bit.dc_fault_check) && (pwm.sample_counter >= 100))
            {
                if (analog.dc.sample > pwm.rip.dclink_fault_table[pwm.sample_counter] )
                {
                    pwm.sine_error_count++;

                    #if defined(Uses_Debug_DCV)
                   if (pwm.sine_error_count > 1)
                        debug_dcv_sine_fault(__FILE__,__LINE__, 0);
                    #endif
                    
                    if (pwm.sine_error_count >= pwm.sine_error_limit)
                    {
                        PWM_BUCK_LATCH_OFF();
                        PWM_SINE_DUTY = 0;
                        HAL_Fault_Set(ALARM_DISC|ALARM_DC_LINK_HIGH,FC_DCV_RIPPLE1,analog.dc.sample,pwm.rip.dclink_fault_table[pwm.sample_counter]);
                        pwm.fault_error_count++;
                    }
                }
                else
                {
                    if (pwm.sine_error_count)
                        pwm.sine_error_count--;
                }
            }
        #endif
            
            if (pwm.sample_counter==199) 
            {
                // If we reach state 199 then its a fault.  
                // The grid freq is 5% faster.  
                PWM_BUCK_LATCH_OFF();                // kill the buck
                pwm.bit.buck_active=0;
                
                HAL_Fault_Set(ALARM_DISC|ALARM_UNDER_FREQUENCY,FC_GEO_ZC_FREQ_LO,0,GEO_Nvs.freq.limit.lower.tmr1);

            }
 
        }
        else
        {
            PWM_BUCK_LATCH_OFF();
            pwm.bit.buck_active=0;
            HAL_H_BRIDGE_Set(H_BRIDGE_DISABLED);
        }
    }
        
//	*************** H-BRIDGE CONTROL ******************
		
    }
	else
    {
        PWM_BUCK_LATCH_OFF();
        pwm.bit.buck_active=0;
        HAL_H_BRIDGE_Set(H_BRIDGE_DISABLED);
        PWM_SINE_DUTY=0;                             // DGA Sine 0 duty 
    }

    #if defined(Uses_Debug_ADC_PWM)
    if (phase.dir.flag==1)
    {
        debug_pwm_adc[pwm.sample_counter] = debug_adc_grid_sample;
        debug_pwm_time1[pwm.sample_counter] = debug_adc_grid_time;
        debug_pwm_time2[pwm.sample_counter] = TMR1;

    }
    #endif
    
	pwm.sample_counter++;
	// ***************** DGA ***************** 	
	
	HAL_ISR_TMR4_PWM_SINE_IF = 0;	// DGA clear ISR flag 
    HAL_ISR_ADC1_IE = 1;			// DGA 2023-09-01 ADC ISR DISABLE 
}

	#if defined(Uses_Sine_Table2)
	Uint16 pwm_sine_table[SINE_TABLE_LENGTH]=
	{
		0,		// 0
		8,		// 1
		16,		// 2
		24,		// 3
		32,		// 4
		40,		// 5
		48,		// 6
		56,		// 7
		64,		// 8
		72,		// 9
		80,		// 10
		88,		// 11
		96,		// 12
		104,		// 13
		112,		// 14
		119,		// 15
		127,		// 16
		135,		// 17
		143,		// 18
		150,		// 19
		158,		// 20
		166,		// 21
		173,		// 22
		181,		// 23
		189,		// 24
		196,		// 25
		203,		// 26
		211,		// 27
		218,		// 28
		225,		// 29
		233,		// 30
		240,		// 31
		247,		// 32
		254,		// 33
		261,		// 34
		268,		// 35
		275,		// 36
		281,		// 37
		288,		// 38
		295,		// 39
		301,		// 40
		308,		// 41
		314,		// 42
		320,		// 43
		327,		// 44
		333,		// 45
		339,		// 46
		345,		// 47
		351,		// 48
		357,		// 49
		362,		// 50
		368,		// 51
		373,		// 52
		379,		// 53
		384,		// 54
		390,		// 55
		395,		// 56
		400,		// 57
		405,		// 58
		410,		// 59
		414,		// 60
		419,		// 61
		424,		// 62
		428,		// 63
		432,		// 64
		437,		// 65
		441,		// 66
		445,		// 67
		449,		// 68
		452,		// 69
		456,		// 70
		460,		// 71
		463,		// 72
		466,		// 73
		470,		// 74
		473,		// 75
		476,		// 76
		479,		// 77
		481,		// 78
		484,		// 79
		486,		// 80
		489,		// 81
		491,		// 82
		493,		// 83
		495,		// 84
		497,		// 85
		499,		// 86
		501,		// 87
		502,		// 88
		503,		// 89
		505,		// 90
		506,		// 91
		507,		// 92
		508,		// 93
		509,		// 94
		509,		// 95
		510,		// 96
		510,		// 97
		510,		// 98
		510,		// 99
//  Top Dead Center of Sine Wave
		510,		// 100
		510,		// 101
		510,		// 102
		510,		// 103
		509,		// 104
		509,		// 105
		508,		// 106
		507,		// 107
		506,		// 108
		505,		// 109
		503,		// 110
		502,		// 111
		501,		// 112
		499,		// 113
		497,		// 114
		495,		// 115
		493,		// 116
		491,		// 117
		489,		// 118
		486,		// 119
		484,		// 120
		481,		// 121
		479,		// 122
		476,		// 123
		473,		// 124
		470,		// 125
		466,		// 126
		463,		// 127
		460,		// 128
		456,		// 129
		452,		// 130
		449,		// 131
		445,		// 132
		441,		// 133
		437,		// 134
		432,		// 135
		428,		// 136
		424,		// 137
		419,		// 138
		414,		// 139
		410,		// 140
		405,		// 141
		400,		// 142
		395,		// 143
		390,		// 144
		384,		// 145
		379,		// 146
		373,		// 147
		368,		// 148
		362,		// 149
		357,		// 150
		351,		// 151
		345,		// 152
		339,		// 153
		333,		// 154
		327,		// 155
		320,		// 156
		314,		// 157
		308,		// 158
		301,		// 159
		295,		// 160
		288,		// 161
		281,		// 162
		275,		// 163
		268,		// 164
		261,		// 165
		254,		// 166
		247,		// 167
		240,		// 168
		233,		// 169
		225,		// 170
		218,		// 171
		211,		// 172
		203,		// 173
		196,		// 174
		189,		// 175
		181,		// 176
		173,		// 177
		166,		// 178
		158,		// 179
		150,		// 180
		143,		// 181
		135,		// 182
		127,		// 183
		119,		// 184
		112,		// 185
		104,		// 186
		96,		// 187
		88,		// 188
		80,		// 189
		72,		// 190
		64,		// 191
		56,		// 192
		48,		// 193
		40,		// 194
		32,		// 195
		24,		// 196
		16,		// 197
		8,		// 198
		0 		// 199
	};
	#endif


	
#endif		