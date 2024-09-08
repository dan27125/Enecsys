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



#if !defined(_UART_H)
#define _UART_H


    #define Uses_Uart1          1
//  #define Uses_Uart2          1


    #define Uart_Data8_NO_PARITY            0
    #define Uart_Data8_EVENPARITY           1
    #define Uart_Data8_ODDPARITY            2
    #define Uart_Data9_NOPARITY             3


#ifdef Uses_Uart1

    #define Uart1_TX_FIFO_SIZE     255
    #define Uart1_RX_FIFO_SIZE     255

void Uart1_Config(unsigned long int baud);
void Uart1_Set_Baud(unsigned long int baud);
void Uart1_Set_Parity(unsigned char parity);
void Uart1_Set_StopBits(unsigned char  value);
void Uart1_Enable();
void Uart1_PutChar(unsigned char value);
void Uart1_PutStr(char *s, int size);
int Uart1_GetChar(void);

int Uart1_Get_Tx_Size();
//	int Uart1_Get_Tx_Wait();
int Uart1_Get_Tx_Free();

int Uart1_IsCharReady(void);
void Uart1_CheckRxError(void);
void Uart1_TX_Isr();
void Uart1_RX_Isr();
void Uart1_RX_process(void); 
void Uart1_TX_process(char wait); 
void Uart_process();
#endif


#if defined( Uses_Uart1 )
    #define Uart2_TX_FIFO_SIZE     255
    #define Uart2_RX_FIFO_SIZE     255
void Uart2_Set_Baud(unsigned long int baud);
void Uart2_Config(unsigned long int baud);
void Uart2_Set_Parity(unsigned char parity);
void Uart2_Set_StopBits(unsigned char  value);
void Uart2_Enable();
void Uart2_PutChar(unsigned char value);
void Uart2_PutStr(char *s, int size);
int Uart2_GetChar(void);
int Uart2_IsCharReady(void);
void Uart2_CheckRxError(void);
void Uart2_RX_process(void); 
void Uart2_TX_process(char wait); 
#endif






#endif