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

#if !defined(  HAL_P24_Task_H )
#define HAL_P24_Task_H

#include "APP_Inverter.h"
		
	public volatile H_BRIDGE_STATE ufb_state;
//	public volatile Uint8 phase_isr;
//	public volatile H_BRIDGE_STATE phase_state;
		
	#define	HAL_H_BRIDGE_Get()	ufb_state
		
	typedef enum
	{
		INVERTER_STOP = 0,          // 0
		#if defined(Uses_Diagnostics)
		INVERTER_DIAG_MODE,			// 1
		#endif
		INVERTER_START,				// 2
		INVERTER_DELAY,             // 3
		INVERTER_CONNECT,           // 4
		INVERTER_RUN,		// 5
	} InverterState;    
	
	public volatile InverterState system_state;
	
	void APP_State_Set(InverterState state);
	#define APP_State_Get()			system_state

	
	typedef union {
		struct 
		{
			unsigned ms100:1;
			unsigned sec:1;
            unsigned state_set;
            
		};
		struct 
		{
			unsigned int value;
		};
	} Pic24_Run_Control_Bits1;
	
	public volatile Pic24_Run_Control_Bits1 rcb1;
		

	void APP_Task_Init();
	void APP_Task_Scheduler();
	void APP_Set_Default_Bits();
	void APP_NVS_Config();
	void NVS_RCB_Write();
	void NVS_Config_Default();
	void APP_Bootloader_Task();
	void APP_Bootloader_Reset();

	void APP_delay_miliseconds(int ms);
    void APP_Console_Config();

	
	Uint8 APP_Inverter_is_Start_Fault();
	Uint8 APP_Inverter_is_Start_OK();
	void APP_Inverter_Start_Reset();
	void APP_Inverter_Start_Connect();
	void APP_Stop_Mode();
		
	void APP_Stop_Mode_Task();
	void APP_Inverter_Diag_Mode_Sec();
	void APP_Inverter_Start_Task();	
	void APP_Inverter_Start_Delay_Task();
	void APP_Inverter_Connecting_Task();
	void APP_Inverter_Operational_Task();
	void APP_Inverter_State_Machine_Task();
	void APP_Inverter_State_Machine_Ms1000();
    void APP_Inverter_Fault_History_Reset();
    void APP_Inverter_console_print_status();
    void APP_Inverter_NVS_Stop_Refresh();
    
    #if defined(Uses_Install_Mode)
    void APP_Inverter_Install_Mode_Refresh();
    #endif

//	******************************************		

	#if defined(Uses_Diagnostics)
	void APP_Inverter_Diag_Mode_Task();
	void APP_Inverter_Diag_Mode_Start();
	#endif



#endif