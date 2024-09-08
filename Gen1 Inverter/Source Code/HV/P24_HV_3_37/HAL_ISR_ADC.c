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

#if defined(ADC_DOCUMENTATION)
PIC 24 ADC hardware and firmware HAL state machine operation.  The PIC24 ADC module can be configured to sequentially scan multiple channels and store the results in a 16 word FIFO.  
boot The ADC is initialized as follows:

	AD1CON3bits.ADRC=0;		// DGA 0 = Clock derived from system clock
	AD1CON3bits.SAMC=4;		// DGA auto sample time 
	AD1CON3bits.ADCS=4;		// 04=ISR 08,525, AC SAMPLES 68.2k
	AD1CON1bits.SSRC = 0b111;	// DGA 
	AD1CON1bits.ASAM=1;		// DGA 
	AD1CON2bits.SMPI = 0b1111;	// DGA get 16 samples before ISR
	AD1CON2bits.CSCNA = 1;		// DGA scan A/D to S/H positive input.


ADC software is configured with a one bit software state machine to sequentially scan two channels at a time.  ADC state 0 sequentially scans two analog inputs, AC and DC with a 99.99% duty cycle. ADC state 1 sequentially scans two analog inputs, AC and TEMPERATURE 60x a seccond during the Phase low zerro crossing, ultra low duty cycle.  When the 16 WORD FIFO is full, after 16 samples it generates an interrupt. Because it is always scanning (2) channels at a time the FIFO has 8 samples of each channel being scanned.  ADC ISR rate an average of about 8,525.   With 8 samples per ISR that means the AC voltage is continuoussly scanned at a rate of about 68khz.   The ISR code sums each channels data then shift divides by 8 to calculate the average.  Note about ADC speed: The hardware conversion clock period has an 8 bit prescale register (AD1CON3bits.ADCS) that controls the sample time.  Right now the software is setting AD1CON3bits.ADCS=4 which results in an ADC ISR frequency of 8,525. Here is a table showing profiled test results 

//  AD1CON3bits.ADCS=5;			// 05=ISR 8.7k AC SAMPLES 69k
//  AD1CON3bits.ADCS=4;			// 04=ISR 10.1k, AC SAMPLES 80.8k
//  AD1CON3bits.ADCS=3;			// 03=ISR 11,900, AC SAMPLES 95.1k 
//  AD1CON3bits.ADCS=2;			// 02=ISR 15.5k, AC SAMPLES 124.2k 
//  AD1CON3bits.ADCS=1;			// 01=ISR 20.8k, AC SAMPLES 166.4k     
//  AD1CON3bits.ADCS=0;			// 00=ISR 30.9k, AC SAMPLES 249.6k         


#define AC_SCALE_FLOAT		3.25     
The ADC AC voltage scale is 3.25 per volt.  For example: a 10 bit ADC read of 325 is 100 AC volts. 

EXAMPLE: ADC DEBUG PROFILE STATISTICS
-------------------------------------------------------------
ADC:8521 0:4293,92,24996 1:30,111,23747 3:4188,281,12611

#endif
    


