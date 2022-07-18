#include<LPC214X.h>
#include"powermeter.h"
#include"powermeter_config.h"
#include"struct.h"

unsigned int TxSetTemperature;
extern unsigned int rightDieSetTemp[4],rightDiehighTemp[4],rightDieLowTemp[4],leftDieSetTemp[4],leftDieSetTemp[4];
extern unsigned int leftDiehighTemp[4],leftDieLowTemp[4],curingTemp[4],tiltingUpTime[4],slideCoreTime[4],ejectionOnTime[4],ejectionOffTime[4];
struct  RxDataSet RxDataSetmac[4];
struct  RxWifi_TxDataSet  RxWifi_TxDataSetMac[4];
unsigned char Response_Macid;
//*****************************************************************************************
void TransmitRequestPF()
{		
static unsigned char DataFrame=0x00;
	unsigned int   CRCResult;	 
	unsigned char NoOfByte;
	unsigned char i;
	
	if(!PowerMeterRefresh_Lpc){return;}
	PowerMeterRefresh_Lpc=0;
	if(Response_Macid >4){return;}
	TxPfLocalPtr=	TXPFLocalBuffer;
/*Replay frame*/

			TXPFLocalBuffer[0] = Response_Macid;	
			TXPFLocalBuffer[1] = 0x10;
			TXPFLocalBuffer[2] = 0x0f;	
			TXPFLocalBuffer[3] = 0xa0+Response_Macid;	
			TXPFLocalBuffer[4] = 0x00;	
			TXPFLocalBuffer[5] = 0x0c;
			TXPFLocalBuffer[6] = 0x18;	  	
			TXPFLocalBuffer[7] = (unsigned char)((RxWifi_TxDataSetMac[Response_Macid-1].rightDieSetTemp)>>8)&0x00ff;	
			TXPFLocalBuffer[8] = (unsigned char)(RxWifi_TxDataSetMac[Response_Macid-1].rightDieSetTemp&0x00ff);
			TXPFLocalBuffer[9] = (unsigned char)((RxWifi_TxDataSetMac[Response_Macid-1].rightDiehighTemp)>>8)&0x00ff;	
			TXPFLocalBuffer[10] = (unsigned char)(RxWifi_TxDataSetMac[Response_Macid-1].rightDiehighTemp&0x00ff);
			TXPFLocalBuffer[11] = (unsigned char)((RxWifi_TxDataSetMac[Response_Macid-1].rightDieLowTemp)>>8)&0x00ff;	
			TXPFLocalBuffer[12] = (unsigned char)(RxWifi_TxDataSetMac[Response_Macid-1].rightDieLowTemp&0x00ff);
			TXPFLocalBuffer[13] = (unsigned char)((RxWifi_TxDataSetMac[Response_Macid-1].leftDieSetTemp)>>8)&0x00ff;	
			TXPFLocalBuffer[14] = (unsigned char)(RxWifi_TxDataSetMac[Response_Macid-1].leftDieSetTemp&0x00ff);
			TXPFLocalBuffer[15] = (unsigned char)((RxWifi_TxDataSetMac[Response_Macid-1].leftDiehighTemp)>>8)&0x00ff;	
			TXPFLocalBuffer[16] = (unsigned char)(RxWifi_TxDataSetMac[Response_Macid-1].leftDiehighTemp&0x00ff);
			TXPFLocalBuffer[17] = (unsigned char)((RxWifi_TxDataSetMac[Response_Macid-1].leftDieLowTemp)>>8)&0x00ff;	
			TXPFLocalBuffer[18] = (unsigned char)(RxWifi_TxDataSetMac[Response_Macid-1].leftDieLowTemp&0x00ff);
			TXPFLocalBuffer[19] = (unsigned char)((RxWifi_TxDataSetMac[Response_Macid-1].curingTemp)>>8)&0x00ff;	
			TXPFLocalBuffer[20] = (unsigned char)(RxWifi_TxDataSetMac[Response_Macid-1].curingTemp&0x00ff);
			TXPFLocalBuffer[21] = (unsigned char)((RxWifi_TxDataSetMac[Response_Macid-1].tiltingUpTime)>>8)&0x00ff;	
			TXPFLocalBuffer[22] = (unsigned char)(RxWifi_TxDataSetMac[Response_Macid-1].tiltingUpTime&0x00ff);
			TXPFLocalBuffer[23] = (unsigned char)((RxWifi_TxDataSetMac[Response_Macid-1].slideCoreTime)>>8)&0x00ff;	
			TXPFLocalBuffer[24] = (unsigned char)(RxWifi_TxDataSetMac[Response_Macid-1].slideCoreTime&0x00ff);
			TXPFLocalBuffer[25] = (unsigned char)((RxWifi_TxDataSetMac[Response_Macid-1].ejectionOnTime)>>8)&0x00ff;	
			TXPFLocalBuffer[26] = (unsigned char)(RxWifi_TxDataSetMac[Response_Macid-1].ejectionOnTime&0x00ff);
			TXPFLocalBuffer[27] = (unsigned char)((RxWifi_TxDataSetMac[Response_Macid-1].ejectionOffTime)>>8)&0x00ff;	
			TXPFLocalBuffer[28] = (unsigned char)(RxWifi_TxDataSetMac[Response_Macid-1].ejectionOffTime&0x00ff);
			TXPFLocalBuffer[29] = (unsigned char)((MacSpecStatus_Rst[Response_Macid-1].MacStatus_Rst)>>8)&0x00ff;	
			TXPFLocalBuffer[30] = (unsigned char)(MacSpecStatus_Rst[Response_Macid-1].MacStatus_Rst&0x00ff);
			CRCResult	  = ASCChecksum(TxPfLocalPtr,31);
			TXHeaderElement.CRCCode_Low  =TXPFLocalBuffer[31]=   (unsigned char)CRCResult&0x00ff;
			TXHeaderElement.CRCCode_High =TXPFLocalBuffer[32]=   (unsigned char)(CRCResult>>8)&0x00ff;
			for(i=0;i<=32;i++)
			{
				LPC_ASC0_vSendData (TXPFLocalBuffer[i]);
			}

}	
//*****************************************************************************************
// @Function      void ReceiveStartFunction(void)
//
//-----------------------------------------------------------------------------------------
// @Description   Starts PEC for SIO receive data.Function to receive PF data
//
//-----------------------------------------------------------------------------------------
// @Returnvalue   None
//
//-----------------------------------------------------------------------------------------
// @Parameters    None
//
//-----------------------------------------------------------------------------------------
// @Date          1/21/2016	 
//
//*****************************************************************************************
void RXDECODERS484DATA(void)
{
   unsigned char i;
   if(!RxCompleteU1C0485){return;}
   RxCompleteU1C0485=0;

	   if(0x01 ==ReceivingData[0])
	   {
		   if(0x03 ==ReceivingData[1])
		   {
			   if(56 ==ReceivingData[2])
			   {													
				   RxDataSetmac[0].temperature1_act  	= ((ReceivingData[4]<<8|ReceivingData[5]))/10;
				   RxDataSetmac[0].temperature2_act   	= ((ReceivingData[6]<<8|ReceivingData[7]))/10;
				   RxDataSetmac[0].production 			= (ReceivingData[8]<<8|ReceivingData[9]);
				   //RxDataSetmac[0].rejection   			= (ReceivingData[10]<<8|ReceivingData[11]);	
				   RxDataSetmac[0].TiltDown_Zeit   		= (ReceivingData[12]<<8|ReceivingData[13]);	
				   RxDataSetmac[0].Rst_Responce   		= (ReceivingData[14]<<8|ReceivingData[15]);
			   }
			}
	    }

		if(0x02 ==ReceivingData[0])
	   {
		   if(0x03 ==ReceivingData[1])
		   {
			   if(57 ==ReceivingData[2])
			   {													
				   RxDataSetmac[1].temperature1_act  	= ((ReceivingData[4]<<8|ReceivingData[5]))/10;
				   RxDataSetmac[1].temperature2_act   	= ((ReceivingData[6]<<8|ReceivingData[7]))/10;
				   RxDataSetmac[1].production 			= (ReceivingData[8]<<8|ReceivingData[9]);
				   //RxDataSetmac[1].rejection   			= (ReceivingData[10]<<8|ReceivingData[11]);	
				   RxDataSetmac[1].TiltDown_Zeit   		= (ReceivingData[12]<<8|ReceivingData[13]);	
				   RxDataSetmac[1].Rst_Responce   		= (ReceivingData[14]<<8|ReceivingData[15]);
			   }
			}
	    }

		if(0x03 ==ReceivingData[0])
	   {
		   if(0x03 ==ReceivingData[1])
		   {
			   if(58 ==ReceivingData[2])
			   {													
				   RxDataSetmac[2].temperature1_act  	= ((ReceivingData[4]<<8|ReceivingData[5]))/10;
				   RxDataSetmac[2].temperature2_act   	= ((ReceivingData[6]<<8|ReceivingData[7]))/10;
				   RxDataSetmac[2].production 			= (ReceivingData[8]<<8|ReceivingData[9]);
				   //RxDataSetmac[2].rejection   			= (ReceivingData[10]<<8|ReceivingData[11]);	
				   RxDataSetmac[2].TiltDown_Zeit   		= (ReceivingData[12]<<8|ReceivingData[13]);	
				   RxDataSetmac[2].Rst_Responce   		= (ReceivingData[14]<<8|ReceivingData[15]);
			   }
			}
	    }

		if(0x04 ==ReceivingData[0])
	   {
		   if(0x03 ==ReceivingData[1])
		   {
			   if(59 ==ReceivingData[2])
			   {													
				   RxDataSetmac[3].temperature1_act  	= ((ReceivingData[4]<<8|ReceivingData[5]))/10;
				   RxDataSetmac[3].temperature2_act   	= ((ReceivingData[6]<<8|ReceivingData[7]))/10;
				   RxDataSetmac[3].production 			= (ReceivingData[8]<<8|ReceivingData[9]);
				   //RxDataSetmac[3].rejection   			= (ReceivingData[10]<<8|ReceivingData[11]);	
				   RxDataSetmac[3].TiltDown_Zeit   		= (ReceivingData[12]<<8|ReceivingData[13]);	
				   RxDataSetmac[3].Rst_Responce   		= (ReceivingData[14]<<8|ReceivingData[15]);
			   }
			}
	    }
		Read_requested_data=0;
	
   for(i=0;i<=40;i++)
   {
   	  ReceivingData[i]=0;
   }
}
//*****************************************************************************************
// @Function      unsigned int ASCChecksum(unsigned char*, unsigned char)
//
//-----------------------------------------------------------------------------------------
// @Description   Calculates checksum for array of data per Modbus 2.0
//
//-----------------------------------------------------------------------------------------
// @Returnvalue   unsigned int calculated checksum
//
//-----------------------------------------------------------------------------------------
// @Parameters    unsigned int* Pointer to data for checksum calculation
//				  unsigned char Number of bytes in array
//
//-----------------------------------------------------------------------------------------
// @Date          7/24/2009
//
//*****************************************************************************************
unsigned int ASCChecksum(unsigned char *ASCSrc, unsigned char NoOfBytes)						 
{
   	unsigned char i, CheckSumBytes;
 	unsigned char CRCRegLow = 0xff;
	unsigned char CRCRegHigh = 0xff;
	unsigned char CRCIndex;	

	CheckSumBytes = NoOfBytes;
   	for(i=0;i < CheckSumBytes;i++)
   	{
	    CRCIndex = CRCRegLow ^ *ASCSrc++; 				//TransmittingData[i];
		CRCRegLow = CRCRegHigh ^ CRCArrayHigh[CRCIndex];
		CRCRegHigh = CRCArrayLow[CRCIndex];

	}  	
	return (CRCRegHigh << 8 | CRCRegLow );                
}

