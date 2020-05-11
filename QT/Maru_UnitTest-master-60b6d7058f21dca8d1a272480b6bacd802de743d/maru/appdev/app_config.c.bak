/*******************************(C) COPYRIGHT 2016 UBC Co., LTD.****************************************
** program		  : lib_config.c
** processor 	  : s3c6410
** compiler		  : armv6 gcc version 4.3.1
** program BY	  : kkryu
** date			  : 2016.5.31
** copy right	  : UBC Co., LTD
******************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/utsname.h>

#include <sys/time.h>
#include <sys/timeb.h>
#include <sys/ioctl.h> 		/* for ioctl */
#include <sys/mman.h> 	/* for mmap */
#include <sys/poll.h>

#include <fcntl.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>

#include <dev/lib_led.h>
#include <dev/lib_lcd.h>

#include "typedef.h"
#include "app_config.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define APP_CONFIG_DBG 	0
#if (APP_CONFIG_DBG==1)
# define	PRINTF(fmt,args...) 	printf(fmt, ##args)
# define	PRINTHEX(fmt,args...)  	PrintHex(fmt, ##args)
# define	HEXDUMP(fmt,args...)  	HEX_Dump(fmt, ##args)
#else
# define    PRINTF(a...)    (void)(0)
# define    PRINTHEX(a...)  (void)(0)
# define    HEXDUMP(a...)   (void)(0)
#endif

#define EBAY_MANAGER_CARE_ENABLE 0

#define LINE_LEN		600
#define ATTR_KEY_LEN	10
#define ATTR_VAL_LEN	50
#define VAL_LEN			500

#define KTC_NUMBER_KOCES	"#####UVM300A1211################"

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
DEVICE_INFO devinfo;
VM_CONFIG g_VmConfig;	
int g_VmType=0;
unsigned long g_ApprovedAmount=0;
/* Private function prototypes -----------------------------------------------*/

unsigned char HW_Version[16]    = "V1.2";
unsigned char FW_Version[16]    = "BG32";		// GATE_OLD_VERSION
unsigned char FW_DateTime[16] = "19:05:20:12";	// YY:MM:DD:hh

#define NAME_MAX_CNT 			32 

const char *Serial_title[NAME_MAX_CNT]={
	LCD_LINE_LOCKER,		
	LCD_LINE_NEWWORLD,
	LCD_LINE_HAPPYNAME,
	LCD_LINE_CAFETODAY,
	LCD_LINE_TOUCHCAFE,
	LCD_LINE_DONGHAK,
	LCD_LINE_WITHME,
	LCD_LINE_NESPRESSO,
	LCD_LINE_RIDE,
	LCD_LINE_JUNE,
	LCD_LINE_ENTERSYS,
	LCD_LINE_MIRE,
	LCD_LINE_ABCO,
	LCD_LINE_GPS,
	LCD_LINE_SUNGHYUN,
	LCD_LINE_OHJIN,
	LCD_LINE_SMCKOREA,
	LCD_LINE_LOHAS,
	LCD_LINE_S_LOCKER,
	LCD_LINE_AGD_FNC,
	LCD_LINE_GK_VEND,
	LCD_LINE_NEWWORLD,
	LCD_LINE_SAECO,
	LCD_LINE_SANDEN,
	LCD_LINE_SHILAX,
	LCD_LINE_7_ELEVEN,
	LCD_LINE_CARD_INSERT,
	LCD_LINE_LOTTI,
	LCD_LINE_NESTLE,
	LCD_LINE_LAB101,
	LCD_LINE_UBCN,
	LCD_LINE_GS25,
};

char* DisplayManufacturer(int clear)
{		
	char *pData;
	
	switch(g_GlobalInfo.Company) {
	case COMPANY_COIN :
	case COMPANY_LOCKER :
		switch(AmountInfo.DisplayMode) {
			case Gs25GateMode :  pData = (char*)Serial_title[31];	break;		
			case UbcnEthMode :  pData = (char*)Serial_title[30];	break;		
			case Lab101GateMode: pData = (char*)Serial_title[29];	break;			
			case MdbNestleMode : pData = (char*)Serial_title[28];	break;			
			case MdbLottiMode    : pData = (char*)Serial_title[27];	break;
			case CardInsertMode : 	pData = (char*)Serial_title[26]; break;
			case SevenElevenGateEthMode : pData = (char*)Serial_title[25]; break;
			case ShilaxMode_1 : pData = (char*)Serial_title[24]; break;
			case MdbSandenMode : pData = (char*)Serial_title[23]; break;
			case SaecoMode : 
			case SaecoCoinMode_1 : 
			case SaecoCoinMode_2 : 
			case SaecoCoinEmart24Mode : 
				pData = (char*)Serial_title[22];
				break;
			case NewWorldVendingEthMode : pData = (char*)Serial_title[21]; break;
			case GkVendingsMode :			pData = (char*)Serial_title[20];			break;
			case AgdfncMode :			pData = (char*)Serial_title[19];			break;
			case SlockerEthMode :			pData = (char*)Serial_title[18];			break;
			case LoHasEthMode :			pData = (char*)Serial_title[17];			break;
			case SmcKoreaEthMode :			pData = (char*)Serial_title[16];			break;
			case MdbOhJinMode :			pData = (char*)Serial_title[15];			break;			
			case SunghyunEthMode :			pData = (char*)Serial_title[14];			break;			
			case KlcMode_Serial :			pData = (char*)Serial_title[13];			break;
			case AbkoEthMode :			pData = (char*)Serial_title[12];			break;
			case MdbMireVendigMode :				pData = (char*)Serial_title[11];			break;
			case EnterSolutionMode :			pData = (char*)Serial_title[10];			break;
			case JuneSystemMode :			pData = (char*)Serial_title[9];			break;
			case BusRideMode :			pData = (char*)Serial_title[8];			break;
			case MdbNespressoMode :			pData = (char*)Serial_title[7];			break;
			case Emart24GateMode :				pData = (char*)Serial_title[6];			break;
			case MdbDonghakMode :			pData = (char*)Serial_title[5];			break;
			case MdbWcmNectaMode :			pData = (char*)Serial_title[4];			break;		
			case CafeTodayMode :			pData = (char*)Serial_title[3];			break;
			case HappyNameMode : pData = (char*)Serial_title[2]; break;
			case NewWorldVendingMode : pData = (char*)Serial_title[1]; break;
			default :			pData = (char*)Serial_title[0];			break;
		}
		break;
	default :
		pData = VM_GetManufacturer();
		break;
	}

	if(clear != 0x80) LCD_Display(LCD_SORT_CENTER, 0, clear, pData);

	return pData;
}


