unsigned int MAC_A_Prod_Input1,MAC_A_Prod_Input2,MAC_A_Prod_Input3,MAC_A_Prod_Input4;
unsigned char MAC_A_Prod_Input1_DeBounce,MAC_A_Prod_Input2_DeBounce,MAC_A_Prod_Input3_DeBounce,MAC_A_Prod_Input4_DeBounce;
unsigned char MAC_A_Prod_Input1_RisingEdge,MAC_A_Prod_Input2_RisingEdge,MAC_A_Prod_Input3_RisingEdge,MAC_A_Prod_Input4_RisingEdge;
unsigned int MAC_A_Prod_Input1_CountBase,MAC_A_Prod_Input2_CountBase,MAC_A_Prod_Input3_CountBase,MAC_A_Prod_Input4_CountBase;
unsigned int MAC_A_Prod_Input1_CountBase_Pos;
unsigned char MAC_A_Prod_Input1_StartTimer,MAC_A_Prod_Input2_StartTimer,MAC_A_Prod_Input3_StartTimer,MAC_A_Prod_Input4_StartTimer;
unsigned char MAC_A_Prod_Input1_FallingEdge,MAC_A_Prod_Input2_FallingEdge,MAC_A_Prod_Input3_FallingEdge,MAC_A_Prod_Input4_FallingEdge;
unsigned char Chanceofrejection;
unsigned char MAC_A_Prod_Input1_ProdComplete,MAC_A_Prod_Input2_ProdComplete,MAC_A_Prod_Input3_ProdComplete,MAC_A_Prod_Input4_ProdComplete;
unsigned char MAC_A_Prod_Input1_StartTimer;
unsigned char Mac73290_In_Data;
unsigned char Keypressvalue;
unsigned char MenuDebounce,ClearDebounce;
unsigned char Mac1_Prod_Time_Data[2],Mac1_Prod_Time_Data[2],Mac1_Prod_Time_Data[2],Mac1_Prod_Time_Data[2];
extern unsigned char FreshStartPod;
extern unsigned char ThresholdexceedRej;

unsigned int MAC_Gen_Prod_Input1_Production,MAC_Gen_Prod_Input2_Production,MAC_Gen_Prod_Input3_Production,MAC_Gen_Prod_Input4_Production;
unsigned int MAC_Gen_Rej_Input_Production[5];
unsigned char Mac1_Prod_Time_Data[2],Mac2_Prod_Time_Data[2],Mac3_Prod_Time_Data[2],Mac4_Prod_Time_Data[2];


extern void delay_ms(unsigned char);
extern void I2C_Write(unsigned char,unsigned char,unsigned char,unsigned int);
extern unsigned int I2C_Read(unsigned char,unsigned char,unsigned char);
extern unsigned char Max73290_IIC_Read(unsigned char Max73290_Add);
extern void Max73290_IIC_Write(unsigned char Max73290_Add,unsigned char PortData);
extern void Eeprom_Intern_Delay(unsigned int x);

extern unsigned char Flag5ms_LPC,Flag10ms_Keyscan;
extern unsigned char TestMemory,TestMemory1,TestMemory2,TestMemory3,TestMemory4,TestMemory5;
extern unsigned char TestMemory,TestMemory6,TestMemory7,TestMemory8,TestMemory9,TestMemory10;
extern unsigned char screennumber;
extern unsigned char SecondsDisplay,MinuteDisplay,HoursDisplay;
extern unsigned char Alarmfunction;

extern unsigned int MAC_A_Prod_Input1_SetTime,MAC_A_Prod_Input2_SetTime,MAC_A_Prod_Input3_SetTime,MAC_A_Prod_Input4_SetTime;
extern unsigned int MAC_A_Temperature_1_Set,MAC_A_Temperature_2_Set,MAC_A_Temperature_3_Set,MAC_A_Temperature_4_Set;
extern unsigned int MAC_A_TemperatureHigh_1_Set,MAC_A_TemperatureHigh_2_Set,MAC_A_TemperatureHigh_3_Set,MAC_A_TemperatureHigh_4_Set;
extern unsigned int MAC_A_TemperatureLow_1_Set,MAC_A_TemperatureLow_2_Set,MAC_A_TemperatureLow_3_Set,MAC_A_TemperatureLow_4_Set;
extern unsigned int SysHysBufferPos,SysHysBufferNeg;
extern unsigned int CurrentTime_Min,CurrentTime_Hr;

