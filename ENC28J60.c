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
#include "enc28j60.h"

extern unsigned char SetFlagSPI;
unsigned char sendbuff[20],*sendbuffptr;


void ENC28j60()
{
   if(!SetFlagSPI)return;
   SetFlagSPI=0;

   sendbuff[0] = 1;
   sendbuff[1] = 2;
   sendbuff[2] = 3;
   sendbuff[3] = 4;
   sendbuff[4] = 5;
   sendbuffptr= sendbuff;
   IO0SET |= (1<<7);
   SPISend(sendbuffptr,10);
   IOCLR0 |= (1<<7);
}