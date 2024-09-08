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

	typedef struct  __attribute__ ((packed)) P24_RPC_TX_Header_x
	{
		int8u cmd1;
		int16u v1;
		int16u v2;
	} __attribute__ ((packed)) P24_RPC_TX_Header;


void ZNet_P24_RPC_TX(Uint8 cmd1, Uint16 v1, Uint16 v2, Uint8 *data, Uint8 size)
{
	TX_AN1157_HDR		hdr;
	P24_RPC_TX_Header	tx;
	
	hdr.cmd = SERIAL_COM_PIC24_RPC;
	if (size > 64)
		size = 64;
	hdr.len = sizeof(tx) + size;
	
	tx.cmd1=cmd1;
	tx.v1 = v1;
	tx.v2 = v2;
	ZNet_TX_protocol_Start();
	ZNet_TX_write((Uint8 *)&hdr, sizeof(hdr));
	ZNet_TX_write((Uint8 *)&tx, sizeof(tx));
	ZNet_TX_write((Uint8 *)data, size);
	ZNet_TX_protocol_finish();	
}


void ZNet_Flash_Read_Addr(Uint16 addr)
{

	Uint8 buf[66],i;
	Uint16 sum;
	
	memset(buf,0,64);
	
	P24_Flash_Read(addr, buf, 64);
	
	sum =0;
	for (i = 0;i < 64; i++)
	{
		sum += buf[i];
	}
	
	printf("ADDR:%X SUM:%X\n",addr,sum);
	
	ZNet_P24_RPC_TX(ZNET_P24_RPC_FLASH_READ_MEM, addr, 0, buf, 64);

}

void ZNet_Flash_Read_Bid(Uint16 bid)
{
	Uint8 buf[64];
	Uint16 addr;
	addr = P24_get_address_from_bid(bid);
	P24_Flash_Read(addr, buf, 64);
	ZNet_P24_RPC_TX(ZNET_P24_RPC_FLASH_READ_BID, bid, flash.app_bid, buf, 64);
}


#if defined(Uses_Debug_PWM2)

void rpc_delay(Uint16 value)
{
    while (value)
    {
        value--;
    }
}

void RPC_Buck_Set()
{
    if (dcb.pwm.pwm1_lock)
    {  
        HAL_ISR_TMR4_PWM_SINE_IE=0;

        OC2CON = OCM2 | OCM1;
        PWM_SINE_PRX = PWM_SINE_PRX_46K;
        dcb.pwm.pwm1_duty = 2;
        PWM_SINE_DUTY = dcb.pwm.pwm1_duty;
        T2CON=DEF_TON;
        
        OC3CON = OCTSEL | OCM2 | OCM1;
        PWM_CLOCK_PRX = PWM_CLOCK_PRX_200K;
        PWM_CLOCK_DUTY = PWM_CLOCK_DUTY50;
        T3CON=DEF_TON;
        
//      HAL_ISR_TMR4_PWM_SINE_IF=0;
//      HAL_ISR_TMR4_PWM_SINE_IE=1;
        PWM_BUCK_LATCH_ON();
        printf("BUCK:ON FREQ:%d,%d DUTY:%d,%d  \n",PWM_SINE_PRX,PWM_CLOCK_PRX,PWM_SINE_DUTY,PWM_CLOCK_DUTY);
        printf("OC3CON:%x OCM:%u \n",OC3CON, OC3CONbits.OCM);

    }
    else
    {
        PWM_BUCK_LATCH_OFF();
        printf("BUCK:OFF\n");
//      PWM_CLOCK_DUTYZ = PWM_CLOCK_DUTY25;
//      PWM_SINE_DUTYZ = (PWM_SINE_PRXZ/4);
    }
        
}
#endif

#if defined(Uses_Debug_PWM2)
void RPC_Buck_Set_Duty()
{
    if (dcb.pwm.pwm1_duty > 10)
    {
        dcb.pwm.pwm1_duty=10;
    }
    else if (dcb.pwm.pwm1_duty < 0)
    {
        dcb.pwm.pwm1_duty=0;
    }
    
    PWM_SINE_DUTY = dcb.pwm.pwm1_duty;
    
    printf("BUCK CLOCK:%u,%u SINE:%u,%u\n",
            PWM_CLOCK_PRX, PWM_CLOCK_DUTY,
            PWM_SINE_PRX,  PWM_SINE_DUTY
    );
}
#endif


