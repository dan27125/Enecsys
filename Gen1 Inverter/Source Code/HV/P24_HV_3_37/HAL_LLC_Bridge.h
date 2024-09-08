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

#if !defined(LLC_BRIDGE_H)
#define LLC_BRIDGE_H

#if defined(Uses_LLC_Bridge)

#define		LLC_Bridge_12F_Baud		2400
#define		LLC_Bridge_Duo_Baud		2400

void LLC_Bridge_initialize();
void LLC_Bridge_seconds_task();
void LLC_Bridge_handle_ota(Uint8 *cmd);
void LLC_Bridge_handle_jst(Uint8 *cmd);
void LLC_Bridge_put_clock_sync();
void LLC_Bridge_task();
void LLC_Bridge_jst_putstr(char *s);
void LLC_Bridge_net_putstr(char *s);
void LLC_Bridge_RX_Flush();


#endif	//	Uses_LLC_Bridge
#endif	// ifndef PVKEY904_H

