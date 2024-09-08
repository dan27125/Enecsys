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

//	#define	Uses_Debug_EEPROM


#include "APP_Inverter.h"

//	#define	Uses_Debug_SPI	1

/*
 EEPROM 25LC02A
    1   CS [Chip Select Input]      to Pic pin 1 /RP9
    2   SO [Serial Data Output]     to Pic pin 42Pic pin /RP6
    3   WP [Write-Protect]          to Pic pin 38/RP21
    4   VSS
    5   SI [Serial Data Input]      to Pic pin 37/RP20
    6   SCK [Serial Clock Input]    to Pic pin 36/RP19
    7   HOLD [Hold Input]           to Pic pin 34/T1CK
    8   VCC
 */

void HAL_EEPROM_Map_Pins()
{
    Pic24_IO_Unock();
    RPINR22bits.SDI2R = 6;		// Assign SDI2R to PIC pin 42-RB6 and EPROM pin 2 SDO
    _RP20R = 10;				// PIC pin 37/RP20 to SDO2/10 and EPROM pin 5 SDI 
    _RP19R = 11;				// PIC pin 36/RP19 to SCK2OUT/8 and EPROM pin 6 SCK
    Pic24_IO_Lock();
}
void HAL_EEPROM_init()
{
	HAL_EEPROM_Map_Pins();
	memset((void*)&spi,0,sizeof(spi));
	
    EEPROM_CS_LATCH = 1;     
    EEPROM_CS_TRIS = 0;
   
    EEPROM_HOLD_LATCH = 1;
    EEPROM_HOLD_TRIS = 0;
    EEPROM_WP_LATCH = 1;
    EEPROM_WP_TRIS = 0;

	HAL_ISR_SPI2_IF = 0;	// clear interrupt flag
	HAL_ISR_SPI2_IE = 0;	// enable interrupt

//    SPI1CON2			=	0x0000;
	
//	24/4=6
//	SPI2CON1 = SEC_PRESCAL_1_1 |     //1:1 secondary prescale
//		PRI_PRESCAL_1_1 |			 //4:1 primary prescale
//		PRI_PRESCAL_4_1 |			 //4:1 primary prescale
//      CLK_POL_ACTIVE_HIGH | //clock active high (CKP = 0)
//        SPI_CKE_ON          | //out changes active to inactive (CKE=1)
//        SPI_MODE8_ON        | //8-bit mode
 //      MASTER_ENABLE_ON;     //master mode
	

//	SPI2CON1			=	0x0012;	original speed
	
//	24/4=6/3=2	
	SPI2CON1bits.MSTEN= 1;		// MSTEN: Master Mode Enable bit
 	SPI2CON1bits.CKE=1;			// Clock Edge Select bit(3)
	SPI2CON1bits.CKP=0;			// CKP: Clock Polarity Select bit			
 	SPI2CON1bits.MODE16=0;		// 8 bit mode 
	SPI2CON1bits.SSEN=0;		// Slave Select Enable bit 
	SPI2CON1bits.SMP=0;			// Data Input Sample Phase 0=middle
	SPI2CON1bits.DISSDO=0;		// 0=Module
	SPI2CON1bits.DISSCK=0;		// 0 = Internal SPI clock is enabled


//	prescale 11=1:1,10=4:1 01=16:1 00=64:1	
//	[1]111	[2]110	[3]101	[4]	100	[5]011	[6]010	[7]001	[8]000	
//	DGA 2022-09-04 Change speed scale from 4:1 to 16:1	
	SPI2CON1bits.PPRE= 0B10;	// 10 = Primary prescale 4:1
//	SPI2CON1bits.PPRE= 0B01;	// 01 = Primary prescale 16:1	DGA 2022-09-04 Change speed scale from 4:1 to 16:1	
//	SPI2CON1bits.SPRE= 0B010;	// 010 = Secondary prescale 6:1
	SPI2CON1bits.SPRE= 0B000;	// 000 = Secondary prescale 8:1

	
	HAL_ISR_SPI2_IF = 0;	// clear interrupt flag	
	IPC8bits.SPI2IP=3;
	HAL_ISR_SPI2_IE = 1;	// enable interrupt
	SPI2STATbits.SPIEN = 1;  //Enable SPI Module	
}

