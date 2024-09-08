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



#if !defined(HAL_LLC_BRIDGE_C)
#define HAL_LLC_BRIDGE_C

#include <xc.h>
#include "APP_Inverter.h"
#include "HAL_LLC_Bridge.h"
#include "HAL_Uart.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "LV_LLC_CMDS.h"
#include "APP_Console.h"

#if defined(Uses_LLC_Bridge)

	#define	LLC_TX_FIFO_SIZE	200
	#define	LLC_TX_DELAY_TICKS	40
	
	Uint16 llc_txFifoHead;
	Uint16 llc_txFifoTail;
	Uint8 llc_txFifo[LLC_TX_FIFO_SIZE];

void LLC_Bridge_RX_Flush()
{
//	memset(pcb1.jst_buf, 0, sizeof(pcb1.jst_buf) );
	pcb1.jst_cnt = 0;
}

void LLC_Bridge_TX_Flush()
{
	memset(llc_txFifo, 0, sizeof(llc_txFifo) );
	llc_txFifoHead = 0;
	llc_txFifoTail = 0;
	timer5_llc_tx = 0;
}

void LLC_Bridge_initialize()
{
	#if defined(SMI480_60)
	Uart2_Config(LLC_Bridge_Duo_Baud);
	#else
	Uart2_Config(LLC_Bridge_12F_Baud);
	#endif

	llc.bit.pic24_jst_bridge = 1;
	llc.bit.auto_sync = 1;
	
	LLC_Bridge_RX_Flush();
	LLC_Bridge_TX_Flush();
}

void LLC_Bridge_TX_Fifo_put(char value)
{
	unsigned int ptr = llc_txFifoHead;
	ptr++;
	if (ptr == LLC_TX_FIFO_SIZE)        // Wrap if needed
	{
		ptr = 0;       // Wrap 
	}
    llc_txFifo[ptr] = value;    //  write to buffer
    llc_txFifoHead = ptr;       //  update the head
}

void LLC_Bridge_TX_Fifo_put_str(char *s)
{
	int size = strlen(s);
	int i;
	for (i = 0; i < size; i++)
	{
		LLC_Bridge_TX_Fifo_put( s[i] );
	}
}

char LLC_Bridge_TX_Fifo_empty()
{
	if ( llc_txFifoHead == llc_txFifoTail )
		return 1;
	return 0;
}

char LLC_Bridge_TX_Fifo_get()
{
	if ( llc_txFifoHead == llc_txFifoTail )
		return 0;
	//Here if at least one spot in the TX buffer
	llc_txFifoTail++;      //increment tail pointer
	if (llc_txFifoTail == Uart1_TX_FIFO_SIZE)
	{
		llc_txFifoTail = 0; //wrap if needed
	}
	return llc_txFifo[ llc_txFifoTail ];   //ret character from fifo 
}

void LLC_Bridge_TX_Fifo_task()
{
	if ( llc_txFifoHead != llc_txFifoTail )	
	{
		#if defined(SMI480_60)
		Uart2_PutChar( LLC_Bridge_TX_Fifo_get() );
		timer5_llc_tx = 0;
		#else
		if (timer5_llc_tx > LLC_TX_DELAY_TICKS)
		{
			Uart2_PutChar( LLC_Bridge_TX_Fifo_get() );
			timer5_llc_tx = 0;
		}
		#endif
	}
}

Uint8 LLC_scan_arg_UINT_16(char *src, char *arg1, Uint16 *dst, Uint8 offset)
{
	char *arg2 = strstr(src,arg1);
	if ( (arg2 != NULL) && (offset!=0))
	{
	
		arg2 = strstr(arg2,",");
	}

	if (arg2 != NULL)
	{
		Uint16 value = 0;
		if (offset)
		{
			arg2 = strstr(arg2,",");
			if (arg2)
			{
				value = atoi(&arg2[1]);		// Point to next char
			}
		}
		else
		{
			int len = strlen(arg1);
			value = atoi(&arg2[len]);
		}
		if (dst)
		 *dst = value;
		return 1;
	}
	return 0;
}

Uint8 LLC_scan_arg_UINT_08(char *src, char *arg1, Uint8 *dst, Uint8 offset)
{
	Uint16 value = 0;
	Uint8 rc = LLC_scan_arg_UINT_16(src, arg1, &value, offset);
	*dst = (Uint8) value;
	return rc;
}

