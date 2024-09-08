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
	
void net_p24_config_set(P24_CONFIG_SET *cfg)
{
/*	
    typedef struct P24_CONFIG_SET_s
    {
        P24Protocol_HDR hdr;
        int8u 	cmd1;
        int8u 	cmd2;		
		int16u 	bits;
	} P24_CONFIG_SET;
*/	

//	cfg.
//	printf("p24_config_set %d %d %d\n", cfg->cmd1, cfg->cmd2, cfg->cmd3);
	
	switch(cfg->cmd1)
	{
//		#if defined( Uses_Console )
//		case IGS_SET_PIC24_OTA_CONSOLE:
//		{
//			console_toggle();
//			break;
//		}
		
		#if defined( Uses_Uart2 ) && !defined(Uses_LLC_Bridge)
		case IGS_SET_PIC24_JST_CONSOLE:
		{
			char c = !dcb1.pic24_jst_debug;
			net_jst_uart_set(c);
			break;
		}
		#endif
	
//		case IGS_SET_PIC24_EM250_ECHO:
//		{
//			dcb1.pic24_em250_debug = !dcb1.pic24_em250_debug;
//			break;
//		}

//		case IGS_SET_PIC24_SERIAL_PROTOCOL:
//		{
//			dcb1.pic24_serial_proto_debug = !dcb1.pic24_serial_proto_debug;
//			break;
//		}
		
//		case IGS_SET_PIC24_WIMESH:
//		{
//			dcb1.pic24_wimesh_debug = !dcb1.pic24_wimesh_debug;
//			console_print_command(TEXT_WIMESH, 1, dcb1.pic24_wimesh_debug);			
//			break;
//		}
		
		case IGS_SET_PIC24_RUN_CLOCK:
		{
			rcb.bit.console_clock =	!rcb.bit.console_clock;
//			console_print_command(TEXT_CLOCK, 0, dcb1.pic24_clock);
			break;
		}
//		#endif

/*		
		case IGS_SET_PIC24_EM250_WDT:
		{
			dcb1.pic24_em250_wdt_debug = !dcb1.pic24_em250_wdt_debug;
			#if defined( Uses_Debug )
			printf("EM250 WDT=%d\n",dcb1.pic24_em250_wdt_debug);
			#endif
			break;
		}
*/		
		case IGS_SET_EM250_RESET:
		{
			NET_EM250_WDT_reset();
			break;
		}

//		case IGS_SET_PIC24_DASHBOARD:
//		{
//			ecb1.telemetry = !ecb1.telemetry;
//			#if defined( Uses_Debug )
//			printf("Dash=%d\n",ecb1.dashboard);
//			#endif
//			break;
//		}

		
//		case IGS_SET_PIC24_LOCKUP_TEST:
//		{
//			pcb1.p24_lockup_test_timer = 6;
//			#if defined( Uses_Debug )
//			printf("Lockup=%d\n", cfg->cmd1);
//			#endif
//			break;
//		}
//
		
/*		
		case IGS_SET_PIC24_BOOTLOAD_DEBUG:
		{
			#if defined( Uses_Debug )
			printf("[P24-BOOT] DEBUG\n");
			#endif
			break;
		}
*/
		
	#if defined(Uses_LLC_Bridge)
	case IGS_SET_PIC24_LLC_FORWARD:
		{
			if (llc.bit.forward)
			{
				llc.bit.forward = 0;
				LLC_Bridge_net_putstr("FWD OFF");
			}
			else
			{
				llc.bit.forward = 1;
				LLC_Bridge_net_putstr("FWD ON");
			}
			break;
		}
		
		case IGS_SET_PIC24_LLC_SYNC:
		{
			if (llc.bit.auto_sync)
			{
				llc.bit.auto_sync = 0;
				LLC_Bridge_net_putstr("AutoSync OFF");
			}
			else
			{
				llc.bit.auto_sync = 1;
				LLC_Bridge_net_putstr("AutoSync ON");				
			}

			break;
		}
	#endif	

		#if defined( Uses_Debug_PWM )
		case IGS_SET_PIC24_SET_PWM_MODE:
		{
			PWM_Mode(cfg->cmd3);
			break;
		}
		
		case IGS_SET_PIC24_SET_PWM_TEST1:
		{
			PWM_Test1(cfg->cmd3);
			break;
		}

		case IGS_SET_PIC24_SET_BUCK:
		{
//			PWM_Set_Buck(cfg->cmd3);
			break;
		}

		case IGS_SET_PIC24_SET_TEST:
		{
//			PWM_Set_Test(cfg->cmd3);
			break;
		}

		case IGS_SET_PIC24_SET_UFB:
		{
//			PWM_Set_UFB(cfg->cmd3);
			break;
		}
		
//		case IGS_SET_PIC24_SET_PWM2_FRQ_UP:
//		{
//			HAL_PWM_Buck_Set_Freq2_Up();
//			break;
//		}
		
//		case IGS_SET_PIC24_SET_PWM2_FRQ_DN:
//		{
//			HAL_PWM_Buck_Set_Freq2_Dn();
//			break;			
//		}
//		case IGS_SET_PIC24_SET_PWM2_DTY_UP:
//		{
//			HAL_PWM_Buck_Set_Duty_Up(0);
//			break;
//		}
		
//		case IGS_SET_PIC24_SET_PWM2_DTY_DN:
//		{
//			HAL_PWM_Buck_Set_Duty_Dn(0);
//			break;
//		}

//		case IGS_SET_PIC24_SET_ADC_TMR_GRID_DN:
//		{
//			GEO_Config.gridSampleTime -= 100;
//			Uint16 gms = CALC_TMR1_US(GEO_Config.gridSampleTime);
//			printf("AC-:%u %u \n",GEO_Config.gridSampleTime,gms);
//			break;
//		}

//		case IGS_SET_PIC24_SET_ADC_TMR_GRID_UP:
//		{
//			GEO_Config.gridSampleTime += 100;
//			Uint16 gms = CALC_TMR1_US(GEO_Config.gridSampleTime);
//			printf("AC+:%u %u \n", GEO_Config.gridSampleTime,gms);
//			break;
//		}
		
		
		#endif
		
	}
}

