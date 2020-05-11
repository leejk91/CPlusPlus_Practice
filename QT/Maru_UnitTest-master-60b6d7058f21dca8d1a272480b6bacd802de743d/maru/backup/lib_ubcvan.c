#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>

#include "typedef.h"

#include "lib_util.h"
#include "lib_rs232.h"
#include "lib_com.h"
#include "lib_vm.h"
#include "lib_lcd.h"
#include "lib_led.h"
#include "lib_rf.h"
#include "lib_sam.h"
#include "lib_msr.h"
#include "lib_ubcvan.h"
#include "lib_scd_sk.h"
#include "lib_koces.h"

#include "app_config.h"

#include "prepaid/process_kb.h"
#include "prepaid/process_tm.h"
#include "prepaid/process_eb.h"
#include "prepaid/process_mybi.h"
#include "prepaid/process_kcash.h"
#include "prepaid/process_psam.h"



/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define LIB_UBCVAN_DEB  0
#if (LIB_UBCVAN_DEB == 1)
# define	PRINTF(fmt,args...) 	printf(fmt, ##args)
# define	PRINTHEX(fmt,args...)  	PrintHex(fmt, ##args)
# define	HEXDUMP(fmt,args...)  	HEX_Dump(fmt, ##args)
#else
# define    PRINTF(a...)    (void)(0)
# define    PRINTHEX(a...)  (void)(0)
# define    HEXDUMP(a...)   (void)(0)
#endif

#define UBCN_VAN_COUNT_MAX (0x0ffff)

#define KCASH_5859_TEST 		0
#define ERROR_CHECK_LOG		1
/* Private macro -------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------*/
VM_TRANS g_VmRx, g_VmTx;

NanoVANBasicReq g_NanoVANBasicReq, g_CurrVANBasicReq, g_CreditVANBasicReq;
NanoVANBasicReq VAN_BasicReq;
NanoVANBasicRes VAN_BasicRes;

NanoVANPrepaidApproveReq VAN_ApproveReq;

UBC_BAKUP  g_NanoBackup, g_TmsBackup;
int g_CreditAuth=0, g_CraditType=0, g_CraditTimeout=0, g_CraditComplete=0, g_CraditCancel=0;
unsigned long g_CraditAmount;
int g_StatInfoCount=0;


unsigned char adjustment_time[7];
int adjustment_check=0;


MSR_INFO m_nMsrInfo;
KB_DATA m_nKbData;
int m_nCraditType;
unsigned long m_nNanoTotCnt;

int g_LogPosition=0;

	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 01:구매,02:취소,03:망취소,04:현금반품,05:구매+현금,06:취소+현금,07: 망취소+현금,58:대체서명, 59:대체서명검증

