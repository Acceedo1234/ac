#include<LPC214X.h>
#include "OLED_I2C_Config.h"
#include "SharedValue.h"
#include "struct.h"
#define C_DisplayDefaultDigits_U8            0xffu // Will display the exact digits in the number


/*Global variable*/
extern unsigned int MAC_A_Temperature_1_Set,MAC_A_TemperatureHigh_1_Set,MAC_A_TemperatureLow_1_Set,MAC_A_Prod_Input1_SetTime;
extern unsigned int Production_reset;
extern unsigned int SysHysBufferPos,SysHysBufferNeg;
extern unsigned char Mac1_Prod_Time_Data[2];
extern unsigned int MAC_A_Prod_Input1_CountBase_Pos;

/*Global Function*/
void Oled_Displayroutine(void);
void Oled_DisplayInit(void);

void Oled_DisplayInit(void)
{
	OLED_Init();
}

void Oled_Displayroutine(void)
{
if(!Oled_Disflag){return;}
Oled_Disflag=0;
				   
OLED_SetCursor(0,0);
//OLED_DisplayString("Mac1:");
OLED_DisplayNumber(10,RxDataSetmac[0].production,4); 
OLED_DisplayString(":");
OLED_DisplayNumber(10,loc_rejectionCount[0].rejection,5);
OLED_SetCursor(2,0);
OLED_DisplayNumber(10,RxDataSetmac[1].production,4); 
OLED_DisplayString(":");
OLED_DisplayNumber(10,loc_rejectionCount[1].rejection,5);
OLED_SetCursor(4,0);  				
OLED_DisplayNumber(10,RxDataSetmac[2].production,4); 
OLED_DisplayString(":");
OLED_DisplayNumber(10,loc_rejectionCount[2].rejection,5);
OLED_SetCursor(6,0);  
OLED_DisplayNumber(10,RxDataSetmac[3].production,4); 
OLED_DisplayString(":");
OLED_DisplayNumber(10,loc_rejectionCount[3].rejection,5); 
OLED_SetCursor(7,0); 
OLED_DisplayString("WIFI:");
OLED_DisplayNumber(10,wifi_command,3);

//OLED_DisplayString("Acceedo");
}