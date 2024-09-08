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

Uint16 Bounds_Check_Uint16(Uint16 x, Uint16 _min, Uint16 _max)
{
	if (x < _min)
	{
//		printf("bounds lo [%d] %d\n",x,_min);
		x = _min;
	}
	
	else if (x > _max)
	{
//		printf("bounds hi [%d] %d\n",x,_max);
		x = _max;
	}

	return x;
}

/*
void NVS_Tune_Write_PV_Ramp_Down(Uint16 _pvdc)
{
	Uint16 dcl,pvdc;
    pvdc = Bounds_Check_Uint16(_pvdc,TUNE_MPPT_RAMP_DN_MIN_PV10X,TUNE_MPPT_RAMP_DN_MAX_PV10X);
	dcl = ADC_PV10X_Calc_DCL(pvdc);
	
	// ******* down adjustment ****
	dcl += 2;	// DGA May 18, 2022 round up 
	
	
//    #if defined(Uses_MPPT_Debug)
//	if (mppt.bit.debug)
//	{
//		printf("%s PV:%d,%d DCL:%d\n",TEXT_DOWN,_pvdc,pvdc,dcl);
//	}
  //  #endif

	NVS_VID_Write_Item(VID_MPPT_RAMP_DN, dcl);
}
*/
void NVS_Tune_Write_PV_Ramp_Up(Uint16 _pvdc)
{
	Uint16 dcl,pvdc;
    pvdc = Bounds_Check_Uint16(_pvdc,TUNE_MPPT_RAMP_UP_MIN_PV10X,TUNE_MPPT_RAMP_UP_MAX_PV10X);
	dcl = ADC_PV10X_Calc_DCL(pvdc);

	NVS_VID_Write_Item(VID_MPPT_RAMP_UP, dcl);
}

void SVC_Tune_Bounds_Check()
{
	Uint16 pvdc_ramp, pvdc_down;
	
    pvdc_ramp = Bounds_Check_Uint16(DCLINK_CALC_PVX10(tune.mppt.lock.up.hvdc),TUNE_MPPT_RAMP_UP_MIN_PV10X,TUNE_MPPT_RAMP_UP_MAX_PV10X);
	tune.mppt.lock.up.hvdc = PV10X_CALC_DCLINK(pvdc_ramp);
    pvdc_down = Bounds_Check_Uint16(DCLINK_CALC_PVX10(tune.mppt.lock.dn.hvdc),TUNE_MPPT_RAMP_DN_MIN_PV10X,TUNE_MPPT_RAMP_DN_MAX_PV10X);
    
	tune.mppt.lock.dn.hvdc = PV10X_CALC_DCLINK(pvdc_down);
	
    tune.mppt.ramp_up_delay = Bounds_Check_Uint16( tune.mppt.ramp_up_delay, TUNE_MPPT_RAMP_UP_DELAY_MIN, TUNE_MPPT_RAMP_UP_DELAY_MAX);
	tune.mppt.ramp_dn_delay = Bounds_Check_Uint16( tune.mppt.ramp_dn_delay, TUNE_MPPT_RAMP_DN_DELAY_MIN, TUNE_MPPT_RAMP_DN_DELAY_MAX);
    tune.mppt.ramp_up_percent = Bounds_Check_Uint16(tune.mppt.ramp_up_percent, TUNE_MPPT_RAMP_UP_PERCENT_MIN, TUNE_MPPT_RAMP_UP_PERCENT_MAX);
    tune.mppt.ramp_trigger = Bounds_Check_Uint16(tune.mppt.ramp_trigger, TUNE_MPPT_RAMP_TRIG_MIN, TUNE_MPPT_RAMP_TRIG_MAX);
    
//  tune.mppt.ramp_dn_percent = Bounds_Check_Uint16(tune.mppt.ramp_dn_percent, TUNE_MPPT_RAMP_DN_PERCENT_MIN, TUNE_MPPT_RAMP_DN_PERCENT_MAX);
//	tune.temperature.hot.on = Bounds_Check_Uint16(tune.temperature.hot.on,TUNE_TEMP_HOT_MIN,TUNE_TEMP_HOT_MAX);
//	tune.power_limit = TUNE_POWER_UPPER_LIMIT_RANGE(tune.power_limit);
    
    GEO_Nvs.freq.F50.lower.herz = Bounds_Check_Uint16(GEO_Nvs.freq.F50.lower.herz,TUNE_GEO_FREQ_50HZ_LOWER_MIN,TUNE_GEO_FREQ_50HZ_LOWER_MAX );
    GEO_Nvs.freq.F50.upper.herz = Bounds_Check_Uint16(GEO_Nvs.freq.F50.upper.herz,TUNE_GEO_FREQ_50HZ_UPPER_MIN,TUNE_GEO_FREQ_50HZ_UPPER_MAX);
    GEO_Nvs.freq.F60.lower.herz = Bounds_Check_Uint16(GEO_Nvs.freq.F60.lower.herz,TUNE_GEO_FREQ_60HZ_LOWER_MIN,TUNE_GEO_FREQ_60HZ_LOWER_MAX );
    GEO_Nvs.freq.F60.upper.herz = Bounds_Check_Uint16(GEO_Nvs.freq.F60.upper.herz,TUNE_GEO_FREQ_60HZ_UPPER_MIN,TUNE_GEO_FREQ_60HZ_UPPER_MAX);
	
}