void NET_EM250_reset()
{
	int value;

	Pic24_IO_Unock();
	IO_PIN_EM250_RESET_TRIS = 0;
	IO_PIN_EM250_RESET = 0;			// Active Low resets 
	for (value = 0; value < 100; value++)
	{
		IO_PIN_EM250_RESET = 0;			// Active Low resets 
	}
	IO_PIN_EM250_RESET = 1;			// HI is normal state 
	IO_PIN_EM250_RESET_TRIS = 1;	// Disable output 	
	Pic24_IO_Lock();
	
	rcb.em250_wdt_timer = 0;
}

void NET_EM250_WDT_reset()
{
	rcb.em250_wdt_timer = 0;
}


void NET_EM250_WDT_Task_Sec()
{
	rcb.em250_wdt_timer++;
	
	#define	EM250_WDT_SECS_MAX		60*60	// DGA 60 minutes 
	
	#if defined( Uses_Debug_Net )
	if (pcb1.em250_wdt_timer == EM250_WDT_SECS_MAX)
	{
		printf("EM250 WDT TMO:%d\n", pcb1.em250_wdt_timer);
	}
	#endif
	
	if (rcb.em250_wdt_timer > EM250_WDT_SECS_MAX)
	{
		rcb.em250_wdt_timer = 0;
		NET_EM250_reset();
	}
}
void NET_handle_input(Uint8 *cmd, Uint8 asize)
{
	#if defined(Uses_LLC_Bridge)
	
	if 	( 
		(cmd[0] == 'L') && (cmd[1] == 'L') && (cmd[2] == 'C') && (cmd[3] == ':') && (cmd[4] == ' ')
		)
	{
		LLC_Bridge_handle_ota( &cmd[5] );
		return;
	}
	#endif

	#if defined(Uses_Console)
	console_handle_input(cmd);
	#endif
	
}
void NET_RPC(Uint8 cmd1, Uint16 v1, Uint16 v2, Uint8 *data, Uint8 size)
{
/*	
    typedef struct Pic24RpcHdr_s
    {
        int8u cmd;
        int8u len;
		int16u v1;
		int16u v2;
    } Pic24RpcHdr;
*/	
//	printf("P24 RPC %d %d %d SZ:%d \n",cmd1, v1, v2, size);
//	if (cmd1==12)
//		printf("[%s]\n",data);

	switch(cmd1)
	{
		case P24_RPC_SPI:
		{
			HAL_EEPROM_spi_rpc(v1,v2,data,size);
			break;
		}
	}
	
}

