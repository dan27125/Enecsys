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

    #define Uses_MPPT_Debug                 1
//    #define Uses_MPPT_Print_Track           1
//  #define Uses_MPPT_Print_Ramp           0

//  #define Uses_MPPT_Print_Flutter          0


#if !defined(_APP_MPPT_H)
#define _APP_MPPT_H

	typedef union 
	{
		struct 
		{
            unsigned debug:1;
			unsigned dclink_sample:1;
			unsigned dclink_min:1;
			unsigned sec:1;
   			unsigned start:1;     // True at reset and while sample state machine busy, False when not
            unsigned run:1;       // True after Start state machine delay and samples 100% complete 
            unsigned track_refresh:1; 
		};

		struct {
			unsigned int value;
		};
	} MPPT_Bits1;

    
	typedef struct 
	{
		Uint16		volt;
	} MPPT_DCLINK_BLOCK;

	typedef struct 
	{
        Uint32			sample;
        Uint16			pvdc;
        Uint16			hvdc;

	} MPPT_OPEN_ITEM;

	typedef struct 
	{
        Uint16			sample_cnt;
		Uint32			hvdc_sum;
        Uint32			sample_sum;
        Uint16			pvdc;
        Uint16			hvdc;
        Uint16			sample;
        Uint16			timer;
        Uint16			time_limit;
	} MPPT_START_ITEM;

    
	typedef struct 
	{
		Uint16			count;
		Uint16			telem;
        Uint16          pcent;
	} MPPT_TRACK_ITEM;

  
    enum FlutterState
    {
        FLUTTER_NONE = 0,
        FLUTTER_LOW = 1,
        FLUTTER_MEDIUM = 2,
        FLUTTER_HIGH = 3
    };
    
	typedef struct 
	{
		Uint16              count;        
        Uint16              time_off;
        enum FlutterState   state;
        Uint16              limit;
	} MPPT_TRACK_FLUTTER;

	typedef struct 
	{
        Uint16				watts;
        Uint16				count;
	} MPPT_TRACK_HISTORY;
	
	typedef struct 
	{
		MPPT_Bits1			bit;		// DGA - parameter bit map field 

		volatile Uint16		watts;		// DGA sets wattage for buck drive
        
		Uint16				hvdc;		// DGA High Voltage DC
		
		MPPT_TRACK_ITEM		up;			// DGA Track UP parameters 
		MPPT_TRACK_ITEM		dn;         // DGA Track DN parameters 
        MPPT_TRACK_FLUTTER  flutter;       // flutter 
        MPPT_TRACK_HISTORY  history;
		
		#if defined(Uses_MPPT_Debug)
		Uint16				print_cnt;
		#endif
 
        MPPT_OPEN_ITEM      open;
        MPPT_START_ITEM     start;
        
        Uint16				dn_sample;      // DGA Must set via DCLINK_GET_ADC();
        Uint16				dn_sample_cnt;  // 
        
        Uint16				up_sample;      // DGA Must set via DCLINK_GET_ADC();
        Uint16				limit_pcent;	// 
        
	} MPP_TRACKER_CONTROL_BLOCK;
    
	
	volatile public MPP_TRACKER_CONTROL_BLOCK mppt;
		
	void MPPT_Init();
	void MPPT_Start(Uint8 opts);
	void MPPT_Calc_Start();
    void MPPT_Calc_Power_Limit();
    void MPPT_Calc_Run();
	void MPPT_Run_Ms1000();	
	void MPPT_Task();

	#if defined(Uses_MPPT_Console)
	Uint8 MPPT_console_parse_args();
	void  MPPT_console_print_config();
    void MPPT_print_config();
	void MPPT_print_pv(Uint16 num);
	void MPPT_print_str_pv(char *s1, Uint16 num);
	#endif
	
	#if defined(Uses_MPPT_Print_Tracking)
	void MPPT_console_print_tracking();
	#endif
	
	
	#define	MPPT_MIN_WATTS		5

	#define	MPPT_READ_NVS			0x01
	#define	MPPT_PRINT_PARAMS		0x02

    #define	MPPT_RAMP_LIMIT_PCENT_NORMAL		95
    #define	MPPT_RAMP_LIMIT_PCENT_FLUT_MED		90
    #define	MPPT_RAMP_LIMIT_PCENT_FLUT_HIGH		80

        
    #define MPPT_RUN_TIMER_LIMIT                (60*15)
    

    #define CALC_RAMP_UP_PERCENT(__hvdc, __ramp)  ( ((Uint32) __ramp * 1000) / __hvdc)
    #define CALC_RAMP_DN_PERCENT(__hvdc, __ramp)  ( ((Uint32) __hvdc * 1000) / __ramp)
		

#endif	// _APP_MPPT_H