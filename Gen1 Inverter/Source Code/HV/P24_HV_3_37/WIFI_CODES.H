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

#if !defined(_ZNET_EM250_PIC24_Protocol_Codes_H)
#define _ZNET_EM250_PIC24_Protocol_Codes_H

	
	#define ZNET_OTA_MAX_PAYLOAD_SIZE			80	

	
//	**************************************************************************************************************	
//	Serial COM Protocol Sliding window control characters  
//	**************************************************************************************************************	
	#define BOOTLOADER_STX 0x06		
	#define BOOTLOADER_DLE 0x05
	#define BOOTLOADER_ETX 0x04

	#define RX_STATE_STX1   0
	#define RX_STATE_STX2   1
	#define RX_STATE_DATA   2

	
	
//	**************************************************************************************************************	
//	Serial COM Protocol Commands
//	**************************************************************************************************************	
	#define SERIAL_COM_BOOTLOADER_RD_VER 					0
	#define SERIAL_COM_BOOTLOADER_RD_FLASH 					1
	#define SERIAL_COM_BOOTLOADER_WT_FLASH					2
	#define SERIAL_COM_BOOTLOADER_ER_FLASH					3
	#define SERIAL_COM_BOOTLOADER_RD_EEDATA					4
	#define SERIAL_COM_BOOTLOADER_WT_EEDATA					5
	#define SERIAL_COM_BOOTLOADER_RD_CONFIG					6
	#define SERIAL_COM_BOOTLOADER_WT_CONFIG					7
	#define SERIAL_COM_BOOTLOADER_VERIFY_OK					8
	#define SERIAL_COM_BOOTLOADER_ZNET_ERASE				9
	#define SERIAL_COM_BOOTLOADER_ZNET_BOOT_APP				10
	#define SERIAL_COM_BOOTLOADER_IDLE_KEEP_ALIVE			11
	#define SERIAL_COM_BOOTLOADER_DEBUG_PRINT				12
	#define SERIAL_COM_BOOTLOADER_WT_FLASH_64				13
	#define SERIAL_COM_BOOTLOADER_APP_START					14
	#define SERIAL_COM_PIC24_CONFIG_SET    					15
		
    #define SERIAL_COM_PIC24_CONSOLE_IO                     16
	#define SERIAL_COM_PIC24_ZNET_TELEMETRY                 17
	#define SERIAL_COM_EM250_KEEP_ALIVE                     18
	#define SERIAL_COM_EM250_BOOTLOAD_STARTED               19
	#define SERIAL_COM_EM250_LAUNCH_BOOTLOADER              20
	#define SERIAL_COM_PIC24_RPC                            21
	//	**************************************************************************************************************	
	//	SUB CODES FOR RPC
	//	**************************************************************************************************************	
		#define P24_RPC_SPI						1

//	**************************************************************************************************************	


	

//	**************************************************************************************************************	