void SystemReboot(void)
{
	if(0x100 != LED_WatchdogStop())	{
		printf("\r\nreboot\r\n");
		system("reboot\n");	
	}
}

void VM_GetConfigRead(void)
{
	FILE *fp = NULL;		
	
	fp = fopen("/usr/rfpinpad/data/Config.dat", "r");
	if(fp!=NULL) {
		fread(&g_VmConfig, sizeof(VM_CONFIG), 1, fp);	
		fclose(fp);
	}
}

int VM_GetConfigVersion(unsigned char *pVer)
{
	FILE *fp = NULL;	
	
	
	fp = fopen("/usr/rfpinpad/data/Config.dat", "r");
	if(fp!=NULL) {
		fread(&g_VmConfig, sizeof(VM_CONFIG), 1, fp);	
		fclose(fp);
	}
	
	strcpy(pVer, FW_Version);		
	
	if( memcmp(&g_VmConfig.Version[0], FW_Version, strlen(FW_Version)) || memcmp(&g_VmConfig.Version[1], HW_Version, strlen(HW_Version)) ) {
		
		strcpy((char *)&g_VmConfig.Version[0],  (char *)FW_Version);
		strcpy((char *)&g_VmConfig.Version[1],  (char *)HW_Version);
		
		fp = fopen("/usr/rfpinpad/data/Config.dat", "w");
		if(fp!=NULL) {
			fwrite(&g_VmConfig, sizeof(VM_CONFIG), 1, fp);	
			fclose(fp);
		}
	}	
	
	fp = fopen("/usr/rfpinpad/data/Config.dat", "r");
	if(fp!=NULL) {
		fread(&g_VmConfig, sizeof(VM_CONFIG), 1, fp);	
		fclose(fp);
	}	
	
	memcpy(devinfo.version,   g_VmConfig.Version, sizeof(devinfo.version));
	strcpy(devinfo.date_time, FW_DateTime);
	
	strcpy(pVer+5, FW_DateTime);		
		
	return sizeof(g_VmConfig.Version);
}

void VM_SetConfigTID(unsigned char *pTID, int nLength)
{
	FILE *fp = NULL;
	
	memcpy(g_VmConfig.TID, pTID, nLength);
	g_VmConfig.TID[nLength] = 0;

	fp = fopen("/usr/rfpinpad/data/Config.dat", "wb");
	if(fp!=NULL) {
		fwrite(&g_VmConfig, sizeof(VM_CONFIG), 1, fp);	
		fclose(fp);
	}
}

void VM_SetConfigTID1(unsigned char *pTID, int nLength)
{
	FILE *fp = NULL;
	
	memcpy(g_VmConfig.TID1, pTID, nLength);
	g_VmConfig.TID1[nLength] = 0;

	fp = fopen("/usr/rfpinpad/data/Config.dat", "wb");
	if(fp!=NULL) {
		fwrite(&g_VmConfig, sizeof(VM_CONFIG), 1, fp);	
		fclose(fp);
	}
}

void VM_SetConfigTID2(unsigned char *pTID, int nLength)
{
	FILE *fp = NULL;
	
	memcpy(g_VmConfig.TID2, pTID, nLength);
	g_VmConfig.TID2[nLength] = 0;

	fp = fopen("/usr/rfpinpad/data/Config.dat", "wb");
	if(fp!=NULL) {
		fwrite(&g_VmConfig, sizeof(VM_CONFIG), 1, fp);	
		fclose(fp);
	}
}


int VM_GetConfigTID(unsigned char *pTID)
{
	FILE *fp = NULL;
	
	fp = fopen("/usr/rfpinpad/data/Config.dat", "rb");
	if(fp!=NULL) {
		fread(&g_VmConfig, sizeof(VM_CONFIG), 1, fp);	
		fclose(fp);

		strcpy(pTID, g_VmConfig.TID);
		return strlen(g_VmConfig.TID);
	}
	pTID[0]=0;
	return 0;
}