void NVS_VID_Read_All()
{
	
	NVS_GEO_Read();		// DGA read the GEO
    
    
//************************************************************************	
	if (NVS_Read_Word(NVS_GEO_FREQ_50HZ_LOWER, (Uint16 *) &GEO_Nvs.freq.F50.lower.herz,NVS_VALID_DEFAULT)) 
	{
        GEO_Nvs.freq.F50.lower.herz = TUNE_GEO_FREQ_50HZ_LOWER_DEF;
		NVS_Write_Word(NVS_GEO_FREQ_50HZ_LOWER ,GEO_Nvs.freq.F50.lower.herz,NVS_VALID_DEFAULT);
	}
//************************************************************************

//************************************************************************	
	if (NVS_Read_Word(NVS_GEO_FREQ_50HZ_UPPER, (Uint16 *) &GEO_Nvs.freq.F50.upper.herz,NVS_VALID_DEFAULT)) 
	{
        GEO_Nvs.freq.F50.upper.herz = TUNE_GEO_FREQ_50HZ_UPPER_DEF;
		NVS_Write_Word(NVS_GEO_FREQ_50HZ_UPPER ,GEO_Nvs.freq.F50.upper.herz,NVS_VALID_DEFAULT);
	}
//************************************************************************
    
//************************************************************************	
	if (NVS_Read_Word(NVS_GEO_FREQ_60HZ_LOWER, (Uint16 *) &GEO_Nvs.freq.F60.upper.herz,NVS_VALID_DEFAULT)) 
	{
        GEO_Nvs.freq.F60.lower.herz = TUNE_GEO_FREQ_60HZ_LOWER_DEF;
		NVS_Write_Word(NVS_GEO_FREQ_60HZ_LOWER ,GEO_Nvs.freq.F60.lower.herz,NVS_VALID_DEFAULT);
	}
//************************************************************************

//************************************************************************	
	if (NVS_Read_Word(NVS_GEO_FREQ_60HZ_UPPER, (Uint16 *) &GEO_Nvs.freq.F60.upper.herz,NVS_VALID_DEFAULT)) 
	{
        GEO_Nvs.freq.F60.upper.herz = TUNE_GEO_FREQ_60HZ_UPPER_DEF;
		NVS_Write_Word(NVS_GEO_FREQ_60HZ_UPPER ,GEO_Nvs.freq.F60.upper.herz,NVS_VALID_DEFAULT);
	}
//************************************************************************

//************************************************************************	
	if (NVS_Read_Word(NVS_MPPT_AUTO_MODE, (Uint16 *) &tune.mppt.auto_mode,NVS_VALID_DEFAULT)) 
	{
        tune.mppt.auto_mode = TUNE_MPPT_MODE_DEF;
		NVS_Write_Word(NVS_MPPT_AUTO_MODE,tune.mppt.auto_mode,NVS_VALID_DEFAULT);
	}
//************************************************************************

//************************************************************************	
	if (NVS_Read_Word(NVS_MPPT_RAMP_UP_PERCENT, (Uint16 *) &tune.mppt.ramp_up_percent,NVS_VALID_DEFAULT)) 
	{
        tune.mppt.ramp_up_percent = TUNE_MPPT_RAMP_UP_PERCENT_DEF;
		NVS_Write_Word(NVS_MPPT_RAMP_UP_PERCENT,tune.mppt.ramp_up_percent,NVS_VALID_DEFAULT);
	}
//************************************************************************

#if defined(Old_Code)    
//************************************************************************	
	if (NVS_Read_Word(NVS_MPPT_RAMP_UP_TIMER, (Uint16 *) &tune.mppt.ramp_up_timer,NVS_VALID_DEFAULT)) 
	{
        tune.mppt.ramp_up_timer = TUNE_MPPT_RAMP_UP_TIMER_DEF;
		NVS_Write_Word(NVS_MPPT_RAMP_UP_TIMER,tune.mppt.ramp_up_timer,NVS_VALID_DEFAULT);
	}
//************************************************************************
#endif    

//************************************************************************	
	if (NVS_Read_Word(NVS_MPPT_RAMP_TRIG, (Uint16 *) &tune.mppt.ramp_trigger,NVS_VALID_DEFAULT)) 
	{
        tune.mppt.ramp_trigger = TUNE_MPPT_RAMP_TRIG_DEF;
		NVS_Write_Word(NVS_MPPT_RAMP_TRIG,tune.mppt.ramp_trigger,NVS_VALID_DEFAULT);
	}
//************************************************************************
    
//************************************************************************	
    
	if (NVS_Read_Word(NVS_MPPT_RAMP_DN_PERCENT, (Uint16 *) &tune.mppt.ramp_dn_percent,NVS_VALID_DEFAULT)) 
	{
        tune.mppt.ramp_dn_percent = TUNE_MPPT_RAMP_DN_PERCENT_DEF;
		NVS_Write_Word(NVS_MPPT_RAMP_DN_PERCENT,tune.mppt.ramp_dn_percent,NVS_VALID_DEFAULT);
	}
//************************************************************************
    
//************************************************************************	
	if (NVS_Read_Word(NVS_MPPT_RCB, (Uint16 *) &tune.mppt.rcb,NVS_VALID_DEFAULT)) 
	{
        tune.mppt.rcb.value = TUNE_MPPT_BITS_DEF;
		NVS_Write_Word(NVS_MPPT_RCB,tune.mppt.rcb.value,NVS_VALID_DEFAULT);
	}
//************************************************************************

//************************************************************************	
	if (NVS_Read_Word(NVS_MPPT_RAMP_UP, (Uint16 *) &tune.mppt.lock.up.hvdc,NVS_VALID_DEFAULT)) 
	{
		
       	tune.mppt.lock.up.hvdc = ADC_PV10X_Calc_DCL( TUNE_MPPT_RAMP_UP_DEF_PV10X );
		NVS_Write_Word(NVS_MPPT_RAMP_UP,tune.mppt.lock.up.hvdc,NVS_VALID_DEFAULT);
		
        #if defined(Uses_MPPT_Debug)
		if (mppt.bit.debug)
		{
			printf("%s DEFAULT",TEXT_RAMP);
			print_source();
		}
        #endif

	}
	#if defined(Uses_Debug) && defined(Uses_Debug_Tune)
	else
	{
		printf("RAMP:%u ",tune.mppt.ramp.hvdc);
		print_source();
	
	}
	#endif
//************************************************************************


	
//************************************************************************	
	if (NVS_Read_Word(NVS_MPPT_RAMP_DN, (Uint16 *) &tune.mppt.lock.dn.hvdc,NVS_VALID_DEFAULT)) 
	{

		tune.mppt.lock.dn.hvdc = ADC_PV10X_Calc_DCL( TUNE_MPPT_RAMP_DN_DEF_PV10X );
		NVS_Write_Word(NVS_MPPT_RAMP_DN,tune.mppt.lock.dn.hvdc,NVS_VALID_DEFAULT);
		
		#if defined(Uses_Debug) && defined(Uses_Debug_Tune)
		printf("ERROR MPPT DOWN");
		print_source();
		#endif
	}
	#if defined(Uses_Debug) && defined(Uses_Debug_Tune)
	else
	{
	
	}
	#endif
//************************************************************************	
	
	
//************************************************************************
	if (NVS_Read_Word(NVS_MPPT_DELAY_RAMP, (Uint16 *)  &tune.mppt.ramp_up_delay,NVS_VALID_DEFAULT)) 
	{

		tune.mppt.ramp_up_delay = TUNE_MPPT_RAMP_UP_DELAY_DEF;
		NVS_Write_Word(NVS_MPPT_DELAY_RAMP,tune.mppt.ramp_up_delay,NVS_VALID_DEFAULT);
		
		#if defined(Uses_Debug) && defined(Uses_Debug_Tune)
		printf("ERROR DELAY");
		print_source();
		#endif
	}
	#if defined(Uses_Debug) && defined(Uses_Debug_Tune)
	else
	{
		printf("RAMP DELAY=%u",tune.mppt.ramp.delay);
		print_source();
	}
	#endif
//************************************************************************
	

//************************************************************************
	if (NVS_Read_Word(NVS_MPPT_DELAY_DOWN, (Uint16 *)  &tune.mppt.ramp_dn_delay,NVS_VALID_DEFAULT)) 
	{

		tune.mppt.ramp_dn_delay = TUNE_MPPT_RAMP_DN_DELAY_DEF;
		NVS_Write_Word(NVS_MPPT_DELAY_DOWN,tune.mppt.ramp_dn_delay,NVS_VALID_DEFAULT);
		
		#if defined(Uses_Debug) && defined(Uses_Debug_Tune)
		printf("ERROR DELAY");
		print_source();
		#endif
	}
	#if defined(Uses_Debug) && defined(Uses_Debug_Tune)
	else
	{
		printf("DOWN DELAY=%u",tune.mppt.down.delay);
		print_source();
	}
	#endif
//************************************************************************

	
//************************************************************************
	if (NVS_Read_Word(NVS_POWER_UPPER_LIMIT, (Uint16 *) &tune.nvs_power_limit,NVS_VALID_DEFAULT)) 
	{
		tune.nvs_power_limit = TUNE_POWER_UPPER_LIMIT_DEF;
		NVS_Write_Word(NVS_POWER_UPPER_LIMIT,tune.nvs_power_limit,NVS_VALID_DEFAULT);
		#if defined(Uses_Debug) && defined(Uses_Debug_Tune)
		printf("ERROR DELAY");
		print_source();
		#endif
	}
	#if defined(Uses_Debug) && defined(Uses_Debug_Tune)
	else
	{
//		printf("DELAY=%u",tune.mppt.ramp.delay);
		print_source();
	}
	#endif
//************************************************************************

	
	SVC_Tune_Bounds_Check();
	
	tune.mppt.lock.up.pvdc = DCLINK_CALC_PVX10(tune.mppt.lock.up.hvdc);
	tune.mppt.lock.dn.pvdc = DCLINK_CALC_PVX10(tune.mppt.lock.dn.hvdc);

}      

