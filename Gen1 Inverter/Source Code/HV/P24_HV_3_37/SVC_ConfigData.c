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



	#include "PVKey904.h"
	#include "SVC_CountrySpecificData.h"


	const ConfigData ConfigData_50Hz = 
	{
	    GRID_SAMPLE_TIME_50HZ, 
	    C_POWER_SAMPLE_COUNT_50HZ,
//	    TMR_1_COUNT_LIMIT_50HZ,
	    NO_AC_TIMEOUT_MS_50HZ,
//	    MIN_DC_LINK_SAMPLE_TIME_50HZ,
//	    MAX_DC_LINK_SAMPLE_TIME_50HZ,
//	    DC_LINK_SAMPLE_TIME_OFFSET_50HZ,
		DC_LINK_SAMPLE_TIME_50HZ,
//	    DC_LINK_AMPLITUDE_SCALE_50HZ,
	    PHASE_SAMPLE_TIME_50HZ,
	    FAST_DISCONNECT_MIN_GRID_PERIOD_50HZ,
	    FAST_DISCONNECT_MAX_GRID_PERIOD_50HZ,
	    SINE_CONSTANT_50HZ,
	    SINE_CONSTANT_OFFSET_50HZ,
	    SAMPLE_OFFSET_50HZ,
	    INITIAL_SAMPLE_COUNT_50HZ,
	    LOW_POWER_AMPLITUDE_50HZ,
	    MIN_RECONNECT_GRID_PERIOD_50HZ,
	    MAX_RECONNECT_GRID_PERIOD_50HZ,
	    PFC_DELAY_50HZ,
	    TEMPERATURE_SAMPLE_TIME_50HZ,   
		LOW_POWER_SAMPLE_COUNT_50HZ, 
		SAMPLE_NUMBER_OFFSET_50HZ,
		50
	};    
	
	const ConfigData ConfigData_60Hz = 
	{
	    GRID_SAMPLE_TIME_60HZ, 
	    C_POWER_SAMPLE_COUNT_60HZ,
//	    TMR_1_COUNT_LIMIT_60HZ,
		NO_AC_TIMEOUT_MS_60HZ,
//	    MIN_DC_LINK_SAMPLE_TIME_60HZ,
//	    MAX_DC_LINK_SAMPLE_TIME_60HZ,
//	    DC_LINK_SAMPLE_TIME_OFFSET_60HZ,
		DC_LINK_SAMPLE_TIME_60HZ,
//	    DC_LINK_AMPLITUDE_SCALE_60HZ,
	    PHASE_SAMPLE_TIME_60HZ,
	    FAST_DISCONNECT_MIN_GRID_PERIOD_60HZ,
	    FAST_DISCONNECT_MAX_GRID_PERIOD_60HZ,
		
	    SINE_CONSTANT_60HZ,
	    SINE_CONSTANT_OFFSET_60HZ,
	    SAMPLE_OFFSET_60HZ,
	    INITIAL_SAMPLE_COUNT_60HZ,
		
	    LOW_POWER_AMPLITUDE_60HZ,
	    MIN_RECONNECT_GRID_PERIOD_60HZ,
	    MAX_RECONNECT_GRID_PERIOD_60HZ,
	    PFC_DELAY_60HZ,
	    SVC_ST_GET_60HZ_PERIOD_FROM_50HZ(TEMPERATURE_SAMPLE_TIME_50HZ),
		LOW_POWER_SAMPLE_COUNT_60HZ,
		SAMPLE_NUMBER_OFFSET_60HZ,
		60
	};    
	
