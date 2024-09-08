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

#if !defined(  _APP_CONSOLE_H )
#define	_APP_CONSOLE_H

	#include "APP_Inverter.h"

	#if defined (Uses_Console)

	#define CONSOLE_LF   10
	#define CONSOLE_CR   13
	#define CONSOLE_STDOUT  1

	#define	TEXT_50HZ			"50HZ"
    #define	TEXT_60HZ			"60HZ"

	#define	TEXT_AC				"AC"
    #define	TEXT_BUCK			"BUCK"
	#define	TEXT_CLOCK			"CLOCK"
	#define	TEXT_COLD			"COLD"
    #define	TEXT_DEBUG			"DEBUG"
	#define	TEXT_DELAY			"DELAY"
	#define	TEXT_DEFAULT		"DEFAULT"
    #define	TEXT_DIAG			"DIAG"
	#define	TEXT_DISC			"DISC"
	#define	TEXT_DN             "DN"
	#define	TEXT_DOWN			"DOWN"
	#define	TEXT_ERROR			"ERROR"
	#define	TEXT_FAULT			"FAULT"
//  #define	TEXT_FRP 			"FR+"
	#define	TEXT_FREQ			"FREQ"
    #define	TEXT_FLUT     		"FLUT"
	#define	TEXT_GAP			"GAP"
    #define	TEXT_GEO			"GEO"
    #define	TEXT_HIGH         	"HIGH"
	#define	TEXT_HOT			"HOT"
	#define	TEXT_HVDC			"HVDC"
	#define	TEXT_ISLAND			"ISLAND"
	#define	TEXT_KWH			"KWH"
	#define	TEXT_LIMIT			"LIMIT"
	#define	TEXT_LOCK			"LOCK"
	#define	TEXT_LOW			"LOW"
    #define	TEXT_MAX         	"MAX"
    #define	TEXT_MED         	"MED"
    #define	TEXT_MIN         	"MIN"
    #define	TEXT_MODE			"MODE"
	#define	TEXT_MPPT			"MPPT"
    #define	TEXT_NONE         	"NONE"
    #define	TEXT_OFF			"OFF"
    #define	TEXT_ON				"ON"
	#define	TEXT_PANEL			"PANEL"
	#define	TEXT_parameters		"parameters"

    #define	TEXT_PERCENT		"PERCENT"
	#define	TEXT_PHASE			"PHASE"
    #define	TEXT_PV				"PV"
	#define	TEXT_RAMP			"RAMP"
    #define	TEXT_RANGE			"RANGE"
	#define	TEXT_READ			"READ"
	#define	TEXT_REPORT			"REPORT"
	#define	TEXT_RESET			"RESET"
	#define	TEXT_RUN			"RUN"
	#define	TEXT_SHOW			"SHOW"
	#define	TEXT_SET			"SET"
    #define	TEXT_START			"START"
	#define	TEXT_STOP			"STOP"
	#define	TEXT_SURGE			"SURG"
	#define	TEXT_TEMP			"TEMP"
    #define	TEXT_TEST			"TEST"
    #define	TEXT_TIMER  		"TIMER"
    #define	TEXT_TRACK     		"TRACK"
    #define	TEXT_TRIG     		"TRIG"
	#define	TEXT_UFB			"UFB"
	#define	TEXT_UP             "UP"
	#define	TEXT_VER			"VER"
	#define	TEXT_WIMESH			"WIMESH"

	#define	TEXT_24		"24v"
	#define	TEXT_36		"36v"

	#define	TEXT_60		"60"
	#define	TEXT_72		"72"

	#define	CONSOLE_MAX_ARG_SIZE	32
	#define	CONSOLE_MAX_ARGS		10

	typedef struct 
	{
		Uint8	size;
		Uint8	cnt; 
		Uint8	str		[CONSOLE_MAX_ARGS][CONSOLE_MAX_ARG_SIZE];
		Uint16	value	[CONSOLE_MAX_ARGS];
	} TString_Info_Block;

	public TString_Info_Block csib;

	#define	arg1		(Uint8 *) csib.str[0]
	#define	arg2		(Uint8 *) csib.str[1]
	#define	arg3		(Uint8 *) csib.str[2]
	#define	arg4		(Uint8 *) csib.str[3]
		
	void console_handle_input(Uint8 *str);
		
	void console_putc(char c);
	void console_print_version();
	void console_print_copyright();
	
	#if defined(Uses_Console_Alarms)
	void console_print_alarms(char *desc, Alarm_Fault_Bits bit);
	void console_print_alarm_bits(Alarm_Fault_Bits bit);
	#endif

	int strcmp2(const char *str1, const char *str2, Uint8 *arg);
	
	int console_scan_string_raw(Uint8 *str, TString_Info_Block *p);

	void console_print_bit(char *desc, Uint8 value);
	void console_clock_print();
	void console_Task_Ms1000();
		
#if defined (Uses_Debug ) || defined(Uses_LLC_Bridge)	

	void console_print_debug();
	

	void console_print_hex(char *s, int size);
	#define	HAL_PrintHex	console_print_hex
	void console_handle_key_event(int evt);
	void console_nvs_rcb_print();
	#endif		//	Uses_Debug

	#if defined (Uses_Debug )
	void console_print_tuning();
	char * console_get_VID_ascii(int vid);
	void console_print_fault();
	#endif


	void console_print_command(char *cmd, char mode, Uint8 set);
	char console_scan_command(TString_Info_Block *sip, char mode, Uint8 set);
	Uint8 console_parse_args();
    
    Uint8 console_scan_cmd_on(Uint8 *str1, Uint8 *str2);
    Uint8 console_scan_cmd_off(Uint8 *str1,Uint8 *str2);


	
	#if defined(Uses_Console_Run)
	void console_print_alarm_bits(Alarm_Fault_Bits bit);
	void console_print_alarms(char *desc, Alarm_Fault_Bits bit);
	void console_print_faults();
    char *console_get_run_state();
	#endif

	
	
#endif		// Uses_Console

#endif		// APP_CONSOLE_H 