void ADC_Init(void)
{
	memset((void*)&analog, 0, sizeof(analog));
    
    
		
	AD1CON1	= 0x0000;	// DGA CLEAR ALL BITS	
	AD1CON2	= 0x0000;	// DGA CLEAR ALL BITS
    AD1CSSL	= 0x0000;	// DGA CLEAR ALL BITS
//	DGA Microchip DS39705B-page 17-9
	AD1CON3bits.ADRC=0;	// DGA 0 = Clock derived from system clock

//  AD1CON3bits.ADCS=5;			// 05=ISR 8.7k AC SAMPLES 69k
//  AD1CON3bits.ADCS=4;			// 04=ISR 10.1k, AC SAMPLES 80.8k
//  AD1CON3bits.ADCS=3;			// 03=ISR 11,900, AC SAMPLES 95.1k 
//  AD1CON3bits.ADCS=2;			// 02=ISR 15.5k, AC SAMPLES 124.2k 
//  AD1CON3bits.ADCS=1;			// 01=ISR 20.8k, AC SAMPLES 166.4k     
//  AD1CON3bits.ADCS=0;			// 00=ISR 30.9k, AC SAMPLES 249.6k         
    
//    #if defined(Uses_Debug_ADC_PWM)
//    AD1CON3bits.ADCS=3;			// 00=ISR 31,200, AC SAMPLES 249.6k         
//    #else
//	AD1CON3bits.ADCS=3;			// 04=ISR 08,525, AC SAMPLES 68.2k
//    #endif

//  AD1CON3bits.ADCS=3;			// 04=ISR 08,525, AC SAMPLES 68.2k    
    AD1CON3bits.ADCS=1;			// 04=ISR 08,525, AC SAMPLES 68.2k
    
	AD1CON1bits.SSRC = 0b111;	// DGA 
	AD1CON2bits.SMPI = 0b1111;	// DGA get 16 samples before ISR

	AD1CON2bits.CSCNA = 1;		// DGA scan A/D to S/H positive input.
	AD1CSSL = 0;						// 
	AD1CSSLbits.CSSL_AC_GRID = 1;		// DGA Enable AN11 AC GRID
	AD1CSSLbits.CSSL_TEMP = 1;			// DGA Enable Temperature 
	
	analog.bit.grid_boot=0;

    temperature.ac_vdrop = 66;  // SET TO INIT DEFAULT 
	analog.ac_fault.sample = ACV_CALC_ADC35(ACV_ADC_VOLT_FAULT);
    
  	AD1CON1bits.ASAM=1;			// DGA 
		
	HAL_ISR_ADC1_IE = 1;			// DGA ISR ENABLE 
	HAL_ISR_ADC1_IF = 0;			// DGA CLEAR FLAG
	AD1CON1bits.ADON = 1;			// DGA TURN ON 
    
    
//#endif
    
    
}

/*
// 2021-12-24 DGA - 
 * AC non linear even when temperature is same
 * Inside on a test bench with VARIAC auto transformer 
 * when using default scale of 331 and temperature of 80F
 * Seems that ACV below about 230v starts to skew about 1% the value 
  * 240 shows about 240 
 * 230 shows up about 229 a 1% error 
 * 120 volts only shows up as 110 - a 10% error 
 * When AC voltage 120 the error 10% 
 * the following code is a fix to correct 
*/

Uint16 ADC_AC_LO_Correction(Uint16 acv)
{
	
	Uint16 limit = 230;;
	if (acv < limit)
	{
		Uint16  x;
		x = (limit-acv) / 13;	// 2021-12-24 DGA - 
		acv += x;
	}
	return acv;
}