const char *g_TmTransaction[12] = {
	"00","01","02","03","04","05","06","07","08","09","58","59"
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 01:신용,02:현금영수증,03:포인트,04:선불,05:직불,06:수표,08:집계,09:단말기,10:현금수집
const char *g_NanoTransaction[11] = {
	"00","01","02","03","04","05","06","07","08","09","10"
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 01:승인,02:취소,03:인증+승인,04:적립,05:적립취소,06:조회,07:인증,08:망상취소,09:정산,
// 10:단말기다운로드,11:수집,12:운영정보,13:상태정보, 14:DUPKT 갱신, 15:파라미터 갱신, 16: 파라미터 갱신확인
const char *g_NanoPacket[17] = {
	"00","01","02","03","04","05","06","07","08","09","10","11","12","13","14", "15", "16"
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 01:티머니,02:마이비,03:KCASH,04:HIPLUS,05:CashBee,09:기타?
const char *g_OrganCode[7] = {
	"0","1","2","3","4","5","9"
};

/* Private function prototypes --------------------------------------------------*/


KB_DATA g_KbData;
TMS_PURCHASE_INFO_REQ  g_TmsPurchaseInfoReq;


void UBC_SetCreditAuth(int nAuth)
{
	g_CreditAuth = nAuth;
	
	if(!nAuth) {	
		// 신용카드 거래요청을 하였는데 응답을 받기 전에 망이 끊어 졌을 경우  취소를 시켜 준다.
		PRINTF("g_CraditComplete = %d\r\n", g_CraditComplete);					
		if(!g_CraditComplete) VM_KbApprovalCancel(1);
	}

}

int UBC_GetCreditAuth(void)
{
	return g_CreditAuth;
}


void UBC_SetCreditComplete(int complete)
{
	g_CraditComplete = complete;
}

int UBC_GetCreditComplete(void)
{
	return g_CraditComplete;
}

void UBC_ResCodeClear(void)
{
	memset(VAN_BasicRes.ResCode, 0x00, sizeof(VAN_BasicRes.ResCode));
}

int UBC_GetResCode(char* in_code)
{
	memcpy(in_code, VAN_BasicRes.ResCode, sizeof(VAN_BasicRes.ResCode));
	return memcmp(VAN_BasicRes.ResCode, "\x00\x00\x00\x00", sizeof(VAN_BasicRes.ResCode)) ;
}

void UBC_LogInitial(void)
{
	FILE *fp = NULL;
	
	fp = fopen("/usr/rfpinpad/log/trans_counter.dat", "r");
	if (fp  == NULL) {
		memset((char*)&g_NanoBackup, 0, sizeof(UBC_BAKUP));
	}
	else {
		fread(&g_NanoBackup, sizeof(UBC_BAKUP), 1, fp);	
		fclose(fp);	
	}
	
	fp = fopen("/usr/rfpinpad/log/trans_backup.dat", "r");

	if(fp==NULL) {
		g_TmsLog.TotCnt=1;
		memset((char*)&g_TmsPurchaseInfoReq, 0,  sizeof(TMS_PURCHASE_INFO_REQ));
	}
	else {
		fread(&g_TmsPurchaseInfoReq, sizeof(TMS_PURCHASE_INFO_REQ), 1, fp);	
		fclose(fp);

		g_TmsLog.TotCnt = 1+ atoi(g_TmsPurchaseInfoReq.TotCnt);


		if((g_TmsPurchaseInfoReq.Time[0]>='2') && (g_TmsPurchaseInfoReq.Time[0]<='9')) 	RTC_Set(g_TmsPurchaseInfoReq.Time);
		system("date\0");
	}
}

void UBC_LogClear(void)
{
	system("rm /usr/rfpinpad/log/trans_* \0");
	system("rm /usr/rfpinpad/log/transaction_* \0");
	system("rm /usr/rfpinpad/log/Data.cap \0");
	system("rm /usr/rfpinpad/log/EMVL2.cfg \0");
	memset((char*)&g_NanoBackup, 0, sizeof(UBC_BAKUP));
	
}

int UBC_LogCheck(void)
{
	if((g_NanoBackup.cnt < (UBC_LOG_MAX-2)) && (g_NanoBackup.push==g_NanoBackup.pop)) return 0;
	return -1;
}

int UBC_LogCount(void)
{
	return g_TmsLog.TotCnt;
}

int UBC_LogRead(unsigned char *pData, int rLen)
{
	FILE *fp = NULL;
	char buf[256];
	int len=0;

	memset(pData, 0x00, rLen);
	
	fp = fopen("/usr/rfpinpad/log/trans_counter.dat", "r");
	if (fp  == NULL) {
		return 0;
	}
	fread(&g_NanoBackup, sizeof(UBC_BAKUP), 1, fp);	
	fclose(fp);	

	g_LogPosition = 0;

	if(g_NanoBackup.push != g_NanoBackup.pop) 
	{		
		sprintf(buf, "/tmp/transaction_%d.dat", g_NanoBackup.pop);	

		fp = fopen(buf, "r");		
		if(fp == NULL) {			
			g_LogPosition += 1;
			sprintf(buf, "/usr/rfpinpad/log/transaction_%d.dat", g_NanoBackup.pop);	

			fp = fopen(buf, "r");		
			if(fp == NULL) 
			{
				printf("\r\n\n\tNo serch file ... %s\r\n\n", buf);	
			
				UBC_NanoPop();				
				return 0;
			}
		}
		
		g_LogPosition += 1;
		
		len = fread(pData, rLen, 1, fp);	
		fclose(fp);			
	}
	return len;
}

int UBC_LogCheckIP(void)
{
	FILE *fp = NULL;
	char buf[256];
	int len=0;
	unsigned char *pData;
	NanoVANBasicReq nVANBasicReq;
	
	pData = (unsigned char *)&nVANBasicReq;
	memset(pData, 0x00, sizeof(NanoVANBasicReq));
	
	fp = fopen("/usr/rfpinpad/log/trans_counter.dat", "r");
	if (fp  == NULL) {
		return 0;
	}
	fread(&g_NanoBackup, sizeof(UBC_BAKUP), 1, fp);	
	fclose(fp);	

	g_LogPosition = 0;
	
	if((g_NanoBackup.push != g_NanoBackup.pop) && (g_NanoBackup.cnt>0))
	{

		sprintf(buf, "/tmp/transaction_%d.dat", g_NanoBackup.pop);	

		fp = fopen(buf, "r");		
		if(fp == NULL) {	
			g_LogPosition += 1;
			sprintf(buf, "/usr/rfpinpad/log/transaction_%d.dat", g_NanoBackup.pop);	

			fp = fopen(buf, "r");
			if(fp == NULL) {
				printf("\r\n\n\tNo serch file ... %s\r\n\n", buf);			
				UBC_NanoPop();	
				return 0;
			}
		}
		
		g_LogPosition += 1;
		len = fread(pData, sizeof(NanoVANBasicReq), 1, fp);	
		fclose(fp);	
				
		
		if( !memcmp(nVANBasicReq.TransactionClass, "0905", 4)  ) {
			// KORAIL || COCA
			VM_ServerIpChange(SCD_SEL_STAT_IP);				
		}
		else {
			VM_ServerIpChange(SCD_SEL_UBC_IP);
		}
		
	}
	return len;
}

void UBC_NanoPop(void) 
{
	FILE *fp = NULL;
	char rm_buf[64];
	
	
	g_StatInfoCount = 0;	
	
	if((g_NanoBackup.push != g_NanoBackup.pop) && (g_NanoBackup.cnt>0))
	{	
		// TRACK 정보 삭제
		UBC_TrackDataClear();
		
		if(g_LogPosition==1) 
		{	// 휘발상 파일 삭제	
			sprintf(rm_buf, "rm /tmp/transaction_%d.dat\0 &", g_NanoBackup.pop);		
			system(rm_buf);
		}
		else {
			// 비휘발성 신용망상 승인, 선불/ 현금 수집 삭제
			sprintf(rm_buf, "rm /usr/rfpinpad/log/transaction_%d.dat\0 &", g_NanoBackup.pop);		
			system(rm_buf);
		}
		
		g_NanoBackup.pop = (1+g_NanoBackup.pop)%UBC_LOG_MAX;
		g_NanoBackup.cnt -=1;				

		fp = fopen("/usr/rfpinpad/log/trans_counter.dat", "w");
		if(fp!=NULL) {
			fwrite(&g_NanoBackup, sizeof(UBC_BAKUP), 1, fp);	
			fclose(fp);		
		}

		memcpy(g_CurrVANBasicReq.TransactionClass, g_NanoTransaction[0], 2);

		#ifdef DBG_KTC
			printf("\r\n\t%d.RD <%d/%d>\r\n", g_NanoBackup.cnt, g_NanoBackup.push, g_NanoBackup.pop);	
		#endif
	}
	
	if(g_NanoBackup.push==g_NanoBackup.pop) {		
		VM_Start(1);
	}	
	
	
}


void UBC_AditionalResponseData(word *tlv_len, unsigned char *tlv_data)
{
	*tlv_len = 26;
	Char2BCD(&koces_ic_res.ard[0], tlv_data, 52);
}

void UBC_IssuerAuthenticationData(word *tlv_len, unsigned char *tlv_data)
{
	*tlv_len = 10;
	Char2BCD(&koces_ic_res.iad[0], tlv_data, 20);
}

void UBC_IssuerScript(word *tlv_len, unsigned char *tlv_data)
{
	*tlv_len = 260;
	Char2BCD(&koces_ic_res.issuer_script[0], tlv_data, 520);	
}

int UBC_ResMessage(char *pMsg)
{
	memcpy(pMsg, VAN_BasicRes.ResMessage, VAN_BasicRes.LenMessage);	
	
	return VAN_BasicRes.LenMessage;
}

int UBC_RcvVan(unsigned char *pData, int nLen) 
{
	int i, nRes=RC_SUCCESS, nCreditOk=RC_FAILURE;
	/////////////////////////////////////////////////
	// WIFI 에서 시간을 동기.
	//#if((CDMA_SEL==CDMA_WIZ_WIFI) || (CDMA_SEL==CDMA_CSW_WIFI))
	unsigned char tBuf[256];	
	//#endif

	#ifdef DBG_KTC
	//	HEX_Dump( (unsigned char*)pData, nLen , "VAN << %d ", nLen);
	#endif
	
	memcpy(VAN_BasicRes.ResCode, "XXXX", 4);			
	memset((unsigned char*)VAN_BasicRes.Date, 0, sizeof(NanoVANBasicRes)-0x27);
	// 1~11 번까지 복사..		
	memcpy((unsigned char*)&VAN_BasicRes, pData, 0x27);
		
	// 1.	00 bin  	1	STX
	if(VAN_BasicRes.stx != 0x02) return RC_FAILURE;
	pData += 1;
	// 2.	01 an   	4	LEN					단말기번호 - ETX 길이	
	pData += 4;
	// 3.	05 an   	10	TID					단말기 번호 "1234567890"
	if(memcmp(VAN_BasicRes.TID, g_VmConfig.TID, 10)) return RC_FAILURE;
	pData += 10;
	// 4.	0F an   	2	거래 구분			01:신용,02:현금영수증,03:포인트,04:선불,05:직불,06:수표,08:집계,09:단말기
	if(memcmp(pData, g_CurrVANBasicReq.TransactionClass, 2)) return RC_FAILURE;
	pData += 2;
	// 5.	11 an   	2	전문 구분			01:승인,02:취소,03:인증+승인,04:적립,05:적립취소,06:조회,07:인증,08:망상취소,09:정산,10:단말기다운로드,11:수집,12:운영정보
	if(memcmp(pData, g_CurrVANBasicReq.PacketClass, 2)) return RC_FAILURE;
	pData += 2;
	// 6.	13 an	1	입력매체 구분코드		K:KEYIN, S:SWIPE, R:RF, I:IC, X:NON, 'E' : PayOn, 'F' : Mobile PayOn
	pData += 1;
	// 7.	14 an	5	단말기거래 일련번호 	00001 - 99999
	pData += 5;
	// 8.	19 an	2	전문버젼				"T1"
	pData += 2;
	// 9.	1B an	1	전문 암화화 여부		0:암호화안함, 1:암호화
	pData += 1;
	// 10.	1C an	10	예비용
	pData += 10;
	// 11.	26 bin	1	0x1C
	if(*pData != 0x1C)  return RC_FAILURE;
	pData += 1;
	// 12.	27 an	8	거래 일자			YYYYMMDD
	if(*pData != 0x1C) {
		for(i=0; pData[i]!=0x1C && i<8; i++) VAN_BasicRes.Date[i]=pData[i];
		HEXDUMP(pData, i, "거래 일자 ");
		pData += i;
	}
	// 13.	2F bin	0x1C
	// 송신 데이터 인지 확인 후 진행.
	if(*pData != 0x1C)  return RC_FAILURE;	
	pData += 1;
	// 14.	30 an	6	거래 시각			hhmmss
	if(*pData != 0x1C) {
		for(i=0; pData[i]!=0x1C && i<6; i++)VAN_BasicRes.Time[i]=pData[i];
		HEXDUMP(pData, i, "거래 시각 ");

		/////////////////////////////////////////////////
		// WIFI 에서 시간을 동기.
		//if(!memcmp(VAN_BasicRes.Date, "2015", 1))
		//if(memcmp(VAN_BasicRes.Time, " ", 1))
		{
			for(i=0; i<8; i++) tBuf[i] = VAN_BasicRes.Date[i];
			for(i=0; i<6; i++) tBuf[i+8] = VAN_BasicRes.Time[i];
			RTC_Set(tBuf);		
			GetTimeString(NULL, tBuf);
			
			PRINTF("\r\n 거래 시간    %s\r\n", tBuf);
		}
 		//#endif
	
		pData += i;
	}
	// 15.	31 bin	1	0x1C
	pData += 1;
	// 16.	37 an	10	승인금액			거래금액 / 수집완료건수
	if(*pData != 0x1C) {
		for(i=0; pData[i]!=0x1C && i<10; i++);
		HEXDUMP(pData, i, "승인금액 ");
		pData += i;
	}
	// 17.	41 bin	1	0x1C
	pData += 1;
	// 18.	42 an	9	발생 포인트
	if(*pData != 0x1C) {
		for(i=0; pData[i]!=0x1C && i<9; i++);
		HEXDUMP(pData, i, "발생 POINT");
		pData += i;
	}
	// 19.	4B bin	1	0x1C
	pData += 1;
	// 20.	4C an	9	누적 포인트
	if(*pData != 0x1C) {
		for(i=0; pData[i]!=0x1C && i<9; i++);
		HEXDUMP(pData, i, "누적 POINT");
		pData += i;
	}
	// 21.	55 bin	1	0x1C
	pData += 1;
	// 22.	56 an	9	사용가능 포인트
	if(*pData != 0x1C) {
		for(i=0; pData[i]!=0x1C && i<9; i++);
		HEXDUMP(pData, i, "가능 POINT");
		pData += i;
	}
	// 23.	5F bin	1	0x1C
	pData += 1;
	// 24.	60 an	1	매입 구분
	if(*pData != 0x1C) {
		for(i=0; pData[i]!=0x1C && i<1; i++);
		HEXDUMP(pData, i, "매입 구분");
		pData += i;
	}
	// 25.	61 bin	1	0x1C
	pData += 1;
	// 26.	62 an	10	사업자 번호
	if(*pData != 0x1C) {
		for(i=0; pData[i]!=0x1C && i<10; i++);
		HEXDUMP(pData, i, "사업자 번호");
		pData += i;
	}
	// 27.	6C bin	0x1C	
	pData += 1;	
	// 28.	6D an	15	가맹점 번호
	if(*pData != 0x1C) {
		for(i=0; pData[i]!=0x1C && i<15; i++);
		HEXDUMP(pData, i, "가맹점 번호");
		pData += i;
	}
	// 29.	7B bin	0x1C
	pData += 1;		
	// 30.	7C an	32	가맹점 이름
	if(*pData != 0x1C) {
		for(i=0; pData[i]!=0x1C && i<32; i++);
		HEXDUMP(pData, i, "가맹점 이름");
		pData += i;
	}
	// 31.	9C bin	0x1C
	pData += 1;
	// 32.	9D an	20	발급사 명
	if(*pData != 0x1C) {
		for(i=0; pData[i]!=0x1C && i<20; i++);
		HEXDUMP(pData, i, "발 급 사");
		pData += i;
	}
	
	// 33.	9E bin	0x1C;
	pData += 1;		
	// 34.	B2 an	20	매입사 명
	if(*pData != 0x1C) {
		for(i=0; pData[i]!=0x1C && i<20; i++);
		HEXDUMP(pData, i, "매 입 사");
		pData += i;
	}
	
	// 35.	C6 bin  0x1C
	pData += 1;
	// 36.	C7 an	10	승인 번호
	if(*pData != 0x1C) {
		memset(VAN_BasicRes.ApproveNum, 0x20, sizeof(VAN_BasicRes.ApproveNum));
		for(i=0; pData[i]!=0x1C && i<10; i++) VAN_BasicRes.ApproveNum[i]=pData[i];
		HEXDUMP(pData, i, "승인 번호 ");
		pData += i;
	}
	
	// 37.	D1 bin	0x1C 
	pData += 1;
	// 38.	D2 an	4	응답 코드
	if(*pData != 0x1C) {
		for(i=0; pData[i]!=0x1C && i<4; i++) VAN_BasicRes.ResCode[i]=pData[i];
		//printf("\r\n%c%c%c%c\r\n", VAN_BasicRes.ResCode[0], VAN_BasicRes.ResCode[1], VAN_BasicRes.ResCode[2], VAN_BasicRes.ResCode[3]);
		HEXDUMP(pData, i, "응답 코드");
		
		// 전문오류,  ........		
		if(!memcmp(VAN_BasicRes.ResCode, "5108", 4) ||  !memcmp(VAN_BasicRes.ResCode, "7004", 4)) {						
			// AS 처리가 맞다고 봄.	
			// 성공으로 하여 POP 처리.
		}
		else // 거래내역 없슴  || 잠시후 재 요망 || 유효기간 경과 카드 || 카드사 전화 요망 || 이미 취소 된 거래  || 이미 취소 된 거래  || 수기입력번호오류
		if(!memcmp(VAN_BasicRes.ResCode, "7140", 4)   || 
			!memcmp(VAN_BasicRes.ResCode, "5113", 4) ||
			!memcmp(VAN_BasicRes.ResCode, "8314", 4) || 
			!memcmp(VAN_BasicRes.ResCode, "8373", 4) || 
			!memcmp(VAN_BasicRes.ResCode, "8411", 4) || 
			!memcmp(VAN_BasicRes.ResCode, "7002", 4)  ||
			!memcmp(VAN_BasicRes.ResCode, "7181", 4)) {	
			// 성공으로 하여 POP 처리.
			g_CraditComplete = -1;
		}	
		//else // 카드사 통신장애 ........
		//if(!memcmp(VAN_BasicRes.ResCode, "7118", 4)) {			
		//}	
		//else // 미등록 가맹점 ........
		//if(!memcmp(VAN_BasicRes.ResCode, "7571", 4)) {
		//}		
		//else // 거래 금액 너무 적음 ........
		//if(!memcmp(VAN_BasicRes.ResCode, "8032", 4)) {
		//}	
		//else // 유효기간 경과 카드 ........
		//if(!memcmp(VAN_BasicRes.ResCode, "8314", 4)) {
		//}			
		//else // 카드사 카드사 전화요망 ........
		//if(!memcmp(VAN_BasicRes.ResCode, "8373", 4)) {
		//}	
		//
		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// K-CASH 파라미터 갱신 절차
		// 	- K-CASH 파라미터 갱신은 필요시 운영자에 의해 요청전문을 전송할 수 있다.
		// 	- 수집거래, 단말기 상태정보 요청전문에 대한 응답전문의 응답코드가  '8011' (K-CASH파라미터 갱신요망)이면  
		// 		① K-CASH 파라미터 갱신 요청전문을 VAN시스템으로 요청
		//      ② K-CASH 파라미터 갱신 응답전문을 VAN시스템으로부터 수신
		// 		③ 기전송하여'8011'응답받았던 요청전문을 VAN시스템으로 요청
		// 		④ 응답전문 수신
		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//if(!memcmp(VAN_BasicRes.ResCode, "5505", 4) //  5505통신장애발생	)
//		if(!memcmp(VAN_BasicRes.ResCode, "8011", 4)) {
//			UBC_KcashUpdateParameterReq();
//		}	
//		else // KCASH 변경할 파라메터 없음
//		if(!memcmp(VAN_BasicRes.ResCode, "6024", 4)) {
//			g_VmKcashStat = 1;
//		}	
//		else // KCASH 서명값 오류
//		if(!memcmp(VAN_BasicRes.ResCode, "6060", 4)) {
//		}	
//		else // KCASH 파라메터ID 오류거래 일자
//		if(!memcmp(VAN_BasicRes.ResCode, "6027", 4)) {			
//		}
		else // 중복전송 || 정상........
		if(!memcmp(VAN_BasicRes.ResCode, "8412", 4) || !memcmp(VAN_BasicRes.ResCode, "0000", 4))
		{				
			nCreditOk=RC_SUCCESS;
			
			if( !memcmp(VAN_BasicRes.TransactionClass, "09", 2)  ) {
				if( !memcmp(VAN_BasicRes.PacketClass, "09", 2)  )  {						
					PRINTF( "마감처리\r\n");			
					
					adjustment_check = 2;
					GetTimeBCD((time_t)NULL, (char *)adjustment_time);					
					play_wav("sound/transaction_complete.wav");
				}
			}	
			else	// K-CASH 파라미터 응답 전문
			if( !memcmp(VAN_BasicRes.TransactionClass, "15", 2)  ) {		
				
			}			
		}
		else 
		{	
			// 응답코드가 정상이 아니면 .........	
			nRes = RC_FAILURE;	
		}		
		
		pData += i;
	}
	// 39.	D6 bin	0x1C 
	pData += 1;
	// 40.	D7 an	32	응답 메시지
	if(*pData != 0x1C) {
		for(i=0; pData[i]!=0x1C && i<32; i++)VAN_BasicRes.ResMessage[i]=pData[i];
		VAN_BasicRes.LenMessage = i;
		HEXDUMP( (unsigned char*)pData, i, "응답 메시지");
		pData += i;
	}
	// 41.	FE bin	0x1C
	pData += 1;
	
	//an	부가 정보	
	for(i=0; pData[i]!=0x03 && i<2048; ) {		
		VAN_BasicRes.AddedInfo[i] = pData[i];	
		i++;		
	}

	// 8412 : 중복전송
	if(i>0 && ( !memcmp(VAN_BasicRes.ResCode, "8412", 4) || !memcmp(VAN_BasicRes.ResCode, "0000", 4)) ) 
	{
		pData[i] = 0;		
		
		// HEXDUMP(VAN_BasicRes.AddedInfo, i, "ADD Info");
		
		if( !memcmp(VAN_BasicRes.TransactionClass, "0101", 4)  ) {
			// 신용승인
			if(VM_GetKTC() == KTC_KOCES_INCLUDE) {
				switch(VAN_BasicRes.TransactionType) {
				case 'S' :	// SWIPE
				case 'E' : // PAYON
				case 'F' : // MOBILE PAYON
				case 'K' : // KEYIN
					koces_credit_response(VAN_BasicRes.AddedInfo, i);
					break;
				case 'I' :
					if(RC_SUCCESS != koces_ic_response(VAN_BasicRes.AddedInfo, i) )
					{
						// 2nd Generation failure....
						nCreditOk = RC_FAILURE;
					}
					break;
				}
				koces_get_ksn_counter();			
			}
		}
		else
		if( !memcmp(VAN_BasicRes.TransactionClass, "0201", 4)  ) {
			// 현금영수증
			if(VM_GetKTC() == KTC_KOCES_INCLUDE) {
				koces_cash_response(VAN_BasicRes.AddedInfo, i);
				koces_get_ksn_counter();		
			}
				
		}
		else
		if( !memcmp(VAN_BasicRes.TransactionClass, "0914", 4)  ) {
			// 보안키 갱신
			if(VM_GetKTC() == KTC_KOCES_INCLUDE) {
				koces_dupkt_response(VAN_BasicRes.AddedInfo, i);
				koces_get_ksn_counter();			
			}
		}
		else
		if(!memcmp(VAN_BasicRes.TransactionClass, "0412", 2)) 
		{					
			// 선불 운영정보	
			switch(VAN_BasicRes.AddedInfo[0]) {
			case '1' : // Tmoney					
				UBC_TmSetIssuerID(pData + 1);				
				break;
			case '5' : // Cashbe		
				EB_GetCenterInfoUpdate(i, pData + 1);
				break;
		
			}
		}	
		
	}
	

	#ifdef DBG_KTC
		HEX_Dump(VAN_BasicRes.ResCode, 4, "응답 코드 %02X %02X", nRes, nCreditOk);
	#endif
	
	g_CreditAuth = 0;
	
 	if(nRes == RC_SUCCESS) { 				
		UBC_NanoPop();
		 
		// 20120711 : 자판기에서 취소 플레그가 생길경우로 넘김.	
 		if(	g_CraditCancel ) {
 			g_CraditCancel = 0;
			g_CraditAmount = 0;
 		}			

		if( (nCreditOk==RC_SUCCESS) && ( !memcmp(VAN_BasicRes.TransactionClass, "0101", 4) ) ) g_CraditComplete = 1;	
	}
	else {					
		g_CraditType = 0;	
	}
			
	VM_NetClose();
						
 	return nRes;
	
}

unsigned short UBC_ReadTempVANBasicReq(int nNum) 
{
	unsigned short nLen=0;	
	unsigned char buf[64];
	FILE *fp = NULL;

	NanoVANBasicReq TempVANBasicReq;

	sprintf(buf, "/usr/rfpinpad/log/transaction_%d.dat", nNum);	
	if ((fp = fopen(buf, "r")) == NULL)  return;

	fread(&TempVANBasicReq, sizeof(NanoVANBasicReq), 1, fp);
	fclose(fp);
		
	nLen = (unsigned short)Char2Int(TempVANBasicReq.MsgLen, 0)+5;
	//HEX_Dump(&TempVANBasicReq, nLen+2, "VAN >> %d", nLen);		
	
	if(memcmp((unsigned char*)&TempVANBasicReq, (unsigned char*)&g_NanoVANBasicReq, sizeof(NanoVANBasicReq))) {				
		PRINTF( " >> REFLASH\r\n");
		UBC_Backup();
	}
	else {
		PRINTF( " >> OK\r\n");
	}
		
	if(nLen > sizeof(NanoVANBasicReq) || !nLen) return 0;	
	return nLen;		
	
}

const unsigned char can_array[28] = {'0', 'A', 'B', 'E', 'C', 'F', 'H', 'K', 'D', 'G', 'I', 'L', 'J', 'M', 'N', 'O',};
/*******************************************************************************
* Function Name  : UBC_SetStatusInfo
* Description    : 
* Input          :
* Output         : None
* Return         : None
*******************************************************************************/
void UBC_SetStatusInfo(void)
{				
//	int i,j,k,l, w,x,y,z;
	int i,j,w,x,y,z;
	unsigned char *pdata, buf[8];
	unsigned short crc;
	
	GetTimeBCD((time_t)NULL, (char *)g_Today);
	
	memset((char*)&VAN_BasicReq, ' ', sizeof(NanoVANBasicReq));		
	// 00	
	VAN_BasicReq.stx = 0x02;
	// 01  an 4 단말기번호 - ETX 길이
	// sprintf((char*)VAN_BasicReq.MsgLen,                "%04d", );
	// 05  an 10 단말기 번호 "1234567890"	
	sprintf((char*)VAN_BasicReq.TID,                   "%s",   g_VmConfig.TID);	
	// 0F an 2 거래(업무) 구분
	sprintf((char*)VAN_BasicReq.TransactionClass,      "%s",   g_NanoTransaction[9]);			
	// 11 an 2 전문 구분, 수집
	sprintf((char*)VAN_BasicReq.PacketClass,           "%s",   g_NanoPacket[13]);				
	// 13 an 1 입력매체 구분코드 , K:KEYIN, S:SWIPE, R:RF, I:IC, X:NON	
	VAN_BasicReq.TransactionType = 'X';		
	// 14 an 5 단말기 거래일련 번호, 총 거래 Cnt
	// 20140602 부팅 시 거래 일련 번호 체크를 위해서 변경.
	//sprintf((char*)VAN_BasicReq.TransactionCount,      "%05d", g_TmsLog.TotCnt);
	// 19 an 2 전문 버젼
	sprintf((char*)VAN_BasicReq.PacketVersion,         "%s",   "T1");					
	// 1B an 1 전문 암호화 여부 안함																	
	VAN_BasicReq.Encryption = '0';	
	// 1C an 10 예비용														
	//sprintf((char*)VAN_BasicReq.RFU1,                  "%010d", ' ');
	// 26 FS																//
	VAN_BasicReq.FS1 = FS;
	// 27 an 40 인식 카드번호 
	//memcpy(VAN_BasicReq.CSN, g_MsrInfo.nTrack2, 37);
	// 4F FS	
	VAN_BasicReq.FS2 = FS;    	
	// 50 an 2 할부 개월 수
	//sprintf((char*)VAN_BasicReq.Installment,           "%02d", 0);	
	// 52 FS							
	VAN_BasicReq.FS3 = FS;
	// 53 an 10 승인금액   
	//sprintf((char*)VAN_BasicReq.Amount,      			"%010d", nAmount);
	// 5D FS
	VAN_BasicReq.FS4 = FS;
	// 5E an 10 봉사료	
	//sprintf((char*)VAN_BasicReq.ServicePrice, 			"%010d", 0);
	// 68 FS						
	VAN_BasicReq.FS5 = FS;
	// 69 an 10 부가가치세	
	//sprintf((char*)VAN_BasicReq.Tax, 					"%010d", 0);
	// 73 FS												
	VAN_BasicReq.FS6 = FS;	
	// 74 an 1 전표사용구분, 0:소비자소득공제, 1:사업자지출증빙 
	VAN_BasicReq.Bill= '0';		
	// 75 FS																			
	VAN_BasicReq.FS7 = FS;
	// 76 an 8 원거래 일자
	//memset(VAN_BasicReq.ApproveDate, ' ', 8);
	// 7E FS								
	VAN_BasicReq.FS8 = FS;
	// 7F an 10 원거래 승인 번호
	//memset(VAN_BasicReq.ApproveNum, ' ', 10);
	// 89 FS											
	VAN_BasicReq.FS9 = FS;					
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 8A 	
	
//	HEX_Dump((char*)&g_VmErrData, sizeof(VM_ERR_DATA), "ERR1");
//	HEX_Dump((char*)g_VmErrData.Can, 13, "ERR2");
	 
	if(strstr(g_VmName.Name, "LVC") && strstr(g_VmName.Name, "440")) 
	{			
		for(i=0;i<16;i++) {		
			VAN_BasicReq.AddedInfo[i] = g_VmErrData.Can[(i+4)/8] &(1<<((i+4)%8)) ? '1' : '0';
		}
	}
	else {
//		k=l=0;
		y=0;//0x04;
		z=0;//0x80;
		// 멀티 자판기.	상태정보가 50개로 이므로 수정 필요.	
		for(i=0,j=50;i<50;i++,j++) {		

			w= g_VmErrData.Can[i/8] & (1<<(i%8)) ? 0x01 : 0;
			x= g_VmErrData.Can[j/8] & (1<<(j%8)) ? 0x02 : 0;

			
			VAN_BasicReq.AddedInfo[i] = can_array[w+x+y+z];
		}
	}
	
	HEXDUMP((char*)VAN_BasicReq.AddedInfo, 50, "ERR3");
	
	for(i=0;i<50;i++) {		
		VAN_BasicReq.AddedInfo[50 + i] = '0';
	}
				
	// 해당 에러가 하나라도 존재한다면 ...
	VAN_BasicReq.AddedInfo[50+0] = g_VmErrData.Flag.bits.mask 			? 'A' : '0';
	// 코인메카 이상
	VAN_BasicReq.AddedInfo[50+1] = g_VmErrData.Flag.bits.coin 			? 'A' : '0';
	// 지폐식별기 이상
	VAN_BasicReq.AddedInfo[50+2] = g_VmErrData.Flag.bits.bill 			? 'A' : '0';
	// 
	VAN_BasicReq.AddedInfo[50+3] = g_VmErrData.Flag.bits.water 			? 'A' : '0';
	// 
	VAN_BasicReq.AddedInfo[50+4] = g_VmErrData.Flag.bits.hotwater 		? 'A' : '0';
	// 
	VAN_BasicReq.AddedInfo[50+5] = g_VmErrData.Flag.bits.ice 			? 'A' : '0';
	// 
	VAN_BasicReq.AddedInfo[50+6] = g_VmErrData.Flag.bits.cup 			? 'A' : '0';			
	// 
	VAN_BasicReq.AddedInfo[50+7] = g_VmErrData.Flag.bits.waterway 		? 'A' : '0';
	// RESERVED .. 10,11
	VAN_BasicReq.AddedInfo[50+8]  = g_VmErrData.Flag.bits.system 		? 'A' : '0';
	// J. 기타이상
	VAN_BasicReq.AddedInfo[50+9]  = g_VmErrData.Flag.bits.other		 	? 'A' : '0';
	VAN_BasicReq.AddedInfo[50+10] = g_VmErrData.Flag.bits.rfu2	 		? 'A' : '0';
	// L. DOOR 열림
	VAN_BasicReq.AddedInfo[50+11] = g_VmErrData.Flag.bits.door	 		? 'A' : '0';	
	// M. 슬레이브 이상
	VAN_BasicReq.AddedInfo[50+12] = g_VmErrData.Flag.bits.slave	 		? 'A' : '0';
	// 
	VAN_BasicReq.AddedInfo[50+13] = g_VmErrData.Flag.bits.rfu3	 		? 'A' : '0';
	// O. MASTER 통신 불량
	VAN_BasicReq.AddedInfo[50+14] = g_VmErrData.Flag.bits.m_com	 		? 'A' : '0';
	// 
	VAN_BasicReq.AddedInfo[50+15] = g_VmErrData.Flag.bits.rfu4	 		? 'A' : '0';
	// 50원 주화 신호 방출 이상
	//VAN_BasicReq.AddedInfo[50+16] = '0';
	// 100원 주화 신호 방출 이상
	//VAN_BasicReq.AddedInfo[50+17] = '0';
	// 500원 주화 신호 방출 이상
	//VAN_BasicReq.AddedInfo[50+18] = '0';
	// 반환 스위치 이상
	//VAN_BasicReq.AddedInfo[50+19] = '0';
	// 동전 불출 불량
	//VAN_BasicReq.AddedInfo[50+20] = '0';
	// Safety Switch 이상
	//VAN_BasicReq.AddedInfo[50+21] = '0';
	/// Pulse Switch 이상
	//VAN_BasicReq.AddedInfo[50+22] = '0';
	// RESERVED .. 25 .. 29
	// 식별부 이상
	//VAN_BasicReq.AddedInfo[50+30] = '0';
	// Stacker 이상
	//VAN_BasicReq.AddedInfo[50+31] = '0';
	// 지폐 이상
	//VAN_BasicReq.AddedInfo[50+32] = '0';
	// 지폐 부출 이상
	//VAN_BasicReq.AddedInfo[50+33] = '0';
	// Safety Switch 이상
	//VAN_BasicReq.AddedInfo[50+34] = '0';
	// 지폐 만충
	//VAN_BasicReq.AddedInfo[50+35] = '0';
	// RESERVED .. 36 .. 49	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 대표 이상
	VAN_BasicReq.AddedInfo[100+0] = g_VmTerErr.value & 0xffff0000 ? '1' : '0';
	// CDMA 대표 이상(A)
	VAN_BasicReq.AddedInfo[100+1] = (g_VmTerErr.value&0xffff0000) == 0xffff0000		?    '1' : '0';
	// RF 대표 이상(B)
	VAN_BasicReq.AddedInfo[100+2] = '0';
	// MSR 대표 이상(C)
	VAN_BasicReq.AddedInfo[100+3] = '0';
	// 이하는 SAM 유무 Data (SAM 있음 : 0 , SAM 없음/이상 : 1)
	// KB 후불 교통(D)
	VAN_BasicReq.AddedInfo[100+4] ='0';
	// T-Money(E)
	VAN_BasicReq.AddedInfo[100+5] ='0';
	// MYbi 교통(F)
	VAN_BasicReq.AddedInfo[100+6] = '0';
	// EB 교통(F)
	VAN_BasicReq.AddedInfo[100+7] = '0';
	// Kcash 교통(F)
	VAN_BasicReq.AddedInfo[100+8] = '0';
	// 8-10	RFU - 0x20
	// 이하는 세부 이상 Data(정상 : 0, 이상 : 1)
	// 사용가능(이상이 있더라도 사용가능:0 불능:1)
	VAN_BasicReq.AddedInfo[100+10] = '0';
	// 통신 불능
	VAN_BasicReq.AddedInfo[100+11] = '0';
	// 13-15		RFU - 0x20
	// 사용가능(이상이 있더라도 사용가능:0 불능:1)
	VAN_BasicReq.AddedInfo[100+15] = '0';
	// APDU 실패 - 카드와 P/D간 통신 오류
	VAN_BasicReq.AddedInfo[100+16] = '0';
	// 10회 이상의 APDU 실패
	VAN_BasicReq.AddedInfo[100+17] = '0';
	//19-20		RFU - 0x20
	// 사용가능(이상이 있더라도 사용가능:0 불능:1)
	VAN_BasicReq.AddedInfo[100+20] = '0';
	//22-25		RFU - 0x20
	// 사용가능(이상이 있더라도 사용가능:0 불능:1)
	VAN_BasicReq.AddedInfo[100+25] = '0';
	// APDU 실패 - 카드와 P/D간 통신 오류
	VAN_BasicReq.AddedInfo[100+26] = '0';
	// 10회 이상의 APDU 실패
	VAN_BasicReq.AddedInfo[100+27] = '0';
	//29-30		RFU - 0x20
	// 사용가능(이상이 있더라도 사용가능:0 불능:1)
	VAN_BasicReq.AddedInfo[100+30] = '0';
	// APDU 실패 - 카드와 P/D간 통신 오류
	VAN_BasicReq.AddedInfo[100+31] = '0';
	// 10회 이상의 APDU 실패
	VAN_BasicReq.AddedInfo[100+32] = '0';
	//34-35		RFU - 0x20
	// 사용가능(이상이 있더라도 사용가능:0 불능:1)
	VAN_BasicReq.AddedInfo[100+35] = '0';
	// APDU 실패 - 카드와 P/D간 통신 오류
	VAN_BasicReq.AddedInfo[100+36] = '0';
	// 10회 이상의 APDU 실패
	VAN_BasicReq.AddedInfo[100+37] = '0';
	// MAC 생성 오류(사용자 카드의 잔액은 차감, 정산 데이터 미생성)
	VAN_BasicReq.AddedInfo[100+38] = '0';
	// RFU - 0x20
	//41-50	RFU - 0x20	
	// 1E8 FS											
	VAN_BasicReq.AddedInfo[150] = FS;				
	// 8B an 판매된 Column
	//BCD2Char((uchar*)&g_VmAmountColumn.Col, (uchar*)&VAN_BasicReq.AddedInfo[151], 2);	// an 현재 컬럼.	
	VAN_BasicReq.AddedInfo[153] = 0x03;

	// 단말기번호 ~ ETX까지의 길이
	pdata = VAN_BasicReq.TID;
	for(i=0; i<sizeof(NanoVANBasicReq) ; i++) {
		if(pdata[i]==0x03) break;
	}
	sprintf((char*)buf,  "%04d", i+1);
	memcpy((char*)VAN_BasicReq.MsgLen, buf, 4);

	
	// CRC
	crc = (unsigned short)CRC_Update((unsigned char*)&VAN_BasicReq, Char2Int(VAN_BasicReq.MsgLen, 0)+5);
	Int2Byte(crc, &VAN_BasicReq.AddedInfo[154]);
	
	//HEXDUMP(&VAN_BasicReq, sizeof(VAN_BasicReq)-sizeof(NanoVANPrepaidApproveReq), "P/D %d", Char2Int(VAN_BasicReq.MsgLen, 0)+5);
		
	g_NanoVANBasicReq = VAN_BasicReq;	

	UBC_BackupVolatile();	
}

void UBC_SetPrepaidPacket(int nTransactionType)
{
	int i;
	unsigned char *pdata, buf[8];
	unsigned long nBalance=0, nMpda=0, nBALep=0, nTotPsam=0;
	unsigned short crc;
	LONG NTep, NCPsam, lValue;
	unsigned char tmp;

	
	PRINTF( "**********************************\r\n");
	PRINTF( "** UBC_SetPrepaidPacket %d\n", RF_GetClass());
	PRINTF( "**********************************\r\n");
	
	memset((char*)&VAN_ApproveReq, ' ', sizeof(NanoVANPrepaidApproveReq));	
	memset((char*)&VAN_BasicReq, ' ', sizeof(NanoVANBasicReq));
	
	switch(RF_GetClass()) {
	case RF_EBM :
		memset((char*)&g_Tm, 0, sizeof(TM_TRANSACTION));
		
		memcpy((unsigned char*)&nBalance, EBM_Info.BLOCK9_Balance1, 4); 	// 거래 전 잔액
		memcpy((unsigned char*)&nMpda, g_ebmTrans.Mpda, 4); 		// 거래 금액
		memcpy((unsigned char*)&nBALep, g_ebmTrans.Balance, 4); 		// 거래 후 잔액
		Swap_4(EB_SAMInfo.CreditPSAM_TOTsam, (unsigned char*)&nTotPsam); 
		NTep.value=0;
		memcpy(NTep.data, EBM_Info.BLOCK12_RechargeHistoryInfo+4, 2);
		memcpy(NCPsam.data, EB_SAMInfo.CreditPSAM_NCsam, 4); 
		
						
		// 00	
		VAN_BasicReq.stx = 0x02;
		// 01  an 4 단말기번호 - ETX 길이
		//sprintf((char*)VAN_BasicReq.MsgLen,                "%04d", );
		// 05  an 10 단말기 번호 "1234567890"
		sprintf((char*)VAN_BasicReq.TID,                   "%s",   g_VmConfig.TID);						
		// 0F an 2 거래(업무) 구분
		sprintf((char*)VAN_BasicReq.TransactionClass,      "%s",   g_NanoTransaction[4]);			
		// 11 an 2 전문 구분, 수집
		sprintf((char*)VAN_BasicReq.PacketClass,           "%s",   g_NanoPacket[11]);				
		// 13 an 1 입력매체 구분코드 , K:KEYIN, S:SWIPE, R:RF, I:IC, X:NON
		VAN_BasicReq.TransactionType = 'I';		
		// 14 an 5 단말기 거래일련 번호, 총 거래 Cnt
		sprintf((char*)VAN_BasicReq.TransactionCount,      "%05d", g_TmsLog.TotCnt);
		// 19 an 2 전문 버젼
		sprintf((char*)VAN_BasicReq.PacketVersion,         "%s",   "T1");					
		// 1B an 1 전문 암호화 여부 안함																	
		VAN_BasicReq.Encryption = '0';	
		// 1C an 10 예비용														
		//sprintf((char*)VAN_BasicReq.RFU1,                  "%010d", ' ');
		// 26 FS																//
		VAN_BasicReq.FS1 = FS;
		// 27 an 40 인식 카드번호 
		BCD2Char(EB_RFInfo.SelectFile_IDep, VAN_BasicReq.CSN, 8);	
		// 4F FS	
		VAN_BasicReq.FS2 = FS;    	
		// 50 an 2 할부 개월 수
		sprintf((char*)VAN_BasicReq.Installment,           "%02d", 0);	
		// 52 FS							
		VAN_BasicReq.FS3 = FS;
		// 53 an 10 승인금액   
		if(!memcmp(VAN_BasicReq.PacketClass, g_NanoPacket[11], 2))
			sprintf((char*)VAN_BasicReq.Amount,      			"0000000001");					
		else
			sprintf((char*)VAN_BasicReq.Amount,      			"%010d", nMpda);
		// 5D FS
		VAN_BasicReq.FS4 = FS;
		// 5E an 10 봉사료	
		sprintf((char*)VAN_BasicReq.ServicePrice, 			"%010d", 0);
		// 68 FS						
		VAN_BasicReq.FS5 = FS;
		// 69 an 10 부가가치세	
		sprintf((char*)VAN_BasicReq.Tax, 					"%010d", 0);
		// 73 FS												
		VAN_BasicReq.FS6 = FS;	
		// 74 an 1 전표사용구분, 0:소비자소득공제, 1:사업자지출증빙 
		VAN_BasicReq.Bill= '0';		
		// 75 FS																			
		VAN_BasicReq.FS7 = FS;	
		// 76 an 8 원거래 일자
		//memset(VAN_BasicReq.ApproveDate, ' ', 8);		
		// 7E FS								
		VAN_BasicReq.FS8 = FS;
		// 7F an 10 원거래 승인 번호
	//	memset(VAN_BasicReq.ApproveNum, ' ', 10);		
		// 89 FS											
		VAN_BasicReq.FS9 = FS;				
		PRINTHEX( (unsigned char*)EB_RFInfo.Initialize_BALep, 4, ">> Balance %08d", nBalance);
		PRINTHEX( (unsigned char*)&nMpda, 4, ">> Mpda    %08d", nMpda);
		PRINTHEX( (unsigned char*)EB_SAMInfo.CreditPSAM_BALep, 4,">> BALep   %08d", nBALep);
		PRINTHEX( (unsigned char*)NTep.data, 4, ">> NTep");
		PRINTHEX( (unsigned char*)EB_SAMInfo.CreditPSAM_IDsam, 8, ">> IDPsam");		
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//	
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// 8A an 14  거래일시 YYYYMMDD hhmmss
		BCD2Char(g_Today, VAN_ApproveReq.TransactionTime, 1);	
		BCD2Char(EB_SAMInfo.GetCardInformation.TIME, VAN_ApproveReq.TransactionTime+2, 6);	
		// 98 an 1 기관 코드		
		VAN_ApproveReq.OrganCode = UBC_CODE_EB;
		// 99 an 2 거래 종류					
		if(nTransactionType)
			sprintf((char*)VAN_ApproveReq.TransactionClass, "%s",   g_TmTransaction[1]);	
		else 
			sprintf((char*)VAN_ApproveReq.TransactionClass, "%s",   g_TmTransaction[2]);	// an 거래 종류	
		// 9B an 1 입력매체 구분코드 1:KeyI,2:SW,3:RF,4:IC,5:IR	
		VAN_ApproveReq.TransactionType = '3';							
		// 9C an 20 카드번호
		BCD2Char(EBM_Info.DecrypedCSN, VAN_ApproveReq.IDcp, 10);		
		PRINTHEX( VAN_ApproveReq.IDcp, 20, "IDep");

		// B0 an 8 거래전 잔액
		sprintf((char*)&VAN_ApproveReq.Balance, "%08d", nBalance);		
		// B8 an 8 거래 금액
		sprintf((char*)&VAN_ApproveReq.Mpda,    "%08d", nMpda);	
		// C0 an 8 거래 후 잔액			
		sprintf((char*)&VAN_ApproveReq.BALep,  "%08d", nBALep);			
		// C8 an 2 전자화폐 식별	
		BCD2Char("\x0B", VAN_ApproveReq.IDCenter, 1);	
		PRINTHEX( VAN_ApproveReq.IDCenter, 2, "IDCenter");
		// CA an 6 발행기관 ID
		//memcpy((char*)VAN_ApproveReq.RID, g_MCardInfo.AID[0].data, 5);
		//BCD2Char(g_MCardInfo.AID[0].data, VAN_ApproveReq.IDcp, 5);		
		//memcpy(VAN_ApproveReq.IDcp, g_MreturnData.RID, 10); 
		// D0 an 10 카드거래 일련 번호	
		sprintf((char*)VAN_ApproveReq.NTep, "%010u", NTep.value);
		// DA an 20 SAM ID	
		BCD2Char(EB_SAMInfo.GetData_IDsam, VAN_ApproveReq.IDPsam, 8);
		// EE an 10 SAM 거래번호		
		Swap_4(EB_SAMInfo.CreditSAM_NTpsam, lValue.data); 
		sprintf((char*)VAN_ApproveReq.NTPsam, "%010u", lValue.value);
		
		// F8 an 10 SAM 거래 총액	
		sprintf((char*)&VAN_ApproveReq.TotPsam,  "%010u", 0);
		// 102 an 2 수집 키버전		
		memcpy(VAN_ApproveReq.VkindKey, "01", 2);	
		// 104 an 10 수집 일련번호
		sprintf((char*)&VAN_ApproveReq.NCPsam,  "%010u", NCPsam.value);
		// 10E an 8 수집 건수		
		memset(VAN_ApproveReq.NIPsam, '0', sizeof(VAN_ApproveReq.NIPsam));
		//sprintf((char*)VAN_ApproveReq.NIPsam,    "%0.08d",   B2S(NIPsam[1], NIPsam[0]));
		// 116 an 2 알고리즘 ID		
		BCD2Char("\x10", VAN_ApproveReq.Algep, 1);		
		// 118 an 20 인증자(사인값)
		VAN_ApproveReq.SIGNind[0]  = EB_SAMInfo.SIGNind_Check;
		BCD2Char(EB_SAMInfo.CreditSAM_TCC, VAN_ApproveReq.SIGNind+1, 8);	
		// 12C an 2 카드구분코드(현금99) 	
		memcpy(VAN_ApproveReq.CardType, "MF", 2);	
		// 12E an 2 카드 소시자 구분	
		tmp = EBM_Info.BLOCK8_CipheredCardNumber[0]&0x0F;
		tmp += 1;
		BCD2Char(&tmp, VAN_ApproveReq.UserCode, 1);
		PRINTHEX( EBM_Info.BLOCK8_CipheredCardNumber, 10, "BLOCK8_5 %02X", tmp);
		// 130 an 20 사용식별번호 학번/사번	
		//BCD2Char(EB_RFInfo.ReadBinary_DEXP, VAN_ApproveReq.EtcID, 2);
		//BCD2Char(EB_RFInfo.ReadBinary_CSN, VAN_ApproveReq.EtcID+4, 8);
		memset( VAN_ApproveReq.EtcID, '0', 20);
		// 144 an 1 사용구분	
		VAN_ApproveReq.UseType = '2';
		// 145 an 6 매입기관 ID									
		//VAN_ApproveReq.PPpsam[6];				
		// 14B an 16 난수
		//BCD2Char((uchar*)g_MybiSamA0.RN, VAN_ApproveReq.Riep, 8);
		BCD2Char(g_MCardInfo.AID[0].data, VAN_ApproveReq.Riep, 5);
		//memcpy(VAN_ApproveReq.Riep, g_MreturnData.RID, 10);
		// 15B an 4 구매내역 개수									
		//VAN_ApproveReq.NIpp[4];	
		// 15F an 20 원거래정보 SAM ID								
		//VAN_ApproveReq._IDsam[20];	
		// 173 an 10 원거래정보	SAM 거래번호							
		//VAN_ApproveReq._NTsam[10];
		// 17D an 10 거래 일련 번호								
		//VAN_ApproveReq._NTep[10];	
		// 187 an 10 원거래정보	거래 금액 							
		//VAN_ApproveReq._Mpda[10];	
		// 191 an 14 원거래정보 YYYYMMDD hhmmss								
		//VAN_ApproveReq._TransactionTime[14];
		// 19F an 60 입차일시(10)+출차일시(10)+차량번호(20)+주차장명(14)+주차구역(6)			
		//VAN_ApproveReq.Remark[60];		
		Swap_4(EB_SAMInfo.CreditSAM_NTsam, lValue.data); 
		sprintf((char*)VAN_ApproveReq.Remark, "%010u", lValue.value);	
		Swap_4(EB_SAMInfo.ReadBalance_Balance1, lValue.data); 
		sprintf((char*)VAN_ApproveReq.Remark+10, "%010u", lValue.value);	
		Swap_4(EB_SAMInfo.ReadBalance_Balance2, lValue.data); 
		sprintf((char*)VAN_ApproveReq.Remark+20, "%010u", lValue.value);
		Swap_4("0000", lValue.data); // MPsam 마일리지 적립형 구매거래 시  주적 점수
		sprintf((char*)VAN_ApproveReq.Remark+30, "%010u", 0);				
		sprintf((char*)VAN_ApproveReq.Remark+40, "%05u", 0);		
		VAN_ApproveReq.Remark[45]=' ';
		break;	
	case RF_EB :
		memset((char*)&g_Tm, 0, sizeof(TM_TRANSACTION));
		
		Swap_4(EB_RFInfo.Initialize_BALep, (unsigned char*)&nBalance); 	// 거래 전 잔액
		Swap_4(EB_SAMInfo.CreditPSAM_Mpda, (unsigned char*)&nMpda); 	// 거래 금액
		Swap_4(EB_SAMInfo.CreditPSAM_BALep, (unsigned char*)&nBALep); 	// 거래 후 잔액
		Swap_4(EB_SAMInfo.CreditPSAM_TOTsam, (unsigned char*)&nTotPsam); 	
		Swap_4(EB_SAMInfo.CreditPSAM_NTep,NTep.data);
		Swap_4(EB_SAMInfo.CreditPSAM_NCsam, NCPsam.data); 
		
						
		// 00	
		VAN_BasicReq.stx = 0x02;
		// 01  an 4 단말기번호 - ETX 길이
		//sprintf((char*)VAN_BasicReq.MsgLen,                "%04d", );
		// 05  an 10 단말기 번호 "1234567890"
		sprintf((char*)VAN_BasicReq.TID,                   "%s",   g_VmConfig.TID);						
		// 0F an 2 거래(업무) 구분
		sprintf((char*)VAN_BasicReq.TransactionClass,      "%s",   g_NanoTransaction[4]);			
		// 11 an 2 전문 구분, 수집
		sprintf((char*)VAN_BasicReq.PacketClass,           "%s",   g_NanoPacket[11]);				
		// 13 an 1 입력매체 구분코드 , K:KEYIN, S:SWIPE, R:RF, I:IC, X:NON
		VAN_BasicReq.TransactionType = 'I';		
		// 14 an 5 단말기 거래일련 번호, 총 거래 Cnt
		sprintf((char*)VAN_BasicReq.TransactionCount,      "%05d", g_TmsLog.TotCnt);
		// 19 an 2 전문 버젼
		sprintf((char*)VAN_BasicReq.PacketVersion,         "%s",   "T1");					
		// 1B an 1 전문 암호화 여부 안함																	
		VAN_BasicReq.Encryption = '0';	
		// 1C an 10 예비용														
		//sprintf((char*)VAN_BasicReq.RFU1,                  "%010d", ' ');
		// 26 FS																//
		VAN_BasicReq.FS1 = FS;
		// 27 an 40 인식 카드번호 
		BCD2Char(EB_RFInfo.SelectFile_IDep, VAN_BasicReq.CSN, 8);	
		// 4F FS	
		VAN_BasicReq.FS2 = FS;    	
		// 50 an 2 할부 개월 수
		sprintf((char*)VAN_BasicReq.Installment,           "%02d", 0);	
		// 52 FS							
		VAN_BasicReq.FS3 = FS;
		// 53 an 10 승인금액   
		if(!memcmp(VAN_BasicReq.PacketClass, g_NanoPacket[11], 2))
			sprintf((char*)VAN_BasicReq.Amount,      			"0000000001");					
		else
			sprintf((char*)VAN_BasicReq.Amount,      			"%010d", nMpda);
		// 5D FS
		VAN_BasicReq.FS4 = FS;
		// 5E an 10 봉사료	
		sprintf((char*)VAN_BasicReq.ServicePrice, 			"%010d", 0);
		// 68 FS						
		VAN_BasicReq.FS5 = FS;
		// 69 an 10 부가가치세	
		sprintf((char*)VAN_BasicReq.Tax, 					"%010d", 0);
		// 73 FS												
		VAN_BasicReq.FS6 = FS;	
		// 74 an 1 전표사용구분, 0:소비자소득공제, 1:사업자지출증빙 
		VAN_BasicReq.Bill= '0';		
		// 75 FS																			
		VAN_BasicReq.FS7 = FS;	
		// 76 an 8 원거래 일자
		//memset(VAN_BasicReq.ApproveDate, ' ', 8);		
		// 7E FS								
		VAN_BasicReq.FS8 = FS;
		// 7F an 10 원거래 승인 번호
	//	memset(VAN_BasicReq.ApproveNum, ' ', 10);		
		// 89 FS											
		VAN_BasicReq.FS9 = FS;				
		PRINTHEX( (unsigned char*)EB_RFInfo.Initialize_BALep, 4, ">> Balance %08d", nBalance);
		PRINTHEX( (unsigned char*)EB_SAMInfo.CreditPSAM_Mpda, 4, ">> Mpda    %08d", nMpda);
		PRINTHEX( (unsigned char*)EB_SAMInfo.CreditPSAM_BALep, 4,">> BALep   %08d", nBALep);
		PRINTHEX( (unsigned char*)NTep.data, 4, ">> NTep");
		PRINTHEX( (unsigned char*)EB_SAMInfo.CreditPSAM_IDsam, 8, ">> IDPsam");		
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//	
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// 8A an 14  거래일시 YYYYMMDD hhmmss
		BCD2Char(g_Today, VAN_ApproveReq.TransactionTime, 7);	
		// 98 an 1 기관 코드		
		VAN_ApproveReq.OrganCode = UBC_CODE_EB;
		// 99 an 2 거래 종류					
		if(nTransactionType)
			sprintf((char*)VAN_ApproveReq.TransactionClass, "%s",   g_TmTransaction[1]);	
		else 
			sprintf((char*)VAN_ApproveReq.TransactionClass, "%s",   g_TmTransaction[2]);	// an 거래 종류	
		// 9B an 1 입력매체 구분코드 1:KeyI,2:SW,3:RF,4:IC,5:IR	
		VAN_ApproveReq.TransactionType = '3';							
		// 9C an 20 카드번호
		BCD2Char(EB_SAMInfo.CreditPSAM_IDep, VAN_ApproveReq.IDcp, 8);	
		PRINTHEX( VAN_ApproveReq.IDcp, 20, "IDep");

		// B0 an 8 거래전 잔액
		sprintf((char*)&VAN_ApproveReq.Balance, "%08d", nBalance);		
		// B8 an 8 거래 금액
		sprintf((char*)&VAN_ApproveReq.Mpda,    "%08d", nMpda);	
		// C0 an 8 거래 후 잔액			
		sprintf((char*)&VAN_ApproveReq.BALep,  "%08d", nBALep);			
		// C8 an 2 전자화폐 식별	
		BCD2Char(&EB_SAMInfo.CreditPSAM_IDcenter, VAN_ApproveReq.IDCenter, 1);	
		PRINTHEX( VAN_ApproveReq.IDCenter, 2, "IDCenter");
		// CA an 6 발행기관 ID
		//memcpy((char*)VAN_ApproveReq.RID, g_MCardInfo.AID[0].data, 5);
		//BCD2Char(g_MCardInfo.AID[0].data, VAN_ApproveReq.IDcp, 5);		
		//memcpy(VAN_ApproveReq.IDcp, g_MreturnData.RID, 10); 
		// D0 an 10 카드거래 일련 번호	
		sprintf((char*)VAN_ApproveReq.NTep, "%010u", NTep.value);
		// DA an 20 SAM ID			 
		#if 0	
		BCD2Char((uchar*)g_MybiSamA0.ID, VAN_ApproveReq.IDPsam, 8);
		//memcpy(VAN_ApproveReq.IDPsam, g_MybiSamA0.ID, 8);
		#else
		BCD2Char(EB_SAMInfo.CreditPSAM_IDsam, VAN_ApproveReq.IDPsam, 8);
		#endif			
		// EE an 10 SAM 거래번호
		Swap_4(EB_SAMInfo.CreditPSAM_NTpsam, lValue.data); 
		sprintf((char*)VAN_ApproveReq.NTPsam, "%010u", lValue.value);
		// F8 an 10 SAM 거래 총액	
		sprintf((char*)&VAN_ApproveReq.TotPsam,  "%010u", nTotPsam);
		// 102 an 2 수집 키버전		
		BCD2Char(&EB_SAMInfo.CreditPSAM_VKpind, VAN_ApproveReq.VkindKey, 1);	
		// 104 an 10 수집 일련번호
		sprintf((char*)&VAN_ApproveReq.NCPsam,  "%010u", NCPsam.value);		
		PRINTHEX( &EB_SAMInfo.CreditPSAM_TRT, 53, "NCsam");
		// 10E an 8 수집 건수		
		memset(VAN_ApproveReq.NIPsam, '0', sizeof(VAN_ApproveReq.NIPsam));
		//sprintf((char*)VAN_ApproveReq.NIPsam,    "%0.08d",   B2S(NIPsam[1], NIPsam[0]));
		// 116 an 2 알고리즘 ID		
		BCD2Char(&EB_RFInfo.Initialize_ALGep, VAN_ApproveReq.Algep, 1);		
		// 118 an 20 인증자(사인값)
		VAN_ApproveReq.SIGNind[0] = EB_SAMInfo.SIGNind_Check;
		BCD2Char(EB_SAMInfo.CreditPSAM_SIGNind, VAN_ApproveReq.SIGNind+1, 4);	
		// 12C an 2 카드구분코드(현금99) 	
		memcpy(VAN_ApproveReq.CardType, "EB", 2);	
		// 12E an 2 카드 소시자 구분			
		BCD2Char(&EB_RFInfo.SelectFile_USERCODE, VAN_ApproveReq.UserCode, 1);
		//BCD2Char(&EB_RFInfo.BLOCK8_IssuerDate&0x0F, VAN_ApproveReq.UserCode, 1);
		// 130 an 20 사용식별번호 학번/사번	
		BCD2Char(EB_RFInfo.ReadBinary_DEXP, VAN_ApproveReq.EtcID, 2);
		BCD2Char(EB_RFInfo.ReadBinary_CSN, VAN_ApproveReq.EtcID+4, 8);
		// 144 an 1 사용구분	
		VAN_ApproveReq.UseType = '2';
		// 145 an 6 매입기관 ID									
		//VAN_ApproveReq.PPpsam[6];				
		// 14B an 16 난수
		//BCD2Char((uchar*)g_MybiSamA0.RN, VAN_ApproveReq.Riep, 8);
		BCD2Char(g_MCardInfo.AID[0].data, VAN_ApproveReq.Riep, 5);
		//memcpy(VAN_ApproveReq.Riep, g_MreturnData.RID, 10);
		// 15B an 4 구매내역 개수									
		//VAN_ApproveReq.NIpp[4];	
		// 15F an 20 원거래정보 SAM ID								
		//VAN_ApproveReq._IDsam[20];	
		// 173 an 10 원거래정보	SAM 거래번호							
		//VAN_ApproveReq._NTsam[10];
		// 17D an 10 거래 일련 번호								
		//VAN_ApproveReq._NTep[10];	
		// 187 an 10 원거래정보	거래 금액 							
		//VAN_ApproveReq._Mpda[10];	
		// 191 an 14 원거래정보 YYYYMMDD hhmmss								
		//VAN_ApproveReq._TransactionTime[14];
		// 19F an 60 입차일시(10)+출차일시(10)+차량번호(20)+주차장명(14)+주차구역(6)			
		//VAN_ApproveReq.Remark[60];		
		Swap_4(EB_SAMInfo.CreditPSAM_NTsam, lValue.data); 
		sprintf((char*)VAN_ApproveReq.Remark, "%010u", lValue.value);	
		Swap_4(EB_SAMInfo.ReadBalance_Balance1, lValue.data); 
		sprintf((char*)VAN_ApproveReq.Remark+10, "%010u", lValue.value);	
		Swap_4(EB_SAMInfo.ReadBalance_Balance2, lValue.data); 
		sprintf((char*)VAN_ApproveReq.Remark+20, "%010u", lValue.value);
		Swap_4("0000", lValue.data); // MPsam 마일리지 적립형 구매거래 시  주적 점수
		sprintf((char*)VAN_ApproveReq.Remark+30, "%010u", lValue.value);				
		sprintf((char*)VAN_ApproveReq.Remark+40, "%05u", EB_SAMInfo.CreditPSAM_NIsam[1]+EB_SAMInfo.CreditPSAM_NIsam[0]*256);		
		VAN_ApproveReq.Remark[45]=' ';
		break;	
	case RF_TMONEY : 
		nBalance = B2I(g_Tm.Balance.data[3],g_Tm.Balance.data[2],g_Tm.Balance.data[1],g_Tm.Balance.data[0]);
		nMpda    = B2I(g_Tm.Mpda.data[3],g_Tm.Mpda.data[2],g_Tm.Mpda.data[1],g_Tm.Mpda.data[0]);
		nBALep   = B2I(g_Tm.BALep.data[3],g_Tm.BALep.data[2],g_Tm.BALep.data[1],g_Tm.BALep.data[0]);
		nTotPsam = B2I(g_Tm.TotPsam.data[3],g_Tm.TotPsam.data[2],g_Tm.TotPsam.data[1],g_Tm.TotPsam.data[0]);
		NTep.value = g_Tm.NTep.value;	// 카드거래 일련 번호
		NCPsam.value = g_Tm.NCPsam.value;
						
		// 00	
		VAN_BasicReq.stx = 0x02;
		// 01  an 4 단말기번호 - ETX 길이
		//sprintf((char*)VAN_BasicReq.MsgLen,                "%04d", );
		// 05  an 10 단말기 번호 "1234567890"
		sprintf((char*)VAN_BasicReq.TID,                   "%s",   g_VmConfig.TID);						
		// 0F an 2 거래(업무) 구분
		sprintf((char*)VAN_BasicReq.TransactionClass,      "%s",   g_NanoTransaction[4]);			
		// 11 an 2 전문 구분, 수집
		sprintf((char*)VAN_BasicReq.PacketClass,           "%s",   g_NanoPacket[11]);				
		// 13 an 1 입력매체 구분코드 , K:KEYIN, S:SWIPE, R:RF, I:IC, X:NON
		VAN_BasicReq.TransactionType = 'I';		
		// 14 an 5 단말기 거래일련 번호, 총 거래 Cnt
		sprintf((char*)VAN_BasicReq.TransactionCount,      "%05d", g_TmsLog.TotCnt);
		// 19 an 2 전문 버젼
		sprintf((char*)VAN_BasicReq.PacketVersion,         "%s",   "T1");					
		// 1B an 1 전문 암호화 여부 안함																	
		VAN_BasicReq.Encryption = '0';	
		// 1C an 10 예비용														
		//sprintf((char*)VAN_BasicReq.RFU1,                  "%010d", ' ');
		// 26 FS																//
		VAN_BasicReq.FS1 = FS;
		// 27 an 40 인식 카드번호 
		//sprintf((char*)VAN_BasicReq.RFU1,                  "%040d", ' ');
		// 4F FS	
		VAN_BasicReq.FS2 = FS;    	
		// 50 an 2 할부 개월 수
		sprintf((char*)VAN_BasicReq.Installment,           "%02d", 0);	
		// 52 FS							
		VAN_BasicReq.FS3 = FS;
		// 53 an 10 승인금액   
		if(!memcmp(VAN_BasicReq.PacketClass, g_NanoPacket[11], 2))
			sprintf((char*)VAN_BasicReq.Amount,      			"0000000001");					
		else
			sprintf((char*)VAN_BasicReq.Amount,      			"%010d", nMpda);
		// 5D FS
		VAN_BasicReq.FS4 = FS;
		// 5E an 10 봉사료	
		sprintf((char*)VAN_BasicReq.ServicePrice, 			"%010d", 0);
		// 68 FS						
		VAN_BasicReq.FS5 = FS;
		// 69 an 10 부가가치세	
		sprintf((char*)VAN_BasicReq.Tax, 					"%010d", 0);
		// 73 FS												
		VAN_BasicReq.FS6 = FS;	
		// 74 an 1 전표사용구분, 0:소비자소득공제, 1:사업자지출증빙 
		VAN_BasicReq.Bill= '0';		
		// 75 FS																			
		VAN_BasicReq.FS7 = FS;	
		// 76 an 8 원거래 일자
		//memset(VAN_BasicReq.ApproveDate, ' ', 8);		
		// 7E FS								
		VAN_BasicReq.FS8 = FS;
		// 7F an 10 원거래 승인 번호
	//	memset(VAN_BasicReq.ApproveNum, ' ', 10);		
		// 89 FS											
		VAN_BasicReq.FS9 = FS;
	#if 0					
		PRINTHEX( (unsigned char*)&g_Tm.Balance, 4, ">> Balance %08d", nBalance);
		PRINTHEX( (unsigned char*)&g_Tm.Mpda, 4, 	">> Mpda    %08d", nMpda);
		PRINTHEX( (unsigned char*)&g_Tm.BALep, 4, 	">> BALep   %08d", nBALep);
		PRINTHEX( (unsigned char*)&g_Tm.NTep, 4, 	">> NTep");
		PRINTHEX( (unsigned char*)&g_Tm.IDPsam, sizeof(g_Tm.IDPsam), 	">> IDPsam");
	#endif	
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//	
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// 8A an 14  거래일시 YYYYMMDD hhmmss
		BCD2Char(g_Today, VAN_ApproveReq.TransactionTime, 7);	
		// 98 an 1 기관 코드		
		VAN_ApproveReq.OrganCode = UBC_CODE_TMONEY;
		// 99 an 2 거래 종류					
		if(nTransactionType)
			sprintf((char*)VAN_ApproveReq.TransactionClass, "%s",   g_TmTransaction[1]);	
		else 
			sprintf((char*)VAN_ApproveReq.TransactionClass, "%s",   g_TmTransaction[2]);	// an 거래 종류	
		// 9B an 1 입력매체 구분코드 1:KeyI,2:SW,3:RF,4:IC,5:IR	
		VAN_ApproveReq.TransactionType = '3';							
		// 9C an 20 카드번호		
		BCD2Char(g_Tm.IDep, VAN_ApproveReq.IDcp, 8);
		// B0 an 8 거래전 잔액
		sprintf((char*)&VAN_ApproveReq.Balance, "%08d", nBalance);		
		// B8 an 8 거래 금액
		sprintf((char*)&VAN_ApproveReq.Mpda,    "%08d", nMpda);	
		// C0 an 8 거래 후 잔액			
		sprintf((char*)&VAN_ApproveReq.BALep,  "%08d", nBALep);			
		// C8 an 2 전자화폐 식별	
		BCD2Char((uchar*)&g_Tm.IDcenter, (uchar*)VAN_ApproveReq.IDCenter, 1);
		// CA an 6 발행기관 ID
		sprintf((char*)VAN_ApproveReq.RID, 	       "%s",   "      ");	
		// D0 an 10 카드거래 일련 번호	
		sprintf((char*)VAN_ApproveReq.NTep, 	   "%0.10d", ByteToInt((uchar*)&NTep));
		// DA an 20 SAM ID
		BCD2Char((uchar*)&g_Tm.IDPsam, VAN_ApproveReq.IDPsam, 8);
		// EE an 10 SAM 거래번호
		sprintf((char*)VAN_ApproveReq.NTPsam, 	   "%0.10d", ByteToInt((uchar*)&g_Tm.NTPsam));
		// F8 an 10 SAM 거래 총액	
		sprintf((char*)&VAN_ApproveReq.TotPsam, "%0.10d", nTotPsam);
		// 102 an 2 수집 키버전		
		BCD2Char((uchar*)&g_Tm.VkindKey, VAN_ApproveReq.VkindKey, 4);	
		// 104 an 10 수집 일련번호
		BCD2Char((uchar*)&g_Tm.NCPsam, VAN_ApproveReq.NCPsam, 4);
		// 10E an 8 수집 건수		
		sprintf((char*)VAN_ApproveReq.NIPsam,    "%0.08d",   B2S(g_Tm.NIPsam[1], g_Tm.NIPsam[0]));
		// 116 an 2 알고리즘 ID		
		BCD2Char((uchar*)&g_Tm.AlgID, VAN_ApproveReq.Algep, 1);			
		// 118 an 20 인증자(사인값)
		BCD2Char((uchar*)&g_Tm.SIGNIND, VAN_ApproveReq.SIGNind, 4);	
		// 12C an 2 카드구분코드(현금99) 	
		BCD2Char((uchar*)&g_Tm.CardType, VAN_ApproveReq.CardType, 1);
		// 12E an 2 카드 소시자 구분	
		BCD2Char((uchar*)&g_Tm.UserCode, VAN_ApproveReq.UserCode, 1);		
		// 130 an 20 사용식별번호 학번/사번	
		memset((char*)VAN_ApproveReq.EtcID, ' ', 20);	
		// 144 an 1 사용구분	
		//VAN_ApproveReq.UseType = '2'; break;
		// 145 an 6 매입기관 ID									
		//VAN_ApproveReq.PPpsam[6];				
		// 14B an 16 난수		
		//BCD2Char((uchar*)g_MybiSamA0.RN, VAN_ApproveReq.Riep, 8);
		// 15B an 4 구매내역 개수									
		//VAN_ApproveReq.NIpp[4];	
		// 15F an 20 원거래정보 SAM ID								
		//VAN_ApproveReq._IDsam[20];	
		// 173 an 10 원거래정보	SAM 거래번호							
		//VAN_ApproveReq._NTsam[10];
		// 17D an 10 거래 일련 번호								
		//VAN_ApproveReq._NTep[10];	
		// 187 an 10 원거래정보	거래 금액 							
		//VAN_ApproveReq._Mpda[10];	
		// 191 an 14 원거래정보 YYYYMMDD hhmmss								
		//VAN_ApproveReq._TransactionTime[14];
		// 19F an 60 입차일시(10)+출차일시(10)+차량번호(20)+주차장명(14)+주차구역(6)			
		//VAN_ApproveReq.Remark[60];
		break;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	case RF_MYBI :	
	case RF_USAM :
		memset((char*)&g_Tm, 0, sizeof(TM_TRANSACTION));
		
		nBalance = g_MCardInfo.bAmount.value;
		nMpda    = g_MCardInfo.lFare.value;
		nBALep   = g_MCardInfo.value1.value;
		nTotPsam = 0;
		NTep.value = 0;
		memcpy(NCPsam.data, "\x11\x11\x11\x11", 4); 
		
		
		memcpy(g_Tm.SIGNIND.data, g_MybiMac, 4);
				
		// 00	
		VAN_BasicReq.stx = 0x02;
		// 01  an 4 단말기번호 - ETX 길이
		//sprintf((char*)VAN_BasicReq.MsgLen,                "%04d", );
		// 05  an 10 단말기 번호 "1234567890"
		sprintf((char*)VAN_BasicReq.TID,                   "%s",   g_VmConfig.TID);						
		// 0F an 2 거래(업무) 구분
		sprintf((char*)VAN_BasicReq.TransactionClass,      "%s",   g_NanoTransaction[4]);			
		// 11 an 2 전문 구분, 수집
		sprintf((char*)VAN_BasicReq.PacketClass,           "%s",   g_NanoPacket[11]);				
		// 13 an 1 입력매체 구분코드 , K:KEYIN, S:SWIPE, R:RF, I:IC, X:NON
		VAN_BasicReq.TransactionType = 'I';		
		// 14 an 5 단말기 거래일련 번호, 총 거래 Cnt
		sprintf((char*)VAN_BasicReq.TransactionCount,      "%05d", g_TmsLog.TotCnt);
		// 19 an 2 전문 버젼
		sprintf((char*)VAN_BasicReq.PacketVersion,         "%s",   "T1");					
		// 1B an 1 전문 암호화 여부 안함																	
		VAN_BasicReq.Encryption = '0';	
		// 1C an 10 예비용														
		//sprintf((char*)VAN_BasicReq.RFU1,                  "%010d", ' ');
		// 26 FS																//
		VAN_BasicReq.FS1 = FS;
		// 27 an 40 인식 카드번호 
		//sprintf((char*)VAN_BasicReq.RFU1,                  "%040d", ' ');
		// 4F FS	
		VAN_BasicReq.FS2 = FS;    	
		// 50 an 2 할부 개월 수
		sprintf((char*)VAN_BasicReq.Installment,           "%02d", 0);	
		// 52 FS							
		VAN_BasicReq.FS3 = FS;
		// 53 an 10 승인금액   
		if(!memcmp(VAN_BasicReq.PacketClass, g_NanoPacket[11], 2))
			sprintf((char*)VAN_BasicReq.Amount,      			"0000000001");					
		else
			sprintf((char*)VAN_BasicReq.Amount,      			"%010d", nMpda);
		// 5D FS
		VAN_BasicReq.FS4 = FS;
		// 5E an 10 봉사료	
		sprintf((char*)VAN_BasicReq.ServicePrice, 			"%010d", 0);
		// 68 FS						
		VAN_BasicReq.FS5 = FS;
		// 69 an 10 부가가치세	
		sprintf((char*)VAN_BasicReq.Tax, 					"%010d", 0);
		// 73 FS												
		VAN_BasicReq.FS6 = FS;	
		// 74 an 1 전표사용구분, 0:소비자소득공제, 1:사업자지출증빙 
		VAN_BasicReq.Bill= '0';		
		// 75 FS																			
		VAN_BasicReq.FS7 = FS;	
		// 76 an 8 원거래 일자
		//memset(VAN_BasicReq.ApproveDate, ' ', 8);		
		// 7E FS								
		VAN_BasicReq.FS8 = FS;
		// 7F an 10 원거래 승인 번호
	//	memset(VAN_BasicReq.ApproveNum, ' ', 10);		
		// 89 FS											
		VAN_BasicReq.FS9 = FS;
	#if 0					
		PRINTHEX( (unsigned char*)&g_Tm.Balance, 4, ">> Balance %08d", nBalance);
		PRINTHEX( (unsigned char*)&g_Tm.Mpda, 4, 	">> Mpda    %08d", nMpda);
		PRINTHEX( (unsigned char*)&g_Tm.BALep, 4, 	">> BALep   %08d", nBALep);
		PRINTHEX( (unsigned char*)&g_Tm.NTep, 4, 	">> NTep");
		PRINTHEX( (unsigned char*)&g_Tm.IDPsam, sizeof(g_Tm.IDPsam), 	">> IDPsam");
	#endif	
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//	
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// 8A an 14  거래일시 YYYYMMDD hhmmss
		BCD2Char(g_Today, VAN_ApproveReq.TransactionTime, 7);	
		// 98 an 1 기관 코드		
		VAN_ApproveReq.OrganCode = UBC_CODE_MYBI;
		// 99 an 2 거래 종류					
		if(nTransactionType)
			sprintf((char*)VAN_ApproveReq.TransactionClass, "%s",   g_TmTransaction[1]);	
		else 
			sprintf((char*)VAN_ApproveReq.TransactionClass, "%s",   g_TmTransaction[2]);	// an 거래 종류	
		// 9B an 1 입력매체 구분코드 1:KeyI,2:SW,3:RF,4:IC,5:IR	
		VAN_ApproveReq.TransactionType = '3';							
		// 9C an 20 카드번호
	#if 1
		VAN_ApproveReq.IDcp[0]= 0x30 + g_MybiCsn.value/1000000000;  g_MybiCsn.value = g_MybiCsn.value%1000000000;
		VAN_ApproveReq.IDcp[1]= 0x30 + g_MybiCsn.value/100000000;	g_MybiCsn.value = g_MybiCsn.value%100000000;
		VAN_ApproveReq.IDcp[2]= 0x30 + g_MybiCsn.value/10000000;	g_MybiCsn.value = g_MybiCsn.value%10000000;
		VAN_ApproveReq.IDcp[3]= 0x30 + g_MybiCsn.value/1000000;		g_MybiCsn.value = g_MybiCsn.value%1000000;
		VAN_ApproveReq.IDcp[4]= 0x30 + g_MybiCsn.value/100000;		g_MybiCsn.value = g_MybiCsn.value%100000;
		VAN_ApproveReq.IDcp[5]= 0x30 + g_MybiCsn.value/10000;		g_MybiCsn.value = g_MybiCsn.value%10000;
		VAN_ApproveReq.IDcp[6]= 0x30 + g_MybiCsn.value/1000;		g_MybiCsn.value = g_MybiCsn.value%1000;
		VAN_ApproveReq.IDcp[7]= 0x30 + g_MybiCsn.value/100;			g_MybiCsn.value = g_MybiCsn.value%100;
		VAN_ApproveReq.IDcp[8]= 0x30 + g_MybiCsn.value/10;			g_MybiCsn.value = g_MybiCsn.value%10;
		VAN_ApproveReq.IDcp[9]= 0x30 + g_MybiCsn.value;
		
		//HEX_Dump( (unsigned char*)g_MybiCsn.data, 4, "g_MybiCsn %d", ~g_MybiCsn.value + 1);
	#else
		sprintf((char*)VAN_ApproveReq.IDcp, "%010u",   ~g_MybiCsn.value + 1);
	#endif
		// B0 an 8 거래전 잔액
		sprintf((char*)&VAN_ApproveReq.Balance, "%08d", nBalance);		
		// B8 an 8 거래 금액
		sprintf((char*)&VAN_ApproveReq.Mpda,    "%08d", nMpda);	
		// C0 an 8 거래 후 잔액			
		sprintf((char*)&VAN_ApproveReq.BALep,  "%08d", nBALep);			
		// C8 an 2 전자화폐 식별	
		memcpy((char*)VAN_ApproveReq.IDCenter, "  ", 2);
		// CA an 6 발행기관 ID
		//memcpy((char*)VAN_ApproveReq.RID, g_MCardInfo.AID[0].data, 5);
		//BCD2Char(g_MCardInfo.AID[0].data, VAN_ApproveReq.IDcp, 5);		
		//memcpy(VAN_ApproveReq.IDcp, g_MreturnData.RID, 10); 
		// D0 an 10 카드거래 일련 번호	
		sprintf((char*)VAN_ApproveReq.NTep, "%010d", g_MCardInfo.transactionCount1.value);
		// DA an 20 SAM ID			 
		#if 0	
		BCD2Char((uchar*)g_MybiSamA0.ID, VAN_ApproveReq.IDPsam, 8);
		//memcpy(VAN_ApproveReq.IDPsam, g_MybiSamA0.ID, 8);
		#else
		BCD2Char((uchar*)g_pMSAM_CSN.data, VAN_ApproveReq.IDPsam, 4);
		#endif		
		// EE an 10 SAM 거래번호
		//memset((char*)VAN_ApproveReq.NTPsam, 	' ', sizeof(VAN_ApproveReq.NTPsam));
		// F8 an 10 SAM 거래 총액	
		//memset((char*)VAN_ApproveReq.TotPsam, 	' ', sizeof(VAN_ApproveReq.TotPsam));
		// 102 an 2 수집 키버전		
		//BCD2Char((uchar*)&g_Tm.VkindKey, VAN_ApproveReq.VkindKey, 4);	
		// 104 an 10 수집 일련번호
		// BCD2Char((uchar*)&NCPsam, VAN_ApproveReq.NCPsam, 4);
		// 10E an 8 수집 건수		
		//sprintf((char*)VAN_ApproveReq.NIPsam,    "%0.08d",   B2S(NIPsam[1], NIPsam[0]));
		// 116 an 2 알고리즘 ID		
		//BCD2Char((uchar*)&g_Tm.AlgID, VAN_ApproveReq.Algep, 1);			
		// 118 an 20 인증자(사인값)
		BCD2Char((uchar*)&g_Tm.SIGNIND, VAN_ApproveReq.SIGNind, 4);	
		// 12C an 2 카드구분코드(현금99) 	
		BCD2Char((uchar*)&g_MCardInfo.cardCode.data[0], VAN_ApproveReq.CardType, 1);
		// 12E an 2 카드 소시자 구분	
		BCD2Char((uchar*)&g_MCardInfo.cardCode.data[1], VAN_ApproveReq.UserCode, 1);
		//HEX_Dump( (unsigned char*)VAN_ApproveReq.CardType, 2, "MCO %d", 2);
		//HEX_Dump( (unsigned char*)VAN_ApproveReq.UserCode, 2, "MCO %d", 2);
		//HEX_Dump( (unsigned char*)g_MCardInfo.cardCode.data, 4, "MCO %d", 4);
		//HEX_Dump( (unsigned char*)g_MreturnData.cardType.data, 4, "MCO %d", 4);		
		// 130 an 20 사용식별번호 학번/사번	
		memset((char*)VAN_ApproveReq.EtcID, ' ', 20);	
		// 144 an 1 사용구분	
		VAN_ApproveReq.UseType = '2';
		// 145 an 6 매입기관 ID									
		//VAN_ApproveReq.PPpsam[6];				
		// 14B an 16 난수
		//BCD2Char((uchar*)g_MybiSamA0.RN, VAN_ApproveReq.Riep, 8);
		BCD2Char(g_MCardInfo.AID[0].data, VAN_ApproveReq.Riep, 5);
		//memcpy(VAN_ApproveReq.Riep, g_MreturnData.RID, 10);
		// 15B an 4 구매내역 개수									
		//VAN_ApproveReq.NIpp[4];	
		// 15F an 20 원거래정보 SAM ID								
		//VAN_ApproveReq._IDsam[20];	
		// 173 an 10 원거래정보	SAM 거래번호							
		//VAN_ApproveReq._NTsam[10];
		// 17D an 10 거래 일련 번호								
		//VAN_ApproveReq._NTep[10];	
		// 187 an 10 원거래정보	거래 금액 							
		//VAN_ApproveReq._Mpda[10];	
		// 191 an 14 원거래정보 YYYYMMDD hhmmss								
		//VAN_ApproveReq._TransactionTime[14];
		// 19F an 60 입차일시(10)+출차일시(10)+차량번호(20)+주차장명(14)+주차구역(6)			
		//VAN_ApproveReq.Remark[60];
		break;	
	}
	// 1DF an 13 필터									
	memset(VAN_ApproveReq.Filtter, 'A', 13);
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	memcpy((char*)VAN_BasicReq.AddedInfo, (char*)&VAN_ApproveReq, sizeof(NanoVANPrepaidApproveReq));	

	
	// 1E8 FS											
	VAN_BasicReq.AddedInfo[sizeof(NanoVANPrepaidApproveReq)+0]  = FS;				
	// IE9 an 판매된 Column
	BCD2Char((uchar*)&g_VmAmountColumn.Col, (uchar*)&VAN_BasicReq.AddedInfo[sizeof(NanoVANPrepaidApproveReq)+1], 2);	// an 현재 컬럼.
						
	// 1EB 
	VAN_BasicReq.AddedInfo[sizeof(NanoVANPrepaidApproveReq)+3] = 0x03;

	// 단말기번호 ~ ETX까지의 길이
	pdata = VAN_BasicReq.TID;
	for(i=0; i<sizeof(NanoVANBasicReq) ; i++) {
		if(pdata[i]==0x03) break;
	}
	sprintf((char*)buf,  "%04d", i+1);
	memcpy((char*)VAN_BasicReq.MsgLen, buf, 4);
	
	// CRC
	crc = (unsigned short)CRC_Update((unsigned char*)&VAN_BasicReq, Char2Int(VAN_BasicReq.MsgLen, 0)+5);	
	Int2Byte(crc, &VAN_BasicReq.AddedInfo[sizeof(NanoVANPrepaidApproveReq)+4]);
		
	g_NanoVANBasicReq = VAN_BasicReq;	
	g_CreditAuth = 0;	

	HEXDUMP( (unsigned char*)&g_NanoVANBasicReq, Char2Int(g_NanoVANBasicReq.MsgLen, 0)+5+2, "LEN > %d", Char2Int(g_NanoVANBasicReq.MsgLen, 0)+5);
	
	switch(nTransactionType) {	
	case 0 :
		g_VmAmountColumn.Type = '7';		
		play_wav("adpcm_거래취소_8bit.bin"); 	
		break;				
	case 1 :
	//case 58 :	// 대체 서명.
	case 59 : 	// 대체 서명 검증.
		play_wav("adpcm_결제가되었습니다_8.bin");	
		break;
	}		

	UBC_TmsBackUpProcess(nMpda);	
	UBC_Backup();
	
}

void UBC_OperatingInfoReq(int nType)
{		
	int i;
	unsigned char *pdata, buf[8];
	unsigned short crc;
	
	memset((char*)&VAN_ApproveReq, ' ', sizeof(NanoVANPrepaidApproveReq));	
	memset((char*)&VAN_BasicReq, ' ', sizeof(NanoVANBasicReq));	
					
	// 00	
	VAN_BasicReq.stx = 0x02;
	// 01  an 4 단말기번호 - ETX 길이
	//sprintf((char*)VAN_BasicReq.MsgLen,                "%04d", );
	// 05  an 10 단말기 번호 "1234567890"
	sprintf((char*)VAN_BasicReq.TID,                   "%s",   g_VmConfig.TID);						
	// 0F an 2 거래(업무) 구분
	sprintf((char*)VAN_BasicReq.TransactionClass,      "%s",   g_NanoTransaction[4]);			
	// 11 an 2 전문 구분, 운영정보
	sprintf((char*)VAN_BasicReq.PacketClass,           "%s",   g_NanoPacket[12]);				
	// 13 an 1 입력매체 구분코드 , K:KEYIN, S:SWIPE, R:RF, I:IC, X:NON
	VAN_BasicReq.TransactionType = 'I';		
	// 14 an 5 단말기 거래일련 번호, 총 거래 Cnt
	//sprintf((char*)VAN_BasicReq.TransactionCount,      "%05d", g_TmsLog.TotCnt);
	// 19 an 2 전문 버젼
	sprintf((char*)VAN_BasicReq.PacketVersion,         "%s",   "T1");					
	// 1B an 1 전문 암호화 여부 안함																	
	VAN_BasicReq.Encryption = '0';	
	// 1C an 10 예비용														
	//sprintf((char*)VAN_BasicReq.RFU1,                  "%010d", ' ');
	// 26 FS																//
	VAN_BasicReq.FS1 = FS;
	// 27 an 40 인식 카드번호 
	//sprintf((char*)VAN_BasicReq.RFU1,                  "%040d", ' ');
	// 4F FS	
	VAN_BasicReq.FS2 = FS;    	
	// 50 an 2 할부 개월 수
	//sprintf((char*)VAN_BasicReq.Installment,           "%02d", 0);	
	// 52 FS							
	VAN_BasicReq.FS3 = FS;
	// 53 an 10 승인금액   
	//sprintf((char*)VAN_BasicReq.Amount,      			"0000000001");
	// 5D FS
	VAN_BasicReq.FS4 = FS;
	// 5E an 10 봉사료	
	//sprintf((char*)VAN_BasicReq.ServicePrice, 			"%010d", 0);
	// 68 FS						
	VAN_BasicReq.FS5 = FS;
	// 69 an 10 부가가치세	
	//sprintf((char*)VAN_BasicReq.Tax, 					"%010d", 0);
	// 73 FS												
	VAN_BasicReq.FS6 = FS;	
	// 74 an 1 전표사용구분, 0:소비자소득공제, 1:사업자지출증빙 
	//VAN_BasicReq.Bill= '0';		
	// 75 FS																			
	VAN_BasicReq.FS7 = FS;	
	// 76 an 8 원거래 일자
	//memset(VAN_BasicReq.ApproveDate, ' ', 8);		
	// 7E FS								
	VAN_BasicReq.FS8 = FS;
	// 7F an 10 원거래 승인 번호
//	memset(VAN_BasicReq.ApproveNum, ' ', 10);		
	// 89 FS											
	VAN_BasicReq.FS9 = FS;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 8A an 5  운영정보 관리번호
	sprintf((char*)&VAN_BasicReq.AddedInfo[0], "%05d", 0);
	// 8F an 1  기관코드
	sprintf((char*)&VAN_BasicReq.AddedInfo[5], "%s", g_OrganCode[nType]);
	// 90 FS											
	VAN_BasicReq.AddedInfo[6] = FS;				
	// 90 an 판매된 Column
	//BCD2Char((uchar*)&g_VmAmountColumn.Col, (uchar*)VAN_BasicReq.COLUMN, 2);	// an 현재 컬럼.
	// 1EB 
	VAN_BasicReq.AddedInfo[9] = 0x03;

	// 단말기번호 ~ ETX까지의 길이
	pdata = VAN_BasicReq.TID;
	for(i=0; i<sizeof(NanoVANBasicReq) ; i++) {
		if(pdata[i]==0x03) break;
	}
	sprintf((char*)buf,  "%04d", i+1);
	memcpy((char*)VAN_BasicReq.MsgLen, buf, 4);
	
	// CRC
	crc = (unsigned short)CRC_Update((unsigned char*)&VAN_BasicReq, Char2Int(VAN_BasicReq.MsgLen, 0)+5);
	Int2Byte(crc, &VAN_BasicReq.AddedInfo[10]);
		
	//HEX_Dump( (unsigned char*)&VAN_BasicReq, Char2Int(VAN_BasicReq.MsgLen, 0)+5+2, "운영정보");
		
	g_NanoVANBasicReq =VAN_BasicReq;	

	UBC_BackupVolatile();	
}

void UBC_TestPreambleReq(int len, char * pdata)
{		
	unsigned short crc;
	unsigned char *bdata = (unsigned char*)&VAN_BasicReq;

	VAN_BasicReq.stx = 0x02;

	memcpy((char*)&VAN_BasicReq.MsgLen, pdata, len);

	bdata[len+1] = 0x03;
	
	// CRC
	crc = (unsigned short)CRC_Update((unsigned char*)bdata, len+2);
	Int2Byte(crc, &bdata[len+2]);

		
		
	g_NanoVANBasicReq =VAN_BasicReq;	

	UBC_Backup();	
}

///////////////////////////////////////////////////////////////////////////////////////////
// Gate Bank
void GAT_SaleInfoReq(PVM_DETAIL_INFO info)
{		
	unsigned short crc;
	int i, nAddLen;
	unsigned char tmp[12], *pdata, buf[8];
		
	memset((char*)&VAN_ApproveReq, ' ', sizeof(NanoVANPrepaidApproveReq));	
	memset((char*)&VAN_BasicReq, ' ', sizeof(NanoVANBasicReq));	

	switch(info->basic.Format) {
	case 'P' :	
	case 'Q' :
	case 'R' :
		//HEX_Dump(DEBUG_PORT, (u8 *)info, g_VmRx.BC+2, "CAN&PAT %c", info->retail.Format);		
		nAddLen = 0;		
		// 거래일시
		BCD2Char(adjustment_time, &VAN_BasicReq.AddedInfo[nAddLen], 7);	
		nAddLen+=14;
		// dataformat
		VAN_BasicReq.AddedInfo[nAddLen] = 'Q'; // info->retail.Format;	
		nAddLen+=1;
		// 전송모드
		switch(info->retail.Format) {
		case 'P' :	info->retail.Mode = 'A'; break;
		case 'Q' :	info->retail.Mode = 'C'; break;
		case 'R' :	info->retail.Mode = 'A'; break;
		default  : return;
		}
		VAN_BasicReq.AddedInfo[nAddLen] = info->retail.Mode;	
		nAddLen+=1;
		// 총컬럼수
		VAN_BasicReq.AddedInfo[nAddLen++] = '0';
		BCD2Char((uchar*)&info->retail.TotCol, (uchar*)&VAN_BasicReq.AddedInfo[nAddLen], 1);	
		nAddLen+=2;
		// 전송하는 block 수
		VAN_BasicReq.AddedInfo[nAddLen++] = '0';
		BCD2Char((uchar*)"\x01", (uchar*)&VAN_BasicReq.AddedInfo[nAddLen], 1);	
		nAddLen+=2;
		// 현재전송컬럼
		VAN_BasicReq.AddedInfo[nAddLen++] = '0';
		BCD2Char((uchar*)&info->retail.ColNum, (uchar*)&VAN_BasicReq.AddedInfo[nAddLen], 1);	
		nAddLen+=2;
		// 총/컬럼별 판매금액							
		sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "00");
		nAddLen+=2;
		BCD2Char((uchar*)&info->retail.TotAmount, (uchar*)&VAN_BasicReq.AddedInfo[nAddLen], 4);
		nAddLen+=8;	
		// 총/컬럼별 판매수량						
		sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "00");
		nAddLen+=2;
		BCD2Char((uchar*)&info->retail.TotQuantity, (uchar*)&VAN_BasicReq.AddedInfo[nAddLen], 4);
		nAddLen+=8;	
		// 총/컬럼별 현금 판매금액						
		sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "00");
		nAddLen+=2;
		BCD2Char((uchar*)&info->retail.CashAmount, (uchar*)&VAN_BasicReq.AddedInfo[nAddLen], 4);
		nAddLen+=8;	
		// 총/컬럼별 현금 판매수량						
		sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "00");
		nAddLen+=2;
		BCD2Char((uchar*)&info->retail.CashQuantity, (uchar*)&VAN_BasicReq.AddedInfo[nAddLen], 4);
		nAddLen+=8;		
		// 총/컬럼별 현금외(카드등) 판매금액					
		sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "00");
		nAddLen+=2;
		BCD2Char((uchar*)&info->retail.OtherAmount, (uchar*)&VAN_BasicReq.AddedInfo[nAddLen], 4);
		nAddLen+=8;	
		// 총/컬럼별 현금외(카드등) 판매수량				
		sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "00");
		nAddLen+=2;
		BCD2Char((uchar*)&info->retail.OtherQuantity, (uchar*)&VAN_BasicReq.AddedInfo[nAddLen], 4);
		nAddLen+=8;	
		// 총/컬럼별 TEST 판매금액				
		sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "00");
		nAddLen+=2;
		BCD2Char((uchar*)&info->retail.TestAmount, (uchar*)&VAN_BasicReq.AddedInfo[nAddLen], 4);
		nAddLen+=8;	
		// 총/컬럼별 TEST 판매수량			
		sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "00");
		nAddLen+=2;
		BCD2Char((uchar*)&info->retail.TestQuantity, (uchar*)&VAN_BasicReq.AddedInfo[nAddLen], 4);
		nAddLen+=8;	
		// 총/컬럼별 FREE 판매금액			
		sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "0000000000");
		nAddLen+=10;
		// 총/컬럼별 FREE 판매수량			
		sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "0000000000");
		nAddLen+=10;
		// 요금	
		sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "0000");
		nAddLen+=4;
		BCD2Char((uchar*)info->retail.Fare, (uchar*)&VAN_BasicReq.AddedInfo[nAddLen], 3);
		nAddLen+=6;	

		// 상품코드/개소코드(컬럼 00 일시)	

		if(!info->retail.ColNum) {			
			BCD2Char(info->retail.Code, (uchar*)tmp, 4);
			memcpy(&VAN_BasicReq.AddedInfo[nAddLen], &tmp[2], 6);
			nAddLen+=6;	
			sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "    ");
			nAddLen+=4;
		} else {
			BCD2Char(info->retail.Code, (uchar*)tmp, 4);
			memcpy(&VAN_BasicReq.AddedInfo[nAddLen], &tmp[1], 7);
			nAddLen+=7;	
			sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "   ");
			nAddLen+=3;
		}
		
		// TEST 상품코드		
		BCD2Char((uchar*)"\x00\x00\x00\x00", (uchar*)&VAN_BasicReq.AddedInfo[nAddLen], 4);
		nAddLen+=7;	
		sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "   ");
		nAddLen+=3;

		// 예비 1		
		//sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "0000000000");
		nAddLen+=10;
		// 예비 2
		//sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "0000000000");
		nAddLen+=10;
		// 예비 3
		//sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "0000000000");
		nAddLen+=10;
		// 예비 4	
		if(g_TmsLog.TotCnt==1) 	VAN_BasicReq.AddedInfo[nAddLen]='0', VAN_BasicReq.AddedInfo[nAddLen+1]='1';
		nAddLen+=10;		
		break;
	default :
		//HEX_Dump(DEBUG_PORT, (u8 *)info, g_VmRx.BC+2, "상세매상 %c", info->basic.Format);	
		return;
	}



	VAN_BasicReq.AddedInfo[nAddLen] = ' ';
					
	// 00	
	VAN_BasicReq.stx = 0x02;
	// 01  an 4 단말기번호 - ETX 길이
	// sprintf((char*)VAN_BasicReq.MsgLen,                "%04d", );
	// 05  an 10 단말기 번호 "1234567890"
	sprintf((char*)VAN_BasicReq.TID,                   "%s",   g_VmConfig.TID);						
	// 0F an 2 거래(업무) 구분
	sprintf((char*)VAN_BasicReq.TransactionClass,      "%s",   g_NanoTransaction[9]);			
	// 11 an 2 전문 구분, 운영정보
	sprintf((char*)VAN_BasicReq.PacketClass,           "%s",   g_NanoPacket[5]);				
	// 13 an 1 입력매체 구분코드 , K:KEYIN, S:SWIPE, R:RF, I:IC, X:NON
	VAN_BasicReq.TransactionType = 'X';		
	// 14 an 5 단말기 거래일련 번호, 총 거래 Cnt
	//sprintf((char*)VAN_BasicReq.TransactionCount,      "%05d", g_TmsLog.TotCnt);
	// 19 an 2 전문 버젼
	sprintf((char*)VAN_BasicReq.PacketVersion,         "%s",   "T1");					
	// 1B an 1 전문 암호화 여부 안함																	
	VAN_BasicReq.Encryption = '0';	
	// 1C an 10 예비용														
	//sprintf((char*)VAN_BasicReq.RFU1,                  "%010d", ' ');
	// 26 FS																//
	VAN_BasicReq.FS1 = FS;
	// 27 an 40 인식 카드번호 
	//sprintf((char*)VAN_BasicReq.RFU1,                  "%040d", ' ');
	// 4F FS	
	VAN_BasicReq.FS2 = FS;    	
	// 50 an 2 할부 개월 수
	//sprintf((char*)VAN_BasicReq.Installment,           "%02d", 0);	
	// 52 FS							
	VAN_BasicReq.FS3 = FS;
	// 53 an 10 승인금액   
	//sprintf((char*)VAN_BasicReq.Amount,      			"0000000001");
	// 5D FS
	VAN_BasicReq.FS4 = FS;
	// 5E an 10 봉사료	
	//sprintf((char*)VAN_BasicReq.ServicePrice, 			"%010d", 0);
	// 68 FS						
	VAN_BasicReq.FS5 = FS;
	// 69 an 10 부가가치세	
	//sprintf((char*)VAN_BasicReq.Tax, 					"%010d", 0);
	// 73 FS												
	VAN_BasicReq.FS6 = FS;	
	// 74 an 1 전표사용구분, 0:소비자소득공제, 1:사업자지출증빙 
	//VAN_BasicReq.Bill= '0';		
	// 75 FS																			
	VAN_BasicReq.FS7 = FS;	
	// 76 an 8 원거래 일자
	//memset(VAN_BasicReq.ApproveDate, ' ', 8);		
	// 7E FS								
	VAN_BasicReq.FS8 = FS;
	// 7F an 10 원거래 승인 번호