int VM_GetConfigTID1(unsigned char *pTID)
{
	FILE *fp = NULL;
	
	fp = fopen("/usr/rfpinpad/data/Config.dat", "rb");
	if(fp!=NULL) {
		fread(&g_VmConfig, sizeof(VM_CONFIG), 1, fp);	
		fclose(fp);

		strcpy(pTID, g_VmConfig.TID1);
		return strlen(g_VmConfig.TID1);
	}
	pTID[0]=0;
	return 0;
}

int VM_GetConfigTID2(unsigned char *pTID)
{
	FILE *fp = NULL;
	
	fp = fopen("/usr/rfpinpad/data/Config.dat", "rb");
	if(fp!=NULL) {
		fread(&g_VmConfig, sizeof(VM_CONFIG), 1, fp);	
		fclose(fp);

		strcpy(pTID, g_VmConfig.TID2);
		return strlen(g_VmConfig.TID2);
	}
	pTID[0]=0;
	return 0;
}


void VM_SetConfigCatID(unsigned char *pTID, int nLength)
{
	FILE *fp = NULL;
	
	memcpy(g_VmConfig.CATID, pTID, nLength);
	g_VmConfig.CATID[nLength] = 0;

	fp = fopen("/usr/rfpinpad/data/Config.dat", "w");
	if(fp!=NULL) {
		fwrite(&g_VmConfig, sizeof(VM_CONFIG), 1, fp);	
		fclose(fp);
	}
}

int VM_GetConfigCatID(unsigned char *pTID)
{
	FILE *fp = NULL;
	
	fp = fopen("/usr/rfpinpad/data/Config.dat", "r");
	if(fp!=NULL) {
		fread(&g_VmConfig, sizeof(VM_CONFIG), 1, fp);	
		fclose(fp);

		strcpy(pTID, g_VmConfig.CATID);
		return strlen(g_VmConfig.CATID);
	}
	pTID[0]=0;
	return 0;
}

void VM_SetKTC(char nKTC)
{
	FILE *fp = NULL;
	
	fp = fopen("/usr/rfpinpad/data/Config.dat", "r");
	if(fp!=NULL) {
		fread(&g_VmConfig, sizeof(VM_CONFIG), 1, fp);	
		fclose(fp);
	}
	
	g_VmConfig.KTC = nKTC;
	strcpy(g_VmConfig.ApprovalNumber, KTC_NUMBER_KOCES);
	
	fp = fopen("/usr/rfpinpad/data/Config.dat", "w");
	if(fp!=NULL) {
		fwrite(&g_VmConfig, sizeof(VM_CONFIG), 1, fp);	
		fclose(fp);
	}
}

char VM_GetKTC(void)
{
	return g_VmConfig.KTC;
}

char* VM_GetKtcApprovalNumber(void)
{
	strcpy(g_VmConfig.ApprovalNumber, KTC_NUMBER_KOCES);
	return g_VmConfig.ApprovalNumber;
}

void VM_SetCashReceipt(char nCash)
{
	FILE *fp = NULL;
	
	fp = fopen("/usr/rfpinpad/data/Config.dat", "r");
	if(fp!=NULL) {
		fread(&g_VmConfig, sizeof(VM_CONFIG), 1, fp);	
		fclose(fp);
	}
	
	g_VmConfig.Cash = nCash;
	
	fp = fopen("/usr/rfpinpad/data/Config.dat", "w");
	if(fp!=NULL) {
		fwrite(&g_VmConfig, sizeof(VM_CONFIG), 1, fp);	
		fclose(fp);
	}
}


char VM_GetCashReceipt(void)
{
#if 0
	FILE *fp = NULL;
	
	fp = fopen("/usr/rfpinpad/data/Config.dat", "r");
	if(fp!=NULL) {
		fread(&g_VmConfig, sizeof(VM_CONFIG), 1, fp);	
		fclose(fp);
	}
	else {
		g_VmConfig.KTC = CASH_RECEIPT_NONE;
	}
#endif	
	return g_VmConfig.Cash;
}

// 자동 마감 처리 안함.
int VM_GetLotteMachineInfo(void)
{
	if(    strstr(g_GlobalInfo.VmModel, VM_LOTTE_442) || 
		strstr(g_GlobalInfo.VmModel, VM_LOTTE_522) || 
		strstr(g_GlobalInfo.VmModel, VM_LOTTE_531)) 
	{
		if(strstr(g_GlobalInfo.VmModel, VM_ROVEN_5224)) {
			return 0;
		}
		return 1;
	}
	return 0;	
}

int VM_GetYoungSystemMachineInfo(void)
{
	if( strstr(g_GlobalInfo.VmModel, VM_YONG_SYSTEM_COCA) ) {
		return 1;
	}
	
	return 0;	
}

int VM_GetLotteGateBankMachineInfo(void)
{
	switch(g_GlobalInfo.VmUbcn) {
	case VM_BLAND_LOTTE_GATEBANK :
	case VM_BLAND_MAINTECH_GATEBANK :
	case VM_BLAND_LOTTE_GATEBANK_MULTI : 
		return 1;
	}
	return 0;	
}

int VM_GetLovenMachineInfo(void)
{	
	if( 	strstr(g_GlobalInfo.VmModel, VM_ROVEN_5224) || 	
		strstr(g_GlobalInfo.VmModel, VM_ROVEN_5720) ) {
		return 1;
	}
	return 0;	
}


