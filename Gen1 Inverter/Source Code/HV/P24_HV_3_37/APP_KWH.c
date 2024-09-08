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

		#if defined(Uses_Debug) && defined(Uses_Debug_KWH)
		#define Uses_KWH_Simulate_60k	
		#endif

void KWH_EEPROM_Read()
{
	// DGA Has power in KWH been stored previously?
	
	if (NVS_Read_Word(NVS_KWH_WT, &kwh.lifeTimeW, NVS_VALID_DEFAULT))
	{
		#if defined(Uses_Debug_KWH)
		printf("WH CHECK-SUM-ERROR");
		print_source();
		#endif

		NVS_Write_Word(NVS_KWH_WT,0,NVS_VALID_DEFAULT);
		kwh.lifeTimeW = 0;
	}
	
	if (NVS_Read_Word(NVS_KWH_KW,(Uint16 *) &kwh.lifeTimeK,NVS_VALID_DEFAULT))
	{
		#if defined(Uses_Debug_KWH)
		printf("KW CHECK-SUM-ERROR");
		print_source();
		#endif

		NVS_Write_Word(NVS_KWH_KW,0,NVS_VALID_DEFAULT);
		kwh.lifeTimeK = 0;
	}
}

void KWH_Init()
{
	kwh.watt_sec_sum = 0;
	kwh.watt_mil_sum = 0;
	kwh.watt_mil_cnt = 0;
	kwh.watt_sec_cnt = 0;
	kwh.watt_hours = 0;
	KWH_EEPROM_Read();
}


	#define	KWH_UPDATE_SECS				60

void KWH_Calc_Seconds()
{
	Uint16 update_seconds;
	
//	watt_mil_sum = kwh.watt_mil_sum;
	
	kwh.watt_sec_cnt++;
	kwh.watt_sec_sum += kwh.watt_mil_sum;
	kwh.watt_mil_sum = 0;
	kwh.watt_mil_cnt = 0;

	
//	1000 * 60 = 60,000  *60=3,600,000 
//	0100 * 60 = 6,000  *60=360,000 	
	
	update_seconds = KWH_UPDATE_SECS; // DGA Default time
	
	kwh.watt_hours = kwh.watt_sec_sum / KWH_WATT_SECONDS_DIVISOR;

	#if defined(Uses_Debug) && defined(Uses_Debug_KWH)
	if (dcb2.kwh)
	{
		update_seconds = 10;
		printf("KWH %u WH:%u Life:%lu %u.%u %u SUM:%lu",kwh.watt_sec_cnt, kwh.watt_hours, KWH_Get_Lifetime(), kwh.lifeTimeK, kwh.lifeTimeW,
				pwm.watts,
				kwh.watt_sec_sum);
		print_source();
	}
	#endif
	
	if (kwh.watt_sec_cnt >= update_seconds)
	{
		if (kwh.watt_hours)
		{
			kwh.lifeTimeW += kwh.watt_hours;
			kwh.watt_hours=0;	// Reset 
			
			if (kwh.lifeTimeW >= 1000)
			{
				// Normalize power...
				Uint8 kw = (kwh.lifeTimeW / 1000);
				kwh.lifeTimeK += kw;
				kwh.lifeTimeW = kwh.lifeTimeW % 1000;
 				NVS_Write_Word(NVS_KWH_KW,kwh.lifeTimeK, NVS_VALID_DEFAULT);
			}

			NVS_Write_Word(NVS_KWH_WT,kwh.lifeTimeW,NVS_VALID_DEFAULT);
			
			#if defined(Uses_Debug) && defined(Uses_Debug_KWH)
			if (dcb2.kwh)
			{
				printf("UPDATE:%lu %u.%u ",KWH_Get_Lifetime(), kwh.lifeTimeK, kwh.lifeTimeW);
				print_source();
				KWH_EEPROM_Read();
				printf("VERIFY:%lu %u.%u ",KWH_Get_Lifetime(), kwh.lifeTimeK, kwh.lifeTimeW);
				print_source();		
			}
			#endif
		}
		kwh.watt_sec_cnt=0;	// Reset 
		kwh.watt_sec_sum=0;	// Reset 
	}
}

void KWH_Task()
{
	//	2022-02-22 DGA this gets called 60 times a sec
	if (timer.bit.kwh)	//	this bit flag set 60 times a sec
	{
		timer.bit.kwh=0;

		#if defined(Uses_Debug_KWH) && defined(Uses_KWH_Simulate_60k)
		if (dcb2.kwh)
			kwh.watt_mil_sum += 60000;
		else
		{
			kwh.watt_mil_sum += pwm.watts;
		}
		#else
		kwh.watt_mil_sum += pwm.watts;		
		#endif
		
		kwh.watt_mil_cnt++;
		if (kwh.watt_mil_cnt >= 60)
		{
			KWH_Calc_Seconds();
		}
	}
}

void KWH_Clear()
{
	kwh.watt_sec_sum = 0;
	kwh.watt_mil_sum = 0;
	kwh.watt_mil_cnt = 0;
	kwh.watt_sec_cnt = 0;
	kwh.watt_hours=0;
	kwh.lifeTimeK = 0;
	kwh.lifeTimeW = 0;
	NVS_Write_Word(0, NVS_KWH_WT,NVS_VALID_DEFAULT);
	NVS_Write_Word(0, NVS_KWH_KW,NVS_VALID_DEFAULT);
}	

Uint32 KWH_Get_Lifetime()
{
	return ( kwh.lifeTimeW + (1000 * kwh.lifeTimeK) );
}	

#endif