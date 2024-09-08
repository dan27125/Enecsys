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


#if !defined(  HAL_CONSOLE_DEBUG_H )
#define	HAL_CONSOLE_DEBUG_H

	#include "APP_Inverter.h"


	#define	print_source()	printf(" %s,%d\n",__FILE__,__LINE__)	

	#if defined(Uses_Debug) || defined(Uses_AMP_Lock)

	typedef union {
		struct 
		{
		  unsigned pic24_jst_debug  :1;
		  unsigned pic24_em250_debug  :1;
		  unsigned pic24_em250_wdt_debug  :1;
		  unsigned pic24_serial_proto_debug  :1;
			unsigned pic24_wimesh_debug  :1;		  
            unsigned run_state	:1;
			unsigned pwm1	:1;
			unsigned pwm2	:1;			
			unsigned pwm3	:1;	
            unsigned phase_trig:1;
            unsigned phase_range:1;
            unsigned phase_isr_error:1;
            unsigned rcp	:1;	
		};

		struct {
			unsigned int value;
		};
	} Pic24_Debug_Control_Bits1;

	typedef union {
		struct 
		{
			unsigned mppt	:1;
//			unsigned acv	:1;	
            unsigned acw	:1;
            unsigned eff	:1;
			unsigned dclimits:1;
			unsigned temperature:1;	
			unsigned start_logic:1;
			unsigned power:1;
			unsigned kwh:1;
			unsigned scale:1;
       		unsigned freq:1;
            unsigned spi_status:1;
            unsigned spi_debug:1;
   			unsigned adc_all	:1;
//          unsigned adc_ac	:1;
            unsigned adc_geo	:1;            
		};

		struct {
			unsigned int value;
		};
	} Pic24_Debug_Control_Bits2;

    
	typedef union {
		struct 
		{
            unsigned dc1	:1;	
            unsigned dc2	:1;	
            unsigned dc3	:1;
            unsigned fault	:1;
            unsigned acv	:1;
            unsigned acv2	:1;
		};

		struct {
			unsigned int value;
		};
	} Pic24_Debug_Control_Bits3;
    
	public volatile Pic24_Debug_Control_Bits1 dcb1;
	public volatile Pic24_Debug_Control_Bits2 dcb2;
    public volatile Pic24_Debug_Control_Bits3 dcb3;

        
	#if defined(Uses_Debug_UFB)
	typedef union {
		struct 
		{
			unsigned pending:1;
            unsigned active:1;
            unsigned done:1;
			unsigned sample:1;
            unsigned notify:1;
            unsigned limit:1;
		};

		struct {
			unsigned int value;
		};
	} UFB_Debug_Bits;
    
	typedef struct 
	{
        UFB_Debug_Bits  bits;
        Uint16          start;
        Uint16          count;
	} UFB_DEBUG_INFO;
    
    public volatile UFB_DEBUG_INFO  debug_ufb;
    void debug_ufb_toggle();
    void debug_ufb_set(Uint16 state);
    void debug_ufb_reset();
    void debug_ufb_task();
	#endif
  
    
#if defined(Uses_Debug_DCV_Profile)
	typedef union {
		struct 
		{
            unsigned start:1;
			unsigned pending:1;
            unsigned ufb_disabled:1;
			unsigned sample:1;
            unsigned sample_done:1;
            unsigned write:1;
            unsigned done:1;
		};

		struct {
			unsigned int value;
		};
	} DCV_Debug_Bits;
    
    
    #define     DCV_DEBUG_DC_SAMPLE_SIZE    200
    
	typedef struct 
	{
		Uint16          start;
		Uint16          count;
		Uint16          limit;
        DCV_Debug_Bits  bits;
        Uint16          sample[DCV_DEBUG_DC_SAMPLE_SIZE];
        Uint16          write_cnt;
        
	} DCB_DEBUG_INFO;
    
    public volatile DCB_DEBUG_INFO  debug_dcv;
