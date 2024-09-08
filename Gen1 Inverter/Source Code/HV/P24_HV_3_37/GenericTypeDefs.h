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


#ifndef __GENERIC_TYPE_DEFS_H_
#define __GENERIC_TYPE_DEFS_H_

/* Specify an extension for GCC based compilers */
#if defined(__GNUC__)
#define __EXTENSION __extension__
#else
#define __EXTENSION
#endif

#if !defined(__PACKED)
    #define __PACKED
#endif

/* get compiler defined type definitions (NULL, size_t, etc) */
#include <stddef.h> 

/* INT is processor specific in length may vary in size */
typedef signed int          INT;
typedef signed char         INT8;
typedef signed short int    INT16;
typedef signed long int     INT32;

/* MPLAB C Compiler for PIC18 does not support 64-bit integers */
#if !defined(__18CXX)
__EXTENSION typedef signed long long    INT64;
#endif

/* UINT is processor specific in length may vary in size */
typedef unsigned int        UINT;
typedef unsigned char       UINT8;
typedef unsigned short int  UINT16;
/* 24-bit type only on C18 */
#if defined(__18CXX)
typedef unsigned short long UINT24;
#endif
typedef unsigned long int   UINT32;     /* other name for 32-bit integer */
/* MPLAB C Compiler for PIC18 does not support 64-bit integers */
#if !defined(__18CXX)
__EXTENSION typedef unsigned long long  UINT64;
#endif

typedef union
{
    UINT8 Val;
    struct
    {
        __EXTENSION UINT8 b0:1;
        __EXTENSION UINT8 b1:1;
        __EXTENSION UINT8 b2:1;
        __EXTENSION UINT8 b3:1;
        __EXTENSION UINT8 b4:1;
        __EXTENSION UINT8 b5:1;
        __EXTENSION UINT8 b6:1;
        __EXTENSION UINT8 b7:1;
    } bits;
} UINT8_VAL, UINT8_BITS;

typedef union 
{
    UINT16 Val;
    UINT8 v[2] __PACKED;
    struct __PACKED
    {
        UINT8 LB;
        UINT8 HB;
    } byte;
    struct __PACKED
    {
        __EXTENSION UINT8 b0:1;
        __EXTENSION UINT8 b1:1;
        __EXTENSION UINT8 b2:1;
        __EXTENSION UINT8 b3:1;
        __EXTENSION UINT8 b4:1;
        __EXTENSION UINT8 b5:1;
        __EXTENSION UINT8 b6:1;
        __EXTENSION UINT8 b7:1;
        __EXTENSION UINT8 b8:1;
        __EXTENSION UINT8 b9:1;
        __EXTENSION UINT8 b10:1;
        __EXTENSION UINT8 b11:1;
        __EXTENSION UINT8 b12:1;
        __EXTENSION UINT8 b13:1;
        __EXTENSION UINT8 b14:1;
        __EXTENSION UINT8 b15:1;
    } bits;
} UINT16_VAL, UINT16_BITS;


typedef union
{
    UINT32 Val;
    UINT16 w[2] __PACKED;
    UINT8  v[4] __PACKED;
    struct __PACKED
    {
        UINT16 LW;
        UINT16 HW;
    } word;
    struct __PACKED
    {
        UINT8 LB;
        UINT8 HB;
        UINT8 UB;
        UINT8 MB;
    } byte;
    struct __PACKED
    {
        UINT16_VAL low;
        UINT16_VAL high;
    }wordUnion;
    struct __PACKED
    {
        __EXTENSION UINT8 b0:1;
        __EXTENSION UINT8 b1:1;
        __EXTENSION UINT8 b2:1;
        __EXTENSION UINT8 b3:1;
        __EXTENSION UINT8 b4:1;
        __EXTENSION UINT8 b5:1;
        __EXTENSION UINT8 b6:1;
        __EXTENSION UINT8 b7:1;
        __EXTENSION UINT8 b8:1;
        __EXTENSION UINT8 b9:1;
        __EXTENSION UINT8 b10:1;
        __EXTENSION UINT8 b11:1;
        __EXTENSION UINT8 b12:1;
        __EXTENSION UINT8 b13:1;
        __EXTENSION UINT8 b14:1;
        __EXTENSION UINT8 b15:1;
        __EXTENSION UINT8 b16:1;
        __EXTENSION UINT8 b17:1;
        __EXTENSION UINT8 b18:1;
        __EXTENSION UINT8 b19:1;
        __EXTENSION UINT8 b20:1;
        __EXTENSION UINT8 b21:1;
        __EXTENSION UINT8 b22:1;
        __EXTENSION UINT8 b23:1;
        __EXTENSION UINT8 b24:1;
        __EXTENSION UINT8 b25:1;
        __EXTENSION UINT8 b26:1;
        __EXTENSION UINT8 b27:1;
        __EXTENSION UINT8 b28:1;
        __EXTENSION UINT8 b29:1;
        __EXTENSION UINT8 b30:1;
        __EXTENSION UINT8 b31:1;
    } bits;
} UINT32_VAL;

