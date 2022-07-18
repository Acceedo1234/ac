#include<LPC214X.h>
#include <stdio.h>
#include"Application_lay.h"
#include"App_lay_cal.h"
#include"struct.h"

#define ONOFF_B      1
#define LEFTARROW_B  2
#define CLEAR_B      3
#define UPARROW_B    4
#define MENU_B       5
#define DOWNARROW_B  6
#define START_B      7
#define RIGHTARROW_B 8
#define END_B 	     9
#define NOPRESS_B    255

static void Machine2InputScan();
static void Machine3InputScan();
static void Machine4InputScan();

/*void MachineLogic(void)*/
void MachineLogic(void)
{
  if(!Flag5ms_LPC)return;
  Flag5ms_LPC=0;

  MAC_A_Prod_Input1	= (IO0PIN & (1<<15));

   if((!MAC_A_Prod_Input1)&&(MAC_A_Prod_Input1_DeBounce))
   {
	  loc_rejectionCount[0].rejection = loc_rejectionCount[0].rejection+1;
	  I2C_Write(0xA0,0x00,0x01, loc_rejectionCount[0].rejection);
	  Eeprom_Intern_Delay(100);
	  MAC_A_Prod_Input1_DeBounce=0;
   }
   else if(MAC_A_Prod_Input1)
   {
   	  MAC_A_Prod_Input1_DeBounce=1;
   }

   MAC_A_Prod_Input2	= (IO0PIN & (1<<13));
   if((!MAC_A_Prod_Input2)&&(MAC_A_Prod_Input2_DeBounce))
   {
	  loc_rejectionCount[1].rejection = loc_rejectionCount[1].rejection+1;
	  I2C_Write(0xA0,0x00,0x05, loc_rejectionCount[1].rejection);
	  Eeprom_Intern_Delay(100);
	  MAC_A_Prod_Input2_DeBounce=0;
   }
   else if(MAC_A_Prod_Input2)
   {
   	  MAC_A_Prod_Input2_DeBounce=1;
   }
    MAC_A_Prod_Input3	= (IO0PIN & (1<<12));
   if((!MAC_A_Prod_Input3)&&(MAC_A_Prod_Input3_DeBounce))
   {
	  loc_rejectionCount[2].rejection = loc_rejectionCount[2].rejection+1;
	  I2C_Write(0xA0,0x00,0x0a, loc_rejectionCount[2].rejection);
	  Eeprom_Intern_Delay(100);
	  MAC_A_Prod_Input3_DeBounce=0;
   }
   else if(MAC_A_Prod_Input3)
   {
   	  MAC_A_Prod_Input3_DeBounce=1;
   }
    MAC_A_Prod_Input4	= (IO0PIN & (1<<11));
   if((!MAC_A_Prod_Input4)&&(MAC_A_Prod_Input4_DeBounce))
   {
	  loc_rejectionCount[3].rejection = loc_rejectionCount[3].rejection+1;
	  I2C_Write(0xA0,0x00,0x0f, loc_rejectionCount[3].rejection);
	  Eeprom_Intern_Delay(100);
	  MAC_A_Prod_Input4_DeBounce=0;
   }
   else if(MAC_A_Prod_Input4)
   {
   	  MAC_A_Prod_Input4_DeBounce=1;
   }
 
}
 void M_AlarmFunction()
{
   if(!Alarmfunction)return;
   Alarmfunction=0;

   if(( RxWifi_TxDataSetMac[0].rightDiehighTemp < RxDataSetmac[0].temperature1_act)|| (RxWifi_TxDataSetMac[0].rightDieLowTemp > RxDataSetmac[0].temperature1_act)
   ||(RxWifi_TxDataSetMac[0].leftDiehighTemp  < RxDataSetmac[0].temperature2_act) || (RxWifi_TxDataSetMac[0].leftDieLowTemp > RxDataSetmac[0].temperature2_act))
   {
   	 //Temperture not OK
	 IO0SET |= (1<<18);
   }
   else
   {
	 IO0CLR |= (1<<18);
   }
   
   if(( RxWifi_TxDataSetMac[1].rightDiehighTemp < RxDataSetmac[1].temperature1_act)|| (RxWifi_TxDataSetMac[1].rightDieLowTemp > RxDataSetmac[1].temperature1_act)
   ||(RxWifi_TxDataSetMac[1].leftDiehighTemp  < RxDataSetmac[1].temperature2_act) || (RxWifi_TxDataSetMac[1].leftDieLowTemp > RxDataSetmac[1].temperature2_act))
   {
   	  //Temperture not OK
	  IO0SET |= (1<<19);
   }
   else
   {
	  IO0CLR |= (1<<19);
   }

   if(( RxWifi_TxDataSetMac[2].rightDiehighTemp < RxDataSetmac[2].temperature1_act)|| (RxWifi_TxDataSetMac[2].rightDieLowTemp > RxDataSetmac[2].temperature1_act)
   ||(RxWifi_TxDataSetMac[2].leftDiehighTemp  < RxDataSetmac[2].temperature2_act) || (RxWifi_TxDataSetMac[2].leftDieLowTemp > RxDataSetmac[2].temperature2_act))
   {
   	  //Temperture not OK
	  IO1SET |= (1<<24);
   }
   else
   {
	  IO1CLR |= (1<<24);
   }

   if(( RxWifi_TxDataSetMac[3].rightDiehighTemp < RxDataSetmac[3].temperature1_act)|| (RxWifi_TxDataSetMac[3].rightDieLowTemp > RxDataSetmac[3].temperature1_act)
   ||(RxWifi_TxDataSetMac[3].leftDiehighTemp  < RxDataSetmac[3].temperature2_act) || (RxWifi_TxDataSetMac[3].leftDieLowTemp > RxDataSetmac[3].temperature2_act))
   {
   	   //Temperture not OK
	   IO1SET |= (1<<25);
   }
   else
   {
	   IO1CLR |= (1<<25);
   }

}



