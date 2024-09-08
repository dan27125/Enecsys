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

#if !defined(APP_Network_H)
#define APP_Network_H

#include "APP_Inverter.h"
#include "WIFI.h"
	

	public Uint8 rxbuf[256];
	public TPic24Com p24tx;	
	public TPic24Com p24rx;

	public char net_buf[ZNET_OTA_MAX_PAYLOAD_SIZE+16];
	public int net_size;
	public char net_flush;

	void ZNet_TX_putc(Uint8 c);
	void ZNet_TX_protocol_Start();
	void ZNet_TX_protocol_putc_raw(Uint8 c);
	void ZNet_TX_protocol_putch(Uint8 c);
	void ZNet_TX_write(Uint8 *buf, Uint8 size);
	void ZNet_TX_protocol_finish();

	void ZNet_TX_write_packet_raw(Uint8 *buf, Uint8 size);
	void ZNet_TX_write_packet(Uint8 cmd, Uint8 *buf, Uint8 size);

	void ZNet_EM250_reset();
	void ZNet_EM250_WDT_reset();
	void ZNet_EM250_WDT_Task_Sec();
    void ZNet_P24_RPC_RX(Uint8 cmd1, Uint16 v1, Uint16 v2, Uint8 *data, Uint8 size);
    void ZNet_P24_RPC_TX(Uint8 cmd1, Uint16 v1, Uint16 v2, Uint8 *data, Uint8 size);
    void ZNet_ReportFirmwareVersion();
    void ZNet_ReadVID(Uint8 value);	// DGA 2022-10-23 


void ZNet_Send_P24_Boot_Info();
void net_jst_uart_set(Uint8 set);
void ZNet_console_putstr(char *s);
void ZNet_console_write(char *s, int len);
void ZNet_set_flush(char value);
void ZNet_console_flush();
void ZNet_console_delay();
void ZNet_putch(char c); 
void ZNet_telemetry_ota_write();


void ZNet_RX_Packet_Binary(Uint8 *buf, Uint16 asize);
void ZNet_RX_char_ascii(Uint8 c);
Uint8 ZNet_GetAsciiPrn(Uint8 c);
int ZNet_is_bootloader_string(char *s);
void ZZNet_RX_Protocol(Uint8 c);
void ZNet_Task();



#endif