#endif

       
        
    #if defined(Uses_Debug_DCV)
        
    void debug_dcv_set_telem(Uint16 state, Uint16 size);
    void debug_dcv_task();
    void debug_dcv_task_ms100();
    
    #if defined(Uses_PWM_Ripple_Fault)
    void debug_dcv_fault_assert(char *file, Uint16 line, Uint16 run, Uint16 limit);
    void debug_dcv_sine_fault(char *file, Uint16 line, Uint16 code);
    #endif
    

   	typedef struct 
	{
        Uint16     trigger;
        Uint16     trigger_wattage_drop;
        Uint16     wattage_drop_cnt;
        Uint16     trigger_no_dcf;
        Uint8      file[64];
        Uint16     line;
        Uint16     run;
        Uint16     limit;
        Uint16     watts;
        Uint16     volts;
        Uint16     milliamps;
        Uint16     rip_run;
        Uint16     rip_calc;
        Uint16     adc_sample;
        Uint16     sample_counter;
        Uint16     error_count;
        
        Uint16     adc_sample_cnt;
        
        Uint16     print_count;
        Uint16     code;

	} PWM_DEBUG_BLOCK;

    public volatile PWM_DEBUG_BLOCK  debug_dcf;
        
        
	#endif
    
    
	typedef struct 
	{
		Uint16 cnt;
		Uint16 timer;
	} ZC_ISR_DEBUG_INFO;

    
    	typedef struct 
	{
		Uint16 cnt;

        Uint32 timer_sum;
	} ZC_ISR_INFO;

    
	typedef struct 
	{
		ZC_ISR_DEBUG_INFO error1;
		ZC_ISR_DEBUG_INFO error2;
        
        Uint16 cnt;
        ZC_ISR_INFO pos;
        ZC_ISR_INFO neg;
        
	} ZERO_CROSS_DEBUG;

	typedef struct 
	{
		Uint16 cnt;
        Uint32 tmr;
        Uint32 tmr0;
        Uint16 tmin;
        Uint16 tmax;
        Uint16 range_min;
        Uint16 range_max;
	} PHASE_TRIG_DEBUG;

	typedef struct 
	{
        Uint16 pmin;
        Uint16 pmax;
        Uint16 nmin;
        Uint16 nmax;
	} PHASE_STATE_DEBUG;
	
	typedef struct 
	{
        PHASE_TRIG_DEBUG    pos;
        PHASE_TRIG_DEBUG    neg;
        Uint16              fault_code;
        Uint16              fault_timer1;
        Uint16              fault_timer2;
        
		Uint16 timer5_neg_cnt;
		Uint16 timer5_pos_cnt;
        
        Uint16 error;
        Uint16 busy;

        
	} PHASE_DEBUG;
    
	typedef struct 
	{
		Uint16 sample_cnt1;
		Uint16 sample_cnt2;        
	} ADC_DEBUG_TEMPERATUR;
    
    
	typedef struct 
	{
        Uint16      sample_cnt;
        Uint16      zc;
        Uint16      timeout;
	} ADC_DEBUG_AC;

    
	typedef struct 
	{
          Uint16 zc_isr_cnt;
		Uint16 sample_cnt;
	} ADC_DEBUG_DC;
    
    
    
	typedef struct 
	{
		ADC_DEBUG_TEMPERATUR temp;
        Uint16 cnt_isr1;
		Uint16 cnt_sample_1;
	} ADC_DEBUG;

	
	typedef struct 
	{
        Uint16 zc_isr_cnt;
        Uint16 phase_isr_cnt;
		Uint16 phase_pos;
		Uint16 phase_neg;
	} DIAG_INFO_BLOCK;
	
	typedef struct 
	{
/*        
		Uint16 cnt_accum_energy1;
		Uint16 cnt_accum_energy2;
		Uint16 cnt_dclink_flag;
	
		Uint16 sine_sum1;
		Uint16 sine_cnt1;

		Uint16 sine_sum2;
		Uint16 sine_cnt2;

		Uint16 sine_sum3;
		Uint16 sine_cnt3;
		
		Uint16 sine_sum4;
		Uint16 sine_cnt4;
*/	
		#if defined(Uses_Debug_DcLimits)
		Uint16 dcl_limits[32];
		#endif

		#if defined(Uses_Debug_MPPT)
		Uint16 mppt[100];
		#endif

        
        #if defined(Uses_Debug_DCV)
        ADC_DEBUG_DC    dc;
        #endif

        #if defined(Uses_Debug_ACV)
        ADC_DEBUG_AC    ac;
        #endif
        
    	#if defined(Uses_Debug_ADC)
		ADC_DEBUG analog;
        #endif
		
		Uint16 cnt_zc;
		Uint16 diag_zc_cnt;
		
		Uint16 bc1;
		
		#if defined(Uses_Debug_UFB)
		Uint8 ufb_pos;
		Uint8 ufb_neg;
		Uint8 ufb_off;
		#endif
		
//		#if defined(Uses_Debug_PWM) || defined(Uses_Debug_Phase)
//		Uint16 cnt_tmr4;
//		#endif

		ZERO_CROSS_DEBUG zc;
        
//        #if defined(Uses_Debug_ADC_DCV)
//        ADC_DEBUG_DC    dcl;
//        #endif
		
		#if defined(Uses_Diagnostics)
		DIAG_INFO_BLOCK diag;
		#endif

		#if defined(Uses_Debug_Phase)
		PHASE_DEBUG phase;
		#endif
		
	} Trace_Info_Block;
    
	public volatile Trace_Info_Block trace;
        
	typedef struct 
	{
		#if defined(Uses_Clock_Lock)
		Uint8 lock_clock_freq;
		Uint8 lock_clock_duty;
		#endif

		#if defined(Uses_AMP_Lock)
		Uint16 amp_lock;
        Uint16 clock_duty_lock;
		#endif

        #if defined(Uses_Debug_PWM2)
        Uint16 pwm1_lock;
        Uint16 pwm1_duty;
        Uint16 pwm2_duty;
        #endif
		
		Uint8 mode;
		Uint16 prx;
		Uint16 ocx;
		Uint8 buck_lock;
		Uint8 ufb_lock;		
	} Debug_PWM;

	typedef struct 
	{
		Uint16 timer1;
        Uint16 timer2;
        Uint16 trig_neg;
        Uint16 trig_pos;
        Uint16 trig_isr;
        Uint16 count;
	} Debug_INT1_Phase;
	
	typedef struct 
	{
		Debug_PWM pwm;

		Uint16 test_phase_error_sec;
        
        Debug_INT1_Phase    int1_phase;
		
		#if defined(Uses_Debug_MPPT)
		Debug_MPPT mppt;
		#endif
		
	} Debug_Control_Block;
	
	public volatile Debug_Control_Block dcb;
		
	typedef struct 
	{
		Uint16 test;
		Uint16 prx;
		Uint16 ocx;
		Uint16 ocx_max;
	} Buck_Control_Block;

    
    void Debug_Task();
    void Debug_Task_Ms100();
    void Debug_Task_Ms10();    
	void Debug_Initialize();
	Uint8 Debug_console_parse_args();
	void Debug_trace_cnt_clear();

    
	void console_print_list(Uint16* list, Uint8 size, char *desc);
	void console_print_list1(Uint16* list, Uint8 size);
	void console_print_mpp1();
	void console_print_mpp2();
	void console_print_mpp3();
	void console_print_acv_avg1();
	
	#if defined(Uses_Debug_DcLimits)
	void console_print_dclimits();
	#endif

	#if defined(Uses_Debug_Temperature)
	void console_print_temperature();
	#endif

	#if defined(Uses_Debug_MPPT)
	void console_print_mppt1();
	#endif
	
	
	#if defined(Uses_Debug_ADC)
	void console_print_adc1();
	#endif
	
    #if defined(Uses_Debug_ADC_PWM)
    void console_print_adc_sine_item(Uint8 num);
    public volatile Uint16 debug_adc_grid_sample;
    public volatile Uint16 debug_adc_grid_time;
    public volatile Uint16 debug_adc_grid_time;        
    public volatile Uint16 debug_pwm_adc[200]; 
    public volatile Uint16 debug_pwm_time1[200];
    public volatile Uint16 debug_pwm_time2[200];
    public Uint16 debug_print_adc_sine;
        
    #endif
    
	#endif

	
	#if defined(Uses_Debug_Phase)
	void console_print_phase();
	#endif

    #if defined(Uses_Debug_Start)
    void console_print_start_logic();
    #endif
    
    #if defined(Uses_ACV_Console)
    void console_print_float_test();
    #endif
    
    
    #if defined(Uses_Debug_DCV)
    void debug_dclink_print();
    #endif
      
    #if defined(Uses_Debug_ACV)
    void debug_acv_print1();
    #endif
    
    
    public Uint16 debug_print_tick_div;
   
	
#else	// not Uses_Debug
	
	#define	print_fault(x)	

#endif	// HAL_CONSOLE_DEBUG_H