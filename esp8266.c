#include<LPC214X.h>
#include "stdio.h"
#include "struct.h"

void Send_WifiCmd(void);
void ESPRxDecoder(unsigned char Rxwifi_data,unsigned char Rxseqdecoder);
void RxDecoderPLCData(void);
unsigned char DecToASCIIFun(unsigned char TempVar);

unsigned char Valid_DataWifi1;
unsigned char Entry_Response;
unsigned char RxCompleteU2C1WIFI;
unsigned char ESP8266TXData[250];
unsigned char Uart_rx_buffer[70];
unsigned char wifi_command,Timerdelay;
unsigned char Check_CWMODE_For_Hang;
unsigned char Error_Close,WifiDisplay;
unsigned char Rxseqdecoder;
unsigned int bufferptr;
unsigned int Data_bufferptr;
unsigned int url_buffer =0,NoOfdata_byte;
unsigned int url_send =203;
unsigned int MAC_A_Prod_Input1_SetTime,MAC_A_Prod_Input2_SetTime,MAC_A_Prod_Input3_SetTime,MAC_A_Prod_Input4_SetTime;
unsigned int MAC_A_Temperature_1_Set,MAC_A_Temperature_2_Set,MAC_A_Temperature_3_Set,MAC_A_Temperature_4_Set;
unsigned int MAC_A_TemperatureHigh_1_Set,MAC_A_TemperatureHigh_2_Set,MAC_A_TemperatureHigh_3_Set,MAC_A_TemperatureHigh_4_Set;
unsigned int MAC_A_TemperatureLow_1_Set,MAC_A_TemperatureLow_2_Set,MAC_A_TemperatureLow_3_Set,MAC_A_TemperatureLow_4_Set;
unsigned int ResetStatus_1,ResetStatus_2,ResetStatus_3,ResetStatus_4;
unsigned int TemperatureReference;
unsigned int ReceivePLCData14;
unsigned char Macid_Wifi;

unsigned int Err_bufferptr;
extern unsigned char RecvBufferPW[50],TestMemory;
extern unsigned char Mac1_Prod_Time_Data[2],Mac2_Prod_Time_Data[2],Mac3_Prod_Time_Data[2],Mac4_Prod_Time_Data[2];
extern unsigned char HoursDisplay,MinuteDisplay;


extern unsigned int MAC_Gen_Prod_Input1_Production,MAC_Gen_Prod_Input2_Production,MAC_Gen_Prod_Input3_Production,MAC_Gen_Prod_Input4_Production;
extern unsigned int MAC_Gen_Rej_Input_Production[5];
extern unsigned int Temperature_Cnt_1,Temperature_Cnt_2,Temperature_Cnt_3,Temperature_Cnt_4;
extern unsigned char Wifirefreshflag_Lpcs;
extern void LPC_ASC_vSendData (int ch);
extern void Eeprom_Intern_Delay(unsigned int x);
extern void RTCWriteFunction(unsigned char,unsigned char,unsigned char);
extern unsigned char ManuplatingRTC(unsigned char);

unsigned char HexToDecFun(unsigned int TempVar);

