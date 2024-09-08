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


/*******************************************************************/
/* Function:    WMI_ReportFirmwareVersion                          */
/* Description: Send firmware version to Zigbee module.            */
/*                                                                 */
/* Parameters:  none.                                              */
/* Returns:   	nothing.                                           */
/*******************************************************************/
void ZNet_ReportFirmwareVersion()
{
	Uint8 tempBuffer[20] = {0};
	Uint8 tempFirmwareVersion[8] = {0};
	Uint8 length = 0;
	
	//CH FMT Base64(FW_VER) <CR>
	tempBuffer[0] = PIC24_FW_REPORT_CHANNEL;
	tempBuffer[1] = REPORT_TYPE_FORMAT;
	
	tempFirmwareVersion[0] = FIRMWARE_VERSION_NUM_MAJOR;
	tempFirmwareVersion[1] = FIRMWARE_VERSION_NUM_MINOR;
	length = doBase64Encode(tempFirmwareVersion, &tempBuffer[2], 2);
	
	tempBuffer[2 + length] = '\r';
	tempBuffer[3 + length] = 0;

	Uart1_PutStr((char*) tempBuffer, length + 3);
}	

/*******************************************************************/
/* Function:    ZNet_ProcessCommands                                */
/* Description: Process received commands.                         */
/* Parameters:  *wiMeshPacket                                      */
/*******************************************************************/
void ZNet_ProcessCommands(Uint8 *buf)
{
	//CMD CMD_VALUE Base64(FW_VER) <CR>
	
	#define TEMP_CMD_PAYLOAD_SIZE	80
	Uint8 tempBuffer[TEMP_CMD_PAYLOAD_SIZE] = {0};
	Uint8 cmdType = buf[1];
	Uint8 payloadLength = 0;
	
	#if defined (Uses_Debug_Wimesh )
	printf("%s,%d  ZNet_ProcessCommands TYPE:%d\n",__FILE__,__LINE__, commandType);
	#endif
	
	//decode payload
	payloadLength = doBase64Decode((buf+2), tempBuffer, (strlen((char *)(buf+2)))-1);
	
	switch(cmdType)
	{
		case PIC24_POLL_CMD:
		{
			#if defined (Uses_Debug_Wimesh )
			if (dcb1.pic24_wimesh_debug)
				printf("POLL_CMD\n");
			#endif

			ZNet_SendNormalReport(REPORT_TYPE_NORMAL);
			break;
		}//case POLL_CMD:	
	
		
		case PIC24_FIRMWARE_CMD:
		{
			ZNet_ReportFirmwareVersion();
			break;
		}
		
		case PIC24_INVERTER_STATUS_CMD:
		{
			Uint8 value = tempBuffer[0];
			
	   		switch(value)
    		{
        		case 0:
        		case 1:
				NVS_grid_state_write(value);
        		break;
            }  		
			
			break;
		}//case INVERTER_STATUS_CMD:
		
		case PIC24_RESET_CMD:
		{
			#if defined (Uses_Debug_Wimesh )
			if (dcb1.pic24_wimesh_debug)
				printf("RESET_CMD\n");
			#endif
			APP_Bootloader_Reset();
			break;
		}//case RESET_CMD:	
		
		#if defined (Uses_Debug_Wimesh )
		default:
		{
			if (dcb1.pic24_wimesh_debug)
				printf("Unknown CMD\n");
		}
		#endif
		
	}//switch(commandType)		
}
	
