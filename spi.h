/*****************************************************************************
 *   spi.h:  Header file for Philips LPC214x Family Microprocessors
 *
 *   Copyright(C) 2006, Philips Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2005.10.01  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#ifndef __SPI_H__
#define __SPI_H__

/* this flag is to test SPI in either interrupt mode or polling */
#define INTERRUPT_MODE	0

/* SPI read and write buffer size */
#define BUFSIZE		10

/* Delay count after each write */
#define DELAY_COUNT	10

#define SPI0_INT_FLAG	0x01

/* SPI select pin */
#define SPI0_SEL	0x00000080
#define MAX_TIMEOUT	0xFF

#define SPI0_ABORT	0x01		/* below two are SPI0 interrupt */
#define SPI0_MODE_FAULT	0x02
#define SPI0_OVERRUN	0x04
#define SPI0_COL	0x08
#define SPI0_TX_DONE	0x10

#define ABRT		1 << 3		/* SPI0 interrupt status */
#define MODF		1 << 4
#define ROVR		1 << 5
#define WCOL		1 << 6
#define SPIF		1 << 7

#define RORIC		0x00000001
#define RTIC		0x00000002

/* SPI 0 PCR register */
#define SPI0_BE		0x00000004
#define SPI0_CPHA	0x00000008
#define SPI0_CPOL	0x00000010
#define SPI0_MSTR	0x00000020
#define SPI0_LSBF	0x00000040
#define SPI0_SPIE	0x00000080

/* ATMEL 25010 SEEPROM command set */
#define WREN		0x06		/* MSB A8 is set to 0, simplifying test */
#define WRDI		0x04
#define RDSR		0x05
#define WRSR		0x01
#define READ		0x03
#define WRITE		0x02

/* RDSR status bit definition */
#define RDSR_RDY	0x01
#define RDSR_WEN	0x02
 
extern void SPI0Handler (void) __irq;
extern unsigned int SPIInit( void );
extern void SPISend( unsigned char *Buf, unsigned int Length );
extern void SPIReceive( unsigned char *Buf, unsigned int Length );
extern unsigned char SPIReceiveByte( void );
extern unsigned char spiput(unsigned char);

#endif  /* __SPI_H__ */
/*****************************************************************************
**                            End Of File
******************************************************************************/