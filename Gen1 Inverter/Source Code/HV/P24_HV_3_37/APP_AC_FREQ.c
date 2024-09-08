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



#if !defined(Uses_Freq_Hertz)
Uint16 APP_AC_Freq_Query_Timer(Uint16 timer1)
{
    if ( (timer1 <= GEO_Nvs.freq.F60.lower.tmr1) && (timer1 >= GEO_Nvs.freq.F60.upper.tmr1))
	{
		return 60;
	}
	else if ( (timer1 <= GEO_Nvs.freq.F50.lower.tmr1) && (timer1 >= GEO_Nvs.freq.F50.upper.tmr1))
	{
		return 50;
	}
	return 0;
}
void APP_AC_Freq_Limits_Timer(Uint16 timer1)
{
    
//	DGA ***** PHASE LOCK LOOP TO AUTO DETECT GRID FREQUENCY ****
	if (APP_State_Get() == INVERTER_START)	// DGA START state?
	{
		Uint16 freq_query;
		
		freq_query = APP_AC_Freq_Query_Timer(timer1);	// DGA Query grid freq
		
		if (freq_query==0)				// DGA Out of bounds ?
		{
			GEO_freq.zc.start.cnt=0;		// DGA Yes hold PLL in reset 
			return;						// DGA Yes QUIT,ret
		}
		
		else if (GEO_freq.zc.start.cnt==0)			// DGA First cycle ?
		{
			GEO_freq.zc.start.hz = freq_query;	// DGA Yes then save freq 
			GEO_freq.zc.start.cnt++;			// DGA Yes then bump counter to switch flow
		}
		else if (freq_query==GEO_freq.zc.start.hz)	// DGA Does this freq match last one ?
		{
			if (GEO_freq.bit.lock==0)
			{
				if (GEO_freq.zc.start.cnt++ >= GEO_FREQ_LOCK_NUM_SAMPLES)	// DGA enough samples for a LOCK ?
				{
                    GEO_Config.grid_freq = GEO_freq.zc.start.hz;
                    GEO_Set();	
                    
					GEO_freq.zc.start.lock = GEO_freq.zc.start.hz;	
					GEO_freq.bit.lock=1;                            // DGA LOCK GEO
                    HAL_Fault_Clear(ALARM_OVER_FREQUENCY|ALARM_UNDER_FREQUENCY|ALARM_PHASE);

    				#if defined(Uses_Debug_Start)
					printf("GEO LOCK:%u FREQ:%u.%uHZ\n", GEO_freq.zc.start.lock,GEO_freq.zc.instant/10,GEO_freq.zc.instant%10);
					#endif

				}
			}
		}
		else
		{
			GEO_freq.zc.start.cnt=0;	// DGA reset phase lock count 
		}
		
		return;
	}
//	DGA ***** PHASE LOCK LOOP TO AUTO DETECT GRID FREQUENCY ****

    
    // *** WARNING *** GRID FREQ faults must be cleared here 
    // GRID faults are detected and set in module APP_PLL which is called from Zero crossing ISR 
    // That code DOES NOT clear GRID faults, only sets them.  
    // *** WARNING *** GRID FREQ faults must be cleared here 
    // If GRID in range and previous fault then clear fault here after enough good cycles
	if ((timer1 <= GEO_Nvs.freq.limit.lower.tmr1) && (timer1 >= GEO_Nvs.freq.limit.upper.tmr1))
    {
		if (fault.alarm.freq_low)   // if previous fault 
		{
			if (GEO_freq.zc.cycle_cnt_low++ >= GEO_FREQ_CYCLE_RESET_CNT)    // count cycles then reset 
			{
				fault.alarm.freq_low=0;		// DGA clear/reset the fault bit 
				GEO_freq.zc.cycle_cnt_low=0;	// DGA clear/reset the fault counter 
			}
		}
		
		if (fault.alarm.freq_high)
		{
			if (GEO_freq.zc.cycle_cnt_high++ >= GEO_FREQ_CYCLE_RESET_CNT)
			{
				fault.alarm.freq_high=0;    // DGA clear/reset the fault bit 
				GEO_freq.zc.cycle_cnt_high=0;   // DGA clear/reset the fault counter 
			}
		}
	}

}
#endif

void APP_AC_Freq_Reset()
{
//	GEO_freq.bit.lock=0;
	memset((void*)&GEO_freq.zc.start,0,sizeof(GEO_freq.zc.start));
}

void APP_AC_Freq_Task()
{
	if (GEO_freq.bit.zc_sample)
	{
        Uint16 timer_reg;
        
        GEO_freq.bit.zc_sample=0;
        
        if (GEO_freq.zc.tmr2==0)
        {
            timer_reg = GEO_freq.zc.tmr1;
        }
        else
        {
            timer_reg = GEO_freq.zc.tmr2;

            #if defined(Uses_Debug_GEO)
            if (GEO_Nvs.filter++ == 120)
            {
                GEO_Nvs.filter = 0;
                if (GEO_Nvs.bit.print_timer)
                {
                    printf("TMR:%u,%u FREQ:%u,%u AVG:%u\n",
                            GEO_freq.zc.tmr1,GEO_freq.zc.tmr2,
                            TIMER1_CALC_10X(GEO_freq.zc.tmr1),
                            TIMER1_CALC_10X(GEO_freq.zc.tmr2),
                            GEO_freq.zc.average
                    );
                }
            }
            #endif

            GEO_freq.zc.tmr2 = 0;
        }
        
        GEO_freq.zc.cycle_sum += timer_reg;             // DGA Add the average  
        GEO_freq.zc.cycle_cnt++;	// Bump cycle count 

		if (GEO_freq.zc.cycle_cnt == 32)			// DGA Reached avg limit
		{
            GEO_freq.zc.average = (GEO_freq.zc.cycle_sum >> 5); // Div 32
            GEO_freq.zc.instant = TIMER1_CALC_10X(GEO_freq.zc.average);
            GEO_freq.zc.average = GEO_freq.zc.instant;
			GEO_freq.zc.cycle_cnt=0;			// DGA reset to 1 
			GEO_freq.zc.cycle_sum = 0;			// DGA reset to zero 
			GEO_freq.bit.zc_average=1;          // DGA Set bit;
		}
        
		if (!fault.alarm.grid_low)
		{
            APP_AC_Freq_Limits_Timer(timer_reg);
		}
	}
 
	if (GEO_freq.bit.phase_sample)
	{
/*        
		if (!fault.alarm.grid_low)
		{
			// The following code is redundant.  It is being done with the Phase LO ISR 
			//	Which sets the sample flag above.  We bounds check frequency again here
			//  to prevent hardware damage from AC surge 
		}
		GEO_freq.bit.phase_sample=0;
*/
	}

    
}
    
#endif  