void NET_RX_Packet_Binary(Uint8 *src, Uint16 asize)
{
	EM250_PROTOCOL_HDR *hdr = (EM250_PROTOCOL_HDR *) src;
	
//	#if defined( Uses_Debug4 )	
//	if (dcb1.pic24_serial_proto_debug)
//		printf("%s,%d  CMD:%d  SIZE:%d\n", __FILE__, __LINE__,  hdr->cmd, asize);
//	#endif
	
	NET_EM250_WDT_reset();

	switch(hdr->cmd)
    {
        case SERIAL_COM_PIC24_CONSOLE_IO:
        {
			Uint8 len = asize-2;
			Uint8 *data = &src[2];
			if (len==hdr->len)
			{
				data[hdr->len] = 0;
				NET_handle_input(data, strlen((char*)data));
			}
				
            break;
        }

		case SERIAL_COM_PIC24_CONFIG_SET:
        {
			P24_CONFIG_SET *cfg = (P24_CONFIG_SET *) src;
			net_p24_config_set(cfg);
            break;
        }
		
		case SERIAL_COM_EM250_KEEP_ALIVE:
        {
			NET_EM250_WDT_reset();
            break;
        }
		
		case SERIAL_COM_EM250_BOOTLOAD_STARTED:
        {
			rcb.em250_bootloader = 1;
//			em250_Bootloader_set();
            break;
        }

//**** RPC 2020 *********************************************					
		case SERIAL_COM_EM250_P24_RPC:
        {
			Pic24RpcHdr *hdr = (Pic24RpcHdr *) src;
			Uint8 len = asize-sizeof(Pic24RpcHdr);
			Uint8 *data = &src[sizeof(Pic24RpcHdr)];
			data[len] = 0;
			NET_RPC(hdr->cmd2,  hdr->v1, hdr->v2, data,len);
            break;
        }
		
	}
//**** RPC 2020 *********************************************			
}

int NET_is_bootloader_string(char *s)
{
/*
	EM250 Bootloader v20 b09
	1. upload ebl
	2. run
	3. ebl info
	BL >
*/
	if (strstr(s, "EM250 Bootloader")!=NULL)
		return 1;
	
	if (strstr(s, "1. upload")!=NULL)
		return 1;
	
	if (strstr(s, "2. run")!=NULL)
		return 1;
	
	if (strstr(s, "3. ebl info")!=NULL)
		return 1;
	
	if (strstr(s, "BL >")!=NULL)
		return 1;
	
	return 0;
}

