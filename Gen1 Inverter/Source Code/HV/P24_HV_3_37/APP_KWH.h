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

#if !defined(  _KWH_H )
#define _KWH_H

	typedef struct 
	{
		Uint16	watt_mil_cnt;
		Uint32	watt_mil_sum;
		Uint16	watt_sec_cnt;
		Uint32	watt_sec_sum;
		Uint16	watt_hours;
		Uint16 lifeTimeW;
		Uint16 lifeTimeK;

	} Accumulated_Energy_Block;

	public Accumulated_Energy_Block kwh;
		
	#define	KWH_WATT_SECONDS_DIVISOR	((Uint32)216000)	// 3600 seconds per hr + 60 samples per sec
	
	void KWH_Init();
	void KWH_EEPROM_Read();
	void KWH_Clear();
	Uint32 KWH_Get_Lifetime();
	void KWH_Calc_Seconds();
	void KWH_Task();

#endif // _KWH_H