int VM_GetLotteMultiMachineInfo(void)
{	
	if( 	strstr(g_GlobalInfo.VmModel, VM_LOTTE_480) ||
		strstr(g_GlobalInfo.VmModel, VM_LOTTE_482)) {
		return 1;
	}
	return 0;	
}

int VM_GetLotteSmartMachineInfo(void)
{	
	if( 	strstr(g_GlobalInfo.VmModel, "LVK") ) {
		return 1;
	}
	return 0;	
}

int VM_GetRovenJanpenMachineInfo(void)
{	
	if(strstr(g_GlobalInfo.VmModel, "RVM")) 
	{	
 		if(strstr(g_GlobalInfo.VmModel, "3342") || strstr(g_GlobalInfo.VmModel, "3442") ) {
			return 1;
		}
	}
	return 0;	
}


int VM_GetLotteCoffeMachineInfo(char * p_vm_model)
{
	if(   strstr(p_vm_model, VM_LOTTE_COFFEE_2140) ||
		strstr(p_vm_model, VM_LOTTE_COFFEE_3112) ||
		strstr(p_vm_model, VM_LOTTE_COFFEE_3142) || 
		strstr(p_vm_model, VM_LOTTE_COFFEE_6112) || 
		strstr(p_vm_model, VM_LOTTE_COFFEE_6131) ||
		strstr(p_vm_model, VM_LOTTE_COFFEE_6132) ||
		strstr(p_vm_model, VM_LOTTE_COFFEE_6140) ||
		strstr(p_vm_model, VM_LOTTE_COFFEE_6143) ||
		strstr(p_vm_model, VM_LOTTE_COFFEE_6450) ||
		strstr(p_vm_model, VM_LOTTE_COFFEE_6460)  ) 
	{
		return 1;
	}

	return 0;
}

int VM_GetLotteGate2UbcnMachineInfo(char * p_vm_model, unsigned char rdata, unsigned char rlen)
{	
	// LVM-360CIGAR	
	if( strstr(p_vm_model, "LVM-360CIGAR") ) {
		return 1;
	}
	else
	if(rlen==8) {
		if(rdata == 'L') {
			return 1;
		}
		else
		if(rdata == 'K') {
			return 2;
		}
	}

	return 0;
}


int VM_KtcMachineException(void)
{
	if(   	VM_GetLotteMachineInfo() ||
		VM_GetLotteCoffeMachineInfo(g_GlobalInfo.VmModel) ||
		VM_GetLovenMachineInfo() ) 
	{
		return 1;
	}
	return 0;
}

char* VM_GetManufacturer(void)
{
	return devinfo.company;	
}

// return 0 -> emv
// return 1 -> payon, msr 우선거래
// return 2 -> payon, ic    우선거래
// return 3 -> ic, msr 우선거래
// return 4 -> emv, ic 우선거래
int VM_GetKtcMachineInfo(void)
{	
	if(GetCreditRange() == CREDIT_VISA_ONLY) 
	{
		return 0;
	}
	
	switch(g_GlobalInfo.Company) {	
	case COMPANY_LOCKER :
	case COMPANY_COIN :
		switch(g_GlobalInfo.DisplayMode) {
		case Lab101GateMode :
		case Gs25GateMode :
		case Emart24GateMode :
			return 3;
		case MdbWcmNectaMode :
		case MdbMireVendigMode :
			return 1;
		case MdbSandenMode :
		case MdbOhJinMode :
		case MdbNestleMode :
		case MdbLottiMode :
		case CardInsertMode :
		case SaecoCoinEmart24Mode :
			return 2;
		case HappyNameMode :
		case UbcnEthMode :
		case AbkoEthMode :
		case RamenMode_1 :
		case RamenMenMode_1 :
		case RamenDoctorMode_1 :
		case RamenDoctorMode_2 :
		case Ramen2SofoodMode_1 :
		case Ramen2SofoodMode_2 :
		case SongMode_1 :
		case GameMode_1 :
		case PCROOM_1 :
		case KlcMode_1 :
		case KlcMode_2 :
		case PohasMode :
		case GkVendingsMode :
		case MdbNespressoMode :
			return 4;
		}
		return 0;
	case COMPANY_EMART24_KTNG :
	case COMPANY_KTNG :	
	case COMPANY_MINISTOP_KTNG :
	case COMPANY_EMART_EVERYDAY_KTNG :
		return 2;
	case COMPANY_DONGA :
	case COMPANY_PHOENIX :
	case COMPANY_LOTTE :
	case COMPANY_COCA :
	case COMPANY_KSCC :
	case COMPANY_KORAIL :
	case COMPANY_DONGHAK :		
	case COMPANY_UBCN :
	case COMPANY_EDWORDS :
	case COMPANY_WELSTORY :
	case COMPANY_MINIGOLD :
	case COMPANY_EMART24 :	
	case COMPANY_KORAIL_TRIP :
	case COMPANY_MINISTOP :
//		if(VM_GetLotteMultiMachineInfo()) return 0;		
		return 2;
	default :
		break;
	}
		
	return 0;
}

