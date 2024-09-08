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


    #if !defined( _HAL_BUCK_PWM_H )
	#define	_HAL_BUCK_PWM_H

	#define	Uses_Sine_Table2			1

	#if defined(SMI240_60)
	#define	WATT_TABLE_MAX				340
    #define	PWM_WATT_SCALE_BEG_ADC      500
    #define	PWM_WATT_SCALE_MAX			30
    #define	PWM_WATT_SCALE_END_ADC      790
	#endif

	#if defined(SMI360_72)
	#define	WATT_TABLE_MAX				480
    #define	PWM_WATT_SCALE_BEG_ADC      420
    #define	PWM_WATT_SCALE_MAX			32
    #define	PWM_WATT_SCALE_END_ADC      730
	#endif

	#if defined(SMI480_60)
	#define	WATT_TABLE_MAX				600
	#endif


    #define Uses_PWM_Telem

	#if defined(DOCUMENTATION)
	12,000,000 / 120 = 100,000
	#endif

//	#define	PWM_CLOCK_PRX_400K				29	// 12K/400-1
	#define	PWM_CLOCK_PRX_200K				59	// 12000000/59=200000
    #define	PWM_CLOCK_PRX_100K				119	// 12K/200-1=60
            
    #define	PWM_CLOCK_DUTY99                PWM_CLOCK_PRX_200K-1

/*            
    #define	PWM_CLOCK_DUTY98                PWM_CLOCK_PRX_200K-2
    #define	PWM_CLOCK_DUTY95                56
    #define	PWM_CLOCK_DUTY90                53
    #define	PWM_CLOCK_DUTY75                45
    #define	PWM_CLOCK_DUTY50                30
    #define	PWM_CLOCK_DUTY25                15
    #define	PWM_CLOCK_DUTY10                6
    #define	PWM_CLOCK_DUTY05                3
*/
            
	#define	PWM_SINE_PRX_46K                255		// 12000000/255=47,058
	#define SINE_TABLE_LENGTH               200		// 200

//  50HZ=20,000
//  60HZ=24,000
    #define PWM_TIMER_PR4_600HZ             (500)	// 196x120=24,000  12M/=500     60HZ
    #define PWM_TIMER_PR4_588HZ             (510)	// 196x120=23,520  12M/=510     60HZ-2%
            
            
	#define PWM_TIMER_PR4_490HZ             (631)	// 190x100=19,000  12m/=631     50HZ-2%

            
//  200-2%=196  -3%:194 -4%:192
//	#define SINE_TIMER_PR4_60HZ             (500)	// 200x120=24,000  12M/=500
//	#define SINE_TABLE_PR4_50HZ             (600)	// 200x120=20,000  12M/=600
//  #define SINE_TIMER_PR4_60HZ             (526)	// 190x120=22,800  12M/=526
//    #define SINE_TIMER_PR4_60HZ             (510)	// 196x120=23,520  12M/=510
//	#define SINE_TIMER_PR4_50HZ             (631)	// 200x120=19,000  12M/=631
//	#define SINE_TIMER_PR4_50HZ             (631)	// 190x100=19,000  12m/=631
            
    #define SINE_TIMER_PR4_60HZ             PWM_TIMER_PR4_588HZ