void HAL_EEPROM_write(unsigned int addr, unsigned char c)
{
	if (addr < 256)	// bounds check guard 
	{
		unsigned int ptr;
		ptr = spi.write_head + 1;
		if (ptr == EEPROM_WRITE_SIZE)        // Wrap if needed
		{
		    ptr = 0;       // Wrap 
		}
		spi.write[ptr] = addr;// write address to buffer
		spi.write_head = ptr;	// update the head
		spi.cache[addr] = c;	// save data in cache buffer
		if (spi.state==ES_IDLE)
		{
			spi.waddr=addr; 		
			HAL_EEPROM_CS_LO();
			SPI2BUF=EEPROM_CMD_WREN; 	// write to buffer for TX
			spi.state=EW_ENABLE;
		}
		timer.spi_sec=0;
	}
}

void HAL_EEPROM_write16(Uint8 addr, Uint16 data)
{
	HAL_EEPROM_write(addr ,GET_U8_0_FROM_16(data) );
	HAL_EEPROM_write(addr+1 ,GET_U8_1_FROM_16(data) );
}

unsigned char HAL_EEPROM_read(unsigned int addr)
{
	return spi.cache[addr];	// read data in cache buffer
}
int EEPROM_WR_empty()
{
    if (spi.write_head == spi.write_tail)
    {	
        return 1;
    }
	return 0;
}
int EEPROM_Get_WR_Addr()
{
    if (spi.write_head == spi.write_tail)
	{
		return -1;
	}
	//Here if at least one spot in the TX buffer
	spi.write_tail++;      //increment tail pointer
	if (spi.write_tail == EEPROM_WRITE_SIZE)
	{
		spi.write_tail = 0; //wrap if needed
	}
	return spi.write[spi.write_tail];	// return addr in buffer
}
unsigned char EEPROM_Get_WR_Char(unsigned char adr)
{
	return spi.cache[adr];
}

Uint16 EEPROM_Get_WR_Size()
{
	int length;	
    if (spi.write_head == spi.write_tail)
    {	
        return 0;
    }
	if (spi.write_tail < spi.write_head)
		length = spi.write_head - spi.write_tail;
	
	else if(spi.write_tail > spi.write_head)
	{
		 length = EEPROM_WRITE_SIZE - spi.write_head + spi.write_tail;
	}
	else
	{
		length = 0;
	}
	return length;
}

