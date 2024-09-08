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



#ifndef _SPI_EEPROM_H_
#define _SPI_EEPROM_H_


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


	#define SPIBUF              SPI2BUF
	#define SPISTATbits         SPI2STATbits


	#define EEPROM_CS_TRIS      (TRISBbits.TRISB9)      // PIC-01-RB9  to EPROM pin 1 CS/Chip_Select_input	
	#define EEPROM_CS_LATCH     (LATBbits.LATB9)         // PIC-01-RB9  to EPROM pin 1 CS/Chip_Select_input

	#define HAL_EEPROM_CS_Low()      EEPROM_CS_LATCH=0
	#define HAL_EEPROM_CS_High()     EEPROM_CS_LATCH=1


	#define EEPROM_HOLD_TRIS	(TRISAbits.TRISA4)      // PIC-34-RA4  to EPROM pin 7 *Hold_Input 
	#define EEPROM_HOLD_LATCH 	(LATAbits.LATA4)         // PIC-34-RA4  to EPROM pin 7 *Hold_Input 
	#define EEPROM_WP_TRIS      (TRISCbits.TRISC5)          // PIC-38-RC5  to EPROM pin 3 *Write_Protect_Input
	#define EEPROM_WP_LATCH 	(LATCbits.LATC5)             // PIC-38-RC5  to EPROM pin 3 *Write_Protect_Input
	// Rule of thumb: Always read inputs from PORTx and write outputs to LATx. If you need to read what you set an output to, read LATx

    #define  SPI_MODE8_ON           0x0000 // Communication is byte wide 
	#define  SPI_MODE_MASK          (~SPI_MODE16_ON)

	#define  SPI_CKE_ON             0x0100 // Transmit happens from active clock state to idle
	#define  SPI_CKE_OFF            0x0000 // Transmit happens on transition from idle clock state to active clock state 
	#define  SPI_CKE_MASK           (~SPI_CKE_ON)

	#define  CLK_POL_ACTIVE_LOW     0x0040  // Idle state for clock is high, active is low 
	#define  CLK_POL_ACTIVE_HIGH    0x0000  // Idle state for clock is low, active is high 
	#define  CLK_POL_ACTIVE_MASK    (~CLK_POL_ACTIVE_LOW)

	#define  MASTER_ENABLE_ON       0x0020 // Master Mode/
	#define  MASTER_ENABLE_OFF      0x0000  // Slave Mode 
	#define  MASTER_ENABLE_MASK     (~MASTER_ENABLE_ON)

	#define  SEC_PRESCAL_1_1        0x001c // Secondary Prescale 1:1   
	#define  SEC_PRESCAL_2_1        0x0018 // Secondary Prescale 2:1   
	#define  SEC_PRESCAL_3_1        0x0014 // Secondary Prescale 3:1   
	#define  SEC_PRESCAL_4_1        0x0010 // Secondary Prescale 4:1   
	#define  SEC_PRESCAL_MASK       (~SEC_PRESCAL_1_1)

	#define  PRI_PRESCAL_1_1        0x0003 // Primary Prescale 1:1    
	#define  PRI_PRESCAL_4_1        0x0002 // Primary Prescale 4:1    
	#define  PRI_PRESCAL_16_1       0x0001 // Primary Prescale 16:1   
//	#define  PRI_PRESCAL_64_1       0x0000 // Primary Prescale 64:1   
	#define  PRI_PRESCAL_MASK       (~PRI_PRESCAL_1_1)

	#define EEPROM_CMD_READ     (unsigned)0b00000011
	#define EEPROM_CMD_WRITE    (unsigned)0b00000010
	#define EEPROM_CMD_WRDI     (unsigned)0b00000100
	#define EEPROM_CMD_WREN     (unsigned)0b00000110
	#define EEPROM_CMD_RDSR     (unsigned)0b00000101
//	#define EEPROM_CMD_WRSR     (unsigned)0b00000001


	/************************************************************************
	* Structure STATREG and union _EEPROMStatus_                            *
	************************************************************************/

	typedef struct 
    {
        unsigned    WIP:1;
        unsigned    WEL:1;
        unsigned    BP0:1;
        unsigned    BP1:1;
        unsigned    RESERVED:3;
        unsigned    WPEN:1;
	} EEPROM_STATUS_BITS;
    
	typedef union 
    {
        EEPROM_STATUS_BITS  bits;
		struct 
        {
			unsigned char byte;
		};

	} EEPROM_STATUS_REG;
    
	typedef union {
		struct 
		{
			unsigned read1:1;
			unsigned read_done:1;
			unsigned write_busy:1;            
			unsigned kwtest:1;			
			unsigned rfu:5;
		};
		struct 
		{
			unsigned int value;
		};
	} SPI_RCB;

	#define Lo(X)   (unsigned char)(X&0x00ff)

	#define HAL_EEPROM_CS_LO()     EEPROM_CS_LATCH = 0;
	#define HAL_EEPROM_CS_HI()     EEPROM_CS_LATCH = 1;
	
	#define	EEPROM_CACHE_SIZE	256
	#define	EEPROM_WRITE_SIZE	256

    
	typedef enum
	{
        ES_IDLE=0,	
                
        ES_READ_SPI,
                
		ES_WRITE_ENABLE,
		ES_WRITE_CMD,	
		ES_WRITE_ADDRESS,
		ES_WRITE_DATA1,
        ES_WRITE_DATA2,
		ES_WRITE_STATUS1,
		ES_WRITE_STATUS2,
		ES_WRITE_STATUS3,
		ES_WRITE_DISABLE,
        ES_WRITE_DISABLE2,
        ES_WRITE_DONE,

        #if defined(Uses_SPI_Task_Read)                
    	ER_READ_START,	// 
    	ER_READ_START2,	//         
		ER_READ_ADDRESS,// 
    	ER_READ_DATA,
        #endif
	
	} EEPROM_STATE; 		

    
	typedef struct 
	{
		Uint8       cache [EEPROM_CACHE_SIZE];
		Uint8       write [EEPROM_WRITE_SIZE];
		Uint16      write_head;
		Uint16      write_tail;
		Uint8       state;
        
        Uint8       read_next;

		Uint8               waddr;
		Uint16              rdsz;
		Uint8               err;
		Uint8               spibuf;
        EEPROM_STATUS_REG   status;
        
		SPI_RCB        bits;
        
        
        #if defined(Uses_Debug_NVS_Write)
        Uint16              write_addr_max;
        #endif
        
        
	} EEPROM_Block;
	
	public volatile EEPROM_Block spi;

	void HAL_EEPROM_init();
	void HAL_EEPROM_task();
	void HAL_EEPROM_load();
	unsigned char HAL_EEPROM_read(Uint8 addr);
	void HAL_EEPROM_write(Uint8 addr,unsigned char data);
    void HAL_EEPROM_write_start(Uint8 addr);
    void HAL_EEPROM_write16(Uint8 addr, Uint16 data);
	void EEPROM_Put_WR(Uint8 addr, Uint8 c);
    Uint16 EEPROM_Get_WR_Size();
	void HAL_EEPROM_spi_rpc(Uint16 v1, Uint16 v2, Uint8 *data, Uint8 size);
	void HAL_EEPROM_spi_read();
	void HAL_EEPROM_write_wait();
    
    #if defined( Uses_Debug_NVS_SPI ) 
    void HAL_StoreByte(Uint8 data, Uint8 address);
    void HAL_EEPROM_dprint();
    #endif

#endif  //_SPI_EEPROM_H_