static void Machine4InputScan()
{
 
}
static void Machine3InputScan()
{
 
}
static void Machine2InputScan()
{

}


void Read_PowerOn(void)
{
   I2C_Write(0xA0,0x00,0x01, 0);
	  Eeprom_Intern_Delay(100);
	  I2C_Write(0xA0,0x00,0x05, 0);
	  Eeprom_Intern_Delay(100);
	  I2C_Write(0xA0,0x00,0x0a, 0);
	  Eeprom_Intern_Delay(100);
	  I2C_Write(0xA0,0x00,0x0f, 0);
	  Eeprom_Intern_Delay(100);
 
 	 loc_rejectionCount[0].rejection 	= I2C_Read(0xA0,0x00,0x01);	
    delay_ms(30);
	 loc_rejectionCount[1].rejection   	= I2C_Read(0xA0,0x00,0x05);
    delay_ms(30);
   	 loc_rejectionCount[2].rejection	= I2C_Read(0xA0,0x00,0x0a);
	delay_ms(30);
	 loc_rejectionCount[3].rejection 	= I2C_Read(0xA0,0x00,0x0f);
	delay_ms(30);	

}

unsigned char KeyscanRoutine(void)
{
	
	/*First three pin output, second three as input*/
	Max73290_IIC_Write(0x74,0x3b);
	delay_ms(20);
	Mac73290_In_Data =  Max73290_IIC_Read(0x75); //0x3A	//0x39
	if(Mac73290_In_Data == 27){return 1;}
	if(Mac73290_In_Data == 43){return 4;}
	if(Mac73290_In_Data == 51){return 7;}
	delay_ms(20);	 

	Max73290_IIC_Write(0x74,0x3d);
	delay_ms(20);
	Mac73290_In_Data =  Max73290_IIC_Read(0x75); //0x3A	//0x39
	delay_ms(20);
	if(Mac73290_In_Data == 29 ){return 2;}
	if(Mac73290_In_Data == 45){return 5;}
	if(Mac73290_In_Data == 53){return 8;}	
	  
	Max73290_IIC_Write(0x74,0x3e);
	delay_ms(20);
	Mac73290_In_Data =  Max73290_IIC_Read(0x75); //0x3A	//0x39
	delay_ms(20);
	if(Mac73290_In_Data == 30){return 3;}
	if(Mac73290_In_Data == 46){return 6;}
	if(Mac73290_In_Data == 54){return 9;}
				 
	return 0xff;
}

void KeyscanRoutine_Change(void)
{
	if(!Flag10ms_Keyscan){return;}
	Flag10ms_Keyscan=0;

	Keypressvalue = KeyscanRoutine();

	switch(Keypressvalue)
	{
	  case ONOFF_B:

	  break;
	  case LEFTARROW_B:

	  break;
	  case CLEAR_B:
		   if(!ClearDebounce)
		   {
		   	  ClearDebounce=1;
			  MAC_Gen_Prod_Input1_Production=0;
			  I2C_Write(0xA0,0x00,0x01, MAC_Gen_Prod_Input1_Production);
			  Eeprom_Intern_Delay(100);
			  MAC_Gen_Prod_Input2_Production=0;
			  I2C_Write(0xA0,0x00,0x10, MAC_Gen_Prod_Input2_Production);
			  Eeprom_Intern_Delay(100);
			  MAC_Gen_Prod_Input3_Production=0;
			  I2C_Write(0xA0,0x00,0x15, MAC_Gen_Prod_Input3_Production);
			  Eeprom_Intern_Delay(100);
			  MAC_Gen_Prod_Input4_Production=0;
			  I2C_Write(0xA0,0x00,0x25, MAC_Gen_Prod_Input4_Production);
			  Eeprom_Intern_Delay(100);
			  MAC_Gen_Rej_Input_Production[0]=0;
			  I2C_Write(0xA0,0x00,0x05, MAC_Gen_Rej_Input_Production[0]);
		  	  Eeprom_Intern_Delay(100);
			   MAC_Gen_Rej_Input_Production[1]=0;
			  I2C_Write(0xA0,0x00,0x0B, MAC_Gen_Rej_Input_Production[1]);
		  	  Eeprom_Intern_Delay(100);
			   MAC_Gen_Rej_Input_Production[2]=0;
			  I2C_Write(0xA0,0x00,0x1B, MAC_Gen_Rej_Input_Production[2]);
		  	  Eeprom_Intern_Delay(100);
			   MAC_Gen_Rej_Input_Production[3]=0;
			  I2C_Write(0xA0,0x00,0x2B, MAC_Gen_Rej_Input_Production[3]);
		  	  Eeprom_Intern_Delay(100);

		   }
	  break;
	  case UPARROW_B:

	  break;
	  case MENU_B:
		   if(!MenuDebounce)
		   {
		   	MenuDebounce=1;
			if(++screennumber>11){screennumber=0;}

		   }
	  break;
	  case DOWNARROW_B:

	  break;
	  case START_B:

	  break;
	  case RIGHTARROW_B:

	  break;
	  case END_B:

	  break;
	  case NOPRESS_B:
		  MenuDebounce	= 0;
		  ClearDebounce = 0;
	  break;

	  default:
	  break;
	}
}