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

#if !defined(_IGS_EM250_PIC24_Protocol_H)
#define _IGS_EM250_PIC24_Protocol_H

#include "WIFI_CODES.H"

	#define	int16u			Uint16
	#define	int8u			Uint8

	#define	WORD			Uint16
	#define	BYTE			Uint8

typedef struct EmberEUI64_s
{
	int8u data[8];
} EmberEUI64;


#if !defined(__BORLANDC__)

typedef union int16_union_s
{
	int16u value;
	int8u buf[2];
} int16_union;

typedef struct IGS_EM250_Header_s
{
	int8u ver;	
	int8u type;	
	int16u bid;
	int16u rfu2;
	int8u cmd1;
	int8u cmd2;
	int8u rfu1;	
	int8u size;
} IGS_EM250_Header;

typedef struct IGS_EM250_Packet_s
{
	IGS_EM250_Header hdr;	// DGA header
	int8u buf[ZNET_OTA_MAX_PAYLOAD_SIZE+2]; 			// DGA payload buffer
} IGS_EM250_Packet;

typedef struct IGS_EM250_RxPacket_s
{
	EmberEUI64 eui;
	IGS_EM250_Packet x;
} IGS_EM250_Rx_Packet;

typedef struct TComBitField_s
{
	int8u STX	:1;
	int8u DLE	:1;	
	int8u ETX 	:1;
	int8u rfu 	:5;
} TComBitField;

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
	int8u size;
//	int16u size;
	int8u chksum;
} TPic24Com;

	typedef struct Pic24Addr24_s
	{
		int16u lo;
		int8u hi;
	} Pic24Addr24;

    typedef struct TX_AN1157_s
    {
        int8u cmd;
        int8u len;
		Pic24Addr24 adr;
	} TX_AN1157;


    typedef struct EM250_PROTOCOL_HDR_s
    {
        int8u cmd;
        int8u len;
	} EM250_PROTOCOL_HDR;
	
	
    typedef struct Pic24CmdHdr_s
    {
        int8u cmd;
        int8u len;
		Pic24Addr24 adr;
    } Pic24CmdHdr;
	#define		Pic24CmdHdr_SIZE		5
	
    typedef struct Pic24RpcHdr_s
    {
        int8u cmd;
        int8u len;
		int8u cmd2;
		int8u rfu1;	
		int16u v1;
		int16u v2;
    } Pic24RpcHdr;

