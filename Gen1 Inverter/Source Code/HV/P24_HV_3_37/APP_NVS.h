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

#if !defined( HAL_NON_VOLATILE_STORE_H )
#define HAL_NON_VOLATILE_STORE_H

//	March 29,2022 DGA - NVS parms are now using exactly 3 bytes each 
//	Paramters are two bytes (16bits ) wide + one byte for the valid signature
    
    #define NVS_GEO_CODE                            0 // 1   
    #define NVS_ENGINEERING_MODE			        3 // 4
    #define NVS_GRID_STATE					        6 // 7

	#define NVS_KWH_WT								9 // 10	
	#define NVS_KWH_KW								12 // 13	

	#define NVS_POWER_UPPER_LIMIT					15 // 16	
    #define NVS_TEMPERATURE							18 // 19	
    #define NVS_REPORT_INTERVAL                     21 // 22	
    #define NVS_MPPT_RAMP_UP						24 // 25	
    #define NVS_MPPT_DELAY_RAMP						27 // 28	
    #define NVS_MPPT_RAMP_DN						30 // 31	
    #define NVS_MPPT_DELAY_DOWN						33 // 34	
	#define NVS_HOURS_CONSOLE                		36 // 37	
	#define NVS_HOURS_RUNTIME        				39 // 
    #define NVS_DIAG_UFB_SECS        				42 // 
    #define NVS_DIAG_BUCK_SECS        				45 // 
    #define NVS_CLOCK1              				48 // 
    #define NVS_CLOCK2              				51 // 
    #define NVS_MPPT_AUTO_MODE                      53 // 
    #define NVS_MPPT_RAMP_UP_PERCENT                56 // 
    #define NVS_MPPT_RAMP_DN_PERCENT                59 // 

    #define NVS_RFU0                                62 // 

	#define NVS_RCB1								63 // 64
	#define NVS_RCB2								66 // 67
	#define NVS_RCB_RFU1							69 // 70
	#define NVS_RCB_RFU2							72 // 73
	#define NVS_DIAG_UFB							75 // 76
	#define NVS_DIAG_BUCK							78 // 79
	#define NVS_DIAG_RFU1							81 // 82
	#define NVS_DIAG_RFU2							84 // 85
	#define NVS_MPPT_RCB							87
    
    #define NVS_BYTE_RIPPLE_DIV                     90

    #define NVS_MPPT_RAMP_TRIG                      93 // 

    #define NVS_GEO_FREQ_50HZ_LOWER                 96 
    #define NVS_GEO_FREQ_50HZ_UPPER                 99 
    #define NVS_GEO_FREQ_60HZ_LOWER                 102
    #define NVS_GEO_FREQ_60HZ_UPPER                 105

    #define NVS_WORD_FAULT_BITS                    108

    #define NVS_WORD_ACV_DIV                       111


    #define NVS_FAULT_COUNTER_SIG                  134
    #define NVS_FAULT_COUNTER_BEG                  135
    #define NVS_FAULT_COUNTER_TOTAL                18   // X * 6 = 120

    // 6*16=96
//	#define NVS_NAME_TEXT                           239     // 16 BYTES 


	#if defined(SMI240_60)
	#define NVS_AUTO_CONFIG_VARIANT					NVS_AUTO_CONFIG_VARIANT_240
	#endif

//	#if defined(SMI360_72)
//	#define NVS_AUTO_CONFIG_VARIANT					NVS_AUTO_CONFIG_VARIANT_360
//	#endif

	#if defined(SMI480_60)
	#define NVS_AUTO_CONFIG_VARIANT					NVS_AUTO_CONFIG_VARIANT_480
	#endif

    #define MAX_NV_STORE_ADDRESS					255

//********* NVS_VALID_DEFAULT *********
//  #define NVS_VALID_DEFAULT						(0x10)
//  #define NVS_VALID_DEFAULT						(0x11)  // 7/25/23 
//  #define NVS_VALID_DEFAULT						(0x12)  // 9/22/23 
    #define NVS_VALID_DEFAULT						(27)  // 3/16/24
