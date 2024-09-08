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

//	DGA 2022-03-26  Added 3rd dimension to array "AC scale" for correction of AC voltage 
//	When cold AC reads low roughly 5% .  When warm,hot AC reads HIGH 5-10%
//	

/*
    #if defined(2024_OLD)
	static const Temperature_Record temperature_table[] = 
	{
//		Cesius		ADC			AC SCALE
	{	-43,		1023,		14 	},
	{	-42,		1022,		14	},
	{	-41,		1020,		13	},
	{	-40,		1018,		13	},
	{	-39,		1017,		13	},
	{	-38,		1016,		13	},
	{	-37,		1015,		12	},
	{	-36,		1013,		12	},
	{	-35,		1012,		12	},
	{	-34,		1010,		12	},
	{	-33,		1008,		11	},
	{	-32,		1005,		11	},
	{	-31,		1002,		11	},
	{	-30,		1000,		11	},
	{	-29,		997,		10	},
	{	-29,		996,		10	},
	{	-28,		994,		10	},
	{	-27,		991,		10	},
	{	-26,		987,		9	},
	{	-25,		984,		9	},
	{	-24,		981,		9	},
	{	-23,		977,		9	},
	{	-22,		974,		8	},
	{	-21,		971,		8	},
	{	-20,		967,		8	},
	{	-20,		966,		8	},
	{	-19,		963,		7	},
	{	-18,		957,		7	},
	{	-17,		952,		7	},
	{	-16,		947,		7	},		// [-2] 04F -16C  3.31 *2 
	{	-15,		941,		6	},
	{	-14,		936,		6	},
	{	-13,		931,		6	},
	{	-12,		925,		5	},
	{	-11,		920,		5	},
	{	-10,		915,		5	},
	{	-9,			909,		5	},
	{	-9,			907,		4	},
	{	-8,			904,		4	},
	{	-7,			898,		4	},
	{	-6,			892,		4	},
	{	-5,			887,		3	},		// [-1] 23F -5C   3.31
	{	-4,			881,		3	},
	{	-3,			875,		2	},
	{	-2,			869,		2	},
	{	-1,			864,		1	},
	{	0,			858,		1	},		
	{	1,			852,		0	},		// [0] 33F 1C
	{	1,			850,		0	},
	{	2,			845,		0	},
	{	3,			836,		0	},
	{	4,			828,		0	},
	{	5,			819,		0	},
	{	6,			810,		0	},
	{	7,			801,		0	},
	{	8,			792,		0	},
	{	9,			784,		0	},
    
	{	10,			775,		-1	},
	{	10,			771,		-1	},
	{	11,			766,		-1	},
	{	12,			756,		-1	},
	{	13,			746,		-1	},
	{	14,			736,		-1	},
	{	15,			726,		-1	},
	{	16,			716,		-1	},
	{	17,			706,		-1	},
	{	18,			696,		-1	},
	{	18,			691,		-1	},
	{	19,			685,		-1	},
	{	20,			675,		-1	},
	
	{	21,			665,		-2	},
	{	22,			654,		-2	},
	{	23,			644,		-2	},
	{	24,			634,		-2	},
	{	25,			623,		-2	},
	{	26,			613,		-2	},
	{	27,			602,		-2	},
	{	28,			592,		-2	},
	{	29,			582,		-2	},
	{	29,			577,		-2	},
	{	30,			572,		-2	},
	{	31,			562,		-2	},
	
	{	32,			552,		-3	},
	{	33,			542,		-3	},
	{	34,			533,		-3	},
	{	35,			523,		-3	},
	{	36,			513,		-3	},
	{	37,			503,		-3	},
	{	38,			493,		-3	},		// [+1] 102F 38C		1 * 3.31
	{	38,			489,		-3	},
	{	39,			483,		-3	},		
	{	40,			472,		-3	},
	{	41,			462,		-3	},
	{	42,			451,		-3	},
	{	43,			440,		-3	},
	
	{	44,			430,		-4	},
	{	45,			419,		-4	},
	{	46,			408,		-4	},		// +2 114F 46C  [8]		2 * 3.31  6.62
	{	47,			398,		-4	},
	{	48,			387,		-4	},
	
	{	48,			382,		-5	},
	{	49,			377,		-5	},
	{	50,			369,		-5	},
	{	51,			360,		-5	},
	{	52,			352,		-5	},
	{	53,			343,		-5	},
	{	54,			334,		-5	},
	
	{	55,			326,		-6	},
	{	56,			317,		-6	},
	{	57,			309,		-6	},
	{	58,			300,		-6	},
	{	59,			292,		-6	},
	
	{	60,			283,		-7	},		// 
	{	61,			274,		-7	},		// 
	{	62,			266,		-7	},

    
    
//		*******************************************************	
	{	63,			257,		-8	},
	{	64,			249,		-8	},
	{	64,			245,		-8	},		// 
	{	65,			241,		-8	},
	
	{	66,			235,		-9	},
	{	67,			229,		-9	},
	{	68,			223,		-9	},
	
	{	69,			217,		-10	},		// -16
	{	70,			211,		-10	},
	{	70,			208,		-10	},
	{	71,			205,		-10	},
	
	{	72,			200,		-11	},		// 
	{	73,			196,		-11 },
	{	74,			191,		-11	},
	
	{	75,			186,		-12	},
	{	75,			184,		-12	},		// 
	{	76,			181,		-12	},		// 
	{	77,			176,		-12	},
	{	78,			172,		-12	},
	{	79,			167,		-12	},
	{	80,			162,		-12	},
	{	80,			160,		-12	},		//
	{	81,			158,		-12	},
	{	82,			154,		-12	},
	{	83,			150,		-12	},
	{	84,			146,		-12	},		//
	{	85,			142,		-12	},
	{	85,			140,		-12	},
	{	86,			138,		-12	},	
	{	87,			134,		-12	},
	{	88,			131,		-12	},
	{	89,			127,		-12	},
	{	90,			123,		-12	},
	{	90,			122,		-12	},
	
	{	91,			120,		-13	},
	{	92,			117,		-13	},
	{	93,			115,		-13	},
	{	94,			112,		-13	},		//
	
	{	95,			109,		-14	},		//
	{	95,			108,		-14	},
	{	96,			106,		-14	},
	{	97,			103,		-14	},		//
	
	{	98,			101,		-15	},
	{	99,			98,			-15	},
	{	100,		95,			-15	},
	{	101,		92,			-15	},		//
	{	102,		89,			-15	},		// 
	{	103,		87,			-15	},		// 
	{	104,		84,			-15	},		// 
	
	{	105,		81,			-16	},		// 
	{	105,		80,			-16	},		// 
	{	106,		79,			-16 },		// 
	{	107,		77,			-16	},		// 
	{	108,		75,			-16	},		// 
	
	{	109,		73,			-17	},		//
	{	110,		71,			-17	},		// 
	{	111,		69,			-17	},		// 
	{	112,		67,			-17	},		// 
	{	113,		65,			-17	},		// 
	{	114,		63,			-17	},		// 
	{	115,		61,			-17	},		// 
	
	{	116,		59,			-18	},		// 
	{	117,		57,			-18	},		// 
	{	118,		55,			-18	},		// 
	{	119,		53,			-18 },		// 
	{	120,		51,			-18	},		// 
	{	121,		49,			-18}		// 
	};
    #endif
*/

