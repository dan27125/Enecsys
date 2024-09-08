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


#if !defined( _APP_P24_Flash_H )
#define	_APP_P24_Flash_H

	#define APP_PAGE_FIRST				0x1000
	#define APP_PAGE_LAST				0xA400		
	#define	OTA_BLOCK_SIZE				64
	#define	APP_PAGE_FIRST_OTA_BASE		(APP_PAGE_FIRST-(OTA_BLOCK_SIZE/2))
    #define APP_PROG_RESET    	APP_PAGE_FIRST + 0x0200	// User app reset vector location
    #define DELAY_TIME_ADDR 	(APP_PROG_RESET + 0x02)   // BL entry delay location


   	typedef struct 
	{
        Uint16 app_end_addr;
        Uint16 app_bid;
        Uint16 scan_end_addr;
        Uint16 scan_bid;
        DWORD chksum;
	} Flash_Control_Block;
	public Flash_Control_Block flash;

    DWORD P24_Flash_Read_Latch(WORD page, WORD addrLo);
    void P24_Flash_Read(int addr, BYTE *dst, WORD asize);
    Uint16 P24_get_address_from_bid(Uint16 bid);
    Uint16 P24_get_ota_id_from_address(Uint16 _addr);
    Uint16 P24_Flash_Read_End_Bid();
    Uint16 P24_Flash_Scan_End_Bid();
    Uint16 P24_Flash_Get_Block_Checksum(Uint16 bid);
    void P24_Flash_Get_Checksum();
    void P24_Flash_Init();

#endif  //  _APP_P24_Flash_H