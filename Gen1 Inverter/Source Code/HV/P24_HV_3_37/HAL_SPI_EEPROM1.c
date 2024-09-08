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

	#define SPIBUF			SPI2BUF
	#define SPISTATbits		SPI2STATbits

/************************************************************************
* Function: HAL_EEPROM_Write_Enable()                                         *
*                                                                       *
* Preconditions: SPI module must be configured to operate with EEPROM.  *
*                                                                       *
* Overview: This function allows a writing into EEPROM. Must be called  *
* before every writing command.                                         *
*                                                                       *
************************************************************************/

//	#define EEPROM_SS_TRIS      (TRISBbits.TRISB9)      // PIC-01-RB9  to EPROM pin 1 CS/Chip_Select_input	
//	#define EEPROM_SS_PORT			(LATBbits.LATB9)         // PIC-01-RB9  to EPROM pin 1 CS/Chip_Select_input
//	#define EEPROM_HOLD_PORT		(LATAbits.LATA4)         // PIC-34-RA4  to EPROM pin 7 *Hold_Input 
//	#define EEPROM_WP_PORT			(LATCbits.LATC5)         // PIC-38-RC5  to EPROM pin 3 *Write_Protect_Input
//	#define HAL_EEPROM_SSLow()      EEPROM_SS_PORT = 0;
//	#define HAL_EEPROM_SSHigh()     EEPROM_SS_PORT = 1;


	
// send one byte of data and receive one back at the same time
unsigned char HAL_write_SPI( unsigned char i )
{
    SPIBUF = i;					// write to buffer for TX
	while(!SPISTATbits.SPIRBF);	// wait for transfer to complete
	return SPIBUF;    				// read the received value
}	

void HAL_EEPROM_Write_Enable()
{
	HAL_EEPROM_CS_Low();
	HAL_write_SPI(EEPROM_CMD_WREN);
	HAL_EEPROM_CS_High();
}

void HAL_EEPROM_Write_Disable()
{
    HAL_EEPROM_CS_Low();
    HAL_write_SPI(EEPROM_CMD_WRDI);
    HAL_EEPROM_CS_High();
}


/************************************************************************
* Function: HAL_EEPROM_Read_Status()                                          *
*                                                                       *
* Preconditions: SPI module must be configured to operate with  EEPROM. *
*                                                                       *
* Overview: This function reads status register from EEPROM.            *
*                                                                       *
* Input: None.                                                          *
*                                                                       *
* Output: Status register value.                                        *
*                                                                       *
************************************************************************/
Uint8 HAL_EEPROM_Read_Status()
{
	unsigned char value;
    HAL_EEPROM_CS_Low();
	HAL_write_SPI(EEPROM_CMD_RDSR);
    value = HAL_write_SPI(0);
    HAL_EEPROM_CS_High();
    return value;
}

Uint8 HAL_EEPROM_Read_Status_WIP()
{
	EEPROM_STATUS_REG status;
	status.byte = HAL_EEPROM_Read_Status();
	return status.bits.WIP;
}

void HAL_EEPROM_Write_Byte(unsigned int address, unsigned char data)
{
	rcb.bit.spi_tx=1;
	
	HAL_EEPROM_Write_Enable();
	
//	void HAL_EEPROM_Write_Enable()
//	{
//		HAL_EEPROM_CS_Low();
//		HAL_write_SPI(EEPROM_CMD_WREN);
//		HAL_EEPROM_CS_High();
//	}
	
    HAL_EEPROM_CS_Low();
	HAL_write_SPI(EEPROM_CMD_WRITE);
	
	HAL_write_SPI(address);
    HAL_write_SPI(data);

	HAL_EEPROM_CS_High();

    // wait for completion of previous write operation
	while( HAL_EEPROM_Read_Status_WIP()== 1)
	{
		
	}
	
	HAL_EEPROM_Write_Disable();
//	void HAL_EEPROM_Write_Disable()
//	{
//		HAL_EEPROM_CS_Low();
//		HAL_write_SPI(EEPROM_CMD_WRDI);
//		HAL_EEPROM_CS_High();
//	}
	
	
	rcb.bit.spi_tx=0;
}