void NET_RX_Packet_ascii(Uint8 *buf)
{
	Uint16 cmd;
	Uint16 size;
	
	#if defined(Uses_Debug2) 
	Uint16 vid;
	Uint16 sum;
	Uint16 i;
	#endif

	cmd = buf[0];
	size = strlen((char*) buf);

	#if defined(Uses_Debug2) 
	vid = buf[1];
	
	sum = 0;
	for (i = 0; i < size; i++)
	{
		sum += buf[i];
	}
	
//	EM250:PKT SZ:43 SUM:2877 3D Inverter3.c,625 
	if ((size == 42) && (sum == 2877))
	{
		printf("P24 RX TEST PACKET OK  %s,%d\n",__FILE__,__LINE__);
		return;
	}
	else
		printf("P24 RXPKT CMD:%X,%c VID:%X SZ:%d SUM:%d %s,%d\n",cmd,cmd,vid,size,sum,__FILE__,__LINE__);
	#endif

	#if defined (Uses_Debug_Net)
	char *ptr;
	#endif
	
	if (size==0)
	{
		return;
	}

	if (NET_is_bootloader_string((char*) buf))
	{
		rcb.em250_bootloader = 1;
		return;
	}

	if (rcb.em250_bootloader)
		return;
	
	#if defined (Uses_Debug_Net)
	ptr = strstr( (char*) buf, (char*) "WD=");
	if (ptr != NULL)
	{
		if (dcb1.pic24_em250_debug)
		{
			ptr = (char*) &buf[3];
			printf("EM250: %s\n", ptr);
		}
		return;
	}
//	if (dcb1.pic24_wimesh_debug)
//	{
//		printf("WMI_Handle_RxPacket Size=%d \n", (int) byteCount );
//		PrintHex((char*)buf, byteCount);
//	}
	#endif

//	printf("%s,%d EM250: cmd:%d (%s) \n", __FILE__, __LINE__, buf[0], &buf[3] );

	//Is it Command, Read Vid or Write Vid?
	switch( cmd )
	{
		case PIC24_COMMAND_PACKET:	// 99 'c'
		{	
			NET_EM250_WDT_reset();
			HAL_ProcessCommands(buf);
			break;
		}//case COMMAND_PACKET:

		case PIC24_READ_VID_PACKET:		// 114 'r'
		{	
			NET_EM250_WDT_reset();
			HAL_ProcessReadVID(buf);
			break;
		}//case READ_VID_PACKET:

		case PIC24_WRITE_VID_PACKET:	// 119 'w'
		{	
			#if defined (Uses_Debug_Net)
			if (dcb1.pic24_wimesh_debug)
				printf("HAL_Network WRITE_VID_PACKET\n");
			#endif
			NET_EM250_WDT_reset();
			HAL_ProcessWriteVID(buf);
			break;
		}//case WRITE_VID_PACKET:
		
//		default:
//		{
//			#if defined(Uses_Debug) 
//			printf("P24 RX-PKT ERROR-CMD=%d", cmd);
//			print_source();
//			#endif
//		}
			
	}
}
void NET_RX_char_ascii(Uint8 c)
{
//	printf("RX [%X] ST:%d SZ:%d [%c]\n", c, p24rx.state, p24rx.size, NET_GetAsciiPrn(c) );	
	
	if (c >= 32)
	{
		if (p24rx.size < sizeof(rxbuf)-1)
		{
			rxbuf[p24rx.size++] = c;
			rxbuf[p24rx.size] = 0;		//DGA make sure NULL terminated
		}
	}
	else if( (c == '\r') || (c == '\n') )
	{
		
//		printf("P24 RXPKT SZ:%d  %s,%d\n",p24rx.size,__FILE__,__LINE__);
		
		if (p24rx.size > 0)
		{
			NET_RX_Packet_ascii(  rxbuf);
		}
		p24rx.size = 0;  // Reset 
		memset(rxbuf, 0, sizeof(rxbuf));	// Clear memory buf
	}
}

Uint8 NET_GetAsciiPrn(Uint8 c)
{
	if ( (c < 32) || (c > 126))
		c = '*';
	return c;
}

