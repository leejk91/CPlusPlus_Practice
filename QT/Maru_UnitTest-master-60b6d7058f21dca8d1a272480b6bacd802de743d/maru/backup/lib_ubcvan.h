#ifndef __UBC_VAN_H__
#define __UBC_VAN_H__

#include "typedef.h"
#include "lib_vm.h"

#define KB_CREDIT_TEST	0


#define UBC_LOG_MAX			(1024)
#define TMS_LOG_MAX			(1000)
#define KORAIL_LOG_MAX		(1000)


typedef struct _TMS_LOG_{
	unsigned long TotCnt;
	unsigned long TotAmount;	
} TMS_LOG, *PTMS_LOG;
extern TMS_LOG g_TmsLog;

typedef struct _TMS_PURCHASE_INFO_REQ_{
	unsigned char Time[14];
	unsigned char TID[10];
	unsigned char ClearTime[14];
	unsigned char Type;
	unsigned char Col[2];
	unsigned char CurAmount[32];
	unsigned char TotCnt[32];
	unsigned char TotAmount[32];	
} __attribute__ ((packed))  TMS_PURCHASE_INFO_REQ, *PTMS_PURCHASE_INFO_REQ;
extern TMS_PURCHASE_INFO_REQ  g_TmsPurchaseInfoReq;

typedef struct _UBC_VAN_PREPAID_APPROVE_REQ_ {
	unsigned char TransactionTime[14];			//an				YYYYMMDD hhmmss
	unsigned char OrganCode;					//an				��� �ڵ�
	unsigned char TransactionClass[2];			//an				�ŷ� ����
	unsigned char TransactionType;				//an				�Է¸�ü �����ڵ�
	unsigned char IDcp[20];						//an				ī���ȣ
	unsigned char Balance[8];					//an				�ŷ��� �ܾ�
	unsigned char Mpda[8];						//an				�ŷ� �ݾ�
	unsigned char BALep[8];						//an				�ŷ� �� �ܾ�
	unsigned char IDCenter[2];					//an				����ȭ�� �ĺ�
	unsigned char RID[6];						//an				������ ID
	unsigned char NTep[10];						//an				�ŷ� �Ϸ� ��ȣ
	unsigned char IDPsam[20];					//an				SAM ID
	unsigned char NTPsam[10];					//an				SAM �ŷ���ȣ
	unsigned char TotPsam[10];					//an				SAM �ŷ� �Ѿ�
	unsigned char VkindKey[2];					//an				���� Ű����
	unsigned char NCPsam[10];					//an				���� �Ϸù�ȣ
	unsigned char NIPsam[8];					//an				���� �Ǽ�
	unsigned char Algep[2];						//an				�˰��� ID
	unsigned char SIGNind[20];					//an				������
	unsigned char CardType[2];					//an				ī�� Ÿ��
	unsigned char UserCode[2];					//an				ī�� �ҽ��� ����
	unsigned char EtcID[20];					//an				���ĺ���ȣ �й�/���
	unsigned char UseType;						//an				��뱸��
	unsigned char PPpsam[6];					//an				���Ա�� ID
	unsigned char Riep[16];						//an				����
	unsigned char NIpp[4];						//an				���ų��� ����
	unsigned char _IDsam[20];					//an				SAM ID
	unsigned char _NTsam[10];					//an				SAM �ŷ���ȣ
	unsigned char _NTep[10];					//an				�ŷ� �Ϸ� ��ȣ
	unsigned char _Mpda[10];					//an				�ŷ� �ݾ�
	unsigned char _TransactionTime[14];			//an				YYYYMMDD hhmmss
	unsigned char Remark[60];					//an				
	unsigned char Filtter[13];
} __attribute__ ((packed))  NanoVANPrepaidApproveReq, *PNanoVANPrepaidApproveReq;
// ���� ���� _�� ���ŷ� ����
extern NanoVANPrepaidApproveReq VAN_ApproveReq;

#define FS			0x1C

