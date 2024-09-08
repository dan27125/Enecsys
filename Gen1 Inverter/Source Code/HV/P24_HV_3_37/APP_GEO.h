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

#ifndef _GEO_H
#define _GEO_H


	#if defined(DOCUMENTATION)
    //  Timer1 is running at 1.5 MHZ  1,500,000         / 2=750,000   * 10=7,500,000
    //  7500000 can be used to calculate the timer value from Grid frequency x10 
    // Example: Grid Freq 50HZ is expressed as 500 and 60HZ is 600
    // 7500000 / 500 = 15,000
    // 7500000 / 600 = 12,500        
	#endif


    #define GEO_FREQ_LOCK_NUM_SAMPLES               100
    #define GEO_FREQ_CYCLE_RESET_CNT                200
	#define GEO_PHASE_SAMPLE_MIN                    40
	#define GEO_PHASE_ERROR_MAX                     5
	#define GEO_PHASE_LOCK_MIN                      10

	#define NO_AC_TIMEOUT_MS_50HZ					(24)	// 1m/50=20,000 x 1.2 = 24000 us
	#define NO_AC_TIMEOUT_MS_60HZ					(20)	// 1m/60=16,666 x 1.2 = 19999 us

	#define DC_LINK_SAMPLE_TIME_50HZ				(30)		// 6000/50=120/4=30	
	#define DC_LINK_SAMPLE_TIME_60HZ				(25)		// 6000/60=100/4=25

	#define CONSOLE_RUN_LIMIT_NORMAL            (20)
    #define CONSOLE_RUN_LIMIT_ENGINEERING       (60000)

    #define TIMER1_FREQ_DIVISOR     ((Uint32)7500000)     // 1,500,000 /2=750,000   * 10=7,500,000
    #define TIMER1_CALC_10X(_freq_10x)              (Uint16) ( (Uint32) TIMER1_FREQ_DIVISOR / _freq_10x)

    #define TIMER1_ADC_LOWER_LIMIT_60HZ       300       // 89   PHASE POS:60,88 NEG:60,12765 
    #define TIMER1_ADC_LOWER_LIMIT_50HZ       360

    #define TIMER1_50HZ             15000           // 7,500,000 / 500 = 15,000
    #define TIMER1_60HZ             12500           // 7,500,000 / 600 = 12,500
    #define TIMER1_MAX              TIMER1_50HZ*2   // TIMER1_50HZ
    #define TIMER1_INT1_FILTER      400             // 12500-10%=

    #if defined(DOC)
    POS:125,288,A:209 NEG:119,331,A:214,12502    
    #endif

    #define TIMER1_ADC_TRIG_POS_LOWER_60HZ        20
    #define TIMER1_ADC_TRIG_POS_UPPER_60HZ        625   // 400   12500*.05=625   12500*.04=500

    //  Negative phase margin must be higher/wider because of dependency on last ZC timer value needing to be added 
    #define TIMER1_ADC_TRIG_NEG_LOWER_60HZ        20    // 50 Neg phase 
    #define TIMER1_ADC_TRIG_NEG_UPPER_60HZ        625   // 12500*.05=625   12500*.04=500

    #define TIMER1_ADC_TRIG_POS_LOWER_50HZ        45
    #define TIMER1_ADC_TRIG_POS_UPPER_50HZ        750   // 15000*.05=750 
    #define TIMER1_ADC_TRIG_NEG_LOWER_50HZ        45
    #define TIMER1_ADC_TRIG_NEG_UPPER_50HZ        500

	#define	Uses_GEO_USA			1
	#define	Uses_GEO_USA2			1
	#define	Uses_GEO_UK				1

	#define CONNECTION_TIME_ENGINEERING		  		GET_CONNECTION_DELAY_SECCONDS(1)
	#define GET_CONNECTION_DELAY_SECCONDS(_sec)     ((Uint16)(_sec * 10))	// 100 ms increments 