void NET_RX_Protocol(Uint8 c)
{
//	#if defined( Uses_Debug )	
//	if (dcb1.pic24_serial_proto_debug)
//		printf("P24-COM [%X,%d] ST:%d SZ:%d [%c]\n", c, c,p24rx.state, p24rx.size, NET_GetAsciiPrn(c) );
//	#endif
	
	switch ( p24rx.state )
	{
		case RX_STATE_STX1:
		{
			if (c == BOOTLOADER_STX)
			{
				p24rx.state = RX_STATE_STX2;
				p24rx.size = 0;
				p24rx.chksum = 0;
				memset(rxbuf, 0, sizeof(rxbuf));	// Clear memory buf
			}
			else
			{
				#if defined(Uses_Debug_Net)
				if (dcb1.pic24_serial_proto_debug)
					printf(" \n");
				#endif
				NET_RX_char_ascii(c);  // DGA pass event to old code 
			}
			return;
		}
		case RX_STATE_STX2:
		{
			if (c == BOOTLOADER_STX)
			{
	         	p24rx.state = RX_STATE_DATA;
		        p24rx.size = 0;
				p24rx.chksum = 0;
				p24rx.bit.DLE = 0;
				return;
			}
			else
			{
				p24rx.state = RX_STATE_STX1;
				p24rx.bit.DLE = 0;
			}
			return;
		}
		case RX_STATE_DATA:
		{
			if ((p24rx.bit.DLE == 0) && (c==BOOTLOADER_STX))
			{
				p24rx.state = RX_STATE_DATA;
				p24rx.size = 0;
				p24rx.chksum = 0;
			}
			else if ((p24rx.bit.DLE==0) && (c==BOOTLOADER_ETX))
			{
				p24rx.size--;   // Subtract checksum from count
				p24rx.chksum = ~p24rx.chksum+1;
				
				if (p24rx.size)
				{
					#if defined(Uses_Debug_Net)
					if (dcb1.pic24_serial_proto_debug)
					{
						printf(" \n\n");
					}
					#endif
					NET_RX_Packet_Binary( rxbuf, p24rx.size);
					p24rx.size = 0;
								}
				p24rx.state = RX_STATE_STX1;
				return;
			}
			else if ((p24rx.bit.DLE==0) && (c==BOOTLOADER_DLE))
			{
				 p24rx.bit.DLE = 1;	// If DLE, treat next as data
			}
			else
			{
				if (p24rx.bit.DLE)
				{
					p24rx.bit.DLE = 0;
				}

				if (p24rx.size < sizeof(rxbuf))
				{
					rxbuf[p24rx.size++] = c;
					p24rx.chksum += c;
				}
				else
				{
					p24rx.state = RX_STATE_STX1;
					p24rx.chksum = 0;
					p24rx.size = 0;
				 }
			 }
			break;
		}
	}
}

void NET_Task()
{
	while ( Uart1_IsCharReady() != 0)
	{
		NET_RX_Protocol(Uart1_GetChar());
//		trace.cnt_uart1_rx++;
	}
}

void NET_TX_putc(Uint8 c)
{
	if (rcb.em250_bootloader==0)
		Uart1_PutChar(c);
}	

void NET_TX_protocol_Start()
{
	memset(&p24tx, 0, sizeof(p24tx));	// DGA clear structure 
	p24tx.bit.STX = 1;	// DGA set flag to indicate start was called.
	p24tx.chksum = 0;
	NET_TX_putc(BOOTLOADER_STX);
	NET_TX_putc(BOOTLOADER_STX);	
}

void NET_TX_protocol_putc_raw(Uint8 c)
{
	if(c == BOOTLOADER_STX || c == BOOTLOADER_ETX || c == BOOTLOADER_DLE)
	{
		NET_TX_putc(BOOTLOADER_DLE);
	}
	NET_TX_putc(c);
}

void NET_TX_protocol_putch(Uint8 c)
{
//	if (p24tx.bit.STX == 0)
//		pic24_TX_serial_protocol_start();
	NET_TX_protocol_putc_raw(c);
	p24tx.chksum += c;
	p24tx.size++;
}

void NET_TX_write(Uint8 *buf, Uint8 size)
{
	Uint8 i,c;
	for (i=0; i < size; i++)
	{
		c = buf[i];
		NET_TX_protocol_putch(c);
	}
}

void NET_TX_protocol_finish()
{
	p24tx.chksum = ~p24tx.chksum + 1;
	NET_TX_protocol_putc_raw(p24tx.chksum);
	NET_TX_putc(BOOTLOADER_ETX);	
	p24tx.bit.ETX = 1;
	p24tx.bit.STX = 0;	// Turn off start bit
}

void NET_TX_write_packet_raw(Uint8 *buf, Uint8 size)
{
	NET_TX_protocol_Start();
	NET_TX_write(buf, size);
	NET_TX_protocol_finish();
}

void NET_TX_write_packet(Uint8 cmd, Uint8 *buf, Uint8 size)
{
	EM250_PROTOCOL_HDR hdr;
	hdr.cmd = cmd;
	hdr.len = size + 2;
	NET_TX_protocol_Start();
	NET_TX_write((Uint8*)&hdr, 2);
	NET_TX_write(buf, size);
	NET_TX_protocol_finish();
}

