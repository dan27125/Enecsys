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



#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <libpic30.h> 
#include "Pic24_Bootloader.h"
#include "pic24_ports.h"
#include "HAL_Uart.h"


	typedef union Pic24CheckSum_s
	{
		DWORD sum;
		WORD sum16[2];
	} Pic24CheckSum;
  
    

void handle_SerialData_packet_protocol(BYTE *buf, WORD asize)
{
	Pic24CmdHdr *hdr = (Pic24CmdHdr *) buf;
	WORD bid = hdr->adr.lo;
	BYTE hdr_size= sizeof(Pic24CmdHdr);		// DGA 1/26/2018 - Compiler says 6 but its really 5
	BYTE *data = &buf[hdr_size];
	
	#if defined(Uses_OTA_CONSOLE)
	if (cfg1.debug2)
	{
		Put_Str("RX SZ:");
		Put_Dec(asize);
		Put_Str("CMD:");
		Put_Dec(hdr->cmd);
		Put_Str("(");
		Put_Dec(buf[1]);
		Put_Dec(buf[2]);
		Put_Dec(buf[3]);
		Put_Str(")");
		Put_Flush();
	}
	#endif

	#if defined( Uses_Debug2 )
	Puts("packet() sz=");
	PutDec(asize);
	Puts(" ");
	PutDec(hdr->len);
	Puts("  Cmd=");
	PutHex(hdr->cmd);
	Puts(" hdr=");
	PutDec(hdr_size);
	Put_CR();
	#endif
	sourceAddr.v[0] = buf[2];	//get 24-bit address from buffer
	sourceAddr.v[1] = buf[3];
	sourceAddr.v[2] = buf[4];
	sourceAddr.v[3] = 0;
    switch(hdr->cmd)
    {
        case SERIAL_COM_BOOTLOADER_RD_VER:
        {
			#if defined( Uses_Debug	)
		//	PutsCR("RD_VER");
			#endif
			buf[2] = BOOTLOADER_MINOR_VERSION;
			buf[3] = BOOTLOADER_MAJOR_VERSION;
			ZIG_Put_Packet(buf, 4);		
            break;
        }

		case SERIAL_COM_BOOTLOADER_WT_FLASH_64:
        {
//			WORD total_blocks = hdr->value16;
//			ota_total_blocks = total_blocks;
			//	2019-10-12  First block starts at no 1
			if (bid == 1)
			{
				ota_total_blocks = hdr->value16;
				
				#if defined(Uses_OTA_CONSOLE)
				Put_Str("BLOCKS:");
				Put_Dec(ota_total_blocks);
				Put_Str(" BID:");
				Put_Dec(bid);
				Put_Flush();
				#endif

			}
		
			Write_Buffer(bid, data);
            break;
        }
		
		#if defined(Uses_VERIFY)
		case SERIAL_COM_BOOTLOADER_VERIFY:
		{
			Pic24CheckSum p24;
			p24.sum16[0] = hdr->adr.lo;
			p24.sum16[1] = hdr->adr.hi;
			srv_ota_sum = p24.sum;
			if (Verify_New_Bootload()==0)
			{
				Verify_Write_Timeout();

				#if defined(Uses_OTA_CONSOLE)	
				Put_Str("VERIFY");
				Put_Str(" OK:");
				Put_Hex32(app_ota_sum);
				Put_Flush();
				#endif
				
				ZIG_Put_Reply(SERIAL_COM_BOOTLOADER_VERIFY, 0, 0, 0, 0);
//				ZIG_Put_Packet(buffer, 1);	//Respond to sent command
				verify_app_boot_flag++;	// Set flag to trigger boot
				blBusy = 0;
				Uart1_TX_process(1);  // Need TX wait delay here
			}
			else
			{
			//	VERIFY ERROR: 0X123422 VS 0X11656242	
				#if defined(Uses_OTA_CONSOLE)	
				Put_Str("VERIFY");
				Put_Str(" ERROR:");
				Put_Hex32(app_ota_sum);
				Put_Str("vs:");
				Put_Hex32(srv_ota_sum);
				Put_Flush();
				#endif
				ZIG_Put_Reply(SERIAL_COM_BOOTLOADER_VERIFY_ERROR, 0, 0, 0, 0);
				Erase_Pic24();  // DGA erase bad bootload 
			}
			break;
		}
		#endif
			
		case SERIAL_COM_BOOTLOADER_IGS_ERASE:
		{
//			if (app_erase==0)
			Erase_Pic24();	// Erase only once 
			ZIG_Put_Packet(buf, 1);
			break;
		}

		case SERIAL_COM_BOOTLOADER_IGS_BOOT_APP:
		{
			Boot_App();
			break;
		}
		
		case SERIAL_COM_BOOTLOADER_IDLE_KEEP_ALIVE:
		{
			Put_Keep_Alive(1);
			break;
		}

		#if defined( Uses_Debug )
		case SERIAL_COM_BOOTLOADER_DEBUG_PRINT:
		{
			em250_debug_print( &buf[2], hdr->len);
			break;
		}
		#endif

		
		case SERIAL_COM_PIC24_CONFIG_SET:
		{
			#if defined(Uses_OTA_CONSOLE)	
			if ( buf[2]== IGS_SET_PIC24_BOOTLOAD_DEBUG1)
			{
				cfg1.debug = !cfg1.debug;
				Put_Str("DEBUG1=");
				Put_Boolean(cfg1.debug);
				Put_Flush();
			}
			else if ( buf[2]== IGS_SET_PIC24_BOOTLOAD_DEBUG2)
			{
				cfg1.debug2 = !cfg1.debug2;
				Put_Str("DEBUG2=");
				Put_Boolean(cfg1.debug2);
				Put_Flush();
			}
			#endif
			break;
		}
    }
}
void handle_SerialData_RX_protocol(BYTE c)
{
	
	switch ( p24rx.state )
	{
		case RX_STATE_STX1:
		{
			if (c == BOOTLOADER_STX)
			{
				p24rx.state = RX_STATE_STX2;
				p24rx.size = 0;
				p24rx.chksum = 0;
				memset(buffer, 0, sizeof(buffer));	// Clear memory buf
			}
			#if defined( Uses_Debug )
			else
			{
				handle_RX_SerialData_char_ascii(c);  // DGA pass event to old code 
			}
			#endif
			return;
		}
		case RX_STATE_STX2:
		{
			if (c == BOOTLOADER_STX)
			{
				
	         	p24rx.state = RX_STATE_DATA;
		        p24rx.size = 0;
				p24rx.chksum = 0;
				p24rx.bit.DLE = 0;
				return;
			}
			else
			{
				p24rx.state = RX_STATE_STX1;
				p24rx.bit.DLE = 0;
			}
			break;
		}
		case RX_STATE_DATA:
		{
			if ((p24rx.bit.DLE == 0) && (c==BOOTLOADER_STX))
			{
				#if defined(Uses_Debug_Protocol)
				PutsCR("Error STX before ETX");
				#endif

				p24rx.state = RX_STATE_DATA;
				p24rx.size = 0;
				p24rx.chksum = 0;
			}
			else if ((p24rx.bit.DLE==0) && (c==BOOTLOADER_ETX))
			{
				p24rx.size--;    		// Subtract checksum from count
				p24rx.chksum = ~p24rx.chksum+1;
				
			//	#if defined(Uses_Debug_Write)
			//	PutsCR("ETX");
			//	#endif
				
				if (p24rx.size)
				{
					handle_SerialData_packet_protocol(buffer, p24rx.size);
				}
				p24rx.state = RX_STATE_STX1;
				
			}
			else if ((p24rx.bit.DLE==0) && (c==BOOTLOADER_DLE))
			{
				 p24rx.bit.DLE = 1;	// If DLE, treat next as data
			}
			else
			{
				if (p24rx.bit.DLE)
				{
					p24rx.bit.DLE = 0;
				}

				if (p24rx.size < sizeof(buffer))
				{
					buffer[p24rx.size++] = c;
					p24rx.chksum += c;
				}
				else
				{
					#if defined(Uses_Debug_Protocol)
					PutsCR("Buf overrun");
					#endif

					p24rx.state = RX_STATE_STX1;
					p24rx.chksum = 0;
					p24rx.size = 0;
				 }
			 }
			break;
		}
	}
	
}

