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

#if !defined(_ADC_H)
#define _ADC_H

	#define DCLINK_DIVISOR				724	
	#define DCLINK_MULTIPLIER			138

	#define DCLINK_GET_VDC(x)			(Uint16)(((Uint32) x * DCLINK_DIVISOR) / 1000)
	#define DCLINK_GET_ADC(x)			(Uint16)((Uint32)(x * DCLINK_MULTIPLIER)/100)

//  600 * 1.38 = 828 max adc value      65535/828=79.14 Max samples

    #define	DCV_ADC_290v	400	// 290*1.38=400
    #define	DCV_ADC_300v 	414	// 300*1.38=414
    #define	DCV_ADC_320v 	441	// 320*1.38=441
    #define	DCV_ADC_340v 	469	// 340*1.38=469
    #define	DCV_ADC_360v 	496	// 360*1.38=496
    #define	DCV_ADC_370v	510	// 370*1.38=510
    #define	DCV_ADC_380v 	524	// 380*1.38=524
    #define	DCV_ADC_400v	552	// 400*1.38=552
    #define	DCV_ADC_430v	593	// 430/.724=593
    #define	DCV_ADC_420v	580	// 420/.724=580
    
    #define	DCV_ADC_450v	621	// 450*1.38=621
    #define	DCV_ADC_470v	648	// 470*1.38=648
    #define	DCV_ADC_471v	650	// 471*1.38=649.98
    #define	DCV_ADC_500v	690	// 500*1.38=690
    #define	DCV_ADC_510v 	703	// 510*1.38=703
    #define	DCV_ADC_520v 	716	// 520*1.38=716
    #define	DCV_ADC_530v 	731	// 530*1.38=731
    #define	DCV_ADC_540v 	745	// 540*1.38=745
    #define	DCV_ADC_550v 	759	// 550*1.38=759
    #define	DCV_ADC_560v 	773	// 560*1.38=772.8
    #define	DCV_ADC_565v 	780	// 565*1.38=779.7
    #define	DCV_ADC_570v 	786	// 570*1.38=786


    //  R57 430v (Reverse) Breakdown Voltage, Wurth Varistor datasheet 
    #define	DCL_UFB_TEST_LIMIT_DCV  440

    #define	DCL_R57_LIMIT_ADC   	DCL_ADC_420V    // USE saftey limit 


// To calculate PV_DIVISOR DO NOT USE RAW ADC value 
// Simply take the HVDC value and div by voltage 

	#define	PV_DIVISOR_OFF_V240			1771
	#define	PV_DIVISOR_RUN_V240			1591

	#define	PV_DIVISOR_OFF_V360			1277	// DGA 1277
	#define	PV_DIVISOR_RUN_V360			1177	// DGA 1177

//  480-DUO PV Set to 30 yields  ADC of 784 which is 567 HVDC 
//  inverter running at  20w yields  ADC of 616 which is 447 HVDC 

	#define	PV_DIVISOR_OFF_V480			1818
	#define	PV_DIVISOR_RUN_V480			1591

	#if defined(SMI240_60)
	#define	PV_DIVISOR_OFF				PV_DIVISOR_OFF_V240		
	#define	PV_DIVISOR_RUN				PV_DIVISOR_RUN_V240		
	#endif

	#if defined(SMI360_72)
	#define	PV_DIVISOR_OFF				PV_DIVISOR_OFF_V360		
	#define	PV_DIVISOR_RUN				PV_DIVISOR_RUN_V360		
	#endif

	#if defined(SMI480_60)
	#define	PV_DIVISOR_OFF				PV_DIVISOR_OFF_V480		
	#define	PV_DIVISOR_RUN				PV_DIVISOR_RUN_V480
	#endif

    #define ACV_ADC_VOLT_FAULT      268

    #define ACV_ADC_VOLT_MPPT_DN    (ACV_ADC_VOLT_FAULT-2)
    #define ACV_ADC_VOLT_MPPT_UP    (ACV_ADC_VOLT_MPPT_DN-4)

//    #define ACV_ADC_TEMPERATURE_MPPT_UP     71  // 159F 71C
//    #define ACV_ADC_TEMPERATURE_MPPT_DN     80  // 176F 80C

//  #define	ACV_CALC_VOLTS10X(adc)      ((Uint32)(adc + temperature.ac_vdrop) * 100) / 35
    #define	ACV_CALC_VOLTS10X(adc)      ((Sint32)(adc + temperature.ac_vdrop) * 1000) / tune.acv_div

//  #define	ACV_CALC_VOLTS(adc)         ((Uint16)(adc + temperature.ac_vdrop) * 10) / 35
    #define	ACV_CALC_VOLTS(adc)         ((Sint32)(adc + temperature.ac_vdrop) * 100) / tune.acv_div

    #define	ACV_CALC_ADC(volts)          (Sint16) ((((Sint32) volts*tune.acv_div) /100) - temperature.ac_vdrop)

    #define	ACV_CALC_ADC35(volts)         ((Sint16) ((volts*35)/10) - temperature.ac_vdrop)


	#define	PV10X_CALC_DCLINK(_PV_)			(Uint16) (((Uint32) _PV_ * PV_DIVISOR_RUN) / 1000)
	#define	DCLINK_CALC_PV(_DCL_)			(Uint16) (((Uint32) _DCL_ * 100) / PV_DIVISOR_RUN)

	#define	DCLINK_CALC_PVX10(_DCL_)		(Uint16) (((Uint32) _DCL_ * 1000) / PV_DIVISOR_RUN)
	#define	DCLINK_CALC_PVX10_IDLE(_DCL_)	(Uint16) (((Uint32) _DCL_ * 1000) / PV_DIVISOR_OFF)

	#define ADC_AN09_TEMP	         9
	#define ADC_AN11_AC_GRID        11
	#define ADC_AN12_DC_LINK	    12

	#define	CSSL_TEMP				CSSL9
	#define	CSSL_AC_GRID			CSSL11
	#define	CSSL_DC_LINK			CSSL12

	#define AC_GRID_SAMPLE_MAX		10