void VM_MachineInfoDisplay(int nFlush)
{	
	unsigned char buf[16];
	
	VM_GetConfigTID(buf);
	
	devinfo.msg_signal=0;
		
	if(nFlush) {
		LCD_Display(LCD_SORT_CENTER, 0, 1, VM_GetManufacturer());   
		LCD_Display(LCD_SORT_CENTER, 2, 1, "");   
	}
	
	LCD_Display(LCD_SORT_CENTER, 4, 1, VM_GetMachineInfo());
	LCD_Display(LCD_SORT_CENTER, 6, 1, buf);
	
	if(checkEmartMode()) LCD_PageClear(4);
}

void VM_SetApprovalAmount(unsigned long amount)
{
	g_ApprovedAmount = amount;
}

void VM_AddApprovalAmount(unsigned long amount)
{
	g_ApprovedAmount += amount;
}

unsigned long VM_GetApprovalAmount(void)
{	
	return g_ApprovedAmount;
}

int checkKeyExchangeCared(char *pCard)
{
	if(    !memcmp(pCard, "3562974546711763", 16) ||
		 !memcmp(pCard, "379183692627869", 15) ||
		 !memcmp(pCard, "5409260180404077", 16) ||
		 !memcmp(pCard, "6258040025178064", 16) ||
		 !memcmp(pCard, "6556032173042407", 16) ||
		!memcmp(pCard, "5243356182127607", 16) ) 
	{
		return -1;
	}

	return 0;
}	


int checkEbayManagerCardFree(char *pCard)
{

	#if (EBAY_MANAGER_CARE_ENABLE == 1)
	{
		FILE *fp = NULL;
		char tmp;
		
		if(    !memcmp(pCard, "\x10\x19\x16\x00\x37\x28\x40\x49", 8) ||
			 !memcmp(pCard, "\x10\x19\x16\x00\x37\x28\x49\x99", 8) ||
			 !memcmp(pCard, "\x10\x19\x16\x00\x37\x28\x38\x27", 8) ||
	 		 !memcmp(pCard, "\x10\x19\x16\x00\x37\x28\x40\x31", 8) 	) 
		{

		
			fp = fopen("/usr/rfpinpad/data/ebay_free_flag.dat", "wb");
			if(fp!=NULL) {
				tmp = 'O';
				fwrite(&tmp, sizeof(char), 1, fp);	
				fclose(fp);

				play_check_sound("sound/one.wav");	
				sleep(2);
				SystemReboot();			
			}		
			return -1;
		}
			
		if(    !memcmp(pCard, "\x10\x10\x01\x01\x18\x85\x89\x27", 8) ||
			 !memcmp(pCard, "\x10\x10\x01\x01\x30\x81\x93\x45", 8) ||
			 !memcmp(pCard, "\x10\x10\x01\x01\x30\x81\x93\x29", 8) ||
	 		 !memcmp(pCard, "\x10\x10\x01\x01\x30\x81\x93\x37", 8) 	) 
		{

		
			fp = fopen("/usr/rfpinpad/data/ebay_free_flag.dat", "wb");
			if(fp!=NULL) {
				tmp = 'X';
				fwrite(&tmp, sizeof(char), 1, fp);	
				fclose(fp);

				play_check_sound("sound/two.wav");	
				sleep(2);
				SystemReboot();			
			}		
			return -1;
		}
	}
	#endif
	
	return 0;
}	

int checkEbayManagerCardFreeRead(void)
{
	char tmp=0;
	
	#if (EBAY_MANAGER_CARE_ENABLE == 1)
	{
		FILE *fp = NULL;
		
		fp = fopen("/usr/rfpinpad/data/ebay_free_flag.dat", "rb");
		if(fp!=NULL) {
			fread(&tmp, sizeof(char), 1, fp);	
			fclose(fp);		
		}		
	}
	#endif
	
	return tmp == 'O' ? 0 : -1;	
}	

void checkEbayManagerCardFreeClear(void)
{
	FILE *fp = NULL;
	char tmp=0;

	if(checkEbayManagerCardFreeRead()==0) {	
		fp = fopen("/usr/rfpinpad/data/ebay_free_flag.dat", "wb");
		if(fp!=NULL) {
			tmp = 'X';
			fwrite(&tmp, sizeof(char), 1, fp);	
			fclose(fp);
		}
	}
}	

int checkMultiMachine(void) 
{	
	if( !strstr(g_GlobalInfo.VmModel, "RVM") || !strstr(g_GlobalInfo.VmModel, "CVM") ||
		!strstr(g_GlobalInfo.VmModel, "LVM") ||!strstr(g_GlobalInfo.VmModel, "LVE") ) 
	{
		return 1;
	}
	return 0;
}

int checkTouchAndGoMode(void) 
{	
	switch(g_GlobalInfo.Company) {	
	case COMPANY_LOCKER :
	case COMPANY_DONGHAK :
		switch(AmountInfo.DisplayMode) {
		case SaecoCoinMode_1 :
		case Lab101GateMode :
	 	case Gs25GateMode :
	 	case Emart24GateMode :
		case BusRideMode :
		case EnterSolutionMode :
			return 1;
		}
		break;	
	}
	return 0;
}

