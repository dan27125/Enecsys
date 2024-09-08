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

#if defined(Uses_Debug_DCV)

void debug_dclink_print_pv10X(Uint16 hvdc)
{
    printf("%u ", DCLINK_CALC_PVX10(hvdc));
}

void debug_dclink_print_pv(Uint16 hvdc)
{
    Uint16 pv10x = DCLINK_CALC_PVX10(hvdc);
    printf("%u.%u ", (pv10x/10), (pv10x %10));
}

void debug_dclink_print1()
{
    if (fault.alarm.value)
    {
//      printf("rcb=%x run=%d \n",ecb1.value, ecb1.run);
        return;
    }
    
    Uint16 amp;
    Uint16 volts;
            
//  volts = analog.pwm.avg.volts;
    volts = analog.pwm.avg10.volts;
    amp = ( (Uint32) pwm.watts*1000) / volts;
    
//  printf("%uk ",debug_dcf.adc_sample_cnt/1000);
    debug_dcf.adc_sample_cnt=0;

    #if defined(Uses_AMP_Lock)
    if (dcb.pwm.amp_lock)
    {
//      printf("%u ",dcb.pwm.amp_lock);
//      printf("%.3u%% %.3uw ",pwm.amp.scale,pwm.watts);
        printf("%.3u%% %u,%uw ",pwm.amp.scale,mppt.watts, pwm.watts);
    }
    else
    {
//      printf("%.3uw %.3u%% ",pwm.watts,pwm.amp.scale);
        printf("%u,%uw %.3u%% ",mppt.watts, pwm.watts,pwm.amp.scale);
    }
    
    if (dcb.pwm.clock_duty_lock)
    {
        printf("SQW:%d ",PWM_GET_OCX_DUTY(PWM_CLOCK_PRX,PWM_CLOCK_DUTY));
    }
    
    #else
    printf("%u,%uw %.3u%% ",mppt.watts, pwm.watts,pwm.amp.scale);
    #endif
    
    printf("%ua ",analog.pwm.avg10.sample);
    
    printf("%uv ",volts );

//  printf("%ua ",analog.pwm.avg10.sample);
//  printf("%u ", DCLINK_CALC_PVX10(analog.dc.avg.volts));
    
//  printf("I:%u ",amp);
//  printf("%um ",pwm.milliamps);
    printf("%um ",pwm.avg2.milliamps);
    
//  printf("RIP:%.2u,%uv ",analog.dc.ripple.dif.avg.sample,vdif);
    #if defined(Uses_PWM_Ripple_Fault)
//  pwm.rip.calc
    printf("RIP:%.2uv,%.2ua ",DCLINK_GET_VDC(analog.dc.ripple.dif.avg.sample),analog.dc.ripple.dif.avg.sample);
    printf("%.2uv,%.2ua ",DCLINK_GET_VDC(pwm.avg2.calc), pwm.avg2.calc);
    #endif
    
//  printf("C:%u,%u ",pwm.rip.calc,pwm.rip.calc50);
    
//  printf("%u,%u,%u,%u,%u ",pwm.rip.calc40,pwm.rip.calc30,pwm.rip.calc25,pwm.rip.calc20,pwm.rip.calc10);
    
//  printf("DCF:%d,%d,%d,%d ",pwm.dcf_state,pwm.dcf_pcent,pwm.dcf_ripple,pwm.dcf_fault_sample);
    
//  printf("%u,%u ",analog.dc.ripple.hi.pwm_sample, pwm.rip.dclink_fault_table[150]);

//  printf("%uf,%uc ",temperature.fahrenheit, temperature.celsius);
    
//  printf("LO:%u,%u ", analog.dc.ripple.lo.avg.sample,analog.dc.ripple.lo.avg.volts);
//  printf("HI:%u,%u ", analog.dc.ripple.hi.avg.sample,analog.dc.ripple.hi.avg.volts);
    
    
    printf("F:%u %u ", mppt.flutter.count, mppt.up.pcent);

    printf("\n");
    
}

void debug_dclink_print2()
{
    debug_dclink_print1();
}


