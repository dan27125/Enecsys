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


#if !defined(_Pic24_Bootloader_H)
#define _Pic24_Bootloader_H

#include <xc.h>
#include "GenericTypeDefs.h"
#include "Bootloader_Protocol.h"

	#define		Uses_JST_Diag_PCB_LED	1
//	#define		Uses_JST_Diag_PCB_SW2	1

	#define PM_INSTR_SIZE 		4		//bytes per instruction 
	#define PM_ROW_SIZE			256  	//user flash row size in bytes 
	#define PM_PAGE_SIZE 		2048 	// 0X800 user flash page size in bytes
	#define PM_ERASE_SIZE		1024	// DGA 

	#define BOOT_ADDR_LOW		0x200		// start of BL protection area
	#define APP_PAGE_FIRST		0x1000
	#define APP_PAGE_LAST		0xA400		// A400+800= AC00, AC00+800=B400, B400+800=BC00, BC00+800=C400, C400+800=CC00, CC00+800=D400

	#define BOOT_ADDR_HI  		APP_PAGE_FIRST-1

	#define APP_PROG_RESET    APP_PAGE_FIRST + 0x0200	// User app reset vector location
	#define DELAY_TIME_ADDR 	(APP_PROG_RESET + 0x02)   // BL entry delay location
	#define APP_PROG_START    (APP_PROG_RESET + 0x10)	//User app reset vector location

	#define BOOTLOADER_MAJOR_VERSION		1				// Bootloader major version
	#define BOOTLOADER_MINOR_VERSION		24				// Bootloader minor version

	#define DEV_HAS_WORD_WRITE	//Device has word write capability (24FJxxx devices)
	#define DEV_HAS_PPS			//Device has Peripheral Pin Select (PPS) feature

	#define USE_BOOT_PROTECT			//Use BL block protection 
	#define USE_CONFIGWORD_PROTECT		//Protect last page from writes/erases
	#define USE_VECTOR_PROTECT			//Use Reset and IVT protection

	#define FCY     12000000	// (Fosc/2)  6MHZ*PLL_4X=24MHZ FOSC

	#define UARTNUM     1       //Which device UART to use
	#define BAUDRATE    115200

	//Vector section is either 0 to 0x200 or 0 to end of first page, whichever is larger
	#define VECTOR_SECTION      ((0x200>(PM_PAGE_SIZE/2))?0x200:(PM_PAGE_SIZE/2)) 

	#define CONFIG_START 		0xABFC	
	#define CONFIG_END			0xABFE	

    #define PM_PAGE_ERASE 		0x4042	//NVM page erase opcode
    #define PM_ROW_WRITE 		0x4001	//NVM row write opcode

	#ifdef DEV_HAS_WORD_WRITE
	#define PM_WORD_WRITE		0x4003	//NVM word write opcode
	#endif

	#define		Uses_VERIFY				1
	#define		Uses_Auto_Reset			1
	#define		Uses_ERASE				1


	#define		Uses_OTA_CONSOLE		1

//	#define		Uses_Debug_Write		1
//	#define		Uses_Debug				1

	#define WDT_SW_ENABLE1()     RCONbits.SWDTEN = 1
	#define WDT_SW_DISABLE1()    RCONbits.SWDTEN = 0

	#define IO_PIN_ZIGBEE_PIC24_UPDATE_TRIS		(TRISBbits.TRISB11)		//DGA DEC 15, 2017 - PIC24 UPDATE SIGNAL FROM ZIGBEE ETRX 
	#define IO_PIN_ZIGBEE_PIC24_UPDATE			(PORTBbits.RB11)			//DGA DEC 15, 2017 - PIC24 UPDATE SIGNAL FROM ZIGBEE ETRX 
	
	#define	PutChar			PutChar2
	#define	Uses_Timer32

	#define	Uses_Timer32_1000_MS	1

	#define	TIMER_PR2_1_MS		12000
	#define	TIMER_PR3_1_MS		0

	#if !defined(_PUBLIC)
	#define _PUBLIC	extern
	#endif

	_PUBLIC WORD mins;
	_PUBLIC WORD ticks;
	_PUBLIC WORD ticks_ms;
	_PUBLIC WORD secs;
	_PUBLIC BYTE secFlag;
	_PUBLIC WORD write_cnt;
	_PUBLIC WORD write_timer;
	_PUBLIC BYTE blBusy;
	_PUBLIC DWORD_VAL app_Reset;	//user code reset vector
	_PUBLIC BYTE is_valid_app;
	_PUBLIC BYTE pic24_update;
	
	_PUBLIC P24_Bootloader_Config1 cfg1;
	
	_PUBLIC DWORD_VAL sourceAddr;	//general purpose address variable
	_PUBLIC DWORD_VAL app_Reset;	//user code reset vector
	_PUBLIC DWORD_VAL app_Timeout; 	//bootloader entry timeout value
	_PUBLIC WORD app_ResetRead;		//for relocating user reset vector
	_PUBLIC WORD dataCount;
	_PUBLIC BYTE uart2_enter_key;
	_PUBLIC BYTE verify_app_boot_flag;		
	_PUBLIC TPic24Com p24rx;
	_PUBLIC BYTE buffer[300];
	_PUBLIC WORD ota_total_blocks;
	_PUBLIC BYTE block_index[1024];	// Record of blocks received 
	_PUBLIC BYTE app_erase;		// Record of app being erased 
	_PUBLIC DWORD app_ota_sum; 	//check sum we got
	_PUBLIC DWORD srv_ota_sum; 	//check sum specified by server 
			
	
	#if defined(Uses_OTA_CONSOLE)
	_PUBLIC BYTE console_len;
	_PUBLIC BYTE console_buf[100];
	#endif
		
	#define	OTA_BACKUP_MAX		16
	#define	OTA_BUFFER_SIZE		64
	
	typedef struct P24_OTA_Block_Buffer_s
	{
		WORD bid[4];
		WORD addr[4];
		BYTE buf[OTA_BUFFER_SIZE*4];
		BYTE cnt;
		WORD bid_last;
		WORD base_addr;
	} P24_OTA_Block_Buffer;

	typedef struct P24_Out_Order_Block_Buffer_s
	{
		WORD bid[OTA_BACKUP_MAX];
		WORD addr[OTA_BACKUP_MAX];
		BYTE buf[OTA_BACKUP_MAX][OTA_BUFFER_SIZE];
		BYTE size;
	} P24_Out_Order_Block_Buffer;
	
	_PUBLIC P24_OTA_Block_Buffer ota_normal;