//  #define SINE_TIMER_PR4_60HZ             PWM_TIMER_PR4_600HZ
	#define SINE_TIMER_PR4_50HZ             PWM_TIMER_PR4_490HZ
            
	extern Uint16 pwm_sine_table[SINE_TABLE_LENGTH];
	extern Uint16 pwm_watt_table[WATT_TABLE_MAX];    
    
    
   	typedef enum 
	{
	    DAMAGE_RISK_LOW=    0,
        DAMAGE_RISK_MED=    1,
        DAMAGE_RISK_HIGH=   2                
	} PWM_FAULT_DAMAGE_RISK;

        
   	typedef struct 
	{
		Uint16 cnt;
		Uint32 sum;
		Uint16 reg;
		Uint16 num;
	} PWM_AVG_REG2;
	
   	typedef struct 
	{
        PWM_AVG_REG2 ocx1;
        PWM_AVG_REG2 ocx2;
		
		Uint16 cnt_rpt;
		
	} PWM_AVG_REG;
	
   	typedef struct 
	{
		Uint16 enable;
        Uint16 delay;
	} BUCK_CONTROL_BLOCK;
	
   	typedef struct 
	{
		Uint16 ocx;
		Uint16 prx;		
	} PWM_CLOCK_CONTROL;

   	typedef struct 
	{
		Uint16 watts;
		Uint16 scale;
	} AMPLITUDE_CONTROL_BLOCK;
    
   	typedef struct 
	{
        volatile Uint16     calc;
        volatile Uint16     calc50;
        volatile Uint16     div;
        volatile Uint16     inhibit_counter;
        
        volatile Uint16     dclink_fault_table[SINE_TABLE_LENGTH];
        
        
        
        
        
	} PWM_DC_RIPPLE_BLOCK;
    
	typedef union {
		struct 
		{
            unsigned drive_trigger:1; 
            unsigned buck_active:1; 
            unsigned full_cycle:1; 
            unsigned dc_over_420v:1; 
            unsigned dc_fault_check:1; 
            unsigned wattage_drop_10:1; 
            unsigned wattage_drop_50:1;
		};

		struct {
			unsigned int value;
		};
        
	} PWM_Control_Bits;


    #if defined(Uses_Debug_DCV)
   	typedef struct 
	{
        volatile Uint16     cnt;
        volatile Uint16     sum1;
        volatile Uint16     sum2;
        volatile Uint16     milliamps;
        volatile Uint16     calc;
	} PWM_DEBUG_AVERAGE;
    #endif
	
	typedef struct 
	{
	    #if defined(Uses_PWM_Telem)
        volatile PWM_AVG_REG avg;
	    #endif
        
        #if defined(Uses_Debug_DCV)
        Uint8  dcf_state;
        Uint8  dcf_pcent;
        Uint16  dcf_ripple;
        Uint16  dcf_fault_sample;
        #endif

        volatile PWM_Control_Bits               bit;
		volatile BUCK_CONTROL_BLOCK             buck;
		volatile PWM_CLOCK_CONTROL              clock;
        
		volatile AMPLITUDE_CONTROL_BLOCK		amp;
        
        volatile AMPLITUDE_CONTROL_BLOCK		start_amp;
        
		volatile Uint16                         sample_counter;

        volatile Uint16                         fault_error_count;
        volatile Uint16                         sine_error_count;
        volatile Uint16                         sine_error_limit;
        
        volatile Uint16                         watts;
        volatile Uint16                         watts_scale;
        
        volatile Uint16                         milliamps;
        
        #if defined(Uses_Debug_DCV)
        volatile PWM_DEBUG_AVERAGE              avg2;
        #endif
        
        volatile Uint16                         wattage_drop;
        volatile Uint16                         wattage_drop_;
        

        
        #if defined(Uses_PWM_Ripple_Fault)
        PWM_DC_RIPPLE_BLOCK                     rip;
        #endif
		
	} PWM_Control_Block;

	public volatile PWM_Control_Block pwm;
        
	#define getPowerLimit()		pwm.amp.limit

	#define	TIMER2	TMR2
	#define	TIMER3	TMR3
	#define	TIMER4	TMR4
	
	void HAL_PWM_Init();
	void PWM_Telem_Calc_Avg();
	void PWM_ZC_ISR();
    void PWM_Buck_Drive_Control();
    void PWM_Buck_Drive_Task();
    
    void PWM_Run_Task();
	
	Uint16 PWM_Grid_Scale(Uint16 value);
	Uint16 PWM_Grid_LO_Scale_UP(Uint16 value);
	Uint16 PWM_Grid_LO_Scale_DN(Uint16 value);
	Uint8 PWM_Get_Grid_Voltage_Scale();

	Uint16 PWM_Get_Amplitude_Scale(Uint16 watts);
	Uint16 PWM_Get_Wattage_Scale(Uint16 amplitude);

	Uint16 PWM_Amplitude_Calc(Uint16 watts);
	Uint16 PWM_Get_Amplitude_Limit(Uint16 watts);

		
	#define	PWM_CALC_OCX_DUTY(aduty,aprx)	(((aduty * aprx) / 100))
    
	#define	PWM_CALC_PRX_KHZ(khz)		((Uint16) 12000 / (khz+1))
	#define	PWM_CALC_PRX_MHZ(prx)		((Uint32) 12000000 / (prx+1))

    
    Uint16 PWM_GET_OCX_DUTY10X(Uint16 prx, Uint16 ocx);
	Uint8 PWM_GET_OCX_DUTY(Uint16 prx, Uint16 ocx);
    void PWM_Clock_Duty_Set(Uint8 duty);
    
	Uint16 PWM_Clock_Get_Sine_OCX(Uint8 astate);
	void PWM_Power_Limit_Set(Uint16 watts);
	Uint16 PWM_Get_Amplitude_Watts(Uint16 amp);

	#if defined(Uses_Debug)
	void console_print_buck_pwm1();
	void console_print_buck_pwm2();
	void console_print_buck_pwm3();	
	void console_print_buck_pwm3();	

	#if defined(Uses_Clock_Lock)
	void PWM_Console_Clock_Freq_Set(Uint16 frq_khz);
	void PWM_Console_Clock_Duty_Set(Uint8 duty);
	#endif
	
	void PWM_Power_Limit_UP(Uint16 x);
	void PWM_Power_Limit_DN(Uint16 x);	
	void PWM_Test2(Uint16 a);
	
	#endif

    #if defined(Uses_AMP_Lock)    
   	void PWM_Mode(Uint16 a);	
	void PWM_Test1(Uint16 a);
   	void PWM_Console_Amp_Lock_Set(Uint16 amp);
    #endif
        

#endif  // _HAL_BUCK_PWM_H