//	**************************************************************************************************************	
//	SUB CODES FOR CONFIG
//	**************************************************************************************************************	
	#define ZNET_SET_PIC24_OTA_CONSOLE  		1
   	#define ZNET_SET_PIC24_JST_CONSOLE  		2
   	#define ZNET_SET_PIC24_EM250_ECHO   		3
   	#define ZNET_SET_PIC24_RESET		   		4
   	#define ZNET_SET_PIC24_SERIAL_PROTOCOL	5
   	#define ZNET_SET_PIC24_WIMESH	   		6
   	#define ZNET_SET_PIC24_RUN_CLOCK			7	
	#define ZNET_SET_PIC24_EM250_WDT			8
	#define ZNET_SET_EM250_DEBUG        		9
	#define ZNET_SET_EM250_DEBUG_SEC    		10
  	#define ZNET_SET_EM250_RESET	     		11
	#define ZNET_SET_PIC24_DASHBOARD			12			
	#define ZNET_SET_PIC24_LLC_SYNC			13			
	#define ZNET_SET_PIC24_LLC_FORWARD		14
	#define ZNET_SET_PIC24_LOCKUP_TEST		15
	#define ZNET_SET_PIC24_BOOTLOAD_DEBUG	16	

	#define ZNET_SET_PIC24_BOOTLOAD_DEBUG2	17
	#define ZNET_SET_PIC24_BOOTLOAD_DEBUG3	18

   	#define ZNET_SET_PIC24_SET_ADC_TMR_GRID_UP	24
   	#define ZNET_SET_PIC24_SET_ADC_TMR_GRID_DN   25
   	#define ZNET_SET_PIC24_SET_ADC_SAMC_DN   	26
    #define ZNET_SET_PIC24_SET_ADC_SAMC_UP   	27
    #define ZNET_SET_PIC24_SET_ADC_ADCS_UP   	28
    #define ZNET_SET_PIC24_SET_ADC_ADCS_DN   	29

    #define ZNET_SET_PIC24_SET_PWM2_FRQ_UP  		31
    #define ZNET_SET_PIC24_SET_PWM2_FRQ_DN   	32
    #define ZNET_SET_PIC24_SET_PWM2_DTY_UP   	33
    #define ZNET_SET_PIC24_SET_PWM2_DTY_DN   	34

    #define ZNET_SET_PIC24_SET_PWM_MODE			35
    #define ZNET_SET_PIC24_SET_PWM_TEST1 		36
	#define ZNET_SET_PIC24_SET_BUCK				37
   	#define ZNET_SET_PIC24_SET_TEST				38
//	#define ZNET_SET_PIC24_SET_UFB				39
//	#define ZNET_SET_PIC24_SET_DCV_TELEM        40
	#define ZNET_SET_PIC24_SET_BUCK_DEBUG       41


//	**************************************************************************************************************	
//	EM250 OTA PACKET header VALUES
//	**************************************************************************************************************	
	#define	ZNET_OFFSET_VERSION			0
	#define ZNET_OFFSET_MESSAGE_TYPE		1
	#define ZNET_MESSAGE_VERSION			1
	#define ZNET_MESSAGE_TYPE			1
//	**************************************************************************************************************	

	#define ZNET_PROTOCOL_CMD_SET_OFF				0
	#define ZNET_PROTOCOL_CMD_SET_ON					1
	#define ZNET_PROTOCOL_CMD_SET_TOGGLE				2
	#define ZNET_PROTOCOL_CMD_GET					3
	#define ZNET_PROTOCOL_CMD_DATA_ACK_4		   		4



	//	EM250 USB COMMANDS
	#define ZNET_PROTOCOL_PAN_TX_TEST				1
	#define ZNET_PROTOCOL_USB_EM250_DEBUG			2
	#define ZNET_PROTOCOL_USB_BOOTLOAD_DATA_ACK		3
	#define ZNET_PROTOCOL_USB_OTA_BL_TX_OK			4
	#define ZNET_PROTOCOL_USB_OTA_BL_TX_ERROR		5

	//	EM250 OTA COMMANDS
	#define ZNET_PROTOCOL_OTA_TX_TEST				21
	#define ZNET_PROTOCOL_OTA_ACK					21
	#define ZNET_PROTOCOL_OTA_EM250_DEBUG			22
	#define ZNET_PROTOCOL_OTA_EM250_KEY				23
	#define ZNET_PROTOCOL_OTA_EM250_BAUD				24
	#define ZNET_PROTOCOL_EM250_CONFIG_SET			25
	#define ZNET_PROTOCOL_EM250_APP_KEEP_ALIVE		26
	#define ZNET_PROTOCOL_EM250_PING					27
	#define ZNET_PROTOCOL_EM250_EMBER_BOOTLOADER		28
	#define ZNET_PROTOCOL_EM250_CONSOLE_IO			29


	//	PIC24 COMMANDS
	#define ZNET_PROTOCOL_PIC24_BOOTLOAD_STOP		51
	#define ZNET_PROTOCOL_PIC24_BOOTLOAD_IDLE		52
	#define ZNET_PROTOCOL_PIC24_BOOTLOAD_START		53
	#define ZNET_PROTOCOL_PIC24_BOOTLOAD_DATA		54	
	#define ZNET_PROTOCOL_PIC24_BOOTLOAD_DONE_OK	55
	#define ZNET_PROTOCOL_PIC24_BOOTLOAD_SPEED_ACK	56
	#define ZNET_PROTOCOL_PIC24_BOOTLOAD_WRITE_ACK	57
	#define ZNET_PROTOCOL_PIC24_ERASE_FLASH			58
	#define ZNET_PROTOCOL_PIC24_BOOTLOAD_VERIFY		59
	#define ZNET_PROTOCOL_PIC24_BUFFER_ERROR        60
	#define ZNET_PROTOCOL_PIC24_TEXT_INFO			61
	#define ZNET_PROTOCOL_PIC24_ERASE_STATE_ERROR	62
	#define ZNET_PROTOCOL_PIC24_APP_BOOT_OK			63
	#define ZNET_PROTOCOL_PIC24_WRITE64_ACK			64
	#define ZNET_PROTOCOL_PIC24_BOOTLOAD_XOFF		65