void NET_Send_Boot_Info()
{
	Pic24AppBootHdr tx;
	tx.hdr.cmd = SERIAL_COM_BOOTLOADER_APP_START;
	tx.hdr.len = sizeof(Pic24AppBoot);
	tx.data.ver_major = FIRMWARE_VERSION_NUM_MAJOR;
	tx.data.ver_minor = FIRMWARE_VERSION_NUM_MINOR;
	strcpy((char *) tx.data.ver_text, (char *) app.text_firmare_ver);
	strcpy((char *)tx.data.product_text, (char *) VARIANT_TEXT);
//	printf("NET_Send_Boot_Info() %s %s\n\r", tx.data.ver_text, tx.data.product_text );
	NET_TX_write_packet_raw((Uint8 *) &tx, sizeof(tx));
}

void NET_console_write(char *s, int len)
{
	TX_AN1157_HDR hdr;
	hdr.cmd = SERIAL_COM_PIC24_CONSOLE_IO;
	hdr.len = len;
	if (hdr.len > 80)
		hdr.len= 80;
	NET_TX_protocol_Start();
	NET_TX_write((Uint8 *)&hdr, sizeof(hdr));
	NET_TX_write((Uint8 *)s, hdr.len);
	NET_TX_protocol_finish();
//	printf("net_putstr %s\n", s);
}

void NET_console_putstr(char *s)
{
	int len = strlen(s);
	NET_console_write(s, len);
}

void NET_set_flush(char value)
{
	net_flush = value;
}

void net_console_flush() 
{
	if (net_size)
	{
		NET_console_putstr(net_buf);
		net_size = 0;
		memset(net_buf,0,sizeof(net_buf));
	}		
}
void NET_console_delay() 
{
	net_console_flush();
//	APP_delay_miliseconds(1);
}

void NET_putch(char c) 
{
	if ( (net_flush==0) && (c < 32))
	{
		net_console_flush();
		return;
	}
	if (net_size >= sizeof(net_buf))
		net_console_flush();
	net_buf[net_size++] = c;
	net_buf[net_size] = 0;
}

void NET_telemetry_ota_write()
{
	P24_Telemetry p24;
	
	memset(&p24, 0, sizeof(p24));	// Clear
		
	memcpy( (void*)  &p24.rti.tm, (void*) &app.cpu_time, sizeof(p24.rti.tm));
		
	p24.rti.ver_major = FIRMWARE_VERSION_NUM_MAJOR;
	p24.rti.ver_minor = FIRMWARE_VERSION_NUM_MINOR;

	#if defined(Uses_Inverter)
	
	p24.acFreq = (freq.avg20);
	
//#if defined(Uses_Debug_Net)
//	printf("Freq=%d %u  %d\n", p24.acFreq, frequencyDGA, igs_freq );
//	#endif
	
//	p24.acVolt = ac.grid_volt_avg1;
	p24.acVolt = analog.grid.voltage;
	
	
	p24.hvdc = analog.dclink.voltage_avg1;
	p24.pvdc = analog.pvdc.volts10x;
	p24.mppt_dn_cnt = mppt.down.telem;
	p24.mppt_up_cnt = mppt.up.telem;
	p24.mppt_ramp = tune.mppt.ramp.pvdc;
	p24.mppt_down = tune.mppt.down.pvdc;
	p24.mppt_watts = mppt.watts;
	
	p24.mppt_delay_ramp = tune.mppt.ramp.delay;
	p24.mppt_delay_down = tune.mppt.down.delay;
			
	mppt.up.telem=0;
	mppt.down.telem=0;	
		
	p24.acPower = PWM_Get_Wattage();
	
//	p24.status = SVC_SS_GetReportedStatus();	
	p24.status = 0;
	p24.temp_c = temperature.celsius;
	p24.temp_f = temperature.fahrenheit;

	
	p24.lifeTimeW = kwh.lifeTimeW;
	p24.lifeTimeK = kwh.lifeTimeK;
	p24.alarms = HAL_Fault_Get();

	p24.amp = pwm.amp.scale;
	
    p24.pwm_freq = pwm.avg.prx2.num;
    p24.pwm_duty = pwm.avg.ocx1.num;
	if (p24.pwm_freq==0)
		p24.amp=0;
	
	#endif
            
	NET_TX_write_packet(SERIAL_COM_PIC24_IGS_TELEMETRY, (Uint8 *) &p24, sizeof(p24));
	
//	printf("Size=%d  \n",  sizeof(p24) );
	
	
}