void HAL_EEPROM_spi_isr()
{
	spi.spibuf=SPI2BUF;	// read 

	switch(spi.state)
	{
		case EW_ENABLE:	// EEPROM_CMD_WREN finished 
		{
			HAL_EEPROM_CS_HI();
			Nop();
			Nop();
			HAL_EEPROM_CS_LO();
			SPI2BUF=EEPROM_CMD_WRITE;
			spi.state=EW_CMD;
			spi.timer = 0;
			break;
		}
		case EW_CMD:	// EEPROM_CMD_WRITE finished
		{
			SPI2BUF=spi.waddr; // WRITE ADDRESS 
			spi.state=EW_ADDRESS;
			spi.timer = 0;
			break;
		}
		case EW_ADDRESS:	// ADDRESS finished
		{
			SPI2BUF = spi.cache[spi.waddr];	// Put data
			spi.state=EW_DATA;
			spi.timer = 0;
			break;
		}
		case EW_DATA:	// DATA finished
		{
			HAL_EEPROM_CS_HI();	// TRIGGER WRITE
			Nop();	// paranoia strikes again ... extra wait state 
			Nop();	// paranoia strikes again ... extra wait state 
			HAL_EEPROM_CS_LO();
			SPI2BUF=EEPROM_CMD_RDSR; // RDSR cmd 
			spi.state=EW_STATUS1;
			spi.timer = 0;
			break;
		}
		case EW_STATUS1:	// CMD_RDSR finished
		{
			spi.scnt++;
			SPI2BUF=0; 
			spi.spibuf=0;
			spi.state=EW_STATUS2;
			spi.timer = 0;
			break;
		}
		case EW_STATUS2:	// ZERO finished
		{
			spi.scnt++;
			spi.wstat=spi.spibuf;
			HAL_EEPROM_CS_HI();
			Nop();	// paranoia strikes again ... extra wait state 
			Nop();	// paranoia strikes again ... extra wait state 
			#define RDSR_WIP	0x01
			if ( (spi.wstat & RDSR_WIP) )
			{
				HAL_EEPROM_CS_LO();
				Nop();	// paranoia strikes again ... extra wait state 
				Nop();	// paranoia strikes again ... extra wait state 
				SPI2BUF=EEPROM_CMD_RDSR; // RDSR cmd 
				spi.state=EW_STATUS1;
			}
			else
			{
				HAL_EEPROM_CS_LO();
				Nop();	// paranoia strikes again ... extra wait state 
				Nop();	// paranoia strikes again ... extra wait state 
				spi.wcnt++;
				SPI2BUF=EEPROM_CMD_WRDI;
				spi.state=EW_DISABLE;
			}
			spi.timer = 0;
			break;
		}
		case EW_DISABLE:	// CMD_WRDI finished
		{
			HAL_EEPROM_CS_HI();
			if (spi.write_head == spi.write_tail)
			{
				spi.state=0;
			}
			else
			{
				spi.waddr=EEPROM_Get_WR_Addr();	// get address from FIFO,save in waddr 		
				spi.state=EW_ENABLE;
				HAL_EEPROM_CS_LO();
				SPI2BUF=EEPROM_CMD_WREN;	// continue
			}
			spi.timer = 0;
			break;
		}
//*******************************************************				
//*******************************************************		
		case ER_READ_START:
		{
			spi.timer = 0;
			spi.rdsz=0;
			spi.state=ER_READ_ADDRESS;
			HAL_EEPROM_CS_LO();
			Nop();
			SPI2BUF=0;
			break;
		}
		case ER_READ_ADDRESS:
		{
			spi.timer = 0;
			HAL_EEPROM_CS_LO();
			Nop();
			Nop();
			spi.state=ER_READ_DATA;
			SPI2BUF=0;	// Write dummy 
			break;
		}
		case ER_READ_DATA:
		{
			spi.timer = 0;
			HAL_EEPROM_CS_LO();
			Nop();
			Nop();
			spi.cache[spi.rdsz++]=spi.spibuf;	// save data, bump count 
			if (spi.rdsz < 256)
			{
				spi.state=ER_READ_DATA;	// act as NOP
				Nop();
				Nop();
				SPI2BUF=0;	// write dummy 
			}
			else
			{
				HAL_EEPROM_CS_HI();	// stop clock
				spi.state=0;
				spi.rdok++;
				spi.rcb.read_done=1;
			}
			break;
		}
		default:
		{
			HAL_EEPROM_CS_HI();
			spi.timer = 0;
			spi.state=0;
		}
	}

}

void __attribute__((interrupt, no_auto_psv)) _SPI2Interrupt()
{
//	if((SPI2STATbits.SPIRBF) && (spi.state))
	if(SPI2STATbits.SPIRBF)
	{
		if (rcb.bit.spi_tx==0)
			HAL_EEPROM_spi_isr();
	}
	HAL_ISR_SPI2_IF = 0;
}
void __attribute__((interrupt, no_auto_psv)) _SPI2ErrInterrupt()
{
	spi.err++;
	HAL_ISR_SPI2_IF = 0;
}
//=================================================================
//=================================================================

