#include<LPC214X.h>

void Init_UART0(void);
void Init_UART1(void);
//void putchar1(int ch);
//void print_uart1(void);
void LPC_ASC_vSendData (int ch);
void LPC_ASC0_vSendData (int ch);
unsigned short int fdiv;
char uart1_buf[];


void Init_UART0(void)
{
    PINSEL0 |= 0x00000001;									// Select P0.0 = TxD(UART0)
  	PINSEL0 |= 0x00000004;									// Select P0.1 = RxD(UART0)
	U0LCR = 0X83;
    U0FDR = 0xa1;
    fdiv  = ((12000000 * 10)/(16 * 9600* 11));
    U0DLM= fdiv/256;
    U0DLL = fdiv%256;
    U0LCR = 0x03;
    U0FCR = 0x01;
    U0IER = 0x00000003;    //RI and TI interrupt
}
			 
void Init_UART1(void)
{
/*        formula for BR calculation:    =PCLK x mulval / 
                            [(16 x (16 x U0DLM + U0DLL))x(MULVAL + DIVADDVAL)
        9609 = 48Mhz x 5 / [(16 x (16 x 0 + 223)) x (5 + 7)]    */
	PINSEL0 |= 0x00010000;									// Select P0.8 = TxD(UART1)
	PINSEL0 |= 0x00040000;									// Select P0.9 = RxD(UART1)
    U1LCR = 0X83;
    U1FDR = 0xa3;
    fdiv  = ((12000000 * 10)/(16 * 115200 * 13));
    U1DLM= fdiv/256;
    U1DLL = fdiv%256;
    U1LCR = 0x03;
    U1FCR = 0x01;
    U1IER = 0x00000003;    //RI and TI interrupt
}

/****************************/
/* Write character to UART1 */
/****************************/
void LPC_ASC_vSendData (int ch)  
{                  
  while (!(U1LSR & 0x20)); 									// Wait TXD Buffer Empty
  U1THR = ch;
						    			// Write Character
}

/****************************/
/* Write character to UART0 */
/****************************/
void LPC_ASC0_vSendData (int ch)  
{                  
  while (!(U0LSR & 0x20)); 									// Wait TXD Buffer Empty
  U0THR = ch;	
					    			// Write Character
}

/***************************/
/* Print String to UART[1] */
/***************************/
/*void print_uart1(void)
{
  char *p;													// Pointer Buffer
  p = uart1_buf;											// UART Buffer
 
  do 														// Get char & Print Until null
  {
    putchar1(*p); 											// Write char to UART
    p++;													// Next char
  }
  while(*p != '\0');										// End of ASCII (null)
 
  return;
}  */