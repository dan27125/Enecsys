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

static int _NVS_Write8(Uint8 address, Uint8 data)
{
    if (address > MAX_NV_STORE_ADDRESS)
	{
		return 1;
	}
	HAL_EEPROM_write(address,data);
    return 0;
}

static int _NVS_Write16(Uint8 address,Uint16 data)
{
    if (address > MAX_NV_STORE_ADDRESS)
	{
		return 1;
	}
	HAL_EEPROM_write16(address,data);
    return 0;
}

static int _NVS_Read8(Uint8 address,Uint8 *pData)
{
	Uint8 byte0;

    // Check pre-conditions. 
    if (address > MAX_NV_STORE_ADDRESS)
	{
		#if defined(Uses_Debug_NVS_Text)
        printf("NVS_Read ERROR %s","ADDR");
        print_source();
        #endif
		
		return 1;
	}
		
	if (pData==NULL)
	{
		#if defined(Uses_Debug_NVS_Text)
        printf("NVS_Read ERROR %s", "NULL");
        print_source();
        #endif
		
		return 2;
	}
   
	byte0 = HAL_EEPROM_read(address);

	*pData = byte0;
    return 0;
} 

/*
static int _NVS_Read8_2024(Uint8 address,Uint8 *pData)
{
	Uint8 byte0;

    // Check pre-conditions. 
    if (address > MAX_NV_STORE_ADDRESS)
	{
		#if defined(Uses_Debug_NVS_Text)
        printf("NVS_Read ERROR %s","ADDR");
        print_source();
        #endif
		
		return 1;
	}
		
	if (pData==NULL)
	{
		#if defined(Uses_Debug_NVS_Text)
        printf("NVS_Read ERROR %s", "NULL");
        print_source();
        #endif
		
		return 2;
	}
   
	byte0 = HAL_EEPROM_read(address);

	*pData = byte0;
    return 0;
} 
*/

static int _NVS_Read16(Uint8 address,Uint16 *pData)
{
    Uint8 byte0, byte1;
	Uint16 value;
	
    // Check pre-conditions. 
    if (address > MAX_NV_STORE_ADDRESS)
	{
		#if defined(Uses_Debug_NVS_Text)
        printf("NVS_Read ERROR %s","ADDR");
        print_source();
        #endif
	}
		
	if (pData==NULL)
	{
		#if defined(Uses_Debug_NVS_Text)
        printf("NVS_Read ERROR %s", "NULL");
        print_source();
        #endif
		return 2;
	}

	byte0 = HAL_EEPROM_read(address);
	byte1 = HAL_EEPROM_read(address+1);
	value =  GET_16_FROM_2X8(byte1, byte0);
    
    *pData = value;
    
    return 0;
}

void NVS_Set_Valid(Uint8 code)
{
	nvs.valid_signature=code;
}


#if defined(Uses_Debug_NVS_Text)

char* NVS_Get_Text(Uint8 addr)
{
	switch(addr)
	{
		case NVS_GEO_CODE:
			return "GEO"; 
		case NVS_ENGINEERING_MODE:
			return "ENGINEERING"; 			
		case NVS_GRID_STATE:
			return "GRID_STATE";
		case NVS_KWH_WT:
			return "KWH_WT";
		case NVS_KWH_KW:
			return "KWH_WT";
		case NVS_POWER_UPPER_LIMIT:
			return "POWER";
		case NVS_TEMPERATURE:
			return "TEMP";
		case NVS_REPORT_INTERVAL:
			return "REPORT-INTERVAL";
		case NVS_MPPT_RAMP:
			return "MPPT_RAMP";			
		case NVS_MPPT_DELAY_RAMP:
			return "DELAY-RAMP";			
		case NVS_MPPT_DOWN:
			return "MPPT_DOWN";	
		case NVS_MPPT_DELAY_DOWN:
			return "DELAY-DN";	
		case NVS_HOURS_CONSOLE:
			return "HR-CON";			
		case NVS_HOURS_RUNTIME:
			return "HR-RUN";
		case NVS_RCB1:
			return "RCB1";
	}
	return "??";
}

