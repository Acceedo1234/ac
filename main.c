#include<LPC214X.h>
#include <stdio.h>
#include"spi.h"
#include "enc28j60.h"
#include"App_lay_cal.h"
#include"powermeter_config.h"
//RTC//

//Port 0.15 -Int

#define MASKMIN 0x3B00										// Minute 00..59   00000000:00xxxxxx:00000000
#define MASKSEC 0x3F  										// Second 00..59   00000000:00000000:00xxxxxx
#define MASKHR  0x1F0000

/*External function*/
extern void IO_Init(void);
extern void lcd_init(void);
extern void Init_VIC(void);
extern void Init_TIMER0(void);
extern void Init_TIMER1(void);
extern void Init_PLL(void);
extern void Init_UART1(void);
extern void Init_UART0(void);
extern void Send_WifiCmd(void);
extern  void lcd_init(void);
//extern void lcd_display (void);
extern void BasicDisplayRoutine(void);
extern void I2C_Write(unsigned char,unsigned char,unsigned char,unsigned int);
extern unsigned int I2C_Read(unsigned char,unsigned char,unsigned char);
extern void Oled_Displayroutine(void);
extern void Oled_DisplayInit(void);

extern unsigned char RTCReadFunction(unsigned char,unsigned char);
extern unsigned char ManuplatingRTC(unsigned char);
extern void RTCWriteFunction(unsigned char,unsigned char,unsigned char);
extern void MCP9800WriteConfig(unsigned char,unsigned char);
extern void RxDecoderPLCData(void);
static void ColdJnDegFunction(void);
extern unsigned char RTCRefreshFlag,ColdJnBit;

unsigned char toggleflag,TestMemory=0,TestMemory1=0,TestMemory2=0,TestMemory3=0,TestMemory4=0,TestMemory5=0;
unsigned char TestMemory6=0,TestMemory7=0,TestMemory8=0,TestMemory9=0,TestMemory10=0;
unsigned char SecondsDisplay,MinuteDisplay,HoursDisplay;
unsigned int ColdJncDeg;
extern unsigned char Macid_Wifi;


void delay_ms(unsigned char);
static void watchdog_timer(void);
static void RTCFunction(void);
void rtc_int(void);
void Eeprom_Intern_Delay(unsigned int x);

 void rtc_int(void)
{  
 // Initial Internal RTC Function
  // Initial RTC Function
  CCR  = 0;													// Reset All Bit Control
  CCR |= 0x00000011; 										// CLKSRC = 1 = Used EXT 32.768 KHz
	HOUR = 0x17;
	MIN  = 0x3B;
	SEC  = 0x3B;
  CCR |= 0x00000002; 										// Reset   Clock
  CCR &= 0xFFFFFFFD;										// Release Reset
  CCR |= 0x00000001;
}

int main()
{	
	//Lcd Inti 4 bit mode

	Init_PLL();				
	IO_Init();
	lcd_init();	
	Init_VIC();
	Init_TIMER0();	
	Init_TIMER1();
	Init_UART1();
	Init_UART0();
	SPIInit();
	/*Warning:Please dont touch the line*/
	/*Use in all LPC code*/
	PINSEL2 = 0xF7;	  
	Oled_DisplayInit();
//	rtc_int();

/*
	TestMemory  = I2C_Read(0xA0,0x00,0x20);	
	delay_ms(30);
	TestMemory = TestMemory+1;
	delay_ms(50);
	I2C_Write(0xA0,0x00,0x20,TestMemory);
	delay_ms(30);	 */


	
	Read_PowerOn();	
	RTCWriteFunction(0xD0,0x02,14);
	delay_ms(30);
	RTCWriteFunction(0xD0,0x01,20);
	delay_ms(30);
	RTCWriteFunction(0xD0,0x00,1);	
	Macid_Wifi=0;
		
	while(1)                   
  	{ 
			MachineLogic();
			M_AlarmFunction();
			Send_WifiCmd();	
			TransmitRequestPF();
			RTCFunction();
			watchdog_timer();
			RxDecoderPLCData();
			RXDECODERS484DATA();
			Oled_Displayroutine();
//			ENC28j60();	
	}
}	  
void delay_ms(unsigned char n)
{
	int i,j;
	for(i=0;i<=n;i++)
	{
		for(j=0;j<=10;j++)
		{}
	}
}

void Eeprom_Intern_Delay(unsigned int x)
{
	int i,j;
	for(i=0;i<=x;i++)
	{
		for(j=0;j<=6100;j++)
		{}
	}
}


static void watchdog_timer(void)
{
  static unsigned char wd_toggle=0;
  if(!wd_toggle)
  {
  	 wd_toggle=1;
	 IOSET0 = 1 << 17;
  }
  else
  {
  	 wd_toggle=0;
	 IO0CLR |= (1<<17);
  }
}

static void RTCFunction(void)
{
  static unsigned char RTCDate_M,RTCMonth_M,RTCYear_M;
  static unsigned char RTCSeconds_M,RTCMin_M,RTCHours_M;
  static unsigned char DateDisplay,MonthDisplay,YearDisplay;
  if(!RTCRefreshFlag){return;}
  RTCRefreshFlag=0;
 
 	RTCSeconds_M   =    RTCReadFunction(0xD0,0x00);
	SecondsDisplay =	ManuplatingRTC(RTCSeconds_M);
	delay_ms(10);
	RTCMin_M   	   =    RTCReadFunction(0xD0,0x01);
	MinuteDisplay  =	  ManuplatingRTC(RTCMin_M);
	delay_ms(10);
	RTCHours_M     =    RTCReadFunction(0xD0,0x02);
	HoursDisplay   =	  ManuplatingRTC(RTCHours_M);
	delay_ms(10);
    RTCDate_M     =    RTCReadFunction(0xD0,0x04);	 
	DateDisplay   =	  ManuplatingRTC(RTCDate_M);	 
	delay_ms(10);
	RTCMonth_M     =    RTCReadFunction(0xD0,0x05);
	MonthDisplay   =	  ManuplatingRTC(RTCMonth_M);
	delay_ms(10);
	RTCYear_M     =    RTCReadFunction(0xD0,0x06);
	YearDisplay   =	  ManuplatingRTC(RTCYear_M);
	delay_ms(10);
																				
}

static void ColdJnDegFunction(void)
{
   static unsigned char ColdJncOutput;
   if(!ColdJnBit){return;}
   ColdJnBit=0;

   MCP9800WriteConfig(0x90,0x00);
   delay_ms(10);
   ColdJncOutput = MCP9800ReadReg(0x90);

   ColdJncDeg	=  (unsigned int)(ColdJncOutput*0.003906);
}
