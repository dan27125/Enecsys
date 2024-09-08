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

	void HAL_EEPROM_write_start(unsigned char addr);

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
//	HAL_ISR_SPI2_IE = 0;	// enable interrupt
    __write_to_IEC(HAL_ISR_SPI2_IE=0);
	
	SPI2CON1 = SEC_PRESCAL_4_1 |     //1:1 secondary prescale
		PRI_PRESCAL_1_1 |     //1:1 primary prescale
        CLK_POL_ACTIVE_HIGH | //clock active high (CKP = 0)
        SPI_CKE_ON          | //out changes active to inactive (CKE=1)
        SPI_MODE8_ON        | //8-bit mode
        MASTER_ENABLE_ON;     //master mode
	
	HAL_ISR_SPI2_IF = 0;	// clear interrupt flag	
	IPC8bits.SPI2IP=3;
	
	SPI2STATbits.SPIEN = 1;  //Enable SPI Module	
//	HAL_ISR_SPI2_IE = 1;	// enable interrupt
}


void HAL_EEPROM_write16(Uint8 addr, Uint16 data)
{
	HAL_EEPROM_write(addr ,GET_U8_0_FROM_16(data) );
	HAL_EEPROM_write(addr+1 ,GET_U8_1_FROM_16(data) );
}

unsigned char HAL_EEPROM_read(Uint8 addr)
{
	return spi.cache[addr];	// read data in cache buffer
}

unsigned char EEPROM_Get_WR_Char(Uint8 adr)
{
	return spi.cache[adr];
}


#if defined(Uses_Debug_NVS_SPI)
Uint16 EEPROM_Get_WR_Size()
{
	Uint16 length;	
	
    if (spi.write_head == spi.write_tail)
    {	
        return 0;
    }
	
	if (spi.write_tail < spi.write_head)
	{
		length = spi.write_head - spi.write_tail;
		#if defined(Uses_Debug_NVS_SPI)
		printf("head=%u tail=%u len=%u %s,%d\n",spi.write_head,spi.write_tail,length,__FILE__,__LINE__);
		#endif
	}
	
	else if(spi.write_tail > spi.write_head)
	{
		 length = EEPROM_WRITE_SIZE - (spi.write_head + spi.write_tail);
		#if defined(Uses_Debug_NVS_SPI)
//		 Uint16 len2 = (EEPROM_WRITE_SIZE - spi.write_head) + spi.write_tail;
		 printf("head=%u tail=%u len=%u %s,%d\n",spi.write_head,spi.write_tail,length,__FILE__,__LINE__);
		#endif
	}
	else
	{
		length = 0;
		
		#if defined(Uses_Debug_NVS_SPI)
		 printf("3 head=%u tail=%u len=%u\n",spi.write_head,spi.write_tail,length);
		#endif
		
	}
	return length;
}
#endif

Uint16 EEPROM_Get_WR_Addr(Uint8 *dst)
{
    if (spi.write_head == spi.write_tail)
	{
		return 1;
	}
	
//	HAL_ISR_SPI2_IE = 0;	// enable interrupt
    __write_to_IEC(HAL_ISR_SPI2_IE=0);
	
	//Here if at least one spot in the TX buffer
	spi.write_tail++;      //increment tail pointer
	if (spi.write_tail == EEPROM_WRITE_SIZE)
	{
		spi.write_tail = 0; //wrap if needed
	}
	Uint8 addr = spi.write[spi.write_tail];	// return addr in buffer
//	HAL_ISR_SPI2_IE = 1;	// enable interrupt
    __write_to_IEC(HAL_ISR_SPI2_IE=1);
		
	*dst = addr;
	return 0;
}