void NVS_Read_Error(Uint8 addr, Uint8 value, Uint8 sig)
{
	printf("NVS ERROR %u: %s",nvs.read_error, NVS_Get_Text(addr) );
	printf(" (%x,%x)\n",value,sig);
	printf("\n");
	APP_delay_miliseconds(5);
}

#endif

int NVS_Read_Word(Uint8 addr, Uint16 *dst, Uint8 signature)
{
//    enum NVS_MODE  { NVS_MODE_NORMAL, NVS_MODE_IGNORE, NVS_MODE_RESET};
	if (nvs.mode==NVS_MODE_RESET)
		return 1;

	Uint8 value;
	_NVS_Read8(addr+2,&value);
	
	if (value!=signature)
	{
		nvs.read_error++;
		
		#if defined(Uses_Debug_NVS_Text)
		NVS_Read_Error(addr, value, signature);
		#endif

		if (nvs.mode!=NVS_MODE_IGNORE)
			return 1;
	}

	_NVS_Read16(addr,dst);
	return 0;

}

int NVS_Read_Byte(Uint8 addr, Uint8 *dst, Uint8 signature)
{
//    enum NVS_MODE  { NVS_MODE_NORMAL, NVS_MODE_IGNORE, NVS_MODE_RESET};
	if (nvs.mode==NVS_MODE_RESET)
		return 1;

	Uint8 value;
	_NVS_Read8(addr+2,&value);
	
	if (value!=signature)
	{
		nvs.read_error++;
		if (value!=signature)
		{
			#if defined(Uses_Debug_NVS_Text)
			NVS_Read_Error(addr, value, signature);
			#endif

			if (nvs.mode!=NVS_MODE_IGNORE)
				return 1;
		}
	}
	
	_NVS_Read8(addr,dst);
	return 0;

}

int NVS_Read_Byte_2024(Uint8 addr, Uint8 *dst, Uint8 signature)
{
	if (nvs.mode==NVS_MODE_RESET)
		return 1;

	Uint8 value;
    
	_NVS_Read8(addr+1,&value);
	
	if (value!=signature)
	{
		nvs.read_error++;
		if (value!=signature)
		{
			#if defined(Uses_Debug_NVS_Text)
			NVS_Read_Error(addr, value, signature);
			#endif

			if (nvs.mode!=NVS_MODE_IGNORE)
				return 1;
		}
	}
	
	_NVS_Read8(addr,dst);
	return 0;

}
void NVS_Write_Byte_2024(Uint8 addr, Uint8 value, Uint8 valid)
{
    _NVS_Write8(addr,value);
    _NVS_Write8(addr+1,valid); 
}


void NVS_Write_Word(Uint8 addr, Uint16 value, Uint8 valid)
{
	_NVS_Write16(addr,value);
    _NVS_Write8(addr+2,valid);
}

void NVS_Write_Byte(Uint8 addr, Uint8 value, Uint8 valid)
{
    _NVS_Write8(addr,value);
    _NVS_Write8(addr+2,valid); 
}

void NVS_Engineer_Mode_Write(Uint16 value)
{
	NVS_Write_Byte(NVS_ENGINEERING_MODE,value,NVS_VALID_DEFAULT);
	mEngineeringMode = value;
}

/*******************************************************************/
/* Function:    NVS_Engineer_Mode_Read                      */
/* Description: Validate and read the stored engineering mode.     */
/*              If a valid mode is not stored, return "OFF".       */
/* Returns:   	stored engineering mode.                           */
/*******************************************************************/
Uint8 NVS_Engineer_Mode_Read()
{
	if (NVS_Read_Byte(NVS_ENGINEERING_MODE, (Uint8*) &mEngineeringMode, NVS_VALID_DEFAULT))
	{
		NVS_Engineer_Mode_Write(0);
	}
    return mEngineeringMode;
}

void NVS_grid_state_write(Uint8 gridState)
{
	if (gridState != app.gRemoteCommand)
	{
		app.gRemoteCommand = gridState;
		APP_Inverter_NVS_Stop_Refresh();
		APP_Stop_Mode();
	}
	
	NVS_Write_Byte(NVS_GRID_STATE,app.gRemoteCommand,NVS_VALID_DEFAULT);
}