//	#define ZNET_PROTOCOL_PIC24_CONFIG_SET			66
	#define ZNET_PROTOCOL_PIC24_CONSOLE_IO			67
	#define ZNET_PROTOCOL_PIC24_SOFT_RESET			68
	#define ZNET_PROTOCOL_PIC24_HARD_RESET			69
	#define ZNET_PROTOCOL_PIC24_WDT_SET				70  // This is a ZNet API only for it to control P24
	#define ZNET_PROTOCOL_PIC24_RPC					71	// DGA New 2022-10-23	


//	********************P24 RPC API ************************ DGA New 2022-10-23

	#define ZNET_P24_RPC_GET_FIRMWARE_VERSION		1
    #define ZNET_P24_RPC_FLASH_READ_BID 			2
    #define ZNET_P24_RPC_FLASH_READ_MEM     		3
    #define ZNET_P24_RPC_FLASH_READ_END     		4
    #define ZNET_P24_RPC_DC_TELEM1                  5
    #define ZNET_P24_RPC_DC_TELEM2                  6
    #define ZNET_P24_RPC_UFB_TEST                   7
    #define ZNET_P24_RPC_BUCK_TEST                  8

    #define ZNET_P24_RPC_RIP_DIV                    9
    #define ZNET_P24_RPC_ACV_DIV                    10
    #define ZNET_P24_RPC_ACV_DIV2                   11

    #define ZNET_P24_RPC_DCF_STATE             	   12
    #define ZNET_P24_RPC_DCF_PCENT             	   13

    #define ZNET_P24_RPC_BUCK_SET1                 14
    #define ZNET_P24_RPC_BUCK_SET2                 15




//	********************P24 RPC API *********************** DGA New 2022-10-23

	
	#define P24BL_STATE_NULL					0
	#define P24BL_STATE_START_ACK_EM250			1
	#define P24BL_STATE_START_ACK_PIC24			2
	#define P24BL_STATE_WRITE_WAIT_IGS			3
	#define P24BL_STATE_WRITE_DATA				4
	#define P24BL_STATE_WRITE_WAIT				5
	#define P24BL_STATE_ERASE_WAIT				6
	#define P24BL_STATE_ERASE_DONE				7
	#define P24BL_STATE_VERIFY					8
	#define P24BL_STATE_BOOT_WAIT				9	
	#define P24BL_STATE_DONE_OK					10
	

#define     ZNET_SDP_ETX_CR         13
#define     ZNET_SDP_STX			0x06			// If 1st byte is STX then it means the packet is new protocol
#define     ZNET_SDP_ETX			0x05
#define     ZNET_SDP_DLE			0x04

#endif