void debug_dclink_print3()
{

}
    
void debug_dc_fault_print()
{
    
    if (debug_dcf.trigger_wattage_drop)
    {
        printf("*** WATT DROP:%u ***\n",debug_dcf.trigger_wattage_drop);
        debug_dcf.trigger_wattage_drop=0;
    }
/*    
    else if (debug_dcf.trigger_no_dcf)
    {
        printf("NO DCF WATTS:%u RIP:%d  WD:%d,%d DC:%d\n",pwm.watts, pwm.rip.calc, pwm.bit.wattage_drop_10, pwm.wattage_drop, debug_dcf.wattage_drop_cnt);
        debug_dcf.trigger_no_dcf=0;
        debug_dcf.wattage_drop_cnt=0;

    }
*/
    
    if (debug_dcf.trigger==0)
        return;
    
    printf("Fault %s,%u ",debug_dcf.file,debug_dcf.line);
    
    if (debug_dcf.sample_counter)
        printf("S:%.3u,%d ",debug_dcf.sample_counter,debug_dcf.error_count);
    
    printf("RL:%u,%u ",debug_dcf.run, debug_dcf.limit);
    
//  printf("%uw %uv %ui ",debug_dcf.watts,debug_dcf.volts,debug_dcf.milliamps);
    
    printf("%uw ",debug_dcf.watts);
    
    printf("R:%u ",debug_dcf.rip_calc);
    
    printf("C:%u ",debug_dcf.code);
    
    printf("\n");
    debug_dcf.line=0;   // reset 
    
    debug_dcf.print_count++;
    debug_dcf.trigger=0;
}

#if defined(Uses_PWM_Ripple_Fault)
void debug_dcv_sine_fault(char *file, Uint16 line, Uint16 code)
{
    if ((debug_dcf.trigger) || (debug_dcf.print_count >= 4))
        return;

    strcpy((char*)debug_dcf.file,file);
    debug_dcf.line = line;
    debug_dcf.sample_counter=pwm.sample_counter;
    
    debug_dcf.run = analog.dc.sample;
    debug_dcf.limit = pwm.rip.dclink_fault_table[pwm.sample_counter];
    debug_dcf.code = code;
    debug_dcf.error_count=pwm.sine_error_count;
    debug_dcf.watts = pwm.watts;
    debug_dcf.volts = analog.pwm.avg.volts;
    debug_dcf.milliamps = pwm.milliamps;
    debug_dcf.rip_run = analog.dc.ripple.dif.sample;
    debug_dcf.rip_calc = pwm.rip.calc;
    debug_dcf.trigger=1;
}

void debug_dcv_fault_assert(char *file, Uint16 line, Uint16 run, Uint16 limit)
{
/*    
    if ((debug_dcf.trigger) || (dcb3.fault==0))
        return;
    strcpy((char*)debug_dcf.file,file);
    debug_dcf.line = line;
    debug_dcf.run = run;
    debug_dcf.limit = limit;    
    debug_dcf.watts = pwm.watts;
    debug_dcf.volts = analog.pwm.avg.volts;
    debug_dcf.milliamps = pwm.milliamps;
    debug_dcf.rip_run = analog.dc.ripple.dif.sample;
    debug_dcf.rip_calc = pwm.rip.calc;
    debug_dcf.trigger=1;
*/
}
#endif

void debug_dclink_print()
{
    if (dcb3.dc1)
    {
        debug_dclink_print1();
    }
    if (dcb3.dc2)
    {
        debug_dclink_print2();

    }
    if (dcb3.dc3)
    {
        debug_dclink_print3();
    }
    
    /*
    printf("hot on:%d off:%d  dn:%d up:%d  T:%d\n",
            tune.temperature.hot_on, tune.temperature.hot_off,
            tune.temperature.mppt_dn, tune.temperature.mppt_up, 
            temperature.celsius
    );
    */

}
#if defined(Uses_Debug_DCV_Profile)
void debug_dcv_reset()
{
    memset( (void*) &debug_dcv,0,sizeof(debug_dcv));
}

