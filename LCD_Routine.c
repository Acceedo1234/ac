#include<LPC214X.h>
#include "SharedValue.h"
 

unsigned char screennumber=1;
extern unsigned char Flag5ms;
extern void goto_cursor(unsigned char);
extern void lcd_data (unsigned char);
extern void LCD_Valuedisplay(unsigned int value_display);
extern void LCD_Valuedisplay_2(unsigned int value_display);
extern void LCD_Valuedisplay_5(unsigned int value_display);

extern unsigned char wifi_command;
extern unsigned char TestMemory,TestMemory1,TestMemory2,TestMemory3,TestMemory4,TestMemory5;
extern unsigned char TestMemory,TestMemory6,TestMemory7,TestMemory8,TestMemory9,TestMemory10;
extern unsigned int MAC_Gen_Prod_Input1_Production,MAC_Gen_Prod_Input2_Production,MAC_Gen_Prod_Input3_Production,MAC_Gen_Prod_Input4_Production;

extern unsigned char MAC_A_Prod_Input1_FallingEdge;
extern unsigned char MAC_A_Prod_Input1_RisingEdge,MAC_A_Prod_Input2_RisingEdge,MAC_A_Prod_Input3_RisingEdge,MAC_A_Prod_Input4_RisingEdge;

extern unsigned int ColdJncDeg;
extern unsigned int MAC_Gen_Rej_Input_Production[5];
extern unsigned int MAC_A_Temperature_1_Set,MAC_A_Temperature_2_Set,MAC_A_Temperature_3_Set,MAC_A_Temperature_4_Set;
extern unsigned int MAC_A_TemperatureHigh_1_Set,MAC_A_TemperatureHigh_2_Set,MAC_A_TemperatureHigh_3_Set,MAC_A_TemperatureHigh_4_Set;
extern unsigned int MAC_A_TemperatureLow_1_Set,MAC_A_TemperatureLow_2_Set,MAC_A_TemperatureLow_3_Set,MAC_A_TemperatureLow_4_Set;

void BasicDisplayRoutine(void);

