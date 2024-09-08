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

//	#include "xc.h"
//	#include "HAL_Uart.h"
//	#include <stdio.h>
//	#include <libpic30.h>           


    #define CLOCK_FREQ     (unsigned long) 24000000
    #define FCY_FREQ     ((unsigned long) (CLOCK_FREQ/2))


	#define UART1_STAbits    U1STAbits
	#define UART1_TXIF       _U1TXIF 
	#define UART1_TXIE       _U1TXIE 
	#define UART1_TXREG       U1TXREG
	#define UART1_RXREG      U1RXREG
	#define UART1_RXIE       _U1RXIE 
	#define UART1_RXIF       _U1RXIF
	#define UART1_RXIP      _U1RXIP
	#define UART1_MODEbits  U1MODEbits
	#define UART1_BRG        U1BRG


	#if defined( Uses_Uart2)
	#define UART2_STAbits    U2STAbits
	#define UART2_TXIF       _U2TXIF 
	#define UART2_TXIE       _U2TXIE 
	#define UART2_TXREG       U2TXREG
	#define UART2_RXIE       _U2RXIE 
	#define UART2_RXREG      U2RXREG
	#define UART2_RXIF       _U2RXIF
	#define UART2_RXIP      _U2RXIP
	#define UART2_MODEbits  U2MODEbits
	#define UART2_BRG        U2BRG
	#endif

#ifdef Uses_Uart1
volatile unsigned char      uart1_txFifo[ Uart1_TX_FIFO_SIZE ];
volatile Uint16				uart1_txFifoHead = 0;
volatile Uint16				  uart1_txFifoTail = 0;

volatile unsigned char      uart1_rxFifo[ Uart1_RX_FIFO_SIZE ];

volatile unsigned char      uart1_rx;

	
	volatile Uint8       uart1_rxFifoHead = 0;
	volatile Uint8      uart1_rxFifoTail = 0;
	
#endif

#if defined( Uses_Uart2)
volatile unsigned char      uart2_txFifo[ Uart2_TX_FIFO_SIZE ];
volatile Uint16       uart2_txFifoHead = 0;
volatile Uint16       uart2_txFifoTail = 0;

volatile unsigned char      uart2_rxFifo[ Uart2_RX_FIFO_SIZE ];
volatile Uint16		      uart2_rxFifoHead = 0;
volatile Uint16	       uart2_rxFifoTail = 0;
#endif

#ifdef Uses_Uart_Debug
void Uart_ReportError(char *s)
{
 //   printf("Uart error %s %d  [%s]\n", __FILE__, __LINE__, s);
}
#endif




#if defined( Uses_Uart1)
void Uart1_Set_Baud(unsigned long int baudRate)
{
    Uint16 brg1 = ( FCY_FREQ / baudRate);
	Uint16 brg = (brg1 / 16) - 1;
    UART1_MODEbits.BRGH = 0;  
    if (brg < 160)
    {
     brg = (FCY_FREQ / baudRate / 4) - 1;
     UART1_MODEbits.BRGH = 1;
    }
	UART1_BRG = brg;
}

void Uart1_Set_Parity(unsigned char parity)
{
      UART1_MODEbits.PDSEL = parity;
}


void Uart1_Set_StopBits(unsigned char stop_bits)
{

  UART1_MODEbits.STSEL = stop_bits - 1;        
}


void Uart1_Config(unsigned long int baud) 
{
    Uart1_Set_Baud(baud);   // set baud rate
    Uart1_Set_Parity( Uart_Data8_NO_PARITY);    // Set parity 8 bit data, no parity
    Uart1_Set_StopBits(1);  // Set Stop bits
    UART1_RXIF = 0;         //clear the flag
    UART1_RXIP = 1;         //set priority
    UART1_RXIE = 1;         //enable rx interrupt
    UART1_RXIP = 1;         //Set priority
    Uart1_Enable();         //enable the UART
}


void Uart1_Enable() 
{
  UART1_MODEbits.UEN = 0b00; 
  UART1_MODEbits.UARTEN = 1;
  UART1_STAbits.UTXEN = 1;
}