Uint16 ADC_AC_Correction(Uint16 acv)
{
	return ADC_AC_LO_Correction(acv);
}

        
void __attribute__((interrupt,auto_psv)) _ADC1Interrupt(void)
{
	
//	***** DGA - CRITICAL STOP AUTO SAMPLE MODE ******
	AD1CON1bits.ASAM=0;		// DGA STOP AUTO SAMPLE MODE
//	***** DGA - CRITICAL STOP AUTO SAMPLE MODE *******
    
	#if defined(Uses_Debug_ADC)	
	trace.analog.cnt_isr1++;
	#endif
	
	if (AD1CON1bits.DONE)        
	{
		Uint16 sum1;
		Uint16 sum2;
		Uint16 grid_sample;
        
        Uint8 i;
        volatile Uint16 *adc;
		sum1=0;
		sum2=0;
     
        adc = &ADC1BUF0;
		for (i = 0; i < 16; i++)
		{
			sum1 += adc[i++];
			sum2 += adc[i];
		}
        
		if (AD1CSSLbits.CSSL_TEMP)
		{
			analog.temp.sample = (sum1 >> 3);	// div by 8	
            analog.bit.temp_sample = 1;	
			
			grid_sample = (sum2 >> 3);			// div by 8
			
			AD1CON1bits.ADON = 0;			// TURN Off 
			AD1CSSLbits.CSSL_TEMP = 0;		// Disable TEMP scans
			AD1CSSLbits.CSSL_DC_LINK = 1;	// Enable DCLINK scans
			AD1CON1bits.ADON = 1;			// TURN ON 			
			
			#if defined(Uses_Debug_ADC)
			trace.analog.temp.sample_cnt1++;
			#endif

		}
		else
		{
    
            #if defined(Uses_ISR_Nesting)
            HAL_ISR0_AC_ZERO_CROSS_IE=0;
            #endif

			grid_sample = (sum1 >> 3);		// div by 8
			
			#if defined(Uses_Debug_ADC)	
//			trace.cnt_adc_ac2++;
			#endif
            
			analog.dc.sample = (sum2 >> 3);	// div by 8
            
			#if defined(Uses_DCLink_Test_Mode)
			analog.dc.sample = 691;
			#endif

            analog.dc.avg.sum += analog.dc.sample;
			analog.dc.avg.cnt++;
            
            analog.pwm.avg.sum += analog.dc.sample;
            analog.pwm.avg.cnt++;
    
            
            #if defined(Uses_ISR_Nesting)
            HAL_ISR0_AC_ZERO_CROSS_IE=1;
            #endif
            
            #if defined(Uses_Debug_DCV)	
            debug_dcf.adc_sample_cnt++;
            #endif
            
                
//		****************** HVDC PEAK ********************
            if (analog.dc.ripple.hi.adc_sample < analog.dc.sample)
            {
                analog.dc.ripple.hi.adc_sample = analog.dc.sample;
            }
//		****************** HVDC PEAK ********************            

//		****************** HVDC DIP ********************
            if (analog.dc.ripple.lo.adc_sample > analog.dc.sample)
            {
                analog.dc.ripple.lo.adc_sample = analog.dc.sample;
            }
//		****************** HVDC DIP ********************
                
			
//		****************** HVDC RAMP DN ********************
			if (analog.dc.sample <	mppt.dn_sample)
			{
				mppt.bit.dclink_min=1;
			}
			else
			{
                timer.mppt_adc_dn = 0;  // RESET HVDC RAMP DN
				mppt.bit.dclink_min=0;
			}
//		****************** HVDC RAMP LOW ******************** 
            
//		****************** HVDC RAMP UP ********************
			if (analog.dc.sample < mppt.up_sample)
            {
                timer.mppt_adc_up = 0;  // RESET HVDC RAMP UP
            }
//		****************** HVDC RAMP UP ********************
           
		}   // HVDC and AC-GRID
//*******************************************************************************        

        if (analog.ac.cyc.sample0 < grid_sample)
        {
            analog.ac.cyc.sample0 = grid_sample;
        }
        
//	******* CRITICAL SURGE DETECTION ******* 
        
		if (grid_sample >= analog.ac_fault.sample)
		{
            PWM_SINE_DUTY = 0;
            PWM_BUCK_LATCH_OFF();
            HAL_ISR_TMR4_PWM_SINE_IE=0;

			if (fault.alarm.grid_high==0)
			{
                HAL_Fault_Set(ALARM_DISC|ALARM_AC_GRID_HIGH,FC_ACV_HI,grid_sample,0);
                APP_Stop_Mode();    // 2024/01/05 DGA extra measure to make sure buck drive is disabled 
			}
		}

//	******* CRITICAL SURGE DETECTION ******* 			

		
		#if defined(Uses_Diagnostics)
//	******* DIAGNOSTIC UFB FAULT DETECTION ******* 
		#define		DIAG_MODE_ACV_FAULT_LEVEL	10
		if (APP_State_Get()==INVERTER_DIAG_MODE) 
		{
			#if defined(Uses_Diagnostics)
//			diag.acv_adc = grid_sample;
			#endif
			if (grid_sample >= DIAG_MODE_ACV_FAULT_LEVEL)
			{
				APP_Stop_Mode();
			}
		}
//	******* DIAGNOSTIC UFB FAULT DETECTION ******* 		
		#endif
		
		if (analog.bit.temp_start)
		{
			analog.bit.temp_start=0;
			AD1CON1bits.ADON = 0;			// TURN Off 
			AD1CSSLbits.CSSL_TEMP = 1;		// Enable TEMP scans
			AD1CSSLbits.CSSL_DC_LINK = 0;	// Disable DCLINK scans
			AD1CON1bits.ADON = 1;			// TURN ON 
//			analog.sample_cnt=0;
		}

        
    //  ********************************************************************
    //  ********** 2023-09-12 LOOK FOR PHASE GRID VOLTAGE FAULTS **********         
    //  USE ANALOG TO LOOK FOR BINARY PHASE CHANGES AND THEN PERFORM TIMER 
    //  BOUNDS CHECKING 
    //  ********************************************************************        

    #if defined(DOC)
    POS:125,288,A:209 NEG:119,331,A:214,12502
    #endif
    
        if (grid_sample)
        {
            
            if (phase.bit.trig_positive0 == 0)
            {
                
                phase.bit.trig_positive0=1;      // Trigger start of positive0 - this gets reset EVERY other zero crossing, 50/60HZ at the end of the negative phase. 
                phase.bit.trig_positive1=1;      // Trigger start of positive1 - this gets reset EVERY zero crossing.  trig_positive1 is used to activate the buck
                phase.bit.trig_alternate=1;      // Trigger alternate of H Bridge 

                
                if ((GEO_freq.bit.lock) && (!fault.alarm.phase))
                {
                    


//  ********** Compiler error *********
//  Error: Invalid operands specified ('mov.b _phase+16,w0'). 
//                    if (phase.dir.flag) // Is phase backwards? 
//                    {
//                        #if defined (Uses_Fault_Phase_REV_POS)
//                        HAL_Fault_Set(ALARM_DISC|ALARM_PHASE,FC_TRIG_REV_POS,TMR1,1);
//                        #endif
//                    }
//  ********** Compiler error *********                    
                    
                    
                    #if defined (Uses_Fault_Phase_LO_POS)
                    // Check TRIG POS TIMER min, this checks the ZC pulse width 
                    else if (TMR1  < GEO_Nvs.freq.limit.trig.pos.lower) 
                    {
                        HAL_Fault_Set(ALARM_DISC|ALARM_PHASE,FC_TRIG_LO_POS,TMR1,GEO_Nvs.freq.limit.trig.pos.lower);
                    }
                    #endif

                    
                    #if defined (Uses_Fault_Phase_MAX_POS)
                    // Check TRIG POS TIMER max
                    else if (TMR1  > GEO_Nvs.freq.limit.trig.pos.upper) 
                    {
                        #if defined(Uses_DOC)
                        2023-10-05 50 HZ T1:30112 T2:500
                        #endif
                        
                        
                        HAL_Fault_Set(ALARM_DISC|ALARM_PHASE,FC_TRIG_MAX_POS, TMR1,GEO_Nvs.freq.limit.trig.pos.upper);
                    }
                    #endif
                    
                }
                
                
    
                #if defined(Uses_Debug_Phase)
                trace.phase.pos.cnt++;
                trace.phase.pos.tmr += timer_reg;

                if (trace.phase.pos.tmin==0)
                    trace.phase.pos.tmin = timer_reg;
                else if (trace.phase.pos.tmin > timer_reg)
                    trace.phase.pos.tmin = timer_reg;

                if (trace.phase.pos.tmax < timer_reg)
                    trace.phase.pos.tmax = timer_reg;
                #endif


            }
            
            else if (phase.bit.trig_positive1)

            {
                
                if ((GEO_freq.bit.lock) && (!fault.alarm.phase))
                {

                    #if defined(DOC)
                   GEO: 50HZ:490,15306,510,14705  60HZ:576,13020,612,12254 
                   lower.tmr1  50HZ:490,15306	60HZ:576,13020
                   upper.tmr1  50HZ,510,14705	60HZ:612,12254 
                   0-POS PHASE RANGE 00400-15000     
                   1-NEG PHASE RANGE 15400-30000
                    #endif

                     // FC_RANGE_POS =7
                   // 2023-09-27 50HZ  19351 15306
                   
                    if (TMR1  > GEO_Nvs.freq.limit.lower.tmr1)         // FC_RANGE_POS =7
                    {
                        // FC_RANGE_POS =7
                        if (fault.alarm.phase==0)
                        {
                            #if defined(Uses_Fault_Phase_RANGE_POS)
                            HAL_Fault_Set(ALARM_DISC|ALARM_PHASE,FC_RANGE_POS, TMR1,GEO_Nvs.freq.limit.lower.tmr1);
                            #endif
                        }
                    }

                   #if defined(Uses_Fault_Phase_TRIG_REV_POS)
                    else if (TMR1  < GEO_Nvs.freq.limit.upper.tmr1)         // FC_RANGE_POS =7
                    {
                        if (phase.dir.flag) // Is phase backwards? 
                        {
                            HAL_Fault_Set(ALARM_DISC|ALARM_PHASE,FC_TRIG_REV_POS,TMR1,2);
                        }
                    }
                   #endif
                   
                    #if defined(Uses_Debug_Phase)
                    if (trace.phase.pos.range_min==0)
                        trace.phase.pos.range_min = timer_reg;
                    else if (trace.phase.pos.range_min > timer_reg)
                        trace.phase.pos.range_min = timer_reg;
                    if (trace.phase.pos.range_max < timer_reg)
                        trace.phase.pos.range_max = timer_reg;
                    #endif
                   
                }
            }   
            
            

        }
    
//      else if (grid_sample)    
//      else if (IO_PIN_GRID_ZC_PORT==0)
        else if (TMR1 > TIMER1_INT1_FILTER)
        {
            if (phase.bit.trig_negative0)
            {
                #if defined(Uses_Fault_Phase_RANGE_NEG)
                if (GEO_freq.bit.lock)
                {
                    Uint16 timer_tmp;
                    // Negative phase limit must be added to last zero cross timer reading 
                    timer_tmp = timer.tmr1_reg+GEO_Nvs.freq.limit.lower.tmr1;     // Add zero cross TMR1 reading 
                    
                    //  FC_RANGE_NEG =8
                    // Check TIMER max range against lower freq limit which is timer max range
                    if (TMR1 > GEO_Nvs.freq.limit.lower.tmr2) 
                    {
                        if (fault.alarm.phase==0)
                            HAL_Fault_Set(ALARM_DISC|ALARM_PHASE,FC_RANGE_NEG,TMR1,GEO_Nvs.freq.limit.lower.tmr2);
                    }

                }
                #endif
                
                #if defined(Uses_Debug_Phase)
                if (phase.dir.flag)
                {
                    if (trace.phase.neg.range_min==0)
                        trace.phase.neg.range_min = timer_reg;
                    else if (trace.phase.neg.range_min > timer_reg)
                        trace.phase.neg.range_min = timer_reg;
                   if (trace.phase.neg.range_max < timer_reg)
                        trace.phase.neg.range_max = timer_reg;
                }
                #endif
            }
        
        }
   
    
    
    
   
        #if defined(Uses_Debug_ADC_PWM)         
    
        debug_adc_grid_sample = grid_sample;
        debug_adc_grid_time = TMR1;
        
        if ((phase.dir.flag) && (grid_sample==0))
        {
            trace.analog.ac.adc0.cnt++;
            if ( (TMR1 < trace.analog.ac.adc0.tmr_min) || (trace.analog.ac.adc0.tmr_min==0))
                trace.analog.ac.adc0.tmr_min = TMR1;
            if (trace.analog.ac.adc0.tmr_max < TMR1)
                trace.analog.ac.adc0.tmr_max = TMR1;
        }
        if (grid_sample==1)
        {
            trace.analog.ac.adc1.cnt++;
            if ( (TMR1 < trace.analog.ac.adc1.tmr_min) || (trace.analog.ac.adc1.tmr_min==0))
                trace.analog.ac.adc1.tmr_min = TMR1;
            if (trace.analog.ac.adc1.tmr_max < TMR1)
                trace.analog.ac.adc1.tmr_max = TMR1;
        }
        else if (grid_sample==2)
        {
            trace.analog.ac.adc2.cnt++;
            if ( (TMR1 < trace.analog.ac.adc2.tmr_min) || (trace.analog.ac.adc2.tmr_min==0))
                trace.analog.ac.adc2.tmr_min = TMR1;
            if (trace.analog.ac.adc2.tmr_max < TMR1)
                trace.analog.ac.adc2.tmr_max = TMR1;
        }
        else
        {
            trace.analog.ac.adc3.cnt++;
            if ( (TMR1 < trace.analog.ac.adc3.tmr_min) || (trace.analog.ac.adc3.tmr_min==0))
                trace.analog.ac.adc3.tmr_min = TMR1;
            if (trace.analog.ac.adc3.tmr_max < TMR1)
                trace.analog.ac.adc3.tmr_max = TMR1;
        }
        #endif
        
        
	}
    
	AD1CON1bits.ASAM=1; // enable
	HAL_ISR_ADC1_IF = 0;
    

}

