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


#include "xc.h"
#include <stdio.h>
#include "p24FJ64GA004.h"
#include <libpic30.h>           


#include "HAL_Uart.h"
#include "Pic24_Bootloader.h"


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








#ifdef Uses_Uart1
volatile unsigned char      uart1_txFifo[ Uart1_TX_FIFO_SIZE ];
volatile unsigned int       uart1_txFifoHead = 0;
volatile unsigned int       uart1_txFifoTail = 0;
volatile unsigned char      uart1_rxFifo[ Uart1_RX_FIFO_SIZE ];
volatile unsigned int       uart1_rxFifoHead = 0;
volatile unsigned int       uart1_rxFifoTail = 0;
#endif

#if defined(Uses_Uart2)
volatile unsigned char      uart2_txFifo[ Uart2_TX_FIFO_SIZE ];
volatile unsigned int       uart2_txFifoHead = 0;
volatile unsigned int       uart2_txFifoTail = 0;
volatile unsigned char      uart2_rxFifo[ Uart2_RX_FIFO_SIZE ];
volatile unsigned int       uart2_rxFifoHead = 0;
volatile unsigned int       uart2_rxFifoTail = 0;
#endif



void Uart1_Set_Baud(unsigned long int baudRate)
{
    unsigned int brg1 = ( FCY_FREQ / baudRate);
	unsigned int brg = (brg1 / 16) - 1;
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
//	UART1_RXIE = 1;         //enable rx interrupt
    UART1_RXIP = 1;         //Set priority
    Uart1_Enable();         //enable the UART

}


#ifdef Uses_Uart1

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

int __attribute__((optimize(3))) Uart1_Get_Tx_Size()
{
	int length;	
    if (uart1_rxFifoHead == uart1_rxFifoTail)
    {
        return 0;
    }
	if (uart1_txFifoTail < uart1_txFifoHead)
		length = uart1_txFifoHead - uart1_txFifoTail;
	else if(uart1_txFifoTail > uart1_txFifoHead)
		 length = Uart1_TX_FIFO_SIZE - uart1_txFifoHead + uart1_txFifoTail;
	else
		length = 0;

	return length;
}

int __attribute__((optimize(3))) Uart1_Get_Tx_Free()
{
	int size = Uart1_Get_Tx_Size();
	return Uart1_TX_FIFO_SIZE - size;	
}

void __attribute__((optimize(3))) Uart1_PutChar(unsigned char value)
{
    unsigned int ptr = uart1_txFifoHead;
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

    uart1_txFifo[ptr] = value;  //  write to buffer
    uart1_txFifoHead = ptr;     //  update the head
	U1STAbits.UTXEN = 1;		//make sure TX is enabled
}

