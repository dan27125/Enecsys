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


//	#define	Uses_Diag_UFB_Fast

#if defined( Uses_Console ) && defined(Uses_Diagnostics)


#if defined(DOC)
//		else if ( strcmp((char*)"OFF", (char*)arg2)==0)
//		{
//			APP_Stop_Mode();
//			ecb1.diag_ufb = 0;
//			diag.ufb_sec = 0;
//			diag.bit.diag_120_hz=0;
//			ecb1.run = 1;
//			NVS_RCB_Write();
//			printf("%s %s \n",arg1, arg2 );
//		}
		
//		else if ( str_compare("1EP", arg2)==0)
//		{
//			INTCON2bits.INT1EP = !INTCON2bits.INT1EP;
//			printf("%s=%d\n",arg2,INTCON2bits.INT1EP);
//		}
		
		
		else if ( strcmp((char*)"BUCK", (char*)arg2)==0)
		{
			if (ecb1.run)
			{
				ecb1.run = 0;
				printf("STOP OK\n");
			}
			APP_Stop_Mode();	// DGA WIFI ZIGBEE cmd
			printf("BUCK [%s] %d \n",arg3, csib.value[2] );

			int value = csib.value[2];
			
			if (value==0)
			{
				ecb1.diag_buck = 0;
				pcb1.diag_buck_sec = 0;
				printf("%s %s OFF\n",arg1,arg2);
			}
			else
			{
				ecb1.diag_buck = 1;
				pcb1.diag_buck_sec = value;
				pcb1.diag_buck_timer = 0;
				printf("%s %s TMR=%d\n",arg1,arg2, value);
//				NVS_RCB_Write();
			}
			NVS_RCB_Write();

#endif

Uint8 console_parse_args_diagnostics()
{
	if ( strcmp((char*)"DIAG", (char*)arg1)==0)
	{
		if ( strcmp((char*)"UFB", (char*)arg2)==0)
		{
			diag.bit.diag_120_hz=0;
			ecb1.run = 0;
//			fault.alarm.diagnostic=1;
//			ecb1.diag_ufb = 1;
			diag.ufb_sec = 2;
			diag.ufb_timer = 0;
			ecb1.diag_ufb=1;
			NVS_Diag_Write();
			APP_Stop_Mode();
			printf("%s %s (%d) OK\n",arg1, arg2,diag.ufb_sec);
		}
		else if (diag.ufb_sec)
		{
			APP_Inverter_Diag_Mode_Stop();
			APP_Stop_Mode();
		}
	}
	
	else
		return 0;
	
	return 1;
}

void APP_Inverter_Diag_Mode_Stop()
{
	if ((diag.ufb_sec) || (diag.buck_sec) || (ecb1.diag_ufb) || (APP_State_Get()==INVERTER_DIAG_MODE))
	{
		diag.ufb_sec = 0;
		diag.buck_sec = 0;	
		diag.ufb_timer = 0;
		ecb1.diag_ufb=0;
		NVS_Diag_Write();
	}
	diag.bit.fault_stop=0;
}

void APP_Inverter_Diag_Mode_Start()
{
	APP_State_Set(INVERTER_DIAG_MODE);
	fault.alarm.diagnostic = 1;
	HAL_ISR0_AC_ZERO_CROSS_IF=0;
	HAL_ISR0_AC_ZERO_CROSS_IE=1;

	HAL_ISR1_AC_PHASE_IF = 0;
	HAL_ISR1_AC_PHASE_IE = 1;
	
	PWM_SINE_DUTY = 0;
	OC1R = 0;
	OC3R = 0;
	PWM_BUCK_LATCH_OFF();					// Disable BUCK enable
	HAL_ISR_TMR4_PWM_SINE_IE = 0;			// Disable PWM ISR enable	

	diag.state=0;
	diag.timer=0;
	
//	printf("Diag_Mode_Start UFB:%u\n",diag.ufb_sec);
}

//	#define	ASCII_DIAG_UFB		"DIAGNOSTIC UFB"
	#define	ASCII_DIAG_UFB		"DIAG UFB"
	#define	ASCII_UFB_LO		"LO - NEUTRAL Q5/Q6"
	#define	ASCII_UFB_HI		"HI + LIVE +  Q7/Q8"
	#define	ASCII_UFB_AC		"AC   120HZ FAST   "


void APP_Inverter_Diag_UFB_Slow()
{
	diag.ufb_timer++;
	
	if (diag.ufb_timer >= diag.ufb_sec)
	{
		diag.ufb_timer = 0;
		
		if (diag.ufb)
		{
			HAL_H_BRIDGE_Set(H_BRIDGE_REVERSE_NEUTRAL);
			diag.ufb=0;
			printf("%s %d %s\n",ASCII_DIAG_UFB,diag.ufb_sec,ASCII_UFB_LO);
		}
		else
		{
			diag.ufb=1;
			HAL_H_BRIDGE_Set(H_BRIDGE_FORWARD_LIVE);
			printf("%s %d %s\n",ASCII_DIAG_UFB,diag.ufb_sec,ASCII_UFB_HI);
		}
	}
}

void APP_Inverter_Diag_Mode_Task_Sec()
{
//	printf("diag.acv_adc=%d fast=%d,%d\n",diag.acv_adc,diag.ufb_hi,diag.ufb_lo);
	
	if ((ecb1.diag_ufb) && (diag.state==DIAG_STATE_SLOW))
	{
		APP_Inverter_Diag_UFB_Slow();
	}
}

#if defined(Uses_Diag_UFB_Fast)
void APP_Inverter_Diag_Mode_Task_Fast()
{
	if (diag.state==DIAG_STATE_FAST)
	{
		diag.ufb_freq++;
		if (HAL_H_BRIDGE_Get()==H_BRIDGE_FORWARD_LIVE)
		{
//			HAL_H_BRIDGE_Set(H_BRIDGE_DISABLED);
			HAL_H_BRIDGE_Set(H_BRIDGE_REVERSE_NEUTRAL);
			diag.ufb_lo++;
		}
		else
		{
//			HAL_H_BRIDGE_Set(H_BRIDGE_DISABLED);
//			APP_delay_miliseconds(2);
			HAL_H_BRIDGE_Set(H_BRIDGE_FORWARD_LIVE);
			diag.ufb_hi++;
		}
	}
	
}
#endif

void APP_Inverter_Diag_Mode_Task()
{
	if ((ecb1.run) || (fault.alarm.diagnostic==0))
	{
		return;
	}

	if (rcb1.sec)
	{
		rcb1.sec=0;

		#if defined(Uses_Diag_UFB_Fast)
		if (diag.state==DIAG_STATE_SLOW)
		{
			if (++diag.timer >= 4)
			{
				diag.state=DIAG_STATE_FAST;
				diag.timer=0;
				printf("%ds %s %s\n",diag.ufb_sec,ASCII_DIAG_UFB,ASCII_UFB_AC);
			}
		}
		else if (diag.state==DIAG_STATE_FAST)
		{
			if (++diag.timer >= 6)
			{
				diag.state=DIAG_STATE_SLOW;
				diag.timer=0;
			}
		}
	#endif
		
		APP_Inverter_Diag_Mode_Task_Sec();
	}
	
	#if defined(Uses_Diag_UFB_Fast)
	if (timer.bit.diag_100_hz)
	{
		timer.bit.diag_100_hz=0;
		
		if (++diag.div >= 10)
		{
			diag.div=0;
			APP_Inverter_Diag_Mode_Task_Fast();
		}
	}
	#endif

//	if ((diag.isr_zc) || (diag.isr_phase))
//	{
//		printf("DIAG ZC:%u PHASE:%u\n",diag.isr_zc,diag.isr_phase);
//		diag.isr_zc=0;
//		diag.isr_phase=0;
//	}
	
}


/*
 
void diag_set_buck(char value)
{
	if (value)
	{
		IO_PIN_BUCK_ENABLE = 1;
	}
	else
	{
		IO_PIN_BUCK_ENABLE = 0;
	}
}
void process_seconds_diag_buck()
{
//	Verify we are in stop mode 	
	if ( ! hal_stop_mode() )
		return;

	if (ecb1.diag_buck == 0)
		return;
	
	if (igs_freq)
		return;

	pcb1.diag_buck_timer++;
	
	diag_set_buck(pcb1.diag_buck_state);
	
	if (pcb1.diag_buck_timer < pcb1.diag_buck_sec)
		return;
	pcb1.diag_buck_timer = 0;
	if (pcb1.diag_buck_state)
	{
		pcb1.diag_buck_state = 0;
		diag_set_buck(pcb1.diag_buck_state);
		printf("%s LOW %ds\n","BUCK DIAG",pcb1.diag_buck_sec);
	}
	else
	{
		pcb1.diag_buck_state = 1;
		diag_set_buck(pcb1.diag_buck_state);		
		printf("%s HIGH %ds\n","BUCK DIAG",pcb1.diag_buck_sec);
	}
 
}
*/

#endif	// #if defined( Uses_Console ) && defined(Uses_Diagnostics)