//    DataCnt=261 - 5 =256
    typedef struct Pic24PageCmd_s
    {
	    Pic24CmdHdr hdr;
        int8u buf[256];
    } Pic24PageCmd;


    typedef struct P24Protocol_HDR_s
    {
        int8u cmd;
        int8u len;
	} P24Protocol_HDR;
	 
    typedef struct TX_AN1157_HDR_s
    {
        int8u cmd;
        int8u len;
	} TX_AN1157_HDR;

	    typedef struct TX_VERSION_s
    {
        int8u major;
        int8u minor;
	} TX_VERSION;
	
	typedef struct TPic24StatusBits_s
	{
		int8u app				:1;
		int8u em_blmode			:1;	
		int8u rfu1				:6;
		int8u rfu2;
	} TPic24StatusBits;
	
	typedef struct P24IdlePacket_s
	{
		TX_VERSION			ver_bl;
		int16u				sec;
		TPic24StatusBits	bit;
		WORD					apage_beg;
		WORD					apage_end;
		BYTE					text1[16];
	} P24IdlePacket;

	typedef struct OTA_P24IdlePacket_s
	{
		IGS_EM250_Header 	hdr;	// DGA header
		P24IdlePacket 		pay;
	} OTA_P24IdlePacket;

	typedef struct Pic24WriteReply_s
	{
		BYTE cmd;			// 1		
		BYTE result;		// 1		
		WORD bid;			//	2
		WORD buf_size;		//	2
		WORD flashed;		//	2		
	}  Pic24WriteReply;

	typedef struct Pic24_BL_KeepAlive_s
	{
		BYTE ver_major;		// 1		
		BYTE ver_minor;		// 1
		BYTE ver_text[16];	// 16
		WORD run_secs;			// 2
	}  Pic24_BL_KeepAlive;
	
	typedef struct Pic24AppBoot_s
	{
		int8u ver_major;		// 1		
		int8u ver_minor;		// 1
		int8u ver_text[16];	// 16
		int8u product_text[16];	// 16		
		WORD run_secs;			// 2
	}  Pic24AppBoot;

	typedef struct Pic24AppBootHdr_s
	{
		TX_AN1157_HDR	hdr;
		Pic24AppBoot	data;
	}  Pic24AppBootHdr;
    
    
    
	typedef struct P24_2011_Telemetry_s
	{
		WORD	RFU_device_id;	// 00   904 0x0388
		BYTE    RFU_status;     // 02
		WORD    rfu_dcPower;    // 03
		WORD    acPower;		// 05
		WORD    efficency;		// 07
		BYTE 	acFreq_;        // 09
		WORD  	acVolt;			// 10
		BYTE    temp;           // 12
		WORD 	lifeTimeW;      // 13
		WORD	lifeTimeK;      // 15
		WORD	alarm;          // 02
	} P24_2011_Telemetry;		// 19
	
	typedef struct CPU_Runtime_s
	{
		BYTE sec;			// 1		
		BYTE min;			// 1				
		BYTE hr;			// 1
		BYTE day;			// 1
	} CPU_Runtime;

	typedef struct Generic_KeepAlive_s
	{
		BYTE ver_major;		// 1
		BYTE ver_minor;		// 1
		CPU_Runtime	tm;		// 4
	}  Generic_KeepAlive;	// 6


	typedef struct 
    {
//      unsigned    priority:1; 
        unsigned    engineering:1;
//      unsigned    install:1;
        unsigned    rfu4:1;
        unsigned    rfu5:1;
        unsigned    rfu6:1;
        unsigned    rfu7:1;
        unsigned    rfu8:1;
//      unsigned    rfu:8;
	} P24_Telem1_Bits;
    
	typedef union 
    {
        P24_Telem1_Bits  bits;
        Uint16           value;
	} P24_Telem1;
	
	typedef struct P24_Telemetry_s
	{
		Generic_KeepAlive rti;	// 6
        WORD	tmprfu1;           // 2 
        WORD	rcon;           // 2 
        P24_Telem1 tcb1;        // 2
//		Sub total................ 12
        
		WORD	acVolt;			// 2
        WORD	mppt_dn_pcent;		// 2 
        
		WORD	hvdc;			// 2
		WORD	acPower;		// 2
		WORD	pvdc;			// 2
		WORD	temp_f;			// 2   ***********
		WORD	lifeTimeW;		// 2
		WORD	lifeTimeK;		// 2
		
		WORD	acFreq;			// 2	..............................................
		
		WORD	alarms;             // 2
        
		WORD	mppt_up_pcent;		// 2 
//		Sub total................ 22 + 12 = 34 
		
		WORD	pwm_freq;		// 1 
		BYTE	pwm_duty;		// 1
        BYTE	mppt_auto_dn_pcent;	// 1
        
		WORD	amp;			// 1
		
   		WORD mppt_timer;              // 
		WORD mppt_pv_start;           // 
        
		BYTE mppt_delay_ramp;
		BYTE mppt_delay_down;
        
        BYTE mppt_auto_mode;
        BYTE mppt_auto_up_pcent;
		WORD mppt_down;				// 02
		WORD mppt_ramp;				// 02
		WORD mppt_watts;			// 02
		BYTE mppt_up_cnt;			// 01
		BYTE mppt_dn_cnt;			// 01
        WORD mppt_trig;
	} P24_Telemetry;				// 58


	
	typedef struct EM250_to_Pic24_KeepAlive_s
	{
		P24Protocol_HDR hdr;
		Generic_KeepAlive em250;
	}  EM250_to_Pic24_KeepAlive;

	
typedef union {
	struct 
	{
        unsigned pic24_debug  :1;
        unsigned em250_debug  :1;
		  unsigned em250_console :1;		  
		  unsigned uart2_console :1;		  		  
        unsigned run    :1;
		  unsigned rfu		:3;
    };
	 
    struct {
        unsigned char value;
    };
	 
} Pic24_Eprom_Control_Bits11;

	
    typedef struct P24_CONFIG_SET_s
    {
        P24Protocol_HDR hdr;
        int8u 	cmd1;
        int8u 	bad_cmd2;		// This param is bad, not being passed, 2/8/2024 
		int16u 	cmd3;
	} P24_CONFIG_SET;


#endif


    
    
    

#endif