void BasicDisplayRoutine(void)
{
unsigned char LineRef;
if(!Flag5ms){return;}
Flag5ms=0;

for(LineRef=0;LineRef<4;LineRef++)
{
  switch(LineRef)
  {
   	case 0:	 //Line1
	 goto_cursor(0x80);
	 switch(screennumber)
	 {
	 	case 0:
			lcd_print("          ");
			lcd_print("          ");
		break;
		case 1:
			lcd_print("Production:   ");
			LCD_Valuedisplay_5(MAC_Gen_Prod_Input1_Production);
			//lcd_print("  ");
		break;
		case 2:
			lcd_print("Production:   ");
			LCD_Valuedisplay_5(MAC_Gen_Prod_Input2_Production);
		break;
		case 3:
			lcd_print("Production:   ");
			LCD_Valuedisplay_5(MAC_Gen_Prod_Input3_Production);
		break;
		case 4:
			lcd_print("Production:   ");
			LCD_Valuedisplay_5(MAC_Gen_Prod_Input4_Production);
		break;
		case 5:
			lcd_print("Status");
			lcd_print("              ");
		break;
		case 6:
			lcd_print("RisingEdge2:  ");
			LCD_Valuedisplay_5(MAC_A_Prod_Input2_RisingEdge);
		break;
		case 7:
			lcd_print("Mac1 Temp  :  ");
			LCD_Valuedisplay_5(MAC_A_Temperature_1_Set);
		break;
	    case 8:
			lcd_print("Mac1 TempHi:  ");
			LCD_Valuedisplay_5(MAC_A_TemperatureHigh_1_Set);
		break;
		case 9:
			lcd_print("Mac1 TempLo:  ");
			LCD_Valuedisplay_5(MAC_A_TemperatureLow_1_Set);
		break;
		case 10:
			lcd_print("ResetStatus:  ");
			LCD_Valuedisplay_5(ResetStatus_1);
		break;
		case 11:
			lcd_print("OnlineTime>");
			lcd_print("         ");
		break;
	 }
	break;
	case 1:	 //Line2
	 goto_cursor(0xC0);
	 switch(screennumber)
	 {
	 	case 0:
			lcd_print("   LPDC Machine");
			lcd_print("     ");
		break;
		case 1:
			lcd_print("SetTime  9:   ");
			LCD_Valuedisplay_5(MAC_A_Prod_Input1_SetTime);
		break;
		case 2:
			lcd_print("SetTime 10:   ");
			LCD_Valuedisplay_5(MAC_A_Prod_Input2_SetTime);
		break;
		case 3:
			lcd_print("SetTime 11:   ");
			LCD_Valuedisplay_5(MAC_A_Prod_Input3_SetTime);
		break;
		case 4:
			lcd_print("SetTime 12:   ");
			LCD_Valuedisplay_5(MAC_A_Prod_Input4_SetTime);
		break;
		case 5:
			lcd_print("RisingEdge :  ");
			LCD_Valuedisplay_5(MAC_A_Prod_Input1_RisingEdge);
		break;
		case 6:
			lcd_print("RisingEdge3:  ");
			LCD_Valuedisplay_5(MAC_A_Prod_Input3_RisingEdge);
		break;
		case 7:
			lcd_print("Mac2 Temp  :  ");
			LCD_Valuedisplay_5(MAC_A_Temperature_2_Set);
		break;
		case 8:
			lcd_print("Mac2 TempHi:  ");
			LCD_Valuedisplay_5(MAC_A_TemperatureHigh_2_Set);
		break;
		case 9:
			lcd_print("Mac2 TempLo:  ");
			LCD_Valuedisplay_5(MAC_A_TemperatureLow_2_Set);
		break;
		case 10:
			lcd_print("ResetStatus:  ");
			LCD_Valuedisplay_5(ResetStatus_2);
		break;
		case 11:
			LCD_Valuedisplay_5(CurrentTime_Hr);
			lcd_print(":");
			LCD_Valuedisplay_5(CurrentTime_Min);
			lcd_print("        ");
		break;
		
	 }
	break;
	case 2:	 //Line3
	 goto_cursor(0x94);
	 switch(screennumber)
	 {
	 	case 0:
			lcd_print("    UniqueShell");
		    lcd_print("     ");
		break;
		case 1:
			lcd_print("Timer Base:1  ");
			LCD_Valuedisplay(MAC_A_Prod_Input1_CountBase);
		break;
		case 2:
			lcd_print("Timer Base2:  ");
			LCD_Valuedisplay(MAC_A_Prod_Input2_CountBase);
		break;
		case 3:
			lcd_print("Timer Base3:  ");
			LCD_Valuedisplay(MAC_A_Prod_Input3_CountBase);
		break;
		case 4:
			lcd_print("Timer Base4:  ");
			LCD_Valuedisplay(MAC_A_Prod_Input4_CountBase);
		break;
		case 5:
			lcd_print("FallingEdge:  ");
			LCD_Valuedisplay_5(MAC_A_Prod_Input1_RisingEdge);
		break;
		case 6:
			lcd_print("RisingEdge4:  ");
			LCD_Valuedisplay_5(MAC_A_Prod_Input4_RisingEdge);
			lcd_print(" ");
		break;
		case 7:
			lcd_print("Mac3 Temp  :  ");
			LCD_Valuedisplay_5(MAC_A_Temperature_3_Set);
		break;
		case 8:
			lcd_print("Mac3 TempHi:  ");
			LCD_Valuedisplay_5(MAC_A_TemperatureHigh_3_Set);
		break;
		case 9:
			lcd_print("Mac3 TempLo:  ");
			LCD_Valuedisplay_5(MAC_A_TemperatureLow_3_Set);
		break;
		case 10:
			lcd_print("ResetStatus:  ");
			LCD_Valuedisplay_5(ResetStatus_3);
		break;
		case 11:
			lcd_print("RTC       >");
			lcd_print("         ");
		break;
	 }
	break;
	case 3:	 //Line4
	 goto_cursor(0xd4);
	 switch(screennumber)
	 {
	 	case 0:
			lcd_print("   Acceedo  v2.0");
			lcd_print("     ");
		break;
		case 1:
			lcd_print("Rejection :   ");
			LCD_Valuedisplay_5(MAC_Gen_Rej_Input_Production[0]);
			lcd_print(" ");
		break;
		case 2:
			lcd_print("Rejection :   ");
			LCD_Valuedisplay_5(MAC_Gen_Rej_Input_Production[1]);
			lcd_print(" ");
		break;
		case 3:
			lcd_print("Rejection :   ");
			LCD_Valuedisplay_5(MAC_Gen_Rej_Input_Production[2]);
			lcd_print(" ");
		break;
		case 4:
			lcd_print("Rejection :   ");
			LCD_Valuedisplay_5(MAC_Gen_Rej_Input_Production[3]);
			lcd_print(" ");
		break;
		case 5:
			if(WifiDisplay == 0x01)     {lcd_print("Wifi Ok    ");}
			else if(WifiDisplay == 0x02){lcd_print("Server Ok  ");}
			else if(WifiDisplay == 0x03){lcd_print("Wifi NOK   ");}
			else if(WifiDisplay == 0x04){lcd_print("Server NOK ");}
			else						{lcd_print("NO Conn... ");}
			lcd_print("Wifi:");
			LCD_Valuedisplay(wifi_command);
		break;
		case 6:
			lcd_print("RisingEdge2:  ");
			LCD_Valuedisplay_5(MAC_A_Prod_Input2_RisingEdge);
			lcd_print(" ");
		break;
		case 7:
			lcd_print("Mac4 Temp  :  ");
			LCD_Valuedisplay_5(MAC_A_Temperature_4_Set);
		break;
		case 8:
			lcd_print("Mac4 TempHi:  ");
			LCD_Valuedisplay_5(MAC_A_TemperatureHigh_4_Set);
		break;
		case 9:
			lcd_print("Mac4 TempLo:  ");
			LCD_Valuedisplay_5(MAC_A_TemperatureLow_4_Set);
		break;
		case 10:
			lcd_print("ResetStatus:  ");
			LCD_Valuedisplay_5(ResetStatus_4);
		break;		
		case 11:
			LCD_Valuedisplay_5(HoursDisplay);
			lcd_print(":");
			LCD_Valuedisplay_5(MinuteDisplay);
			lcd_print(":");
			LCD_Valuedisplay_5(SecondsDisplay);
		break;
	 }
	break;
  }
}
}		