//  DGA Table revised 2/29/2024 
	static const Temperature_Record temperature_table[] = 
	{
//		Cesius		ADC		VDROP correction 
    {	-30,		1000,	28},
	{	-29,		997, 	28},
	{	-28,		994, 	27},
	{	-27,		991, 	26},
	{	-26,		987, 	26},
	{	-25,		984,	25},
	{	-24,		981, 	24},
	{	-23,		977, 	24},
	{	-22,		974,	23},
	{	-21,		971, 	23},
	{	-20,		967, 	22},
	{	-19,		963, 	21},
	{	-18,		957, 	21},
	{	-17,		952, 	20},
	{	-16,		947, 	19},
	{	-15,		941, 	19},
	{	-14,		936, 	18},
	{	-13,		931, 	18},
	{	-12,		925, 	17},
    
	{	-11,		920, 	16},
	{	-10,		915, 	16},
	{	-9,			909, 	15},
	{	-8,			904, 	14},
	{	-7,			898, 	14},
	{	-6,			892, 	13},
	{	-5,			887, 	13},
	{	-4,			881, 	12},
	{	-3,			875, 	11},
	{	-2,			869, 	11},
	{	-1,			864, 	10},
        
    
	{	0,			858, 	10},		
	{	1,			852, 	9},
	{	2,			845, 	8},
	{	3,			836, 	8},
	{	4,			828, 	7},
	{	5,			819, 	6},
	{	6,			810, 	5},
	{	7,			801, 	4},
	{	8,			792, 	3},
	{	9,			784, 	3},
    
	{	10,			775, 	2},
	{	11,			766, 	2},
	{	12,			756, 	1},
	{	13,			746, 	1},
	{	14,			736, 	0},
    
	{	15,			726, 	-1},
	{	16,			716, 	-2},
	{	17,			706, 	-3},
	{	18,			691, 	-4},
	{	19,			685, 	-5},
	{	20,			675, 	-6},
	{	21,			665, 	-7},
	{	22,			654, 	-8},
	{	23,			644, 	-9},
	{	24,			634, 	-10},
    
	{	25,			623, 	-11},
	{	26,			613, 	-12},
	{	27,			602, 	-12},
	{	28,			592, 	-12},
	{	29,			577, 	-13},
	{	30,			572, 	-13},

	{	31,			562, 	-13},
	{	32,			552, 	-14},
	{	33,			542, 	-14},
	{	34,			533, 	-14},
	{	35,			523, 	-15},
	{	36,			513, 	-15},
	{	37,			503, 	-15},
	{	38,			493, 	-16},
	{	39,			483, 	-16},
	{	40,			472, 	-16},
        
    
	{	41,			462, 	-16},
	{	42,			451, 	-17},
	{	43,			440, 	-17},
	{	44,			430, 	-18},
	{	45,			419, 	-18},
	{	46,			408, 	-19},	
	{	47,			398, 	-20},
	{	48,			387, 	-20},
	{	49,			382, 	-21},
	{	50,			369, 	-22},
    
	{	51,			360, 	-22},
	{	52,			352, 	-23},
	{	53,			343, 	-23},
	{	54,			334,    -24},
	{	55,			326, 	-25},
	{	56,			317, 	-25},
	{	57,			309, 	-30},
	{	58,			300, 	-30},
	{	59,			292, 	-31},
	{	60,			283, 	-32},		
	{	61,			274, 	-32},		
	{	62,			266, 	-33},
	{	63,			257, 	-34},
	{	64,			249, 	-34},		
	{	65,			241,    -35},
	{	66,			235,    -35},
	{	67,			229, 	-36},
	{	68,			223, 	-37},
	{	69,			217, 	-37},
	{	70,			208, 	-38},
	{	71,			205, 	-39},
	{	72,			200,    -29},
	{	73,			196,    -40},
	{	74,			191, 	-40},
	{	75,			184, 	-41},
	{	76,			181, 	-42},
	{	77,			176, 	-42},
	{	78,			172, 	-43},
	{	79,			167,    -44},
	{	80,			162, 	-44},
	{	81,			158, 	-45},
	{	82,			154, 	-45},
	{	83,			150, 	-46},
	{	84,			146, 	-47},
	{	85,			142, 	-47},
	{	86,			138, 	-48},	
	{	87,			134, 	-48},
	{	88,			131, 	-49},
	{	89,			127, 	-50},
	{	90,			122, 	-50},
	{	91,			120, 	-51},
	{	92,			117, 	-52},
	{	93,			115, 	-52},
	{	94,			112, 	-53},	
	{	95,			108, 	-53},	
	{	96,			106, 	-54},
	{	97,			103, 	-55},	
	{	98,			101, 	-55},
	{	99,			98, 	-56},
    
    {	100,		95, 	-56},
	{	101,		92, 	-56},
	{	102,		89, 	-56},
	{	103,		87, 	-56},
	{	104,		84, 	-56},
	{	105,		81, 	-56},
	{	106,		79, 	-56},
	{	107,		77, 	-56},
	{	108,		75, 	-56},
	{	109,		73, 	-56},
	{	110,		71, 	-56},
	{	111,		69, 	-56},
	{	112,		67, 	-56},
	{	113,		65, 	-56},
	{	114,		63, 	-56},
	{	115,		61, 	-56},
	{	116,		59, 	-56},
	{	117,		57, 	-56},
	{	118,		55, 	-56},
	{	119,		53, 	-56},
	{	120,		51, 	-56},
	{	121,		49, 	-56}
    
	};
    
	#define TEMPERATURE_TABLE_RECORDS		(sizeof(temperature_table) / sizeof(Temperature_Record))