void ZNet_P24_RPC_RX(Uint8 cmd1, Uint16 v1, Uint16 v2, Uint8 *data, Uint8 size)
{
    #if defined(Uses_Debug_RPC)
    if (dcb1.rcp)
        printf("RPC %d %d %d SZ:%d \n",cmd1, v1, v2, size);
    #endif

	switch(cmd1)
	{
		case ZNET_P24_RPC_GET_FIRMWARE_VERSION:
		{
//			ZNet_ReadVID(VID_INVERTER_APP_FIRMWARE);
			DWORD test = 0x12345678;
			printf("BID:%d,%d  CHECK-SUM: %lX  TEST:%lx\n", flash.scan_bid,flash.app_bid, flash.chksum,test);	
			break;
		}
		
		case ZNET_P24_RPC_FLASH_READ_BID:
		{
			ZNet_Flash_Read_Bid(v1);
			break;
		}

		case ZNET_P24_RPC_FLASH_READ_MEM:
		{
			ZNet_Flash_Read_Addr(v1);
			break;
		}		

		case ZNET_P24_RPC_FLASH_READ_END:
		{
			if (v1)
			{
				P24_Flash_Read_End_Bid();
				P24_Flash_Scan_End_Bid();
				P24_Flash_Get_Checksum();
			}
			else
			{
				printf("BID:%d,%d  CHECK-SUM: %lX\n", flash.scan_bid,flash.app_bid,flash.chksum);	
			}
			break;
		}		


        #if defined(debug_dcv_set_telem)
        case ZNET_P24_RPC_DC_TELEM1:
		{
            debug_dcv_set_telem(v1,v2);
            break;
		}
        #endif


        #if defined(Uses_Debug_UFB)
        case ZNET_P24_RPC_UFB_TEST:
		{
            debug_ufb_set(v1);
            break;
		}
        #endif

        case ZNET_P24_RPC_RIP_DIV:
		{
            if (v1==0)
            {
                NVS_ripple_divsor_write(tune.fault.ripple_div-1);
            }
            else if (v1==1)
            {
                NVS_ripple_divsor_write(tune.fault.ripple_div+1);
            }            
            printf("RIP DIV:%u\n",tune.fault.ripple_div);
            break;
		}

        case ZNET_P24_RPC_ACV_DIV:
		{
            if (v1==0)
            {
                NVS_acv_divsor_write(tune.acv_div-1);
            }
            else if (v1==1)
            {
                NVS_acv_divsor_write(tune.acv_div+1);
            }            
            printf("ACV DIV:%u\n",tune.acv_div);
            break;
		}
        
/*
        case ZNET_P24_RPC_ACV_DIV2:
		{
            if (v1==0)
            {
                tune.acv_div2--;
            }
            else if (v1==1)
            {
                tune.acv_div2++;
            }            
            printf("ACV DIV2:%u\n",tune.acv_div2);
            break;
		}
*/        

        #if defined(Uses_Debug_DCV)
        case ZNET_P24_RPC_DCF_STATE:
		{
            if (v1==0)
            {
                pwm.dcf_state-=v2;
            }
            else if (v1==1)
            {
                pwm.dcf_state+=v2;
            }            
            printf("DCF STATE:%d\n",pwm.dcf_state);
            break;
		}
        
        case ZNET_P24_RPC_DCF_PCENT:
		{
            if (v1==0)
            {
                pwm.dcf_pcent-=v2;
            }
            else if (v1==1)
            {
                pwm.dcf_pcent+=v2;
            }            
            printf("DCF PCENT:%d\n",pwm.dcf_pcent);
            break;
		}
        #endif

        #if defined(Uses_Debug_PWM2)
        case ZNET_P24_RPC_BUCK_SET1:
		{
            if (dcb.pwm.pwm1_lock)
                dcb.pwm.pwm1_lock=0;
            else
                dcb.pwm.pwm1_lock=1;
            RPC_Buck_Set();
            break;
		}
        #endif

        #if defined(Uses_Debug_PWM2)
        case ZNET_P24_RPC_BUCK_SET2:
		{
            if (v1==0)
            {
//                PWM_SINE_PRXZ--;
    //          PWM_CLOCK_PRXZ--;
                dcb.pwm.pwm1_duty--;
//              PWM_CLOCK_PRXZ++;
            }
            else if (v1==1)
            {
//                PWM_SINE_PRXZ++;
  //            PWM_CLOCK_PRXZ++;
                dcb.pwm.pwm1_duty++;
//              PWM_CLOCK_PRXZ--;
            }            
            RPC_Buck_Set_Duty();
            break;
		}
        #endif

		
//		case P24_RPC_SPI:
//		{
//			HAL_EEPROM_spi_rpc(v1,v2,data,size);
//			break;
//		}
		
	}
	
}
