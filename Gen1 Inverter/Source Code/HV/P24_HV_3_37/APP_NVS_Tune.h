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

#if !defined(_SVC_SYSTEM_TUNING_H)
#define _SVC_SYSTEM_TUNING_H


	#define DC_LINK_FAULT_START			(299)	 
	#define DC_LINK_FAULT_RUN			(280)

	#if defined(SMI240_60)
//	#define TUNE_MPPT_RAMP_MIN_PV10X		240
//	#define TUNE_MPPT_RAMP_MAX_PV10X		340
//	#define TUNE_MPPT_RAMP_DEF_PV10X		291

//	#define TUNE_MPPT_DOWN_MIN_PV10X		240
//	#define TUNE_MPPT_DOWN_MAX_PV10X		291
//	#define TUNE_MPPT_DOWN_DEF_PV10X		265

    #define TUNE_MPPT_RAMP_DN_MIN_PV10X		240
	#define TUNE_MPPT_RAMP_DN_MAX_PV10X		291
    #define TUNE_MPPT_RAMP_DN_DEF_PV10X     265

    #define TUNE_MPPT_RAMP_UP_MIN_PV10X		240
    #define TUNE_MPPT_RAMP_UP_MAX_PV10X		340
    #define TUNE_MPPT_RAMP_UP_DEF_PV10X		291

	#define TUNE_POWER_UPPER_LIMIT_MAX		(330)
	#define TUNE_POWER_UPPER_LIMIT_DEF		(320)


	#endif

	#if defined(SMI360_72)
  
    #define TUNE_MPPT_RAMP_DN_MIN_PV10X		260     // 3/4/2024 changed from 245
	#define TUNE_MPPT_RAMP_DN_MAX_PV10X		350
    #define TUNE_MPPT_RAMP_DN_DEF_PV10X     285     //

    #define TUNE_MPPT_RAMP_UP_MIN_PV10X		275     // 3/4/2024 changed from 265
    #define TUNE_MPPT_RAMP_UP_MAX_PV10X		425
    #define TUNE_MPPT_RAMP_UP_DEF_PV10X		325

	#define TUNE_POWER_UPPER_LIMIT_MAX		(480)
	#define TUNE_POWER_UPPER_LIMIT_DEF		(480)

	#endif

	#if defined(SMI480_60)
	#define TUNE_MPPT_RAMP_MIN_PV10X		240
	#define TUNE_MPPT_RAMP_MAX_PV10X		340
	#define TUNE_MPPT_RAMP_DEF_PV10X		291

	#define TUNE_MPPT_DOWN_MIN_PV10X		240
	#define TUNE_MPPT_DOWN_MAX_PV10X		291
	#define TUNE_MPPT_DOWN_DEF_PV10X		265

	#define TUNE_POWER_UPPER_LIMIT_MAX		(600)
	#define TUNE_POWER_UPPER_LIMIT_DEF		(500)

	#endif

	#define TUNE_MPPT_MODE_MAX      		(1)
    #define TUNE_MPPT_MODE_MIN      		(0)
    #define TUNE_MPPT_MODE_DEF      		(1)

	#define TUNE_MPPT_RAMP_UP_PERCENT_MAX      (99)
	#define TUNE_MPPT_RAMP_UP_PERCENT_MIN      (70)
	#define TUNE_MPPT_RAMP_UP_PERCENT_DEF      (78)

	#define TUNE_MPPT_RAMP_DN_PERCENT_MAX      (99)
	#define TUNE_MPPT_RAMP_DN_PERCENT_MIN      (75)
	#define TUNE_MPPT_RAMP_DN_PERCENT_DEF      (90)