/*******************************************************************/
/* Function:    ZNet_ProcessReadVID                                 */
/* Description: Process the "read VID" command.                    */
/* Parameters:  *wiMeshPacket                                      */
/*******************************************************************/
void ZNet_ProcessReadVID(Uint8 *wiMeshPacket)
{
	Uint8 tempBuffer[64];
	Uint8 tempUserBuffer[32];
	
	Uint8 vid = *(wiMeshPacket+1);
	Uint8 payloadLength = 0;
	
	memset(tempBuffer,0,sizeof(tempBuffer));
	memset(tempUserBuffer,0,sizeof(tempUserBuffer));

//	#if defined (Uses_Debug )	
//	if (dcb1.pic24_wimesh_debug)
//		printf("ReadVID vid=%d\n", vid);
//	#endif
	
//	#if defined(Uses_Debug) 
//	printf("P24 ReadVID %d [%s] %s,%d\n", vid,console_get_VID_ascii(vid), __FILE__,__LINE__);
//	printf("P24 ReadVID %d  %s,%d\n", vid, __FILE__,__LINE__);
//	#endif
	
	//Is it valid VID?
	if(ERROR_OK_E == ZNet_VID_Read(vid, tempUserBuffer, &payloadLength))
	{
		if(payloadLength > 32)
			payloadLength = 32;
		//Do encoding
		payloadLength = doBase64Encode(tempUserBuffer, &tempBuffer[2], payloadLength);
		tempBuffer[0] = PIC24_READ_VID_RESPONSE;
		tempBuffer[1] = vid;//ASCII_FORMAT;
		tempBuffer[payloadLength+2] = '\r';
				
		Uart1_PutStr((char*) tempBuffer, payloadLength+3);
		
	}//if(ReadFromVID(vid, tempUserBuffer, payloadLength))
	
	else
	{
//		#if defined (Uses_Debug )			
//		if (dcb1.pic24_wimesh_debug)
//			printf("Error ZNet_ProcessReadVID \n");
//		#endif

		tempBuffer[0] = PIC24_ERROR_VID_RESPONSE;
		tempBuffer[1] = vid;//ASCII_FORMAT;
		tempBuffer[2] = '\r';
		
		Uart1_PutStr((char*) tempBuffer, 3);

	}//else	
}
	
/*******************************************************************/
/* Function:    ZNet_ProcessWriteVID                                */
/* Description: Process the "write VID" command.                   */
/* Parameters:  *wiMeshPacket                                      */
/*******************************************************************/
void ZNet_ProcessWriteVID(Uint8 *wiMeshPacket)
{
	//'w' VID Base64(PL) <CR>
	
	Uint8 tempBuffer[64];
	Uint8 vid = *(wiMeshPacket+1);
	Uint8 payloadLength = 0;
	
	memset(tempBuffer,0,sizeof(tempBuffer));
	
	//decode payload
	payloadLength = doBase64Decode((wiMeshPacket+2), tempBuffer, (strlen((char *)(wiMeshPacket+2)))-1);
	
	if(payloadLength > 32)
		payloadLength = 32;
	
	//Is it valid VID?
	if(ERROR_OK_E == ZNet_VID_Write(vid, tempBuffer, payloadLength))
	{
		tempBuffer[0] = PIC24_WRITE_VID_RESPONSE;
		tempBuffer[1] = vid; //ASCII_FORMAT;
		tempBuffer[2] = '\r';
		
		Uart1_PutStr((char*) tempBuffer, 3);
		
	}//if(WriteToVID(vid, tempUserBuffer, payloadLength))
	
	else
	{
		tempBuffer[0] = PIC24_ERROR_VID_RESPONSE;
		tempBuffer[1] = vid;//ASCII_FORMAT;
		tempBuffer[2] = '\r';
		
		Uart1_PutStr((char*) tempBuffer, 3);
	}//else
}	
	