//	memset(VAN_BasicReq.ApproveNum, ' ', 10);		
	// 89 FS											
	VAN_BasicReq.FS9 = FS;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	





	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 8A FS											
	VAN_BasicReq.AddedInfo[nAddLen+0] = FS;				
	// 8B an 판매된 Column
	// BCD2Char((uchar*)&g_VmAmountColumn.Col, (uchar*)&VAN_BasicReq.AddedInfo[nAddLen+1], 2);	// an 현재 컬럼.
	// 8D 
	VAN_BasicReq.AddedInfo[nAddLen+3] = 0x03;

	// 단말기번호 ~ ETX까지의 길이
	pdata = VAN_BasicReq.TID;
	for(i=0; i<sizeof(NanoVANBasicReq) ; i++) {
		if(pdata[i]==0x03) break;
	}
	sprintf((char*)buf,  "%04d", i+1);
	memcpy((char*)VAN_BasicReq.MsgLen, buf, 4);
	
	// CRC
	crc = (unsigned short)CRC_Update((unsigned char*)&VAN_BasicReq, Char2Int(VAN_BasicReq.MsgLen, 0)+5);
	Int2Byte(crc, &VAN_BasicReq.AddedInfo[nAddLen+4]);
		
	//HEX_Dump((unsigned char*)&VAN_BasicReq, Char2Int(VAN_BasicReq.MsgLen, 0)+5+2, "판매정보");
		
	g_NanoVANBasicReq =VAN_BasicReq;	

	UBC_BackupVolatile();	

}

