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

#if !defined( HAL_ALARMS_H )
#define HAL_ALARMS_H


/*
	typedef enum 
	{
	    FC_TRIG_REV_POS                         =1,      // NEW  
        FC_TRIG_REV_NEG                         =2,      // NEW  
        FC_TRIG_LO_POS                          =3,      // NEW  TLP
        FC_TRIG_LO_NEG                          =4,      // NEW  TLN
        FC_TRIG_MAX_POS                         =5,      // NEW  TMP
        FC_TRIG_MAX_NEG                         =6,      // NEW  TMN
        FC_RANGE_POS                            =7,      // NEW   TRP
        FC_RANGE_NEG=8,      // NEW  TRN
        FC_ADC_SURGE                           =9,  
        FC_GEO_ZC_FREQ_LO                      =10,
        FC_GEO_ZC_FREQ_HI                      =11,
        FC_ACV_HI                              =12,
        FC_ACV_LO                              =13,
        FC_DCL_LO                              =14,
        FC_TEMP_HOT                            =15,
        FC_TMR5_POS                            =16,     // NEW
        FC_TMR5_NEG                            =17      // NEW

	} FAULTCODE;
*/

	typedef enum 
	{
	    FC_TRIG_REV_POS=    1,
        FC_TRIG_REV_NEG=    2,                 
        FC_TRIG_LO_POS=     3,
        FC_TRIG_LO_NEG=     4,
        FC_TRIG_MAX_POS=    5,
        FC_TRIG_MAX_NEG=    6,
        FC_RANGE_POS=       7,
        FC_RANGE_NEG=       8,
        FC_ADC_SURGE=       9,
        FC_GEO_ZC_FREQ_LO=  10,
        FC_GEO_ZC_FREQ_HI=  11,
        FC_ACV_HI=          12,
        FC_ACV_LO=          13,
        FC_TEMP_HOT=        14,
        FC_TMR5_POS=        15,
        FC_TMR5_NEG=        16,
        FC_DCV_LO=          17,
        FC_DCV_RIPPLE1=      18,
        FC_DCV_RIPPLE2=      19
        
	} FAULTCODE;
    

	#define ALARM_CLEAR                     ((AlarmState)0)
	#define ALARM_ALL                       ((AlarmState)0xFFFF)	
	
	#define ALARM_DISC			             ((AlarmState)BIT_0)
	#define ALARM_AC_GRID_HIGH	              ((AlarmState)BIT_1)
	#define ALARM_AC_GRID_LOW              ((AlarmState)BIT_2)
	#define ALARM_OVER_FREQUENCY            ((AlarmState)BIT_3)
	#define ALARM_UNDER_FREQUENCY           ((AlarmState)BIT_4)
	#define ALARM_ISLANDING_EVENT           ((AlarmState)BIT_5)
	#define ALARM_TEMP_HOT               ((AlarmState)BIT_6) 
	#define ALARM_TEMP_COLD              ((AlarmState)BIT_7) 
	#define ALARM_TEMP_RANGE				((AlarmState)BIT_8)		
	#define ALARM_DELAY                     ((AlarmState)BIT_9)		// 0200     // DGA 2022-08-24 Changed bit 9 from CONFIG_DATA_INVALID
	#define ALARM_DC_LINK_LOW      			((AlarmState)BIT_10)	// 0400
	#define ALARM_DC_LINK_HIGH      		((AlarmState)BIT_11)	// 0800
	#define ALARM_PHASE			        	((AlarmState)BIT_12)	// 1000
	#define ALARM_SURGE			         	((AlarmState)BIT_13)	// 2000
	#define ALARM_DIAGNOSTIC	      		((AlarmState)BIT_14)	// 4000
	#define ALARM_STOP			      		((AlarmState)BIT_15)	// 8000


	typedef union {
		struct 
		{
			unsigned disc:1;			// BIT 0	
			unsigned grid_high:1;		// BIT_1	  
			unsigned grid_low:1;		// BIT_2	  
			unsigned freq_high:1;		// BIT_3	  
			unsigned freq_low:1;		// BIT_4	  
			unsigned island:1;			// BIT_5
			unsigned temp_hot:1;		// BIT_6	
			unsigned temp_cold:1;		// BIT_7	
			unsigned temp_range:1;		// BIT_8	
			unsigned delay:1;           // BIT_9  
			unsigned dclink_low:1;		// BIT_10	
			unsigned dclink_high:1;		// BIT_11	
			unsigned phase:1;           // BIT_12
			unsigned surge:1;	        // BIT_13
			unsigned diagnostic:1;		// BIT_14	
			unsigned stop:1;			// BIT_15	
            
		};
		struct 
		{
			Uint16 value;
		};
	} Alarm_Fault_Bits;

    

	typedef struct 
	{
        Uint16              volts;
        Uint16              sample;
	} Fault_Analog_Item;
    
	typedef struct 
	{
        Uint8               code;
        Uint8               rip_calc;
        Uint16              sample_counter;
        Uint16              error_count;
        Uint16              error_limit;
        Uint16              watts;
        Fault_Analog_Item   run;
        Fault_Analog_Item   limit;
	} Fault_Info_Ripple;
    
	typedef struct 
	{
        Uint16              code1;
        Uint16              code2;
        Uint16              param1;
        Uint16              param2;
        Uint16              hertz_10x;
        Fault_Info_Ripple   ripple;
        Uint8               text[32];
        
	} Fault_Info_Block;
    
    
	typedef struct 
	{
        Uint16              freq_value;
        Alarm_Fault_Bits    alarm;
	} Freq_Test_Block;
   
	typedef struct 
	{
		Alarm_Fault_Bits    alarm;
   		Alarm_Fault_Bits    telem;
        Fault_Info_Block    info;
        Uint16              counter[NVS_FAULT_COUNTER_TOTAL];               
        #if defined(Uses_Console_Fault_Test)
        Freq_Test_Block     test;
        #endif

        
	} Alarm_Fault_Block;

	public volatile Alarm_Fault_Block fault;


    typedef Uint16 AlarmState;

    void HAL_Fault_Send_Report(Uint8 force);
	void HAL_Fault_Clear(AlarmState alarmBit);
	
    #define	HAL_Fault_Get()					fault.alarm.value
	#define	HAL_Fault_Start()				(fault.alarm.value & (ALARM_ALL - (ALARM_DISC+ALARM_DELAY) ))
    
    void HAL_Fault_Set(AlarmState mask, Uint16 error_code, Uint16 timer1, Uint16 timer2);
    


    #if defined(Uses_Console_Fault)
    Uint8  console_fault_parse_args(void);
    Uint16 console_fault_print_item(Uint8 item);
    void console_fault_print(void);
    char * console_fault_text(FAULTCODE code);
    #endif

    
    
    
    

#endif 


