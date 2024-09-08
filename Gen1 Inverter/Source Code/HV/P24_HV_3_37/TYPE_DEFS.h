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



#if !defined( _TYPE_DEF_H )
#define _TYPE_DEF_H



// Specify an extension for GCC based compilers
#if defined(__GNUC__)
#define __EXTENSION __extension__
#else
#define __EXTENSION
#endif

#if !defined(__PACKED)
    #define __PACKED
#endif

	typedef unsigned char           BYTE;                           /* 8-bit unsigned  */
	typedef unsigned short int      WORD;                           /* 16-bit unsigned */
    typedef unsigned char 		Uint8;
    typedef signed char 		Sint8;
    typedef unsigned int 		Uint16;
    typedef signed int 			Sint16;
    typedef unsigned long int 	Uint32;
    typedef unsigned long int 	Sint32;

	typedef unsigned long           DWORD;                          /* 32-bit unsigned */

typedef union
{
    BYTE Val;
    struct __PACKED
    {
        __EXTENSION BYTE b0:1;
        __EXTENSION BYTE b1:1;
        __EXTENSION BYTE b2:1;
        __EXTENSION BYTE b3:1;
        __EXTENSION BYTE b4:1;
        __EXTENSION BYTE b5:1;
        __EXTENSION BYTE b6:1;
        __EXTENSION BYTE b7:1;
    } bits;
} BYTE_VAL, BYTE_BITS;

typedef union
{
    WORD Val;
    BYTE v[2] __PACKED;
    struct __PACKED
    {
        BYTE LB;
        BYTE HB;
    } byte;
    struct __PACKED
    {
        __EXTENSION BYTE b0:1;
        __EXTENSION BYTE b1:1;
        __EXTENSION BYTE b2:1;
        __EXTENSION BYTE b3:1;
        __EXTENSION BYTE b4:1;
        __EXTENSION BYTE b5:1;
        __EXTENSION BYTE b6:1;
        __EXTENSION BYTE b7:1;
        __EXTENSION BYTE b8:1;
        __EXTENSION BYTE b9:1;
        __EXTENSION BYTE b10:1;
        __EXTENSION BYTE b11:1;
        __EXTENSION BYTE b12:1;
        __EXTENSION BYTE b13:1;
        __EXTENSION BYTE b14:1;
        __EXTENSION BYTE b15:1;
    } bits;
} WORD_VAL, WORD_BITS;



typedef union
{
    DWORD Val;
    WORD w[2] __PACKED;
    BYTE v[4] __PACKED;
    struct __PACKED
    {
        WORD LW;
        WORD HW;
    } word;
    struct __PACKED
    {
        BYTE LB;
        BYTE HB;
        BYTE UB;
        BYTE MB;
    } byte;
    struct __PACKED
    {
        WORD_VAL low;
        WORD_VAL high;
    }wordUnion;
    struct __PACKED
    {
        __EXTENSION BYTE b0:1;
        __EXTENSION BYTE b1:1;
        __EXTENSION BYTE b2:1;
        __EXTENSION BYTE b3:1;
        __EXTENSION BYTE b4:1;
        __EXTENSION BYTE b5:1;
        __EXTENSION BYTE b6:1;
        __EXTENSION BYTE b7:1;
        __EXTENSION BYTE b8:1;
        __EXTENSION BYTE b9:1;
        __EXTENSION BYTE b10:1;
        __EXTENSION BYTE b11:1;
        __EXTENSION BYTE b12:1;
        __EXTENSION BYTE b13:1;
        __EXTENSION BYTE b14:1;
        __EXTENSION BYTE b15:1;
        __EXTENSION BYTE b16:1;
        __EXTENSION BYTE b17:1;
        __EXTENSION BYTE b18:1;
        __EXTENSION BYTE b19:1;
        __EXTENSION BYTE b20:1;
        __EXTENSION BYTE b21:1;
        __EXTENSION BYTE b22:1;
        __EXTENSION BYTE b23:1;
        __EXTENSION BYTE b24:1;
        __EXTENSION BYTE b25:1;
        __EXTENSION BYTE b26:1;
        __EXTENSION BYTE b27:1;
        __EXTENSION BYTE b28:1;
        __EXTENSION BYTE b29:1;
        __EXTENSION BYTE b30:1;
        __EXTENSION BYTE b31:1;
    } bits;
} DWORD_VAL;







    
	typedef enum 
	{
		FALSE = 0,
		TRUE
	} Boolean;
    
	typedef enum 
	{
		ERROR_OK_E,
		ERROR_OVER_TEMPERATURE_E,
		ERROR_UNDER_TEMPERATURE_E,
		ERROR_OUT_OF_RANGE_E,
		ERROR_VOLTAGE_OUT_OF_RANGE_E,
		ERROR_CURRENT_OUT_OF_RANGE_E,
		ERROR_FREQUENCY_OUT_OF_RANGE_E,
		ERROR_INVALID_STATE_E,
		ERROR_INVALID_POINTER_E,
		ERROR_NOT_READY_E,
		ERROR_UNKNOWN_VALUE_E,
		NUMBER_OF_ERROR_CODES
	} ErrorCode;


//	#define	TRUE		1
//	#define	FALSE		0
	
	#define ENABLE      (1)
	#define DISABLE     (0)
	#define ON          (1)
	#define OFF         (0)

	#define SET(flag)       (flag = 1)
	#define CLEAR(flag)     (flag = 0)
	
	
// Bit numbers
	#define BIT_0       (1 << 0)
	#define BIT_1       (1 << 1)
	#define BIT_2       (1 << 2)
	#define BIT_3       (1 << 3)
	#define BIT_4       (1 << 4)
	#define BIT_5       (1 << 5)
	#define BIT_6       (1 << 6)
	#define BIT_7       (1 << 7)
	#define BIT_8       (1 << 8)
	#define BIT_9       (1 << 9)
	#define BIT_10      (1 << 10)
	#define BIT_11      (1 << 11)
	#define BIT_12      (1 << 12)
	#define BIT_13      (1 << 13)
	#define BIT_14      (1 << 14)
	#define BIT_15      (1 << 15)


/*******************************************************************/
/* Macro:     	GET_U8_x_FROM_16                                   */
/* Description: Return byte x of a 16-bit value.                   */
/*              NOTE: input is NOT type checked.                   */
/* Parameters:  value;	16-bit input value.                        */
/* Returns:     byte x.                                            */
/*******************************************************************/
	#define GET_U8_0_FROM_16(value) 	((Uint8)( value & 0x00ff))
	#define GET_U8_1_FROM_16(value) 	((Uint8)((value & 0xff00) >> 8))

	#define GET_16_FROM_2X8(slice1, slice0) 	(((((Uint16)slice1) << 8) & 0xff00) + (((Uint16)slice0) & 0x00ff))

//	#define	CALC_PERCENTAGE_U16(_value_, _percent_)		((Uint32) _value_ * _percent_) / 100 
	
	#define	CALC_PERCENTAGE(_value_, _percent_)			(Uint16) (((Uint32) _value_ * _percent_) / 100)












	
	
#endif //