///////////////////////////////////////////////////////////////////////////////////////////
// 
void UBC_SaleInfoReq(PVM_DETAIL_INFO info)
{		
	unsigned short crc;
	int i, nAddLen;
	static unsigned char korailTime[7];
	unsigned char tmp[12], *pdata, buf[8];

	LCD_Display(LCD_SORT_CENTER, 3, 0, "거래정보전송");
	LCD_Display(LCD_SORT_CENTER, 5, 1, "[%d/%d]", vm_detail_info_pop, vm_detail_info_push);
		
	memset((char*)&VAN_ApproveReq, ' ', sizeof(NanoVANPrepaidApproveReq));	
	memset((char*)&VAN_BasicReq, ' ', sizeof(NanoVANBasicReq));	


	switch(info->basic.Format) {
	case 'C' :
		//HEX_Dump(DEBUG_PORT, (u8 *)info, g_VmRx.BC+2, "BASIC %c", info->basic.Format);
		
		nAddLen = 0;
		// 거래일시
		BCD2Char(adjustment_time, &VAN_BasicReq.AddedInfo[nAddLen], 7);	
		nAddLen+=14;
		// dataformat
		VAN_BasicReq.AddedInfo[nAddLen] = 'C'; // info->basic.Format;	
		nAddLen+=1;
		// 전송모드
		VAN_BasicReq.AddedInfo[nAddLen] = 'A';	
		nAddLen+=1;
		// 총컬럼수
		VAN_BasicReq.AddedInfo[nAddLen++] = '0';
		BCD2Char((uchar*)&info->basic.TotCol, (uchar*)&VAN_BasicReq.AddedInfo[nAddLen], 1);	
		nAddLen+=2;
		// 전송하는 block 수
		VAN_BasicReq.AddedInfo[nAddLen++] = '0';
		BCD2Char((uchar*)"\x01", (uchar*)&VAN_BasicReq.AddedInfo[nAddLen], 1);	
		nAddLen+=2;
		// 현재전송컬럼
		VAN_BasicReq.AddedInfo[nAddLen++] = '0';
		BCD2Char((uchar*)&info->basic.ColNum, (uchar*)&VAN_BasicReq.AddedInfo[nAddLen], 1);	
		nAddLen+=2;
		// 총/컬럼별 판매금액							
		sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "00");
		nAddLen+=2;
		BCD2Char((uchar*)&info->basic.TotAmount, (uchar*)&VAN_BasicReq.AddedInfo[nAddLen], 4);
		nAddLen+=8;	
		// 총/컬럼별 판매수량						
		sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "00");
		nAddLen+=2;
		BCD2Char((uchar*)&info->basic.TotQuantity, (uchar*)&VAN_BasicReq.AddedInfo[nAddLen], 4);
		nAddLen+=8;	
		// 총/컬럼별 현금 판매금액						
		sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "00");
		nAddLen+=2;
		BCD2Char((uchar*)&info->basic.CashAmount, (uchar*)&VAN_BasicReq.AddedInfo[nAddLen], 4);
		nAddLen+=8;	
		// 총/컬럼별 현금 판매수량						
		sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "00");
		nAddLen+=2;
		BCD2Char((uchar*)&info->basic.CashQuantity, (uchar*)&VAN_BasicReq.AddedInfo[nAddLen], 4);
		nAddLen+=8;		
		// 총/컬럼별 현금외(카드등) 판매금액					
		sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "00");
		nAddLen+=2;
		BCD2Char((uchar*)&info->basic.OtherAmount, (uchar*)&VAN_BasicReq.AddedInfo[nAddLen], 4);
		nAddLen+=8;	
		// 총/컬럼별 현금외(카드등) 판매수량				
		sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "00");
		nAddLen+=2;
		BCD2Char((uchar*)&info->basic.OtherQuantity, (uchar*)&VAN_BasicReq.AddedInfo[nAddLen], 4);
		nAddLen+=8;	
		// 총/컬럼별 TEST 판매금액			
		//sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "0000000000");
		nAddLen+=10;
		// 총/컬럼별 TEST 판매수량			
		//sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "0000000000");
		nAddLen+=10;
		// 총/컬럼별 FREE 판매금액			
		//sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "0000000000");
		nAddLen+=10;
		// 총/컬럼별 FREE 판매수량			
		//sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "0000000000");
		nAddLen+=10;
		// 요금		
		//sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "0000000000");
		nAddLen+=10;

		// 상품코드/개소코드(컬럼 00 일시)		
		//sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "0000000000");
		nAddLen+=10;
		
		// TEST 상품코드	
		//sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "0000000000");
		nAddLen+=10;
		// 예비 1		
		//sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "0000000000");
		nAddLen+=10;
		// 예비 2
		//sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "0000000000");
		nAddLen+=10;
		// 예비 3
		//sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "0000000000");
		nAddLen+=10;
		// 예비 4	
		if(g_TmsLog.TotCnt==1) 	VAN_BasicReq.AddedInfo[nAddLen]='0', VAN_BasicReq.AddedInfo[nAddLen+1]='1';
		nAddLen+=10;		

		break;
	case 'P' :
		//HEX_Dump(DEBUG_PORT, (u8 *)info, g_VmRx.BC+2, "CAN&PAT %c", info->canpat.Format);
		
		nAddLen = 0;
		// 거래일시
		BCD2Char(adjustment_time, &VAN_BasicReq.AddedInfo[nAddLen], 7);	
		nAddLen+=14;
		// dataformat
		VAN_BasicReq.AddedInfo[nAddLen] = 'P'; // info->canpat.Format;	
		nAddLen+=1;
		// 전송모드
		VAN_BasicReq.AddedInfo[nAddLen] = 'A';	
		nAddLen+=1;
		// 총컬럼수
		VAN_BasicReq.AddedInfo[nAddLen++] = '0';
		BCD2Char((uchar*)&info->canpat.TotCol, (uchar*)&VAN_BasicReq.AddedInfo[nAddLen], 1);	
		nAddLen+=2;
		// 전송하는 block 수
		VAN_BasicReq.AddedInfo[nAddLen++] = '0';
		BCD2Char((uchar*)"\x01", (uchar*)&VAN_BasicReq.AddedInfo[nAddLen], 1);	
		nAddLen+=2;
		// 현재전송컬럼
		VAN_BasicReq.AddedInfo[nAddLen++] = '0';
		BCD2Char((uchar*)&info->canpat.ColNum, (uchar*)&VAN_BasicReq.AddedInfo[nAddLen], 1);	
		nAddLen+=2;
		// 총/컬럼별 판매금액							
		sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "00");
		nAddLen+=2;
		BCD2Char((uchar*)&info->canpat.TotAmount, (uchar*)&VAN_BasicReq.AddedInfo[nAddLen], 4);
		nAddLen+=8;	
		// 총/컬럼별 판매수량						
		sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "00");
		nAddLen+=2;
		BCD2Char((uchar*)&info->canpat.TotQuantity, (uchar*)&VAN_BasicReq.AddedInfo[nAddLen], 4);
		nAddLen+=8;	
		// 총/컬럼별 현금 판매금액						
		sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "00");
		nAddLen+=2;
		BCD2Char((uchar*)&info->canpat.CashAmount, (uchar*)&VAN_BasicReq.AddedInfo[nAddLen], 4);
		nAddLen+=8;	
		// 총/컬럼별 현금 판매수량						
		sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "00");
		nAddLen+=2;
		BCD2Char((uchar*)&info->canpat.CashQuantity, (uchar*)&VAN_BasicReq.AddedInfo[nAddLen], 4);
		nAddLen+=8;		
		// 총/컬럼별 현금외(카드등) 판매금액					
		sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "00");
		nAddLen+=2;
		BCD2Char((uchar*)&info->canpat.OtherAmount, (uchar*)&VAN_BasicReq.AddedInfo[nAddLen], 4);
		nAddLen+=8;	
		// 총/컬럼별 현금외(카드등) 판매수량				
		sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "00");
		nAddLen+=2;
		BCD2Char((uchar*)&info->canpat.OtherQuantity, (uchar*)&VAN_BasicReq.AddedInfo[nAddLen], 4);
		nAddLen+=8;	
		// 총/컬럼별 TEST 판매금액				
		sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "00");
		nAddLen+=2;
		BCD2Char((uchar*)&info->canpat.TestAmount, (uchar*)&VAN_BasicReq.AddedInfo[nAddLen], 4);
		nAddLen+=8;	
		// 총/컬럼별 TEST 판매수량			
		sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "00");
		nAddLen+=2;
		BCD2Char((uchar*)&info->canpat.TestQuantity, (uchar*)&VAN_BasicReq.AddedInfo[nAddLen], 4);
		nAddLen+=8;	
		// 총/컬럼별 FREE 판매금액			
		sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "0000000000");
		nAddLen+=10;
		// 총/컬럼별 FREE 판매수량			
		sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "0000000000");
		nAddLen+=10;
		// 요금	
		sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "0000");
		nAddLen+=4;
		BCD2Char((uchar*)info->canpat.Fare, (uchar*)&VAN_BasicReq.AddedInfo[nAddLen], 3);
		nAddLen+=6;	

		// 상품코드/개소코드(컬럼 00 일시)	

		if(!info->retail.ColNum) {			
			BCD2Char(info->canpat.Code, (uchar*)tmp, 4);
			memcpy(&VAN_BasicReq.AddedInfo[nAddLen], &tmp[2], 6);
			nAddLen+=6;	
			sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "    ");
			nAddLen+=4;
		} else {
			BCD2Char(info->canpat.Code, (uchar*)tmp, 4);
			memcpy(&VAN_BasicReq.AddedInfo[nAddLen], &tmp[1], 7);
			nAddLen+=7;	
			sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "   ");
			nAddLen+=3;
		}
		
		// TEST 상품코드		
		BCD2Char(info->canpat.Code, (uchar*)tmp, 4);
		memcpy(&VAN_BasicReq.AddedInfo[nAddLen], &tmp[1], 7);
		nAddLen+=7;	
		memcpy((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "   ", 3);
		nAddLen+=3;

		// 예비 1		
		//sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "0000000000");
		nAddLen+=10;
		// 예비 2
		//sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "0000000000");
		nAddLen+=10;
		// 예비 3
		//sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "0000000000");
		nAddLen+=10;
		// 예비 4	
		if(g_TmsLog.TotCnt==1) 	VAN_BasicReq.AddedInfo[nAddLen]='0', VAN_BasicReq.AddedInfo[nAddLen+1]='1';
		nAddLen+=10;		

		break;
	case 'Q' :
		//HEX_Dump(DEBUG_PORT, (u8 *)info, g_VmRx.BC+2, "retail %c", info->retail.Format);
		
		nAddLen = 0;
		// 거래일시
		BCD2Char(adjustment_time, &VAN_BasicReq.AddedInfo[nAddLen], 7);	
		nAddLen+=14;
		// dataformat
		VAN_BasicReq.AddedInfo[nAddLen] = 'Q'; // info->retail.Format;	
		nAddLen+=1;
		// 전송모드
		VAN_BasicReq.AddedInfo[nAddLen] = info->retail.Mode;	
		nAddLen+=1;
		// 총컬럼수
		VAN_BasicReq.AddedInfo[nAddLen++] = '0';
		BCD2Char((uchar*)&info->retail.TotCol, (uchar*)&VAN_BasicReq.AddedInfo[nAddLen], 1);	
		nAddLen+=2;
		// 전송하는 block 수
		VAN_BasicReq.AddedInfo[nAddLen++] = '0';
		BCD2Char((uchar*)"\x01", (uchar*)&VAN_BasicReq.AddedInfo[nAddLen], 1);	
		nAddLen+=2;
		// 현재전송컬럼
		VAN_BasicReq.AddedInfo[nAddLen++] = '0';
		BCD2Char((uchar*)&info->retail.ColNum, (uchar*)&VAN_BasicReq.AddedInfo[nAddLen], 1);	
		nAddLen+=2;
		// 총/컬럼별 판매금액							
		sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "00");
		nAddLen+=2;
		BCD2Char((uchar*)&info->retail.TotAmount, (uchar*)&VAN_BasicReq.AddedInfo[nAddLen], 4);
		nAddLen+=8;	
		// 총/컬럼별 판매수량						
		sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "00");
		nAddLen+=2;
		BCD2Char((uchar*)&info->retail.TotQuantity, (uchar*)&VAN_BasicReq.AddedInfo[nAddLen], 4);
		nAddLen+=8;	
		// 총/컬럼별 현금 판매금액						
		sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "00");
		nAddLen+=2;
		BCD2Char((uchar*)&info->retail.CashAmount, (uchar*)&VAN_BasicReq.AddedInfo[nAddLen], 4);
		nAddLen+=8;	
		// 총/컬럼별 현금 판매수량						
		sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "00");
		nAddLen+=2;
		BCD2Char((uchar*)&info->retail.CashQuantity, (uchar*)&VAN_BasicReq.AddedInfo[nAddLen], 4);
		nAddLen+=8;		
		// 총/컬럼별 현금외(카드등) 판매금액					
		sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "00");
		nAddLen+=2;
		BCD2Char((uchar*)&info->retail.OtherAmount, (uchar*)&VAN_BasicReq.AddedInfo[nAddLen], 4);
		nAddLen+=8;	
		// 총/컬럼별 현금외(카드등) 판매수량				
		sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "00");
		nAddLen+=2;
		BCD2Char((uchar*)&info->retail.OtherQuantity, (uchar*)&VAN_BasicReq.AddedInfo[nAddLen], 4);
		nAddLen+=8;	
		// 총/컬럼별 TEST 판매금액				
		sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "00");
		nAddLen+=2;
		BCD2Char((uchar*)&info->retail.TestAmount, (uchar*)&VAN_BasicReq.AddedInfo[nAddLen], 4);
		nAddLen+=8;	
		// 총/컬럼별 TEST 판매수량			
		sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "00");
		nAddLen+=2;
		BCD2Char((uchar*)&info->retail.TestQuantity, (uchar*)&VAN_BasicReq.AddedInfo[nAddLen], 4);
		nAddLen+=8;	
		// 총/컬럼별 FREE 판매금액			
		sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "0000000000");
		nAddLen+=10;
		// 총/컬럼별 FREE 판매수량			
		sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "0000000000");
		nAddLen+=10;
		// 요금	
		sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "0000");
		nAddLen+=4;
		BCD2Char((uchar*)info->retail.Fare, (uchar*)&VAN_BasicReq.AddedInfo[nAddLen], 3);
		nAddLen+=6;	

		// 상품코드/개소코드(컬럼 00 일시)	

		if(!info->retail.ColNum) {			
			BCD2Char(info->retail.Code, (uchar*)tmp, 4);
			memcpy(&VAN_BasicReq.AddedInfo[nAddLen], &tmp[2], 6);
			nAddLen+=6;	
			sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "    ");
			nAddLen+=4;
		} else {
			BCD2Char(info->retail.Code, (uchar*)tmp, 4);
			memcpy(&VAN_BasicReq.AddedInfo[nAddLen], &tmp[1], 7);
			nAddLen+=7;	
			sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "   ");
			nAddLen+=3;
		}
		
		// TEST 상품코드		
		BCD2Char(info->retail.TestCode, (uchar*)tmp, 4);
		memcpy(&VAN_BasicReq.AddedInfo[nAddLen], &tmp[1], 7);
		nAddLen+=7;	
		memcpy((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "   ", 3);
		nAddLen+=3;

		// 예비 1		
		//sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "0000000000");
		nAddLen+=10;
		// 예비 2
		//sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "0000000000");
		nAddLen+=10;
		// 예비 3
		//sprintf((char*)(uchar*)&VAN_BasicReq.AddedInfo[nAddLen], "0000000000");
		nAddLen+=10;
		// 예비 4	
		if(g_TmsLog.TotCnt==1) 	VAN_BasicReq.AddedInfo[nAddLen]='0', VAN_BasicReq.AddedInfo[nAddLen+1]='1';
		nAddLen+=10;		
		break;
	default :
		//HEX_Dump(DEBUG_PORT, (u8 *)info, g_VmRx.BC+2, "상세매상 %c", info->basic.Format);	
		return;
	}



	VAN_BasicReq.AddedInfo[nAddLen] = ' ';
					
	// 00	
	VAN_BasicReq.stx = 0x02;
	// 01  an 4 단말기번호 - ETX 길이
	//sprintf((char*)VAN_BasicReq.MsgLen,                "%04d", );
	// 05  an 10 단말기 번호 "1234567890"
	sprintf((char*)VAN_BasicReq.TID,                   "%s",   g_VmConfig.TID);						
	// 0F an 2 거래(업무) 구분
	sprintf((char*)VAN_BasicReq.TransactionClass,      "%s",   g_NanoTransaction[9]);			
	// 11 an 2 전문 구분, 운영정보
	sprintf((char*)VAN_BasicReq.PacketClass,           "%s",   g_NanoPacket[5]);				
	// 13 an 1 입력매체 구분코드 , K:KEYIN, S:SWIPE, R:RF, I:IC, X:NON
	VAN_BasicReq.TransactionType = 'X';		
	// 14 an 5 단말기 거래일련 번호, 총 거래 Cnt
	//sprintf((char*)VAN_BasicReq.TransactionCount,      "%05d", g_TmsLog.TotCnt);
	// 19 an 2 전문 버젼
	sprintf((char*)VAN_BasicReq.PacketVersion,         "%s",   "T1");					
	// 1B an 1 전문 암호화 여부 안함																	
	VAN_BasicReq.Encryption = '0';	
	// 1C an 10 예비용														
	//sprintf((char*)VAN_BasicReq.RFU1,                  "%010d", ' ');
	// 26 FS																//
	VAN_BasicReq.FS1 = FS;
	// 27 an 40 인식 카드번호 
	//sprintf((char*)VAN_BasicReq.RFU1,                  "%040d", ' ');
	// 4F FS	
	VAN_BasicReq.FS2 = FS;    	
	// 50 an 2 할부 개월 수
	//sprintf((char*)VAN_BasicReq.Installment,           "%02d", 0);	
	// 52 FS							
	VAN_BasicReq.FS3 = FS;
	// 53 an 10 승인금액   
	//sprintf((char*)VAN_BasicReq.Amount,      			"0000000001");
	// 5D FS
	VAN_BasicReq.FS4 = FS;
	// 5E an 10 봉사료	
	//sprintf((char*)VAN_BasicReq.ServicePrice, 			"%010d", 0);
	// 68 FS						
	VAN_BasicReq.FS5 = FS;
	// 69 an 10 부가가치세	
	//sprintf((char*)VAN_BasicReq.Tax, 					"%010d", 0);
	// 73 FS												
	VAN_BasicReq.FS6 = FS;	
	// 74 an 1 전표사용구분, 0:소비자소득공제, 1:사업자지출증빙 
	//VAN_BasicReq.Bill= '0';		
	// 75 FS																			
	VAN_BasicReq.FS7 = FS;	
	// 76 an 8 원거래 일자
	//memset(VAN_BasicReq.ApproveDate, ' ', 8);		
	// 7E FS								
	VAN_BasicReq.FS8 = FS;
	// 7F an 10 원거래 승인 번호