unsigned char MachineId=31;
unsigned int current_Mac_id;
unsigned int MAC_Gen_Prod_Send,MAC_Gen_Rej_Send,Temperature_Send;
unsigned char Prod_time_Hr,Prod_time_Min;
unsigned int CurrentTime_Hr,CurrentTime_Min;
unsigned int Production_reset,EntryStatus;
unsigned int SysHysBufferPos,SysHysBufferNeg;
unsigned int rightDieSetTemp[4],rightDiehighTemp[4],rightDieLowTemp[4],leftDieSetTemp[4],leftDieSetTemp[4];
unsigned int leftDiehighTemp[4],leftDieLowTemp[4],curingTemp[4],tiltingUpTime[4],slideCoreTime[4],ejectionOnTime[4],ejectionOffTime[4];
struct  machinestatus MacSpecStatus_Rst[4];
struct  loc_rejection  loc_rejectionCount[4];
unsigned int AckMacstatus[4];
/*ESP8266 Transmit function*/
void Send_WifiCmd(void)
{
	unsigned char i;
	unsigned int x,y;
	
	if(!Wifirefreshflag_Lpcs){return;}
	Wifirefreshflag_Lpcs=0;
	
	switch(wifi_command)
 	{
 	case 5:
		IO1SET |= (1<<25);	
		for(x=0;x<=2000;x++)
		{
		   for(y=0;y<=200;y++);
		}
		IO1CLR |= (1<<25);
		wifi_command=13;
		
	break; 
 	case 10://reset
	ESP8266TXData[0] = 'A';
	ESP8266TXData[1] = 'T';
	ESP8266TXData[2] = '+';
	ESP8266TXData[3] = 'R';
	ESP8266TXData[4] = 'S';
	ESP8266TXData[5] = 'T';
	ESP8266TXData[6] = 0x0d;
	ESP8266TXData[7] = 0x0a;
	NoOfdata_byte=7;
	wifi_command=13;  
	Timerdelay=0;
	bufferptr=0;
	for(i=0;i<8;i++)
	{
		  	 LPC_ASC_vSendData(ESP8266TXData[i]);
			 //Delay_Mc_Ms(10);
	}
	break;
	case 11:
	if(++Timerdelay >6)
	{
		Timerdelay=0;
		wifi_command=14;
	}
	break;
	case 12:
	ESP8266TXData[0] = 'A';
	ESP8266TXData[1] = 'T';
	ESP8266TXData[2] = 'E';
	ESP8266TXData[3] = '0';
	ESP8266TXData[4] = 0x0d;
	ESP8266TXData[5] = 0x0a;
	NoOfdata_byte=6;
	wifi_command=13;
	Timerdelay=0;
	for(i=0;i<6;i++)
	{
  	 LPC_ASC_vSendData(ESP8266TXData[i]);
	 //Delay_Mc_Ms(10);
	}
	break;
	case 13:
	if(++Timerdelay >15)
	{
		Timerdelay=0;
		wifi_command=20;
	}
	break;
	case 14:
	ESP8266TXData[0] = 'A';
	ESP8266TXData[1] = 'T';
	ESP8266TXData[2] = 0x0d;
	ESP8266TXData[3] = 0x0a;
	NoOfdata_byte=3;
	Rxseqdecoder=8;
	wifi_command=20;//default wait 30sec
	Timerdelay=0;
	for(i=0;i<4;i++)
	{
  	 LPC_ASC_vSendData(ESP8266TXData[i]);
	 //Delay_Mc_Ms(10);
	}
	break;
	case 15:
	if(++Timerdelay >2)
	{
		Timerdelay=0;
	//	if(++retrypriolost>3){wifi_command=10;retrypriolost=0;}
	//	else{wifi_command=14;}
	}
	break;
	case 20:   //CWMODE		//hardrest sequce//1.0 for wifi reset
	ESP8266TXData[0] = 'A';
	ESP8266TXData[1] = 'T';
	ESP8266TXData[2] = '+';
	ESP8266TXData[3] = 'C';
	ESP8266TXData[4] = 'W';
	ESP8266TXData[5] = 'M';
	ESP8266TXData[6] = 'O';
	ESP8266TXData[7] = 'D';
	ESP8266TXData[8] = 'E';
	ESP8266TXData[9] = '=';
	ESP8266TXData[10] = '3';
	ESP8266TXData[11] = 0x0d;
	ESP8266TXData[12] = 0x0a;
	NoOfdata_byte=12;

	Rxseqdecoder=1;	  //add retry
	Timerdelay=0;
	bufferptr=0;
	for(i=0;i<13;i++)
	{
  	 LPC_ASC_vSendData(ESP8266TXData[i]);
	 //Delay_Mc_Ms(10);
	}
	Check_CWMODE_For_Hang = Check_CWMODE_For_Hang+1;
	wifi_command=21;

	break;
	case 21:	//waiting for Ok  && if above 30 seconds resend command
	if(++Timerdelay >5)
	{
		Timerdelay=0;
		if(5 ==  Check_CWMODE_For_Hang)
		{
			wifi_command=5;	 //Hardware reset
			Check_CWMODE_For_Hang = 0;
		}
		else
		{
			wifi_command=10;
		}
	}
	break;
	case 30:   //CWJAP	   //userid-9digit,psw 8digit  //userid-5digit,psw 8digit
	ESP8266TXData[0] = 'A';
	ESP8266TXData[1] = 'T';
	ESP8266TXData[2] = '+';
	ESP8266TXData[3] = 'C';
	ESP8266TXData[4] = 'W';
	ESP8266TXData[5] = 'J';
	ESP8266TXData[6] = 'A';
	ESP8266TXData[7] = 'P';
	ESP8266TXData[8] = '=';
	ESP8266TXData[9] = '"';
	ESP8266TXData[10] = 'A';	
	ESP8266TXData[11] = 'c';
	ESP8266TXData[12] = 'c';
	ESP8266TXData[13] = 'e';
	ESP8266TXData[14] = 'e';
	ESP8266TXData[15] = 'd';
	ESP8266TXData[16] = 'o'; 
	ESP8266TXData[17] = '_';
	ESP8266TXData[18] = 'D';
	ESP8266TXData[19] = 'e';
	ESP8266TXData[20] = 'm';
	ESP8266TXData[21] = 'o';
	ESP8266TXData[22] = '"';
	ESP8266TXData[23] = ',';	
	ESP8266TXData[24] = '"';
	ESP8266TXData[25] = 'A';
	ESP8266TXData[26] = 'c';
	ESP8266TXData[27] = 'C';//(unsigned char)(((WifiUserId%10000)/1000)+0x30);	4259241555	  
	ESP8266TXData[28] = 'e';//(unsigned char)(((WifiUserId%1000)/100)+0x30);//'e';
	ESP8266TXData[29] = 'E';//(unsigned char)(((WifiUserId%100)/10)+0x30);//'s';
	ESP8266TXData[30] = 'd';//(unsigned char)((WifiUserId%10)+0x30);//'h';
	ESP8266TXData[31] = 'O';//(unsigned char)(((WifiUserId2%10000)/1000)+0x30);
	ESP8266TXData[32] = '@';//(unsigned char)(((WifiUserId2%1000)/100)+0x30);//'e';
	ESP8266TXData[33] = '1';//(unsigned char)(((WifiUserId2%100)/10)+0x30);//'s';
	ESP8266TXData[34] = '2';//(unsigned char)(((WifiUserId%10000)/1000)+0x30);		  
	ESP8266TXData[35] = '3';
	ESP8266TXData[36] = '"';
	ESP8266TXData[37] = 0x0d;
	ESP8266TXData[38] = 0x0a;								   
	NoOfdata_byte=38;//32;	   
	Rxseqdecoder=2;
	wifi_command=41;
	Timerdelay=0;
	for(i=0;i<39;i++)
	{
  	 LPC_ASC_vSendData(ESP8266TXData[i]);
	 //Delay_Mc_Ms(10);
	}
	break;		   //add retry
	case 41:	   //resend if o replay
	if(++Timerdelay >20)
	{
		Timerdelay=0;
		wifi_command=10;	//41
	}
	break;
	case 49:
	if(++Timerdelay >10)
	{
		Timerdelay=0;				
		wifi_command=50;
	}
	break;
	case 50:   //CWJAP
	ESP8266TXData[0] = 'A';
	ESP8266TXData[1] = 'T';
	ESP8266TXData[2] = '+';
	ESP8266TXData[3] = 'C';
	ESP8266TXData[4] = 'W';
	ESP8266TXData[5] = 'J';
	ESP8266TXData[6] = 'A';
	ESP8266TXData[7] = 'P';
	ESP8266TXData[8] = '?';
	ESP8266TXData[9] = 0x0d;
	ESP8266TXData[10] = 0x0a;																	   
	NoOfdata_byte=10;
	wifi_command=51;
	Rxseqdecoder=3;
	Timerdelay=0;
	for(i=0;i<11;i++)
	{
  	 LPC_ASC_vSendData(ESP8266TXData[i]);
	 //Delay_Mc_Ms(10);
	}
	break;
	case 51:
	if(++Timerdelay >15)
	{
		Timerdelay=0;
		wifi_command=10; //50
	}

	break;
	case 70:   //CIPMUX
	ESP8266TXData[0] = 'A';
	ESP8266TXData[1] = 'T';
	ESP8266TXData[2] = '+';
	ESP8266TXData[3] = 'C';
	ESP8266TXData[4] = 'I';
	ESP8266TXData[5] = 'P';
	ESP8266TXData[6] = 'M';
	ESP8266TXData[7] = 'U';
	ESP8266TXData[8] = 'X';
	ESP8266TXData[9] = '=';
	ESP8266TXData[10] = '1';
	ESP8266TXData[11] = 0x0d;
	ESP8266TXData[12] = 0x0a;
	NoOfdata_byte=12;
	wifi_command=71;
	Rxseqdecoder=4;
	Timerdelay=0;
	for(i=0;i<13;i++)
	{
  	 LPC_ASC_vSendData(ESP8266TXData[i]);
	 //Delay_Mc_Ms(10);
	}
	break;	  //2sec
	case 71:
	if(++Timerdelay >15)
	{
		Timerdelay=0;
		wifi_command=10;   //70
	}
	break;
	case 80:  
	ESP8266TXData[0] = 'A';
	ESP8266TXData[1] = 'T';
	ESP8266TXData[2] = '+';
	ESP8266TXData[3] = 'C';
	ESP8266TXData[4] = 'I';
	ESP8266TXData[5] = 'P';
	ESP8266TXData[6] = 'S';
	ESP8266TXData[7] = 'T';
	ESP8266TXData[8] = 'A';
	ESP8266TXData[9] = 'R';
	ESP8266TXData[10] = 'T';
	ESP8266TXData[11] = '=';
	ESP8266TXData[12] = '1';
	ESP8266TXData[13] = ',';
	ESP8266TXData[14] = '"';
	ESP8266TXData[15] = 'T';
	ESP8266TXData[16] = 'C';
	ESP8266TXData[17] = 'P';
	ESP8266TXData[18] = '"';
	ESP8266TXData[19] = ',';
	ESP8266TXData[20] = '"';
	ESP8266TXData[21] = 'u';
	ESP8266TXData[22] = 's';
	ESP8266TXData[23] = 'm';
	ESP8266TXData[24] = '4';
	ESP8266TXData[25] = '-';
	ESP8266TXData[26] = 'g';
	ESP8266TXData[27] = 'd';
	ESP8266TXData[28] = 'c';
 	ESP8266TXData[29] = '.';
	ESP8266TXData[30] = 'a';
	ESP8266TXData[31] = 'c';
	ESP8266TXData[32] = 'c';
	ESP8266TXData[33] = 'e';
	ESP8266TXData[34] = 'e';
	ESP8266TXData[35] = 'd';
	ESP8266TXData[36] = 'o';
	ESP8266TXData[37] = '.';
	ESP8266TXData[38] = 'i';
	ESP8266TXData[39] = 'n';
	ESP8266TXData[40] = '"';
	ESP8266TXData[41] = ',';
	ESP8266TXData[42] = '9';
	ESP8266TXData[43] = '0';
	ESP8266TXData[44] = '1';
	ESP8266TXData[45] = '2';
	ESP8266TXData[46] = 0x0d;
	ESP8266TXData[47] = 0x0a; 
	NoOfdata_byte=47;//41;
	wifi_command=81;
	Rxseqdecoder=5;
	Timerdelay=0;
	for(i=0;i<48;i++)
	{
  	 LPC_ASC_vSendData(ESP8266TXData[i]);
	 //Delay_Mc_Ms(10);
	}
	break;
	case 81:					//5sec
	if(++Timerdelay >20)
	{
		Timerdelay=0;
		wifi_command=10;
	}
	break;
	case 90:   //CIPSEND
	ESP8266TXData[0] = 'A';
	ESP8266TXData[1] = 'T';
	ESP8266TXData[2] = '+';
	ESP8266TXData[3] = 'C';
	ESP8266TXData[4] = 'I';
	ESP8266TXData[5] = 'P';
	ESP8266TXData[6] = 'S';
	ESP8266TXData[7] = 'E';
	ESP8266TXData[8] = 'N';
	ESP8266TXData[9] = 'D';
	ESP8266TXData[10] = '=';
	ESP8266TXData[11] = '1';
	ESP8266TXData[12] = ',';
 	ESP8266TXData[13] = (unsigned char)(((129%1000)/100)+0x30);	  //url_send	ESP8266TXData[13] = '2';
	ESP8266TXData[14] = (unsigned char)(((129%100)/10)+0x30);	//	ESP8266TXData[14] = '0';	
	ESP8266TXData[15] = (unsigned char)((129%10)+0x30);		//	ESP8266TXData[15] = '3';	//179-178  //194
	ESP8266TXData[16] = 0x0d;
	ESP8266TXData[17] = 0x0a;
	NoOfdata_byte	= 17;
	wifi_command=91;
	Rxseqdecoder=6;
	Timerdelay=0;
	for(i=0;i<18;i++)
	{
  	 LPC_ASC_vSendData(ESP8266TXData[i]);
	 //Delay_Mc_Ms(10);
	}
	break;	
	case 91:
	if(++Timerdelay >15)
	{
		Timerdelay=0;
		wifi_command=10;
	}
	break;
	case 100:
//	http://168.63.148.113:9004/productionCount?mac=11&dlt=100&drt=200&pq=20&rq=20&mx=0&pt=60&pars=0
	if(Macid_Wifi ==11){Macid_Wifi=12;}
	else if(Macid_Wifi ==12){Macid_Wifi=13;}
	else if(Macid_Wifi ==13){Macid_Wifi=14;}
	else if(Macid_Wifi ==14){Macid_Wifi=11;}
	else{Macid_Wifi=11;}
	url_buffer =0;
   	ESP8266TXData[url_buffer] = 'G';
    url_buffer++;
	ESP8266TXData[url_buffer] = 'E';
    url_buffer++;
	ESP8266TXData[url_buffer] = 'T';
    url_buffer++;
	ESP8266TXData[url_buffer] = ' ';
    url_buffer++;
	ESP8266TXData[url_buffer] = '/';
    url_buffer++;				  
	ESP8266TXData[url_buffer] = 'p';
    url_buffer++;
	ESP8266TXData[url_buffer] = 'r';
    url_buffer++;
	ESP8266TXData[url_buffer] = 'o';
    url_buffer++;				 
	ESP8266TXData[url_buffer] = 'd';
    url_buffer++;
	ESP8266TXData[url_buffer] = 'u';
    url_buffer++;
	ESP8266TXData[url_buffer] = 'c';
    url_buffer++;
	ESP8266TXData[url_buffer] = 't';
    url_buffer++;
	ESP8266TXData[url_buffer] = 'i';
    url_buffer++;	 
	ESP8266TXData[url_buffer] = 'o';
    url_buffer++;	 
	ESP8266TXData[url_buffer] = 'n';
    url_buffer++;
	ESP8266TXData[url_buffer] = 'C';
    url_buffer++;
	ESP8266TXData[url_buffer] = 'o';
    url_buffer++;
	ESP8266TXData[url_buffer] = 'u';
    url_buffer++;
	ESP8266TXData[url_buffer] = 'n';
    url_buffer++;
	ESP8266TXData[url_buffer] = 't';
    url_buffer++;	
	ESP8266TXData[url_buffer] = '?';
    url_buffer++;
	ESP8266TXData[url_buffer] = 'm';
    url_buffer++;
	ESP8266TXData[url_buffer] = 'a';
    url_buffer++;
	ESP8266TXData[url_buffer] = 'c';
    url_buffer++;
	ESP8266TXData[url_buffer] = '=';
    url_buffer++;
	ESP8266TXData[url_buffer] = (unsigned char)(((Macid_Wifi%100)/10)+0x30);//dinesh  
    url_buffer++;
	ESP8266TXData[url_buffer] = (unsigned char)((Macid_Wifi%10)+0x30);;	//Dinesh
    url_buffer++;
	ESP8266TXData[url_buffer] = '&';
    url_buffer++;	
	ESP8266TXData[url_buffer] = 'd'; //Die1 Left Temperature 
    url_buffer++;
	ESP8266TXData[url_buffer] = 'l';
    url_buffer++;
	ESP8266TXData[url_buffer] = 't';
    url_buffer++;
	ESP8266TXData[url_buffer] = '=';
	url_buffer++;
	ESP8266TXData[url_buffer] = (unsigned char)(((RxDataSetmac[Macid_Wifi-11].temperature1_act%10000)/1000)+0x30);		  
    url_buffer++;
	ESP8266TXData[url_buffer] = (unsigned char)(((RxDataSetmac[Macid_Wifi-11].temperature1_act%1000)/100)+0x30);
    url_buffer++;
	ESP8266TXData[url_buffer] = (unsigned char)(((RxDataSetmac[Macid_Wifi-11].temperature1_act%100)/10)+0x30);
    url_buffer++;			
	ESP8266TXData[url_buffer] = (unsigned char)((RxDataSetmac[Macid_Wifi-11].temperature1_act%10)+0x30);
    url_buffer++;									
	ESP8266TXData[url_buffer] = '&';
    url_buffer++;
	ESP8266TXData[url_buffer] = 'd';	//Die1 Right Temperature
    url_buffer++;
	ESP8266TXData[url_buffer] = 'r';
    url_buffer++;
	ESP8266TXData[url_buffer] = 't';
    url_buffer++;
	ESP8266TXData[url_buffer] = '=';
    url_buffer++;
	ESP8266TXData[url_buffer] = (unsigned char)(((RxDataSetmac[Macid_Wifi-11].temperature2_act%10000)/1000)+0x30);		 
    url_buffer++;
	ESP8266TXData[url_buffer] = (unsigned char)(((RxDataSetmac[Macid_Wifi-11].temperature2_act%1000)/100)+0x30);
    url_buffer++;
	ESP8266TXData[url_buffer] = (unsigned char)(((RxDataSetmac[Macid_Wifi-11].temperature2_act%100)/10)+0x30);
    url_buffer++;												
	ESP8266TXData[url_buffer] = (unsigned char)((RxDataSetmac[Macid_Wifi-11].temperature2_act%10)+0x30);
    url_buffer++;
	ESP8266TXData[url_buffer] = '&';
    url_buffer++;
	ESP8266TXData[url_buffer] = 'p';	//Poduction quantity quantity
    url_buffer++;
	ESP8266TXData[url_buffer] = 'q';
    url_buffer++;
	ESP8266TXData[url_buffer] = '=';
    url_buffer++;
	ESP8266TXData[url_buffer] = (unsigned char)(((RxDataSetmac[Macid_Wifi-11].production%100000)/10000)+0x30);		
    url_buffer++;
	ESP8266TXData[url_buffer] = (unsigned char)(((RxDataSetmac[Macid_Wifi-11].production%10000)/1000)+0x30);
    url_buffer++;
	ESP8266TXData[url_buffer] = (unsigned char)(((RxDataSetmac[Macid_Wifi-11].production%1000)/100)+0x30);
    url_buffer++;
	ESP8266TXData[url_buffer] = (unsigned char)(((RxDataSetmac[Macid_Wifi-11].production%100)/10)+0x30);
    url_buffer++;												
	ESP8266TXData[url_buffer] = (unsigned char)((RxDataSetmac[Macid_Wifi-11].production%10)+0x30);
    url_buffer++;
	ESP8266TXData[url_buffer] = '&';
    url_buffer++;
	ESP8266TXData[url_buffer] = 'r';	//rejection quantity
    url_buffer++;
	ESP8266TXData[url_buffer] = 'q';
    url_buffer++;
	ESP8266TXData[url_buffer] = '=';
    url_buffer++;
	ESP8266TXData[url_buffer] = (unsigned char)(((loc_rejectionCount[Macid_Wifi-11].rejection%100000)/10000)+0x30);		
    url_buffer++;
	ESP8266TXData[url_buffer] = (unsigned char)(((loc_rejectionCount[Macid_Wifi-11].rejection%10000)/1000)+0x30);
    url_buffer++;
	ESP8266TXData[url_buffer] = (unsigned char)(((loc_rejectionCount[Macid_Wifi-11].rejection%1000)/100)+0x30);
    url_buffer++;
	ESP8266TXData[url_buffer] = (unsigned char)(((loc_rejectionCount[Macid_Wifi-11].rejection%100)/10)+0x30);
    url_buffer++;												
	ESP8266TXData[url_buffer] = (unsigned char)((loc_rejectionCount[Macid_Wifi-11].rejection%10)+0x30);
    url_buffer++;	
	ESP8266TXData[url_buffer] = '&';
    url_buffer++;
	ESP8266TXData[url_buffer] = 'm';	//machine status
	url_buffer++;
	ESP8266TXData[url_buffer] = 'x';
    url_buffer++;
	ESP8266TXData[url_buffer] = '=';
    url_buffer++;
	ESP8266TXData[url_buffer] = (unsigned char)(((0%100)/10)+0x30);
    url_buffer++;
	ESP8266TXData[url_buffer] = (unsigned char)((0%10)+0x30);
    url_buffer++;	
	ESP8266TXData[url_buffer] = '&';
    url_buffer++;
	ESP8266TXData[url_buffer] = 'p';	//Pouring time
	url_buffer++;
	ESP8266TXData[url_buffer] = 't';
    url_buffer++;
	ESP8266TXData[url_buffer] = '=';
    url_buffer++;
	ESP8266TXData[url_buffer] = (unsigned char)((0%10)+0x30);
    url_buffer++;
	ESP8266TXData[url_buffer] = (unsigned char)(((RxDataSetmac[Macid_Wifi-11].TiltDown_Zeit%1000)/100)+0x30);
    url_buffer++;												
	ESP8266TXData[url_buffer] = (unsigned char)(((RxDataSetmac[Macid_Wifi-11].TiltDown_Zeit%100)/10)+0x30);
	url_buffer++;	
	ESP8266TXData[url_buffer] = '&';
	url_buffer++;
    ESP8266TXData[url_buffer] = 'p';	//Production auto reset status
	url_buffer++;
	ESP8266TXData[url_buffer] = 'a';
    url_buffer++;
	ESP8266TXData[url_buffer] = 'r';
    url_buffer++;
	ESP8266TXData[url_buffer] = 's';
    url_buffer++;
	ESP8266TXData[url_buffer] = '=';
    url_buffer++;
	ESP8266TXData[url_buffer] = (unsigned char)(((RxDataSetmac[Macid_Wifi-11].Rst_Responce%100)/10)+0x30);														   
    url_buffer++;
	ESP8266TXData[url_buffer] = (unsigned char)(((RxDataSetmac[Macid_Wifi-11].Rst_Responce%10))+0x30);
    url_buffer++;
	ESP8266TXData[url_buffer] = '&';
    url_buffer++;
	ESP8266TXData[url_buffer] = 'T';
    url_buffer++;
	ESP8266TXData[url_buffer] = 'l';
    url_buffer++;
	ESP8266TXData[url_buffer] = 't';
    url_buffer++;
	ESP8266TXData[url_buffer] = 'z';
    url_buffer++;
	ESP8266TXData[url_buffer] = '=';
    url_buffer++;
	ESP8266TXData[url_buffer] = (unsigned char)((0%10)+0x30);
    url_buffer++;
	ESP8266TXData[url_buffer] = (unsigned char)(((RxDataSetmac[Macid_Wifi-11].TiltDown_Zeit%1000)/100)+0x30);
    url_buffer++;												
	ESP8266TXData[url_buffer] = (unsigned char)(((RxDataSetmac[Macid_Wifi-11].TiltDown_Zeit%100)/10)+0x30);
	url_buffer++;
	ESP8266TXData[url_buffer] = 0x0D;
    url_buffer++;
	ESP8266TXData[url_buffer] = 0x0A;
    url_buffer++;
	ESP8266TXData[url_buffer] = 'H';
    url_buffer++;
	ESP8266TXData[url_buffer] = 'o';
    url_buffer++;
	ESP8266TXData[url_buffer] = 's';
    url_buffer++;
	ESP8266TXData[url_buffer] = 't';
    url_buffer++;
	ESP8266TXData[url_buffer] = ':';
    url_buffer++;
	ESP8266TXData[url_buffer] = ' ';
    url_buffer++;
	ESP8266TXData[url_buffer] = 'u';
    url_buffer++;
	ESP8266TXData[url_buffer] = 's';
    url_buffer++;
	ESP8266TXData[url_buffer] = 'm';
    url_buffer++;
	ESP8266TXData[url_buffer] = '4';
    url_buffer++;
	ESP8266TXData[url_buffer] = '-';
    url_buffer++;
	ESP8266TXData[url_buffer] = 'g';
    url_buffer++;
	ESP8266TXData[url_buffer] = 'd';
    url_buffer++;
	ESP8266TXData[url_buffer] = 'c';
    url_buffer++;
	ESP8266TXData[url_buffer] = '.';
    url_buffer++;
	ESP8266TXData[url_buffer] = 'a';
    url_buffer++;
	ESP8266TXData[url_buffer] = 'c';
    url_buffer++;
	ESP8266TXData[url_buffer] = 'c';
    url_buffer++;
	ESP8266TXData[url_buffer] = 'e';
    url_buffer++;
	ESP8266TXData[url_buffer] = 'e';	
    url_buffer++;//221	
	ESP8266TXData[url_buffer] = 'd';
    url_buffer++;//221
	ESP8266TXData[url_buffer] = 'o';
    url_buffer++;//221
	ESP8266TXData[url_buffer] = '.';
    url_buffer++;//221
	ESP8266TXData[url_buffer] = 'i';
    url_buffer++;//221
	ESP8266TXData[url_buffer] = 'n';
    url_buffer++;//221
	ESP8266TXData[url_buffer] = ':';
    url_buffer++;
	ESP8266TXData[url_buffer] = '9';
    url_buffer++;
	ESP8266TXData[url_buffer] = '0';
    url_buffer++;
	ESP8266TXData[url_buffer] = '1';
    url_buffer++;
	ESP8266TXData[url_buffer] = '2';
	url_buffer++;
	ESP8266TXData[url_buffer] = 0x0D;
    url_buffer++;
	ESP8266TXData[url_buffer] = 0x0A;
    url_buffer++;										       
	ESP8266TXData[url_buffer] = 0x0D;
    url_buffer++;
	ESP8266TXData[url_buffer] = 0x0A;

	url_send = url_buffer+1;
    url_buffer++;

	NoOfdata_byte	= url_buffer;
	wifi_command=101;
	Timerdelay=0;
	Rxseqdecoder=7;
	for(i=0;i<url_buffer;i++)
	{
  	 LPC_ASC_vSendData(ESP8266TXData[i]);
	 //Delay_Mc_Ms(10);
	}
	break; 
	

	case 101:
		if(++Timerdelay>40)
	   {
	   	Timerdelay=0;
		wifi_command=10;
	   }
	break;
	case 102:
	   if(++Timerdelay>2)
	   {
	   	Timerdelay=0;
		wifi_command=50;
	   }
	break;
	case 103:
	   if(++Timerdelay>2)
	   {
	   	Timerdelay=0;
		wifi_command=10;
	   }
	break;
	case 110:
	ESP8266TXData[0] = 'A';
	ESP8266TXData[1] = 'T';
	ESP8266TXData[2] = '+';
	ESP8266TXData[3] = 'C';
	ESP8266TXData[4] = 'I';
	ESP8266TXData[5] = 'P';
	ESP8266TXData[6] = 'C';
	ESP8266TXData[7] = 'L';
	ESP8266TXData[8] = 'O';
	ESP8266TXData[9] = 'S';
	ESP8266TXData[10] = 'E';
	ESP8266TXData[11] = 0x0a;
	ESP8266TXData[12] = 0x0d;
	NoOfdata_byte=12;
	wifi_command=50;
	for(i=0;i<18;i++)
	{
  	 LPC_ASC_vSendData(ESP8266TXData[i]);
	 //Delay_Mc_Ms(10);
	}
	break;
	case 120:
	break; 
	default:
	wifi_command=5;
	break;
 } 
}


