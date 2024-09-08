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


#if !defined( SVC_TEMPERATURE_H )
#define SVC_TEMPERATURE_H

	typedef struct
	{
		Sint16	celsius;
		Uint16	sample;
        Sint16  vdrop_scale;
	} Temperature_Record;

	typedef union {
		struct 
		{
			unsigned hot_max:1;
			unsigned hot_run:1;			
			unsigned cold_max:1;
			unsigned cold_run:1;
			unsigned ac_adjust:1;			
		};

		struct {
			unsigned int value;
		};
	} Temperature_Bits;
	
	typedef struct 
	{
		Uint8				on;
		Uint8				off;
	} Temperature_Count;
	
	typedef struct 
	{
		Temperature_Count	count;

	} Temperature_Item;

	typedef struct 
	{
		Temperature_Bits	bit;
		#if defined(Uses_Debug_Temperature)
		Uint16				dcnt;
		#endif

        volatile Sint16     ac_vdrop;

		Uint16				sample;
		Sint16				celsius;
		Sint16				fahrenheit;
        
		Temperature_Item	hot;
		
	} Temperature_Control_Block;
	
	public volatile Temperature_Control_Block temperature;
	
	void Temperature_Lookup(Uint16 adcReading);
	Sint16 Temperature_celsius_to_fahrenheit(Sint16 c);
	void Temperature_Sample(Uint16 sample);
	void Temperature_Set_Limits();
	void Temperature_Start();
	

#endif 