Uint8 NVS_grid_state_read()
{
	if (NVS_Read_Byte(NVS_GRID_STATE,&app.gRemoteCommand,NVS_VALID_DEFAULT)) 
	{
		NVS_grid_state_write(ON);
	}
    return (app.gRemoteCommand);   
}

void NVS_RCB_Write()
{
	NVS_Write_Word(NVS_RCB1,ecb1.value,NVS_VALID_DEFAULT);
//  printf("WRITE rcb=%x run=%d \n",ecb1.value, ecb1.run);
}

void NVS_RCB_Read()
{
	if (NVS_Read_Word(NVS_RCB1,(Uint16*) &ecb1.value, NVS_VALID_DEFAULT))
	{
		ecb1.run = 1;	// DGA 2022-09-26  Default ON 
		NVS_Write_Word(NVS_RCB1,ecb1.value,NVS_VALID_DEFAULT);
	}
    
//  printf("READ rcb=%x run=%d \n",ecb1.value, ecb1.run);
    
}

#if defined(Uses_Diagnostics)
void NVS_Diag_Read()
{
	if (NVS_Read_Byte(NVS_DIAG_UFB_SECS,(Uint8*) &diag.ufb_sec, NVS_VALID_DEFAULT))
	{
		diag.ufb_sec = 0;
		NVS_Write_Byte(NVS_DIAG_UFB_SECS,diag.ufb_sec,NVS_VALID_DEFAULT);
	}
	
	if (NVS_Read_Byte(NVS_DIAG_BUCK_SECS,(Uint8*) &diag.buck_sec, NVS_VALID_DEFAULT))
	{
		diag.buck_sec = 0;
		NVS_Write_Byte(NVS_DIAG_BUCK_SECS,diag.buck_sec,NVS_VALID_DEFAULT);
	}
	
	if (diag.ufb_sec)
	{
		APP_Stop_Mode();// DGA WIFI ZIGBEE cmd
		ecb1.run = 0;
		ecb1.diag_ufb=1;
	}
	
//	if ( (diag.ufb_sec) || (ecb1.diag_buck))
//	{
//		APP_Stop_Mode();// DGA WIFI ZIGBEE cmd
//		ecb1.run = 0;
//	}
	
}

void NVS_Diag_Write()
{
	NVS_Write_Word(NVS_DIAG_UFB_SECS,diag.ufb_sec,NVS_VALID_DEFAULT);
	NVS_Write_Word(NVS_DIAG_BUCK_SECS,diag.buck_sec,NVS_VALID_DEFAULT);

}


#endif


void NVS_Console_Hours_Write(Uint16 value)
{
	nvs.hours_console = value;
	NVS_Write_Word(NVS_HOURS_CONSOLE,nvs.hours_console,NVS_VALID_DEFAULT);
	#if defined(Uses_Debug_NVS_Hours)
	printf("WRITE Console HRS:%u \n",nvs.hours_console);
	#endif
	
}

Uint16 NVS_Console_Hours_Read()
{
	if (NVS_Read_Word(NVS_HOURS_CONSOLE,(Uint16 *)&nvs.hours_console,NVS_VALID_DEFAULT)) 
	{
		NVS_Console_Hours_Write(0);
	}
	return nvs.hours_console;
}

void NVS_Runtime_Hours_Write(Uint16 value)
{
	nvs.hours_runtime = value;
	NVS_Write_Word(NVS_HOURS_RUNTIME,nvs.hours_runtime,NVS_VALID_DEFAULT);
}

Uint16 NVS_Runtime_Hours_Read()
{
	if (NVS_Read_Word(NVS_HOURS_RUNTIME,(Uint16 *)&nvs.hours_runtime,NVS_VALID_DEFAULT)) 
	{
		NVS_Runtime_Hours_Write(0);
	}
	return nvs.hours_runtime;
}

#if defined(Uses_Debug_NVS_Clock)

