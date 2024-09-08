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

/*
Sint16 debug_acv_sample_temp_adjust(Uint16 adc)
{
    Sint16 celsius_center;
    Uint16 celsius_adc_adj_scale;
    Sint16 celsius_adc_adj;
    celsius_center = temperature.celsius-5;
    celsius_adc_adj_scale = (tune.acv_div*10) / tune.acv_div2;
    celsius_adc_adj = (celsius_center * celsius_adc_adj_scale) / 1000;

    if (dcb3.acv2)
    {
        printf("%dc,%d ",temperature.celsius,celsius_center);
        printf("DIV:%d,%d,%d ",tune.acv_div,tune.acv_div2,celsius_adc_adj_scale);
//      printf("ADJ:%d,%dv ",celsius_adc_adj,ACV_PEAK_CALC_VOLTS10X(celsius_adc_adj));
        printf("\n");
    }
    
    return adc - celsius_adc_adj;
}
*/

#if defined(Uses_Debug) && defined(Uses_Debug_ACV)


#define ADC_TEMP_GET_MVDC(x)	(Uint16)(((Uint32) x * 323) / 100)


void debug_acv_print2()
{
    Uint16 mv = ADC_TEMP_GET_MVDC(temperature.sample);
    
    printf("TEMP ADC:%d MV:%d %df %dc ",temperature.sample, mv, temperature.fahrenheit,temperature.celsius);
    
    printf("\n");
}

void debug_acv_print1()
{
    
    if (dcb3.acv2)
        debug_acv_print2();
    
    if (fault.alarm.grid_low)
    {
        return;
    }
    
    printf("AC ");


 /*
    {
        Uint16 v1,v2,v3;

        #define	ACV_CALC_ADC2(volts)          ((((Sint32) volts*tune.acv_div) /100) - temperature.ac_vdrop)
        #define	ACV_CALC_ADC3(volts)          (((Sint32)(volts*tune.acv_div)/100) - temperature.ac_vdrop)
        
        v1 = ACV_CALC_ADC35(240);
        v2 = ACV_CALC_ADC2(240);
        v3 = ACV_CALC_ADC3(240);
        
        printf("%u,%u,%u ",v1,v2,v3);
        
    }
*/
    
    
//  printf("%uh ",trace.ac.zc);
    
    if (trace.ac.timeout)
        printf("TMO:%u ",trace.ac.timeout);
    
    printf("%u,%u,%u,%u ",analog.ac.cyc.volts, analog.ac.cyc.sample,
        analog.ac.avg.volts,analog.ac.avg.sample) ;
    
    printf("ADC:%d ", ACV_CALC_ADC(analog.ac.avg.volts) );
        
    printf("LIM:%d,%d ",analog.ac_fault.volts,analog.ac_fault.sample);
    
//  printf("%d ",ACV_CALC_ADC_PEAK(analog.ac_fault.volts) );
    
//   if (trace.ac.sample_cnt)
//        printf("SC:%u ",trace.ac.sample_cnt);
    
//  analog.grid_sample_fault = ACV_ADC_PEAK_FAULT;    
    
//  printf("TA:%d,%d ",ACV_PEAK_CALC_VOLTS10X(sample_adj), sample_adj );
    
//  temperature.ac_vdrop
    
    printf("%d,%df,%dc,%d ",temperature.sample, temperature.fahrenheit,temperature.celsius,temperature.ac_vdrop);
    
//  printf("%d,%d ",temperature.ac_scale_volts, temperature.ac_scale_sample);

//    if (trace.ac.zc)
//        printf("ZC:%u ",trace.ac.zc);
    
    //  analog.ac_fault.sample = ACV_CALC_ADC_PEAK(ACV_ADC_VOLT_FAULT);
    
    printf("\n");
}

#endif