void __attribute__((optimize(3))) Uart1_PutStr(char *s, int size)
{
    int i;
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


int __attribute__((optimize(3))) Uart1_GetChar(void)
{
	if (uart1_rxFifoHead == uart1_rxFifoTail)
    {
        return -1;
    }
    
    uart1_rxFifoTail++;
    if (uart1_rxFifoTail == Uart1_RX_FIFO_SIZE) 
    {
        uart1_rxFifoTail = 0;     // wrap
    }
    
    int c = uart1_rxFifo[ uart1_rxFifoTail ];  
    
    return c;
}


void __attribute__((optimize(3))) Uart1_TX_process(char wait) 
{
	while ( uart1_txFifoHead != uart1_txFifoTail )
	{
		U1STAbits.UTXEN = 1;	//make sure TX is enabled
		while (U1STAbits.UTXBF)
		{
			if (wait==0)
				return;
		}
		//Here if at least one spot in the TX buffer
		uart1_txFifoTail++;      //increment tail pointer
		if (uart1_txFifoTail == Uart1_TX_FIFO_SIZE)
		{
			uart1_txFifoTail = 0; //wrap if needed
		}
		UART1_TXIF = 0;   //clear the interrupt flag
		UART1_TXREG =  uart1_txFifo[ uart1_txFifoTail ];         //put character from fifo to transmit buffer
	}	

}

void __attribute__((optimize(3))) Uart1_RX_process(void) 
{
	unsigned int value;
		
	if((U1STA & 0x000E) != 0x0000)
	{
		value = U1RXREG; 			//dummy read to clear FERR/PERR
		if (value)
		{
		}
		U1STAbits.OERR = 0;			//clear OERR to keep receiving
	}
	
	while (U1STAbits.URXDA)
	{
		value = UART1_RXREG;	// read character
		uart1_rxFifoHead++;     //increment head pointer
  		if (uart1_rxFifoHead == Uart1_RX_FIFO_SIZE)
	    {
			uart1_rxFifoHead = 0;                // wrap
		}
		uart1_rxFifo[ uart1_rxFifoHead ] = value;   //put in buffer
	}
}



#if defined ( Uses_Debug )


void Uart2_Config(unsigned long int baud) 
{
    Uart2_Set_Baud(baud);   // set baud rate
    Uart2_Set_Parity( Uart_Data8_NO_PARITY);    // Set parity 8 bit data, no parity
    Uart2_Set_StopBits(1);  // Set Stop bits
    UART2_RXIF = 0;         //clear the flag
    UART2_RXIP = 1;         //set priority
 // UART2_RXIE = 1;         //enable rx interrupt
    UART2_RXIP = 1;         //Set priority
    Uart2_Enable();         //enable the UART
}


void Uart2_Set_Baud(unsigned long int baudRate)
{
    unsigned int brg1 = ( FCY_FREQ / baudRate);
	unsigned int brg = (brg1 / 16) - 1;
    UART2_MODEbits.BRGH = 0;  
    if (brg < 160)
    {
     brg = (FCY_FREQ / baudRate / 4) - 1;
     UART2_MODEbits.BRGH = 1;
    }
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


void Uart2_PutChar(unsigned char value)
{
    unsigned int ptr = uart2_txFifoHead;
    ptr++;
  
    if (ptr == Uart2_TX_FIFO_SIZE)        // Wrap if needed
    {
        ptr = 0;       // Wrap 
    }
      
    #ifdef Uses_Uart_Debug
    while (ptr == uart2_txFifoTail)
    {
        Uart_ReportError("UART2 TX Fifo buffer is full");
    }
    #endif

    uart2_txFifo[ptr] = value;      //  write to buffer
    uart2_txFifoHead = ptr;       //  update the head
    if (UART2_TXIE==0)
    {
//        UART2_TXIE = 1;                  //  Enable interrupts
    }
}

void Uart2_PutStr(char *s, int size)
{
    int i;
    for (i = 0; i < size; i++)
    {
        Uart2_PutChar( s[i]);
    }
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
    
    uart2_rxFifoTail++;
    if (uart2_rxFifoTail == Uart2_RX_FIFO_SIZE) 
    {
        uart2_rxFifoTail = 0;     // wrap
    }
    
    int c = uart2_rxFifo[ uart2_rxFifoTail ];  
    
    return c;
    
}


void Uart2_TX_process(char wait) 
{
	while ( uart2_txFifoHead != uart2_txFifoTail )
	{
		U2STAbits.UTXEN = 1;	//make sure TX is enabled
		while (U2STAbits.UTXBF)
		{
			if (wait==0)
				return;
		}
		//Here if at least one spot in the TX buffer
		uart2_txFifoTail++;      //increment tail pointer
        
		if (uart2_txFifoTail == Uart2_TX_FIFO_SIZE)
		{
			uart2_txFifoTail = 0; //wrap if needed
		}
        
		UART2_TXIF = 0;   //clear the interrupt flag
		UART2_TXREG =  uart2_txFifo[ uart2_txFifoTail ];         //put character from fifo to transmit buffer
	}
}


void Uart2_RX_process() 
{
	unsigned int value;
		
	if((U2STA & 0x000E) != 0x0000)
	{
		value = U2RXREG; 			//dummy read to clear FERR/PERR
		if (value)
		{
		}
		U2STAbits.OERR = 0;			//clear OERR to keep receiving
	}
	
	if(U2STAbits.URXDA)
	{
//		UART1_RXIF = 0;          // clear UART RX interrupt bit
//		Uart1_CheckRxError();
		value = UART2_RXREG;	// read character
		uart2_rxFifoHead++;     //increment head pointer
  		if (uart2_rxFifoHead == Uart2_RX_FIFO_SIZE)
	    {
			uart2_rxFifoHead = 0;                // wrap
		}
		uart2_rxFifo[ uart2_rxFifoHead ] = value;   //put in buffer
	}
}

#endif

void Uart_process()
{
	Uart1_RX_process(); 
	Uart1_TX_process(0);
	#if defined(Uses_Debug)
	Uart2_RX_process(); 
	Uart2_TX_process(0);
	#endif
}



/*
void UART_ISR_PSV _U1TXInterrupt (void) 
{
    Uart1_TX_Isr();
}

void UART_ISR_PSV _U1RXInterrupt (void) 
{
    Uart1_RX_Isr();
}
*/


#endif

