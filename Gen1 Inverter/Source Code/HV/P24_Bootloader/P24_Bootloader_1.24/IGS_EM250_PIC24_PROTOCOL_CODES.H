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



#if !defined(_IGS_EM250_PIC24_Protocol_Codes_H)
#define _IGS_EM250_PIC24_Protocol_Codes_H



	
	#define IGS_EM250_OTA_MAX_PAYLOAD_SIZE			80	

	
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
//	#define SERIAL_COM_BOOTLOADER_RD_FLASH 					1
//	#define SERIAL_COM_BOOTLOADER_WT_FLASH					2
//	#define SERIAL_COM_BOOTLOADER_ER_FLASH					3
//	#define SERIAL_COM_BOOTLOADER_RD_EEDATA					4
//	#define SERIAL_COM_BOOTLOADER_WT_EEDATA					5
//	#define SERIAL_COM_BOOTLOADER_RD_CONFIG					6
//	#define SERIAL_COM_BOOTLOADER_WT_CONFIG					7
	#define SERIAL_COM_BOOTLOADER_VERIFY_ERROR				7
	#define SERIAL_COM_BOOTLOADER_VERIFY					8	
	#define SERIAL_COM_BOOTLOADER_IGS_ERASE					9
	#define SERIAL_COM_BOOTLOADER_IGS_BOOT_APP				10
	#define SERIAL_COM_BOOTLOADER_IDLE_KEEP_ALIVE			11
	#define SERIAL_COM_BOOTLOADER_DEBUG_PRINT				12
	#define SERIAL_COM_BOOTLOADER_WT_FLASH_64				13
	#define SERIAL_COM_BOOTLOADER_APP_START					14
	#define SERIAL_COM_PIC24_CONFIG_SET    					15
	#define SERIAL_COM_PIC24_RESET      					16

//	**************************************************************************************************************	
//	SUB CODES FOR CONFIG
//	**************************************************************************************************************	
	#define IGS_SET_PIC24_OTA_CONSOLE  		1
   	#define IGS_SET_PIC24_JST_CONSOLE  		2
   	#define IGS_SET_PIC24_EM250_ECHO   		3
   	#define IGS_SET_PIC24_RESET		   		4
   	#define IGS_SET_PIC24_SERIAL_PROTOCOL	5
   	#define IGS_SET_PIC24_WIMESH	   		6
   	#define IGS_SET_PIC24_RUN_CLOCK			7	
	#define IGS_SET_PIC24_EM250_WDT			8
	#define IGS_SET_EM250_DEBUG        		9
	#define IGS_SET_EM250_DEBUG_SEC    		10
  	#define IGS_SET_EM250_RESET	     		11
	#define IGS_SET_PIC24_DASHBOARD			12			
	#define IGS_SET_PIC24_LLC_SYNC			13
	#define IGS_SET_PIC24_LLC_FORWARD		14
	#define IGS_SET_PIC24_LOCKUP_TEST		15
	#define IGS_SET_PIC24_BOOTLOAD_DEBUG1	16
	#define IGS_SET_PIC24_BOOTLOAD_DEBUG2	17
	#define IGS_SET_PIC24_BOOTLOAD_DEBUG3	18


    #define SERIAL_COM_PIC24_CONSOLE_IO   	    			16
	#define SERIAL_COM_PIC24_APP_KEEP_ALIVE 				17
	#define SERIAL_COM_EM250_KEEP_ALIVE 		 			18
//	**************************************************************************************************************	


//	**************************************************************************************************************	
//	EM250 OTA PACKET header VALUES
//	**************************************************************************************************************	
	#define	IGS_OFFSET_VERSION			0
	#define IGS_OFFSET_MESSAGE_TYPE		1
	#define IGS_MESSAGE_VERSION			1
	#define IGS_MESSAGE_TYPE			1
//	**************************************************************************************************************	



	#define IGS_PROTOCOL_CMD_SET_OFF				0
	#define IGS_PROTOCOL_CMD_SET_ON					1
	#define IGS_PROTOCOL_CMD_SET_TOGGLE				2
	#define IGS_PROTOCOL_CMD_GET					3
	#define IGS_PROTOCOL_CMD_DATA_ACK_4		   		4



	//	EM250 USB COMMANDS
	#define IGS_PROTOCOL_PAN_TX_TEST				1
	#define IGS_PROTOCOL_USB_EM250_DEBUG			2
	#define IGS_PROTOCOL_USB_BOOTLOAD_DATA_ACK		3
	#define IGS_PROTOCOL_USB_OTA_BL_TX_OK			4
	#define IGS_PROTOCOL_USB_OTA_BL_TX_ERROR		5

	//	EM250 OTA COMMANDS
	#define IGS_PROTOCOL_OTA_TX_TEST				21
	#define IGS_PROTOCOL_OTA_ACK					21
	#define IGS_PROTOCOL_OTA_EM250_DEBUG			22
	#define IGS_PROTOCOL_OTA_EM250_KEY				23
	#define IGS_PROTOCOL_OTA_EM250_BAUD				24
	#define IGS_PROTOCOL_EM250_CONFIG_SET			25
	#define IGS_PROTOCOL_EM250_APP_KEEP_ALIVE		26
	#define IGS_PROTOCOL_EM250_PING					27
	#define IGS_PROTOCOL_EM250_EMBER_BOOTLOADER		28
	#define IGS_PROTOCOL_EM250_CONSOLE_IO			29


	//	PIC24 COMMANDS
	#define IGS_PROTOCOL_PIC24_BOOTLOAD_STOP		51
	#define IGS_PROTOCOL_PIC24_BOOTLOAD_IDLE		52
	#define IGS_PROTOCOL_PIC24_BOOTLOAD_START		53
	#define IGS_PROTOCOL_PIC24_BOOTLOAD_DATA		54	
	#define IGS_PROTOCOL_PIC24_BOOTLOAD_DONE_OK		55
	#define IGS_PROTOCOL_PIC24_BOOTLOAD_SPEED_ACK	56
	#define IGS_PROTOCOL_PIC24_BOOTLOAD_WRITE_ACK	57
	#define IGS_PROTOCOL_PIC24_ERASE_FLASH			58
	#define IGS_PROTOCOL_PIC24_BOOTLOAD_VERIFY		59
	#define IGS_PROTOCOL_PIC24_BUFFER_ERROR         60
	#define IGS_PROTOCOL_PIC24_TEXT_INFO			61
	#define IGS_PROTOCOL_PIC24_ERASE_STATE_ERROR	62
	#define IGS_PROTOCOL_PIC24_APP_BOOT_OK			63
	#define IGS_PROTOCOL_PIC24_WRITE64_ACK			64
	#define IGS_PROTOCOL_PIC24_BOOTLOAD_XOFF		65
	#define IGS_PROTOCOL_PIC24_CONFIG_SET			66
	#define IGS_PROTOCOL_PIC24_CONSOLE_IO			67
	#define IGS_PROTOCOL_PIC24_SOFT_RESET			68
	#define IGS_PROTOCOL_PIC24_HARD_RESET			69




//		#define IGS_PROTOCOL_PIC24_DEVICE_READ_VID		0x0056
//		#define IGS_PROTOCOL_PIC24_DEVICE_WRITE_VID		0x0057



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

	
	

#define     IGS_SDP_ETX_CR		13
#define     IGS_SDP_STX			0x06			// If 1st byte is STX then it means the packet is new protocol
#define     IGS_SDP_ETX			0x05
#define     IGS_SDP_DLE			0x04



#endif