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



#define BAD             (255)
#define DECODE64(c)     (isascii(c) ? base64val[c] : BAD)
#define isascii(x)      TRUE


const Uint8 base64digits[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";
 
const Uint8 base64val[] = {
     BAD,BAD,BAD,BAD, BAD,BAD,BAD,BAD, BAD,BAD,BAD,BAD, BAD,BAD,BAD,BAD,
     BAD,BAD,BAD,BAD, BAD,BAD,BAD,BAD, BAD,BAD,BAD,BAD, BAD,BAD,BAD,BAD,
     BAD,BAD,BAD,BAD, BAD,BAD,BAD,BAD, BAD,BAD,BAD,BAD, BAD,62 ,BAD,BAD,
      52, 53, 54, 55,  56, 57, 58, 59,  60, 61,BAD,BAD, BAD,BAD,BAD,BAD,
     BAD,  0,  1,  2,   3,  4,  5,  6,   7,  8,  9, 10,  11, 12, 13, 14,
      15, 16, 17, 18,  19, 20, 21, 22,  23, 24, 25,BAD, BAD,BAD,BAD, 63,
     BAD, 26, 27, 28,  29, 30, 31, 32,  33, 34, 35, 36,  37, 38, 39, 40,
      41, 42, 43, 44,  45, 46, 47, 48,  49, 50, 51,BAD, BAD,BAD,BAD,BAD
 };


Uint8 to64frombits(Uint8 *out, const Uint8 *in,Uint16 inlen)
 {
	 Uint8 count=0;
	 
         for (; inlen >= 3; inlen -= 3)
         {
                 *out++ = base64digits[in[0] >> 2];
                 *out++ = base64digits[((in[0] << 4) & 0x30) | (in[1] >> 4)];
                 *out++ = base64digits[((in[1] << 2) & 0x3c) | (in[2] >> 6)];
                 *out++ = base64digits[in[2] & 0x3f];
                 in += 3;
				 count += 4;
         }
 
         if (inlen > 0)
         {
                 Uint8 fragment;
 
                 *out++ = base64digits[in[0] >> 2];
                 fragment = (in[0] << 4) & 0x30;
 
                 if (inlen > 1)
                         fragment |= in[1] >> 4;
 
                 *out++ = base64digits[fragment];
                 *out++ = (inlen < 2) ? '=' : base64digits[(in[1] << 2) & 0x3c];
                 *out++ = '=';
				 count += 4;
         }
         
         *out = '\0';
		 return count;
 }
 
 
Uint16 from64tobits(Uint8 *out, Uint8 *in, Uint8 inlen)
{
         Uint16 len = 0;
         Uint8 digit1, digit2, digit3, digit4;
 
         if (in[0] == '-' && in[1] == ' ')
                 in += 2;
         if (inlen == 0)
                 return(0);
 
         do {
                 digit1 = in[0];
                 if (DECODE64(digit1) == BAD)
                         return(0);
                 digit2 = in[1];
                 if (DECODE64(digit2) == BAD)
                         return(0);
                 digit3 = in[2];
                 if (digit3 != '=' && DECODE64(digit3) == BAD)
                         return(0);
                 digit4 = in[3];
                 if (digit4 != '=' && DECODE64(digit4) == BAD)
                         return(0);
                 in += 4;
				 inlen -= 4;
                 *out++ = (DECODE64(digit1) << 2) | (DECODE64(digit2) >> 4);
                 ++len;
                 if (digit3 != '=')
                 {
                         *out++ = ((DECODE64(digit2) << 4) & 0xf0) | (DECODE64(digit3) >> 2);
                         ++len;
                         if (digit4 != '=')
                         {
                                 *out++ = ((DECODE64(digit3) << 6) & 0xc0) | DECODE64(digit4);
                                 ++len;
                         }
                 }
         } while (*in && inlen && digit4 != '=');
 
         return (len);
}



/*******************************************************************/
/*                                                                 */
/* Function:    doBase64Encode                                     */
/*                                                                 */
/* Description: base 64 encoder                                    */
/*                                                                 */
/* Parameters:  inBuffer                                           */
/*              outBuffer                                          */
/*              inDataLength                                       */
/*                                                                 */
/* Returns:   	outDataLength                                      */
/*                                                                 */
/*******************************************************************/
Uint8 doBase64Encode(Uint8 *inBuffer, Uint8 *outBuffer, Uint8 inDatalength)
{
    Uint8 outDataLength=0;
    
    outDataLength = to64frombits(outBuffer,inBuffer,inDatalength);
    
    return outDataLength;
}


/*******************************************************************/
/*                                                                 */
/* Function:    doBase64Decode                                     */
/*                                                                 */
/* Description: base 64 decoder                                    */
/*                                                                 */
/* Parameters:  inBuffer                                           */
/*              outBuffer                                          */
/*              inDataLength                                       */
/*                                                                 */
/* Returns:   	outDataLength                                      */
/*                                                                 */
/*******************************************************************/
Uint8 doBase64Decode(Uint8 *inBuffer, Uint8 *outBuffer, Uint8 inDatalength)
{
    Uint8 outDataLength=0;
    Uint16 len=0;
        
    len = from64tobits(outBuffer,inBuffer,(Uint16)inDatalength);
    if(len > 0)
    {
        outDataLength = len;
    }
    
    return outDataLength;
}





Uint8 AsciiHexChar(Uint8 decimalValue)
{
	if(decimalValue <= 9)
		return decimalValue+0x30;
	else
		return decimalValue+0x37;
}//Uint8 AsciiHexChar(uint8 decimalValue)


Uint8 btohexa_high(Uint8 b)
{
	b >>= 4;
	return (b>0x9u) ? b+'A'-10:b+'0';
}

Uint8 btohexa_low(Uint8 b)
{
	b &= 0x0F;
	return (b>9u) ? b+'A'-10:b+'0';
}

void BinaryToString(Uint8 *src, Uint8 size, Uint8 *dest)
{
	Uint8 count = 0;
	Uint8 indexSrc = size-1;
	
	for(count=0; count<size; count++)
	{
		*(dest+2*count) = btohexa_high(*(src+indexSrc));
		*(dest+2*count+1) = btohexa_low(*(src+indexSrc));
		indexSrc--;
	}	
}
