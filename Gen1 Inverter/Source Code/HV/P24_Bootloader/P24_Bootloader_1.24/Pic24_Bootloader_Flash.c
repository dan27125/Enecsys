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


void Erase_Pic24()
{
	WORD addr;
	addr = APP_PAGE_FIRST;
	blBusy = 0;
	
//	#if defined(Uses_OTA_CONSOLE)
//	Put_CONSOLE_DEBUG_2("ERASE");
//	#endif
		
	while (addr <= APP_PAGE_LAST )
	{
		Erase(0, addr, PM_PAGE_ERASE);
		addr += PM_ERASE_SIZE;
	}

	memset(&block_index, 0, sizeof(block_index));
	HAL_set_OTA_base_address(APP_PAGE_FIRST);
	app_erase++;	// Indicate app has been erased

	#if defined(Uses_OTA_CONSOLE)
	Put_CONSOLE_DEBUG_2("ERASE OK");
	#endif
}

void Write_PM(WORD addr, BYTE *src, WORD size)
{
	WORD write_size;
	DWORD_VAL data;
	write_size = 0;	//first 5 buffer locations are cmd,len,addr	
	if(addr < APP_PAGE_FIRST)
	{
		return;
	}
		        
	#if defined(Uses_OTA_CONSOLE)
	if (cfg1.debug2)
	{
		Put_Str("Write_PM:");
		Put_Hex16(addr);
		Put_Dec(size);
		Put_Flush();
	}
	
	#endif 
	
	while(write_size < size)
	{
		asm("clrwdt");
		Uart1_TX_process(0);	
		//get data to write from buffer
		data.v[0] = src[write_size];
		data.v[1] = src[write_size+1];
		data.v[2] = src[write_size+2];
		data.v[3] = src[write_size+3];

		//4 bytes per instruction: low word, high byte, phantom byte
		write_size += PM_INSTR_SIZE;
		
		//put information from reset vector in user reset vector location
		if(addr == APP_PROG_RESET)
		{
			if(app_ResetRead)
			{  //has reset vector been grabbed from location 0x0?
				
				#if defined(Uses_OTA_CONSOLE)
				Put_CONSOLE_DEBUG_2("RESET1");
				#endif 

				//if yes, use that reset vector
				data.Val = app_Reset.Val;	
			}
			else
			{

				#if defined(Uses_OTA_CONSOLE)
				Put_CONSOLE_DEBUG_2("RESET2");
				#endif 
				
				//if no, use the user's indicated reset vector
				app_Reset.Val = data.Val;
				
			}
		}

		//If address is delay timer location, store data and write empty word
		if(addr == DELAY_TIME_ADDR) 
		{
			app_Timeout.Val = data.Val;
			data.Val = 0xFFFFFF;
		}
		//write data into latches
 		WriteLatch(0, addr, data.word.HW, data.word.LW);
		
		//write to flash memory if complete row is finished
		if((write_size % PM_ROW_SIZE) == 0)
		{
			WriteMem(PM_ROW_WRITE);	
		}

		addr += 2;		// increment addr by 2
	} 
	app_erase = 0;	// Reset erase signal 
	write_size++;
	write_timer = 0;
}

void Read_Flash(int addr, BYTE *dst, WORD asize)
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
		data.Val = ReadLatch(0, addr+cnt);
		bufw[cnt] = data.word.LW;
		bufw[cnt+1] = data.word.HW;
		cnt += 2;
	}
}

int Verify_New_Bootload()
{
	WORD bid;

	if (ota_total_blocks < 100)
		return 1;
	
	for (bid = 1; bid <= ota_total_blocks; bid++)
	{
		if (block_index[bid] != 1)
		{
			
			#if defined(Uses_OTA_CONSOLE)			
//			Put_Str("REC ");
			Put_Str("ERR ");
			Put_Str("BLK:");
			Put_Dec(bid);	
			Put_Str(" OF ");
			Put_Dec(ota_total_blocks);
			Put_Flush();
			#endif

			return 1;
		}
	}
//	srv_ota_sum DGA Oct 15, 2019, passing this new param for checksum 
	if (app_ota_sum != srv_ota_sum)
	{
		return 1;
	}
	
	return 0;
}

//	void ReadChecksumPM(WORD length, DWORD_VAL sourceAddr)

#if defined( Uses_Read	)
void ReadPM(WORD length, DWORD_VAL sourceAddr)
{
	WORD bytesRead = 0;
	DWORD_VAL temp;

	//Read length instructions from flash
	while(bytesRead < length*PM_INSTR_SIZE)
	{
		//read flash
		temp.Val = ReadLatch(sourceAddr.word.HW, sourceAddr.word.LW);

		buffer[bytesRead+5] = temp.v[0];   	//put read data onto 
		buffer[bytesRead+6] = temp.v[1];	//response buffer
		buffer[bytesRead+7] = temp.v[2];
		buffer[bytesRead+8] = temp.v[3];
	
		//4 bytes per instruction: low word, high byte, phantom byte
		bytesRead+=PM_INSTR_SIZE; 

		sourceAddr.Val = sourceAddr.Val + 2;  //increment addr by 2
	}//end while(bytesRead < length*PM_INSTR_SIZE)
	
}   //end ReadPM(WORD length, DWORD_VAL sourceAddr)
#endif

