#include<LPC214X.h>


#define MAC_A_Prod_Input1_SetTime_cons 20

#define RDR        0x01
#define THRE    0x20
#define RDA        0x04

void Init_VIC(void);
void Timer0ISR(void)__irq;
void Timer1ISR(void)__irq;
void UART0ISR(void)__irq;
void UART1ISR(void)__irq;
void Exint0ISR(void)__irq;
void RTC_ISR(void)__irq;
extern unsigned char toggleflag,Rxseqdecoder;
extern unsigned char TestMemory,TestMemory1,TestMemory2,TestMemory3,TestMemory4,TestMemory5;
extern unsigned char TestMemory,TestMemory6,TestMemory7,TestMemory8,TestMemory9,TestMemory10;
extern unsigned int MAC_A_Prod_Input1,MAC_A_Prod_Input2,MAC_A_Prod_Input3,MAC_A_Prod_Input4;
extern unsigned char MAC_A_Prod_Input1_DeBounce,MAC_A_Prod_Input2_DeBounce,MAC_A_Prod_Input3_DeBounce,MAC_A_Prod_Input4_DeBounce;
extern unsigned char MAC_A_Prod_Input1_RisingEdge,MAC_A_Prod_Input2_RisingEdge,MAC_A_Prod_Input3_RisingEdge,MAC_A_Prod_Input4_RisingEdge;
extern unsigned int MAC_A_Prod_Input1_CountBase,MAC_A_Prod_Input2_CountBase,MAC_A_Prod_Input3_CountBase,MAC_A_Prod_Input4_CountBase;
extern unsigned int MAC_A_Prod_Input1_CountBase_Pos;
extern unsigned char MAC_A_Prod_Input1_StartTimer,MAC_A_Prod_Input2_StartTimer,MAC_A_Prod_Input3_StartTimer,MAC_A_Prod_Input4_StartTimer;
extern unsigned char MAC_A_Prod_Input1_FallingEdge,MAC_A_Prod_Input2_FallingEdge,MAC_A_Prod_Input3_FallingEdge,MAC_A_Prod_Input4_FallingEdge;
extern unsigned char Chanceofrejection;
extern unsigned char MAC_A_Prod_Input1_ProdComplete,MAC_A_Prod_Input2_ProdComplete,MAC_A_Prod_Input3_ProdComplete,MAC_A_Prod_Input4_ProdComplete;
extern unsigned char MAC_A_Prod_Input1_StartTimer;

extern unsigned int MAC_A_Prod_Input1_SetTime,MAC_A_Prod_Input2_SetTime,MAC_A_Prod_Input3_SetTime,MAC_A_Prod_Input4_SetTime;
extern unsigned int SysHysBufferPos,SysHysBufferNeg;


unsigned char Flag5ms,Flag5ms_LPC,CountATimer1,Wifirefreshflag_Lpcs,PowerMeterRefresh_Lpc,CountATimer0,Flag10ms_Keyscan;
unsigned char rxdat_1,rxdat_2,checkrx;
unsigned char RTCRefreshFlag,ColdJnBit;
unsigned char SetFlagSPI;
unsigned char Oled_Disflag;	
unsigned char Alarmfunction;

unsigned char FreshStartPod;
unsigned char ThresholdexceedRej;	 

extern unsigned int I2C_Read(unsigned char,unsigned char,unsigned char);


void Init_VIC(void)
{
    VICIntSelect = 0x00000000;          //select timer0 interrupt ad IRQ i.e bit 4=0
    //VICDefVectAddr  = (unsigned long int)timer0ISR; do not use when more than one interrupts are used
    VICVectAddr0 = (unsigned long int)Timer0ISR;
    VICVectCntl0 = 0x24;                 //bit 5 is enabled the slot to produce unique interrupt address for enabled timer0 interrupt
    VICIntEnable = 0x00000010;          //enable timer0 interrupt

    //VICDefVectAddr  = (unsigned long int)timer0ISR; do not use when more than one interrupts are used
    VICVectAddr1 = (unsigned long int)Timer1ISR;
    VICVectCntl1 = 0x25;                 //bit 5 is enabled the slot to produce unique interrupt address for enabled timer0 interrupt
    VICIntEnable |= 0x00000020;          //enable timer0 interrupt

    VICVectCntl2 = 0x26;
    VICVectAddr2 = (unsigned long int)UART0ISR;
    VICIntEnable |= 0x00000040;          //enable UART0 interrupt


    VICVectAddr3 = (unsigned long int)UART1ISR;
    VICVectCntl3 = 0x27;       
    VICIntEnable |= 0x00000080;

	VICVectAddr4 = (unsigned long int) RTC_ISR;
	VICVectCntl4 = 0x0000002D;
	VICIntEnable = 0x00002000;


    VICIntEnable |= 0x00004000;          //enable EINT0 interrupt
    VICIntSelect = 0x00000001;          //select EINT0 interrupt as IRQ i.e bit 14=0
    VICVectAddr4 = (unsigned long int)Exint0ISR;
    VICVectCntl4 = 0x2e;
}