//	memset(VAN_BasicReq.ApproveNum, ' ', 10);		
	// 89 FS											
	VAN_BasicReq.FS9 = FS;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	





	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 8A FS											
	VAN_BasicReq.AddedInfo[nAddLen+0] = FS;				
	// 8B an 판매된 Column
	// BCD2Char((uchar*)&g_VmAmountColumn.Col, (uchar*)&VAN_BasicReq.AddedInfo[nAddLen+1], 2);	// an 현재 컬럼.
	// 8D 
	VAN_BasicReq.AddedInfo[nAddLen+3] = 0x03;

	// 단말기번호 ~ ETX까지의 길이
	pdata = VAN_BasicReq.TID;
	for(i=0; i<sizeof(NanoVANBasicReq) ; i++) {
		if(pdata[i]==0x03) break;
	}
	sprintf((char*)buf,  "%04d", i+1);
	memcpy((char*)VAN_BasicReq.MsgLen, buf, 4);
	
	// CRC
	crc = (unsigned short)CRC_Update((unsigned char*)&VAN_BasicReq, Char2Int(VAN_BasicReq.MsgLen, 0)+5);
	Int2Byte(crc, &VAN_BasicReq.AddedInfo[nAddLen+4]);
		
	//HEX_Dump((unsigned char*)&VAN_BasicReq, Char2Int(VAN_BasicReq.MsgLen, 0)+5+2, "판매정보");
		
	g_NanoVANBasicReq =VAN_BasicReq;	

	UBC_BackupVolatile();	

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

