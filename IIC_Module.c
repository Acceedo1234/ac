#include<LPC214X.h>
//Port 0.15 -Int
void EEPROMI2CDelay(void);
void start(void);
void stop(void);
void get_ack(void);
void send_nack(void);
unsigned char Read_Byte();
void Send_Byte(unsigned char);
void I2C_Write(unsigned char,unsigned char,unsigned char,unsigned int);
unsigned int I2C_Read(unsigned char,unsigned char,unsigned char);
void RTCWriteFunction(unsigned char,unsigned char,unsigned char);
unsigned char RTCReadFunction(unsigned char,unsigned char);
unsigned char I2Cread_byte();
unsigned char ManuplatingRTC(unsigned char);
unsigned char DecToHex(unsigned char);
unsigned char reead;
void MCP9800WriteConfig(unsigned char,unsigned char);
unsigned int MCP9800ReadReg(unsigned char);
unsigned char Max73290_IIC_Read(unsigned char Max73290_Add);
void Max73290_IIC_Write(unsigned char Max73290_Add,unsigned char PortData);


void EEPROMI2CDelay(void)
{
unsigned int i;
	//for(i=0;i<=1000;i++)
	for(i=0;i<=1000;i++)
	{
	}
}


void start(void)
{
   IO0SET |= (1<<3);//Data
   EEPROMI2CDelay();
   IO0SET |= (1<<2);//Clock
   EEPROMI2CDelay();   
   IO0CLR |= (1<<3);
   EEPROMI2CDelay();
   IO0CLR |= (1<<2);
   EEPROMI2CDelay();
}

void stop(void)
{
   IO0CLR |= (1<<3);  //Data
   IO0SET |= (1<<2);  //Clock
   EEPROMI2CDelay();
   IO0SET |= (1<<3);
   EEPROMI2CDelay(); 
   IO0CLR |= (1<<2);
}

void get_ack(void)
{
   IO0CLR |= (1<<3);//Data
   IO0SET |= (1<<2);//Clock
   EEPROMI2CDelay();
   EEPROMI2CDelay();
   IO0CLR |= (1<<2);
   EEPROMI2CDelay();
}

void send_nack(void)
{
   IO0SET |= (1<<3);
   //EEPROMI2CDelay();
   IO0SET |= (1<<2);
   EEPROMI2CDelay();
   IO0CLR |= (1<<2);
   EEPROMI2CDelay();
}

void Send_Byte(unsigned char value)	 	//send byte serially
{
	unsigned int i,j;
	unsigned char send;
	unsigned char SDA_state;
	send  =  value;
	for(i=0;i<8;i++)
	{
		SDA_state  = send/128;					//extracting MSB
		if(SDA_state){IO0SET |= (1<<3);/*IOSET0 	= 1 << 3;*/}
		else{IO0CLR |= (1<<3);/*IOCLR0   = 1 << 3;*/}
		send = send<<1;					//shiftng left
		IO0SET |= (1<<2);
		//IOSET0 	= 1 << 2;
		for(j=0;j<=100;j++)
		{
		}
		EEPROMI2CDelay();
		IO0CLR |= (1<<2);
		//IOCLR0   = 1 << 2;
		EEPROMI2CDelay();
	}
// ack=SDA;							  //reading acknowledge
   IO0CLR |= (1<<2);
   //IOCLR0   = 1 << 2;
}