//    #define DEFAULT_START_OVER_VOLTAGE_LIMIT            265
//    #define DEFAULT_RUN_OVER_VOLTAGE_LIMIT              268

    #define DEFAULT_START_OVER_VOLTAGE_LIMIT            275
    #define DEFAULT_RUN_OVER_VOLTAGE_LIMIT              278
    
    
    #define DEFAULT_60HZ_START_OVER_VOLTAGE_LIMIT		DEFAULT_START_OVER_VOLTAGE_LIMIT
    #define DEFAULT_60HZ_RUN_OVER_VOLTAGE_LIMIT			DEFAULT_RUN_OVER_VOLTAGE_LIMIT

    #define DEFAULT_60HZ_START_UNDER_VOLTAGE_LIMIT		100     // 200
    #define DEFAULT_60HZ_RUN_UNDER_VOLTAGE_LIMIT		95      // 195
    
    #define DEFAULT_50HZ_START_OVER_VOLTAGE_LIMIT		DEFAULT_START_OVER_VOLTAGE_LIMIT
    #define DEFAULT_50HZ_START_UNDER_VOLTAGE_LIMIT		190
	#define DEFAULT_50HZ_RUN_OVER_VOLTAGE_LIMIT			DEFAULT_RUN_OVER_VOLTAGE_LIMIT
    #define DEFAULT_50HZ_RUN_UNDER_VOLTAGE_LIMIT		180

//	*************** 60 HZ ***************
	#if defined(Uses_GEO_USA)
	#define USA_STARTING_OVER_VOLTAGE_LIMIT			DEFAULT_60HZ_START_OVER_VOLTAGE_LIMIT
    #define USA_STARTING_UNDER_VOLTAGE_LIMIT		DEFAULT_60HZ_START_UNDER_VOLTAGE_LIMIT
    #define USA_RUNNING_OVER_VOLTAGE_LIMIT			DEFAULT_60HZ_RUN_OVER_VOLTAGE_LIMIT
    #define USA_RUNNING_UNDER_VOLTAGE_LIMIT			DEFAULT_60HZ_RUN_UNDER_VOLTAGE_LIMIT
    #define USA_CONNECTION_TIME             		GET_CONNECTION_DELAY_SECCONDS(300)
	#endif

	#if defined(Uses_GEO_USA2)
	#define USA2_STARTING_OVER_VOLTAGE_LIMIT		DEFAULT_60HZ_START_OVER_VOLTAGE_LIMIT
    #define USA2_STARTING_UNDER_VOLTAGE_LIMIT		DEFAULT_60HZ_START_UNDER_VOLTAGE_LIMIT
    #define USA2_RUNNING_OVER_VOLTAGE_LIMIT			DEFAULT_60HZ_RUN_OVER_VOLTAGE_LIMIT
    #define USA2_RUNNING_UNDER_VOLTAGE_LIMIT		DEFAULT_60HZ_RUN_UNDER_VOLTAGE_LIMIT
    #define USA2_CONNECTION_TIME             		GET_CONNECTION_DELAY_SECCONDS(30)
	#endif


//	*************** 50 HZ ***************
	#if defined(Uses_GEO_UK)
	#define UK_STARTING_OVER_VOLTAGE_LIMIT			DEFAULT_50HZ_START_OVER_VOLTAGE_LIMIT
    #define UK_STARTING_UNDER_VOLTAGE_LIMIT         DEFAULT_50HZ_START_UNDER_VOLTAGE_LIMIT
    #define UK_RUNNING_OVER_VOLTAGE_LIMIT			DEFAULT_50HZ_RUN_OVER_VOLTAGE_LIMIT
    #define UK_RUNNING_UNDER_VOLTAGE_LIMIT			DEFAULT_50HZ_RUN_UNDER_VOLTAGE_LIMIT
    #define UK_CONNECTION_TIME                      GET_CONNECTION_DELAY_SECCONDS(182)
	#endif
