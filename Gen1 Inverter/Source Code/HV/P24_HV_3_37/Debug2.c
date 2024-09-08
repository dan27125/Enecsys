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








#if defined(Uses_Debug2)


#if defined (Uses_Debug1)
char * console_get_VID_ascii(int vid)
{
	switch(vid)
	{
		case VID_MPPT_RAMP:
		{
			return "DC-RAMP";
		}
		case VID_POWER_UPPER_LIMIT:
		{			
			return "POWER-LIMIT";
		}
		case VID_SLOW_CHANGE_RATE:
		{					
			return "SLOW";
		}
		case VID_MPPT_RAMP_DELAY:
		{						
			return "FAST";
		}
		case VID_MPPT_PERIOD:
		{						
			return "MPPT";
		}
		case VID_GRID_AVERAGING_SCALE:
		{						
			return "AC-SCALE";
		}
		
		case VID_MPPT_LOW:
		{						
			return "DC-MIN";
		}
		
		case VID_TEMPERATURE_HOT:
		{						
			return "TEMP";
		}
		
		case VID_BUCK_CURRENT_SCALE:
		{						
			return "BUCK";
		}
		
		case VID_DC_INJECTION_THRESHOLD:
		{						
			return "DC-INJECT";
		}

		case VID_FW:
		{
			return "FW";
		}

		case VID_INVERTER_ON_OFF:
		{
			return "ON/OFF";
		}

		case VID_GO_STOP:
		{
			return TEXT_STOP;
		}
		
		case VID_GEO_CODE:
		{
			return "COUNTRY";
		}
		case VID_ENGINEERING_MODE:
		{
			return "ENG-MODE";
		}
		case VID_CLEAR_INTERNAL_ALARM:
		{
			return "CLEAR-ALARM";
		}
		case VID_INVERTER_APP_FIRMWARE:
		{
			return "APP-FIRM";
		}
		case VID_ACCUMULATED_ENERGY:
		{
			return "KWH";
		}
		case VID_READ_ALARM_STATE:
		{
			return "ALARM";
		}
		case VID_STORED_ENGINEERING_MODE:
		{
			return "ENG-MODE";
		}
		case VID_GRID_FREQUENCY:
		{
			return "FREQ";
		}
		case VID_RUN_STATE:
		{
			return "GRID";
		}
		case VID_DEFAULT_TUNING_PARAMETERS:
		{
			return "DEF-TUNE";
		}
		case VID_UPDATE_TUNING_PARAMETERS:
		{
			return "UP-TUNE";
		}
    }
	return "ERROR";
}
#endif


#endif