/*ESP8266 Receive function*/
void ESPRxDecoder(unsigned char Rxwifi_data,unsigned char Rxseqdecoder)
{
   	 
   static unsigned char Error_Retry;
	 switch(Rxseqdecoder)
	   {
	   	case 1:
			 if((Rxwifi_data=='O')&&(bufferptr==0))
			 {
			 	bufferptr=1;
			 }
			 else if((Rxwifi_data=='K')&&(bufferptr==1))
			 {
			 	bufferptr=0;
				Rxseqdecoder=0;
				wifi_command=30;
//				WifiStatusVar = 1;
				Check_CWMODE_For_Hang=0;
			 }
			 else{;}
			
			 if((Rxwifi_data=='n')&&(bufferptr==0))
			 {
			 	bufferptr=1;
			 }
			 else if((Rxwifi_data=='o')&&(bufferptr==1))
			 {
			 	bufferptr=2;
			 }
			 else if(bufferptr==2)
			 {
			 	bufferptr=3;
			 }
			 else if((Rxwifi_data=='c')&&(bufferptr==3))
			 {
			 	bufferptr=4;
			 }
			 else if((Rxwifi_data=='h')&&(bufferptr==4))
			 {
			 	bufferptr=5;
			 }
			 else if((Rxwifi_data=='a')&&(bufferptr==5))
			 {
			 	bufferptr=6;
			 }
			 else if((Rxwifi_data=='n')&&(bufferptr==6))
			 {
			 	bufferptr=7;
			 }
			 else if((Rxwifi_data=='g')&&(bufferptr==7))
			 {
			 	bufferptr=8;
			 }
			 else if((Rxwifi_data=='e')&&(bufferptr==8))
			 {
			 	bufferptr=0;
				Rxseqdecoder=0;
				wifi_command=30;
				Check_CWMODE_For_Hang=0;
			 }
			 else{;}
		break;
		case 2:
			 if((Rxwifi_data=='O')&&(bufferptr==0))
			 {
			 	bufferptr=1;
			 }
			 else if((Rxwifi_data=='K')&&(bufferptr==1))
			 {
			 	bufferptr=0;
				Rxseqdecoder=0;
				wifi_command=49;
			 }
		break;
		case 3:
			if((Rxwifi_data=='A')&&(bufferptr==0))
			 {
			 	bufferptr=1;
			 }
			 else if((Rxwifi_data=='c')&&(bufferptr==1))
			 {
			 	bufferptr=2;
			 }
			 else if((Rxwifi_data=='c')&&(bufferptr==2))
			 {
			 	bufferptr=3;
			 }

			 else if((Rxwifi_data=='e')&&(bufferptr==3))
			 {
			 	bufferptr=4;
			 }
			 else if((Rxwifi_data=='e')&&(bufferptr==4))
			 {
			 	bufferptr=5;
			 }
			 else if((Rxwifi_data=='d')&&(bufferptr==5))
			 {
			 	bufferptr=6;
			 }
			 else if((Rxwifi_data=='_')&&(bufferptr==6))
			 {
			 	bufferptr=7;
			 }
			 else if((Rxwifi_data=='D')&&(bufferptr==7))
			 {
			 	bufferptr=8;
			 }
			 else if((Rxwifi_data=='e')&&(bufferptr==8))
			 {
			 	bufferptr=9;
			 }
			 else if((Rxwifi_data=='m')&&(bufferptr==9))
			 {
			 	bufferptr=10;
			 }
			 else if((Rxwifi_data=='o')&&(bufferptr==10))
				 {
			 	bufferptr=0;
				Err_bufferptr=0;
				Rxseqdecoder=0;
				wifi_command=70;
				Error_Retry=0;
				WifiDisplay = 1;
//				WifiStatusVar = 2;
			 }	 
		
		
		/*	 if((Rxwifi_data=='t')&&(bufferptr==0))
			 {
			 	bufferptr=1;
			 }
			 else if((Rxwifi_data=='e')&&(bufferptr==1))
			 {
			 	bufferptr=2;
			 }
			 else if((Rxwifi_data=='s')&&(bufferptr==2))
			 {
			 	bufferptr=3;
			 }
			 else if((Rxwifi_data=='t')&&(bufferptr==3))
			 {
			 	bufferptr=0;
				Err_bufferptr=0;
				Rxseqdecoder=0;
				wifi_command=70;
				Error_Retry=0;
			 }	 */
			/* Rxwifi_data = UART_ubGetData8();
			 if((Rxwifi_data=='N')&&(bufferptr==0))
			 {
			 	bufferptr=1;
			 }
			 else if((Rxwifi_data=='O')&&(bufferptr==1))
			 {
			 	bufferptr=2;
			 }
			 else if((Rxwifi_data=='K')&&(bufferptr==2))
			 {
			 	bufferptr=3;
			 }
			 else if((Rxwifi_data=='I')&&(bufferptr==3))
			 {
			 	bufferptr=4;
			 }
			 else if((Rxwifi_data=='A')&&(bufferptr==4))
			 {
			 	bufferptr=0;
				Err_bufferptr=0;
				Rxseqdecoder=0;
				wifi_command=70;
				Error_Retry=0;
			 }	*/

			 if((Rxwifi_data=='N')&&(Err_bufferptr==0))
			 {
			 	Err_bufferptr=1;
			 }
			 else if((Rxwifi_data=='o')&&(Err_bufferptr==1))
			 {
			 	Err_bufferptr=2;
			 }
			 else if((Rxwifi_data=='A')&&(Err_bufferptr==2))
			 {
			 	Err_bufferptr=3;
			 }
			 else if((Rxwifi_data=='P')&&(Err_bufferptr==3))
			 {
			 	Err_bufferptr=0;
				bufferptr=0;
				Rxseqdecoder=0;
				WifiDisplay = 3;
				if(++Error_Retry<3)		//retry for error
				{
				   wifi_command=50;
				}
				else
				{
				   wifi_command=10;
				   Error_Retry=0;
				}
			 }
		break;
		case 4:	   //retry need to be added
			 if((Rxwifi_data=='O')&&(bufferptr==0))
			 {
			 	bufferptr=1;
			 }
			 else if((Rxwifi_data=='K')&&(bufferptr==1))
			 {
			 	bufferptr=0;
				Rxseqdecoder=0;
				wifi_command=80;
			 }
		break;
		case 5:
			 if((Rxwifi_data=='C')&&(bufferptr==0))
			 {
			 	bufferptr=1;
			 }
			 else if((Rxwifi_data=='O')&&(bufferptr==1))
			 {
			 	bufferptr=2;
			 }
			 else if((Rxwifi_data=='N')&&(bufferptr==2))
			 {
			 	bufferptr=3;
			 }
			 else if((Rxwifi_data=='N')&&(bufferptr==3))
			 {
			 	bufferptr=4;
			 }
			 else if((Rxwifi_data=='E')&&(bufferptr==4))
			 {
			 	bufferptr=5;
			 }
			 else if((Rxwifi_data=='C')&&(bufferptr==5))
			 {
			 	bufferptr=6;
			 }
			 else if((Rxwifi_data=='T')&&(bufferptr==6))
			 {
			 	Rxseqdecoder=0;
				wifi_command=90;
				bufferptr=0;
				Error_Retry=0;
				Err_bufferptr=0; //wifi connected
			   WifiDisplay = 2;
			 }
			 	if((Rxwifi_data=='L')&&(bufferptr==0))	  //linked
			 {
			 	bufferptr=1;
			 }
			 else if((Rxwifi_data=='i')&&(bufferptr==1))
			 {
			 	bufferptr=2;
			 }
			 else if((Rxwifi_data=='n')&&(bufferptr==2))
			 {
			 	bufferptr=3;
			 }
			 else if((Rxwifi_data=='k')&&(bufferptr==3))
			 {
			 	bufferptr=4;
			 }
			 else if((Rxwifi_data=='e')&&(bufferptr==4))
			 {
			 	bufferptr=5;
			 }
			 else if((Rxwifi_data=='d')&&(bufferptr==5))
			 {
			 	Rxseqdecoder=0;
				wifi_command=90;
				bufferptr=0;
				Error_Retry=0;
				Err_bufferptr=0;
			 }
			 if((Rxwifi_data=='E')&&(Err_bufferptr==0))
			 {
			 	Err_bufferptr=1;
			 }
			 else if((Rxwifi_data=='R')&&(Err_bufferptr==1))
			 {
			 	Err_bufferptr=2;
			 }
			 else if((Rxwifi_data=='R')&&(Err_bufferptr==2))
			 {
			 	Err_bufferptr=3;
			 }
			 else if((Rxwifi_data=='O')&&(Err_bufferptr==3))
			 {
			 	Err_bufferptr=4;
			 }
			 else if((Rxwifi_data=='R')&&(Err_bufferptr==4))
			 {
			   	Err_bufferptr=0;
				bufferptr=0;
				Rxseqdecoder=0;
				WifiDisplay = 4;
				if(++Error_Retry<3)		//retry for error
				{
				   wifi_command=80;
				}
				else
				{
				   wifi_command=50;	 // modified by satheesh
				   Error_Retry=0;
				}
			 }
		break;
		case 6:
			 if((Rxwifi_data=='>')&&(bufferptr==0))
			 {
			 	bufferptr=0;
				Rxseqdecoder=0;
				wifi_command=100;
			 }
		break;
		case 7:
			 
			  if((Rxwifi_data=='$')&&(Data_bufferptr==0))
 				{
                     Data_bufferptr=1;
                     Valid_DataWifi1= 1;
 				}
			 	else if(Data_bufferptr==1)      //machineDetails 
				 {
                    Uart_rx_buffer[0] = DecToASCIIFun(Rxwifi_data); 
					Data_bufferptr=2;
				}
				else if(Data_bufferptr==2)
				{
				     Uart_rx_buffer[1] = DecToASCIIFun(Rxwifi_data);
				     Data_bufferptr=3;
				}
				else if(Data_bufferptr==3)      //,(to seperate data)
				{
				     Data_bufferptr=4;
				}
				else if(Data_bufferptr==4)	   //rightDieSetTemp
				{
				     Uart_rx_buffer[2] = DecToASCIIFun(Rxwifi_data);
				     Data_bufferptr=5;
				}
				else if(Data_bufferptr==5)
				{
				     Uart_rx_buffer[3] = DecToASCIIFun(Rxwifi_data);		
				     Data_bufferptr=6;
				}
				else if(Data_bufferptr==6)    
				{
				     Uart_rx_buffer[4] = DecToASCIIFun(Rxwifi_data);
					 Data_bufferptr=7;
				}
				else if(Data_bufferptr==7)       					
				{
                    Uart_rx_buffer[5] = DecToASCIIFun(Rxwifi_data); 
					Data_bufferptr=8;
				}
				else if(Data_bufferptr==8)      //,(to seperate data)
				{
				     Data_bufferptr=9;
				}
				else if(Data_bufferptr==9)	//rightDiehighTemp
				{
				     Uart_rx_buffer[6] = DecToASCIIFun(Rxwifi_data);
				     Data_bufferptr=10;
				}
				else if(Data_bufferptr==10)
				{
				     Uart_rx_buffer[7] = DecToASCIIFun(Rxwifi_data);
				     Data_bufferptr=11;
				}
				else if(Data_bufferptr==11)
				{
				     Uart_rx_buffer[8] = DecToASCIIFun(Rxwifi_data);	
				     Data_bufferptr=12;
				}
				else if(Data_bufferptr==12)
				{
				     Uart_rx_buffer[9] = DecToASCIIFun(Rxwifi_data);	
				     Data_bufferptr=13;
				}
				else if(Data_bufferptr==13)      //,(to seperate data)
				{
				     Data_bufferptr=14;
				}
				else if(Data_bufferptr==14)       //rightDieLowTemp
				 {
                    Uart_rx_buffer[10] = DecToASCIIFun(Rxwifi_data); 
					Data_bufferptr=15;
				}			
				else if(Data_bufferptr==15)     
				{
				     Data_bufferptr=16;
					 Uart_rx_buffer[11] = DecToASCIIFun(Rxwifi_data); 
				}
				else if(Data_bufferptr==16)       				 
				 {
                    Uart_rx_buffer[12] = DecToASCIIFun(Rxwifi_data); 
					Data_bufferptr=17;
				}
				else if(Data_bufferptr==17)
				{
				     Uart_rx_buffer[13] = DecToASCIIFun(Rxwifi_data);
				     Data_bufferptr=18;
				}

				else if(Data_bufferptr==18)		 //,(to seperate data)
				{
				     Data_bufferptr=19;
				}
				else if(Data_bufferptr==19)		 //leftDieSetTemp
				{
				     Uart_rx_buffer[14] = DecToASCIIFun(Rxwifi_data);	 
				     Data_bufferptr=20;
				}
				else if(Data_bufferptr==20)      
				{
				     Data_bufferptr=21;
					 Uart_rx_buffer[15] = DecToASCIIFun(Rxwifi_data);	 
				}

				else if(Data_bufferptr==21)       	
				 {
                    Uart_rx_buffer[16] = DecToASCIIFun(Rxwifi_data); 
					Data_bufferptr=22;
				}
				else if(Data_bufferptr==22)
				{
				     Uart_rx_buffer[17] = DecToASCIIFun(Rxwifi_data);
				     Data_bufferptr=23;
				}
				else if(Data_bufferptr==23)      //,(to seperate data)
				{
				     Data_bufferptr=24;
				}
				else if(Data_bufferptr==24)		 //leftDiehighTemp
				{
				     Uart_rx_buffer[18] = DecToASCIIFun(Rxwifi_data);
				     Data_bufferptr=25;
				}
				else if(Data_bufferptr==25)
				{
				     Uart_rx_buffer[19] = DecToASCIIFun(Rxwifi_data);	 
				     Data_bufferptr=26;
				}
				else if(Data_bufferptr==26)     
				 {
                    Uart_rx_buffer[20] = DecToASCIIFun(Rxwifi_data); 
					Data_bufferptr=27;
				}
				else if(Data_bufferptr==27)
				{
				     Uart_rx_buffer[21] = DecToASCIIFun(Rxwifi_data);
				     Data_bufferptr=28;
				}
				else if(Data_bufferptr==28)	   //,(to seperate data)
				{
				     Data_bufferptr=29;
				}
				else if(Data_bufferptr==29)	   //leftDieLowTemp
				{
				     Uart_rx_buffer[22] = DecToASCIIFun(Rxwifi_data);	
				     Data_bufferptr=30;
				}
				else if(Data_bufferptr==30)
				{
				     Uart_rx_buffer[23] = DecToASCIIFun(Rxwifi_data);	
				     Data_bufferptr=31;
				}
				else if(Data_bufferptr==31)      
				 {
                    Uart_rx_buffer[24] = DecToASCIIFun(Rxwifi_data); 
					Data_bufferptr=32;
				}
				else if(Data_bufferptr==32)
				{
				     Uart_rx_buffer[25] = DecToASCIIFun(Rxwifi_data);
				     Data_bufferptr=35;//33,34 left out
				}
				else if(Data_bufferptr==35)		  //,(to seperate data)
				{
				     Data_bufferptr=36;
				}
				else if(Data_bufferptr==36)		  //curingTemp
				{
				     Uart_rx_buffer[26] = DecToASCIIFun(Rxwifi_data);	
				     Data_bufferptr=37;
				}
				else if(Data_bufferptr==37)      
				{
				     Uart_rx_buffer[27] = DecToASCIIFun(Rxwifi_data);
					 Data_bufferptr=38;
				}	
				else if(Data_bufferptr==38)       
				 {
                    Uart_rx_buffer[28] = DecToASCIIFun(Rxwifi_data); 
					Data_bufferptr=39;
				}
				else if(Data_bufferptr==39)
				{
				     Uart_rx_buffer[29] = DecToASCIIFun(Rxwifi_data);
				     Data_bufferptr=40;
				}
				else if(Data_bufferptr==40)	 //,
				{
				     Data_bufferptr=41;
				}
				else if(Data_bufferptr==41)	  //tiltingUpTime
				{
				     Uart_rx_buffer[30] = DecToASCIIFun(Rxwifi_data);	  
				     Data_bufferptr=42;					 
				}				 
				else if(Data_bufferptr==42)       
				 {
                    Uart_rx_buffer[31] = DecToASCIIFun(Rxwifi_data); 
					Data_bufferptr=43;
				}
				else if(Data_bufferptr==43)
				{
				     Uart_rx_buffer[32] = DecToASCIIFun(Rxwifi_data);
				     Data_bufferptr=44;
				}
				else if(Data_bufferptr==44)
				{
				     Uart_rx_buffer[33] = DecToASCIIFun(Rxwifi_data);
				     Data_bufferptr=45;
				}
				else if(Data_bufferptr==45)      //,(to seperate data)
				{
				     Data_bufferptr=46;
				}			 
			 	else if(Data_bufferptr==46)       //slideCoreTime
				 {
                    Uart_rx_buffer[34] = DecToASCIIFun(Rxwifi_data); 
					Data_bufferptr=47;
				}
				else if(Data_bufferptr==47)
				{
				     Uart_rx_buffer[35] = DecToASCIIFun(Rxwifi_data);
				     Data_bufferptr=48;
				}
				else if(Data_bufferptr==48)
				{
				     Uart_rx_buffer[36] = DecToASCIIFun(Rxwifi_data);
				     Data_bufferptr=49;
				}
				else if(Data_bufferptr==49)
				{
				     Uart_rx_buffer[37] = DecToASCIIFun(Rxwifi_data);	  
				     Data_bufferptr=50;
				}
				else if(Data_bufferptr==50)      //,(to seperate data)
				{
				     Data_bufferptr=51;
				}			 
			 	else if(Data_bufferptr==51)      //ejectionOnTime
				{
                    Uart_rx_buffer[38] = DecToASCIIFun(Rxwifi_data); 
					Data_bufferptr=52;
				}
				else if(Data_bufferptr==52)
				{
				     Uart_rx_buffer[39] = DecToASCIIFun(Rxwifi_data);
				     Data_bufferptr=53;
				}
				else if(Data_bufferptr==53)
				{
				     Uart_rx_buffer[40] = DecToASCIIFun(Rxwifi_data);
				     Data_bufferptr=54;
				}
				else if(Data_bufferptr==54)
				{
				     Uart_rx_buffer[41] = DecToASCIIFun(Rxwifi_data);	  
				     Data_bufferptr=55;
				}
				else if(Data_bufferptr==55)      //,(to seperate data)
				{
				     Data_bufferptr=56;
				}

				else if(Data_bufferptr==56)      //ejectionOffTime 
				 {
                    Uart_rx_buffer[42] = DecToASCIIFun(Rxwifi_data); 
					Data_bufferptr=57;
				}
				else if(Data_bufferptr==57)
				{
				     Uart_rx_buffer[43] = DecToASCIIFun(Rxwifi_data);
				     Data_bufferptr=58;
				}
				else if(Data_bufferptr==58)
				{
				     Uart_rx_buffer[44] = DecToASCIIFun(Rxwifi_data);
				     Data_bufferptr=59;
				}
				else if(Data_bufferptr==59)
				{
				     Uart_rx_buffer[45] = DecToASCIIFun(Rxwifi_data);	  
				     Data_bufferptr=60;
				}
				else if(Data_bufferptr==60)      //,(to seperate data)
				{
				     Data_bufferptr=61;
				}
				else if(Data_bufferptr==61)      //hrs 
				 {
                    Uart_rx_buffer[46] = DecToASCIIFun(Rxwifi_data); 
					Data_bufferptr=62;
				}
				else if(Data_bufferptr==62)
				{
				     Uart_rx_buffer[47] = DecToASCIIFun(Rxwifi_data);
				     Data_bufferptr=63;
				}
				else if(Data_bufferptr==63)      //,(to seperate data)
				{
				     Data_bufferptr=64;
				}
				else if(Data_bufferptr==64)		 //min
				{
				     Uart_rx_buffer[48] = DecToASCIIFun(Rxwifi_data);
				     Data_bufferptr=65;
				}
				else if(Data_bufferptr==65)
				{
				     Uart_rx_buffer[49] = DecToASCIIFun(Rxwifi_data);	 
				     Data_bufferptr=66;
				}
				else if(Data_bufferptr==66)      //,(to seperate data)
				{
				     Data_bufferptr=67;
				}
				else if(Data_bufferptr==67)		 //clearIndication
				{
				     Uart_rx_buffer[50] = DecToASCIIFun(Rxwifi_data);	 
				     Data_bufferptr=68;
				}
				else if(Data_bufferptr==68)
				{
				     Uart_rx_buffer[51] = DecToASCIIFun(Rxwifi_data);	 
				     Data_bufferptr=0;
					 RxCompleteU2C1WIFI=1;
				}
	
				
		   if(Valid_DataWifi1)
		   {
			 if((Rxwifi_data=='U')&&(bufferptr==0))
			 {
			 	bufferptr=1;
			 }
			 else if((Rxwifi_data=='n')&&(bufferptr==1))
			 {
			 	bufferptr=2;
			 }
			 else if((Rxwifi_data=='l')&&(bufferptr==2))
			 {
			    bufferptr=3;
			 }
			 else if((Rxwifi_data=='i')&&(bufferptr==3))
			 {
			    bufferptr=4;
			 }
			 else if((Rxwifi_data=='n')&&(bufferptr==4))
			 {
			    bufferptr=5;
			 }
			 else if((Rxwifi_data=='k')&&(bufferptr==5))
			 {
			    bufferptr=0;
				Rxseqdecoder=0;
				wifi_command=102;
				Error_Close=0;
				Valid_DataWifi1=0;
				RxCompleteU2C1WIFI=1;			  /*receive complete*/
			 }
			 if((Rxwifi_data=='C')&&(bufferptr==0))
			 {
			 	bufferptr=1;
			 }
			 else if((Rxwifi_data=='L')&&(bufferptr==1))
			 {
			 	bufferptr=2;
			 }
			 else if((Rxwifi_data=='O')&&(bufferptr==2))
			 {
			    bufferptr=3;
			 }
			 else if((Rxwifi_data=='S')&&(bufferptr==3))
			 {
			    bufferptr=4;
			 }
			 else if((Rxwifi_data=='E')&&(bufferptr==4))
			 {
			    bufferptr=5;
			 }
			 else if((Rxwifi_data=='D')&&(bufferptr==5))
			 {
			    bufferptr=0;
				Rxseqdecoder=0;
				wifi_command=102;
				Error_Close=0;
				Valid_DataWifi1=0;
				RxCompleteU2C1WIFI=1;				  /*receive complete*/
			 }
			}
			else
			{
			  if((Rxwifi_data=='U')&&(bufferptr==0))
			 {
			 	bufferptr=1;
			 }
			 else if((Rxwifi_data=='n')&&(bufferptr==1))
			 {
			 	bufferptr=2;
			 }
			 else if((Rxwifi_data=='l')&&(bufferptr==2))
			 {
			    bufferptr=3;
			 }
			 else if((Rxwifi_data=='i')&&(bufferptr==3))
			 {
			    bufferptr=4;
			 }
			 else if((Rxwifi_data=='n')&&(bufferptr==4))
			 {
			    bufferptr=5;
			 }
			 else if((Rxwifi_data=='k')&&(bufferptr==5))
			 {
			    bufferptr=0;
				Rxseqdecoder=0;
				wifi_command=102;
				Error_Close=0;
				Valid_DataWifi1=0;
				RxCompleteU2C1WIFI=1;				    /*receive complete*/
			 } 
			   
			   if((Rxwifi_data=='C')&&(bufferptr==0))
			 {
			 	bufferptr=1;
			 }
			 else if((Rxwifi_data=='L')&&(bufferptr==1))
			 {
			 	bufferptr=2;
			 }
			 else if((Rxwifi_data=='O')&&(bufferptr==2))
			 {
			    bufferptr=3;
			 }
			 else if((Rxwifi_data=='S')&&(bufferptr==3))
			 {
			    bufferptr=4;
			 }
			 else if((Rxwifi_data=='E')&&(bufferptr==4))
			 {
			    bufferptr=5;
			 }
			 else if((Rxwifi_data=='D')&&(bufferptr==5))
			 {
			    bufferptr=0;
				Rxseqdecoder=0;
				if(++Error_Close>3){wifi_command=10;Error_Close=0;}
				else{wifi_command=102;}
				Valid_DataWifi1=0;
				RxCompleteU2C1WIFI=0;   /*Wrong receive*/
			 }
			}
		break;
		case 8:
			 if((Rxwifi_data=='O')&&(bufferptr==0))
			 {
			 	bufferptr=1;
			 }
			 else if((Rxwifi_data=='K')&&(bufferptr==1))
			 {
			 	bufferptr=0;
				Rxseqdecoder=0;
				wifi_command=20;
			 }
		break;
	   	default:
		break;
		}
}