typedef struct _UBC_VAN_BASIC_REQ_ {
	unsigned char stx;							//bin	STX
	unsigned char MsgLen[4];					//an	�ܸ����ȣ - ETX ����
	unsigned char TID[10];						//an	�ܸ��� ��ȣ "1234567890"
	unsigned char TransactionClass[2];			//an	�ŷ� ����
	unsigned char PacketClass[2];				//an	���� ����
	unsigned char TransactionType;				//an	�Է¸�ü �����ڵ�
	unsigned char TransactionCount[5];			//an	�ܸ���ŷ� �Ϸù�ȣ
	unsigned char PacketVersion[2];				//an	"T1"
	unsigned char Encryption;					//an	���� ��ȭȭ ����, 0:��ȣȭ����, 1:��ȣȭ 
	unsigned char RFU1[10];						//ab 	�����
	unsigned char FS1 ;							//bin	0x1C
	unsigned char CSN[40];						//an	ī���ȣ
	unsigned char FS2 ;							//bin	0x1C
	unsigned char Installment[2];				//an	�Һ� ������
	unsigned char FS3 ;							//bin	0x1C
	unsigned char Amount[10];					//an	���αݾ�, �Ѱŷ� �ݾ� 
	unsigned char FS4 ;							//bin	0x1C
	unsigned char ServicePrice[10];				//an	�����
	unsigned char FS5 ;							//bin	0x1C
	unsigned char Tax[10];						//an	�ΰ���ġ��
	unsigned char FS6 ;							//bin	0x1C
	unsigned char Bill;							//an	��ǥ��뱸��
	unsigned char FS7 ;							//bin	0x1C
	unsigned char ApproveDate[8];				//an	�� �ŷ� ����
	unsigned char FS8 ;							//bin	0x1C
	unsigned char ApproveNum[10];				//an	���ŷ� ���� ��ȣ
	unsigned char FS9 ;							//bin	0x1C
	unsigned char AddedInfo[2048];					//an				�ΰ� ����

//	unsigned char FS10;							//bin	0x1C
//	unsigned char COLUMN[2];						//bin	�Ǹŵ� Column	
	
//	unsigned char etx;							//bin			ETX
//	unsigned char CRC[2]; 									
} __attribute__ ((packed))  NanoVANBasicReq, *PNanoVANBasicReq;
extern NanoVANBasicReq VAN_BasicReq;
extern NanoVANBasicReq g_NanoVANBasicReq, g_CurrVANBasicReq;

typedef struct _UBC_BAKUP_{
	int push, pop;
	int cnt;
} UBC_BAKUP, *PUBC_BAKUP;
extern UBC_BAKUP g_NanoBackup, g_NanoBackup1, g_TmsBackup, g_TmsBackup1;

#define UBC_VAN_BASIC_RES_ADD_INFO_LEN	2048
typedef struct _UBC_VAN_BASIC_RES_{
	unsigned char stx;							//bin	STX
	unsigned char MsgLen[4];					//an	�ܸ����ȣ - ETX ����
	unsigned char TID[10];						//an	�ܸ��� ��ȣ "1234567890"
	unsigned char TransactionClass[2];			//an	�ŷ� ����
	unsigned char PacketClass[2];				//an	���� ����
	unsigned char TransactionType;				//an	�Է¸�ü �����ڵ�
	unsigned char TransactionCount[5];			//an	�ܸ���ŷ� �Ϸù�ȣ
	unsigned char PacketVersion[2];				//an	"T1"
	unsigned char Encryption;					//an	���� ��ȭȭ ����
	unsigned char RFU1[10];						//

	unsigned char FS1 ;							//bin	0x1C
	unsigned char Date[8];						//an	�ŷ� ����	YYYYMMDD
	unsigned char FS2 ;							//bin	0x1C
	unsigned char Time[6];						//an	�ŷ� �ð�	hhmmss
	unsigned char FS3 ;							//bin	0x1C
	unsigned char Amount[10];					//an	���αݾ�
	unsigned char FS4 ;							//bin	0x1C
	unsigned char Point[9];						//an	�߻� ����Ʈ
	unsigned char FS5 ;							//bin	0x1C
	unsigned char TotalPoint[9];				//an	���� ����Ʈ
	unsigned char FS6 ;							//bin	0x1C
	unsigned char UsablePoint[9];				//an	��밡�� ����Ʈ
	unsigned char FS7 ;							//bin	0x1C
	unsigned char PurchaseType;					//an	���� ����
	unsigned char FS8 ;							//bin	0x1C
	unsigned char EnterpriseID[10];				//an	����� ��ȣ
	unsigned char FS9 ;		
	unsigned char MerchantID[15];				//an	������ ID
	unsigned char FS10 ;		
	unsigned char EnterpriseName[32];			//an	������ �̸�
	unsigned char FS11 ;		
	unsigned char IssuerName[20];				//an	�߱޻� ��
	unsigned char FS12 ;		
	unsigned char AcquirerName[20];				//an	���Ի� ��
	unsigned char FS13 ;		
	unsigned char ApproveNum[10];				//an	���� ��ȣ
	unsigned char FS14 ;						//bin	0x1C
	unsigned char ResCode[4];					//an	���� �ڵ�
	unsigned char FS15 ;						//bin	0x1C
	unsigned char ResMessage[32];				//an	���� �޽���
	unsigned char FS16 ;						//bin	0x1C
	unsigned char AddedInfo[UBC_VAN_BASIC_RES_ADD_INFO_LEN];				//an	�ΰ� ����
	unsigned char etx;							//bin	ETX	
	unsigned char CRC[2]; 																	
	unsigned char eot; 		
	unsigned char LenMessage;
}__attribute__ ((packed))  NanoVANBasicRes, *PNanoVANBasicRes;
extern NanoVANBasicRes VAN_BasicRes;