//	#define ADC_CHANNEL_MONITOR_3V3    (ADC_CH0NB_AN1 +  6)
//	#define ADC_CHANNEL_MONITOR_5V     (ADC_CH0NB_AN1 +  8)
		
	typedef union {
		struct 
		{
			unsigned grid_timer:1;
			unsigned grid_timeout:1; 
			unsigned grid_sample:1;
			unsigned grid_boot:1;
			unsigned dclink_calc_avg:1;
        	unsigned temp_start:1;
			unsigned temp_sample:1;
			unsigned temp_timer:1;
            unsigned zc_isr_pause:1;
            
		};

		struct {
			unsigned int value;
		};
	} ADC_Control_Bits1;

	typedef struct 
	{
        volatile Uint16 sample0;
		volatile Uint16 sample;
		volatile Uint16 volts;
	} ADC_AC_Item1;
    
	typedef struct 
	{
		volatile Uint32 sum;
		volatile Uint16 cnt;
		volatile Uint16 sample;
		volatile Uint16 volts;
	} ADC_AC_Average_Item;

	typedef struct 
	{
        volatile ADC_AC_Average_Item    avg;
        volatile ADC_AC_Item1           cyc;
        
	} ADC_ACV_Average_Block;


	typedef struct 
	{
		volatile Uint32 sum;
		volatile Uint16 cnt;
		volatile Uint16 sample;
		volatile Uint16 volts;
	} ADC_DC_Average_Item;

    typedef struct 
	{
		volatile Uint16 sample;
        volatile Uint16 volts;
        volatile ADC_DC_Average_Item     avg;
	} ADC_DC_Ripple_Diff;
    
	typedef struct 
	{
		volatile Uint16 adc_sample;
        volatile Uint16 pwm_sample;
        volatile ADC_DC_Average_Item     avg;
	} ADC_DC_Ripple_Item;
    
	typedef struct 
	{
        volatile ADC_DC_Ripple_Item          hi;
        volatile ADC_DC_Ripple_Item          lo;
        volatile ADC_DC_Ripple_Diff          dif;
        volatile ADC_DC_Average_Item         avg;
	} ADC_DC_Ripple;
    
	typedef struct 
	{
        volatile ADC_DC_Average_Item     avg;
        volatile ADC_DC_Average_Item     avg10;
          
	} ADC_PWM_Block;
    
	typedef struct 
	{
		volatile Uint16 sample;
		volatile Uint16 voltage;

        volatile ADC_DC_Ripple           ripple;
        
        ADC_DC_Average_Item     avg;
        ADC_DC_Average_Item     avg10;
        
	} ADC_DCLink_Block;
    

	typedef struct 
	{
		Uint8 num;
		Uint8 dec;
	} Item_Decimal;
	
	typedef struct 
	{
		Uint16 volts10x;
		Item_Decimal volts;
		Uint16 volts_avg10x;
	} ADC_PVDC_Block;
	
	typedef struct 
	{
		volatile Uint16 sample;
	} ADC_Temp_Block;
    
    typedef struct 
	{
        volatile Uint32     sum;
		volatile Uint16     cnt;
        volatile Uint16     sample;
        volatile Uint16     volts;
	} ADC_DC_PWM;

	typedef struct 
	{
        volatile Sint16 sample;
		volatile Sint16 volts;
	} ADC_AC_Fault;

    
	typedef struct 
	{
        volatile ADC_PWM_Block      pwm;
        
        ADC_Control_Bits1           bit;
        
        ADC_ACV_Average_Block      ac;
        
        ADC_AC_Fault                ac_fault;
        
		volatile ADC_DCLink_Block dc;
                
		ADC_PVDC_Block	 pvdc;
		ADC_Temp_Block  temp;
		
	} ADC_Control_Block;
	
	public volatile ADC_Control_Block analog;
        

	void ADC_Init(void);
	void ADC_GridVoltageLimits(Uint16 gridSample);
	Uint16 ADC_AC_LO_Correction(Uint16 acv);
	Uint16 ADC_AC_Correction(Uint16 acv);
	void ADC_AC_Calc_Limit();

	Uint16 ADC_PV_Calc_DCL(Uint16 pv);
    
	Uint16 ADC_PV10X_Calc_DCL(Uint16 pv);
    Uint16 ADC_DCL_Calc_PV10X(Uint16 dcl);
	
    void ADC_DC_Calc();
	void ADC_Calc_DC_Instant();
	void ADC_ZC_ISR();
	Uint8 ADC_Calc_AC_Duty(Uint16 live_count, Uint16 neutral_count);
	void ADC_Temperature_Update();
    void ADC_Task();


    
#endif // _ADC_H