void Uart1_CheckRxError(void) 
{
    unsigned char c;

    if ( UART1_STAbits.PERR) 
    {
        c = UART1_RXREG;     //clear error
        #ifdef Uses_Uart_Debug
        Uart_ReportError("UART1 parity error");
        #endif
    }
  
    if (UART1_STAbits.FERR) 
    {
        c = UART1_RXREG;     //clear error
        #ifdef Uses_Uart_Debug
        Uart_ReportError("UART1 framing error");
        #endif
    }
  
    if (UART1_STAbits.OERR) 
    {
        UART1_STAbits.OERR = 0;  //clear error
        #ifdef Uses_Uart_Debug
        Uart_ReportError("UART1 overrun error");
        #endif
    }
}

int Uart1_Get_Tx_Size()
{
	Uint16 length;	
	
    if (uart1_txFifoHead == uart1_txFifoTail)
    {	
//		printf("%s,%d empty\n",__FILE__, __LINE__);
        return 0;
    }

	if (uart1_txFifoTail < uart1_txFifoHead)
		length = uart1_txFifoHead - uart1_txFifoTail;
	
	else if(uart1_txFifoTail > uart1_txFifoHead)
	{
//		 length = Uart1_TX_FIFO_SIZE - uart1_txFifoHead + uart1_txFifoTail;
		 length = Uart1_TX_FIFO_SIZE - (uart1_txFifoHead + uart1_txFifoTail);
	}
	else
	{
		length = 0;
//		printf("%s,%d empty\n",__FILE__, __LINE__);
	}

	return length;
}

int Uart1_Get_Tx_Free()
{
	Uint16 size = Uart1_Get_Tx_Size();
	return Uart1_TX_FIFO_SIZE - size;	
}

void Uart1_PutChar(unsigned char value)
{
	Uint16 ptr = uart1_txFifoHead;
	ptr++;
  
    if (ptr == Uart1_TX_FIFO_SIZE)        // Wrap if needed
    {
        ptr = 0;       // Wrap 
    }
      
    #ifdef Uses_Uart_Debug
    while (ptr == uart1_txFifoTail)
    {
        Uart_ReportError("UART1 TX Fifo buffer is full");
    }
    #endif

    uart1_txFifo[ptr] = value;      //  write to buffer
    uart1_txFifoHead = ptr;           //  update the head
	if (UART1_TXIE==0)
		UART1_TXIE = 1;                  //  Enable interrupts
}

void Uart1_PutStr(char *s, int size)
{
	Uint16 i;
	for (i = 0; i < size; i++)
		Uart1_PutChar( s[i]);
}

int Uart1_IsCharReady(void)
{
    if (uart1_rxFifoHead != uart1_rxFifoTail)
    {
        return 1;
    }
    return 0;
}

void Uart1_RxClear(void)
{
	uart1_rxFifoHead = 0;
	uart1_rxFifoTail = 0;
	memset(&uart1_rxFifo,0,sizeof(uart1_rxFifo));
}

int Uart1_GetChar(void)
{
    if (uart1_rxFifoHead == uart1_rxFifoTail)
    {
        return -1;
    }
    
	UART1_RXIE = 0;
    uart1_rxFifoTail++;
    if (uart1_rxFifoTail == Uart1_RX_FIFO_SIZE) 
    {
        uart1_rxFifoTail = 0;     // wrap
    }
    
	Uint16 c = uart1_rxFifo[ uart1_rxFifoTail ];  
    
	UART1_RXIE = 1;
	
    return c;
}

int Uart1_GetCharDebug(void)
{
	Uint16 c = uart1_rx;
	uart1_rx = 0;
	return c;
}

void Uart1_TX_Isr(void) 
{
    if ( uart1_txFifoHead == uart1_txFifoTail )
    {
		UART1_TXIE = 0;       // Empty TX buffer, disable interrupt
    } 
    else 
    {
        //Here if at least one spot in the TX buffer
        uart1_txFifoTail++;      //increment tail pointer
		
        if (uart1_txFifoTail == Uart1_TX_FIFO_SIZE)
        {
            uart1_txFifoTail = 0; //wrap if needed
        }
        
        UART1_TXIF = 0;   //clear the interrupt flag
        UART1_TXREG =  uart1_txFifo[ uart1_txFifoTail ];   //put character from fifo to transmit buffer

    }
}