#define NHN_BASIC_CNT	20
#define NHN_BASIC_GAP	4800
typedef struct _NHN_REG_{	
	LONG lbasic[NHN_BASIC_CNT];
	LONG ldata;
	unsigned long idcard_count; 
	unsigned long idcard_numer1, idcard_numer2 ; 
	unsigned long idcard_retry; 
} __attribute__ ((packed))  NHN_REG, *PNHN_REG;
extern NHN_REG nhn;

extern const char *g_NanoTransaction[11];

extern const char *g_NanoPacket[17];
extern int m_nCraditType;
extern int g_CreditAuth, g_CraditType, g_CraditTimeout, g_CraditComplete, g_CraditCancel;
extern unsigned long g_CraditAmount;
extern unsigned long m_nAmount, m_nNanoTotCnt;
extern int g_StatInfoCount;

extern unsigned char adjustment_time[7];
extern int adjustment_check;

extern void NHN_KbApprovalReq(int nType, unsigned long nAmount);

extern void UBC_OperatingInfoReq(int nType);
extern void UBC_SetStatusInfo(void);
extern void UBC_SetPrepaidPacket(int nTransactionType);
extern void UBC_KbApprovalReq(int nType, unsigned long nAmount);
extern void CNS_KbApprovalReq(int nType, unsigned long nAmount);
extern void UBC_CashApproval(int nType, unsigned long nAmount,  char *pNumber);
extern void UBC_Adjustment(void);

extern int VM_ReceivePacket(void) ;

extern int UBC_EbPurchase(LONG nTemp);
extern int UBC_EbRePurchase(LONG nTemp);
extern int UBC_TmBalance(unsigned long *nBalance);
extern int UBC_TmCancel(void);
extern int UBC_TmPurchase(LONG nTemp);
extern int UBC_TmRePurchase(LONG nTemp);
extern int UBC_ReadBalance(int nSort);
extern int UBC_MybiPurchase(LONG lFare);
extern int UBC_KbBalance(void);
extern void UBC_GetKbTrack2(unsigned char *pTrack2);
extern void UBC_TrackDataClear(void);
extern void UBC_TrackBackupDataClear(void);
extern int UBC_TmSetIssuerID(unsigned char *pData);

extern void VM_RegInit(void);
extern void VM_ExtInterrupt(void);
extern void UBC_Backup(void);
extern void UBC_TmsBackUpProcess(int nAmount);

extern unsigned char UBC_RcvTms(unsigned char *pData, int nLen);
extern int UBC_RcvVan(unsigned char *pData, int nLen);
extern unsigned short UBC_ReadTempVANBasicReq(int nNum);
extern void UBC_CreditBufferClear(void);
extern void UBC_NanoPop(void);
extern int UBC_LogCount(void);
extern int UBC_LogRead(unsigned char *pData, int rLen);
extern void UBC_LogInitial(void);
extern void UBC_LogClear(void);
extern int UBC_LogCheck(void);
extern int UBC_LogCheckIP(void);
extern void UBC_OperatingInfo(void);
extern void UBC_SetCreditAuth(int nAuth);
extern int UBC_GetCreditAuth(void);
extern void UBC_SetCreditComplete(int complete);
extern int UBC_GetCreditComplete(void);
extern void UBC_SaleInfoReq(PVM_DETAIL_INFO info);
extern void UBC_SetTmonyTrack2(unsigned char *pTrack2, unsigned char *pExpire);

extern void UBC_AditionalResponseData(word *tlv_len, unsigned char *tlv_data);
extern void UBC_IssuerAuthenticationData(word *tlv_len, unsigned char *tlv_data);
extern void UBC_IssuerScript(word *tlv_len, unsigned char *tlv_data);

extern void UBC_TestPreambleReq(int len, char * pdata);

extern void UBC_ResCodeClear(void);
extern int UBC_GetResCode(char* in_code);
extern int UBC_ResMessage(char *pMsg);
#endif