void NVS_Clock_Read()
{
	Clock_Control clock;
	NVS_Read_Word(NVS_CLOCK1,(Uint16 *)&clock.b,NVS_VALID_DEFAULT);
	NVS_Read_Word(NVS_CLOCK2,(Uint16 *)&clock.a,NVS_VALID_DEFAULT);
	app.boot_sec = clock.value;
}
void NVS_Clock_Write()
{
	Clock_Control clock;
	clock.value = app.boot_sec;
	NVS_Write_Word(NVS_CLOCK1,clock.b,NVS_VALID_DEFAULT);
	NVS_Write_Word(NVS_CLOCK2,clock.a,NVS_VALID_DEFAULT);
}

#endif

void NVS_GEO_Write(Uint16 code)
{
	if (APP_State_Get() == INVERTER_RUN)
		APP_Stop_Mode();			// DGA shut down 		
	
	tune.geo_code = code;
	NVS_Write_Word(NVS_GEO_CODE,tune.geo_code, NVS_VALID_DEFAULT);
	APP_AC_Freq_Reset();
	GEO_Set();
}

Uint16 NVS_GEO_Read()
{
	tune.geo_code = 0;
	
	if (NVS_Read_Word(NVS_GEO_CODE,(Uint16 *)&tune.geo_code,NVS_VALID_DEFAULT)) 
	{
		NVS_GEO_Write(GEO_FACTORY);
	}

	APP_AC_Freq_Reset();
	GEO_Set();
    
	return tune.geo_code;
}

void NVS_ripple_divsor_write(Uint8 value)
{
    if ((value < TUNE_RIPPLE_DIV_MIN) || (value > TUNE_RIPPLE_DIV_MAX))
        value = TUNE_RIPPLE_DIV_DEF;
    NVS_Write_Byte(NVS_BYTE_RIPPLE_DIV,value, NVS_VALID_DEFAULT);
    tune.fault.ripple_div = value;
}

Uint8 NVS_ripple_divsor_read()
{
    
	if (NVS_Read_Byte(NVS_BYTE_RIPPLE_DIV, (Uint8*) &tune.fault.ripple_div, NVS_VALID_DEFAULT)) 
	{
        NVS_ripple_divsor_write(TUNE_RIPPLE_DIV_DEF);
	}
    if ((tune.fault.ripple_div < TUNE_RIPPLE_DIV_MIN) || (tune.fault.ripple_div > TUNE_RIPPLE_DIV_MAX))
        tune.fault.ripple_div = TUNE_RIPPLE_DIV_DEF;
    
    tune.fault.ripple_dc1 = 6;
    tune.fault.ripple_dc2 = 6;
    tune.fault.ripple_dc3 = 7;
    
	return tune.fault.ripple_div;
}

void NVS_acv_divsor_write(Uint16 value)
{
    if ((value < TUNE_ACV_DIV_MIN) || (value > TUNE_ACV_DIV_MAX))
        value = TUNE_ACV_DIV_DEF;
    tune.acv_div = value;
    NVS_Write_Word(NVS_WORD_ACV_DIV,tune.acv_div, NVS_VALID_DEFAULT);
}
Uint16 NVS_acv_divsor_read()
{
	if (NVS_Read_Word(NVS_WORD_ACV_DIV, (Uint16*) &tune.acv_div, NVS_VALID_DEFAULT)) 
	{
        NVS_acv_divsor_write(TUNE_ACV_DIV_DEF);
	}
    if ((tune.acv_div < TUNE_ACV_DIV_MIN) || (tune.acv_div > TUNE_ACV_DIV_MAX))
        tune.acv_div = TUNE_ACV_DIV_DEF;
	return tune.acv_div;    
}

void NVS_fault_bits_write(Uint16 value)
{
    NVS_Write_Word(NVS_WORD_FAULT_BITS,value, NVS_VALID_DEFAULT);
    tune.fault.bit.value = value;
}
void NVS_fault_bits_read()
{
	if (NVS_Read_Word(NVS_WORD_FAULT_BITS, (Uint16 *) &tune.fault.bit.value, NVS_VALID_DEFAULT))
	{
        NVS_fault_bits_write(0xFFFF);   // default ALL ON
	}

}


