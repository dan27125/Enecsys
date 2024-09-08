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
	

	typedef struct P24_IO_Packet_s
	{
		TX_AN1157_HDR	hdr;
		char buf[100];
	} P24_IO_Packet;
 

#if defined(Uses_OTA_CONSOLE)
void __attribute__((optimize(s))) Put_CONSOLE_RAW(char *s1, char *s2)
{
	P24_IO_Packet tx;
	int len1,len2;
	memset(&tx, 0, sizeof(tx));
	len1 = strlen(s1);
	len2 = strlen(s2);
//	max_len = sizeof(tx.buf) -1;	
	tx.hdr.cmd =  SERIAL_COM_PIC24_CONSOLE_IO;
	if (len1)
	{
		strcpy(tx.buf, s1);
		tx.hdr.len = len1;
	}
	if (len2)
	{
		if (len1)
		{
			strcat(tx.buf, s2);
			tx.hdr.len = len1 + len2;
		}
		else
		{
			strcpy(tx.buf, s2);
			tx.hdr.len = len2;
		}
	}
	
	ZIG_Put_Packet((BYTE *) &tx, sizeof(tx));
}

void __attribute__((optimize(s))) Put_CONSOLE(char *text)
{
//	Put_Flush();
	Put_CONSOLE_RAW("[P24-BOOT] ", text);
}

void __attribute__((optimize(s))) Put_CONSOLE_DEBUG(char *text)
{
	if (cfg1.debug)
	{
		Put_CONSOLE(text);
		Uart1_TX_process(1);
	}
}

void __attribute__((optimize(s))) Put_CONSOLE_DEBUG_2(char *text)
{
	if (cfg1.debug2)
	{
		Put_CONSOLE(text);
		Uart1_TX_process(1);
	}
}


void __attribute__((optimize(2))) Put_Char(BYTE c)
{
	if (c < 32)
	{
		Put_Flush();
	}
	else
	{
		console_buf[console_len++] = c;
		console_buf[console_len] = 0;
	}
}

void __attribute__((optimize(1))) Put_Flush()
{
	if (console_len)
	{
		Put_CONSOLE((char*) console_buf);
		console_len = 0;
	}
}

void __attribute__((optimize(s))) Put_Str_len(char *s, int size)
{
	int i;
	for (i = 0; i < size; i++)
	{
		Put_Char( s[i]);
	}
}

void __attribute__((optimize(1))) Put_Str(char *s)
{
	int i, len;
	
	len = strlen(s);
	for (i = 0; i < len; i++)
	{
		Put_Char( s[i]);
	}
}

void __attribute__((optimize(s))) Put_Dec16(WORD num)
{
	char buf[32];
	int i;
	int rem;
	int len;
	WORD n;
	
	len = 0;
	n = num;

	while (n != 0)
	{
		len++;
		n /= 10;
	}
	
	for (i = 0; i < len; i++)
	{
		rem = num % 10;
		num = num / 10;
		buf[i] = rem + '0';
	}

	for (i = len-1; i >= 0; i--)
	{
		Put_Char( buf[i] );
	}
}

void __attribute__((optimize(s))) Put_Dec8(unsigned char dec)
{
    unsigned char res;
    unsigned char printed_already = 0;

    res = dec;

    if (res/100)
    {
        Put_Char( res/100 + '0' );
        printed_already = 1;
    }
    res = res - (res/100)*100;

    if ((res/10) || (printed_already == 1))
    {
        Put_Char( res/10 + '0' );
    }
    res = res - (res/10)*10;

    Put_Char( res + '0' );
}

void __attribute__((optimize(s))) Put_Dec(WORD num)
{
	if (num < 256)
		Put_Dec8(num);
	else
		Put_Dec16(num);
	Put_Char(' ');
}


void __attribute__((optimize(s))) Put_Boolean(int value)
{
    if (value==0)
        Put_Str("OFF");
    else
        Put_Str("ON");
}


const unsigned char CharacterArray[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

/*
void JST_PutHex(BYTE c )
{
    int printVar;
    printVar = c;
    c = (c>>4) & 0x0F;
    JST_PutChar( CharacterArray[c] );
    c = printVar & 0x0F;
    JST_PutChar( CharacterArray[c] );
}
*/

void __attribute__((optimize(s))) Put_Hex8(BYTE toPrint, BYTE ascii)
{
    unsigned int printVar;
	
    printVar = (toPrint>>4) & 0x0F;
    Put_Char( CharacterArray[printVar] );
	
    printVar = toPrint & 0x0F;
    Put_Char(CharacterArray[printVar]);
	
	if (ascii)
		Put_Char(ascii);
}

void __attribute__((optimize(s))) Put_Hex16(WORD toPrint )
{
    unsigned int printVar;
    printVar = (toPrint>>12) & 0x0F;
    Put_Char( CharacterArray[printVar] );
    printVar = (toPrint>>8) & 0x0F;
    Put_Char( CharacterArray[printVar] );
	Put_Hex8(toPrint, ' ');
}


	typedef union DWORD_mask_s
	{
		DWORD value32;
		WORD value16[2];
	} DWORD_mask;


void __attribute__((optimize(s))) Put_Hex32(DWORD toPrint)
{
    unsigned int printVar;
	DWORD toPrint2 = toPrint;
	Put_Str("0x");
    printVar = (toPrint>>28) & 0x0F;
	if (printVar)
	{
		Put_Char( CharacterArray[printVar] );
	}
    printVar = (toPrint>>24) & 0x0F;
    Put_Char( CharacterArray[printVar] );
    printVar = (toPrint>>20) & 0x0F;
    Put_Char( CharacterArray[printVar] );
    printVar = (toPrint>>16) & 0x0F;
    Put_Char( CharacterArray[printVar] );
	Put_Hex16(toPrint2);
}


#endif