/*******************************************************************/
/* Function:    ZNet_SendNormalReport                               */
/* Description: Send a report each interval.                       */
/* Parameters:  reportType.                                        */
/*******************************************************************/
void ZNet_SendNormalReport(Uint8 reportType)
{
	Uint8 tempBuffer[64] = {0};
	Uint8 tempLength = 0;
	Uint16 ACpower;
    Uint16 DCpower;
	Uint8 tempBufferFinal[64] = {0};
	Uint8 tempFinalLength = 0;
	
//	#if defined (Uses_Debug )
//	char *type = "REGULAR_REPORT";
//	#endif
		
	if(REPORT_TYPE_NORMAL == reportType)
	{
		tempBufferFinal[0] = PIC24_REGULAR_REPORT_CHANNEL;	//CHANNEL
	}	
	else
	{
		tempBufferFinal[0] = PIC24_ALARM_REPORT_CHANNEL;	//CHANNEL
//		#if defined (Uses_Debug )
//		type = "ALARM_REPORT";
//		#endif
	}
	
	//FORMAT		00
	tempBufferFinal[1] = REPORT_TYPE_FORMAT;
	
//	struct P24_APP_Inverter_Report {
//		WORD device_id; // 00   904 0x0388
//		BYTE status; // 02
//		WORD dcPower; // 03
//		WORD acPower; // 05
//		WORD efficency; // 07
//		BYTE acFreq; // 09
//		WORD acVolt; // 10
//		BYTE temp; // 12
//		WORD lifeTimeW; // 13
//		WORD lifeTimeK; // 15
//		WORD alarm; // 17
//	};
	
	#define	PVKEY_DEVICE_ID		904
	//DeviceID		01
	tempBuffer[tempLength++] = GET_U8_1_FROM_16(PVKEY_DEVICE_ID);
	tempBuffer[tempLength++] = GET_U8_0_FROM_16(PVKEY_DEVICE_ID);
	
	//Status		02
	tempBuffer[tempLength++] = 0;
//	tempBuffer[tempLength++] = SVC_SS_GetReportedStatus();
	
	ACpower = pwm.watts;
    DCpower = ACpower;
    
    #if defined(Uses_Debug) 
    if (dcb2.acw)
    {
        ACpower = 100;
        DCpower = 100;
    }
    #endif
	
    //DCPwr			03
//	tempBuffer[tempLength++] = GET_U8_1_FROM_16(correctedDcPower);
//	tempBuffer[tempLength++] = GET_U8_0_FROM_16(correctedDcPower);
//	tempBuffer[tempLength++] = GET_U8_1_FROM_16(ACpower);
//	tempBuffer[tempLength++] = GET_U8_0_FROM_16(ACpower);
    
	tempBuffer[tempLength++] = GET_U8_1_FROM_16(DCpower);
	tempBuffer[tempLength++] = GET_U8_0_FROM_16(DCpower);
    
	
	//ACPwr			05
	tempBuffer[tempLength++] = GET_U8_1_FROM_16(ACpower);
	tempBuffer[tempLength++] = GET_U8_0_FROM_16(ACpower);
	
	//Efficiency	07
    // Express efficiency to 3 digits. 

//	tempBuffer[tempLength++] = GET_U8_1_FROM_16(efficiency);
//	tempBuffer[tempLength++] = GET_U8_0_FROM_16(efficiency);

    tempBuffer[tempLength++] = GET_U8_1_FROM_16(1000);                
    tempBuffer[tempLength++] = GET_U8_0_FROM_16(1000);

    
    #if defined(Uses_Debug_Effic)
    if (dcb2.eff)
    {
    	tempBuffer[tempLength++] = GET_U8_1_FROM_16(500);
        tempBuffer[tempLength++] = GET_U8_0_FROM_16(500);
    }
    #endif
	
//Frequency must be extracted from the grid period, then scaled into one byte.
//	#if defined(Uses_Test_Mode)
//	freq = SVC_TM_GetFrequency(frequencyDGA);   
//	#else
//	freq = fre
    
    if ((fault.alarm.freq_low) || (fault.alarm.freq_high))
    {
        tempBuffer[tempLength++] = (fault.info.hertz_10x / 10);
    }
    else
        tempBuffer[tempLength++] = (Uint8) (GEO_freq.zc.average / 10);    //Frequency 	09
	
	//Grid Voltage  10
//	tempBuffer[tempLength++] = GET_U8_1_FROM_16(ac.grid_volt_avg);
//	tempBuffer[tempLength++] = GET_U8_0_FROM_16(ac.grid_volt_avg);

	tempBuffer[tempLength++] = GET_U8_1_FROM_16(analog.ac.avg.volts);
	tempBuffer[tempLength++] = GET_U8_0_FROM_16(analog.ac.avg.volts);
	
	
	//Internal Temperature		12
	tempBuffer[tempLength++] = temperature.celsius;
	
	
	//Accumulated Watt hours.	13
	tempBuffer[tempLength++] = GET_U8_1_FROM_16(kwh.lifeTimeW);
	tempBuffer[tempLength++] = GET_U8_0_FROM_16(kwh.lifeTimeW);
	
	//Accumulated Kilo Watt hours.	15
	tempBuffer[tempLength++] = GET_U8_1_FROM_16(kwh.lifeTimeK);
	tempBuffer[tempLength++] = GET_U8_0_FROM_16(kwh.lifeTimeK);
	
	//Alarm state			17
	tempBuffer[tempLength++] = GET_U8_1_FROM_16( HAL_Fault_Get() );
	tempBuffer[tempLength++] = GET_U8_0_FROM_16( HAL_Fault_Get() );
	
	tempFinalLength = doBase64Encode(tempBuffer, &tempBufferFinal[2], tempLength);
	
	tempBufferFinal[tempFinalLength + 2] = '\r';
	tempBufferFinal[tempFinalLength + 3] = 0;

	Uart1_PutStr((char*) tempBufferFinal, tempFinalLength + 3);

//	#if defined (Uses_Debug )			
//	if (dcb1.pic24_wimesh_debug)
//		printf("SendNormalReport Sz=%d Type=%d %s\n", tempFinalLength, reportType, type);
//	#endif

}

