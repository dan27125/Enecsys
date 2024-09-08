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

	#include "Pic24_Bootloader.h"
	#include <stdio.h>
	#include <ctype.h>
	#include <stdarg.h>
	#include <string.h>
	#include <stdlib.h>
	#include <libpic30.h> 
	#include "Pic24_Bootloader.h"
	#include "pic24_ports.h"
	#include "Bootloader_Protocol.h"
	#include "HAL_Uart.h"
	#include "Pic24_LED_PCB.h"


void HAL_clear_OTA_Buffers()
{
	memset(&ota_normal, 0x00, sizeof(ota_normal));
	srv_ota_sum = 0;
}

void HAL_set_OTA_base_address(WORD addr)
{
	if (addr < 0x1000)
		return;
	ota_normal.base_addr = addr;
	HAL_clear_OTA_Buffers();
	app_ota_sum = 0;
}

#define	OTA_BLOCK_SIZE				64
#define	APP_PAGE_FIRST_OTA_BASE		(APP_PAGE_FIRST-(OTA_BLOCK_SIZE/2))

WORD HAL_get_address_from_ota_id(WORD ota_id)
{
	WORD addr = APP_PAGE_FIRST_OTA_BASE + ((ota_id * OTA_BLOCK_SIZE) /2);
	return addr;
}

WORD HAL_get_ota_id_from_address(WORD addr)
{
	if (addr < APP_PAGE_FIRST)
		return 0;
	WORD base = (addr-APP_PAGE_FIRST_OTA_BASE) ;
	WORD ota = base / (OTA_BLOCK_SIZE/2);
	return ota;
}

WORD __attribute__((optimize(3))) HAL_get_block_chksum(BYTE *src)
{
	int i;
	WORD sum = 0;
	for (i = 0; i < 64; i++)
	{
		sum += src[i];
	}
	return sum;
}

void Write_Buffer(WORD bid, BYTE *src)
{
	char rep_flag;
	
	#if defined(Uses_Debug_Write)
	WORD addr = HAL_get_address_from_ota_id(bid);
	#endif

	WORD ndx = bid % 4;

	if (ndx==0)
		ndx = 4;
	
	ndx--;	//Base zero 

//	Print_Hex_Payload(bid, src);
	
	#if defined(Uses_Debug_Write)
	JST_Puts("Write Bid=");
	JST_PutDecWord(bid);
	JST_Puts(" Max=");
	JST_PutDecWord(ota_total_blocks);
	JST_Puts(" Adr=");
	JST_PutHexWord( addr );
	JST_Puts(" cnt=");	
	JST_PutDec(ota_normal.cnt);
	JST_Puts(" ndx=");	
	JST_PutDec(ndx);
	JST_Puts("  Last=");		
	JST_PutDecWord(ota_normal.bid_last);
	JST_Put_CR();
	Uart2_TX_process(1);
	#endif

	if (bid == 1)
	{
		HAL_set_OTA_base_address( APP_PAGE_FIRST );
	}

	rep_flag = 0;
	if (ndx >= 3)
		rep_flag = 1;
	
	ZIG_Put_Reply(SERIAL_COM_BOOTLOADER_WT_FLASH_64, bid, 0, ndx, rep_flag);

	if (block_index[bid]==0)	// Ignore repeat packets 
	{
		WORD sum;
		sum = HAL_get_block_chksum(src);
		app_ota_sum += sum;

		#if defined(Uses_OTA_CONSOLE)
		
		if (cfg1.debug)
		{
			Put_Str("BLK:");
			Put_Dec(bid);
			Put_Str(" CHK:");
			Put_Hex16(sum);	
			Put_Hex32(app_ota_sum);			
			Put_Flush();
		}
		#endif

	}
	Write_Buffer_raw(bid, src, ndx);
}

void Write_Buffer_raw(WORD bid, BYTE *src, BYTE fid)
{
	BYTE *dst;
	WORD bid_A;
	BYTE ndx;
	WORD addr_write;
				
	if (fid > 3)
	{
		#if defined(Uses_Debug_Write)
		JST_Put_CR();
		JST_Puts("ERROR fid=");
		JST_PutDec(fid);
		JST_Put_CR();
		#endif

		return;
	}

 	ndx = 0;
	ota_normal.bid_last = bid;
	if (fid)
	{
		ndx = fid * OTA_BUFFER_SIZE;	
	}
	if (bid > ota_total_blocks)
	{
		#if defined(Uses_Debug)
		JST_Puts_Warn("BID high");
		#endif

		return;
	}
	dst = &ota_normal.buf[ndx];
	ota_normal.bid[fid] = bid;
	memcpy(dst,src,OTA_BUFFER_SIZE);	
	block_index[bid] = 1;
	if (fid < 3)
	{
		ota_normal.cnt++;
		return;
	}
	
	bid_A = ota_normal.bid[0];							// DGA get block id of 1st block  
	addr_write = HAL_get_address_from_ota_id(bid_A);	// DGA get address of 1st block 
	Write_PM(addr_write, ota_normal.buf, 256);
//	**********************************************	
	memset(ota_normal.bid, 0, sizeof(ota_normal.bid));

//	**********************************************		
	ota_normal.cnt = 0;
	
}
