 struct  RxDataSet
{
	unsigned int temperature1_act;
	unsigned int temperature2_act;
	unsigned int production;
	unsigned int rejection;
	unsigned int TiltDown_Zeit;
	unsigned int Rst_Responce;
};

struct RxWifi_TxDataSet
{
   unsigned int rightDieSetTemp;
   unsigned int rightDiehighTemp;
   unsigned int rightDieLowTemp;
   unsigned int leftDieSetTemp;
   unsigned int leftDiehighTemp;
   unsigned int leftDieLowTemp;
   unsigned int curingTemp;
   unsigned int tiltingUpTime;
   unsigned int slideCoreTime;
   unsigned int ejectionOnTime;
   unsigned int ejectionOffTime;
};

struct machinestatus
{
	unsigned int MacStatus_Rst;
};

struct loc_rejection
{
   unsigned int rejection;
};
extern struct  RxDataSet RxDataSetmac[4];
extern struct  RxWifi_TxDataSet  RxWifi_TxDataSetMac[4];
extern struct  machinestatus MacSpecStatus_Rst[4];
extern struct  loc_rejection  loc_rejectionCount[4];