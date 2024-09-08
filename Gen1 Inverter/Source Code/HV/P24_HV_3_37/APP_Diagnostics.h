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

#if defined( Uses_Console ) && !defined( _APP_DIAGNOSTIC_H )
#define	_APP_DIAGNOSTICS_H

#if defined(Uses_Diagnostics)

	typedef union {
		struct 
		{
			unsigned start:1;
            unsigned fault_stop:1;
			unsigned diag_120_hz:1;
			unsigned diag_60_hz:1;
			unsigned diag_ufb_hi:1;
			unsigned rfu :2;
		};

		struct {
			unsigned int value;
		};

	} Diagnostic_Control_Bits;
		
	typedef struct 
	{
		Uint8	ufb;
		Uint8	ufb_timer;
		Uint8	ufb_sec;
        Uint8	state;
       	Uint8	timer;
		Uint8	buck_sec;		
		Uint16	ufb_freq;
		Uint16	ufb_hi;	
		Uint16	ufb_lo;
		Uint16	isr_zc;
		Uint16	isr_phase;
        Uint16	acv_adc;
		Uint8   div;
		Diagnostic_Control_Bits bit;
	} Diagnostic_Control_Block;


	public volatile Diagnostic_Control_Block diag;
		
	Uint8 console_parse_args_diagnostics();
    void APP_Inverter_Diag_Mode_Stop();
    
    #define DIAG_STATE_SLOW         0
    #define DIAG_STATE_FAST         1
		
		
#endif  // 	Uses_Diagnostics

#endif  // 	_APP_DIAGNOSTICS_H