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

#if defined(Uses_Console)

	extern int __C30_UART;
	int __attribute__((__weak__, __section__(".libc")))
	
write(int handle, void *buffer, unsigned int len) 
{
	
	if (handle == CONSOLE_STDOUT)
	{
		int i;
			
		for ( i =0; i < len; i++)
		{
			
			
			char c = (((char*)buffer)[i]);
			{

//	***** 2022-01-19 CODE changed in line, Two function calls eliminated to speed up debugging 

				if ( (net_flush==0) && (c < 32))
				{
					ZNet_console_flush();
				}
				else
				{
					if (net_size >= sizeof(net_buf))
						ZNet_console_flush();
					net_buf[net_size++] = c;
					net_buf[net_size] = 0;				
				}
			}
	 
		}
     }

	return len;
}

void console_print_cr( Uint8 cnt)
{
	Uint8 i;
	for (i=0; i < cnt; i++)
	{
		printf(" \n");
	}
}

#if defined(Uses_Lockup_Test)
void p24_lockup_test()
{
	printf("*** Lockup Test *** \n" );
	for (;;)
	{
	}
}
#endif


#if defined (Uses_Debug_Old )
void console_print_hex(char *s, int size)
{
	int i;
	unsigned char c;
	printf("HEX ");
	for (i = 0; i < size; i++)
	{
		c = s[i];
		printf("%.2d[%.2X] ", i, c);
		if (i)
		{
			if (i%16==0)
			{
				printf("\nHEX ");
			}
		}
		
	}
	printf("\n");
}

#endif		//Uses_Debug


#if defined(Uses_Console)

void console_print_version()
{
	printf("%s\n",app.text_variant_ver);
}
void console_print_copyright()
{
	printf("%s\n",app.text_copyright);
}

void console_print_bit(char *desc, Uint8 value)
{
	if (value)
	{
		printf("%s %s\n", desc, TEXT_ON);
	}
	else
	{
		printf("%s %s\n", desc, TEXT_OFF);
	}
}

int console_scan_string_raw(Uint8 *str, TString_Info_Block *p)
{
	int i;
	int j;
	char eos_flag;

//	memset(p,0,sizeof(String_Info_Block));

	p->size = strlen( (const char*) str);

	j = 0;
	eos_flag = 0;

	for (i = 0; i < p->size; i++)
	{
		char c = str[i];

		if (c >= 127)
			continue;

		if (c >='a')
		{
			c &= ~(1 << 5);	//	DGA - Convert to upper case
		}

		if ( (c <= 32) || (c == '='))
		{
			if (j==0)
				continue;
			eos_flag++;		// Trigger EOS 
		}
		else
		{
			if (j < CONSOLE_MAX_ARG_SIZE)
			{
				p->str[p->cnt][j] = c;	// Copy char 
			}
			j++;		// Inc

			if (i >= (p->size -1) )
			{
				eos_flag++;		// Trigger EOS 
			}
		}


		if (eos_flag)
		{
			p->str[ p->cnt ][j] = 0;	// Null Term
			p->value[ p->cnt ] = atoi( (char*) p->str[ p->cnt] );

//			if (dcb1.console_cmd)
//				printf("Arg %d [%s] value=%d \n", (p->cnt+1), p->str[p->cnt], p->value[p->cnt] );


			eos_flag = 0;
			j = 0;
			p->cnt++;
			if (p->cnt >= CONSOLE_MAX_ARGS)
				break;
		}
	}

	if (p->cnt==0)
		return 1;

	return 0;
}

int strcmp2(const char *str1, const char *str2, Uint8 *arg)
{
	if (strcmp((char*)str1, (char*)arg)==0)
		return 0;
		
	if (strcmp((char*)str2, (char*)arg)==0)
		return 0;	

	return 1;
}

void console_clock_print()
{
	printf("P24 CLOCK %lu ", _boot_sec);
   	printf("%.2d:%.2d:%.2d:%.2d \n", cpu_time.day, cpu_time.hr, cpu_time.min, cpu_time.sec);
	
/*	
	if (ecb1.run)
	{
		printf("RUN");
	}
	else
	{
		#if defined(Uses_Diagnostics)
		if (APP_State_Get()==INVERTER_DIAG_MODE)
		{
			printf("DIAG");
		}
		else
			printf("STOP");
		#else
		printf("STOP");
		#endif
	}
*/
	
	#if defined(Uses_Debug_NVS_Clock)
	printf("RESET:%d\n", (NVS_Clock_Reset_Sec-nvs.run_sec));
	#else
//	APP_Inverter_console_print_status();
    #endif
	
}


void console_Task_Ms1000()
{
	#if defined(Uses_Console)
	if (rcb.bit.console_clock)
	{
		console_clock_print();
	}
	#endif
	
	#if defined(Uses_Debug)
	console_print_debug();
	Debug_trace_cnt_clear();	
	#endif
	
}


#endif	// Uses_Console


#endif		