#if defined( Uses_Debug_NVS_SPI ) 
char * spi_get_state_text(Uint8 state)
{
	switch(state)
	{
		case ES_IDLE:	// EEPROM_CMD_WREN finished 
		{
			return "IDLE";
		}
		
		case ES_READ_SPI:	
		{
			return "WRITE_SPI";
		}
		
		case ES_WRITE_ENABLE:	
		{
			return "ENABLE";
		}
		
	
		case ES_WRITE_CMD:
		{
			return "CMD";
		}
		case ES_WRITE_ADDRESS:	// ADDRESS finished
		{
			return "ADDRESS";
		}
		case ES_WRITE_DATA1:	// DATA finished
		{
			return "DATA1";
		}
		
		case ES_WRITE_STATUS1:	// CMD_RDSR finished
		{
			return "STATUS-1";
		}
		
		case ES_WRITE_STATUS2:	// CMD_RDSR finished
		{
			return "STATUS-2";
		}
		
		case ES_WRITE_STATUS3:	// ZERO finished
		{
			return "STATUS-3";
		}
		
		case ES_WRITE_DISABLE:	// CMD_WRDI finished
		{
			return "DISABLE";
		}
		
		case ES_WRITE_DONE:	// CMD_WRDI finished
		{
			return "DONE";
		}
		
//*******************************************************				
//*******************************************************		
		#if defined(Uses_SPI_Task_Read)
		case ER_READ_START:
		{
			return "READ_START";
		}
		case ER_READ_ADDRESS:
		{
			return "READ_ADDR";
		}
		case ER_READ_DATA:
		{
			return "READ_DATA1";
		}
		#endif
	}
	return "??";
}

void HAL_EEPROM_dprint()
{
	printf("%d-%s ",spi.state,spi_get_state_text(spi.state));
	printf("rdsz:%u ",spi.rdsz);
	printf("RBF=%d TBF=%d ", SPI2STATbits.SPIRBF, SPI2STATbits.SPITBF);
	printf("\n");
}
#endif

void HAL_EEPROM_write(Uint8 addr, unsigned char c)
{
	if (addr < 256)	// bounds check guard 
	{
		if (spi.state==ES_IDLE)
		{
			spi.cache[addr] = c;			// save data in cache buffer
			HAL_EEPROM_write_start(addr);
		}
		else
		{
			Uint16 ptr;
//			HAL_ISR_SPI2_IE = 0;	// disable IE/ISR
            __write_to_IEC(HAL_ISR_SPI2_IE=0);
			ptr = spi.write_head + 1;
			if (ptr == EEPROM_WRITE_SIZE)        // Wrap if needed
			{
			    ptr = 0;       // Wrap 
			}
			spi.write[ptr] = addr;// write address to buffer
			spi.write_head = ptr;	// update the head
			spi.cache[addr] = c;	// save data in cache buffer
//			HAL_ISR_SPI2_IE = 1;	// enable IE/ISR
            __write_to_IEC(HAL_ISR_SPI2_IE=1);
		}
		
		timer.spi_sec=0;
        
        #if defined(Uses_Debug_NVS_Write)
        if (spi.write_addr_max < addr)
        {
            spi.write_addr_max = addr;
        }
        #endif
        
	}
}

void HAL_EEPROM_read_SPI(Uint8 next_state)
{
	spi.read_next = next_state;
	spi.state = ES_READ_SPI;
}

void HAL_EEPROM_write_SPI(Uint8 value, Uint8 next_state)
{
	HAL_EEPROM_read_SPI(next_state);
//	while (!SPISTATbits.SPITBF);	// wait for transfer to complete
//	{
//		printf("SPI-TX-wait\n");
//	}
	SPIBUF = value;
}

void HAL_EEPROM_write_start(Uint8 addr)
{
//	#if defined(Uses_Debug_NVS_SPI)
//	Uint8 byte = spi.cache[addr];
//	printf("WRITE START ADDR:%X BYTE:%X\n",addr,byte);
//	#endif

	spi.waddr=addr; 		
	HAL_EEPROM_CS_LO();
	HAL_EEPROM_write_SPI(EEPROM_CMD_WREN, ES_WRITE_ENABLE);
	
}