void HAL_EEPROM_spi_read()
{
	spi.state=ER_READ_START;
	HAL_EEPROM_CS_LO();
	SPI2BUF=EEPROM_CMD_READ;
}

void HAL_EEPROM_spi_wait()
{
	if (spi.rcb.read_done==0)
	{
		if (spi.state==0)
			HAL_EEPROM_spi_read();
	}
	while (spi.rcb.read_done==0)
	{
		Nop();
	}
}

void HAL_EEPROM_task()
{
	if (rcb.bit.initializing==0)
		return;
	
	if ((spi.state==EW_STATUS1) || (spi.state==EW_STATUS2))
	{
		if (spi.timer >= 10)
		{
			#if defined( Uses_Debug_SPI )
			printf("SPI STATUS TIMER ERROR STATE=%d\n",spi.state);
			#endif
			HAL_EEPROM_spi_isr();
		}
	}
	if ((spi.state) && (spi.timer >= 200))
	{
		#if defined( Uses_Debug_SPI )
		printf("%s,%d SPI TIMER ERR STATE=%d\n",__FILE__,__LINE__,spi.state);
		#endif
		HAL_EEPROM_spi_isr();
	}
	
	#if defined( Uses_Debug_SPI )
	if (spi.rdok)
	{
		printf("READ CACHE OK\n");
		spi.rdok=0;
	}
	#endif
}

void HAL_EEPROM_secs(void)
{
	if (rcb.bit.initializing==0)
		return;	
	
	#if defined( Uses_Debug_SPI )
	
//	if (spi.wcnt)
//	{
//		printf("WR-CNT:%d\n",spi.wcnt);
//		spi.wcnt=0;
//	}
//	if (spi.okcnt)
//	{
//		printf("WRITE OK-CNT:%d \n",spi.okcnt);
//		spi.okcnt=0;
//	}
	
//	if (spi_isr)
//	{
//		printf("SPI-ISR:%d\n",spi_isr);
//		spi_isr=0;
//	}
	
	if (spi.state) 
	{
		if (spi.state < ER_READ_START)
			printf("%s,%d SPI STATE:%d SC:%d WS:%x\n",__FILE__,__LINE__,spi.state,spi.scnt,spi.wstat);
		else if (spi.rdsz)
		{
			printf("SPI RD:%d\n",spi.rdsz);
		}
	}
	
	if (spi.err)
	{
		printf("warning: SPI ERR:%d \n",spi.err);
		spi.err=0;
	}

	if (spi.wcnt)
	{
		printf("WRITE CACHE:%d\n",spi.wcnt);
		spi.wcnt=0;
	}
	
//	if (!EEPROM_WR_empty())		// If FIFO is not empty 
//	{
//		int sz = EEPROM_Get_WR_Size();
//		if (sz > 2)
//			printf("%s,%d Warning: TX-FIFO:%d \n",__FILE__,__LINE__,sz);
//	}

	if (spi.rcb.kwtest)
		HAL_EEPROM_spi_write_test_kw_sec();
	#endif
}

void HAL_EEPROM_spi_rpc(Uint16 v1, Uint16 v2, Uint8 *data, Uint8 size)
{
//	printf("P24 SPI %d %d SZ:%d \n",v1, v2, size);
	#define RPC_SPI_WRITE	1
	#define RPC_SPI_READ	2

	switch(v1)
	{
		#if defined( Uses_Debug_SPI )
		case RPC_SPI_WRITE:
		{
//			HAL_EEPROM_spi_write_test();
			HAL_EEPROM_spi_write_test_kw();
			break;
		}
		#endif
		case RPC_SPI_READ:
		{
			#if defined(Uses_Debug_SPI)
			printf("RPC SPI READ %s,%d\n",__FILE__,__LINE__);
			HAL_EEPROM_spi_read_test_kw();
			#endif

			HAL_EEPROM_spi_read();
			break;
		}
	}
}