void LLC_Bridge_handle_jst(Uint8 *cmd)
{
	if (llc.bit.forward)
	{
		LLC_Bridge_net_putstr( (char*) pcb1.jst_buf );
	}
	
//	RS:32848,03 PV:113,46 FD:27,92 TE:63 OS:13,84 SY:82 GB:00,20
	if (
		 LLC_scan_arg_UINT_16((char *) cmd, "RS:", &llc.fld.run_sec,0))
	{
		LLC_scan_arg_UINT_08((char *) cmd, "RS:", &llc.fld.state,1);
		
		LLC_scan_arg_UINT_08((char *) cmd, "PV:", &llc.fld.pvadc,0);
		LLC_scan_arg_UINT_08((char *) cmd, "PV:", &llc.fld.pvdiv,1);
		
		LLC_scan_arg_UINT_08((char *) cmd, "FD:", &llc.fld.pr2,0);
		LLC_scan_arg_UINT_08((char *) cmd, "FD:", &llc.fld.dty,1);
		
		LLC_scan_arg_UINT_08((char *) cmd, "TE:", &llc.fld.temp,0);
		
		LLC_scan_arg_UINT_08((char *) cmd, "OS:", &llc.fld.ostune,0);
		LLC_scan_arg_UINT_08((char *) cmd, "OS:", &llc.fld.tmr0,1);
		
		LLC_scan_arg_UINT_08((char *) cmd, "SY:", &llc.fld.syn_tm,0);
		
		LLC_scan_arg_UINT_16((char *) cmd, "GB:", &llc.fld.syn_good,0);
		LLC_scan_arg_UINT_08((char *) cmd, "GB:", &llc.fld.syn_bad,1);

		LLC_scan_arg_UINT_08((char *) cmd, "PC:", &llc.fld.duo_pow,0);
		LLC_scan_arg_UINT_08((char *) cmd, "PC:", &llc.fld.duo_cur,1);

		llc.fld.tran++;	// Inc transaction number
		
//		llc.fld.ostune = llc.fld.tran;
		
		#if defined(SMI480_60)
		llc.fld.ver = 48;		// DGA Feb 9, 2020 480 LV variant has different range for PV AND FREQ
		#else
		llc.fld.ver = 24;
		#endif
		
	}
	else if (llc.bit.forward==0)
	{
		LLC_Bridge_net_putstr( (char*) pcb1.jst_buf );
	}
}

void LLC_Bridge_handle_ota(Uint8 *cmd)
{
	LLC_Bridge_jst_putstr((char*)cmd);
}

void LLC_Bridge_net_putstr(char *s)
{
	char buf[200];
	int len;

	len = strlen(s);
	
	if (len >= sizeof(buf))
	{
		len = sizeof(buf)-1;
		s[len] = 0;	// shorten,  truncate string
	}
	
//	PV:27,150 ST:00 DUTY:00 FREQ:90 TEMP:111
	sprintf(buf, "LLC: %s",s);
	NET_console_putstr(buf);
}

void LLC_Bridge_seconds_task()
{
	
//	LLC_Bridge_net_putstr( "LLC_Bridge_seconds_task()" );
	
	if (llc.bit.pic24_jst_bridge)
	{
		if (llc.bit.auto_sync)
		{
//			LLC_Bridge_net_putstr( "AUTO SYNC ON" );
//			ON
			 if (LLC_Bridge_TX_Fifo_empty())
			 {
				Uart2_PutChar( SYNC2_CHAR );
				timer5_llc_tx = 0;
				
//				LLC_Bridge_net_putstr( "AUTO SYNC ON" );				
				
			 }
		}
//		else
//		{
//			LLC_Bridge_net_putstr( "AUTO SYNC OFF" );
//		}
	}
	
//	else
//	{
//		LLC_Bridge_net_putstr( "2-*** Bridge OFF ***" );
//	}

}

void LLC_Bridge_task()
{
	int key;
	
	LLC_Bridge_TX_Fifo_task();
	
	key = Uart2_GetChar();

	if (key == -1)
		return;

	if (llc.bit.pic24_jst_bridge==0)
	{
		return;
	}
	
	if (key >= 32)
	{
		if (pcb1.jst_cnt < sizeof(pcb1.jst_buf)-1)
		{
			pcb1.jst_buf[pcb1.jst_cnt] = key;
			pcb1.jst_cnt++;
			pcb1.jst_buf[pcb1.jst_cnt] = 0;	// Null terminate 
		}
	}
	
	else if ( (key == 13) && (pcb1.jst_cnt))
	{
		LLC_Bridge_handle_jst( pcb1.jst_buf );
		LLC_Bridge_RX_Flush();
	}
	
	
}

void LLC_Bridge_jst_putstr(char *s)
{
	static char cmd[128];
	int i;
	int j = 0;
	int len = strlen(s);
	
	for (i = 0; i < len; i++)
	{
		char c = s[i];
		if (c < 32)
			continue;
		cmd[j++] = c;
		cmd[j] = 0;	// Null terminate 
	}

	if (len == 1)
	{
		if (cmd[0] == ' ')
			j = 0;
	}
	cmd[j++] = 13;
	cmd[j] = 0;	// Null terminate 
	
	LLC_Bridge_TX_Fifo_put_str(cmd);
}


#endif	//	Uses_LLC_Bridge
#endif	// ifndef PVKEY904_H