//**************************************************************************


    #if defined(Uses_MPPT_Ramp_Up_Delay_Lock)
	#define TUNE_MPPT_RAMP_UP_DELAY_MIN			(20)
	#define TUNE_MPPT_RAMP_UP_DELAY_MAX			(20)
	#define TUNE_MPPT_RAMP_UP_DELAY_DEF			(20)    // 03/7/24 changed from 10 to 20
    #else
	#define TUNE_MPPT_RAMP_UP_DELAY_MIN			(10)
	#define TUNE_MPPT_RAMP_UP_DELAY_MAX			(1000)
	#define TUNE_MPPT_RAMP_UP_DELAY_DEF			(20)    // 03/7/24 changed from 10 to 20
    #endif

	#define TUNE_MPPT_RAMP_DN_DELAY_MIN			(1)
	#define TUNE_MPPT_RAMP_DN_DELAY_MAX			(10)
	#define TUNE_MPPT_RAMP_DN_DELAY_DEF			(5)

    #define TUNE_GEO_FREQ_50HZ_LOWER_MIN          (450)   // 500 - 10% = 450
    #define TUNE_GEO_FREQ_50HZ_LOWER_MAX          (499)   // 

    
    

    #define TUNE_GEO_FREQ_50HZ_UPPER_MIN         (501)   // 
    #define TUNE_GEO_FREQ_50HZ_UPPER_MAX         (550)   // 500 + 10%


    #if defined(Uses_Freq_Fault_Wide)
    #define TUNE_GEO_FREQ_50HZ_LOWER_DEF         (485)   // 500 - 3% = 485
    #define TUNE_GEO_FREQ_50HZ_UPPER_DEF         (515)   // 500 + 3%
    #else
    #define TUNE_GEO_FREQ_50HZ_LOWER_DEF         (490)   // 500 - 2% = 490
    #define TUNE_GEO_FREQ_50HZ_UPPER_DEF         (510)   // 500 + 2% = 510
    #endif

    #define TUNE_GEO_FREQ_60HZ_LOWER_MIN          (540)   // 600 - 10% = 540
    #define TUNE_GEO_FREQ_60HZ_LOWER_MAX          (599)   // 
    #define TUNE_GEO_FREQ_60HZ_LOWER_DEF          (582)   // 600 - 3% = 582

    #define TUNE_GEO_FREQ_60HZ_UPPER_MIN         (601)   // 
    #define TUNE_GEO_FREQ_60HZ_UPPER_MAX         (624)   // 600 + 10% = 660
    #define TUNE_GEO_FREQ_60HZ_UPPER_DEF         (618)   // (612)   // 600 + 3% = 618

	#define TUNE_MPPT_RAMP_TRIG_MIN   			(900)
	#define TUNE_MPPT_RAMP_TRIG_MAX             (995)
	#define TUNE_MPPT_RAMP_TRIG_DEF             (985)

	#define TUNE_ACV_DIV_MIN                    (175)
	#define TUNE_ACV_DIV_MAX                    (700)
	#define TUNE_ACV_DIV_DEF                    (353)

    #if defined(SMI240_60)
	#define TUNE_RIPPLE_DIV_MIN 			(16)
    #define TUNE_RIPPLE_DIV_MAX 			(50)
    #define TUNE_RIPPLE_DIV_DEF 			(22)
    #endif

    #if defined(SMI360_72)
	#define TUNE_RIPPLE_DIV_MIN 			(4)
    #define TUNE_RIPPLE_DIV_MAX 			(50)
    #define TUNE_RIPPLE_DIV_DEF 			(18)
    #endif

	#define TUNE_TEMP_HOT_MIN					(40)
	#define TUNE_TEMP_HOT_MAX					(140)
    #define TUNE_TEMP_HOT_DEF					110      // 
    #define TUNE_TEMP_HOT_OFF_DIFF              10
    #define TUNE_TEMP_MPPT_DN_DIFF              10
    #define TUNE_TEMP_MPPT_UP_DIFF              15


	#define TUNE_POWER_UPPER_LIMIT_RANGE(x)		Bounds_Check_Uint16(x,MPPT_MIN_WATTS,TUNE_POWER_UPPER_LIMIT_MAX)


//**************************************************************************


    	typedef union {
		struct 
		{
            unsigned flutter_mode:1;
            unsigned print_track:1;
            unsigned print_ramp:1;
			unsigned rfu :14;
		};

		struct {
			Uint16 value;
		};

	} MPPT_Control_Bits;

    #define TUNE_MPPT_BITS_FLUTTER_MODE     01
    #define TUNE_MPPT_BITS_PRINT_TRACK      02
    #define TUNE_MPPT_BITS_PRINT_RAMP       04
    
    #define TUNE_MPPT_BITS_DEF      		(0)

	typedef struct 
	{
		Uint16 hvdc;
		Uint16 pvdc;
	} MPPT_Ramp_Dn_Item;
	
	typedef struct 
	{
		Uint16 hvdc;
		Uint16 pvdc;
	} MPPT_Ramp_Up_Item;
    
	typedef struct 
	{
		MPPT_Ramp_Up_Item		up;
		MPPT_Ramp_Dn_Item		dn;
	} MPPT_Ramp_Items;

	typedef struct 
	{
		Uint16 fast;
		Uint16 slow;
	} MPPT_Delay_Items;
    

	typedef struct 
	{
        MPPT_Ramp_Items         run;
        MPPT_Ramp_Items         lock;
        MPPT_Ramp_Items         calc;
        
        Uint16 ramp_up_delay;

        Uint16                  ramp_trigger;
        Uint16                  ramp_dn_delay;
        
		Uint16				auto_mode;
        Uint16				ramp_up_percent;
        Uint16				ramp_dn_percent;
        
        Uint16				ramp_dn_sample_cnt;
        
        MPPT_Control_Bits   rcb;

	} MPPT_Tune;

	typedef struct 
	{
		Sint8					on;
		Sint8					off;
	} Temperature_Tune_Item;
	
	typedef struct 
	{
        Sint16                  hot_on;
        Sint16                  hot_off;
        Sint16                  mppt_up;
        Sint16                  mppt_dn;
        
	} Temperature_Tune;

   	typedef union 
    {
		struct 
		{
            unsigned fault_ripple :1;
            unsigned fault_ripple_pwm_sine:1;
		};

		struct {
			Uint16              value;
		};

	} Fault_Control_Bits;
    
    
	typedef struct 
	{
        Fault_Control_Bits      bit;
        Uint8                   ripple_div;
        
        Uint8                   ripple_dc1;
        Uint8                   ripple_dc2;
        Uint8                   ripple_dc3;
        
	} Fault_Tune;
	
	typedef struct
	{
		MPPT_Tune				mppt;
		Uint16					nvs_power_limit;
        Uint16					run_power_limit;
		Temperature_Tune		temperature;
   		Uint16					geo_code;
        Uint16					acv_div;
        Fault_Tune              fault;
	} SystemTuning;    

	public volatile SystemTuning tune;

	void  NVS_VID_Read_All();
	ErrorCode NVS_VID_Write_Item(Uint8 vid, Uint16 intData);
	ErrorCode NVS_VID_Read_Item(Uint8 parameter, Uint8 *data);
	Uint16 Bounds_Check_Uint16(Uint16 x, Uint16 xmin, Uint16 xmax);
	void NVS_Tune_Write_PV_Ramp_Down(Uint16 pvdc);
	void NVS_Tune_Write_PV_Ramp_Up(Uint16 pvdc);
	void NVS_Tune_Write_Temp_Hot(Uint16 celsius);
    
    #if defined(Uses_Debug_NVS)
    void NVS_Tune_Print_All();
    #endif

	
		
#endif // SVC_SYSTEM_TUNING_H