void ADC_ZC_ISR()
{
    if (phase.dir.flag==0)
        return;
    
    #if defined (Uses_Debug_ACV)
    trace.ac.zc++;
    #endif

    analog.ac.cyc.sample = analog.ac.cyc.sample0;
    analog.ac.cyc.sample0=0;
    
	analog.bit.grid_sample=1;	// Set flag 	
	analog.bit.temp_start=1;	// Trigger temperature reading
	analog.bit.grid_timeout=0;    
}
            
Uint16 adc_calc_acv(Uint16 sample)
{
    Uint16 acv;
    
    acv= ACV_CALC_VOLTS10X(sample);
    
    return acv;
}

void ADC_AC_Grid_Limits(Uint16 grid_volt)
{
	Uint16 acv_over_limit;
	Uint16 acv_under_limit;
	
	if (APP_State_Get() == INVERTER_RUN)
	{
		acv_over_limit = GEO_Limits.grid_run_over;
		acv_under_limit = GEO_Limits.grid_run_under;
        
//      #if defined(Uses_Debug_ACV)
//      acv_over_limit = 275;;
//      #endif
        
	}
	else
	{
		acv_over_limit = GEO_Limits.grid_start_over;
		acv_under_limit = GEO_Limits.grid_start_under;
	}
	
	if (grid_volt >= acv_over_limit)	// AC GRID HIGH ++++++
	{                
		HAL_Fault_Clear(ALARM_AC_GRID_LOW);

		if (fault.alarm.grid_high==0)
		{
            
			#if defined(Uses_Debug_ACV)
			printf("AC+HI %uv %ua ",grid_volt, analog.ac.cyc.sample);
			printf("AVG:%uv,%ua ", analog.ac.avg.volts, analog.ac.avg.sample);
			printf("Limit:%u \n", acv_over_limit);
			#endif

//			HAL_Fault_Set(ALARM_DISC|ALARM_AC_GRID_HIGH,FC_ACV_HI,grid_volt,acv_over_limit);
		}
	}
	else
	{
		if (grid_volt <= acv_under_limit)	// AC GRID LOW -------
		{                
			HAL_Fault_Clear(ALARM_AC_GRID_HIGH);
            
            if (fault.alarm.grid_low==0)
            {
                HAL_Fault_Set(ALARM_DISC|ALARM_AC_GRID_LOW,FC_ACV_LO,grid_volt,acv_under_limit);
                
			#if defined(Uses_Debug)
			printf("*** AC-LOW *** ACV:%u,%u LIMIT:%u\n", grid_volt, grid_volt, acv_under_limit);
            #endif
                
            }
		}
		else	// AC GRID OK 
		{
			#if defined(Uses_Debug_Fault)
			if (fault.alarm.grid_high)
			{
				console_print_alarm_bits(fault.alarm);
				printf("CLEAR AC+ %s,%d",__FILE__,__LINE__);
				console_print_alarm_bits(fault.alarm);
				printf("\n");
			}
			#endif
			
			HAL_Fault_Clear(ALARM_AC_GRID_LOW|ALARM_AC_GRID_HIGH|ALARM_SURGE|ALARM_DELAY);
		}
	}
	
}