void debug_dcv_set_telem(Uint16 state, Uint16 size)
{
    debug_dcv_reset();
    debug_dcv.start = state;
    debug_dcv.limit = DCV_DEBUG_DC_SAMPLE_SIZE;
    if (fault.alarm.value==0)
    {
        debug_dcv.bits.pending=1;
    }
    else
    {
        debug_dcv.start = 0;
        debug_dcv.bits.start=1;
        debug_dcv.bits.sample=1;
    }
    printf("DCV set TELEM %u %u\n", state, size);
}

void debug_dcv_start()
{
    printf("DCV %u start\n", debug_dcv.start);
}

void debug_dcv_sample_done()
{
    printf("DCV collected samples:%u\n", debug_dcv.count);
    debug_dcv.bits.write=1;
}

void debug_dcv_done()
{
    debug_dcv.bits.done=1;
    printf("DCV done\n");
}

	typedef struct 
	{
        Uint8           buf[50];
        Uint16          watts;
		Uint16          avg;
		Uint16          peak;
		Uint16          dip;
        Uint16          bits;
	} DCV_TELEM_INFO;


void debug_dcv_write_payload(Uint8 id)
{
    DCV_TELEM_INFO  dct;
    Uint16 i, state,ndx;
    memset((void*) &dct,0,sizeof(dct));
    ndx = (id * 50);
    state = (debug_dcv.start + ndx) % 200;
    
//    printf("id=%u ndx=%u state=%u\n",id,ndx,state);
    
    for (i = 0; i < 50;i++)
    {
        Uint16 adc,dc1,dc2;
        adc = debug_dcv.sample[ndx+i];
        dc1 = DCLINK_GET_VDC( adc );
        dc2 = (dc1 & ~0xFF00);
        dct.buf[i] = dc2;
    }
    
    if (id==0)
    {
        dct.watts=mppt.watts;
        dct.avg=analog.dc.avg.volts;
        dct.dip=analog.dc.ripple.lo.pwm_sample;
        dct.peak=analog.dc.ripple.hi.pwm_sample;
        
        if (debug_dcv.bits.ufb_disabled)
            dct.bits = 1;
        else
            dct.bits = 0;
        printf("WATTS:%u  AVG:%u  DIP:%u  PEAK:%u\n",dct.watts,dct.avg,dct.dip,dct.peak);
        ZNet_P24_RPC_TX(ZNET_P24_RPC_DC_TELEM1,state,id, (Uint8 *) &dct, sizeof(dct));
    }
    else
        ZNet_P24_RPC_TX(ZNET_P24_RPC_DC_TELEM1,state,id, (Uint8 *) &dct.buf, 50);
        
    
}

void debug_dcv_write()
{
    debug_dcv_write_payload(debug_dcv.write_cnt);
    if (++debug_dcv.write_cnt >= 4)
    {
        debug_dcv.bits.write=0;
        debug_dcv_done();
    }
}

#endif

void debug_dcv_task_ms100()
{
    if (debug_dcf.print_count)
    {
        debug_dcf.print_count--;
    }
    
/*    
    if (debug_dcf.trigger)
    {
        if (++debug_dcf.trigger >= 2)
        {
            debug_dcf.trigger = 0;
        }
    }
*/

    #if defined(Uses_Debug_DCV_Profile)
    if (debug_dcv.bits.write)
    {
        debug_dcv_write();
    }
    #endif


/*    
    if (dcb3.dc4)
    {
        debug_dclink_print4();
    }
*/
    
}

void debug_dcv_task()
{
    #if defined(Uses_Debug_DCV_Profile)
    if (debug_dcv.bits.start)
    {
        debug_dcv_start();
        debug_dcv.bits.start=0;
    }
    if (debug_dcv.bits.sample_done)
    {
        debug_dcv_sample_done();
        debug_dcv.bits.sample_done=0;
    }
    #endif
  
    #if defined(Uses_PWM_Ripple_Fault)
    debug_dc_fault_print();
    #endif
    
/*    
    if (debug_dcf.fault==1)
    {
    }
*/
    
}


#endif



