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


#include "Pic24_Bootloader.h"

/********************************************************************
; Function: 	void WriteMem(WORD cmd)
;
; PreCondition: Appropriate data written to latches with WriteLatch
;
; Input:    	cmd - type of memory operation to perform
;                               
; Output:   	None.
;
; Side Effects: 
;
; Overview: 	Write stored registers to flash memory
;*********************************************************************/

void WriteMem(WORD cmd)
{
	NVMCON = cmd;
	__builtin_write_NVM();
	while(NVMCONbits.WR == 1);
}	

/********************************************************************
; Function: 	void WriteLatch(WORD page, WORD addrLo, 
;
; Input:    	page 	- upper byte of address
;				addrLo 	- lower word of address
;				dataHi 	- upper byte of data
;				addrLo	- lower word of data
;                               
; Side Effects: TBLPAG changed
;
; Overview: 	Stores data to write in hardware latches
;*********************************************************************/	
void WriteLatch(WORD page, WORD addrLo, WORD dataHi, WORD dataLo)
{
	TBLPAG = page;
	__builtin_tblwtl(addrLo,dataLo);
	__builtin_tblwth(addrLo,dataHi);
}	

/********************************************************************
; Function: 	DWORD ReadLatch(WORD page, WORD addrLo)
; Input:    	page 	- upper byte of address
;				addrLo 	- lower word of address
; Output:   	data 	- 32-bit data in W1:W0
; Side Effects: TBLPAG changed
; Overview: 	Read from location in flash memory
;*********************************************************************/
DWORD ReadLatch(WORD page, WORD addrLo)
{
	DWORD_VAL temp;
	TBLPAG = page;
	temp.word.LW = __builtin_tblrdl(addrLo);
	temp.word.HW = __builtin_tblrdh(addrLo);
	return temp.Val;
}


//*********************************************************************
// Function: 	void ResetDevice(WORD addr);
// Input:    	addr 	- 16-bit address to vector to
// Output:   	None.
// Overview: 	used to vector to user code
//*********************************************************************
void ResetDevice(WORD addr)
{
	asm("goto %0" : : "r"(addr));
}

//********************************************************************
// Function: 	void Erase(WORD page, WORD addrLo, WORD cmd);
// Input:    	page 	- upper byte of address
//				addrLo 	- lower word of address
//			cmd		- type of memory operation to perform
// Output:   	None.
// Side Effects: TBLPAG changed
// Overview: 	Erases page of flash memory at input address
//********************************************************************

void Erase(WORD page, WORD addrLo, WORD cmd)
{
	WORD temp;	
	temp = TBLPAG;
	TBLPAG = page;
	NVMCON = cmd;
	__builtin_tblwtl(addrLo,addrLo);
	__builtin_write_NVM();
	while(NVMCONbits.WR == 1);
	TBLPAG = temp;
}