//	*************** 50 HZ ***************


	typedef union {
		struct 
		{
			unsigned print_timer	:1;
		};

		struct {
			unsigned int value;
		};
	} GEO_Control_Bits1;

	typedef struct
	{
		Uint16  grid_start_over;
		Uint16  grid_start_under;
		Uint16  grid_run_over;
		Uint16  grid_run_under;
        
		Uint16  connection_time;             // Units of 100 ms
	} GEO_LIMITS_DATA;
    
	typedef struct
	{
		Uint16  lower;
        Uint16  upper;
        
		Uint16  lower_min;
        Uint16  lower_max;
        
	} PHASE_TIMER_LIMIT;
    
	typedef struct
	{
		Uint16  herz;
        Uint16  tmr1;
        Uint16  tmr2;
	} FREQ_LIMIT_LOWER;

	typedef struct
	{
		Uint16  herz;
        Uint16  tmr1;
	} FREQ_LIMIT_UPPER;
    
	typedef struct
	{
		Uint16  lower;
        Uint16  upper;
	} PHASE_TRIG_ITEM;
    
	typedef struct
	{
        PHASE_TRIG_ITEM    pos;
        PHASE_TRIG_ITEM    neg;
	} PHASE_TRIG_LIMIT;
    
	typedef struct
	{
        PHASE_TRIG_LIMIT    trig;
        FREQ_LIMIT_LOWER    lower;
        FREQ_LIMIT_UPPER    upper;
	} FREQ_LIMIT_ITEM2;
    
	typedef struct
	{
        FREQ_LIMIT_ITEM2    F50;
        FREQ_LIMIT_ITEM2    F60;
        FREQ_LIMIT_ITEM2    limit;
	} GEO_NVS_FREQ_LIMITS;

	typedef struct
	{
        GEO_NVS_FREQ_LIMITS     freq;
        GEO_Control_Bits1       bit;
        Uint16                  filter;
	} GEO_NVS_DATA;
    	
	typedef enum
	{
		GEO_FACTORY  = 0,
		GEO_USA      = 1,                
		GEO_GREECE   = 30,
		GEO_FRANCE   = 33,
		GEO_SPAIN    = 34,
		GEO_ITALY    = 39,
		GEO_UK       = 44,
		GEO_GERMANY  = 49,
		GEO_INDIA    = 91, 
		GEO_USA2    = 1314,
        GEO_HAWAII  = 1808
	} GEO_CODE; 

	typedef struct
	{
		Uint8   grid_freq;
        Uint8   grid_freq2x;
		Uint16  no_ac_timeout;
		Uint16  dclink_sample_time;
		Uint16  sine_pr4;
		Uint16  rfu;
	} GEO_DATA_BLOCK;
	
	public volatile GEO_DATA_BLOCK		GEO_Config;
	public volatile GEO_LIMITS_DATA		GEO_Limits;
    public volatile GEO_NVS_DATA		GEO_Nvs;
    public volatile Uint8               mEngineeringMode;
        
    	typedef union {
		struct 
		{
			unsigned trig_positive0:1;
            unsigned trig_positive1:1;
            unsigned trig_negative0:1;
            unsigned trig_negative1:1;
            unsigned trig_alternate:1;
		};

		struct {
			unsigned int value;
		};
	} Phase_Control_Bits;
        
	typedef struct 
	{
		Uint16 negative_cnt;
		Uint16 positive_cnt;
		Uint8 flag;
	} Phase_Sample_Block;
	
	typedef struct 
	{
		Phase_Sample_Block new;
		Phase_Sample_Block old;
		Phase_Sample_Block dir;
		Uint16 lock_cnt;
		Uint16 error_cnt;
		Uint16 error_neg;
		Uint16 error_pos;
		Uint8 lock_flag;
		Uint16 ufb_error_cnt;
        Uint16 pwm_error_cnt;
        
        Phase_Control_Bits  bit;
        
	} Phase_Control_Block;
    
	typedef union {
		struct 
		{
            unsigned lock  :1;
			unsigned zc_sample  :1;
            unsigned zc_average  :1;
            unsigned phase_sample:1;
            unsigned phase_average:1;  
			unsigned rfu :10;
		};

		struct {
			unsigned int value;
		};

	} Frequency_Counter_Bits;

	typedef struct 
	{
        Uint16 cnt;
        Uint16 hz;
        Uint16 lock;
	} Frequency_Start_Block;
  
	typedef struct 
	{
   		Uint16 cycle_cnt;
        Uint32 cycle_sum;
        Uint16 instant;
        Uint16 average;
       
        Uint16 cycle_cnt_low;
        Uint16 cycle_cnt_high; 
        
        Uint16 tmr1; 
        Uint16 tmr2;

        Frequency_Start_Block   start;
        
	} Frequency_Counter_Block_ZC;

	typedef struct 
	{
   		Uint16 cycle_cnt;
        Uint32 cycle_sum;
        Uint16 instant;
        Uint16 avg20;
        Uint16 cycle_cnt_low;
        Uint16 cycle_cnt_high; 
        Uint16 tmr1; 
        Frequency_Start_Block   start;
	} Frequency_Counter_Block_Phase;
    
	typedef struct 
	{
        Frequency_Counter_Bits          bit;
        Frequency_Counter_Block_ZC      zc;
        Frequency_Counter_Block_Phase   phase;
        Uint16 fault_10x; 
	} Frequency_Control_Block;
	
	public volatile Phase_Control_Block phase;
	public volatile Frequency_Control_Block GEO_freq;
        
        
        
	void GEO_ZC_ISR(void);
	void GEO_ZC_Reset(void);
        
        
    void GEO_Set(void);
    void GEO_freq_calc_timers(void);
    
    #if defined(Uses_Debug_NVS)	
    void GEO_Print(void);
    #endif
    
    #if defined(Uses_GEO_Console)
    Uint8 GEO_console_parse_args(void);
    #endif

#endif // ifndef _GEO_H