/*******************************************************************************
* Function Name  : UBC_KbApprovalReq
* Description    : 
* Input          : nType 
*					- 01:승인,    02:취소,          03:인증+승인, 04:적립,
*					  05:적립취소,06:조회,          07:인증,      08:망상취소,
*                     09:정산,    10:단말기다운로드,11:수집,      12:운영정보
* Output         : None
* Return         : None
*******************************************************************************/
void UBC_KbApprovalReq(int nType, unsigned long nAmount)
{				
	int i;
	unsigned char *pdata, buf[8];
	unsigned short crc;

	if((nType==1) && UBC_LogCheck())  return;

	if(nType==1) UBC_ResCodeClear();
	
	if(VM_GetKTC() == KTC_KOCES_INCLUDE) {
		switch(RF_GetCard()) {
		case RF_EMV :
			koces_ic_request(nType, nAmount);
			break;
		case RF_MSR :
		case RF_KB :	
			koces_credit_request(nType, nAmount);
			break;
		}
		return;
	}
	
	memset((char*)&VAN_BasicReq, ' ', sizeof(NanoVANBasicReq));		
	// 00	
	VAN_BasicReq.stx = 0x02;
	// 01  an 4 단말기번호 - ETX 길이
	//sprintf((char*)VAN_BasicReq.MsgLen,                "%04d", );
	// 05  an 10 단말기 번호 "1234567890"	
	sprintf((char*)VAN_BasicReq.TID,                   "%s",   g_VmConfig.TID);
	// 0F an 2 거래(업무) 구분
	sprintf((char*)VAN_BasicReq.TransactionClass,      "%s",   g_NanoTransaction[1]);			
	// 11 an 2 전문 구분, 수집
	sprintf((char*)VAN_BasicReq.PacketClass,           "%s",   g_NanoPacket[nType]);				
	// 13 an 1 입력매체 구분코드 , K:KEYIN, S:SWIPE, R:RF, I:IC, X:NON
	
	switch(nType) {
	case 0x02 :
	case 0x08 : // 망상 취소	
		switch(m_nCraditType) {
		case 1 :	// MSR
			VAN_BasicReq.TransactionType = 'S';	
			break;
		case 2 :	// 후불
			VAN_BasicReq.TransactionType = 'R';	
			break;
		case 3 : // RF_EMV
			VAN_BasicReq.TransactionType = 'R';	
			break;
		}
		// 14 an 5 단말기 거래일련 번호, 총 거래 Cnt
		sprintf((char*)VAN_BasicReq.TransactionCount,      "%05d", m_nNanoTotCnt);
		break;
	default : 	
		switch(g_CraditType) {
		case 1 :	// RF_MSR
			VAN_BasicReq.TransactionType = 'S';	
			break;
		case 2 :	// RF_KB
			VAN_BasicReq.TransactionType = 'R';	
			break;
		case 3 : // RF_EMV
			VAN_BasicReq.TransactionType = 'R';	
			break;
		}
		m_nCraditType = g_CraditType;
		// 14 an 5 단말기 거래일련 번호, 총 거래 Cnt
		sprintf((char*)VAN_BasicReq.TransactionCount,      "%05d", g_TmsLog.TotCnt);
		m_nNanoTotCnt = g_TmsLog.TotCnt;
		break;
	}
	
	// 19 an 2 전문 버젼
	sprintf((char*)VAN_BasicReq.PacketVersion,         "%s",   "T1");					
	// 1B an 1 전문 암호화 여부 안함																	
	VAN_BasicReq.Encryption = '0';	
	// 1C an 10 예비용														
	//sprintf((char*)VAN_BasicReq.RFU1,                  "%010d", ' ');
	// 26 FS																//
	VAN_BasicReq.FS1 = FS;
	// 27 an 40 인식 카드번호 
	
	
	switch(nType) {
	case 0x02 :
	case 0x08 : // 망상 취소	
		switch(m_nCraditType) {
		case 1 :		
			memcpy(VAN_BasicReq.CSN, m_nMsrInfo.nTrack2, m_nMsrInfo.nCnt2);
			break;
		case 2 :
			Hex2Ascii(m_nKbData.CID, &VAN_BasicReq.CSN[0], 8);
			VAN_BasicReq.CSN[16]='=';
			Hex2Ascii(m_nKbData.ExpireDate, &VAN_BasicReq.CSN[17], 2);
			break;
		case 3 :		
			memcpy(VAN_BasicReq.CSN, m_nMsrInfo.nTrack2, m_nMsrInfo.nCnt2);
			break;
		}
		break;
	default :
		switch(g_CraditType) {
		case 1 :		
			memcpy(VAN_BasicReq.CSN, g_MsrInfo.nTrack2, g_MsrInfo.nCnt2);
			break;
		case 2 :
			UBC_GetKbTrack2(VAN_BasicReq.CSN);
			break;
		case 3 :
			EMV_GetTrack2(g_MsrInfo.nTrack2, &g_MsrInfo.nCnt2);
			memcpy(VAN_BasicReq.CSN, g_MsrInfo.nTrack2, g_MsrInfo.nCnt2);
			break;
		}
		m_nKbData  = g_KbData;
		m_nMsrInfo = g_MsrInfo;
		break;
	}
	
	
	// 4F FS	
	VAN_BasicReq.FS2 = FS;    	
	// 50 an 2 할부 개월 수
	sprintf((char*)VAN_BasicReq.Installment,           "%02d", 0);	
	// 52 FS							
	VAN_BasicReq.FS3 = FS;
	// 53 an 10 승인금액   	
	switch(nType) {
	case 0x02 :
	case 0x08 : // 망상 취소			
		if(!m_nAmount) return;
		sprintf((char*)VAN_BasicReq.Amount,      			"%010d", m_nAmount);
		m_nAmount = 0;
		break;
	default :	
		sprintf((char*)VAN_BasicReq.Amount,      			"%010d", nAmount);
		m_nAmount = nAmount;
		break;
	}
	// 5D FS
	VAN_BasicReq.FS4 = FS;
	// 5E an 10 봉사료	
	sprintf((char*)VAN_BasicReq.ServicePrice, 			"%010d", 0);
	// 68 FS						
	VAN_BasicReq.FS5 = FS;
	// 69 an 10 부가가치세	
	
	sprintf((char*)VAN_BasicReq.Tax, 					"%010d", 0);
	
	// 73 FS												
	VAN_BasicReq.FS6 = FS;	
	// 74 an 1 전표사용구분, 0:소비자소득공제, 1:사업자지출증빙 
	VAN_BasicReq.Bill= '0';		
	// 75 FS																			
	VAN_BasicReq.FS7 = FS;	
	switch(nType) {
	case 0x02 : // 취소	
		// 76 an 8 원거래 일자
		for(i=0; i<8; i++) VAN_BasicRes.Date[i] = VAN_BasicReq.ApproveDate[i];		
		// 7E FS								
		VAN_BasicReq.FS8 = FS;
		// 7F an 10 원거래 승인 번호
		for(i=0; i<8; i++) VAN_BasicRes.ApproveNum[i] = VAN_BasicReq.ApproveNum[i];
		break;
	case 0x08 : // 망상취소
	default :
		// 76 an 8 원거래 일자
		memset(VAN_BasicReq.ApproveDate, ' ', 8);
		// 7E FS								
		VAN_BasicReq.FS8 = FS;
		// 7F an 10 원거래 승인 번호
		memset(VAN_BasicReq.ApproveNum, ' ', 10);
		break;
	}
	// 89 FS											
	VAN_BasicReq.FS9 = FS;					
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 8A FS											
	VAN_BasicReq.AddedInfo[0] = FS;				
	// 8B an 판매된 Column
	BCD2Char((uchar*)&g_VmAmountColumn.Col, (uchar*)&VAN_BasicReq.AddedInfo[1], 2);	// an 현재 컬럼.
	// 8D 
	VAN_BasicReq.AddedInfo[3] = 0x03;

	// 단말기번호 ~ ETX까지의 길이
	pdata = VAN_BasicReq.TID;
	for(i=0; i<sizeof(NanoVANBasicReq) ; i++) {
		if(pdata[i]==0x03) break;
	}
	sprintf((char*)buf,  "%04d", i+1);
	memcpy((char*)VAN_BasicReq.MsgLen, buf, 4);
	
	// CRC
	crc = (unsigned short)CRC_Update((unsigned char*)&VAN_BasicReq, Char2Int(VAN_BasicReq.MsgLen, 0)+5);
	Int2Byte(crc, &VAN_BasicReq.AddedInfo[4]);
	
		
	g_NanoVANBasicReq = VAN_BasicReq;	
	
	switch(nType) {
	case 0x02 : // 취소	
	case 0x08 : // 망상취소
		HEXDUMP( (unsigned char*)&g_NanoVANBasicReq, Char2Int(VAN_BasicReq.MsgLen, 0)+5, "KB CAN >> %d", Char2Int(VAN_BasicReq.MsgLen, 0)+5);
		
		UBC_TmsBackUpProcess(-1);				
		UBC_Backup();
		break;
	case 0x06 : // 조회
        break;
    case 0x01 :					
		HEXDUMP( (unsigned char*)&VAN_BasicReq, Char2Int(VAN_BasicReq.MsgLen, 0)+5, "KB REQ %d", Char2Int(VAN_BasicReq.MsgLen, 0)+5);
		
		UBC_TmsBackUpProcess(nAmount);				
		
		UBC_BackupVolatile();	// UBC_Backup();	
				
		VM_ReadVanFlash();		
		break;			
	}
	

}

/*******************************************************************************
* Function Name  : UBC_CashApproval
* Description    : 
* Input          :
* Output         : None
* Return         : None
*******************************************************************************/
void UBC_CashApproval(int nType, unsigned long nAmount , char *pNumber)
{	
	int i;
	unsigned short crc;
	unsigned char *pData, *bdata, buf[8];

	
	if(VM_GetKTC() == KTC_KOCES_INCLUDE) {		
		koces_cash_request(nType, nAmount, pNumber);
		return;
	}
	
	memset((char*)&VAN_BasicReq, ' ', sizeof(NanoVANBasicReq));		
	// 00	
	VAN_BasicReq.stx = 0x02;
	// 01  an 4 단말기번호 - ETX 길이
	sprintf((char*)VAN_BasicReq.MsgLen,                "%04d", sizeof(NanoVANBasicReq)-1-4-2-sizeof(NanoVANPrepaidApproveReq)-4+8);
	// 05  an 10 단말기 번호 "1234567890"	
	sprintf((char*)VAN_BasicReq.TID,                   "%s",   g_VmConfig.TID);	
	// 0F an 2 거래(업무) 구분
	sprintf((char*)VAN_BasicReq.TransactionClass,      "%s",   g_NanoTransaction[10]);			
	// 11 an 2 전문 구분, 수집
	sprintf((char*)VAN_BasicReq.PacketClass,           "%s",   g_NanoPacket[1]);				
	// 13 an 1 입력매체 구분코드 , K:KEYIN, S:SWIPE, R:RF, I:IC, X:NON	
	VAN_BasicReq.TransactionType = 'X';		
	// 14 an 5 단말기 거래일련 번호, 총 거래 Cnt
	sprintf((char*)VAN_BasicReq.TransactionCount,      "%05d", g_TmsLog.TotCnt);
	// 19 an 2 전문 버젼
	sprintf((char*)VAN_BasicReq.PacketVersion,         "%s",   "T1");					
	// 1B an 1 전문 암호화 여부 안함																	
	VAN_BasicReq.Encryption = '0';	
	// 1C an 10 예비용														
	//sprintf((char*)VAN_BasicReq.RFU1,                  "%010d", ' ');
	// 26 FS																//
	VAN_BasicReq.FS1 = FS;
	// 27 an 40 인식 카드번호 
	//memcpy(VAN_BasicReq.CSN, g_MsrInfo.nTrack2, 37);
	// 4F FS	
	VAN_BasicReq.FS2 = FS;    	
	// 50 an 2 할부 개월 수
	//sprintf((char*)VAN_BasicReq.Installment,           "%02d", 0);	
	// 52 FS							
	VAN_BasicReq.FS3 = FS;
	// 53 an 10 승인금액   
	sprintf((char*)VAN_BasicReq.Amount,      			"%010d", nAmount);
	// 5D FS
	VAN_BasicReq.FS4 = FS;
	// 5E an 10 봉사료	
	//sprintf((char*)VAN_BasicReq.ServicePrice, 			"%010d", 0);
	// 68 FS						
	VAN_BasicReq.FS5 = FS;
	// 69 an 10 부가가치세	
	//sprintf((char*)VAN_BasicReq.Tax, 					"%010d", 0);
	// 73 FS												
	VAN_BasicReq.FS6 = FS;	
	// 74 an 1 전표사용구분, 0:소비자소득공제, 1:사업자지출증빙 
	VAN_BasicReq.Bill= '0';		
	// 75 FS																			
	VAN_BasicReq.FS7 = FS;
	// 76 an 8 원거래 일자
	//memset(VAN_BasicReq.ApproveDate, ' ', 8);
	BCD2Char(g_Today, VAN_BasicReq.ApproveDate, 4);
	// 7E FS								
	VAN_BasicReq.FS8 = FS;
	// 7F an 10 원거래 승인 번호
	//memset(VAN_BasicReq.ApproveNum, ' ', 10);
	BCD2Char(&g_Today[4], VAN_BasicReq.ApproveNum, 3);
	pData = &VAN_BasicReq.ApproveNum[6];
	// 89 FS											
	pData[0] = FS;					
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 8A FS	
	//sprintf((char*)&pData[1], "%08u",   B2I(g_VmAmountColumn.Count[3],g_VmAmountColumn.Count[2],g_VmAmountColumn.Count[1],g_VmAmountColumn.Count[0]));
	Hex2Ascii(g_VmAmountColumn.Count, pData+1, 4);
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 94 FS		
	pData[9] = FS;				
	// 8B an 판매된 Column
	BCD2Char((uchar*)&g_VmAmountColumn.Col, (uchar*)&pData[10], 2);	// an 현재 컬럼.
	// 8D 
	pData[12] = 0x03;
	// CRC

	// 단말기번호 ~ ETX까지의 길이
	bdata = VAN_BasicReq.TID;
	for(i=0; i<sizeof(NanoVANBasicReq) ; i++) {
		if(bdata[i]==0x03) break;
	}
	sprintf((char*)buf,  "%04d", i+1);
	memcpy((char*)VAN_BasicReq.MsgLen, buf, 4);
	
	crc = (unsigned short)CRC_Update((unsigned char*)&VAN_BasicReq, Char2Int(VAN_BasicReq.MsgLen, 0)+5);
	Int2Byte(crc, &pData[13]);
	
	HEXDUMP( (unsigned char*)&VAN_BasicReq, Char2Int(VAN_BasicReq.MsgLen, 0)+5, "\r\n현금 %d", Char2Int(VAN_BasicReq.MsgLen, 0)+5);
		
	g_NanoVANBasicReq = VAN_BasicReq;	

	//HEX_Dump( (unsigned char*)pReq, sizeof(NanoVANBasicReq), "NANO REQ %d", g_TmsLog.TotCnt);				
		
	UBC_TmsBackUpProcess(nAmount);				
	UBC_Backup();

	RF_SetResCode(TM_FIND_CARD);
}