// USER CODE BEGIN (U0C0_General,10)
void Recive485_service(unsigned char receivedata)
{
	static unsigned char count=0;
	switch(U1C0_count)
	{
		case 0:
		 ReceivingData[0] = receivedata;
		 if((ReceivingData[0]==0x01)||(ReceivingData[0]==0x02)||(ReceivingData[0]==0x03)||(ReceivingData[0]==0x04))
		 {
		 	U1C0_count++;
		 }
		 else
		 {
		    U1C0_count=0;
		 }
		break;
		case 1:
		 ReceivingData[1]=receivedata;
		 if(ReceivingData[1]==0x03)
		 {
		 	U1C0_count++;
		 }
		 else
		 {
		  	U1C0_count=0;
		 }
		break;
		case 2:
			ReceivingData[2]= receivedata; /*56 to 59*/
			Response_Macid=ReceivingData[2]-55;
			U1C0_count++;
		break;
		case 3:
		 ReceivingData[3]=receivedata;	/*total number of bytes*/
		 U1C0NoOfBytes = ReceivingData[3];
		 U1C0_count++;
		 count=4;
		break;
		case 4: //Receiving data bytes
		 ReceivingData[count]  = receivedata;
		 count++;
		 U1C0NoOfBytes--;
		 if(U1C0NoOfBytes==0)
		 {
		   U1C0_count++;
		   count =0;
		 }
		break;
		case 5:
		 U1C0Rx_Checksumlow=receivedata;
		 U1C0_count++;
		 RxCompleteU1C0485 = 1;
		break;
		case 6:
		 U1C0Rx_Checksumhigh=receivedata;	
		 U1C0_count=0;
		 RxCompleteU1C0485 = 1;
		 PowerMeterRefresh_Lpc=1;
		break;
		default:
		break;  
	}
}