#include<LPC214X.h>
#include <stdio.h>


unsigned char a, rxdat,rxdat1,data,NEWS;
unsigned int gllready,functonfinished;
unsigned int count=0;
unsigned int i1=0,GPS_DATALOG,datamove=0,datamove1=0;
unsigned char GPS_Longitude[9];
unsigned char GPS_Latitude[9];

void Init_VIC(void);
void UART0ISR(void)__irq;
void UART1ISR(void)__irq;

void Init_VIC(void)
	{
					VICVectCntl2 = 0x26;
					VICVectAddr2 = (unsigned long int)UART0ISR;
					VICIntEnable |= 0x00000040;          //enable UART0 interrupt


					VICVectAddr3 = (unsigned long int)UART1ISR;
					VICVectCntl3 = 0x27;       
					VICIntEnable |= 0x00000080;//enable UART1 interrupt
}
	
void UART0ISR(void)__irq   // Interuppt routine for uart 
{
   // if(U0IIR & RDA)
	 //U0THR =rxdat1;   //  uart1 receive data transmit to uart0 
   //VICVectAddr = 0x00;
}
                                
void UART1ISR(void)__irq
			{	
				{
					if(U1IIR & RDA)
					rxdat1 = U1RBR;
					data = rxdat1;
					

					
					
					
					}
			
			
			while(!(U1LSR & THRE));//U1THR = rxdat;    //  uart1 receive data transmit to uart0 
   VICVectAddr = 0x00;
}