void Timer0ISR(void)__irq
{
   	static unsigned char CountTimer10ms;
    T0IR = 0xff; /*Interrupt for timer*/
	Flag5ms	=1;
	Flag5ms_LPC=1;

	if(++CountTimer10ms >=2)
	{
	   CountTimer10ms=0;
	   Flag10ms_Keyscan =1;
	}

	if(++CountATimer0 >= 200)
	{
	CountATimer0=0;
//	IO0SET |= (1<<25);
	}
	if (CountATimer0==150)
	{
//	IO0CLR |= (1<<25);
	}
    VICVectAddr = 0x00;

}


void Timer1ISR(void)__irq
{
    unsigned int ThresholdMacAProdTime,ThresholdMacAProdTimePos;
	T1IR = 0xff;
   	SetFlagSPI =1;
	Alarmfunction=1;
	if(MAC_A_Prod_Input1_StartTimer)
	{
	 	 ++MAC_A_Prod_Input1_CountBase;	 //replace MAC_A_Prod_Input1_SetTime_cons with MAC_A_Prod_Input1_SetTime
		 
		 //((MAC_A_Prod_Input1_CountBase  >= MAC_A_Prod_Input1_SetTime_cons*10))//	||
		 //if((MAC_A_Prod_Input1_CountBase  >= (MAC_A_Prod_Input1_SetTime)*10))
		 ThresholdMacAProdTime = ((MAC_A_Prod_Input1_SetTime*10)-(SysHysBufferNeg*10));
		 if(MAC_A_Prod_Input1_CountBase  >= ThresholdMacAProdTime)
		 {
		 	 MAC_A_Prod_Input1_CountBase	= ThresholdMacAProdTime;
			 MAC_A_Prod_Input1_ProdComplete = 1;
			 MAC_A_Prod_Input1_StartTimer	= 0;
			 Chanceofrejection				= 0;
		 }
		 else
		 {
		 	 Chanceofrejection	= 0;
		 }			
	}

	 ThresholdMacAProdTimePos =	((MAC_A_Prod_Input1_SetTime*10)+(SysHysBufferPos*10));
	 if(FreshStartPod)
	 {
	 ++MAC_A_Prod_Input1_CountBase_Pos;
		 if(MAC_A_Prod_Input1_CountBase_Pos > ThresholdMacAProdTimePos)
		 {
		 	 ThresholdexceedRej =1;
			 FreshStartPod=0;
			 MAC_A_Prod_Input1_CountBase_Pos = ThresholdMacAProdTimePos;
		 }
	 }


	if(++CountATimer1 >= 10)
	{
		CountATimer1=0;
		Wifirefreshflag_Lpcs = 1;
		toggleflag = ~toggleflag;
		RTCRefreshFlag=1;
		ColdJnBit=1;
		Oled_Disflag=1;		 	 
	}	    

    VICVectAddr = 0x00;
}

void Exint0ISR(void)__irq
{
    EXTINT = 0x01;    
       
    VICVectAddr = 0x00;
}

void UART0ISR(void)__irq
{
    if(U0IIR & RDA)
       rxdat_2 = U0RBR;

	++checkrx;
    Recive485_service(rxdat_2);
	
	VICVectAddr = 0x00;
}
                                
void UART1ISR(void)__irq
{        
    if(U1IIR & RDA)
        rxdat_1 = U1RBR;
   
   	ESPRxDecoder(rxdat_1,Rxseqdecoder);
    VICVectAddr = 0x00;
}

void RTC_ISR(void)__irq
{
if (ILR & 0x01)
	{
	 ILR = ILR | 0x01;

	}
	if (ILR & 0x02)
	{
		ILR = ILR | 0x02;
	}
//	VICVectAddr = 0;
 //  VICVectAddr = 0x00;

}