void UART_ISR_PSV _U1TXInterrupt (void) 
{
    Uart1_TX_Isr();
}

void UART_ISR_PSV _U1RXInterrupt (void) 
{
	Uint16 value;
	
	if((U1STA & 0x000E) != 0x0000)
	{
		value = U1RXREG; 			//dummy read to clear FERR/PERR
		if (value)
		{
		}
		U1STAbits.OERR = 0;			//clear OERR to keep receiving
	}
	
//	**** 2021-09-20 DGA *******************
//	UART HAS FIFO BUFFERRING 
//	CRITICAL TO LOOP WHILE URXDA BIT TRUE 
	while (U1STAbits.URXDA)
	{
	    value = UART1_RXREG;     // read next character
		uart1_rxFifoHead++;      // increment head pointer
  		if (uart1_rxFifoHead == Uart1_RX_FIFO_SIZE)
	    {
	        uart1_rxFifoHead = 0;   // wrap
	    }
		uart1_rxFifo[ uart1_rxFifoHead ] = value;   //put in buffer	
	}
//	**** 2021-09-20 DGA *******************	
	
  UART1_RXIF = 0;    // clear UART RX interrupt bit
}

#endif


#if defined (Uses_Uart2)
//	#if defined (Uses_Debug ) || defined(Uses_LLC_Bridge)

void Uart2_Config(unsigned long int baud) 
{
    Uart2_Set_Baud(baud);   // set baud rate
    Uart2_Set_Parity( Uart_Data8_NO_PARITY);    // Set parity 8 bit data, no parity
    Uart2_Set_StopBits(1);  // Set Stop bits
    UART2_RXIF = 0;         //clear the flag
    UART2_RXIP = 1;         //set priority
    UART2_RXIE = 1;         //enable rx interrupt
    UART2_RXIP = 1;         //Set priority
    Uart2_Enable();         //enable the UART
}


void Uart2_Set_Baud(unsigned long int baudRate)
{
    unsigned int brg1 = ( FCY_FREQ / baudRate);
//	unsigned int brg = ( FCY_FREQ / baudRate / 16) - 1;
	unsigned int brg = (brg1 / 16) - 1;
    UART2_MODEbits.BRGH = 0;  
    if (brg < 160)
    {
     brg = (FCY_FREQ / baudRate / 4) - 1;
     UART2_MODEbits.BRGH = 1;
    }
//	brg = 25;
//	UART2_MODEbits.BRGH = 1;
	UART2_BRG = brg;

}

void Uart2_Set_Parity(unsigned char parity)
{
      UART2_MODEbits.PDSEL = parity;
}


void Uart2_Set_StopBits(unsigned char stop_bits)
{

  UART2_MODEbits.STSEL = stop_bits - 1;        
}

void Uart2_Enable() 
{
  UART2_MODEbits.UEN = 0b00; 
  UART2_MODEbits.UARTEN = 1;
  UART2_STAbits.UTXEN = 1;
  UART2_RXIE = 1;
}

void Uart2_Disable() 
{
  UART2_MODEbits.UARTEN = 0;
  UART2_STAbits.UTXEN = 0;
  UART2_RXIE = 0;
}


void Uart2_CheckRxError(void) 
{
    unsigned char c;

    if ( UART2_STAbits.PERR) 
    {
        c = UART2_RXREG;     //clear error
        #ifdef Uses_Uart_Debug
        Uart_ReportError("UART2 parity error");
        #endif
    }
  
    if (UART2_STAbits.FERR) 
    {
        c = UART2_RXREG;     //clear error
        #ifdef Uses_Uart_Debug
        Uart_ReportError("UART2 framing error");
        #endif
    }
  
    if (UART2_STAbits.OERR) 
    {
        UART2_STAbits.OERR = 0;  //clear error
        #ifdef Uses_Uart_Debug
        Uart_ReportError("UART2 overrun error");
        #endif
    }
 
    
}