int checkMdbMode(void) 
{	
	switch(g_GlobalInfo.Company) {	
	case COMPANY_LOCKER :
	case COMPANY_DONGHAK :
		switch(AmountInfo.DisplayMode) {
	 	case MdbWcmNectaMode :
		case MdbNespressoMode :
		case MdbDonghakMode :
		case MdbMireVendigMode :
		case MdbOhJinMode :
		case MdbNestleMode :
		case MdbLottiMode :
		case MdbSandenMode :
			return 1;
		}
		break;	
	}
	return 0;
}

int checkSerialSkipMode(void)
{
	switch(g_GlobalInfo.Company) {	
	case COMPANY_LOCKER :
	case COMPANY_DONGHAK :
		switch(AmountInfo.DisplayMode) {
	 	case AgdfncMode :
			return 1;
		}
		break;	
	}
	return 0;

}

int checkKtngMode(void) 
{	
	switch(g_GlobalInfo.Company) {
	case COMPANY_KTNG :	
	case COMPANY_EMART24_KTNG :
	case COMPANY_MINISTOP_KTNG :
	case COMPANY_EMART_EVERYDAY_KTNG :
		return 1;
	}
	return 0;
}

int checkCashOnlyMode(void) 
{	
	if(AmountInfo.CashOnlyMode == CASH_ONLY_INCLUDE) return 1;
	return 0;
}

int checkKsccMode(void) 
{	
	switch(g_GlobalInfo.Company) {
	case COMPANY_KSCC :	
		return 1;
	}
	return 0;
}

int checkEmartMode(void) 
{	
	switch(g_GlobalInfo.Company) {
	case COMPANY_EMART24 :	
	case COMPANY_EMART24_KTNG :		
		switch(AmountInfo.DisplayMode) {
	 	case SaecoYoungSysMode :
			return SaecoYoungSysMode;
		}
		return 1;
	}
	return 0;
}

int checkEmartEverydayMode(void) 
{	
	switch(g_GlobalInfo.Company) {
	case COMPANY_EMART_EVERYDAY_KTNG :	
		return 1;
	}
	return 0;
}

int checkMinistopMode(void) 
{	
	switch(g_GlobalInfo.Company) {
	case COMPANY_MINISTOP:	
	case COMPANY_MINISTOP_KTNG :		
		switch(AmountInfo.DisplayMode) {
	 	case SaecoYoungSysMode :
			return SaecoYoungSysMode;
		}
		return 1;
	}
	return 0;
}

int checkOfflineMode(void)
{
	switch(g_GlobalInfo.Company) {
	case COMPANY_WELSTORY :
	case COMPANY_EDWORDS :
		return 1;
	}
	return 0;
}

int checkSaleInfoMachine(void)
{
	switch(g_GlobalInfo.Company) {
	case COMPANY_COCA :
	case COMPANY_KSCC :
	case COMPANY_KORAIL :
	case COMPANY_MINISTOP :
		return 1;
	case COMPANY_LOCKER :
	case COMPANY_COIN :
		// abco -> eth || serial 일 경우 무시
		return 0;
	}	
	return 0;
}

int checkButton1(void)
{
	switch(AmountInfo.DisplayMode) {
	case KlcMode_1 : 
	case ShilaxMode_1 :  
	case PCROOM_1 : 
	case RamenMode_1 : 
	case RamenMenMode_1 :  
	case RamenDoctorMode_1 : 
	case Ramen2SofoodMode_1 : 
	case SaecoCoinEmart24Mode : 
		return 1;
	}
	return 0;
}

int checkButton2(void)
{
	switch(AmountInfo.DisplayMode) {
	case RamenMode_2 : 
	case RamenDoctorMode_2 :  
	case Ramen2SofoodMode_2 : 
	case SaecoMode : 
	case SaecoCoinMode_2 : 
		return 1;
	}
	return 0;
}

int dispDataTrasmit(void)
{	
	int count;
	static int log_count=0;
	unsigned char buf[16];
	
	VM_GetConfigTID(buf);
	
	if(!VM_GetDetailInfo()) {
		
		if(devinfo.msg_signal != SIG_MSG_MODEM_CONNECT) 
		{
			devinfo.msg_signal=SIG_MSG_MODEM_CONNECT;
			LCD_Flush(0);
			LCD_Display(LCD_SORT_CENTER, 0, 0, VM_GetManufacturer());   	
		}
		count = UBC_LogCount();

		if((count>0) && (log_count^count)){
			LCD_Display(LCD_SORT_CENTER, 4, 1, "[%d]", UBC_LogCount());
			LCD_Display(LCD_SORT_CENTER, 6, 1, "데이터전송중");
		}

		if(!count) {
			switch(g_GlobalInfo.Company) {
			case COMPANY_COIN :
			case COMPANY_LOCKER :
			case COMPANY_DONGHAK :		
			case COMPANY_EMART24 :
			case COMPANY_EMART24_KTNG :
			case COMPANY_EMART_EVERYDAY_KTNG :
				break;
			default :
				if( LCD_TextCompare(4, VM_GetMachineInfo())) {
					LCD_Display(LCD_SORT_CENTER, 4, 1, VM_GetMachineInfo());
					LCD_Display(LCD_SORT_CENTER, 6, 1, buf);
				}
				break;
			}

		}
		log_count = count;
	}
	
	return UBC_LogCount();
}