void ADC_AC_Calc()
{
    #if defined(Uses_Debug_ACV)
	trace.ac.sample_cnt++;
    #endif

    analog.bit.grid_sample=0;           // Reset flag 

    analog.ac.cyc.volts = ACV_CALC_VOLTS10X(analog.ac.cyc.sample);
    analog.ac.avg.sum += analog.ac.cyc.sample;
    if (++analog.ac.avg.cnt >= 50)
    {
        analog.ac.avg.sample = (analog.ac.avg.sum /analog.ac.avg.cnt) ;
        analog.ac.avg.sum=0;
        analog.ac.avg.cnt=0;
        analog.ac.avg.volts = ACV_CALC_VOLTS10X(analog.ac.avg.sample) / 10;
        ADC_AC_Grid_Limits(analog.ac.avg.volts);
    }
   
}

void ADC_Temperature_Update()
{
	analog.bit.temp_sample = 0;					// Reset flag 
	Temperature_Sample(analog.temp.sample);		// Calc temperature from ADC sample
  
    analog.ac_fault.sample = ACV_CALC_ADC(ACV_ADC_VOLT_FAULT);
//  analog.ac_fault.sample = ACV_CALC_ADC35(ACV_ADC_VOLT_FAULT);
    
    analog.ac_fault.volts = ACV_CALC_VOLTS(analog.ac_fault.sample);
}


