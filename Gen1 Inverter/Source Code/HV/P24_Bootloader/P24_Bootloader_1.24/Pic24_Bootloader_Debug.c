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
	#include "boot2.h"


//	void Put_CONSOLE_DEBUG(char *text);



#if defined ( Uses_Debug_old )


void JST_Put_Chars(char c, char cnt)
{
	char i;
	for (i = 0; i < cnt; i++)
	{
		JST_PutChar(c);
	}
}

void JST_PutStr(BYTE *s, int size)
{
	int i;
	for (i = 0; i < size; i++)
	{
		JST_PutChar( s[i]);
	}
}

void JST_Puts_Warn(const char *s)
{
	JST_Put_CR();
	JST_Puts("WARNING ");
	JST_Puts(s);
	JST_Put_CR();
}

void JST_Puts(const char *s)
{
	int size = strlen( (char*) s);
	JST_PutStr((BYTE*)s,size);
}

void JST_PutsCR(const char *s)
{
	JST_Puts(s);
	JST_Put_CR();
}

void JST_PutDec(unsigned char dec)
{
    unsigned char res;
    unsigned char printed_already = 0;

    res = dec;

    if (res/100)
    {
        JST_PutChar( res/100 + '0' );
        printed_already = 1;
    }
    res = res - (res/100)*100;

    if ((res/10) || (printed_already == 1))
    {
        JST_PutChar( res/10 + '0' );
    }
    res = res - (res/10)*10;

    JST_PutChar( res + '0' );
}


void JST_PutDecWord(WORD num)
{
	char buf[32];
	
	int i, rem, len = 0, n;

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
//		PutChar1 (rem + '0');
	}

	for (i = len-1; i >= 0; i--)
	{
		JST_PutChar ( buf[i] );
	}
	
	JST_PutChar(' ');
}


const unsigned char CharacterArray[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

void JST_PutHex(BYTE c )
{
    int printVar;
    printVar = c;
    c = (c>>4) & 0x0F;
    JST_PutChar( CharacterArray[c] );
    c = printVar & 0x0F;
    JST_PutChar( CharacterArray[c] );
}

void JST_PutHexWord( WORD toPrint )
{
    unsigned int printVar;

    printVar = (toPrint>>12) & 0x0F;
    JST_PutChar( CharacterArray[printVar] );
    printVar = (toPrint>>8) & 0x0F;
    JST_PutChar( CharacterArray[printVar] );
	
    printVar = (toPrint>>4) & 0x0F;
    JST_PutChar( CharacterArray[printVar] );

    printVar = toPrint & 0x0F;
    JST_PutChar ( CharacterArray[printVar] );
}



void em250_debug_print(BYTE *s, BYTE len)
{
	s[len] = 0;
//	#if defined ( Uses_Debug )
	debug  = 1;
	JST_Puts( "EM250: ");
	JST_PutsCR((const char *)s);
//	#endif
	
}

void pic24_set_config(BYTE value)
{
	if (value)
	{
		if (debug==0)
		{
			debug  = 1;
			JST_PutsCR( "Debug enabled");
		}
	}
	else
	{
		debug  = 0;
	}
}

void handle_Uart2_RX_Keyboard(BYTE c)
{
	switch ( c )
	{
		case 13:
		{
			uart2_enter_key++;
//			debug = 1;
			Uart2_Print_Name_And_Copyright();
			Put_Keep_Alive(1);
			break;
		}
	}
	
}

void Uart2_Print_Name_And_Copyright()
{
	JST_Put_CRs(10);
//	JST_PutsCR( "IGS Bootloader");
	JST_PutsCR( "IGS PIC24 Bootloader - Made in USA - Enecsysparts.com");
	JST_PutsCR( "Copyright 2017 - Daniel G Ambrose - St Louis, Missouri");
	JST_Put_CR();
	Uart2_TX_process(1);		// Wait here until done transmitting 
}



#endif