void RxDecoderPLCData(void)
{
	unsigned int ReceivePLCData,ReceivePLCData2,ReceivePLCData3,ReceivePLCData4;
	unsigned int ReceivePLCData7,ReceivePLCData8;
	unsigned int ReceivePLCData9,ReceivePLCData10,ReceivePLCData11,ReceivePLCData12,ReceivePLCData13;
	unsigned int ReceivePLCData5,ReceivePLCData6;
	
	if(!RxCompleteU2C1WIFI){return;}
	RxCompleteU2C1WIFI=0;
	MachineId = Uart_rx_buffer[1]+(Uart_rx_buffer[0]*10);
	ReceivePLCData  = Uart_rx_buffer[5]+(Uart_rx_buffer[4]*10)+(Uart_rx_buffer[3]*100)+(Uart_rx_buffer[2]*1000);//rightDieSetTemp
	ReceivePLCData2 = Uart_rx_buffer[9]+(Uart_rx_buffer[8]*10)+(Uart_rx_buffer[7]*100)+(Uart_rx_buffer[6]*1000);//rightDiehighTemp
	ReceivePLCData3 = Uart_rx_buffer[13]+(Uart_rx_buffer[12]*10)+(Uart_rx_buffer[11]*100)+(Uart_rx_buffer[10]*1000); //rightDieLowTemp
	ReceivePLCData4 = Uart_rx_buffer[17]+(Uart_rx_buffer[16]*10)+(Uart_rx_buffer[15]*100)+(Uart_rx_buffer[14]*1000);//leftDieSetTemp
	ReceivePLCData5 = Uart_rx_buffer[21]+(Uart_rx_buffer[20]*10)+(Uart_rx_buffer[19]*100)+(Uart_rx_buffer[18]*1000);//leftDiehighTemp
	ReceivePLCData6 = Uart_rx_buffer[25]+(Uart_rx_buffer[24]*10)+(Uart_rx_buffer[23]*100)+(Uart_rx_buffer[22]*1000);//leftDieLowTemp
	ReceivePLCData7 = Uart_rx_buffer[29]+(Uart_rx_buffer[28]*10)+(Uart_rx_buffer[27]*100)+(Uart_rx_buffer[26]*1000);//curingTemp
	ReceivePLCData8 = Uart_rx_buffer[33]+(Uart_rx_buffer[32]*10)+(Uart_rx_buffer[31]*100)+(Uart_rx_buffer[30]*1000);//tiltingUpTime
	ReceivePLCData9 = Uart_rx_buffer[37]+(Uart_rx_buffer[36]*10)+(Uart_rx_buffer[35]*100)+(Uart_rx_buffer[34]*1000);//slideCoreTime
	ReceivePLCData10 = Uart_rx_buffer[41]+(Uart_rx_buffer[40]*10)+(Uart_rx_buffer[39]*100)+(Uart_rx_buffer[38]*1000);//ejectionOnTime
	ReceivePLCData11 = Uart_rx_buffer[45]+(Uart_rx_buffer[44]*10)+(Uart_rx_buffer[43]*100)+(Uart_rx_buffer[42]*1000);//ejectionOffTime
	ReceivePLCData12 = Uart_rx_buffer[47]+(Uart_rx_buffer[46]*10);//hrs
	ReceivePLCData13 = Uart_rx_buffer[49]+(Uart_rx_buffer[48]*10);//mins
	ReceivePLCData14 = Uart_rx_buffer[51]+(Uart_rx_buffer[50]*10);//clearIndication
	
		  if((ReceivePLCData != 0) && (ReceivePLCData != RxWifi_TxDataSetMac[MachineId-11].rightDieSetTemp)&&(ReceivePLCData<5000))
		  {
		   RxWifi_TxDataSetMac[MachineId-11].rightDieSetTemp = ReceivePLCData;
		   I2C_Write(0xA0,0x00,0x02, RxWifi_TxDataSetMac[MachineId-11].rightDieSetTemp);
		   Eeprom_Intern_Delay(100);
		  }
		  if((ReceivePLCData2 != 0) && (ReceivePLCData2 != RxWifi_TxDataSetMac[MachineId-11].rightDiehighTemp)&&(ReceivePLCData2<5000))
		  {
		   RxWifi_TxDataSetMac[MachineId-11].rightDiehighTemp = ReceivePLCData2;
		   I2C_Write(0xA0,0x00,0x06, RxWifi_TxDataSetMac[MachineId-11].rightDiehighTemp);
		   Eeprom_Intern_Delay(100);
		  }		
		  if((ReceivePLCData3 != 0) && (ReceivePLCData3 != RxWifi_TxDataSetMac[MachineId-11].rightDieLowTemp)&&(ReceivePLCData3<5000))
		  {
		   RxWifi_TxDataSetMac[MachineId-11].rightDieLowTemp = ReceivePLCData3;
		   I2C_Write(0xA0,0x00,0x0a, RxWifi_TxDataSetMac[MachineId-11].rightDieLowTemp);
		   Eeprom_Intern_Delay(100);
		  }
		  if((ReceivePLCData4 != 0) && (ReceivePLCData4 != RxWifi_TxDataSetMac[MachineId-11].leftDieSetTemp)&&(ReceivePLCData4<5000))
		  {
		   RxWifi_TxDataSetMac[MachineId-11].leftDieSetTemp = ReceivePLCData4;
		   I2C_Write(0xA0,0x00,0x10, RxWifi_TxDataSetMac[MachineId-11].leftDieSetTemp);
		   Eeprom_Intern_Delay(100);
		  }		
		  if((ReceivePLCData5 != 0) && (ReceivePLCData5 != RxWifi_TxDataSetMac[MachineId-11].leftDiehighTemp)&&(ReceivePLCData5<5000))
		  {
		   RxWifi_TxDataSetMac[MachineId-11].leftDiehighTemp = ReceivePLCData5;
		   I2C_Write(0xA0,0x00,0x14, RxWifi_TxDataSetMac[MachineId-11].leftDiehighTemp);
		   Eeprom_Intern_Delay(100);
		  }
		  if((ReceivePLCData6 != 0) && (ReceivePLCData6 != RxWifi_TxDataSetMac[MachineId-11].leftDieLowTemp)&&(ReceivePLCData6<5000))
		  {
		   RxWifi_TxDataSetMac[MachineId-11].leftDieLowTemp = ReceivePLCData6;
		   I2C_Write(0xA0,0x00,0x18, RxWifi_TxDataSetMac[MachineId-11].leftDieLowTemp);
		   Eeprom_Intern_Delay(100);
		  }		
		  if((ReceivePLCData7 != 0) && (ReceivePLCData7 != RxWifi_TxDataSetMac[MachineId-11].curingTemp)&&(ReceivePLCData7<5000))
		  {
		   RxWifi_TxDataSetMac[MachineId-11].curingTemp = ReceivePLCData7;
		   I2C_Write(0xA0,0x00,0x1c, RxWifi_TxDataSetMac[MachineId-11].curingTemp);
		   Eeprom_Intern_Delay(100);
		  }
		  if((ReceivePLCData8 != 0) && (ReceivePLCData8 != RxWifi_TxDataSetMac[MachineId-11].tiltingUpTime)&&(ReceivePLCData8<5000))
		  {
		   RxWifi_TxDataSetMac[MachineId-11].tiltingUpTime = ReceivePLCData8;
		   I2C_Write(0xA0,0x00,0x21, RxWifi_TxDataSetMac[MachineId-11].tiltingUpTime);
		   Eeprom_Intern_Delay(100);
		  }
		  if((ReceivePLCData9 != RxWifi_TxDataSetMac[MachineId-11].slideCoreTime)&&(ReceivePLCData9<5000))
		  {
		   RxWifi_TxDataSetMac[MachineId-11].slideCoreTime = ReceivePLCData9;
		   I2C_Write(0xA0,0x00,0x25, RxWifi_TxDataSetMac[MachineId-11].slideCoreTime);
		   Eeprom_Intern_Delay(100);
		  }
		  if((ReceivePLCData10 != RxWifi_TxDataSetMac[MachineId-11].ejectionOnTime)&&(ReceivePLCData10<5000))
		  {
		   RxWifi_TxDataSetMac[MachineId-11].ejectionOnTime = ReceivePLCData10;
		   I2C_Write(0xA0,0x00,0x2a, RxWifi_TxDataSetMac[MachineId-11].ejectionOnTime);
		   Eeprom_Intern_Delay(100);
		  }
		  if((ReceivePLCData11 != RxWifi_TxDataSetMac[MachineId-11].ejectionOffTime)&&(ReceivePLCData11<5000))
		  {
		   RxWifi_TxDataSetMac[MachineId-11].ejectionOffTime = ReceivePLCData11;
		   I2C_Write(0xA0,0x00,0x2a, RxWifi_TxDataSetMac[MachineId-11].ejectionOffTime);
		   Eeprom_Intern_Delay(100);
		  }
		  MacSpecStatus_Rst[MachineId-11].MacStatus_Rst = ReceivePLCData14;
		  
		 
		  	 if(MacSpecStatus_Rst[0].MacStatus_Rst ==1)
			 {
			 loc_rejectionCount[0].rejection=0;
			 I2C_Write(0xA0,0x00,0x01, loc_rejectionCount[0].rejection);
	  		 Eeprom_Intern_Delay(100);
			 }
			 if(MacSpecStatus_Rst[1].MacStatus_Rst ==1)
			 {
			 loc_rejectionCount[1].rejection = 0;
	  		 I2C_Write(0xA0,0x00,0x05, loc_rejectionCount[1].rejection);
	  		 Eeprom_Intern_Delay(100);
			 }
			 if(MacSpecStatus_Rst[2].MacStatus_Rst ==1)
			 {
			 loc_rejectionCount[2].rejection = 0;
	  		 I2C_Write(0xA0,0x00,0x0a, loc_rejectionCount[2].rejection);
	  		 Eeprom_Intern_Delay(100);
			 }
			  if(MacSpecStatus_Rst[3].MacStatus_Rst ==1)
			 {
			 loc_rejectionCount[3].rejection = 0;
	  		 I2C_Write(0xA0,0x00,0x0f, loc_rejectionCount[3].rejection);
	  		 Eeprom_Intern_Delay(100);
		  	}
					
}