void ADC_AC_Timeout()
{
	#if defined(Uses_Debug_ACV)
	trace.ac.timeout++;
	#endif	
	
	memset((void*)&analog.ac,0,sizeof(analog.ac));	// Reset / clear all grid related variables
	analog.bit.temp_start=1;	// Trigger temperature reading
	analog.bit.grid_timer=0;
	analog.bit.grid_timeout=1;	// Set bit to remember 

    PWM_BUCK_LATCH_OFF();  // Kill buck
    PWM_SINE_DUTY=0;
    
    if (fault.alarm.grid_low==0)
    {
        fault.alarm.grid_high=0;
        HAL_Fault_Set(ALARM_DISC|ALARM_AC_GRID_LOW,FC_ACV_LO,0,0);
        #if defined(Uses_Debug_ACV)
        printf("FAULT ACV TIMEOUT %s,%d\n",__FILE__,__LINE__);
        #endif
        
    }
    
	APP_AC_Freq_Reset();
}


Uint16 ADC_PV10X_Calc_DCL(Uint16 pv)
{
	return PV10X_CALC_DCLINK(pv) + 1;
}

Uint16 ADC_DCL_Calc_PV10X(Uint16 dcl)
{
	return DCLINK_CALC_PVX10(dcl) + 1;
}