void HAL_EEPROM_task()
{
	switch(spi.state)
	{
		case ES_READ_SPI:
		{
			if (SPISTATbits.SPIRBF);	// wait for transfer to complete
			{
				spi.spibuf = SPIBUF;
				spi.status.byte = spi.spibuf;
				spi.state = spi.read_next;
			}
			break;
		}
		
		case ES_WRITE_ENABLE:
		{
			#if defined(Uses_Debug_NVS_SPI)
//			printf("EW_ENABLE1  %s,%d\n",__FILE__,__LINE__);
			#endif
		
			spi.bits.write_busy=1;
			HAL_EEPROM_CS_High();
			spi.state=ES_WRITE_CMD;
			break;
		}

		case ES_WRITE_CMD:	// 
		{
			#if defined(Uses_Debug_NVS_SPI)
//			printf("CMD_WRITE1  %s,%d\n",__FILE__,__LINE__);
			#endif
			
			HAL_EEPROM_CS_Low();
			HAL_EEPROM_write_SPI(EEPROM_CMD_WRITE, ES_WRITE_ADDRESS);
			break;
		}

		case ES_WRITE_ADDRESS:
		{
			#if defined(Uses_Debug_NVS_SPI)
//			printf("EW_ADDRESS  %s,%d\n",__FILE__,__LINE__);
			#endif

			HAL_EEPROM_write_SPI(spi.waddr, ES_WRITE_DATA1);
			break;
		}

		case ES_WRITE_DATA1:
		{
			Uint8 byte = spi.cache[spi.waddr];
			#if defined(Uses_Debug_NVS_SPI)
//			printf("EW_DATA1:%X  @%d\n",byte, __LINE__);
			#endif
			
			HAL_EEPROM_write_SPI( byte, ES_WRITE_DATA2);
			break;
		}
		
		case ES_WRITE_DATA2:
		{
			#if defined(Uses_Debug_NVS_SPI)
//			printf("EW_DATA2  %s,%d\n",__FILE__,__LINE__);
			#endif

			HAL_EEPROM_CS_High();
			spi.state = ES_WRITE_STATUS1;
			break;
		}
			
		case ES_WRITE_STATUS1:
		{
			#if defined(Uses_Debug_NVS_SPI)
//			printf("EW_STATUS1  @%d\n",__LINE__);
			#endif

			HAL_EEPROM_CS_Low();
			HAL_EEPROM_write_SPI(EEPROM_CMD_RDSR, ES_WRITE_STATUS2);
			break;
		}
		case ES_WRITE_STATUS2:	// 
		{
			HAL_EEPROM_write_SPI(0, ES_WRITE_STATUS3);
			break;
		}
		case ES_WRITE_STATUS3:	// 
		{
			HAL_EEPROM_CS_High();
			if (spi.status.bits.WIP)
			{
				spi.state = ES_WRITE_STATUS1;
			}
			else
			{
				spi.state = ES_WRITE_DISABLE;
			}
			break;
		}
		
		case ES_WRITE_DISABLE:
		{
			HAL_EEPROM_CS_Low();
			HAL_EEPROM_write_SPI(EEPROM_CMD_WRDI, ES_WRITE_DISABLE2);
			break;
		}
		case ES_WRITE_DISABLE2:	// 
		{
			HAL_EEPROM_CS_High();
			if (spi.write_head == spi.write_tail)
			{
				spi.state=ES_WRITE_DONE;
			}
			else
			{
				Uint8 c;
				if (EEPROM_Get_WR_Addr(&c)==0)
				{
					HAL_EEPROM_write_start(c);
				}
				else
				{
					spi.state=ES_WRITE_DONE;
				}
			}
			break;
		}
		case ES_WRITE_DONE:
		{
			#if defined(Uses_Debug_NVS_SPI)
//			printf("EW_DONE  %s,%d\n",__FILE__,__LINE__);
			#endif
			spi.spibuf=SPI2BUF;	// clear
			spi.state=ES_IDLE;
			spi.bits.write_busy=0;
			spi.state=0;
			break;		
		}

		
		#if defined(Uses_SPI_Task_Read)
		case ER_READ_START:
		{
			#if defined(Uses_Debug_NVS_SPI)
			printf("READ_START\n");
			#endif

			spi.rdsz=0;
			spi.bits.read_done=0;
		//	HAL_EEPROM_CS_Low();
			HAL_EEPROM_CS_LO();
			HAL_EEPROM_write_SPI(EEPROM_CMD_READ, ER_READ_START2);
			break;
		}
		
		case ER_READ_START2:
		{
//			HAL_EEPROM_CS_Low();
			HAL_EEPROM_CS_LO();
			HAL_EEPROM_write_SPI(0, ER_READ_ADDRESS);		// 01
			break;
		}
		
		case ER_READ_ADDRESS:
		{
//			HAL_EEPROM_CS_Low();
			HAL_EEPROM_CS_LO();
			HAL_EEPROM_write_SPI(0, ER_READ_DATA);		// 01
			break;
		}
		
		case ER_READ_DATA:
		{
//			HAL_EEPROM_CS_Low();
			HAL_EEPROM_CS_LO();
			spi.cache[spi.rdsz]=spi.spibuf;	// save data
			spi.rdsz++;
			if (spi.rdsz < 256)
			{
				spi.read_next = ER_READ_DATA;
				spi.state = ES_READ_SPI;
//				HAL_EEPROM_write_SPI(0, ER_READ_DATA);
			}
			else
			{
				HAL_EEPROM_CS_High();
				spi.state=0;
				spi.bits.read_done=1;
			}
			break;
		}
		#endif
	}

}