/* MPLAB C Compiler for PIC18 does not support 64-bit integers */
#if !defined(__18CXX)
typedef union
{
    UINT64 Val;
    UINT32 d[2] __PACKED;
    UINT16 w[4] __PACKED;
    UINT8 v[8]  __PACKED;
    struct __PACKED
    {
        UINT32 LD;
        UINT32 HD;
    } dword;
    struct __PACKED
    {
        UINT16 LW;
        UINT16 HW;
        UINT16 UW;
        UINT16 MW;
    } word;
    struct __PACKED
    {
        __EXTENSION UINT8 b0:1;
        __EXTENSION UINT8 b1:1;
        __EXTENSION UINT8 b2:1;
        __EXTENSION UINT8 b3:1;
        __EXTENSION UINT8 b4:1;
        __EXTENSION UINT8 b5:1;
        __EXTENSION UINT8 b6:1;
        __EXTENSION UINT8 b7:1;
        __EXTENSION UINT8 b8:1;
        __EXTENSION UINT8 b9:1;
        __EXTENSION UINT8 b10:1;
        __EXTENSION UINT8 b11:1;
        __EXTENSION UINT8 b12:1;
        __EXTENSION UINT8 b13:1;
        __EXTENSION UINT8 b14:1;
        __EXTENSION UINT8 b15:1;
        __EXTENSION UINT8 b16:1;
        __EXTENSION UINT8 b17:1;
        __EXTENSION UINT8 b18:1;
        __EXTENSION UINT8 b19:1;
        __EXTENSION UINT8 b20:1;
        __EXTENSION UINT8 b21:1;
        __EXTENSION UINT8 b22:1;
        __EXTENSION UINT8 b23:1;
        __EXTENSION UINT8 b24:1;
        __EXTENSION UINT8 b25:1;
        __EXTENSION UINT8 b26:1;
        __EXTENSION UINT8 b27:1;
        __EXTENSION UINT8 b28:1;
        __EXTENSION UINT8 b29:1;
        __EXTENSION UINT8 b30:1;
        __EXTENSION UINT8 b31:1;
        __EXTENSION UINT8 b32:1;
        __EXTENSION UINT8 b33:1;
        __EXTENSION UINT8 b34:1;
        __EXTENSION UINT8 b35:1;
        __EXTENSION UINT8 b36:1;
        __EXTENSION UINT8 b37:1;
        __EXTENSION UINT8 b38:1;
        __EXTENSION UINT8 b39:1;
        __EXTENSION UINT8 b40:1;
        __EXTENSION UINT8 b41:1;
        __EXTENSION UINT8 b42:1;
        __EXTENSION UINT8 b43:1;
        __EXTENSION UINT8 b44:1;
        __EXTENSION UINT8 b45:1;
        __EXTENSION UINT8 b46:1;
        __EXTENSION UINT8 b47:1;
        __EXTENSION UINT8 b48:1;
        __EXTENSION UINT8 b49:1;
        __EXTENSION UINT8 b50:1;
        __EXTENSION UINT8 b51:1;
        __EXTENSION UINT8 b52:1;
        __EXTENSION UINT8 b53:1;
        __EXTENSION UINT8 b54:1;
        __EXTENSION UINT8 b55:1;
        __EXTENSION UINT8 b56:1;
        __EXTENSION UINT8 b57:1;
        __EXTENSION UINT8 b58:1;
        __EXTENSION UINT8 b59:1;
        __EXTENSION UINT8 b60:1;
        __EXTENSION UINT8 b61:1;
        __EXTENSION UINT8 b62:1;
        __EXTENSION UINT8 b63:1;
    } bits;
} UINT64_VAL;
#endif /* __18CXX */

/***********************************************************************************/


typedef char                    CHAR8;
typedef unsigned char           UCHAR8;

typedef unsigned char           BYTE;                           /* 8-bit unsigned  */
typedef unsigned short int      WORD;                           /* 16-bit unsigned */
typedef unsigned long           DWORD;                          /* 32-bit unsigned */
/* MPLAB C Compiler for PIC18 does not support 64-bit integers */
__EXTENSION
typedef unsigned long long      QWORD;                          /* 64-bit unsigned */
typedef signed char             CHAR;                           /* 8-bit signed    */
typedef signed short int        SHORT;                          /* 16-bit signed   */
typedef signed long             LONG;                           /* 32-bit signed   */

__EXTENSION


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




#undef __EXTENSION

#endif /* __GENERIC_TYPE_DEFS_H_ */