void NVS_temperature_hot_write(Sint16 value)
{
    tune.temperature.hot_on = Bounds_Check_Uint16(value,TUNE_TEMP_HOT_MIN,TUNE_TEMP_HOT_MAX);
    tune.temperature.hot_off = tune.temperature.hot_on-TUNE_TEMP_HOT_OFF_DIFF;
    tune.temperature.mppt_dn = tune.temperature.hot_on-TUNE_TEMP_MPPT_DN_DIFF;
    tune.temperature.mppt_up = tune.temperature.hot_on-TUNE_TEMP_MPPT_UP_DIFF;
    NVS_Write_Word(NVS_TEMPERATURE,value, NVS_VALID_DEFAULT);

    #if defined(Uses_Debug_Hot)
    printf("write hot_on=%d %d\n",tune.temperature.hot_on,value);
    #endif
    
}
void NVS_temperature_hot_read()
{
	if (NVS_Read_Word(NVS_TEMPERATURE, (Uint16 *) &tune.temperature.hot_on, NVS_VALID_DEFAULT))
	{
        NVS_temperature_hot_write(TUNE_TEMP_HOT_DEF);
	}
    tune.temperature.hot_on = Bounds_Check_Uint16(tune.temperature.hot_on,TUNE_TEMP_HOT_MIN,TUNE_TEMP_HOT_MAX);
    tune.temperature.hot_off = tune.temperature.hot_on-TUNE_TEMP_HOT_OFF_DIFF;
    tune.temperature.mppt_dn = tune.temperature.hot_on-TUNE_TEMP_MPPT_DN_DIFF;
    tune.temperature.mppt_up = tune.temperature.hot_on-TUNE_TEMP_MPPT_UP_DIFF;
    
    #if defined(Uses_Debug_Hot)
    printf("read hot_on=%d def=%d\n",tune.temperature.hot_on,TUNE_TEMP_HOT_DEF);
    #endif
    
    
}



Uint8 _NVS_Fault_Counter_Get_Addr(Uint8 item)
{
    Uint8 addr = 0;
//  sizeof(NVS_Fault_Hist)
    
    if ((item > 0) && (item <= NVS_FAULT_COUNTER_TOTAL))
    {
         addr = NVS_FAULT_COUNTER_BEG + ((item-1)* sizeof(NVS_Fault_Hist) );
//       printf("FC-ADDR ITEM %d ADDR %d,%d\n",item,NVS_FAULT_COUNTER_BEG,addr);
    }
    return addr;
}
void NVS_Fault_Counter_Read_Verify()
{
    Uint8 value = 0;
	_NVS_Read8(NVS_FAULT_COUNTER_SIG,(Uint8 *)&value);
    if ((value != NVS_VALID_DEFAULT) || (nvs.mode==NVS_MODE_RESET))
    {
        _NVS_Write8(NVS_FAULT_COUNTER_SIG,NVS_VALID_DEFAULT);
        NVS_Fault_Hist_Reset_All();
    }
}
Uint16 NVS_Fault_Hist_Read(Uint8 item, NVS_Fault_Hist *p)
{
    Uint8 addr;
    p->count=0;
    p->item1=0;
    p->item2=0;
    addr = _NVS_Fault_Counter_Get_Addr(item);
    if (addr)
    {
        _NVS_Read16(addr,(Uint16 *)&p->count);
        _NVS_Read16(addr+2,(Uint16 *)&p->item1);
        _NVS_Read16(addr+4,(Uint16 *)&p->item2);
    }
    return p->count;
}
void NVS_Fault_Hist_Write(Uint8 item, NVS_Fault_Hist *p) 
{
    Uint8 addr;
    addr = _NVS_Fault_Counter_Get_Addr(item);
    if (addr)
    {
        _NVS_Write16(addr,p->count);
        _NVS_Write16(addr+2,p->item1);
        _NVS_Write16(addr+4,p->item2);
    }
}
void NVS_Fault_Hist_Increment(Uint8 item, Uint16 hist1, Uint16 hist2) 
{
    NVS_Fault_Hist x;
    NVS_Fault_Hist_Read(item, &x);
    x.count++;
    x.item1 = hist1;
    x.item2 = hist2;
    NVS_Fault_Hist_Write(item, &x);
}
void NVS_Fault_Hist_Reset_Item(Uint8 item)
{
    Uint8 addr = _NVS_Fault_Counter_Get_Addr(item);
    if (addr)
    {
        _NVS_Write16(addr,0);
        _NVS_Write16(addr+2,0);
        _NVS_Write16(addr+4,0);
//      printf("FC-RESET ITEM %d ADDR %d\n",item,addr);
    }
}
void NVS_Fault_Hist_Reset_All()
{
    Uint8 i;
    for (i = 1; i <= NVS_FAULT_COUNTER_TOTAL; i++)
        NVS_Fault_Hist_Reset_Item(i);
}