//=================================================================
// send one byte of data and receive one back at the same time
unsigned char HAL_EEPROM_Read_SPI( Uint8 i )
{
    SPIBUF = i;					// write to buffer for TX
	while(!SPISTATbits.SPIRBF);	// wait for transfer to complete
	return SPIBUF;    				// read the received value
}
unsigned char HAL_EEPROM_Read_Byte(Uint8 addr)
{
	unsigned char value;
	HAL_EEPROM_CS_LO();
    HAL_EEPROM_Read_SPI(EEPROM_CMD_READ);
	HAL_EEPROM_Read_SPI(addr);
	value = HAL_EEPROM_Read_SPI(0);
	HAL_EEPROM_CS_High();
	return value;
}

void HAL_EEPROM_spi_read()
{
	spi.state=0;
	for (spi.rdsz = 0; spi.rdsz <= 255; spi.rdsz++)
	{
		unsigned char c = HAL_EEPROM_Read_Byte(spi.rdsz);
		spi.cache[spi.rdsz]= c;
	}
	spi.state=0;
	spi.bits.read_done=1;
}
//=================================================================

void HAL_EEPROM_spi_rpc(Uint16 v1, Uint16 v2, Uint8 *data, Uint8 size)
{
//	printf("P24 SPI %d %d SZ:%d \n",v1, v2, size);
	#define RPC_SPI_WRITE	1
	#define RPC_SPI_READ	2

	switch(v1)
	{
		#if defined( Uses_Debug_NVS_SPI )
		case RPC_SPI_WRITE:
		{
//			HAL_EEPROM_spi_write_test();
//			HAL_EEPROM_spi_write_test_kw();
			break;
		}
		#endif
		case RPC_SPI_READ:
		{
//			#if defined(Uses_Debug_NVS_SPI)
//			printf("RPC SPI READ %s,%d\n",__FILE__,__LINE__);
//			HAL_EEPROM_spi_read_test_kw();
//			#endif

			HAL_EEPROM_spi_read();
			break;
		}
	}
}

void __attribute__((interrupt, no_auto_psv)) _SPI2Interrupt()
{
	HAL_ISR_SPI2_IF = 0;
}
void __attribute__((interrupt, no_auto_psv)) _SPI2ErrInterrupt()
{
	spi.err++;
	HAL_ISR_SPI2_IF = 0;
}