unsigned char I2Cread_byte()			//reading from EEPROM serially
{
	unsigned int i;
	IO0SET |= (1<<3);
	reead=0;
	//P1_DIR=0xf6; //0xF6	   //0xc6
	IO0DIR &= ~((1<<3));
	//IO0DIR |= (0<<3);
	for(i=0;i<8;i++)
	{
		reead=reead<<1;
		//IOSET0 	= 1 << 2;
		IO0SET |= (1<<2);
		if(IOPIN0 & (1<<3)){reead++;}
		EEPROMI2CDelay();
		IO0CLR |= (1<<2);
		//IOCLR0   = 1 << 2;
		EEPROMI2CDelay();
	}
	//P1_DIR=0xfe;   //0xFE  //0xce
	IO0DIR |= (1<<3);
//	IO0CLR |= (1<<3);
	return reead;				//Returns 8 bit data here
}
void I2C_Write(unsigned char Device,unsigned char addr_h,unsigned char addr_l,unsigned int data_write)
{ 
  unsigned char Datahigh,Datalow;
  start();
  Send_Byte(Device);
  get_ack();
  EEPROMI2CDelay();

  Send_Byte(addr_h);
  get_ack();
  EEPROMI2CDelay();

  Send_Byte(addr_l);
  get_ack();
  EEPROMI2CDelay();

  Datahigh=(unsigned char)(data_write>>8)&0x00ff;
  Datalow=(unsigned char)data_write&0x00ff;

  Send_Byte(Datalow);
  get_ack();
  EEPROMI2CDelay();

  Send_Byte(Datahigh);
  get_ack();
  EEPROMI2CDelay();
  
  stop();
}

unsigned int I2C_Read(unsigned char Device,unsigned char address_H,unsigned char address_L)
{
 unsigned char read_h,read_l;
 unsigned int readword;
 start();
 Send_Byte(Device);
 get_ack();
 EEPROMI2CDelay();

 Send_Byte(address_H);
 get_ack();
 EEPROMI2CDelay();

 Send_Byte(address_L);
 get_ack();
 EEPROMI2CDelay();

 start();
 Send_Byte(Device+1);
 get_ack();
 EEPROMI2CDelay();
 EEPROMI2CDelay();
 read_l = I2Cread_byte();
 get_ack();
 EEPROMI2CDelay();

 read_h = I2Cread_byte();
 send_nack();
 EEPROMI2CDelay();

 stop();
 readword	=  (unsigned int)((read_h<<8)|read_l);
 return readword;
}

void RTCWriteFunction(unsigned char RTCDevAddress,unsigned char WAddress,unsigned char SetTime)
{
   start();
   Send_Byte(RTCDevAddress);
   get_ack();
   EEPROMI2CDelay();

   Send_Byte(WAddress);
   get_ack();
   EEPROMI2CDelay();

   Send_Byte(SetTime);
   get_ack();
   EEPROMI2CDelay();

   stop();
}

unsigned char RTCReadFunction(unsigned char RTCDevAdd,unsigned char WAddress)
{
	unsigned char RTCData;
	start();
	Send_Byte(RTCDevAdd);	
	get_ack();
	EEPROMI2CDelay();

	Send_Byte(WAddress);	
	get_ack();
	EEPROMI2CDelay();
	start();
	EEPROMI2CDelay();
	Send_Byte(RTCDevAdd+1);	
	get_ack();
	EEPROMI2CDelay();

	RTCData = I2Cread_byte();
 	send_nack();
 	EEPROMI2CDelay();
	return RTCData;
}

unsigned char ManuplatingRTC(unsigned char Time_var)
{
    unsigned char Temp1,Temp2,Temp3;
	Temp1 = (unsigned char)(((Time_var & 0xf0)>>4)*10);
	Temp2 = (Time_var&0x0f);									
	Temp3 =  Temp1+	Temp2;
	return(Temp3);
}

void MCP9800WriteConfig(unsigned char MCP9800DeviceAdd,unsigned char MCP9800ConfigReg)
{
   start();
   EEPROMI2CDelay();
   Send_Byte(MCP9800DeviceAdd);
   get_ack();
   EEPROMI2CDelay();

   Send_Byte(MCP9800ConfigReg);
   get_ack();
   EEPROMI2CDelay();

   stop();

}

unsigned int MCP9800ReadReg(unsigned char MCP9800DeviceAdd)
{
  unsigned char MSBRegdata,LSBRegData;
  unsigned int DegregOut;
  start();
  Send_Byte(MCP9800DeviceAdd+1);
  get_ack();
  EEPROMI2CDelay();

  MSBRegdata	=  I2Cread_byte();	//MSB data
  get_ack();
  EEPROMI2CDelay();

  LSBRegData	=  I2Cread_byte();	//MSB data
  send_nack();
  EEPROMI2CDelay();

  stop();

  DegregOut	= (unsigned int)((MSBRegdata<<8)|LSBRegData);

  return DegregOut;
}

