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



#if defined(OLD_CODE)

void console_print_buck_pwm1()
{
	if (APP_State_Get() != INVERTER_RUN)
		return;
//	printf("ZC:%u ",trace.cnt_zc);	
	printf("PR1:%u OC1:%u,%u%c ", PRX_SINE, pwm.avg.ocx1.reg, pwm.avg.ocx1.num,'%');
//	printf("PR2:%u,%u OC2:%u,%u%c", pwm.avg.prx2.reg, pwm.avg.prx2.num, pwm.avg.ocx2.reg, pwm.avg.ocx2.num,'%');
	#if defined(Uses_Clock_Lock)
	if (dcb.pwm.lock_clock_duty)
		printf(" DTY:%u,%u ",dcb.pwm.lock_clock_duty, PWM_CALC_OCX_DUTY(pwm.clock.prx,dcb.pwm.lock_clock_duty) );
	#endif
	printf("\n");
    
}

void console_print_buck_pwm1()
{
	if (APP_State_Get() != INVERTER_RUN)
		return;
//	printf("ZC:%u ",trace.cnt_zc);	
	printf("PR1:%u OC1:%u,%u%c ", PRX_SINE, pwm.avg.ocx1.reg, pwm.avg.ocx1.num,'%');
//	printf("PR2:%u,%u OC2:%u,%u%c", pwm.avg.prx2.reg, pwm.avg.prx2.num, pwm.avg.ocx2.reg, pwm.avg.ocx2.num,'%');
	#if defined(Uses_Clock_Lock)
	if (dcb.pwm.lock_clock_duty)
		printf(" DTY:%u,%u ",dcb.pwm.lock_clock_duty, PWM_CALC_OCX_DUTY(pwm.clock.prx,dcb.pwm.lock_clock_duty) );
	#endif
	printf("\n");
}
void console_print_buck_pwm2()
{
	Uint16 pwm_cnt = pwm.avg.cnt_rpt/1000;

	if ((APP_State_Get() != INVERTER_CONNECT) && (APP_State_Get() != INVERTER_RUN))
		return;
	
	printf("%uk ",pwm_cnt);
	
	if (IO_PIN_BUCK_PORT==0)
		printf("IO:%d ",IO_PIN_BUCK_PORT);
	
	if (! pwm.buck.enable)
	{
		printf("EN:%d ",pwm.buck.enable);
	}
    
	if (pwm.buck.count==0)
	{
		printf("BC:%d ",pwm.buck.count);
	}
	
//	if (ac.gGridInRange==0)
//	{	
//		printf("GR:%u ",ac.gGridInRange);
//	}
	
	if ((IO_PIN_BUCK_LATCH) && (HAL_H_BRIDGE_Get()==H_BRIDGE_DISABLED) )	
	{
		printf("*** WARNING **** ");
	}
	
//	printf("HB:%u,%u,%u ",trace.ufb_pos,trace.ufb_neg, HAL_H_BRIDGE_Get() );

	printf("TM:%u ",timer.buck_sec);
	
//	printf("PR:%u,%u OC:%u,%u ",PRX_CLOCK, pwm.avg.prx.reg, OCX_CLOCK, pwm.avg.ocx.reg);
	printf("PR:%u,%u OC:%u,%u ",PRX_CLOCK, pwm.avg.prx2.reg, OCX_CLOCK, pwm.avg.ocx2.reg);	
	
	printf("AC:%u ",analog.grid.avg_voltage);
	printf("DC:%u \n",analog.dc.voltage_avg1);	


}
#endif



#if defined(Uses_Inverter) && defined(Uses_AMP_Lock)


void PWM_Console_Amp_Lock_Set(Uint16 amp)
{
	if (amp==0)
	{
		dcb.pwm.amp_lock=0;
		printf("%s %s\n","AMP", "OFF");
	}
	else
	{
		dcb.pwm.amp_lock=amp;
		printf("%s=%u\n","AMP",dcb.pwm.amp_lock);
	}
}


/*
void PWM_Clock_Duty_Set(Uint8 duty)
{
	if (duty==0)
	{
		dcb.pwm.amp_lock=0;
		printf("%s %s\n","AMP", "OFF");
	}
	else
	{
		dcb.pwm.amp_lock=amp;
		printf("%s=%u\n","AMP",dcb.pwm.amp_lock);
	}
}
*/

void PWM_Power_Limit_UP(Uint16 x)
{
	if (dcb.pwm.amp_lock==0)
	{
		tune.nvs_power_limit += x;
		printf("POWER UP:%u \n",tune.nvs_power_limit);
//		PWM_Amplitude_Limit_Calc();
	}
	else
	{
		PWM_Console_Amp_Lock_Set(dcb.pwm.amp_lock+x);
	}
}

void PWM_Power_Limit_DN(Uint16 x)
{
	if (dcb.pwm.amp_lock==0)
	{
		tune.nvs_power_limit -= x;
		printf("POWER DN:%u \n",tune.nvs_power_limit);
//		PWM_Amplitude_Limit_Calc();	
	}
	else
	{
		PWM_Console_Amp_Lock_Set(dcb.pwm.amp_lock-x);
	}
		
}