void NVS_Read_All(enum NVS_MODE mode)
{
	nvs.mode = mode;
	nvs.read_error=0;
	
	NVS_RCB_Read();				// DGA read run control bits
    #if defined(Uses_Diagnostics)
	NVS_Diag_Read();			// DGA read diagnostic bits
    #endif
	NVS_grid_state_read();			// DGA read Grid control bit 
	NVS_Engineer_Mode_Read();	// DGA read Engineering mode bit 
	NVS_VID_Read_All();		// DGA read tuning parameters 
	KWH_EEPROM_Read();	
	NVS_Console_Hours_Read();
	NVS_Runtime_Hours_Read();

    NVS_temperature_hot_read();
    NVS_ripple_divsor_read();
    NVS_acv_divsor_read();
    NVS_fault_bits_read();
        
    NVS_Fault_Counter_Read_Verify();
    
    #if defined(Uses_Install_Mode)
	APP_Inverter_Install_Mode_Refresh();
    #endif
	
	#if defined(Uses_Debug_NVS_Clock)
    NVS_Clock_Read();
    #endif
	
//	#if defined(Uses_Debug)
//	printf("NVS-READ %u errors\n",nvs.read_error);
//	#endif

	nvs.mode = NVS_MODE_NORMAL;
}

#if defined(Uses_NVS_Console) || defined(Uses_Debug_NVS)


#if defined( Uses_Debug_NVS_SPI ) 
void NVS_Write_Wait()
{
	int i;
	for (i = 0; i < 1000; i++)
	{
		if (spi.bits.write_busy==0)
		{
			printf("Write %d OK\n",i);
			return;
		}
		APP_delay_miliseconds(1);
		HAL_EEPROM_task();
	}
	printf("Write Timeout\n");
}
#endif

void NVS_Format(Uint8 value, Uint8 start, Uint8 size)
{
	Uint8 i;
//	printf("FORMAT:%d,0x%x SZ:%u START:%d,0x%x\n", value,value, size,start,start);
	for (i = 0; i < size; i++)
	{
		HAL_EEPROM_write(start+i,value);
	}
	#if defined( Uses_Debug_NVS_SPI ) 
	NVS_Write_Wait();
	#endif
}


void NVS_Print_Addr(Uint16 addr)
{
	Uint8 i;
	Uint8 data;
//	printf("NVS Print:%x \n", addr );
//	NVS 00: 00 11 22 33 44 55 66 77 88 99 88 22 22 22 22 22 22 33 44 
//	NVS[10] 00 11 22 33 44 55 66 77 88 99 88 22 22 22 22 22 22 33 44 	

//	NVS 10: 00 11 22 33 44 55 66.77.88.99.88.22.22 22 22 22 22 33 44 	
//	NVS 20: 00.11.22.33.44.55.66.77.88.99.88.22.22 22 22 22 22 33 44 		
	printf("NVS[%02x] ", addr );
	
	for (i = 0; i < 16; i++)
	{
		_NVS_Read8(addr+i, &data);
		printf("%02x ", data );
	}
	printf("\n");
}

void NVS_Print_All()
{
	nvs.print_limit	= 240;
//	nvs.print_limit	= 0x80-16;
	nvs.print=1;
	nvs.print_addr=0;
}

void NVS_Print_Next()
{
	if (nvs.print)
	{
		NVS_Print_Addr(nvs.print_addr);
		if (nvs.print_addr < 240)
	 		nvs.print_addr += 16;
		else
		{
			nvs.print=0;
		}
	}
}
void NVS_Task()
{
	if (nvs.print)
	{
		NVS_Print_Addr(nvs.print_addr);
	
//		#define	NVS_PRINT_LIMIT		240	
		if (nvs.print_addr < nvs.print_limit)
	 		nvs.print_addr += 16;
		else
		{
			nvs.print=0;
		}
	}
}