void Temperature_Lookup(Uint16 sample)
{
    Uint16 i;
	// Scan the table for the slope and offset values. 
	for (i = 0; i < TEMPERATURE_TABLE_RECORDS - 1; i++)
	{
		if (sample >= temperature_table[i].sample)
		{
			// Found the base reading
			break;
		}
	}    	
	temperature.sample = sample;
	temperature.celsius = temperature_table[i].celsius;
    temperature.ac_vdrop = (66 + temperature_table[i].vdrop_scale);
	temperature.fahrenheit = Temperature_celsius_to_fahrenheit(temperature.celsius);
    
//  printf("TEMP:%df %dc AC-SCALE:%d\n", temperature.fahrenheit, temperature.celsius, temperature.ac_scale);
	
}

Sint16 Temperature_celsius_to_fahrenheit(Sint16 c)
{
//	celsius to fahrenheit
// C × 9/5) + 32 = 32°F
//	Sint16 a,f;
//	a = c * 9;
//	f = (a / 5) + 32;
	return  (((c * 9)/5) + 32);
}

void Temperature_Start()
{
	memset( (void*) &temperature, 0, sizeof(temperature));
	Temperature_Set_Limits();
	temperature.bit.ac_adjust=1;	// Default ON
    temperature.ac_vdrop = 0;       // room temperature
    
}