int Uart2_Get_Tx_Size()
{
	int length = 0;	
	
    if (uart2_txFifoHead == uart2_txFifoTail)
    {	
//		printf("%s,%d empty\n",__FILE__, __LINE__);
        return 0;
    }

	if (uart2_txFifoTail < uart2_txFifoHead)
		length = uart2_txFifoHead - uart2_txFifoTail;
	
	else if(uart2_txFifoTail > uart2_txFifoHead)
	{
		 length = Uart2_TX_FIFO_SIZE - uart2_txFifoHead + uart2_txFifoTail;
	}

	return length;
}

int Uart2_Get_Tx_Free()
{
	int size = Uart2_Get_Tx_Size();
	return Uart2_TX_FIFO_SIZE - size;	
}

void Uart2_PutChar(unsigned char value)
{
	unsigned int ptr;
	
//	UART2_TXIE = 0;
//	UART2_TXIE = 0;       // Empty TX buffer, disable interrupt	
	
	ptr = uart2_txFifoHead;
	ptr++;
  
    if (ptr == Uart2_TX_FIFO_SIZE)        // Wrap if needed
    {
	    ptr = 0;       // Wrap 
//		return;
    }
    
	uart2_txFifoHead = ptr;       //  update the head
	uart2_txFifo[ptr] = value;    //  write to buffer
	UART2_TXIE = 1;               //  Enable interrupts
}

void Uart2_write(char *s, int size)
{
    int i;
    for (i = 0; i < size; i++)
    {
		Uart2_PutChar( s[i]);
    }
}

void Uart2_PutStr(char *s)
{
	Uart2_write(s, strlen(s) );
}


int Uart2_IsCharReady(void)
{
    if (uart2_rxFifoHead != uart2_rxFifoTail)
    {
        return 1;
    }
    return 0;
}
    

int Uart2_GetChar(void)
{
	if (uart2_rxFifoHead == uart2_rxFifoTail)
    {
        return -1;
    }
    
	UART2_RXIE = 0;
	
    uart2_rxFifoTail++;
    if (uart2_rxFifoTail == Uart2_RX_FIFO_SIZE) 
    {
        uart2_rxFifoTail = 0;     // wrap
    }
	
    int c = uart2_rxFifo[ uart2_rxFifoTail ];  
    
	UART2_RXIE = 1;
	
    return c;
}


void Uart2_TX_Isr(void) 
{
    if ( uart2_txFifoHead == uart2_txFifoTail )
    {
		UART2_TXIE = 0;       // Empty TX buffer, disable interrupt
    } 
    else 
    {
        //Here if at least one spot in the TX buffer
		uart2_txFifoTail++;      //increment tail pointer
		
        if (uart2_txFifoTail == Uart2_TX_FIFO_SIZE)
        {
            uart2_txFifoTail = 0; //wrap if needed
        }
        
        UART2_TXIF = 0;   //clear the interrupt flag
        UART2_TXREG =  uart2_txFifo[ uart2_txFifoTail ];   //put character from fifo to transmit buffer
    }
}


void Uart2_Put_Char_RX(Uint8 c) 
{
	uart2_rxFifoHead++;                     //increment head pointer
  
    if (uart2_rxFifoHead == Uart2_RX_FIFO_SIZE)
    {
        uart2_rxFifoHead = 0;                // wrap
    }
	uart2_rxFifo[ uart2_rxFifoHead ] = c;   //put in buffer   
}


void Uart2_RX_Isr(void) 
{
    UART2_RXIF = 0;                         // clear UART RX interrupt bit
    Uart2_CheckRxError();
    Uint8 c = UART2_RXREG;       // read character
	Uart2_Put_Char_RX(c);
}


void UART_ISR_PSV _U2TXInterrupt (void) 
{
    Uart2_TX_Isr();
}

void UART_ISR_PSV _U2RXInterrupt (void) 
{
    Uart2_RX_Isr();
}

#endif


#ifdef Uses_Uart1
#ifdef Uses_Uart2

int Uart_IsCharReady(void)
{
    if (__C30_UART == 1)
        return Uart1_IsCharReady();
    return Uart2_IsCharReady();
}
        
int Uart_GetChar(void)
{
    if (__C30_UART == 1)
        return Uart1_GetChar();
    return Uart2_GetChar();
}    

#endif
#endif