//********* NVS_VALID_DEFAULT *********


    enum NVS_MODE  { NVS_MODE_NORMAL, NVS_MODE_IGNORE, NVS_MODE_RESET};


	typedef struct 
	{
        Uint16 hours_console;          // DGA 
        Uint16 hours_runtime;          // DGA 
        Uint8 format;                  // NVS Print state machine 	
        Uint8 print_limit;             // DGA 
       
        enum NVS_MODE mode;

        #if defined(Uses_NVS_Console)
        Uint8 print;                        // NVS Print state machine 
        Uint8 print_addr;                        // NVS Print state machine 
        #endif
        
        Uint8 set_default;
        Uint8 use_signature;
        Uint8 valid_signature;
        
        Uint16 read_error;          // DGA         
        
    	#if defined(Uses_Debug_NVS_Clock)    
        Uint16 run_sec;
        Uint8 test_reset;    
        #endif
	} NVS_Block;
        
    public volatile NVS_Block nvs; 

        
	int NVS_Read_Word(Uint8 addr, Uint16 *dst, Uint8 valid);
	int NVS_Read_Byte(Uint8 addr, Uint8 *dst, Uint8 valid);
	void NVS_Write_Word(Uint8 addr, Uint16 value, Uint8 valid);
	void NVS_Write_Byte(Uint8 addr, Uint8 value, Uint8 valid);

	Uint8 NVS_Engineer_Mode_Read();
    void NVS_Engineer_Mode_Write(Uint16 value);
    
    
    Uint16 NVS_GEO_Read();
    void NVS_GEO_Write(Uint16 code);
    
    Uint16 NVS_Console_Hours_Read();
    void NVS_Console_Hours_Write(Uint16 value);
    
    Uint16 NVS_Console_Hours_Read();
    void NVS_Runtime_Hours_Write(Uint16 value);
    
    
    
    
   	void NVS_Set_Valid(Uint8 code);
        
	void NVS_Read_All(enum NVS_MODE mode);

    void NVS_Task();
    
    #if defined(Uses_NVS_Console)
    Uint8 NVS_console_parse_args();

    #if defined(Uses_Debug_NVS)
    void NVS_Format(Uint8 value, Uint8 size);
    void NVS_Print_Addr(Uint16 addr);
    void NVS_Print_All();
    #endif
    #endif
    
    #if defined(Uses_Debug_NVS_Clock)
    void NVS_Clock_Read();
    void NVS_Clock_Write();
    #endif

    #if defined(Uses_Diagnostics)
    void NVS_Diag_Read();
    void NVS_Diag_Write();
    #endif

    
   	typedef struct 
	{
        Uint16 count; 
        Uint16 item1; 
        Uint16 item2; 
    } NVS_Fault_Hist;

    
    Uint16 NVS_Fault_Hist_Read(Uint8 item, NVS_Fault_Hist *p);
    void NVS_Fault_Hist_Write(Uint8 item, NVS_Fault_Hist *p);
    Uint16 NVS_Fault_Hist_Read(Uint8 item, NVS_Fault_Hist *p);
    void NVS_Fault_Hist_Write(Uint8 item, NVS_Fault_Hist *p);
    void NVS_Fault_Hist_Increment(Uint8 item, Uint16 p1, Uint16 p2);
    void NVS_Fault_Hist_Reset_Item(Uint8 item);
    void NVS_Fault_Hist_Reset_All();

    void NVS_grid_state_write(Uint8 gridState);
    Uint8 NVS_grid_state_read();
    
    void NVS_ripple_divsor_write(Uint8 value);
    Uint8 NVS_ripple_divsor_read();

    void NVS_acv_divsor_write(Uint16 value);
    Uint16 NVS_acv_divsor_read();
    
    void NVS_fault_bits_write(Uint16 value);
    void NVS_fault_bits_read();

    void NVS_temperature_hot_write(Sint16 value);
    void NVS_temperature_hot_read();
    
    

#endif 