#if defined(Uses_Debug_NVS)
void NVS_Tune_Print_All()
{
	GEO_Print();
	MPPT_print_config();
	printf("\n");
}
#endif

ErrorCode NVS_VID_Read_Item(Uint8 vid, Uint8 *pData)
{
    ErrorCode errorCode = ERROR_OK_E;
    Uint16 intData = 0;

    // Check pre-conditions.
	if (NULL == pData)
		return ERROR_INVALID_POINTER_E;

	// Get the data
    switch(vid)
    {
        case VID_MPPT_RAMP_UP:
			intData = tune.mppt.lock.up.hvdc;
        break;
        
        case VID_POWER_UPPER_LIMIT:
			intData = tune.nvs_power_limit;
			if (intData< MPPT_MIN_WATTS)	// DGA prevent fault 
				intData = MPPT_MIN_WATTS;	// DGA prevent fault 
        break;
        
        case VID_MPPT_DELAY_RAMP_DN:
		{
			intData = tune.mppt.ramp_dn_delay;
		}
        break;
        
		case VID_MPPT_DELAY_RAMP_UP:
		intData = tune.mppt.ramp_up_delay;
		break;
        
        case VID_MPPT_MODE:         // READ 
        intData = tune.mppt.auto_mode;
//      printf("READ VID_MPPT_MODE %d\n",tune.mppt.auto_mode);
        break;
        
        case VID_MPPT_RAMP_UP_PERCENT: // READ 
        intData = tune.mppt.ramp_up_percent;
//      printf("READ VID_MPPT_AUTO_PERCENT %d\n",tune.mppt.ramp_percent);
        break;

        case VID_MPPT_RAMP_TRIG:   // READ 
        intData = tune.mppt.ramp_trigger;
        break;
        
        case VID_MPPT_RAMP_DN_PERCENT: // READ 
        intData = tune.mppt.ramp_dn_percent;
//      printf("READ VID_MPPT_DOWN_PERCENT %d\n",tune.mppt.down_percent);
        break;

        case VID_GRID_AVERAGING_SCALE:
		intData = (Uint16)(tune.acv_div);
        break;
        
        case VID_MPPT_RAMP_DN:
        intData = tune.mppt.rcb.value;
        break;
        
//      case VID_BUCK_CURRENT_SCALE:
        case VID_MPPT_RCB:
			intData = tune.mppt.rcb.value;
      break;
		
        case VID_TEMPERATURE_HOT:
			intData = tune.temperature.hot_on;
            break;
            
        case VID_GEO_FREQ_50HZ_MIN:
        {
            intData = GEO_Nvs.freq.F50.lower.herz;
            break;
        }
        case VID_GEO_FREQ_50HZ_MAX:
        {
            intData = GEO_Nvs.freq.F50.upper.herz;
            break;
        }
        case VID_GEO_FREQ_60HZ_MIN:
        {
            intData = GEO_Nvs.freq.F60.lower.herz;
            break;
        }
        case VID_GEO_FREQ_60HZ_MAX:
        {
            intData = GEO_Nvs.freq.F60.upper.herz;
            break;
        }
        
        default:
            intData = 0;
            errorCode = ERROR_OUT_OF_RANGE_E;
        break;                        
    }  
      
	// Format the output string and put it in the buffer. 
	sprintf((char *)pData, "%d", intData);

	#if defined (Uses_Debug_Tuning)
	printf("%s,%d  VID:%d,%s,%d,%s\n", __FILE__, __LINE__, 
		vid, console_get_VID_ascii(vid), intData, pData );
	#endif
	
    return (errorCode);  
}    

