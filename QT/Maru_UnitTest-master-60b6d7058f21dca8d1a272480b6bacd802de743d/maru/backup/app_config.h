#ifndef _APP_CONFIG_H_
#define _APP_CONFIG_H_


#define VM_TYPE_ROVEN				1
#define VM_TYPE_LOTTE_MULTI		2
#define VM_TYPE_LOTTE_LVC440		3
#define VM_TYPE_YOUNG_GATE_MULTI		4
#define VM_TYPE_ROBOQ				10

#define TID_LEN		64
#define SSID_LEN		32
#define IP_LEN		32
#define AP_TID_LEN	32
#define TIME_LEN	14

typedef struct _VM_AP
{		
	unsigned char SSID[SSID_LEN];
	unsigned char PW[SSID_LEN];
	unsigned char SIP[IP_LEN];			// STATIC IP
	unsigned char GIP[IP_LEN];			// GATEWAT IP
	unsigned char SMASK[16];		// SUBNET MAST 
	unsigned char EID[SSID_LEN];
	unsigned char EPW[SSID_LEN];
	unsigned char AuthMode;
	unsigned char Encrypt;
	unsigned char KeyDefault;
	unsigned char KeyLength;
	unsigned char KeyFormat;
	unsigned char DHCP;
	unsigned char Channel[4];
	unsigned char RSSI[4];
	unsigned char BSSID[16];
	unsigned char Change;
	unsigned char TID[SSID_LEN];
	unsigned char rfu[32];
} VM_AP;

typedef struct _VM_CONFIG
{		
	unsigned char Version[2][32];
	unsigned char ClrTime[16];
	unsigned char TID[SSID_LEN];			// Terminal ID	
	unsigned char CATID[SSID_LEN];			// CAT ID	
	unsigned char VIP1[IP_LEN];			// VAN IP MAIN
	unsigned char VIP2[IP_LEN];			// VAN IP SUB
	unsigned char VIP3[IP_LEN];			// VAN IP SUB
	unsigned char VIP4[IP_LEN];			// VAN IP SUB
	unsigned char VIP5[IP_LEN];			// VAN IP SUB
	unsigned char TIP1[IP_LEN];			// TMS IP MAIN
	unsigned char TIP2[IP_LEN];			// TMS IP SUB
	unsigned char TIP3[IP_LEN];			// TMS IP SUB
	unsigned char TIP4[IP_LEN];			// TMS IP SUB
	unsigned char TIP5[IP_LEN];			// TMS IP SUB
	unsigned char TYPE[8];			// 
	unsigned char Tm;
	unsigned char Mb;
	unsigned char Kb;
	unsigned char Dg;
	unsigned char Kc;
	unsigned char CWA;			// 
	unsigned char MODA;			// 
	unsigned char CWB;			// 
	unsigned char MODB;			// 
	unsigned char Eb;
	unsigned char Ebm;
	unsigned char Usam;
	unsigned char lotte_sam;
	unsigned char rfu[20];			
															
	VM_AP ap;	
	
	char ApprovalNumber[36];
	
	char KTC;
	char Cash;
	
	unsigned char RailPlus;
	unsigned char TID1[SSID_LEN];			// Terminal ID	
	unsigned char TID2[SSID_LEN];			// Terminal ID	
	
} VM_CONFIG;
extern VM_CONFIG g_VmConfig;

typedef struct{
	unsigned int amount;
	unsigned char text[16];
} BUTTON;
extern BUTTON button[4], btn_select;

typedef struct{
	/* Amount Button Info	*/
	BUTTON Button[4];

	/* Pulse Mode Info		*/
	unsigned long PulseMode;
	unsigned long PulseLenMode;
	unsigned long PulseLength; 
	unsigned long PulseCoinMode;
	unsigned long CashMode;	
	unsigned long DisplayMode;
	int BaudRate, ParityBit, StopBit;
	unsigned long PulseHwVersion;
	unsigned long CompleteTime;
	unsigned long MaxAmount;

	/* Amount Button Info	*/
	BUTTON btnTime[4];

	/* Selling Time */
	unsigned char StartTime[32];
	unsigned char StopTime[32];
	unsigned char SaleTimeFlag;
	unsigned char CashOnlyMode;
	unsigned char CancelMode;
	
} Amount_Info;
extern Amount_Info AmountInfo, AmountInfoBakup;

