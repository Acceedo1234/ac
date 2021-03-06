/*****************************************************************************
 *   spi.c:  SPI C file for Philips LPC214x Family Microprocessors
 *
 *   Copyright(C) 2006, Philips Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2005.10.01  ver 1.00    Prelimnary version, first Release
 *
*****************************************************************************/
#include<LPC214X.h>			/* LPC21XX Peripheral Registers	*/
#include "spi.h"

volatile unsigned int SPI0Status = 0;
volatile unsigned int TxCounter = 0;

/*****************************************************************************
** Function name:		SPI0Handler
**
** Descriptions:		SPI0 interrupt handler
**
** parameters:			None
** Returned value:		None
** 

SCK  : P0.4
MISO : P0.5
MOSI : P0.6
CS   : P0.7
*****************************************************************************/
void SPI0Handler (void) __irq 
{
    unsigned int regValue;
  
    S0SPINT = SPI0_INT_FLAG;		/* clear interrupt flag */
 //   IENABLE;				/* handles nested interrupt */

    regValue = S0SPSR;
    if ( regValue & WCOL )
    {
	SPI0Status |= SPI0_COL;
    }
    if ( regValue & SPIF )
    {
	SPI0Status |= SPI0_TX_DONE;
	TxCounter++;
    }
 //   IDISABLE;
    VICVectAddr = 0;		/* Acknowledge Interrupt */
}

/*****************************************************************************
** Function name:		SPIInit
**
** Descriptions:		SPI port initialization routine
**				
** parameters:			None
** Returned value:		true or false, if the interrupt handler
**				can't be installed correctly, return false.
** 
*****************************************************************************/
unsigned int SPIInit( void )
{
    TxCounter = 0;

    S0SPCR = 0x00;
    PINSEL0 &= 0xFFFF00FF;
    PINSEL0 |= 0x00001500;	   //PINSEL0 is set up to use the GPIO manually as the chipselect line instead of using SSEL0
    IO0DIR |= (1<<7);		  //The code manually choices to use a GPIO pin as SC/CE
	IO0SET |= (1<<7);
//	IODIR0 = SPI0_SEL;		
    IOSET0 = SPI0_SEL;

    /* Setting SPI0 clock, for Atmel SEEPROM, SPI clock should be no more 
    than 3Mhz on 4.5V~5.5V, no more than 2.1Mhz on 2.7V~5.5V */
    S0SPCCR = 0x8;
#if INTERRUPT_MODE
  if ( install_irq( SPI0_INT, (void *)SPI0Handler ) == FALSE )
    {
	return (FALSE);
    }
    /* 8 bit, CPOL=CPHA=0, master mode, MSB first, interrupt enabled */
    S0SPCR = SPI0_SPIE | SPI0_MSTR;
#else
    S0SPCR = SPI0_MSTR;
#endif
    return( 1 );
}

/*****************************************************************************
** Function name:		SPISend
**
** Descriptions:		Send a block of data to the SPI port, the first
**				parameter is the buffer pointer, the 2nd 
**				parameter is the block length.
**
** parameters:			buffer pointer, and the block length
** Returned value:		None
** 
*****************************************************************************/
void SPISend( unsigned char *buf, unsigned int Length )
{
    unsigned int i;
    unsigned char Dummy;

    if ( Length == 0 )
	return;
    for ( i = 0; i < Length; i++ )
    {
	S0SPDR = *buf;
#if INTERRUPT_MODE
	/* In the interrupt, there is nothing to be done if TX_DONE, SPI transfer 
	complete bit, is not set, so it's polling if the flag is set or not which 
	is being handled inside the ISR. Not an ideal example but show how the 
	interrupt is being set and handled. */ 
	while ( (SPI0Status & SPI0_TX_DONE) != SPI0_TX_DONE );
	SPI0Status &= ~SPI0_TX_DONE;
#else
	while ( !(S0SPSR & SPIF) );
#endif
	Dummy = S0SPDR;		/* Flush the RxFIFO */
	buf++;
    }
    return; 
}

/*****************************************************************************
** Function name:		SPIReceive
** Descriptions:		the module will receive a block of data from 
**				the SPI, the 2nd parameter is the block length.
** parameters:			buffer pointer, and block length
** Returned value:		None
** 
*****************************************************************************/
void SPIReceive( unsigned char *buf, unsigned int Length )
{
    unsigned int i;

    for ( i = 0; i < Length; i++ )
    {
	*buf = SPIReceiveByte();
	buf++;
    }
    return; 
}

/*****************************************************************************
** Function name:		SPIReceiveByte
**
** Descriptions:		Receive one byte of data from the SPI port
**				Write a dummy byte, wait until SPI transfer
**				complete, then, read the data register to
**				get the SPI data.
**
** parameters:			None
** Returned value:		the data byte received
** 
*****************************************************************************/
unsigned char SPIReceiveByte( void )
{
    unsigned char data;

    /* wrtie dummy byte out to generate clock, then read data from MISO */
    S0SPDR = 0xFF;
    /* Wait for transfer complete, SPIF bit set */
#if INTERRUPT_MODE
    /* In the receive routine, there is nothing to be done if TX_DONE, or
    SPI transfer complete bit, is not set, so it's polling if the flag is set 
    or not which is being handled inside the ISR. Not an ideal example but 
    show how the interrupt is being set and handled. */ 
    while ( (SPI0Status & SPI0_TX_DONE) != SPI0_TX_DONE );
    SPI0Status &= ~SPI0_TX_DONE;
#else
    while ( !(S0SPSR & SPIF) );
#endif
    data = S0SPDR;
    return ( data ); 
}

unsigned char spiput(unsigned char valueIn)
{
	unsigned char res;
	S0SPDR = valueIn;
	while ( !(S0SPSR & SPIF) );
	res = S0SPDR;

	return 	res;
}

/******************************************************************************
**                            End Of File
******************************************************************************/