void ADC_Calc_DC_Instant()
{
	analog.dc.voltage = DCLINK_GET_VDC(analog.dc.sample);
	
	if (pwm.buck.enable)
		analog.pvdc.volts10x = DCLINK_CALC_PVX10(analog.dc.voltage);
	else
		analog.pvdc.volts10x = DCLINK_CALC_PVX10_IDLE(analog.dc.voltage);
	
	analog.pvdc.volts.dec = (analog.pvdc.volts10x % 10);
	analog.pvdc.volts.num = analog.pvdc.volts10x / 10;

}

void ADC_DC_Calc()
{
	ADC_Calc_DC_Instant();
    
    Uint32 avg;
    
//  	HAL_ISR_ADC1_IE = 0;		// DGA 2023-09-01 ADC ISR DISABLE

    analog.pwm.avg10.sum += analog.pwm.avg.sample;
    analog.pwm.avg10.cnt++;
    if (analog.pwm.avg10.cnt >= 10)
    {
		avg = (analog.pwm.avg10.sum / analog.pwm.avg10.cnt);
		analog.pwm.avg10.sample = avg;
        analog.pwm.avg10.volts = DCLINK_GET_VDC(avg);
        analog.pwm.avg10.sum = 0;
        analog.pwm.avg10.cnt = 0;
    }
    
	if (analog.dc.avg.sum)
	{
		avg = (analog.dc.avg.sum / analog.dc.avg.cnt);
		analog.dc.avg.sample = avg;
        analog.dc.avg.volts = DCLINK_GET_VDC(avg);
        analog.dc.avg.cnt=0;
        analog.dc.avg.sum=0;
	}
	else
	{
		analog.dc.avg.sample = 0;
        analog.dc.avg.volts = 0;
	}
    
    analog.dc.avg10.sum += analog.dc.avg.sample;
    analog.dc.avg10.cnt++;
    if (analog.dc.avg10.cnt >= 10)
    {
		avg = (analog.dc.avg10.sum / analog.dc.avg10.cnt);
		analog.dc.avg10.sample = avg;
        analog.dc.avg10.volts = DCLINK_GET_VDC(avg);
        analog.dc.avg10.sum = 0;
        analog.dc.avg10.cnt = 0;
        
    }
	
	analog.bit.dclink_calc_avg=0;
    
    
    #define RIPPLE_AVG_CYCLES       50      // 20
    
    analog.dc.ripple.dif.avg.sum += analog.dc.ripple.dif.sample;
    if (++analog.dc.ripple.dif.avg.cnt >= RIPPLE_AVG_CYCLES)
    {
        analog.dc.ripple.dif.avg.sample = analog.dc.ripple.dif.avg.sum/analog.dc.ripple.dif.avg.cnt;
        analog.dc.ripple.dif.avg.volts = DCLINK_GET_VDC(analog.dc.ripple.dif.avg.sample);
        analog.dc.ripple.dif.avg.sum=0;
        analog.dc.ripple.dif.avg.cnt=0;
    }
    
    analog.dc.ripple.avg.sum += analog.dc.ripple.avg.sample;
    if (++analog.dc.ripple.avg.cnt >= RIPPLE_AVG_CYCLES)
    {
		avg = (analog.dc.ripple.avg.sum / analog.dc.ripple.avg.cnt);
        analog.dc.ripple.avg.sample = avg;
        analog.dc.ripple.avg.volts = DCLINK_GET_VDC(avg);
        analog.dc.ripple.avg.sum=0;
        analog.dc.ripple.avg.cnt = 0;
    }
    
    
//		****************** HVDC PEAK ********************
    analog.dc.ripple.hi.avg.sum += analog.dc.ripple.hi.pwm_sample;
    if (++analog.dc.ripple.hi.avg.cnt >= RIPPLE_AVG_CYCLES)
    {
		avg = (analog.dc.ripple.hi.avg.sum / analog.dc.ripple.hi.avg.cnt);
        analog.dc.ripple.hi.avg.sample = avg;
        analog.dc.ripple.hi.avg.volts = DCLINK_GET_VDC(avg);
        analog.dc.ripple.hi.avg.sum=0;
        analog.dc.ripple.hi.avg.cnt = 0;
    }
//		****************** HVDC PEAK ********************            
    
//		****************** HVDC DIP ********************
    analog.dc.ripple.lo.avg.sum += analog.dc.ripple.lo.pwm_sample;
    if (++analog.dc.ripple.lo.avg.cnt >= RIPPLE_AVG_CYCLES)
    {
		avg = (analog.dc.ripple.lo.avg.sum / analog.dc.ripple.lo.avg.cnt);
        analog.dc.ripple.lo.avg.sample = avg;
        analog.dc.ripple.lo.avg.volts = DCLINK_GET_VDC(avg);
        analog.dc.ripple.lo.avg.sum=0;
        analog.dc.ripple.lo.avg.cnt = 0;
    }

//		****************** HVDC DIP ********************


//    HAL_ISR_ADC1_IE = 1;		// DGA 2023-09-01 ADC ISR enable
    
	
	#if defined(Uses_DCV_Test_Mode)
	analog.dc.sample_avg1 = DCLINK_GET_ADC(Uses_DCV_Test_Mode);
	analog.dc.voltage = Uses_DCV_Test_Mode;
	#endif

	APP_DC_Link_Limits(analog.dc.voltage);
		
	mppt.bit.dclink_sample=1;	// DGA set flag 
    
    timer.dclink=0; // reset watchdog timer 

    #if defined(Uses_Debug_DCV)
	trace.dc.sample_cnt++;
    #endif
    
	
}


void ADC_Task()
{
	if (analog.bit.dclink_calc_avg)	// Is is time to make DC calculations ?
	{
		ADC_DC_Calc();			// Calculate AC 
	}
			
	if (analog.bit.grid_sample)	
	{
		ADC_AC_Calc();			// Calculate AC 
	}
	
//	******* AC GRID TIMEOUT **********************************************
	if (analog.bit.grid_timer)
    {
		ADC_AC_Timeout();
	}	
//	******* AC GRID TIMEOUT **********************************************	
	
//	******* Temperature **********************************************	
	if (analog.bit.temp_timer)		//	DGA 2022-03-26 - 500 ms interval 	
	{
		analog.bit.temp_timer=0;	//	DGA 2022-03-26 - Reset 500 ms interval 	
		if (analog.bit.temp_sample)
		{
			ADC_Temperature_Update();
		}
	}
//	******* Temperature **********************************************		
	
}