void dispMsgCashOnly(void)
{
	if(devinfo.msg_signal != SIG_MSG_CASH_ONLY)
	{	
		devinfo.msg_signal=SIG_MSG_CASH_ONLY;
		
		LCD_Display(LCD_SORT_CENTER,2, 1, "");	
		LCD_Display(LCD_SORT_CENTER,4, 1, "현금으로");
		LCD_Display(LCD_SORT_CENTER,6, 1, "사용 해 주세요");
	}
}

void dispMsgOtherCard(void)
{		
	LED_Error();	

	UBC_TrackDataClear();
		
	LCD_Flush(0);
	
	LCD_Display(LCD_SORT_CENTER, 4, 0,"다른 카드를");		
	LCD_Display(LCD_SORT_CENTER, 6, 0,"사용해 주세요");
	
	play_check_sound("sound/card_other.wav");	
}

void dispMsgPushButton(void)
{
	// play_wav("adpcm_버튼을선택_8.bin");	
	LCD_Display(LCD_SORT_CENTER,2, 1, "");	

	if(VM_GetLotteSmartMachineInfo()) {
		LCD_Display(LCD_SORT_CENTER,4, 1, "");
		LCD_Display(LCD_SORT_CENTER,6, 1, "");
	}
	else {
		LCD_Display(LCD_SORT_CENTER,4, 1, "상품 번호를");
		LCD_Display(LCD_SORT_CENTER,6, 1, "눌러 주세요");
	}
}

void dispMsgCupJam(void)
{
	LCD_Display(LCD_SORT_CENTER,2, 1, "");
	LCD_Display(LCD_SORT_CENTER,4, 1, "%s", "컵품절");
	LCD_Display(LCD_SORT_CENTER,6, 1, "%s", "컵걸림");	
}

void dispMsgGoodsGetOut(void)
{	
	LCD_Display(LCD_SORT_CENTER,2, 1, "");
	LCD_Display(LCD_SORT_CENTER,4, 1, "상품을");
	LCD_Display(LCD_SORT_CENTER,6, 1, "꺼내 주세요");	
}

void dispMsgWaitMoment(void)
{
	LCD_Display(LCD_SORT_CENTER,2, 1, "");
	LCD_Display(LCD_SORT_CENTER,4, 1, "잠시만");
	LCD_Display(LCD_SORT_CENTER,6, 1, "기다려 주세요");
}

void dispMsgVmApproval(void)
{				
	LCD_Display(LCD_SORT_CENTER,2, 1, "");		
	LCD_Display(LCD_SORT_CENTER,4, 1, "");
	LCD_Display(LCD_SORT_CENTER,6, 1, "승인 중 입니다");
				
//	play_wav("adpcm_승인중_8.bin");
}

void SetIC(int ic_reader)
{
	FILE *fp = NULL;
	
	fp = fopen("/usr/rfpinpad/data/Global.dat", "rb");
	if(fp!=NULL) {
		fread(&g_GlobalInfo, sizeof(GLOBAL_INFO), 1, fp);	
		fclose(fp);
	}
	
	g_GlobalInfo.IC = ic_reader;
	
	fp = fopen("/usr/rfpinpad/data/Global.dat", "wb");
	if(fp!=NULL) {
		fwrite(&g_GlobalInfo, sizeof(GLOBAL_INFO), 1, fp);	
		fclose(fp);
	}
}

int GetIC(void)
{
	FILE *fp = NULL;
	
	fp = fopen("/usr/rfpinpad/data/Global.dat", "rb");
	if(fp!=NULL) {
		fread(&g_GlobalInfo, sizeof(GLOBAL_INFO), 1, fp);	
		fclose(fp);			
		
		if(g_GlobalInfo.IC>=IC_TYPE_MAX) {
			SetIC(IC_TYPE_ALCOR);
		}		
	}
	else {
		SetIC(IC_TYPE_ALCOR);
	}
	return g_GlobalInfo.IC;
}

void SetUbcn(char nUbcn)
{
	FILE *fp = NULL;
		
	g_GlobalInfo.VmUbcn = nUbcn;
	
	fp = fopen("/usr/rfpinpad/data/Global.dat", "wb");
	if(fp!=NULL) {
		fwrite(&g_GlobalInfo, sizeof(GLOBAL_INFO), 1, fp);	
		fclose(fp);
	}	
}

char GetUbcn(void)
{	
	return g_GlobalInfo.VmUbcn;
}

void SetPrinter(unsigned char on_off)
{
	FILE *fp = NULL;
	
	fp = fopen("/usr/rfpinpad/data/Global.dat", "rb");
	if(fp!=NULL) {
		fread(&g_GlobalInfo, sizeof(GLOBAL_INFO), 1, fp);	
		fclose(fp);
	}
	
	if(!on_off) g_GlobalInfo.Printer = 'O';
	else g_GlobalInfo.Printer = 'X';
	
	fp = fopen("/usr/rfpinpad/data/Global.dat", "wb");
	if(fp!=NULL) {
		fwrite(&g_GlobalInfo, sizeof(GLOBAL_INFO), 1, fp);	
		fclose(fp);
	}
}

char GetPrinter(void)
{		
	switch(g_GlobalInfo.Company) {
	case COMPANY_COIN :
	case COMPANY_LOCKER :
	case COMPANY_DONGHAK :
		return g_GlobalInfo.Printer;
	}
	
	return 'X';
}