/*******************************************************************************
* Function Name  : UBC_Adjustment
* Description    : 
* Input          :
* Output         : None
* Return         : None
*******************************************************************************/
void UBC_Adjustment(void)
{	
	int i;
	unsigned char *pdata, buf[8];
	unsigned short crc;
	
	adjustment_check=1;
	
	memset((char*)&VAN_BasicReq, ' ', sizeof(NanoVANBasicReq));		
	// 00	
	VAN_BasicReq.stx = 0x02;
	// 01  an 4 단말기번호 - ETX 길이
	//sprintf((char*)VAN_BasicReq.MsgLen,                "%04d", );
	// 05  an 10 단말기 번호 "1234567890"	
	sprintf((char*)VAN_BasicReq.TID,                   "%s",   g_VmConfig.TID);	
	// 0F an 2 거래(업무) 구분
	sprintf((char*)VAN_BasicReq.TransactionClass,      "%s",   g_NanoTransaction[9]);			
	// 11 an 2 전문 구분, 수집
	sprintf((char*)VAN_BasicReq.PacketClass,           "%s",   g_NanoPacket[9]);				
	// 13 an 1 입력매체 구분코드 , K:KEYIN, S:SWIPE, R:RF, I:IC, X:NON	
	VAN_BasicReq.TransactionType = 'X';		
	// 14 an 5 단말기 거래일련 번호, 총 거래 Cnt
	//sprintf((char*)VAN_BasicReq.TransactionCount,      "%05d", g_TmsLog.TotCnt);
	// 19 an 2 전문 버젼
	sprintf((char*)VAN_BasicReq.PacketVersion,         "%s",   "T1");					
	// 1B an 1 전문 암호화 여부 안함																	
	VAN_BasicReq.Encryption = '0';	
	// 1C an 10 예비용														
	//sprintf((char*)VAN_BasicReq.RFU1,                  "%010d", ' ');
	// 26 FS																//
	VAN_BasicReq.FS1 = FS;
	// 27 an 40 인식 카드번호 
	//memcpy(VAN_BasicReq.CSN, g_MsrInfo.nTrack2, 37);
	// 4F FS	
	VAN_BasicReq.FS2 = FS;    	
	// 50 an 2 할부 개월 수
	//sprintf((char*)VAN_BasicReq.Installment,           "%02d", 0);	
	// 52 FS							
	VAN_BasicReq.FS3 = FS;
	// 53 an 10 승인금액   
	//sprintf((char*)VAN_BasicReq.Amount,      			"%010d", 0);
	// 5D FS
	VAN_BasicReq.FS4 = FS;
	// 5E an 10 봉사료	
	//sprintf((char*)VAN_BasicReq.ServicePrice, 			"%010d", 0);
	// 68 FS						
	VAN_BasicReq.FS5 = FS;
	// 69 an 10 부가가치세	
	//sprintf((char*)VAN_BasicReq.Tax, 					"%010d", 0);
	// 73 FS												
	VAN_BasicReq.FS6 = FS;	
	// 74 an 1 전표사용구분, 0:소비자소득공제, 1:사업자지출증빙 
	VAN_BasicReq.Bill= '0';		
	// 75 FS																			
	VAN_BasicReq.FS7 = FS;
	// 76 an 8 원거래 일자
	//memset(VAN_BasicReq.ApproveDate, ' ', 8);
	// 7E FS								
	VAN_BasicReq.FS8 = FS;
	// 7F an 10 원거래 승인 번호
	//memset(VAN_BasicReq.ApproveNum, ' ', 10);
	// 89 FS											
	VAN_BasicReq.FS9 = FS;					
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 8A FS											
	VAN_BasicReq.AddedInfo[0] = FS;				
	// 8B an 판매된 Column
	BCD2Char((uchar*)&g_VmAmountColumn.Col, (uchar*)&VAN_BasicReq.AddedInfo[1], 2);	// an 현재 컬럼.
	// 8D 
	VAN_BasicReq.AddedInfo[3] = 0x03;
	// CRC

	// 단말기번호 ~ ETX까지의 길이
	pdata = VAN_BasicReq.TID;
	for(i=0; i<sizeof(NanoVANBasicReq) ; i++) {
		if(pdata[i]==0x03) break;
	}
	sprintf((char*)buf,  "%04d", i+1);
	memcpy((char*)VAN_BasicReq.MsgLen, buf, 4);
	
	crc = (unsigned short)CRC_Update((unsigned char*)&VAN_BasicReq, Char2Int(VAN_BasicReq.MsgLen, 0)+5);
	Int2Byte(crc, &VAN_BasicReq.AddedInfo[4]);
	
	//HEXDUMP( (unsigned char*)&VAN_BasicReq, sizeof(VAN_BasicReq)-sizeof(NanoVANPrepaidApproveReq), "KB REQ %d", Char2Int(VAN_BasicReq.MsgLen, 0)+5);		
	g_NanoVANBasicReq = VAN_BasicReq;	
	//HEX_Dump( (unsigned char*)pReq, sizeof(NanoVANBasicReq), "NANO REQ %d", g_TmsLog.TotCnt);				
	RF_SetResCode(TM_FIND_CARD);	
	
	UBC_BackupVolatile();

	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//  												    //
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void UBC_TmsBackUpProcess(int nAmount) 
{	
	FILE *fp = NULL;
	unsigned char buf[64];

	sprintf(buf, "/usr/rfpinpad/log/trans_backup.dat", g_NanoBackup.push);	

	
	fp = fopen(buf, "r");

	if(fp==NULL) {
		g_TmsLog.TotCnt=1;
		memset((char*)&g_TmsPurchaseInfoReq, 0,  sizeof(TMS_PURCHASE_INFO_REQ));
	}
	else {
		fread(&g_TmsPurchaseInfoReq, sizeof(TMS_PURCHASE_INFO_REQ), 1, fp);	
		fclose(fp);

	}
	
	BCD2Char(g_Today, g_TmsPurchaseInfoReq.Time, 7);
	sprintf((char*)&g_TmsPurchaseInfoReq.TID, "%s", g_VmConfig.TID);						// an 단말기 번호 "1234567890"
	memcpy((char*)g_TmsPurchaseInfoReq.ClearTime , (char*)g_VmConfig.ClrTime, 14);
			
	PRINTF( "Total Amount %d\r\n", nAmount);

	if(nAmount>=0) 
	{	
		g_TmsLog.TotAmount += nAmount;
						
		g_TmsPurchaseInfoReq.Type = g_VmAmountColumn.Type;
		BCD2Char((uchar*)&g_VmAmountColumn.Col, (uchar*)&g_TmsPurchaseInfoReq.Col, 2);	// an 현재 컬럼.
		sprintf((char*)&g_TmsPurchaseInfoReq.CurAmount, "%d", nAmount);					// an 현재 금액.
		sprintf((char*)&g_TmsPurchaseInfoReq.TotCnt, "%d", g_TmsLog.TotCnt);					// an 전체 횟수	
		sprintf((char*)&g_TmsPurchaseInfoReq.TotAmount, "%d", g_TmsLog.TotAmount);			// an 전체 금액	
		
		g_TmsLog.TotCnt++;

		if(g_TmsLog.TotCnt>UBCN_VAN_COUNT_MAX) g_TmsLog.TotCnt=1;

		fp = fopen(buf, "w");
		fwrite(&g_TmsPurchaseInfoReq, sizeof(TMS_PURCHASE_INFO_REQ), 1, fp);	
		fclose(fp);
		
		PRINTF( "%d.Total Amount %d\r\n", g_TmsLog.TotCnt, g_TmsLog.TotAmount);	
	}
		
}

void UBC_Backup(void) 
{
#if (LOTTE_NAVER_TEST_MODE==0)

	int push, cnt;
	unsigned int nRetry;
	unsigned char buf[64], rm_buf[64];
	FILE *fp = NULL;
	UBC_BAKUP  NanoBackup;
	NanoVANBasicReq VANBasicReq;

b00 :

	nRetry=0;
	cnt = g_NanoBackup.cnt;
	push = g_NanoBackup.push;
		
	if (g_NanoBackup.cnt<UBC_LOG_MAX) {	

b01 :				
		sprintf(buf, "/usr/rfpinpad/log/transaction_%d.dat", g_NanoBackup.push);
		
		fp = fopen(buf, "w");
		if(fp != NULL) {
			fwrite(&g_NanoVANBasicReq, sizeof(NanoVANBasicReq), 1, fp);	
			fclose(fp);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		// verify write
		////////////////////////////////////////////////////////////////////////////////////////////////////
		fp = fopen(buf, "r");
		if(fp != NULL) {
			fread(&VANBasicReq, sizeof(NanoVANBasicReq), 1, fp);	
			fclose(fp);
		}

		if(memcmp((char *)&g_NanoVANBasicReq, (char *)&VANBasicReq, sizeof(NanoVANBasicReq))) {
			printf( "\r\n\t%1.WRITE Retry <%d>\r\n", nRetry);
			sprintf(rm_buf, "rm /usr/rfpinpad/log/transaction_%d.dat\0", g_NanoBackup.push);		
			system(rm_buf);
			if(++nRetry<3) goto b01;
		}
		
		
		g_NanoBackup.push = (1+g_NanoBackup.push)%UBC_LOG_MAX;
		g_NanoBackup.cnt  += 1;

		nRetry=0;
b02 :

		sprintf(buf, "/usr/rfpinpad/log/trans_counter.dat");	
		
		fp = fopen(buf, "w");
		if(fp != NULL) {
			fwrite(&g_NanoBackup, sizeof(UBC_BAKUP), 1, fp);	
			fclose(fp);
		}

		fp = fopen(buf, "r");
		if(fp != NULL) {
			fread(&NanoBackup, sizeof(UBC_BAKUP), 1, fp);	
			fclose(fp);
		}

		if(memcmp((char *)&g_NanoBackup, (char *)&NanoBackup, sizeof(UBC_BAKUP))) {
			printf( "\r\n\n\t%2.WRITE Retry <%d>\r\n\n", nRetry);
			if(++nRetry<3) goto b02;
		}
					
		if(g_CreditAuth) {
			g_CreditAuth = 4;	
		}	

		#ifdef DBG_KTC
			printf( "\r\n\t%d.WR <%d/%d>\r\n", g_NanoBackup.cnt, g_NanoBackup.push, g_NanoBackup.pop);
		#endif
		
		if((cnt == g_NanoBackup.cnt) || (push == g_NanoBackup.push)) goto b00;
	}
#endif
}


void UBC_BackupVolatile(void) 
{
	int push, cnt;
	unsigned int nRetry;
	unsigned char buf[64], rm_buf[64];
	FILE *fp = NULL;
	UBC_BAKUP  NanoBackup;
	NanoVANBasicReq VANBasicReq;

b00 :

	nRetry=0;
	cnt = g_NanoBackup.cnt;
	push = g_NanoBackup.push;
		
	if (g_NanoBackup.cnt<UBC_LOG_MAX) {	

b01 :			
		sprintf(buf, "/tmp/transaction_%d.dat", g_NanoBackup.push);
		
		fp = fopen(buf, "w");
		if(fp != NULL) {
			fwrite(&g_NanoVANBasicReq, sizeof(NanoVANBasicReq), 1, fp);	
			fclose(fp);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		// verify write
		////////////////////////////////////////////////////////////////////////////////////////////////////
		fp = fopen(buf, "r");
		if(fp != NULL) {
			fread(&VANBasicReq, sizeof(NanoVANBasicReq), 1, fp);	
			fclose(fp);
		}

		if(memcmp((char *)&g_NanoVANBasicReq, (char *)&VANBasicReq, sizeof(NanoVANBasicReq))) {
			printf( "\r\n\t%11.WRITE Retry <%d>\r\n", nRetry);	
			sprintf(rm_buf, "rm /tmp/transaction_%d.dat\0", g_NanoBackup.push);		
			system(rm_buf);
			if(++nRetry<3) goto b01;
		}
		
		
		g_NanoBackup.push = (1+g_NanoBackup.push)%UBC_LOG_MAX;
		g_NanoBackup.cnt  += 1;

		nRetry=0;
b02 :

		sprintf(buf, "/usr/rfpinpad/log/trans_counter.dat");	
		
		fp = fopen(buf, "w");
		if(fp != NULL) {
			fwrite(&g_NanoBackup, sizeof(UBC_BAKUP), 1, fp);	
			fclose(fp);
		}

		fp = fopen(buf, "r");
		if(fp != NULL) {
			fread(&NanoBackup, sizeof(UBC_BAKUP), 1, fp);	
			fclose(fp);
		}

		if(memcmp((char *)&g_NanoBackup, (char *)&NanoBackup, sizeof(UBC_BAKUP))) {
			printf( "\r\n\n\t%22.WRITE Retry <%d>\r\n\n", nRetry);
			if(++nRetry<3) goto b02;
		}
					
		if(g_CreditAuth) {
			g_CreditAuth = 4;	
		}	
				
		#ifdef DBG_KTC
			printf( "\r\n\t%d,WR <%d/%d>\r\n", g_NanoBackup.cnt, g_NanoBackup.push, g_NanoBackup.pop);
		#endif
		
		if((cnt == g_NanoBackup.cnt) || (push == g_NanoBackup.push)) goto b00;
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	1. 발행사 ID 다운로드 [0XF1]
//
int UBC_TmSetIssuerID(unsigned char *pData)
{
	unsigned char recNo, temp[ISSUER_ID_MAX_SIZE];
	int i;
	FILE *fp = NULL;
		
	// 운영정보관리번호	
	memcpy(g_Tmoney.ManagementNum, pData, 5);
	g_Tmoney.ManagementNum[5]=0;
	pData += 5;
	
	// 발행사 정보 메모리에 저장
	memset(temp, 0xFF, ISSUER_ID_MAX_SIZE);
	memcpy(temp, pData, 3);	
	
	recNo = 6*( (temp[0]&0x0f)*100 + (temp[1]&0x0f)*10 + (temp[2]&0x0f) );
		
	// 시간 저장
	Char2BCD(&pData[3], &temp[3], 14);
		
	for(i=0; i<recNo; i++) {	
		temp[10 + i] = pData[17 + i];
	}
	
	// CRC Check	
	if(memcmp((unsigned char *)g_Tmoney.I_inform_RFU, temp, ISSUER_ID_MAX_SIZE)) 
	{    	
		memcpy(g_Tmoney.I_inform_RFU ,temp, ISSUER_ID_MAX_SIZE);	
		
		fp = fopen("/usr/rfpinpad/data/Tmoney.dat", "w");
		fwrite(&g_Tmoney, sizeof(MAPB_INFO), 1, fp);	
		fclose(fp);
		
    		PRINTHEX( temp, 10+recNo, "\r\nSetIssuerID");
	}	
	TM_LoadTerminalProperty();	
	return RC_SUCCESS;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// EB 잔액 조회 						                                                                //
//////////////////////////////////////////////////////////////////////////////////////////////////////////
int UBC_EbBalance(unsigned long *nBalance) 
{
	int res = RC_FAILURE;	

	*nBalance = 0;
	memset(EB_RFInfo.ReadBalance_BAL, 0, 4);
	
	// Read EF_PurseInfo
	if (EB_RF_ReadBalance() == RC_SUCCESS) {
		EB_RF_ReadBinary(0x0B);
		if(EB_SAM_GetDataIDsam()==RC_SUCCESS) 
		{
			*nBalance = B2I(EB_RFInfo.ReadBalance_BAL[3],EB_RFInfo.ReadBalance_BAL[2],EB_RFInfo.ReadBalance_BAL[1],EB_RFInfo.ReadBalance_BAL[0]);

			// 충전 금액 상한...
			if(*nBalance > TM_MAX_BALANCE || (*nBalance==0) ||
				(EB_RF_ReadBinary(0x0e)!=RC_SUCCESS) ||
				(EB_GetCenter(EB_TypeAB, EB_RFInfo.SelectFile_IDep)<0)) {	
				if(!play_wav_check())  	
					play_wav("adpcm_현금판매.bin");		
			}
			else {
				res = RC_OK;				
			}
		}
	}
	    	
	return res;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////
// EB 판매 취소 Data(DC=00, Bit 4)			                                                                //
//////////////////////////////////////////////////////////////////////////////////////////////////////////
int UBC_EbCancel(void)
{
	int res = RC_FAILURE;
	unsigned char Mpda[4];
	unsigned char pData[256];
	unsigned char nPos, nLen;
	int nCenter;

	memcpy(Mpda, EB_SAMInfo.CreditPSAM_Mpda, 4);

	memset(&EB_SAMInfo.ReadInfo_ALGsam, 0x00, sizeof(EB_SAMInfo)-12);
	memset(&EB_RFInfo.SelectFile_CARDtype, 0x00, sizeof(EB_RFInfo));
	
	if(EB_SAM_GetDataIDsam()!=RC_SUCCESS)
		return RC_SAM_ERROR;	
	
	if(EB_SAM_ReadBalance(0)!=RC_SUCCESS)
		return RC_SAM_ERROR;	
	
	res = EB_GetBalance();
	if(res!=RC_DATA)
		return res;

	if(EB_RF_ReadBinary(0x0B)!=RC_SUCCESS)
		memset(&EB_RFInfo.ReadBinary_MCARDtype, 0x00, 11);

	EB_RF_ReadRecord(1,0x04, 0x1A);
	nCenter = EB_GetCenter(EB_TypeAB, EB_RFInfo.SelectFile_IDep);	

	if(nCenter==-1)
		return RC_INVALID_CARD;
	res = EB_CheckValidateFunction(CMD_EB_PURCHASE, nCenter, NULL);
	if(res!=RC_DATA)
		return res;
	
	if(EB_RF_Initialize(_EB_TypeAB_RF_Initialize_CancelPurchase_, Mpda, 4)!=RC_SUCCESS)
		return RC_INVALID_CARD;
		
	if(EB_SAM_InitializePSAM(_EB_SAM_InitializePSAM_CancelPurchase_,  Mpda)!=RC_SUCCESS)
		return RC_SAM_ERROR;
	
	nLen =nPos = 0;
	nLen = 8;				memcpy(&pData[nPos], EB_SAMInfo.InitializePSAM_IDsam, nLen);		nPos +=nLen;
	nLen = 4;				memcpy(&pData[nPos], EB_SAMInfo.InitializePSAM_NTpsam, nLen);		nPos +=nLen;
	nLen = 2;				memcpy(&pData[nPos], EB_SAMInfo.InitializePSAM_SCsam, nLen);		nPos +=nLen;
	nLen = 4;				memcpy(&pData[nPos], EB_SAMInfo.InitializePSAM_SIGN2, nLen);		nPos +=nLen;
	if(EB_RF_Purchase(_EB_TypeAB_RF_Purchase_CancelPurchase_)!=RC_SUCCESS){
		return RC_READ_BALANCE;
	}
	
	nLen =nPos = 0;
	nLen = 4;				memcpy(&pData[nPos], &EB_RFInfo.Purchase_SIGN3, nLen);		nPos +=nLen;
	if(EB_SAM_CancellationPSAM()!=RC_SUCCESS){
		return RC_FAILURE;
	}

	if(EB_SAM_ReadBalance(2)!=RC_SUCCESS){
		return RC_FAILURE;
	}	
	
	UBC_SetPrepaidPacket(0);			
	g_TransactionOk=0;
	
	PRINTF( "EB_TRANSACTION_CANCEL  ... %d\r\n", g_SamInfo.nRcvLen);
	return RC_DATA;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// EB 판매금액 및 컬럼 DATA (DC = 01)                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////////////////////
int UBC_EbPurchase(LONG nTemp)
{	
	int i, res=RC_FAILURE;
	unsigned long nBalance;
	char nData[8];
	
	unsigned char Mpda[4];
	unsigned char pData[256];
	unsigned char nPos, nLen;
	unsigned long	lvalue1=0, lvalue2=0, lvalue3=0; 
	int nCenter=0;
	
	g_TransactionOk	= 0;

	// 금액이 0 이면
	//if(!nTemp.value) return RC_FAILURE;
		
	res = RF_Polling( 0, RF_EB);
	if(	res != RF_EB ) return RC_FAILURE;

	RF_SetCard(RF_EB, nBalance);
	
	if((res = EB_GetBalance())!=RC_DATA)
		return res;

	if((res = EB_RF_ReadBinary(0x0e))!=RC_SUCCESS)
		return res;

	Swap_4(nTemp.data, Mpda);

	Swap_4(EB_RFInfo.ReadBalance_BAL, (unsigned char*)&lvalue1);
	Swap_4(Mpda, (unsigned char*)&lvalue2);

	if(EB_SAM_ReadBalance(0)!=RC_SUCCESS)
		return RC_SAM_ERROR;
		
	PRINTHEX( (unsigned char *)&lvalue1, 4,"BALANCE %ld\n", lvalue1);
	PRINTF("Mpda %ld\n", lvalue2);

	if(lvalue1<lvalue2)
		return RC_BAL_ERROR;

	PRINTHEX( EB_RFInfo.SelectFile_IDep, 8,"SelectFile_IDep");
	nCenter = EB_GetCenter(EB_TypeAB, EB_RFInfo.SelectFile_IDep);
	
	if(nCenter<0)
		return RC_INVALID_CARD;
	
	res = EB_CheckValidateFunction(CMD_EB_PURCHASE, nCenter, NULL);
	if(res!=RC_DATA)
		return res;

	if(EB_RF_Initialize(_EB_TypeAB_RF_Initialize_Purchase_, Mpda, 4)!=RC_SUCCESS)
		return RC_INVALID_CARD;
	
	if(EB_SAM_InitializePSAM(_EB_SAM_InitializePSAM_Purchase_,  Mpda)!=RC_SUCCESS) {		
		return RC_SAM_ERROR;
	}
	
	nLen =nPos = 0;
	nLen = 8;				memcpy(&pData[nPos], EB_SAMInfo.InitializePSAM_IDsam, nLen);		nPos +=nLen;
	nLen = 4;				memcpy(&pData[nPos], EB_SAMInfo.InitializePSAM_NTpsam, nLen);		nPos +=nLen;
	nLen = 2;				memcpy(&pData[nPos], EB_SAMInfo.InitializePSAM_SCsam, nLen);		nPos +=nLen;
	nLen = 4;				memcpy(&pData[nPos], EB_SAMInfo.InitializePSAM_SIGN2, nLen);		nPos +=nLen;

	res = EB_RF_Purchase(_EB_TypeAB_RF_Purchase_Purchase_);
	
		
	if(res != RC_SUCCESS) {			
		PRINTF( ">>> EB Purchase Fail...%d\r\n", res);		
		LED_Error();
		goto f01;
	}
	else {
		// Credit PSAM	
		res = nLen =nPos = 0;
		nLen = 4;				memcpy(&pData[nPos], EB_RFInfo.Purchase_SIGN3, nLen);		nPos +=nLen;

		EB_SAMInfo.SIGNind_Check = 0x30;
		
		res = EB_SAM_CreditPSAM(pData, nPos);

		if(res!=RC_SUCCESS)
		{			
f01 :		// 재거래 루틴			
			res = RF_Polling( 0, RF_EB);
			if(	res != RF_EB) return RC_FAILURE;

			RF_SetCard(RF_EB, nBalance);
			
			res=EB_RF_Initialize(_EB_TypeAB_RF_Initialize_RePurchase_, Mpda, 4);

			if(res!=RC_SUCCESS) {
				res = RC_FAILURE;	
			} else {
				res = EB_SAM_InitializePSAM(_EB_SAM_InitializePSAM_RePurchase_,  Mpda);
				if(res!=RC_SUCCESS) {
					res = RC_FAILURE;	
				} else {
					res = EB_RF_Purchase(_EB_TypeAB_RF_Purchase_RePurchase_);
					if(res!=RC_SUCCESS) {
						res = RC_FAILURE;	
					} else {
					
						nLen =nPos = 0;
						nLen = 4;				
						memcpy(&pData[nPos], EB_RFInfo.Purchase_SIGN3, nLen);		
						nPos +=nLen;

						res = EB_SAM_ReCreditPSAM(pData, nPos);
						if(res!=RC_SUCCESS) {
							res = RC_FAILURE;	
						} else {
							// 아래에서 중복 실행. 
							//res = EB_SAM_ReadBalance(1);
							//if(res==RC_SUCCESS) 
							res = RC_DATA;
						}
					}
				}
			}
			
			if(res != RC_DATA)  {
				EB_SAMInfo.SIGNind_Check = 0x32;
				
				memcpy(EB_SAMInfo.CreditPSAM_IDsam, EB_SAMInfo.GetData_IDsam,8);
				memcpy(EB_SAMInfo.CreditPSAM_IDep, EB_RFInfo.Initialize_IDep,8);
				Swap_4(EB_RFInfo.Initialize_NTep, (unsigned char*)&lvalue1);
				lvalue1++;
				Swap_4((unsigned char*)&lvalue1,EB_SAMInfo.CreditPSAM_NTep);
				EB_SAMInfo.CreditPSAM_TRT = 0x01;
				Swap_4(EB_RFInfo.ReadBalance_BAL,(unsigned char*)&lvalue1);
				Swap_4(Mpda, (unsigned char*)&lvalue2);
				lvalue3 = lvalue1- lvalue2;
				Swap_4((unsigned char*)&lvalue3,EB_SAMInfo.CreditPSAM_BALep);
			}
		}

		// 20140421	: 인재 대학교 해킹.
		// 카드잔액 차감 후 오류 발생시 결과값을 0x32->Sing생성실패 로 올려야 합니다.
		// res = RC_SUCCESS;		
		if (res == RC_SUCCESS || res == RC_DATA) 
		{
			EB_SAM_ReadBalance(1);	
						
			nBalance = B2I(EB_SAMInfo.CreditPSAM_BALep[3],EB_SAMInfo.CreditPSAM_BALep[2],EB_SAMInfo.CreditPSAM_BALep[1],EB_SAMInfo.CreditPSAM_BALep[0]);			
								
			sprintf(nData, "%06d", nBalance);	
			
			//PRINTHEX( nData, 6, "구매후금액");
				
			g_VmBalance.BC = 0x04;
			g_VmBalance.DC = HST_DC_CARD_DATA;
			
			// BCD FORMAT			
			g_VmBalance.Data[2] = (nData[1]&0x0f) | ((nData[0]&0x0f)<<4);
			g_VmBalance.Data[1] = (nData[3]&0x0f) | ((nData[2]&0x0f)<<4);
			g_VmBalance.Data[0] = (nData[5]&0x0f) | ((nData[4]&0x0f)<<4);
			  		
			g_VmBalance.Lrc = g_VmBalance.BC^g_VmBalance.DC;
			
			for(i=0; i<(g_VmBalance.BC-1); i++) {
				g_VmBalance.Lrc ^= g_VmBalance.Data[i];		
			}		
			
			// 복원을 위해 백업
			g_TransactionOk	= 1;
			for(i=0;i<8;i++) g_IDepPrev[i]=EB_SAMInfo.CreditPSAM_IDep[i];
					
			UBC_SetPrepaidPacket(1);		
		
			RF_SetResCode(TM_PERCHASE_OK);
				
			//PRINTHEX( &g_VmBalance.BC, g_VmBalance.BC+2, "구매후금액 %d >> ", nBalance);
			//PRINTHEX( (unsigned char*)&g_Tm, sizeof(TM_TRANSACTION), "TM_TRANSACTION");	
			//PRINTF( "TM_PERCHASE_OK\r\n");
		}
		else {						
			PRINTF( ">>> EB Purchase Fail...%d\r\n", res);
						
			LED_Error();
		}
	}
	return res;
}

int UBC_EbRePurchase(LONG nTemp)
{	
	int i, res=RC_FAILURE;
	unsigned long nBalance;
	char nData[8];
	
	unsigned char Mpda[4];
	unsigned char pData[256];
	unsigned char nPos, nLen;
	unsigned long	lvalue1=0, lvalue2=0, lvalue3=0; 
	int nCenter=0;
	
	g_TransactionOk	= 0;
	
	// 재거래 루틴			
	res = RF_Polling( 0, RF_EB);
	if(	res != RF_EB) return RC_FAILURE;

	RF_SetCard(RF_EB, nBalance);
	
	if((res = EB_GetBalance())!=RC_DATA)
		return res;
	
	res=EB_RF_Initialize(_EB_TypeAB_RF_Initialize_RePurchase_, Mpda, 4);

	if(res!=RC_SUCCESS) {
		res = RC_FAILURE;	
	} else {
		res = EB_SAM_InitializePSAM(_EB_SAM_InitializePSAM_RePurchase_,  Mpda);
		if(res!=RC_SUCCESS) {
			res = RC_FAILURE;	
		} else {
			res = EB_RF_Purchase(_EB_TypeAB_RF_Purchase_RePurchase_);
			if(res!=RC_SUCCESS) {
				res = RC_FAILURE;	
			} else {
			
				nLen =nPos = 0;
				nLen = 4;				
				memcpy(&pData[nPos], EB_RFInfo.Purchase_SIGN3, nLen);		
				nPos +=nLen;

				res = EB_SAM_ReCreditPSAM(pData, nPos);
				if(res!=RC_SUCCESS) {
					res = RC_FAILURE;	
				} else {
					// 아래에서 중복 실행. 
					//res = EB_SAM_ReadBalance(1);
					//if(res==RC_SUCCESS) 
					res = RC_DATA;
				}
			}
		}
	}
			
	if(res != RC_DATA)  {
		EB_SAMInfo.SIGNind_Check = 0x32;
		
		memcpy(EB_SAMInfo.CreditPSAM_IDsam, EB_SAMInfo.GetData_IDsam,8);
		memcpy(EB_SAMInfo.CreditPSAM_IDep, EB_RFInfo.Initialize_IDep,8);
		Swap_4(EB_RFInfo.Initialize_NTep, (unsigned char*)&lvalue1);
		lvalue1++;
		Swap_4((unsigned char*)&lvalue1,EB_SAMInfo.CreditPSAM_NTep);
		EB_SAMInfo.CreditPSAM_TRT = 0x01;
		Swap_4(EB_RFInfo.ReadBalance_BAL,(unsigned char*)&lvalue1);
		Swap_4(Mpda, (unsigned char*)&lvalue2);
		lvalue3 = lvalue1- lvalue2;
		Swap_4((unsigned char*)&lvalue3,EB_SAMInfo.CreditPSAM_BALep);
	}
	
	if (res == RC_SUCCESS || res == RC_DATA) 
	{
		EB_SAM_ReadBalance(1);	
					
		nBalance = B2I(EB_SAMInfo.CreditPSAM_BALep[3],EB_SAMInfo.CreditPSAM_BALep[2],EB_SAMInfo.CreditPSAM_BALep[1],EB_SAMInfo.CreditPSAM_BALep[0]);			
							
		sprintf(nData, "%06d", nBalance);	
		
		//PRINTHEX( nData, 6, "구매후금액");
			
		g_VmBalance.BC = 0x04;
		g_VmBalance.DC = HST_DC_CARD_DATA;
		
		// BCD FORMAT			
		g_VmBalance.Data[2] = (nData[1]&0x0f) | ((nData[0]&0x0f)<<4);
		g_VmBalance.Data[1] = (nData[3]&0x0f) | ((nData[2]&0x0f)<<4);
		g_VmBalance.Data[0] = (nData[5]&0x0f) | ((nData[4]&0x0f)<<4);
		  		
		g_VmBalance.Lrc = g_VmBalance.BC^g_VmBalance.DC;
		
		for(i=0; i<(g_VmBalance.BC-1); i++) {
			g_VmBalance.Lrc ^= g_VmBalance.Data[i];		
		}		
		
		// 복원을 위해 백업
		g_TransactionOk	= 1;
		for(i=0;i<8;i++) g_IDepPrev[i]=EB_SAMInfo.CreditPSAM_IDep[i];
				
		UBC_SetPrepaidPacket(1);		
	
		RF_SetResCode(TM_PERCHASE_OK);
			
		//PRINTHEX( &g_VmBalance.BC, g_VmBalance.BC+2, "구매후금액 %d >> ", nBalance);
		//PRINTHEX( (unsigned char*)&g_Tm, sizeof(TM_TRANSACTION), "TM_TRANSACTION");	
		//PRINTF( "TM_PERCHASE_OK\r\n");
	}
	else {						
		PRINTF( ">>> EB Purchase Fail...%d\r\n", res);
					
		LED_Error();
	}
	
	return res;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Tmoney 잔액 조회 						                                                                //
//////////////////////////////////////////////////////////////////////////////////////////////////////////
int UBC_TmBalance(unsigned long *nBalance) 
{
	int res = RC_FAILURE;
	char nData[8];

	*nBalance = 0;
			
	// Initialize RF Card
	if (RF_InfField(CLA_TMONEY_RF, 0x40, 0x03, 0x00, 0x04, "\x00\x00\x00\x00", 0x17)== RC_SUCCESS) {			
		*nBalance = B2I(RFInfo.nRcvData[5],RFInfo.nRcvData[4],RFInfo.nRcvData[3],RFInfo.nRcvData[2]);


		// 충전 금액 상한...
		if(*nBalance > TM_MAX_BALANCE || (*nBalance==0)) {	
			if(!play_wav_check())  	
				play_wav("adpcm_현금판매.bin");		
		}
		else {
			res = RC_OK;			
		}
	}
	   	
	return res;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Tmoney 판매 취소 Data(DC=00, Bit 4)			                                                                //
//////////////////////////////////////////////////////////////////////////////////////////////////////////
int UBC_TmCancel(void)
{
	int i, res = RC_FAILURE;
	
#if 0				
	if(!TM_CheckTerminalFunction(TERMINAL_OFFLINE_PURCHASE) || !TM_CheckIssuerFunction(ISSUER_PURCHASE)) {				
		RFInfo.nResCode = TM_PERCHASE_CANCEL;
		return RC_NOT_SUPPORT;
	}	
#endif	

	//nBalance = B2I(g_Tm.Mpda.data[3],g_Tm.Mpda.data[2],g_Tm.Mpda.data[1],g_Tm.Mpda.data[0]);
	//PRINTHEX( (unsigned char*)&g_Tm.Mpda.data[0], 4, ">> Tm.Mpda %d ", nBalance);
	
	// Initialize RF Card, 거래 금액
	if ((res = RF_InfField(CLA_TMONEY_RF, 0x40, 0x05, 0x00, 0x04, (uchar*)&g_Tm.Mpda.data, 0x27)) != RC_SUCCESS) {
		return res;
	}
	
	g_Tm.CardType = g_TmPacket.CardType;
	g_Tm.AlgID    = g_TmPacket.AlgID;
	g_Tm.UserCode = g_TmPacket.UserCode;
		
	// 거래전 금액
	g_Tm.Balance.data[0] = RFInfo.nRcvData[2];
	g_Tm.Balance.data[1] = RFInfo.nRcvData[3];
	g_Tm.Balance.data[2] = RFInfo.nRcvData[4];
	g_Tm.Balance.data[3] = RFInfo.nRcvData[5];	
			
	// Initialize PSAM for Cancelation
	for(i=0; i<4; i++) {
		RFInfo.nRcvData[RFInfo.nRcvLen++] = g_Tm.Mpda.data[i]; // 거래금액 추가
	}
		
	res = SAM_InfField(CLA_TMONEY_PSAM, 0x46, 0x00, 0x00, RFInfo.nRcvLen, RFInfo.nRcvData, 0x12);	
	if (res != RC_SUCCESS) {		
		return res;
	}
	
	res = RF_InfField(CLA_TMONEY_RF, 0x50, 0x00, 0x00, 0x12, g_SamInfo.nRcvData, 0x00);
	if (res != RC_SUCCESS) {		
		return RC_CARD_ERROR;
	}

	// Cancelation PSAM
	res = SAM_InfField(CLA_TMONEY_PSAM, 0x48, 0x00, 0x00, 0x00, RFInfo.nRcvData, 0x31);	
	if (res != RC_SUCCESS) {	
		return res;
	}
	
	memcpy((char*)&g_Tm.TRT, (char*)g_SamInfo.nRcvData, g_SamInfo.nRcvLen);
	
	
	UBC_SetPrepaidPacket(0);			
	g_TransactionOk=0;
	
	PRINTF( "TM_TRANSACTION_CANCEL  ... %d\r\n", g_SamInfo.nRcvLen);
		
	return RC_DATA;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 판매금액 및 컬럼 DATA (DC = 01)                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////////////////////
int UBC_TmPurchase(LONG nTemp)
{	
	int i, res;
	unsigned long nBalance;
	char nData[8];
	LONG nAmount;

	
	g_TransactionOk	= 0;

	// 금액이 0 이면
	if(!nTemp.value) 	return RC_FAILURE;
	
	if(!TM_CheckIssuerFunction(ISSUER_PURCHASE)) {
	
		TM_LoadTerminalProperty();
		
		if(!TM_CheckIssuerFunction(ISSUER_PURCHASE)) {	
			return RC_FAILURE; //RC_NOT_SUPPORT;
		}
	}
	
	res = RF_Polling( 0, RF_TMONEY);
	if( res != RF_TMONEY ) return RC_FAILURE;
	
	RF_SetCard(RF_TMONEY, nBalance);
	
	res = TM_Select(0);	
	if (res != RC_SUCCESS) {
		return res;
	}

	nAmount.data[0] = nTemp.data[3];
	nAmount.data[1] = nTemp.data[2];
	nAmount.data[2] = nTemp.data[1];
	nAmount.data[3] = nTemp.data[0];	
	
	// Initialize RF Card
	res = RF_InfField(CLA_TMONEY_RF, 0x40, 0x03, 0x00, 0x04, (unsigned char*)&nAmount.data, 0x17);
	if (res != RC_SUCCESS) {
		return res;
	}	
		
	g_Tm.AlgID    = g_TmPacket.AlgID;
	g_Tm.CardType = g_TmPacket.CardType;
	g_Tm.UserCode = g_TmPacket.UserCode;
		
	// 거래전 금액
	g_Tm.Balance.data[0] = RFInfo.nRcvData[2];
	g_Tm.Balance.data[1] = RFInfo.nRcvData[3];
	g_Tm.Balance.data[2] = RFInfo.nRcvData[4];
	g_Tm.Balance.data[3] = RFInfo.nRcvData[5];	
		
	HEXDUMP( (unsigned char*)g_Tm.Balance.data, 4, "구매전금액 %d >> ", B2I(g_Tm.Balance.data[3],g_Tm.Balance.data[2],g_Tm.Balance.data[1],g_Tm.Balance.data[0]));	
	
	// Initialize PSAM	
	for(i=0; i<4; i++) {
		RFInfo.nRcvData[ RFInfo.nRcvLen++] = nAmount.data[i]; // 거래금액 추가
	}	
			
	res = SAM_InfField(CLA_TMONEY_PSAM, 0x02, 0x00, 0x00, RFInfo.nRcvLen, RFInfo.nRcvData, 0x12);		
	if (res != RC_SUCCESS){	
		return res;	
	}
	
	// Purchase RF	
	res = RF_InfField(CLA_TMONEY_RF, 0x46, 0x00, 0x00, g_SamInfo.nRcvLen-2, g_SamInfo.nRcvData, 0x04);				
	
	if(res != RC_SUCCESS) {	
		// 재거래 루틴
		res = RF_Polling( 0, RF_TMONEY);
		if(	res != RF_TMONEY ) return RC_FAILURE;

		RF_SetCard(RF_TMONEY, nBalance);
		
		res = TM_Select(0);	
		if (res != RC_SUCCESS) {
			return res;
		}
		nAmount.data[0] = nTemp.data[3];
		nAmount.data[1] = nTemp.data[2];
		nAmount.data[2] = nTemp.data[1];
		nAmount.data[3] = nTemp.data[0];
		
		// Initialize RF Card
		res = RF_InfField(CLA_TMONEY_RF, 0x40, 0x04, 0x00, 0x04, (unsigned char*)&nAmount.data, 0x27);
		if (res != RC_SUCCESS) {
			return res;
		}	
			
		g_Tm.AlgID    = g_TmPacket.AlgID;
		g_Tm.CardType = g_TmPacket.CardType;
		g_Tm.UserCode = g_TmPacket.UserCode;
			
		// 거래전 금액
		g_Tm.Balance.data[0] = RFInfo.nRcvData[2];
		g_Tm.Balance.data[1] = RFInfo.nRcvData[3];
		g_Tm.Balance.data[2] = RFInfo.nRcvData[4];
		g_Tm.Balance.data[3] = RFInfo.nRcvData[5];	

		// Re Initialize PSAM	
		for(i=0; i<4; i++) {
			RFInfo.nRcvData[ RFInfo.nRcvLen++] = nAmount.data[i]; // 거래금액 추가
		}	
				
		res = SAM_InfField(CLA_TMONEY_PSAM, 0x22, 0x00, 0x00, RFInfo.nRcvLen, RFInfo.nRcvData, 0x12);		
		if (res != RC_SUCCESS){	
			return res;	
		}

		// Re Purchase RF	
		res = RF_InfField(CLA_TMONEY_RF, 0x48, 0x00, 0x00, g_SamInfo.nRcvLen-2, g_SamInfo.nRcvData, 0x04);
		if (res != RC_SUCCESS){			
			PRINTF( ">>> T-Money Purchase Fail...%d\r\n", res);
					
			
			LED_Error();
			return res;	
		}
		// Re Credit PSAM
		res = SAM_InfField(CLA_TMONEY_PSAM, 0x44, 0x00, 0x00, RFInfo.nRcvLen, RFInfo.nRcvData, 0x31);
		if (res != RC_SUCCESS)
		{			
			return res;
		}	
		goto f01;
	}
	else {
		// Credit PSAM	
		res = SAM_InfField(CLA_TMONEY_PSAM, 0x04, 0x00, 0x00, RFInfo.nRcvLen, RFInfo.nRcvData, 0x31);	
		if (res == RC_SUCCESS) {
f01 :		
			memcpy(&g_Tm.TRT, g_SamInfo.nRcvData, g_SamInfo.nRcvLen);
				
			nBalance = B2I(g_Tm.BALep.data[3],g_Tm.BALep.data[2],g_Tm.BALep.data[1],g_Tm.BALep.data[0]);			
								
			sprintf(nData, "%06d", nBalance);	
			
			PRINTHEX( nData, 6, "구매후금액");
				
			g_VmBalance.BC = 0x04;
			g_VmBalance.DC = HST_DC_CARD_DATA;
			
			// BCD FORMAT			
			g_VmBalance.Data[2] = (nData[1]&0x0f) | ((nData[0]&0x0f)<<4);
			g_VmBalance.Data[1] = (nData[3]&0x0f) | ((nData[2]&0x0f)<<4);
			g_VmBalance.Data[0] = (nData[5]&0x0f) | ((nData[4]&0x0f)<<4);
			  		
			g_VmBalance.Lrc = g_VmBalance.BC^g_VmBalance.DC;
			
			for(i=0; i<(g_VmBalance.BC-1); i++) {
				g_VmBalance.Lrc ^= g_VmBalance.Data[i];		
			}		
			
			// 복원을 위해 백업
			g_TransactionOk	= 1;
			for(i=0;i<8;i++) g_IDepPrev[i]=g_Tm.IDep[i];

				
			UBC_SetPrepaidPacket(1);		

			RF_SetResCode(TM_PERCHASE_OK);
				
			//PRINTHEX( &g_VmBalance.BC, g_VmBalance.BC+2, "구매후금액 %d >> ", nBalance);
			//PRINTHEX( (unsigned char*)&g_Tm, sizeof(TM_TRANSACTION), "TM_TRANSACTION");	
			//PRINTF( "TM_PERCHASE_OK\r\n");
		}
		else {						
			PRINTF( ">>> T-Money Purchase Fail...%d\r\n", res);
						
			LED_Error();
		}
	}
	return res;
}

int UBC_TmRePurchase(LONG nTemp)
{
	
	int i, res;
	unsigned long nBalance;
	char nData[8];
	LONG nAmount;
	
	g_TransactionOk	= 0;
	
	// 재거래 루틴
	res = RF_Polling( 0, RF_TMONEY);
	if(	res != RF_TMONEY ) return RC_FAILURE;

	RF_SetCard(RF_TMONEY, nBalance);
	
	res = TM_Select(0);	
	if (res != RC_SUCCESS) {
		return res;
	}
	nAmount.data[0] = nTemp.data[3];
	nAmount.data[1] = nTemp.data[2];
	nAmount.data[2] = nTemp.data[1];
	nAmount.data[3] = nTemp.data[0];
	
	// Initialize RF Card
	res = RF_InfField(CLA_TMONEY_RF, 0x40, 0x04, 0x00, 0x04, (unsigned char*)&nAmount.data, 0x27);
	if (res != RC_SUCCESS) {
		return res;
	}	
		
	g_Tm.AlgID    = g_TmPacket.AlgID;
	g_Tm.CardType = g_TmPacket.CardType;
	g_Tm.UserCode = g_TmPacket.UserCode;
		
	// 거래전 금액
	g_Tm.Balance.data[0] = RFInfo.nRcvData[2];
	g_Tm.Balance.data[1] = RFInfo.nRcvData[3];
	g_Tm.Balance.data[2] = RFInfo.nRcvData[4];
	g_Tm.Balance.data[3] = RFInfo.nRcvData[5];	

	// Re Initialize PSAM	
	for(i=0; i<4; i++) {
		RFInfo.nRcvData[ RFInfo.nRcvLen++] = nAmount.data[i]; // 거래금액 추가
	}	
			
	res = SAM_InfField(CLA_TMONEY_PSAM, 0x22, 0x00, 0x00, RFInfo.nRcvLen, RFInfo.nRcvData, 0x12);		
	if (res != RC_SUCCESS){	
		return res;	
	}

	// Re Purchase RF	
	res = RF_InfField(CLA_TMONEY_RF, 0x48, 0x00, 0x00, g_SamInfo.nRcvLen-2, g_SamInfo.nRcvData, 0x04);
	if (res != RC_SUCCESS){			
		PRINTF( ">>> T-Money Purchase Fail...%d\r\n", res);
		LED_Error();
		return res;	
	}
	// Re Credit PSAM
	res = SAM_InfField(CLA_TMONEY_PSAM, 0x44, 0x00, 0x00, RFInfo.nRcvLen, RFInfo.nRcvData, 0x31);
	if (res == RC_SUCCESS)
	{	
		memcpy(&g_Tm.TRT, g_SamInfo.nRcvData, g_SamInfo.nRcvLen);
			
		nBalance = B2I(g_Tm.BALep.data[3],g_Tm.BALep.data[2],g_Tm.BALep.data[1],g_Tm.BALep.data[0]);			
							
		sprintf(nData, "%06d", nBalance);	
		
		PRINTHEX( nData, 6, "구매후금액");
			
		g_VmBalance.BC = 0x04;
		g_VmBalance.DC = HST_DC_CARD_DATA;
		
		// BCD FORMAT			
		g_VmBalance.Data[2] = (nData[1]&0x0f) | ((nData[0]&0x0f)<<4);
		g_VmBalance.Data[1] = (nData[3]&0x0f) | ((nData[2]&0x0f)<<4);
		g_VmBalance.Data[0] = (nData[5]&0x0f) | ((nData[4]&0x0f)<<4);
		  		
		g_VmBalance.Lrc = g_VmBalance.BC^g_VmBalance.DC;
		
		for(i=0; i<(g_VmBalance.BC-1); i++) {
			g_VmBalance.Lrc ^= g_VmBalance.Data[i];		
		}		
		
		// 복원을 위해 백업
		g_TransactionOk	= 1;
		for(i=0;i<8;i++) g_IDepPrev[i]=g_Tm.IDep[i];

			
		UBC_SetPrepaidPacket(1);		

		RF_SetResCode(TM_PERCHASE_OK);
			
		//PRINTHEX( &g_VmBalance.BC, g_VmBalance.BC+2, "구매후금액 %d >> ", nBalance);
		//PRINTHEX( (unsigned char*)&g_Tm, sizeof(TM_TRANSACTION), "TM_TRANSACTION");	
		//PRINTF( "TM_PERCHASE_OK\r\n");
	}
	else {						
		PRINTF( ">>> T-Money Purchase Fail...%d\r\n", res);
					
		LED_Error();
	}
	
	return res;
}

int UBC_MybiPurchase(LONG lFare) 
{
	int res = RC_FAILURE;
	LONG lPpurse, lTransactionCount;
	unsigned char bErrorBlock = 0;
	LONG lTerminalID;
	unsigned char *transactionTime = &g_Today[1];	
		
	// 금액이 0 이면
	if(!lFare.value)
		return RC_FAILURE;
	
	RF_GetCard();
		
	//TID  31 30 30 30 30 30 30 30 30 33 00 FF               1000000003..	
	Char2BCD(&g_VmConfig.TID[2], lTerminalID.data,  8);
		
	if((res = MYBI_AuthenticationRFCard())!= RC_OK)
		return res;	

	res = MIBI_ReadCardInfo(&lPpurse, &lTransactionCount, &bErrorBlock);
	if(res != RC_OK)
		return RC_RFCardDataReadError;	
			
	if( MYBI_SyncBlock(bErrorBlock)==RC_OK) {				
		if( MYBI_PurchaseTaxi(lFare) == RC_OK) {
			res = MYBI_MakeReturnData(transactionTime, lTerminalID, lFare);
			if(res == RC_SUCCESS) {						
				UBC_SetPrepaidPacket(1);
				RF_SetResCode(TM_PERCHASE_OK);
			}
		}
	}
	
	return res;
}

int UBC_MybiBalance(unsigned long *nBalance) 
{
	int res = RC_FAILURE;
	LONG lTmpBlock[4];
	char nData[8];
	LONG lPpurse, lTransactionCount;
	unsigned char bErrorBlock = 0;
		
	RF_GetCard();

	if(	RFInfo.nCard==RF_MYBI || RFInfo.nCard==RF_USAM ) 
	{
		
		if((res = MYBI_AuthenticationRFCard())== RC_OK) 
		{	

			res = MIBI_ReadCardInfo(&lPpurse, &lTransactionCount, &bErrorBlock);
			if(res != RC_OK) {
				if(!play_wav_check())  	
					play_wav("adpcm_현금판매.bin");		
				return res;
			}
			res = MYBI_IsSameCertifier(&g_MCardInfo.b14.stat, g_MCardInfo.b14.purchase_certi_code);

			if(res != RC_OK)  {
				if(!play_wav_check())  	
					play_wav("adpcm_현금판매.bin");		
				return res;
			}

			
			MF_AuthBlock(MF_CLASSIC_KEYA, 12);
			if(MF_ReadBlock(12, (unsigned char*)lTmpBlock) == RC_SUCCESS) {
				*nBalance = lTmpBlock[0].value;
				if(*nBalance > TM_MAX_BALANCE || ((long)*nBalance<=0)) {
					if(!play_wav_check())  	
						play_wav("adpcm_현금판매.bin");		
				}
				else {
					res = RC_OK;
					
					sprintf(nData, "%06d", *nBalance);		
					// BCD FORMAT			
					g_VmBalance.Data[2] = (nData[1]&0x0f) | ((nData[0]&0x0f)<<4);
					g_VmBalance.Data[1] = (nData[3]&0x0f) | ((nData[2]&0x0f)<<4);
					g_VmBalance.Data[0] = (nData[5]&0x0f) | ((nData[4]&0x0f)<<4);
				}
			}
		}
	}

	return res;
}

void UBC_GetKbTrack2(unsigned char *pTrack2)
{
	Hex2Ascii(g_KbData.CID, &pTrack2[0], 8);
	pTrack2[16]='=';
	Hex2Ascii(g_KbData.ExpireDate, &pTrack2[17], 2);			

	g_MsrInfo.nCnt2 = 21;
	memcpy(g_MsrInfo.nTrack2, pTrack2, g_MsrInfo.nCnt2);
}

void UBC_SetTmonyTrack2(unsigned char *pTrack2, unsigned char *pExpire)
{
	memcpy(g_KbData.CID, pTrack2, 8);
	memcpy(g_KbData.ExpireDate, pExpire, 2);		
}

void UBC_SetTrack2Backup(unsigned long nAmount)
{	
	m_nKbData  = g_KbData;
	m_nMsrInfo = g_MsrInfo;
	
	m_nAmount = nAmount;
	m_nCraditType = g_CraditType;
	
	m_nNanoTotCnt = g_TmsLog.TotCnt;
}

void UBC_TrackDataClear(void)
{
	unsigned char dmask;
	int i;

	for(i=0,dmask=0; i<3; i++) {
		// MSR 정보 삭제.
		memset(g_MsrInfo.nTrack2, dmask, sizeof(g_MsrInfo.nTrack2));
		// Payon 정보 삭제
		memset((char *)&g_KbData, dmask, sizeof(KB_DATA));
		memset((char *)&g_Tm, dmask, sizeof(TM_TRANSACTION));		
		// 요청 트랙 대이터  삭제.	
		memset((char *)&VAN_BasicReq.CSN, dmask, sizeof(VAN_BasicReq.CSN));
		memset((char *)&g_NanoVANBasicReq.CSN, dmask, sizeof(g_NanoVANBasicReq.CSN));
		memset((char *)&g_CurrVANBasicReq.CSN, dmask, sizeof(g_CurrVANBasicReq.CSN));	
		// 요청 암호화 버퍼  삭제.	
		memset((char *)&VAN_BasicReq.AddedInfo, dmask, sizeof(VAN_BasicReq.AddedInfo));
		memset((char *)&g_NanoVANBasicReq.AddedInfo, dmask, sizeof(g_NanoVANBasicReq.AddedInfo));
		memset((char *)&g_CurrVANBasicReq.AddedInfo, dmask, sizeof(g_CurrVANBasicReq.AddedInfo));	
		// 응답 암호화 버퍼  삭제
		memset((char *)VAN_BasicRes.AddedInfo, dmask, sizeof(VAN_BasicRes.AddedInfo));	

		dmask = dmask ? 0 : 0xFF;
	}
	
	#ifdef DBG_KTC
		HEX_Dump(g_MsrInfo.nTrack2, 64, "t2 clr address : 0x%08X", g_MsrInfo.nTrack2);
	#endif
}

void UBC_TrackBackupDataClear(void)
{
	unsigned char dmask;
	int i;

	for(i=0,dmask=0; i<3; i++) {
		// MSR 정보 삭제.
		memset(m_nMsrInfo.nTrack2, dmask, m_nMsrInfo.nCnt2);		
		// Payon 정보 삭제
		memset((char *)&m_nKbData, dmask, sizeof(KB_DATA));	
		dmask = dmask ? 0 : 0xFF;
	}
}
	
		

int UBC_KbBalance(void) 
{
	int i, res=RC_CARD_ERROR;
	KB_C0_B0 nB0;
	KB_C0_B2 nB2;
	KB_C12_B48 nB48;
	unsigned char *pB0=(unsigned char*)&nB0, *pB2=(unsigned char*)&nB2, *pB48=(unsigned char*)&nB48, *pData;
	unsigned char tmp[20];
	
	pData = (unsigned char*)&g_KbData;

 	// get working key for sector 0 
 	switch(g_samMap[g_SamInfo.nCh].Type) {
 	case PSAM_KB : 	
 		res = KB_GetMifareWorkingkey(0);
 		break;
 	case PSAM_CNC :
 		res = CNC_GetMifareWorkingKey(0,0);
 		break;
 	}
 	
    if (res != RC_SUCCESS) {  
        return RC_SAM_ERROR;
    }
  
			
	for(i=0;i<6;i++) {
		RFInfo.nKeyA[0][i] = g_SamInfo.nRcvData[i];
	}
		
	MF_AuthBlock(MF_CLASSIC_KEYA, 0);
	// key auth and get read a block 0 	
	res = MF_ReadBlock(0, pB0);		
	if( res != RC_SUCCESS) return RC_CARD_ERROR;	
			
	// key auth and get read a block 2 
	res = MF_ReadBlock(2, pB2);	
	if( res != RC_SUCCESS) return RC_CARD_ERROR;			
		
	// get key for sector 12 
 	switch(g_samMap[g_SamInfo.nCh].Type) {
 	case PSAM_KB : 	
 		res = KB_GetMifareWorkingkey(12);
 		break;
 	case PSAM_CNC :

		memcpy(tmp, g_CardInfo.csn.data, 4);
		memcpy(tmp+4, pB2, 16);
		CNC_CertifyCode(tmp, 20);
		
 		res= CNC_GetMifareWorkingKey(12,'1');
 		break;
 	}
		
	if (res != RC_SUCCESS) {
        return RC_SAM_ERROR;
    }
            	
	for(i=0;i<6;i++) {
		RFInfo.nKeyA[12][i] = g_SamInfo.nRcvData[i];
	}	
		
	MF_AuthBlock(MF_CLASSIC_KEYA, 48);
	res = MF_ReadBlock(48, pB48);
		
	if( res != RC_SUCCESS) {
		return RC_CARD_ERROR;
	}
    	
	for(i=0;i<16;i++) pData[i]    = pB0[i];				// 발급사종보	
	for(i=0;i<16;i++) pData[i+16] = pB2[i] ^pB48[i];	// 카드번호, 카드만료일, 카드 종류, 고유일련번호, 일련보호 xor
		
	PRINTHEX( pData, 32, "KB");
	
	return RC_DATA;
}

void UBC_CreditBufferClear(void) {
	g_CraditType = 0;
	g_CreditAuth = 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
//

int UBC_Polling(unsigned long *pBalance) 
{	
	int ret = -1;
	unsigned char nData[8];
	
	*pBalance=0;
	ret = RF_GetCard();
	////////////////////////////////////////////////////////////////////////////////////////
	//
	if( g_NanoBackup.push == g_NanoBackup.pop) 
	{		
		switch(ret) {
		case RF_MSR :
			g_CraditType = 1;	
	 		break;
		case RF_KB :		
			g_CraditType = 2;
			break;
		case RF_EMV :				
			g_CraditType = 3;
			break;	
		default :
			g_CraditType = 0;
			break;
		}			

		if(ret>0 && g_CraditType) 
		{
		     	g_CreditAuth = 1;		
			if((g_GlobalInfo.VmUbcn == VM_BLAND_LOTTE_GATEBANK) || (g_GlobalInfo.VmUbcn == VM_BLAND_MAINTECH_GATEBANK))
				RF_SetResCode(TM_CARD_IS);	
			else 
				RF_SetResCode(TM_AUTH_PROGRESS);	
			memcpy(g_VmBalance.Data, "\x00\x43\x3d", 3);
			return ret;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////
	//

	switch(ret) {
	case RF_USAM :
		break;
	case RF_TMONEY :
		*pBalance = RF_GetBalance();	
		break;
	case RF_EB :
		*pBalance = RF_GetBalance();	
		break;
	case RF_KB :
	case RF_EMV :
	case RF_MSR :
		break;
	default :
		break;
	}

	if(ret>0) {			
		if((g_GlobalInfo.VmUbcn == VM_BLAND_LOTTE_GATEBANK) || (g_GlobalInfo.VmUbcn == VM_BLAND_MAINTECH_GATEBANK))
			RF_SetResCode(TM_CARD_IS);	
		else 
			RF_SetResCode(TM_AUTH_COMPLETE);	
		
		sprintf(nData, "%06d", *pBalance);		
		// BCD FORMAT			
		g_VmBalance.Data[2] = (nData[1]&0x0f) | ((nData[0]&0x0f)<<4);
		g_VmBalance.Data[1] = (nData[3]&0x0f) | ((nData[2]&0x0f)<<4);
		g_VmBalance.Data[0] = (nData[5]&0x0f) | ((nData[4]&0x0f)<<4);
	}
	else {
		*pBalance = 0;
		memcpy(g_VmBalance.Data, "\x00\x00\x00", 3);
	}
	return ret;

}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
int UBC_ReadBalance(int nSort)
{
	static int retry=0;
	static unsigned char bal[4];
	
	int ret=-1;
	unsigned long nBalance=0;
				

	if(!nSort) {		
		retry = 0;
		if( (LED_GetStat() != LED_READY) && (LED_GetStat() != LED_PROCESS )){
			LED_Ready();
			
			LCD_Flush(0);
			LCD_Display(LCD_SORT_CENTER, 0, 0, VM_GetManufacturer());   
			LCD_Display(LCD_SORT_CENTER, 5, 0, VM_GetMachineInfo());					
		}		

		RF_CheckBalanceOn();
		
		ret = UBC_Polling(&nBalance);			
		if(ret>0) {
			if(!memcmp(g_VmBalance.Data, "\x00\x00\x00", 3)) 
			{											
				RF_SetCard(0,0);										
				RF_SetResCode(TM_TRANSACTION_CANCEL);					
			}
			else {			
				RF_SetClass(RF_GetCard());
				RF_CheckBalanceOff();
			}
			HEXDUMP(g_VmBalance.Data,  3, "\r\nbal %d", nBalance);
		}
		else {	
		 	//PRINTF( ".Not Find.\r\n");				
			RF_CheckOut();			
			RF_SetResCode(TM_FIND_CARD);			
					
			// BCD FORMAT				
			memcpy(g_VmBalance.Data,  "\x00\x00\x00", 3);		
			memcpy(g_VmBalance2.Data, "\xFF\xFF\xFF", 3);
			
		}			
	}
	else {
		ret = UBC_Polling(&nBalance);	
	
		if(nBalance) {
			retry = 0;
			memcpy(bal, g_VmBalance.Data, 3);
		}
		else {
			retry += 1;
			memcpy(g_VmBalance.Data, bal, 3);
		}
		
		if(!nBalance && (retry>3) ) 
		{					
			RF_CheckOut();			
			if((g_GlobalInfo.VmUbcn == VM_BLAND_LOTTE_GATEBANK) || (g_GlobalInfo.VmUbcn == VM_BLAND_MAINTECH_GATEBANK))
				RF_SetResCode(TM_TRANSACTION_CANCEL);
			else 
				RF_SetResCode(TM_AUTH_COMPLETE);					
		}
		else 
		if((g_GlobalInfo.VmUbcn == VM_BLAND_LOTTE_GATEBANK) || (g_GlobalInfo.VmUbcn == VM_BLAND_MAINTECH_GATEBANK)) {
			RF_SetResCode(TM_READ_BALANCE);
		}		
	}
	
	
	return ret;
}

void UBC_OperatingInfo(void)
{	
	//if(PSAM_FindCh(RF_USAM)>=0)
		UBC_OperatingInfoReq(5); // CASHBEE OPERATIONG INFORMATION
		
	if(PSAM_FindCh(RF_TMONEY)>=0)
		UBC_OperatingInfoReq(1); // TMONEY OPERATIONG INFORMATION		
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