DWORD ReadLatch(WORD, WORD);
void Erase(WORD, WORD, WORD);
void WriteLatch(WORD, WORD, WORD, WORD);
void WriteMem(WORD);
void ResetDevice(WORD);
//	void ReadPM(WORD, DWORD_VAL);
void Write_PM(WORD _addr, BYTE *src, WORD size);
void ErasePM(WORD, DWORD_VAL);

int Verify_New_Bootload();

void Verify_Write_Timeout();

void Write_Buffer(WORD bid, BYTE *src);

#if defined(USE_BOOT_PROTECT) || defined(USE_RESET_SAVE)
void replaceBLReset(DWORD_VAL);
#endif

#define Pic24_IO_Unock()    __builtin_write_OSCCONL(OSCCON & 0xBF)
#define Pic24_IO_Lock()     __builtin_write_OSCCONL(OSCCON | 0x40)


void ZIG_PutChar(BYTE txChar);

void P24_IO_port_map();
void P24_Safe_Mode();
void Set_Timer_32();

void ZIG_Put_Packet(BYTE *buf, WORD responseLen);
int ZIG_pole_P24_bootload_mode_input();
void ZIG_Put_Reply(BYTE cmd, WORD bid, BYTE rc, WORD w1, WORD w2);

void GetChar(BYTE * ptrChar);
void task_run();
void Erase_Pic24();
void Boot_App();
void ResetDevice(WORD addr);

void Put_Keep_Alive(char force);
void handle_SerialData_task();
void handle_SerialData_RX_protocol(BYTE c);
void handle_SerialData_packet_protocol(BYTE *buf, WORD size);
void main_run();

void HAL_clear_OTA_Buffers();
void HAL_set_OTA_base_address(WORD addr);

void ota_backup_clear();
void ota_backup_write(WORD bid, BYTE *src);
WORD ota_backup_find(WORD bid);
int ota_backup_read(WORD bid, BYTE *dst);
int ota_backup_read_fid(WORD bid, BYTE fid);
void Write_Buffer_raw(WORD bid, BYTE *src, BYTE fid);

void task_1_ms();
void task_quit();
void task_Keep_Alive();
int Verify_New_Bootload();
void ZIG_Put_Reply(BYTE cmd, WORD bid, BYTE rc, WORD w1, WORD w2);

#if defined ( Uses_Debug )
void JST_PutChar(BYTE txChar);
void JST_Put_CR();
void JST_PutStr(BYTE *s, int size);
void JST_Puts_Warn(const char *s);
void JST_Puts(const char *s);
void JST_PutsCR(const char *s);
void JST_PutDec(unsigned char dec);
void JST_PutHex(BYTE c );
void JST_PutHexWord( WORD toPrint );
void JST_PutHex32(unsigned long toPrint );
void JST_PutDecWord(WORD num);
void em250_debug_print(BYTE *buf, BYTE size);
void Uart2_Print_Name_And_Copyright();
void Put_CRs(char cnt);
void handle_Uart2_RX_Keyboard(BYTE c);
void pic24_set_config(BYTE value);
#endif

	
	#if defined(Uses_OTA_CONSOLE)
	void Put_CONSOLE_RAW(char *text1, char *text2);
	void Put_CONSOLE(char *text);
	void Put_CONSOLE_DEBUG(char *text);
	void Put_CONSOLE_DEBUG_2(char *text);
	void Put_CONSOLE_DEBUG_3(char *text);

	void Put_Char(BYTE c);
	void Put_Flush();
	void Put_Str_len(char *s, int size);
	void Put_Str(char *s);
    void Put_Boolean(int value);
	void Put_Dec(WORD num);
	void Put_Hex8(BYTE toPrint, BYTE ascii);
	void Put_Hex16(WORD toPrint );
	void Put_Hex32(DWORD toPrint);
	void Print_Buffer(WORD bid, BYTE *src);
	void Print_BLK_Checksum(WORD bid);
	#endif
	

#endif		// _Pic24_Bootloader_H