void PWM_Mode(Uint16 x)
{
//	printf("PWM_Mode%u\n",x);	
	
	#if defined( Uses_AMP_Lock)
	if (dcb.pwm.amp_lock)
	{
		dcb.pwm.amp_lock = 0;
		printf("AMP Lock OFF\n");	
	}
	else
	{
		dcb.pwm.amp_lock = 1;
		if (dcb.pwm.amp_lock < 1)
			dcb.pwm.amp_lock = 1;
		printf("AMP LOCK:%u ON\n",dcb.pwm.amp_lock);	
	}
	#endif
	
	#if defined(Uses_Clock_Lock)
	if (dcb.pwm.lock_clock_freq==0)
	{
		dcb.pwm.lock_clock_freq++;

		PWM_Print_Clock_Freq("CLOCK=LOCK");
	}
	else
	{
		dcb.pwm.lock_clock_freq=0;
		printf("CLOCK=AUTO\n");
	}
	#endif
}


void PWM_Test1(Uint16 x)
{
	#if defined(Uses_Clock_Lock) || defined(Uses_AMP_Lock)
	switch(x)
	{
		#if defined(Uses_Clock_Lock)
		case 2:
		{
			if (dcb.pwm.lock_clock_duty==0)
			{
				dcb.pwm.lock_clock_duty=50;
				printf("DUTY=LOCK\n");
			}
			else
			{
				dcb.pwm.lock_clock_duty=0;
				printf("DUTY=AUTO\n");
			}
			break;
		}
		#endif
		#if defined(Uses_AMP_Lock)
		case 3:
		{
			PWM_Power_Limit_UP(1);
			break;
		}
		case 4:
		{
			PWM_Power_Limit_DN(1);
			break;
		}
		case 5:
		{
			PWM_Power_Limit_UP(10);
			break;
		}
		case 6:
		{
			PWM_Power_Limit_DN(10);
			break;
		}

		case 7:
		{
			PWM_Power_Limit_UP(50);
			break;
		}

		case 8:
		{
			PWM_Power_Limit_DN(50);	
			break;
		}
		#endif

		default:
		{
			
		}
	}
	#endif

//	printf("TEST ARG:%u \n",x);		

}

#endif


#if defined(Uses_Inverter) && defined(Uses_Debug_PWM)

void PWM_Print_Clock_Freq(char *str)
{
	if (str)
		printf("%s ",str);
	printf("FREQ:%uk PR2:%u\n",PWM_CALC_PRX_KHZ(dcb.pwm.prx),dcb.pwm.prx);
//	printf("%s FREQ:%lu [ %uk ] PR2:%u\n",str, HAL_Calc_PRX_Freq_Mhz(dcb.pwm.prx),PWM_CALC_PRX_KHZ(dcb.pwm.prx),dcb.pwm.prx);
}

#if defined(Uses_Clock_Lock)
#define	str_range	"%s range %u to %u \n"
void PWM_Console_Clock_Freq_Set(Uint16 frq_khz)
{
	if (frq_khz==0)
	{
		dcb.pwm.lock_clock_freq=0;
		printf("%s %s\n","FREQ", "OFF");
	}
	else
	{
		Uint16 prx;
		prx = PWM_CALC_PRX_KHZ(frq_khz);

		if ( (prx < PWM2_PRX_LOWER_LIMIT) || (prx > PWM2_PRX_UPPER_LIMIT))
		{
			printf(str_range,"FREQ", PWM_CALC_PRX_KHZ(PWM2_PRX_LOWER_LIMIT), PWM_CALC_PRX_KHZ(PWM2_PRX_UPPER_LIMIT));
		}
		else
		{
			dcb.pwm.prx = prx;
			PWM_Print_Clock_Freq(0);
			dcb.pwm.lock_clock_freq=1;
			
		}
	}
}
void PWM_Console_Clock_Duty_Set(Uint8 duty)
{
	if (duty==0)
	{
		dcb.pwm.lock_clock_duty=0;
		printf("%s %s\n","DUTY", "OFF");
	}
	else
	{
		if ( (duty < 1) || (duty > 99))
		{
			printf(str_range,"DUTY", 1,99);
		}
		else
		{
			dcb.pwm.lock_clock_duty=duty;
			
//			printf("CALC %u,%u,%u",PWM_CALC_OCX_DUTY(1,100),PWM_CALC_OCX_DUTY(50,100),PWM_CALC_OCX_DUTY(99,100) );
//			printf("  %u,%u,%u\n",PWM_CALC_OCX_DUTY(2,50),PWM_CALC_OCX_DUTY(50,50),PWM_CALC_OCX_DUTY(99,50) );
			
			printf("%s=%u\n","DUTY",dcb.pwm.lock_clock_duty);
		}
	}
}


#endif

void console_print_buck_pwm1()
{
//	printf("PRX_SINE:%u  MAX:%u\n",PRX_SINE, PRX_SINE-1);
  
    if (fault.alarm.value)
        return;
    
//    pwm.avg.ocx1.sum
//    pwm.avg.ocx1.num
        
    
    if (dcb.pwm.amp_lock)
    {
//        Uint16 duty10x;
//        duty10x = PWM_GET_OCX_DUTY10X(PRX_SINE, 127);
          printf("PWM AMP:%u OCX:%u\n",dcb.pwm.amp_lock,pwm.avg.ocx1.num);
        
    }
    
    
    
}
void console_print_buck_pwm2()
{
    printf("debug pwm 2\n");
}

#endif

