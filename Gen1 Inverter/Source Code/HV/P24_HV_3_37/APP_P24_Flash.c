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

#include "APP_Inverter.h"


/********************************************************************
; Function: 	DWORD ReadLatch(WORD page, WORD addrLo)
; Input:    	page 	- upper byte of address
;				addrLo 	- lower word of address
; Output:   	data 	- 32-bit data in W1:W0
; Side Effects: TBLPAG changed
; Overview: 	Read from location in flash memory
;*********************************************************************/

DWORD P24_Flash_Read_Latch(WORD page, WORD addr)
{
	DWORD_VAL data;
//	TBLPAG = page;
	TBLPAG = 0;
	data.word.LW = __builtin_tblrdl(addr);
	data.word.HW = __builtin_tblrdh(addr);
	
	if (page==0)
	{
		// Look for bootloader delay time address 
		if (addr==DELAY_TIME_ADDR)		// Bootloader changes DELAY_TIME_ADDR (0x1202) from 0x0002 to 0x000a
		{
			data.word.LW = 0x0002;		// Change this back like hex file 
		}
		//	 On a flash when the hex file says 0xFFFF-FFFF you get 0xFFFF-00FF
		else if ((data.word.LW == 0xFFFF) && (data.word.HW == 0xFF))
		{
			data.word.HW = 0xFFFF;	//	DGA 2022-10-27 	Correct reading to match hex file
		}
	}
	
	return data.Val;
}

void P24_Flash_Read(int addr, BYTE *dst, WORD asize)
{
	DWORD_VAL data;
	WORD *bufw;
	BYTE cnt;
	WORD size;
	bufw = (WORD *) dst;
	cnt = 0;
	size = asize / 2;
	while (cnt < size)
	{
		data.Val = P24_Flash_Read_Latch(0, addr+cnt);
		bufw[cnt] = data.word.LW;
		bufw[cnt+1] = data.word.HW;
		cnt += 2;
	}
}

Uint16 P24_get_address_from_bid(Uint16 bid) 
{
	Uint16 addr = APP_PAGE_FIRST_OTA_BASE + ((bid * OTA_BLOCK_SIZE) / 2);
	return addr;
}

#define	Uses_Debug_BID

Uint16 P24_get_ota_id_from_address(Uint16 _addr) 
{
	if (_addr < APP_PAGE_FIRST)
		return 0;
	
	Uint16 rnd = 0;
	Uint16 quad = 0;
	Uint16 addr = (_addr-APP_PAGE_FIRST_OTA_BASE);
	Uint16 bid = addr / 32;
	
	if ((addr%32)!=0)
		bid++;
	
	quad = (bid %4);
	if (quad)
	{
		//	75,50,25
		rnd = quad;
	}
	
	#if defined(Uses_Debug_BID)
	if (!rcb.bit.initializing)
		printf("ADDR:%X,%d BID:%d,%d RND:%d QUAD:%d\n",_addr,addr,bid,bid+rnd,rnd,quad);
	#endif
	
	return bid+rnd;
}

WORD P24_Flash_Read_End_Bid()
{
	DWORD_VAL	data;	
	data.Val = P24_Flash_Read_Latch(0, APP_PAGE_FIRST+2);
	flash.app_end_addr = data.word.LW;
	flash.app_bid = P24_get_ota_id_from_address(flash.app_end_addr);
	
//	printf("READ x1002=%X  BID:%d\n",flash.app_end_addr,flash.app_bid);
	return flash.app_bid;
}

Uint16 P24_Flash_Scan_End_Bid()
{
	WORD		addr;
		
	#define APP_PAGE_LAST2	(0xA400-128)
	addr = APP_PAGE_LAST2;
	
//	#if defined(Uses_Pad_Debug)	
//	if (addr == 0xFFFF)
//		dummy1();
//	#endif
	
	while (addr > APP_PAGE_FIRST)
	{
		DWORD_VAL	data;
		data.Val = P24_Flash_Read_Latch(1, addr);
		if ((data.word.LW != 0xFFFF) || (data.word.HW != 0xFF))
		{
			flash.scan_end_addr = addr;
			flash.scan_bid = P24_get_ota_id_from_address(addr-2);
//			printf("SCAN OK ADDR:%X,%d  BID:%d\n",addr,addr,bid);
			return flash.scan_bid;
		}
		addr -= 2;
	}
	flash.scan_end_addr=0;
	flash.scan_bid = 0;
//	printf("Failure at ADDR:%u %X \n",addr,addr);
	return 0;
}

Uint16 __attribute__((optimize(2))) P24_get_block_chksum(unsigned char *src)
{
	Uint16 i;
	Uint16 sum = 0;
	for (i = 0; i < 64; i++)
	{
		sum += src[i];
	}
	return sum;
}

Uint16 P24_Flash_Get_Block_Checksum(Uint16 bid)
{
	Uint8 buf[64];
	Uint16 addr;
	addr = P24_get_address_from_bid(bid);
	P24_Flash_Read(addr, buf, 64);
	return P24_get_block_chksum(buf);
}
void P24_Flash_Get_Checksum()
{
	WORD bid;
	flash.chksum = 0;
	for (bid = 1; bid <= flash.app_bid; bid++)
	{
		DWORD sum = P24_Flash_Get_Block_Checksum(bid);
		flash.chksum += sum;
	}

	if (!rcb.bit.initializing)
		printf("BID:%d,%d  CHECK-SUM: %lX\n",flash.app_bid,flash.scan_bid,flash.chksum);
}

void P24_Flash_Init()
{
	P24_Flash_Read_End_Bid();	// 1st read Bootloader written value
	P24_Flash_Scan_End_Bid();	// 2nd scan flash memory for redundancy check 
	P24_Flash_Get_Checksum();	// Now read entire APP flash space and calculate the checksum 
	
}