ErrorCode ZNet_VID_Write(Uint8 vid, Uint8 *data, Uint8 dataLength)
{
    Uint16 value;
	ErrorCode errorCode = ERROR_OK_E;
        

	if ( (data==0) || (data == NULL))
	{
//		#if defined (Uses_Debug )				
//			printf("WriteToVID ERROR NULL %s,%d\n", __FILE__, __LINE__);
//		#endif
		return 0;
	}
	
    value = atoi((char *)data);
//	#if defined( Uses_Debug )
//	printf("WriteVID %d,%d %s,%d\n",vid, *data,__FILE__,__LINE__);
//	#endif
	
	switch(vid)
	{	
		case VID_INVERTER_ON_OFF:
			#if defined(Uses_Debug_Wimesh)
			printf("INVERTER=%s",data);
			print_source();
			#endif
			NVS_grid_state_write(value);
		break;
		
		case VID_GO_STOP:
			APP_Stop_Mode();	// DGA WIFI ZIGBEE
		break;
		
		case VID_GEO_CODE:
			NVS_GEO_Write(value);
		break;
		
		case VID_ENGINEERING_MODE:
		case VID_STORED_ENGINEERING_MODE:
			NVS_Engineer_Mode_Write(value);
		break;
		
//		case VID_CLEAR_INTERNAL_ALARM:
//			HAL_ClearInternalAlarmStateBits();
//		break;
		
		case VID_ACCUMULATED_ENERGY:
			KWH_Clear();
		break;
		
//		case VID_MONITOR_INTERNAL_VARIABLE:
//			SVC_MIV_SelectInternalVariable(data);
//		break;
		
		
		case VID_RUN_STATE:
			
			#if defined(Uses_Debug)
			printf("GRID=%s",data);
			print_source();
			#endif
			
			NVS_grid_state_write(value);
		break;
		
		case VID_MPPT_RAMP_UP:
		case VID_POWER_UPPER_LIMIT:
//		case VID_SLOW_CHANGE_RATE:
		case VID_MPPT_DELAY_RAMP_UP:
		case VID_MPPT_DELAY_RAMP_DN:
		case VID_MPPT_MODE:
		case VID_MPPT_RAMP_UP_PERCENT:
        case VID_MPPT_RAMP_DN_PERCENT:
		case VID_GRID_AVERAGING_SCALE:
		case VID_MPPT_RAMP_DN:
		case VID_TEMPERATURE_HOT:
		case VID_MPPT_RCB:
        case VID_GEO_FREQ_50HZ_MIN:
        case VID_GEO_FREQ_50HZ_MAX:
        case VID_GEO_FREQ_60HZ_MIN:
        case VID_GEO_FREQ_60HZ_MAX:
            
		    errorCode = NVS_VID_Write_Item(vid, value);
		break;
		
		case VID_DEFAULT_TUNING_PARAMETERS:
//			NVS_Config_Default();
		break;
		
		case VID_UPDATE_TUNING_PARAMETERS:
			NVS_VID_Read_All();
		break;
		
//		case VID_TEMPERATURE_SHUT_OFF_DISABLED:
//		    /* USE WITH EXTREME CAUTION!!! */
//		    SVC_TL_SetTemperatureShutOffDisabledState((Boolean)atoi((char *)data));
//		break;
		
		default:
			errorCode = ERROR_UNKNOWN_VALUE_E;
		break;
	}//switch(vid)

	if (errorCode == ERROR_OK_E)
	{
		APP_Inverter_Fault_History_Reset();
	}
	
	return (errorCode);
	
}//ErrorCode ZNet_PA_WriteToVID(uint8 vid, uint8 *data, uint8 dataLength)