extern DEVICE_INFO devinfo;

extern int g_VmType;
extern int g_VmTypeGate;

extern char* DisplayManufacturer(int clear);
extern void SystemReboot(void);
extern void VM_GetConfigRead(void);
extern int VM_GetConfigVersion(unsigned char *pVer);
extern void VM_SetConfigTID(unsigned char *pTID, int nLength);
extern int VM_GetConfigTID(unsigned char *pTID);
extern void VM_SetConfigTID1(unsigned char *pTID, int nLength);
extern int VM_GetConfigTID1(unsigned char *pTID);
extern void VM_SetConfigTID2(unsigned char *pTID, int nLength);
extern int VM_GetConfigTID2(unsigned char *pTID);
extern void VM_SetConfigCatID(unsigned char *pTID, int nLength);
extern int VM_GetConfigCatID(unsigned char *pTID);

extern void VM_SetKTC(char nKTC);
extern char VM_GetKTC(void);
extern char* VM_GetKtcApprovalNumber(void);
extern void VM_SetCashReceipt(char nCash);
extern char VM_GetCashReceipt(void);

extern char* VM_GetManufacturer(void);
extern int VM_GetLotteMachineInfo(void);
extern int VM_GetLotteGateBankMachineInfo(void);
extern int VM_GetLotteMultiMachineInfo(void);
extern int VM_GetYoungSystemMachineInfo(void);
extern int VM_GetLotteCoffeMachineInfo(char * p_vm_model);
extern int VM_GetKtcMachineInfo(void);
extern int VM_GetLotteGate2UbcnMachineInfo(char * p_vm_model, unsigned char rdata, unsigned char rlen);
extern int VM_GetLotteSmartMachineInfo(void);
extern int VM_GetRovenJanpenMachineInfo(void);

extern void VM_MachineInfoDisplay(int nFlush);
extern void VM_SetApprovalAmount(unsigned long amount);
extern void VM_AddApprovalAmount(unsigned long amount);
extern unsigned long VM_GetApprovalAmount(void);

extern int checkKeyExchangeCared(char *pCard);
extern int checkEbayManagerCardFree(char *pCard);
extern int checkEbayManagerCardFreeRead(void);
extern void checkEbayManagerCardFreeClear(void);
extern int checkTouchAndGoMode(void);
extern int checkMdbMode(void);
extern int checkKtngMode(void);
extern int checkCashOnlyMode(void) ;
extern int checkEmartMode(void);
extern int checkEmartEverydayMode(void) ;
extern int checkOfflineMode(void);
extern int checkSerialSkipMode(void);
extern void checkKernelVer(void);
extern int checkMultiMachine(void) ;
extern int checkSaleInfoMachine(void);
extern int checkButton1(void);
extern int checkButton2(void);

extern void dispMsgCashOnly(void);
extern int dispDataTrasmit(void);
extern void dispMsgOtherCard(void);
extern void dispMsgPushButton(void);
extern void dispMsgCupJam(void);
extern void dispMsgGoodsGetOut(void);
extern void dispMsgWaitMoment(void);
extern void dispMsgVmApproval(void);

extern void SetIC(int ic_reader);
extern int GetIC(void);

extern void SetUbcn(char nUbcn);
extern char GetUbcn(void);

extern void SetPrinter(unsigned char on_off);
extern char GetPrinter(void);


extern void SetCreditRange(unsigned char visa);
extern int GetCreditRange(void);

extern unsigned int GetButtonTimeout(int btn);
extern unsigned int GetButtonAmount(int btn);

extern unsigned long GetSaleStartTime(void);
extern unsigned long GetSaleStopTime(void);

extern void initMsgSignal(void);
extern int signalCheck(void);
extern void signalSet(int sig);

extern int signalCheckException(void);
#endif	// _APP_CONFIG_H_