unsigned char DecToHex(unsigned char DecValue)
{
	unsigned char HexValue;
	switch(DecValue)
	{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		HexValue=DecValue;
		break;
		case 10:
			HexValue=0x10;
		break;	
		case 11:
			HexValue=0x11;
		break;
		case 12:
			HexValue=0x12;
		break;
		case 13:
			HexValue=0x13;
		break;
		case 14:
			HexValue=0x14;
		break;
		case 15:
			HexValue=0x15;
		break;
		case 16:
			HexValue=0x16;
		break;
		case 17:
			HexValue=0x17;
		break;
		case 18:
			HexValue=0x18;
		break;
		case 19:
			HexValue=0x19;
		break;
		case 20:
			HexValue=0x20;
		break;
		case 21:
			HexValue=0x21;
		break;
		case 22:
			HexValue=0x22;
		break;
		case 23:
			HexValue=0x23;
		break;
	    case 24:
			HexValue=0x24;
		break;
		case 25:
			HexValue=0x25;
		break;
		case 26:
			HexValue=0x26;
		break;
		case 27:
			HexValue=0x27;
		break;
		case 28:
			HexValue=0x28;
		break;
		case 29:
			HexValue=0x29;
		break;
		case 30:
			HexValue=0x30;
		break;
		case 31:
			HexValue=0x31;
		break;
		case 32:
			HexValue=0x32;
		break;
		case 33:
			HexValue=0x33;
		break;
		case 34:
			HexValue=0x34;
		break;
		case 35:
			HexValue=0x35;
		break;
		case 36:
			HexValue=0x36;
		break;
		case 37:
			HexValue=0x37;
		break;
		case 38:
			HexValue=0x38;
		break;
		case 39:
			HexValue=0x39;
		break;
		case 40:
			HexValue=0x40;
		break;
		case 41:
			HexValue=0x41;
		break;
		case 42:
			HexValue=0x42;
		break;
		case 43:
			HexValue=0x43;
		break;
		case 44:
			HexValue=0x44;
		break;
		case 45:
			HexValue=0x45;
		break;
		case 46:
			HexValue=0x46;
		break;
		case 47:
			HexValue=0x47;
		break;
		case 48:
			HexValue=0x48;
		break;
		case 49:
			HexValue=0x49;
		break;
		case 50:
			HexValue=0x50;
		break;
		case 51:
			HexValue=0x51;
		break;
		case 52:
			HexValue=0x52;
		break;
		case 53:
			HexValue=0x53;
		break;
		case 54:
			HexValue=0x54;
		break;
		case 55:
			HexValue=0x55;
		break;
		case 56:
			HexValue=0x56;
		break;
		case 57:
			HexValue=0x57;
		break;
		case 58:
			HexValue=0x58;
		break;
		case 59:
			HexValue=0x59;
		break; 
		default:
		HexValue= DecValue;
		break;
	  }
	  return  HexValue;
}

/*I2C Port expander*/
//Port 0.15 -Int

void  Max73290_IIC_Write(unsigned char Max73290_Add,unsigned char PortData)
{
    start();
	EEPROMI2CDelay();
	Send_Byte(Max73290_Add);
	get_ack();
	EEPROMI2CDelay();
	Send_Byte(PortData);
	get_ack();
	EEPROMI2CDelay();
	Send_Byte(PortData);
	get_ack();
	EEPROMI2CDelay();
	Send_Byte(PortData);
	get_ack();
	EEPROMI2CDelay();
	Send_Byte(PortData);
	get_ack();
	EEPROMI2CDelay();
   	stop();
}


unsigned char Max73290_IIC_Read(unsigned char Max73290_Add)
{
   static unsigned char IOStatus,MSBRegdata;

	start();
	EEPROMI2CDelay();
	Send_Byte(Max73290_Add);
	get_ack();
	EEPROMI2CDelay();
	MSBRegdata = I2Cread_byte();
	EEPROMI2CDelay();
	send_nack();
	EEPROMI2CDelay();
   	stop();

   return MSBRegdata;
}










