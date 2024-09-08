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


//	OLD SETTINGS BEFORE 9/1/2022
//	#define DC_LINK_FAULT_START			(260)
//	#define DC_LINK_FAULT_RUN			(260)

	#if defined(SMI360_72)

//	#define DC_LINK_FAULT_START			(330)	// 
//	#define DC_LINK_FAULT_RUN			(300)


	#endif

	#if defined(SMI240_60)
//	#define DC_LINK_FAULT_START			(330)	// 
	//#define DC_LINK_FAULT_RUN			(300)
	#endif

	#if defined(SMI480_60)
	#define DC_LINK_FAULT_START			(330)	// 
	#define DC_LINK_FAULT_RUN			(300)
	#endif


	
void APP_DC_Link_Limits(Uint16 dc_volts)
{
	Uint16 dc_min;
	
	
//	**** FILTER RESET WAIT STATE ********	
	if (APP_State_Get()== INVERTER_STOP)
		return;	// DONT RUN CODE BELOW
//	*************************************	

	if(  (APP_State_Get() == INVERTER_RUN) && (pwm.buck.enable) )
	{
		dc_min = DC_LINK_FAULT_RUN;		// RUN threshold
	}
	else
	{
		dc_min = DC_LINK_FAULT_START;	// START threshold
	}
	
	if (dc_volts < dc_min)
	{
		if(fault.alarm.dclink_low==0)
		{
            HAL_Fault_Set(ALARM_DISC|ALARM_DC_LINK_LOW,FC_DCV_LO,dc_volts,dc_min);
			#if defined(Uses_Debug_DC_Link)
			if(app.runSecCpu)
			{
				printf("FAULT DCLINK %uv below %uv  %s,%d\n",dc_volts, dc_min, __FILE__,__LINE__);
			}
			#endif
		}
	}
	else
	{
		HAL_Fault_Clear(ALARM_DC_LINK_LOW|ALARM_DC_LINK_HIGH);
	}
	
}

    
#endif  