void handle_SerialData_task()
{
	while ( Uart1_IsCharReady() != 0)
	{
		BYTE c = Uart1_GetChar();
		handle_SerialData_RX_protocol(c);
		Uart_process();
	}
	
	#if defined(Uses_Debug)
	while ( Uart2_IsCharReady() != 0)
	{
		BYTE c = Uart2_GetChar();
		handle_Uart2_RX_Keyboard(c);
	}
	#endif
	
}

int ZIG_pole_P24_bootload_mode_input()
{
	if (IO_PIN_ZIGBEE_PIC24_UPDATE == 0)
	{
		if (pic24_update)
		{
			pic24_update = 0;
			
			#if defined ( Uses_Debug )
			JST_PutsCR("BLM ");
			JST_PutsCR("OFF\n");
			#endif
		}
		return 0;
	}
	
	if (pic24_update==0)
	{
		pic24_update++;
		#if defined ( Uses_Debug )
		JST_PutsCR("BLM ");
		JST_PutsCR("ON\n");
		#endif
	}
	
	return 1;
}

void ZIG_PutChar(BYTE txChar)
{
	Uart1_PutChar(txChar);
}

void ZIG_Put_Packet(BYTE *txbuf, WORD responseLen)
{
	WORD i;
	BYTE data;
	BYTE checksum;

	#if defined(Uses_Debug_Response)
	Puts("PutResponse ");
	PutHex( responseLen );
	Puts("  ");	
	#endif

	ZIG_PutChar(BOOTLOADER_STX);			//Put 2 STX characters
	ZIG_PutChar(BOOTLOADER_STX);
	
	#if defined(Uses_Debug_Response)
	PutHex( BOOTLOADER_STX );
	PutHex( BOOTLOADER_STX );			
	Puts(" ");		
	#endif

	//Output buffer as response packet
	checksum = 0;
	#if defined(Uses_Debug_Response)	
	Puts(" ");
	#endif
	for(i = 0; i < responseLen; i++) 
	{
		asm("clrwdt");		//looping code so clear WDT
	
		data = txbuf[i];	//get data from response buffer
		checksum += data;	//accumulate checksum
	
		#if defined(Uses_Debug_Response)
		Puts("[");
		PutHex( data );
		Puts("] ");
		#endif

		//if control character, stuff DLE
		if(data == BOOTLOADER_STX || data == BOOTLOADER_ETX || data == BOOTLOADER_DLE)
		{
			ZIG_PutChar(BOOTLOADER_DLE);
			#if defined(Uses_Debug_Response)
			Puts("DLE ");
			#endif
		}
		ZIG_PutChar(data);  	//send data
	}
	checksum = ~checksum + 1;	//keep track of checksum
	//if control character, stuff DLE
	if(checksum == BOOTLOADER_STX || checksum == BOOTLOADER_ETX || checksum == BOOTLOADER_DLE)
	{
		ZIG_PutChar(BOOTLOADER_DLE);
		#if defined(Uses_Debug_Response)
		Puts("DLE ");
		#endif
	}
	#if defined(Uses_Debug_Response)
	Puts("CHKSUM-");
	#endif
	ZIG_PutChar(checksum);		//put checksum
	ZIG_PutChar(BOOTLOADER_ETX);			//put End of text
	#if defined(Uses_Debug_Response)
	JST_PutHex( checksum );	
	JST_PutsCR(" ETX ");
	#endif
}  //end PutResponse()