void SetCreditRange(unsigned char visa)
{
	FILE *fp = NULL;
	
	fp = fopen("/usr/rfpinpad/data/Global.dat", "rb");
	if(fp!=NULL) {
		fread(&g_GlobalInfo, sizeof(GLOBAL_INFO), 1, fp);	
		fclose(fp);
	}
	
	g_GlobalInfo.VISA = visa;
	
	fp = fopen("/usr/rfpinpad/data/Global.dat", "wb");
	if(fp!=NULL) {
		fwrite(&g_GlobalInfo, sizeof(GLOBAL_INFO), 1, fp);	
		fclose(fp);
	}
}

int GetCreditRange(void)
{
	FILE *fp = NULL;
	
	fp = fopen("/usr/rfpinpad/data/Global.dat", "rb");
	if(fp!=NULL) {
		fread(&g_GlobalInfo, sizeof(GLOBAL_INFO), 1, fp);	
		fclose(fp);			

		switch(g_GlobalInfo.VISA) {
		case CREDIT_VISA_ONLY :
		case CREDIT_EMV_ALL :
			break;
		default :		
			SetCreditRange(CREDIT_EMV_ALL);
			break;
		}		
	}
	else {
		SetCreditRange(CREDIT_EMV_ALL);
	}

	return g_GlobalInfo.VISA;
}

unsigned int GetButtonTimeout(int btn)
{
	return AmountInfo.btnTime[btn].amount;
}

unsigned int GetButtonAmount(int btn)
{
	return AmountInfo.Button[btn].amount;
}

unsigned long GetSaleStartTime(void)
{
	return atol(AmountInfo.StartTime); 
}

unsigned long GetSaleStopTime(void)
{
	return atol(AmountInfo.StopTime); 
}

static unsigned long	s_dwBootloader;				// 부트로더 버전
static time_t			s_tKernel;					// 커널 버전
static time_t			s_tNewzen;					// newzen 버전

void checkKernelVer(void)
{	
	int nPos;
	char *pVer;
	char sTemp[64], sVersion[64];
	struct utsname utsName;

	int return_stat;
	char *file_name;
	struct stat file_info;

	struct passwd *my_passwd;
	struct group  *my_group;

	mode_t file_mode;

	FILE *fp = NULL;
	unsigned char buf[16];

	//////////////////////////////////////////////////////////////////////////////
	// newzen 버전
	file_name = "/usr/rfpinpad/newzen";
	stat(file_name, &file_info);
	s_tNewzen = file_info.st_mtime;

	file_mode = file_info.st_mode;
	
	printf("=======================================\r\n");
	printf("FILE NAME : %s\n", file_name);
	printf("FILE TYPE : ");
	
	if (S_ISREG(file_mode))
	{
	    printf("regular file\n");
	}
	else if (S_ISLNK(file_mode))
	{
	    printf("symbolic link\n");
	}
	else if (S_ISDIR(file_mode))
	{
	    printf("directory\n");    
	}
	else if (S_ISCHR(file_mode))
	{
	    printf("char device\n");
	}
	else if (S_ISBLK(file_mode))
	{
	    printf("block device\n");
	}
	else if (S_ISFIFO(file_mode))
	{
	    printf("FIFO\n");
	}
	else if (S_ISSOCK(file_mode))
	{
	    printf("socket\n");
	}
	
	printf("\r\n");
	my_passwd = getpwuid(file_info.st_uid);
	my_group  = getgrgid(file_info.st_gid);
	
	printf("owner                   : %s\n", my_passwd->pw_name);
	printf("group                   : %s\n", my_group->gr_name);
	printf("file size               : %d\n", file_info.st_size);
	printf("recently read time      : %d\n", file_info.st_atime);
	printf("recently modified time  : %d\n", file_info.st_mtime);
	printf("hyper linked file count : %d\n", file_info.st_nlink);


	//////////////////////////////////////////////////////////////////////////////
	// 커널 버전
	printf("=======================================\r\n");
	uname(&utsName);

	nPos = 0;
	pVer = utsName.version;

	printf("uname : %s\r\n\n", pVer);	

	if(strstr(pVer, "2018")) 
	{
		fp = fopen("/usr/rfpinpad/CONF/RailPlus.ini", "wb");
		if(fp!=NULL) 
		{
			buf[0] = 'O';
			fwrite(buf, sizeof(unsigned char), 1, fp);	
			fclose(fp);
		}		
	}
	
	// uname : #320 Tue Jul 19 17:03:14 KST 2016
	strcpy(sTemp, "20160719170314");
	nPos = 14;
	
	Ascii2BCD((unsigned char *)sTemp, nPos, (unsigned char *)sVersion);	
	s_tKernel = GetBCDDateTimeTick((unsigned char *)sVersion);

	util_strptime(s_tKernel);
	printf("=======================================\r\n");
}

void initMsgSignal(void)
{
	devinfo.msg_signal = 0;
}

int signalCheck(void)
{
	return devinfo.msg_signal;
}

void signalSet(int sig)
{
	devinfo.msg_signal = sig;
}

int signalCheckException(void) 
{
	switch(devinfo.msg_signal) {
	case SIG_MSG_NOT_FUNCTION :
	case SIG_MSG_OUT_SUPPORT_CARD :
		return 0;
	}
	return -1;
}



/************************************** COPYRIGHT 2016 UBC Co., LTD.*************************************/

