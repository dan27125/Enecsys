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



#if !defined(_Bootloader_Protocol_H)
#define _Bootloader_Protocol_H

#include "IGS_EM250_PIC24_PROTOCOL_CODES.H"

	#define	int8u		BYTE

	typedef struct Pic24Addr24_s
	{
		WORD lo;
		WORD hi;
	} Pic24Addr24;

    typedef struct Pic24CmdHdr_s
    {
        BYTE cmd;			// 1
        BYTE len;			// 1
		Pic24Addr24 adr;	// 3
		WORD value16;
    } Pic24CmdHdr;

	typedef struct Pic24WriteReply_s
	{
		BYTE cmd;			// 1		
		BYTE result;		// 1		
		WORD bid;			//	2
		WORD buf_size;		//	2
		WORD flashed;		//	2		
	}  Pic24WriteReply;


typedef struct TPic24ComBit_s
{
	int8u STX	:1;
	int8u DLE	:1;	
	int8u ETX 	:1;
	int8u rfu 	:5;
} TPic24ComBitField;


typedef struct TPic24Com_s
{
	TPic24ComBitField bit;
	int8u state;
	WORD size;
	int8u chksum;
} TPic24Com;


    typedef struct TX_AN1157_HDR_s
    {
        BYTE cmd;
        BYTE len;
	} TX_AN1157_HDR;

    typedef struct TX_VERSION_s
    {
        BYTE major;
        BYTE minor;
	} TX_VERSION;


	typedef struct TPic24StatusBits_s
	{
		int8u app				:1;
		int8u em_blmode		:1;
		int8u rfu1				:6;
		int8u	rfu2;
	} TPic24StatusBits;
	
	typedef struct P24IdlePacket_s
	{
		TX_VERSION			ver_bl;			// 2
		WORD					sec;				// 2
		TPic24StatusBits	bit;				// 1
		WORD					apage_beg;		// 2
		WORD					apage_end;		// 2
		BYTE					text1[16];		// 16
	} P24IdlePacket;

	typedef struct P24IdlePacketTX_s
	{
      TX_AN1157_HDR		hdr;				// 2
		P24IdlePacket		x;
	} P24IdlePacketTX;
	
	typedef struct P24_OTA_Block_s
	{
		WORD id;
		BYTE data[64];
	} P24_OTA_Block;

	typedef struct TX_P24_OTA_Block_s
	{
		TX_AN1157_HDR	hdr;		// 2
		P24_OTA_Block  x;
	} TX_P24_OTA_Block;
	


typedef union {
	struct 
	{
		unsigned debug			:1;
		unsigned debug2			:1;
		unsigned debug3			:1;
        unsigned rfu			:13;
    };
	 
    struct {
        unsigned int value;
    };
} P24_Bootloader_Config1;

	
#endif 