ErrorCode NVS_VID_Write_Item(Uint8 vid, Uint16 intData)
{
	#if defined (Uses_Debug_Tuning)
	{
		printf("VID:%d,%s,%d",vid, console_get_VID_ascii(vid), intData);
		print_source();
	}
	#endif
	
	APP_Inverter_Fault_History_Reset();

	switch(vid)
	{
		case VID_MPPT_RAMP_UP:
		{
			Uint16 pvdc,hvdc;
//			pvdc = TUNE_MPPT_RAMP_PVDC_RANGE( ADC_DCL_Calc_PV10X(intData) );
            pvdc = Bounds_Check_Uint16(ADC_DCL_Calc_PV10X(intData),TUNE_MPPT_RAMP_UP_MIN_PV10X,TUNE_MPPT_RAMP_UP_MAX_PV10X);
			hvdc = ADC_PV10X_Calc_DCL(pvdc);
			NVS_Write_Word(NVS_MPPT_RAMP_UP,hvdc,NVS_VALID_DEFAULT);
			tune.mppt.lock.up.hvdc = hvdc;	// DGA 
			tune.mppt.lock.up.pvdc = pvdc;
            #if defined(Uses_MPPT_Debug)
			if (mppt.bit.debug)
			{
				printf("WRITE RAMP HV:%u,%u PV:%u\n",intData,hvdc, pvdc);
			}
            #endif
		}
		break;
		
//****************************************************************
		
		case VID_MPPT_RAMP_DN:
		{			
			Uint16 pvdc,hvdc;
//			pvdc = TUNE_MPPT_DOWN_PVDC_RANGE( ADC_DCL_Calc_PV10X(intData) );
            pvdc = Bounds_Check_Uint16(ADC_DCL_Calc_PV10X(intData),TUNE_MPPT_RAMP_DN_MIN_PV10X,TUNE_MPPT_RAMP_DN_MAX_PV10X);
            
			hvdc = ADC_PV10X_Calc_DCL(pvdc);
			NVS_Write_Word(NVS_MPPT_RAMP_DN,intData,NVS_VALID_DEFAULT);
			tune.mppt.lock.dn.hvdc = hvdc;
			tune.mppt.lock.dn.pvdc = pvdc;	
//			tune.mppt.down_lock1.sample = DCLINK_GET_ADC(hvdc);
            
            #if defined(Uses_MPPT_Debug)
			if (mppt.bit.debug)
			{
				printf("WRITE DOWN HV:%u,%u PV:%u\n",intData,hvdc, pvdc);
			}
            #endif
			
		}
		break;
        
//****************************************************************
		case VID_MPPT_DELAY_RAMP_UP:
		{
			intData = Bounds_Check_Uint16(intData,TUNE_MPPT_RAMP_UP_DELAY_MIN,TUNE_MPPT_RAMP_UP_DELAY_MAX);
			NVS_Write_Word(NVS_MPPT_DELAY_RAMP,intData,NVS_VALID_DEFAULT);
			tune.mppt.ramp_up_delay = intData;

			#if defined(Uses_Debug) && defined(Uses_Debug_Tune)				
			printf("DELAY RAMP:%d",intData);
			print_source();
			#endif
		}
		break;
//****************************************************************

#if defined(Old_Code)                    
//****************************************************************
		case VID_MPPT_RAMP_UP_TIMER:
		{
			intData = Bounds_Check_Uint16(intData,TUNE_MPPT_RAMP_UP_TIMER_MIN,TUNE_MPPT_RAMP_UP_TIMER_MAX);
			NVS_Write_Word(NVS_MPPT_RAMP_UP_TIMER,intData,NVS_VALID_DEFAULT);
			tune.mppt.ramp_up_timer = intData;

			#if defined(Uses_Debug) && defined(Uses_Debug_Tune)				
			printf("RAMP-TIMER:%d",intData);
			print_source();
			#endif
		}
		break;
//****************************************************************
#endif
        
//****************************************************************
		case VID_MPPT_RAMP_TRIG:
		{
			intData = Bounds_Check_Uint16(intData,TUNE_MPPT_RAMP_TRIG_MIN ,TUNE_MPPT_RAMP_TRIG_MAX);
			NVS_Write_Word(NVS_MPPT_RAMP_TRIG,intData,NVS_VALID_DEFAULT);
			tune.mppt.ramp_trigger = intData;

			#if defined(Uses_Debug) && defined(Uses_Debug_Tune)				
			printf("RAMP-TIMER:%d",intData);
			print_source();
			#endif
		}
		break;
//****************************************************************
		
//****************************************************************
		case VID_MPPT_DELAY_RAMP_DN:
		{
			intData = Bounds_Check_Uint16(intData,TUNE_MPPT_RAMP_DN_DELAY_MIN,TUNE_MPPT_RAMP_DN_DELAY_MAX);
			NVS_Write_Word(NVS_MPPT_DELAY_DOWN,intData,NVS_VALID_DEFAULT);
			tune.mppt.ramp_dn_delay = intData;

			#if defined(Uses_Debug) && defined(Uses_Debug_Tune)				
			printf("DELAY DOWN:%d",intData);
			print_source();
			#endif
		}
		break;

//****************************************************************
		
		case VID_TEMPERATURE_HOT:
		{
            NVS_temperature_hot_write(intData);
//			printf("hot:%u",intData);
//			print_source();
			
		}
		break;
//****************************************************************
		
		case VID_POWER_UPPER_LIMIT:
		{
			intData = TUNE_POWER_UPPER_LIMIT_RANGE(intData);
			if (intData)
			{
				NVS_Write_Word(NVS_POWER_UPPER_LIMIT,intData,NVS_VALID_DEFAULT);
				tune.nvs_power_limit = intData;
			}
		}
		break;
//****************************************************************
        
		case VID_MPPT_MODE:   //  WRITE
        {
            //  WRITE
            tune.mppt.auto_mode = Bounds_Check_Uint16(intData,TUNE_MPPT_MODE_MIN,TUNE_MPPT_MODE_MAX);
            NVS_Write_Word(NVS_MPPT_AUTO_MODE,tune.mppt.auto_mode,NVS_VALID_DEFAULT);
//          printf("WRITE VID_MPPT_MODE %d\n", tune.mppt.auto_mode);
        }
        break;
        
		case VID_MPPT_RAMP_UP_PERCENT:   //  WRITE
        {
            //  WRITE
            tune.mppt.ramp_up_percent = Bounds_Check_Uint16(intData,TUNE_MPPT_RAMP_UP_PERCENT_MIN,TUNE_MPPT_RAMP_UP_PERCENT_MAX);
            NVS_Write_Word(NVS_MPPT_RAMP_UP_PERCENT,tune.mppt.ramp_up_percent,NVS_VALID_DEFAULT);
        }
        break;
  
		case VID_MPPT_RAMP_DN_PERCENT:   //  DOWN
        {
            //  DOWN
            tune.mppt.ramp_dn_percent = Bounds_Check_Uint16(intData,TUNE_MPPT_RAMP_DN_PERCENT_MIN,TUNE_MPPT_RAMP_DN_PERCENT_MAX);
            NVS_Write_Word(NVS_MPPT_RAMP_DN_PERCENT,tune.mppt.ramp_dn_percent,NVS_VALID_DEFAULT);
        }
        break;
          
		case VID_MPPT_RCB:   //  WRITE
        {
            //  WRITE
            tune.mppt.rcb.value = intData;
            NVS_Write_Word(NVS_MPPT_RCB,tune.mppt.rcb.value,NVS_VALID_DEFAULT);
        }
        break;
        
        
//		case VID_SLOW_CHANGE_RATE:
		case VID_GRID_AVERAGING_SCALE:
		{
//          tune.acv_div = intData;
            NVS_acv_divsor_write(intData);
  		}	
		break;
        
        case VID_GEO_FREQ_50HZ_MIN:
        {
            GEO_Nvs.freq.F50.lower.herz = Bounds_Check_Uint16(intData,TUNE_GEO_FREQ_50HZ_LOWER_MIN,TUNE_GEO_FREQ_50HZ_LOWER_MAX );
			NVS_Write_Word(NVS_GEO_FREQ_50HZ_LOWER, GEO_Nvs.freq.F50.lower.herz,NVS_VALID_DEFAULT);
            break;
        }

        case VID_GEO_FREQ_50HZ_MAX:
        {
            GEO_Nvs.freq.F50.upper.herz = Bounds_Check_Uint16(intData,TUNE_GEO_FREQ_50HZ_UPPER_MIN,TUNE_GEO_FREQ_50HZ_UPPER_MAX);
            NVS_Write_Word(NVS_GEO_FREQ_50HZ_UPPER, GEO_Nvs.freq.F50.upper.herz,NVS_VALID_DEFAULT);
            break;
        }
        
        case VID_GEO_FREQ_60HZ_MIN:
        {
            GEO_Nvs.freq.F60.lower.herz = Bounds_Check_Uint16(intData,TUNE_GEO_FREQ_60HZ_LOWER_MIN,TUNE_GEO_FREQ_60HZ_LOWER_MAX );
			NVS_Write_Word(NVS_GEO_FREQ_60HZ_LOWER, GEO_Nvs.freq.F60.lower.herz,NVS_VALID_DEFAULT);
            break;
        }
        
        case VID_GEO_FREQ_60HZ_MAX:
        {
            GEO_Nvs.freq.F60.upper.herz = Bounds_Check_Uint16(intData,TUNE_GEO_FREQ_60HZ_UPPER_MIN,TUNE_GEO_FREQ_60HZ_UPPER_MAX);
            NVS_Write_Word(NVS_GEO_FREQ_60HZ_UPPER, GEO_Nvs.freq.F60.upper.herz,NVS_VALID_DEFAULT);
            break;
        }
        

		default:
			return ERROR_OUT_OF_RANGE_E;
	}    

    return ERROR_OK_E; 	
}