void Temperature_Set_Limits()
{
	
}	

void Temperature_Sample(Uint16 sample)
{
//	500 ms interval 	
//	DGA 2022-03-26 - We are here 2x per sec

	Temperature_Lookup(sample);
	
	#if defined(Uses_Debug_Temperature)
	temperature.dcnt++;
	#endif
	
//	hot_off = tune.temperature_hot - 20;
	
	if (temperature.celsius >= tune.temperature.hot_on)
	{
		if (temperature.bit.hot_max==0)
		{
			if (temperature.hot.count.on++ >= 10)
			{
				temperature.bit.hot_max=1;
				temperature.hot.count.off = 0;

				#if defined(Uses_Debug_Temperature)
				printf("HOT ON %u\n",temperature.celsius);
				#endif				
			}
		}
	}
	if ( (temperature.bit.hot_max==1) && (temperature.celsius <= (tune.temperature.hot_off)))
	{
		if (temperature.hot.count.off++ >= 10)
		{
			temperature.bit.hot_max=0;
			temperature.hot.count.on = 0;
			#if defined(Uses_Debug_Temperature)
			printf("HOT OFF %u\n",temperature.celsius);
			#endif			
		}
	}
	
	if (temperature.bit.hot_max)
	{
        if (fault.alarm.temp_hot==0)
            HAL_Fault_Set(ALARM_DISC|ALARM_TEMP_HOT,FC_TEMP_HOT,temperature.celsius,tune.temperature.hot_on);
	}
	else
	{
		HAL_Fault_Clear(ALARM_TEMP_HOT | ALARM_TEMP_COLD);		
	}
	
}