unsigned char HexToDecFun(unsigned int TempVar)
{
   unsigned char Dec1,Dec2;
   unsigned char decimal;
   Dec1 = TempVar%10;
   Dec2 = (TempVar%100)/10;
   

   if(Dec1 >= '0' && Dec1 <= '9')
   {
            decimal += (Dec1 - 48) * 1;
   }
   else if(Dec1 >= 'A' && Dec1 <= 'F')
   {
            decimal += (Dec1 - 55) * 1;
   }
   else if(Dec1 >= 'a' && Dec1 <= 'f')
   {
            decimal += (Dec1 - 87) * 1;
   }
   else
   {
   		   decimal+= 0;
   }
   
   if(Dec2 >= '0' && Dec2 <= '9')
   {
            decimal += (Dec2 - 48) * 16;
   }
   else if(Dec1 >= 'A' && Dec1 <= 'F')
   {
            decimal += (Dec2 - 55) * 16;
   }
   else if(Dec1 >= 'a' && Dec1 <= 'f')
   {
            decimal += (Dec2 - 87) * 16;
   }
   return decimal;
}
unsigned char DecToASCIIFun(unsigned char TempVar)
{
	unsigned char  ASCIIHex;
	switch(TempVar)
	{
	case 48:
			ASCIIHex = 0x00;
			break;
	case 49:
			ASCIIHex = 0x01;
			break;
	case 50:
			ASCIIHex = 0x02;
			break;
	case 51:
			ASCIIHex = 0x03;
			break;
	case 52:
			ASCIIHex = 0x04;
			break;
	case 53:
			ASCIIHex = 0x05;
			break;
	case 54:
			ASCIIHex = 0x06;
			break;
	case 55:
			ASCIIHex = 0x07;
			break;
	case 56:
			ASCIIHex = 0x08;
			break;
	case 57:
			ASCIIHex = 0x09;
			break;
	case 65:
			ASCIIHex = 0x0A;
			break;
	case 66:
			ASCIIHex = 0x0B;
			break;
	case 67:
			ASCIIHex = 0x0C;
			break;
	case 68:
			ASCIIHex = 0x0D;
			break;
	case 69:
			ASCIIHex = 0x0E;
			break;
	case 70:
			ASCIIHex = 0x0F;
			break;
	}
	return (ASCIIHex);
}