Uint8 NVS_console_parse_args()
{
	if ( strcmp("NVS", (char*)arg1)==0)
	{
        #if defined(Uses_Debug_NVS_Write)
        if (strcmp("MAX", (char*)arg2)==0)
        {
            printf("MAX=%d\n",spi.write_addr_max);
            return 1;
        }
        #endif

        
		if (strcmp("FORMAT", (char*)arg2)==0)
		{
            if (csib.value[3])
            {
                Uint8 value,start,size;
                value = csib.value[2];
                start = csib.value[3];
                size = csib.value[4];
    //          printf("value=%d start=%d size=%d\n",value,start,size);
                NVS_Format(value, start,size);
            }
            else
                NVS_Format(csib.value[2],0,0xff);
			
			#if defined(Uses_Debug_NVS_Clock)
			app.boot_sec = 0;
			nvs.test_reset=0;
			NVS_Clock_Write();
			#endif
			
		}

		#if defined( Uses_Debug_NVS_SPI ) 
		else if (strcmp("DEBUG", (char*)arg2)==0)
		{
			void HAL_EEPROM_dprint();
			HAL_EEPROM_dprint();
		}
		#endif
		
		else if (strcmp("HOURS", (char*)arg2)==0)
		{
			Uint16 value = csib.value[2];
			
			if (strcmp("RESET", (char*)arg3)==0)
			{
				printf("%s %s %s\n",arg1,arg2,arg3);
				NVS_Console_Hours_Write(0);
			}
			else if (strcmp("+", (char*)arg3)==0)
			{
				nvs.hours_console++;
				NVS_Console_Hours_Write(nvs.hours_console);
			}
			else if (strcmp("-", (char*)arg3)==0)
			{
				if (nvs.hours_console)
				{
					nvs.hours_console++;
					NVS_Console_Hours_Write(nvs.hours_console);
				}
			}
			else
			{
				nvs.hours_console = value;
				printf("UPDATE HRS:%u\n",nvs.hours_console);
				NVS_Console_Hours_Write(nvs.hours_console);
			}
			return 1;
		}

		if (strcmp("READ", (char*)arg2)==0)
		{
			NVS_Read_All(NVS_MODE_IGNORE);
			return 1;
		}

		else if ((strcmp("RESET", (char*)arg2)==0) || (strcmp(TEXT_DEFAULT, (char*)arg2)==0))
		{
			printf("%s\n",arg2);
			
			NVS_Read_All(NVS_MODE_RESET);
			
			#if defined(Uses_Debug_NVS_Clock)
			app.boot_sec = 0;
			nvs.test_reset=0;
			NVS_Clock_Write();
			#endif
			
			return 1;
		}
		else if (strcmp("PRINT", (char*)arg2)==0)
		{
			if (csib.value[2])
			{
				NVS_Print_Addr(csib.value[2]);
			}
			else
				NVS_Print_All();
		}

//		else if (strcmp("PRINT1", (char*)arg2)==0)
//		{
//			HAL_EEPROM_spi_read();	
//			NVS_Print_Addr(0);	
//		}
		else if (strcmp("LOAD", (char*)arg2)==0)
		{
			printf("%s\n",arg2);
			HAL_EEPROM_spi_read();	
			NVS_Print_All();
		}

		
//		else if (strcmp("TEST", (char*)arg2)==0)
//		{
//			NVS_Test1();
//		}
		
//		else if (strcmp("TEST2", (char*)arg2)==0)
//		{
//			NVS_Test2();
//		}
		
//		#if defined(Uses_Debug_NVS)
//		else if (strcmp("FIFO", (char*)arg2)==0)
//		{
//			int a;
//			unsigned int b;
//			printf("%s=%d SZ=%d,%d,%d\n",arg2,EEPROM_Get_WR_Size(),sizeof(a),sizeof(b),sizeof(Uint16));
//		}
//		#endif
		
		return 1;
	}

	return 0;	
}

#endif