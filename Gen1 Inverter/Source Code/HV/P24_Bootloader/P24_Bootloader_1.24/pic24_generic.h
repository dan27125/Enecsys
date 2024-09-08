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



#ifndef _PIC24_GENERIC_H_
#define _PIC24_GENERIC_H_



/** unsigned 8-bit integers
 * \note Prefix for all uint8s is <em>u8_</em>
 */
typedef unsigned char       uint8;   //8 bits
/** unsigned 16-bit integers
 * \note Prefix for all uint16s is <em>u16_</em>
 */
typedef unsigned short      uint16;  //16 bits
/** unsigned 32-bit integers
 * \note Prefix for all uint32s is <em>u32_</em>
 */
typedef unsigned long       uint32;  //32 bits
/** unsigned 64-bit integers
 * \note Prefix for all uint64s is <em>u64_</em>
 */
typedef unsigned long long  uint64;  //64 bits

/** signed 8-bit integers
 * \note Prefix for all int8s is <em>i8_</em>
 */
typedef signed char         int8;    //8 bits
/** signed 16-bit integers
 * \note Prefix for all int16s is <em>i16_</em>
 */
typedef signed short        int16;   //16 bits
/** signed 32-bit integers
 * \note Prefix for all int32s is <em>i32_</em>
 */
typedef signed long         int32;   //32 bits
/** signed 64-bit integers
 * \note Prefix for all int64s is <em>i64_</em>
 */
typedef signed long long    int64;   //64 bits

typedef union _union16 {
  uint16 u16;
  uint8 u8[2];
} union16;

typedef union _union32 {
  uint32 u32;

  struct {
    uint16 ls16;
    uint16 ms16;
  } u16;

  uint8 u8[4];
} union32;

typedef union _union64 {
  uint32 u32[2];
  uint16 u16[4];
  uint8 u8[8];
} union64;


#endif



