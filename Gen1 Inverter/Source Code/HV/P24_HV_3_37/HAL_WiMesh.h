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



#if !defined(HAL_WIMESH_H)
#define HAL_WIMESH_H

#include "APP_Inverter.h"
//	#include "WMI_WiMeshHeaderFiles.h"




    #define REPORT_TYPE_ALARM           0
	#define REPORT_TYPE_NORMAL          1

	#define REPORT_TYPE_FORMAT          '0'
//For Receive Packets

    //Received Packet Types
	#define PIC24_COMMAND_PACKET		'c'		// 99 0x63 From Zigbee  ;
	#define PIC24_READ_VID_PACKET		'r'		// 114 0x72 From Zigbee  read vid
	#define PIC24_WRITE_VID_PACKET		'w'		// 119 0x77 From Zigbee	 write vid

	#define COMMAND_REPONSE                             'C'

	#define PIC24_POLL_CMD                              '0'
	#define PIC24_INVERTER_STATUS_CMD                   '1'
	#define PIC24_FIRMWARE_CMD                          '2'
	#define PIC24_RESET_CMD                             '3'
    #define PIC24_ERROR_VID_RESPONSE                    'E'

	#define PIC24_FW_REPORT_CHANNEL                     '0'
	#define PIC24_REGULAR_REPORT_CHANNEL                '1'
	#define PIC24_ALARM_REPORT_CHANNEL					'2'
//	#define PIC24_VID_NON_ESSENTIAL_REPORT_CHANNEL 		'4'



	#define PIC24_READ_VID_RESPONSE                    'R'	
	#define PIC24_WRITE_VID_RESPONSE                   'W'
	
	
	
/**************************************************/
/* Valid values ['0' - '9', 'A' - 'Z', 'a' - 'z'] */
/**************************************************/

	#define VID_FW                              '0'     // VID used for reading Firmware version
	#define VID_INVERTER_ON_OFF                 '1'     // 49 - VID for reading/writing gRemoteCommand
	#define VID_GO_STOP                         '2'			// VID for forcing the inverter into STOP mode.
	#define VID_GEO_CODE                        '3'  		// 51 - Vid for setting country code, used for config data. Only works if not connected.
	#define VID_ENGINEERING_MODE                '4' 		// Force the inverter into engineering mode, i.e short connection time etc. Only works if not connected.
	#define VID_CLEAR_INTERNAL_ALARM            '5'         // Clear internally generated alarm flags.
	#define VID_INVERTER_APP_FIRMWARE           '6'         // Used to read the version of the inverter application firmware.
	#define VID_TEMPERATURE_HOT					'7' 
	#define VID_ACCUMULATED_ENERGY              '8'         // Used to force the accumulated energy to zero, (only works if not connected), or to read acc. energy.


	#define VID_GEO_FREQ_50HZ_MIN               'A'
    #define VID_GEO_FREQ_50HZ_MAX               'B'
	#define VID_GEO_FREQ_60HZ_MIN               'C'
    #define VID_GEO_FREQ_60HZ_MAX               'D'


	#define VID_CONFIG_DATA                     'P'
	#define VID_CONFIG_DATA_LOCK                'Q'


	#define VID_READ_ALARM_STATE			    'a' // Used to read the alarm state variable.

	#define VID_DEFAULT_TUNING_PARAMETERS       'd'
	#define VID_STORED_ENGINEERING_MODE		    'e' // Used to store the engineering mode flag in NV memory.
	#define VID_GRID_FREQUENCY				    'f' // Read the grid frequency.
	#define VID_RUN_STATE                       'g' // Used to hold grid off or on state over a power cycle.

	#define VID_MPPT_RAMP_UP                     'i'
	#define VID_POWER_UPPER_LIMIT                'j'	// 106
	#define VID_MPPT_DELAY_RAMP_DN               'k'
	#define VID_MPPT_MODE                        'l'
	#define VID_GRID_AVERAGING_SCALE            'm'
	#define VID_MPPT_RAMP_DN                    'n'

	#define VID_MPPT_DELAY_RAMP_UP				'p'
	#define VID_MPPT_RCB						'q'
    #define VID_MPPT_RAMP_UP_PERCENT            'r'
    #define VID_MPPT_RAMP_DN_PERCENT            's'

	#define VID_UPDATE_TUNING_PARAMETERS   	    'u'

    #define VID_MPPT_RAMP_TRIG                  'v'


/**************************************************/
/*    End of VIDs used by engineering console.    */
/**************************************************/




	void ZNet_ProcessCommands(Uint8 *wiMeshPacket);
	void ZNet_ProcessReadVID(Uint8 *wiMeshPacket);
	void ZNet_ProcessWriteVID(Uint8 *wiMeshPacket);
	void HAL_ResetInverter(void);
	void ZNet_SendNormalReport(Uint8 type);
    
	ErrorCode ZNet_VID_Write(Uint8 vid, Uint8 *data, Uint8 dataLength);
	ErrorCode ZNet_VID_Read(Uint8 vid, Uint8 *data, Uint8 *dataLength);
    
	void ZNet_PA_GetInverterApplicationVersion(Uint8 *pData);

#endif 