ErrorCode ZNet_VID_Read(Uint8 vid, Uint8 *data, Uint8 *dataLength)
{
    // Local variables.
	ErrorCode errorCode = ERROR_OK_E;

	switch(vid)
	{
		
		case VID_INVERTER_ON_OFF:
			sprintf((char *)data, "%d", app.gRemoteCommand);
        break;
		
		case VID_GEO_CODE:
			sprintf((char *)data, "%d", NVS_GEO_Read());
		break;
		
		case VID_INVERTER_APP_FIRMWARE:
			ZNet_PA_GetInverterApplicationVersion(data);
		break;
		
		case VID_TEMPERATURE_HOT:
			sprintf((char *)data, "%d", tune.temperature.hot_on);
		break;
		
		case VID_ACCUMULATED_ENERGY:
		    sprintf((char *)data, "%ld", KWH_Get_Lifetime() );
		break;
		
		case VID_READ_ALARM_STATE:
			sprintf((char *)data, "%d", HAL_Fault_Get());
		break;
		
		case VID_STORED_ENGINEERING_MODE:
			sprintf((char *)data, "%d", NVS_Engineer_Mode_Read());
		break;
		
		case VID_RUN_STATE:
			sprintf((char *)data, "%d", NVS_grid_state_read());
		break;
		
        
        
		case VID_MPPT_RAMP_UP:
		case VID_POWER_UPPER_LIMIT:
		case VID_MPPT_DELAY_RAMP_UP:
		case VID_MPPT_DELAY_RAMP_DN:
		case VID_MPPT_MODE:
		case VID_GRID_AVERAGING_SCALE:
		case VID_MPPT_RAMP_DN:
		case VID_MPPT_RCB:
		case VID_MPPT_RAMP_UP_PERCENT:
        case VID_MPPT_RAMP_DN_PERCENT:
            
        case VID_GEO_FREQ_50HZ_MIN:
        case VID_GEO_FREQ_50HZ_MAX:
        case VID_GEO_FREQ_60HZ_MIN:
        case VID_GEO_FREQ_60HZ_MAX:
            
			
		    errorCode = NVS_VID_Read_Item(vid, data);
		break;
		
		case VID_GRID_FREQUENCY:
			sprintf((char *)data, "%d", GEO_Config.grid_freq);
			break;
		
		default:
            *data = 0;
			errorCode = ERROR_UNKNOWN_VALUE_E;
		break;
	}
		
	// How much data did we read?
	*dataLength = strlen((char *)data);

	return (errorCode);
}

/*******************************************************************/
/* Function:    ZNet_PA_GetInverterApplicationVersion               */
/*                                                                 */
/* Description: Get the version of the inverter application        */
/*              firmware.                                          */
/* Parameters:  pData.                                             */
/*******************************************************************/

void ZNet_PA_GetInverterApplicationVersion(Uint8 *pData)
{
	strcpy((char *)pData, (char *)app.text_variant_ver);
}	

void ZNet_ReadVID(Uint8 value)	// DGA 2022-10-23 
{
	Uint8 data[16];
	data[0] = value;
	data[1] = value;
	data[3] = 0;
	ZNet_ProcessReadVID(data);
}