void ZIG_Put_Reply(BYTE cmd, WORD bid, BYTE rc, WORD w1, WORD w2)
{
	Pic24WriteReply reply;
	reply.cmd = cmd;
	reply.bid = bid;
	reply.result = rc;
	reply.buf_size = w1;
	reply.flashed = w2;	
	ZIG_Put_Packet((BYTE *) &reply, sizeof(reply));
}

void Put_Keep_Alive_Text(char *text)
{
	P24IdlePacketTX p;
	memset(&p, 0, sizeof(p));
	p.hdr.cmd = SERIAL_COM_BOOTLOADER_IDLE_KEEP_ALIVE;
	p.hdr.len = sizeof(P24IdlePacket);
	p.x.ver_bl.major = BOOTLOADER_MAJOR_VERSION;
	p.x.ver_bl.minor = BOOTLOADER_MINOR_VERSION;
	p.x.sec = secs;
	p.x.bit.app = is_valid_app;
	p.x.bit.em_blmode = ZIG_pole_P24_bootload_mode_input();
	p.x.apage_beg = APP_PAGE_FIRST;
	p.x.apage_end = APP_PAGE_LAST;
	if (text)
		strcpy((char*) p.x.text1, text);
	ZIG_Put_Packet((BYTE *) &p, sizeof(p));
//	#if defined ( Uses_Debug )
//	Puts( "Put_Keep_Alive");
//	Put_CR();
//	#endif
}

void Put_Keep_Alive(char force)
{
	if (force == 0)
	{	
		if (is_valid_app)
		{
			if (ZIG_pole_P24_bootload_mode_input()==0)
				return;
		}
	}
	Put_Keep_Alive_Text(0);
//	#if defined ( Uses_Debug )
//	Puts( "Keep_Alive");
//	Put_CR();
//	#endif
}

void task_Keep_Alive()
{
	static int cnt =0;
	cnt++;
	if (write_cnt)
	{
		write_cnt = 0;
		if (cnt < 10)
			return;
	}
	cnt = 0;
	Put_Keep_Alive(1);
}