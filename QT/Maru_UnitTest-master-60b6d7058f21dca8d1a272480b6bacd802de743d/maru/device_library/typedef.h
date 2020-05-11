/*
 * typedef.h
 *  Created on: 2012. 5. 9.
 *      Author: Administrator
 */

#ifndef TYPEDEF_H_
#define TYPEDEF_H_


#define KTC_ENABLE 			1

#define UBCN_IP_VER_1ST		'1'
#define UBCN_IP_VER_KTC 	'K'

#if (KTC_ENABLE == 1)
#define UBCN_IP_VER  	UBCN_IP_VER_KTC
#else
#define UBCN_IP_VER  	UBCN_IP_VER_1ST
#endif


#define GATE_OLD_VERSION	"BGO0"

///////////////////////////////////////////////////////////////////////////////////////////////////////
// 
#define transaction4_samsung_pay	0x03
#define transaction4_credit_cancel	0x04
#define transaction4_credit_inquiry	0x06

#define	CLOCK_TIME_100MS				10
#define	CLOCK_TIME_1S				100
#define	CLOCK_TIME_2S				200
#define	CLOCK_TIME_3S				300
#define	CLOCK_TIME_5S				500
#define	CLOCK_TIME_10S				1000
#define	CLOCK_TIME_15S				1500
#define	CLOCK_TIME_20S				2000
#define	CLOCK_TIME_30S				3000
#define	CLOCK_TIME_60S			6000
#define	CLOCK_TIME_100S			10000
#define	CLOCK_TIME_120S			12000
#define	CLOCK_TIME_180S			18000
#define	CLOCK_TIME_600S			60000
#define	CLOCK_TIME_700S			70000
#define	CLOCK_TIME_750S			75000

#define SIG_MSG_IC				0x01
#define SIG_MSG_SELF_VERIFY		0x02
#define SIG_MSG_OTHER_CARD		0x03
#define SIG_MSG_OUT_CARD		0x04
#define SIG_MSG_OUT_SUPPORT_CARD		0x05
#define SIG_MSG_FALLBACK		0x07
#define SIG_MSG_ARQC		0x08
#define SIG_MSG_WAITING		0x09
#define SIG_MSG_SELF_VERIFY_FAILURE 0x10
#define SIG_MSG_SERIAL			0x11
#define SIG_MSG_SERIAL_READY		0x12
#define SIG_MSG_AAC		0x13
#define SIG_MSG_CANTO_NECTA_COMPLETE 0x14
#define SIG_MSG_CANTO_NECTA_ERROR 0x15
#define SIG_MSG_CANTO_NECTA_PROGRESS 0x16
#define SIG_MSG_CANTO_NECTA_COLUMN_SELECT 0x17
#define SIG_MSG_CANTO_TMONEY_CARD_ERROR 0x18
#define SIG_MSG_CANTO_ANTICOLLISION 0x19
#define SIG_MSG_DOOR_OPEN	 0x20
#define SIG_MSG_REPAIR	 0x21
#define SIG_MSG_TMS	0x22
#define SIG_MSG_NOT_FUNCTION		0x23
#define SIG_MSG_CASH_ONLY	0x24
#define SIG_MSG_MODEM_CONNECT	0x25
#define SIG_MSG_SAECO_FAILURE		0x26

#define SIG_MSG_MAIN		0x8000

#define COIN_RECEIPT_INPUT 		      0
#define CASH_RECEIPT_NONE			'N'
#define CASH_RECEIPT_INCLUDE		'O'

#define CASH_ONLY_NONE			'N'
#define CASH_ONLY_INCLUDE		'O'

#define APPROVAL_REJECT_NONE			'N'
#define APPROVAL_REJECT_INCLUDE		'O'

#define KTC_NONE					'N'
#define KTC_KOCES_INCLUDE			'K'


#define VM_TYPE_ROVEN				1
#define VM_TYPE_LOTTE_MULTI		2
#define VM_TYPE_LOTTE_LVC440		3
#define VM_TYPE_YOUNG_GATE_MULTI		4
#define VM_TYPE_YOUNG_GATE_COCA		5
#define VM_TYPE_ROBOQ				10

#define VM_BLAND_UBCN						'U'
#define VM_BLAND_LOTTE_GATEBANK			'G'	// LVP-560 SMART 자판기전
#define VM_BLAND_LOTTE_GATEBANK_MULTI	'M'	// LVM-480
#define VM_BLAND_MAINTECH_GATEBANK		'B'	// LVP-560

#define VM_LOTTE_COFFEE_2140	"2140"
#define VM_LOTTE_COFFEE_3112	"3112"
#define VM_LOTTE_COFFEE_3142	"3142"
#define VM_LOTTE_COFFEE_6112	"6112"
#define VM_LOTTE_COFFEE_6131	"6131"
#define VM_LOTTE_COFFEE_6132	"6132"
#define VM_LOTTE_COFFEE_6140	"6140"
#define VM_LOTTE_COFFEE_6143	"6143"
#define VM_LOTTE_COFFEE_6450	"6450"
#define VM_LOTTE_COFFEE_6460	"6460"


#define VM_LOTTE_442			"442"
#define VM_LOTTE_522			"522"
#define VM_LOTTE_531			"531"
#define VM_LOTTE_480			"480"
#define VM_LOTTE_482			"482"
#define VM_ROVEN_5024			"5024"
#define VM_ROVEN_5224			"5224"
#define VM_ROVEN_5720			"5720"

#define VM_YONG_SYSTEM_COCA	"YOU"

#define VM_TID						"2000000574"	

#define LOTTE_NAVER_TEST_MODE 	0

#define MODEM_TYPE_SIERRA			0
#define MODEM_TYPE_CNSLINE  		1
#define MODEM_TYPE_S4_ETH  		2
#define MODEM_TYPE_SIERRA_KT		3
#define MODEM_TYPE_OFFLINE		4
#define MODEM_TYPE_ATM_M700_KT	5
#define MODEM_TYPE_WIZ100SR		6
#define MODEM_TYPE_MAX		      (MODEM_TYPE_WIZ100SR+1)

#define IC_TYPE_ALCOR				0
#define IC_TYPE_TERIDIAN			1
#define IC_TYPE_MAX					(IC_TYPE_TERIDIAN+1)

#define CREDIT_VISA_ONLY		'V'
#define CREDIT_EMV_ALL			'A'

#define	SongMode_1			0x31
#define	RamenMode_1			0x32
#define	GameMode_1			0x33
#define	EnterSolutionMode			0x34
#define	RamenDoctorMode_1			0x35
#define PohasMode			0x36
#define MireFlowerMode_1				0x37
#define KlcMode_1			0x38
#define PCROOM_1			0x39
#define RamenDoctorMode_2			0x3A
#define DongGu_1			0x3B
#define RamenMode_2			0x3C
#define MdbWcmNectaMode			0x3D
#define KorailLotteMode_1	0x3E
#define KorailLotteGate560	0x3F
#define NewWorldVendingMode	0x40
#define HappyNameMode			0x41
#define CafeTodayMode			0x42
#define MdbDonghakMode			0x43
#define MireFlowerMode_2			0x44
#define Emart24GateMode				0x45
#define MdbNespressoMode			0x46
#define KlcMode_2			0x47
#define BusRideMode			0x48
#define JuneSystemMode			0x49
#define MdbMireVendigMode			0x4A
#define VisaOnlyMode				0x4B
#define AbkoEthMode 	0x4C
#define KorailLotteGate560S	0x4D	// SMART
#define KorailLotteGate480M	0x4E	// MULTI
#define KlcMode_Serial			0x4F
#define MinistopLotteGate560		0x50
#define MinistopLotteGate480M	0x51	// MULTI
#define CocaUbcnMode	0x52
#define SunghyunEthMode 0x53
#define MdbOhJinMode			0x54
#define SmcKoreaEthMode	0x55
#define LoHasEthMode 	0x56
#define SlockerEthMode 	0x57
#define AgdfncMode   0x58
#define RamenMenMode_1	0x59
#define GkVendingsMode		0x5A
#define NewWorldVendingEthMode		0x5B
#define SaecoYoungSysMode 0x5C
#define SaecoCoinEmart24Mode 0x5D
#define MdbSandenMode 0x5E
#define	Ramen2SofoodMode_1			0x5F
#define	Ramen2SofoodMode_2			0x60
#define	ShilaxMode_1			0x61
#define SaecoMode 	0x62
#define SevenElevenGateEthMode 0x63
#define CardInsertMode 0x64
#define MdbLottiMode			0x65
#define SaecoCoinMode_1 	0x66
#define MdbNestleMode 	0x67
#define RovenVts2Mode		0x68
#define CoCaGateCP24	0x69	// CP24
#define SaecoCoinMode_2 	0x6A
#define Lab101GateMode				0x6B
#define UbcnEthMode 	0x6C
#define KolonEmployeeMode			0x6D
#define LockerFreeMode			0x6E
#define PhoenixEbayMode			0x6F
#define Gs25GateMode				0x70

typedef enum brand_emm {
	COMPANY_COIN=0,
	COMPANY_DONGA,
	COMPANY_PHOENIX,
	COMPANY_LOTTE,
	COMPANY_COCA,
	COMPANY_KORAIL,
	COMPANY_DONGHAK,
	COMPANY_KC_CERTI,
	COMPANY_UBCN,
	COMPANY_LOCKER,
	COMPANY_EDWORDS,
	COMPANY_MINIGOLD,
	COMPANY_EMART24,
	COMPANY_KORAIL_TRIP,
	COMPANY_WELSTORY,
	COMPANY_EMART24_KTNG,
	COMPANY_KTNG,
	COMPANY_MINISTOP,
	COMPANY_SEOUL_MILK,
	COMPANY_KSCC,
	COMPANY_MINISTOP_KTNG,
	COMPANY_EMART_EVERYDAY_KTNG,
	COMPANY_KOLON,
	COMPANY_LOCKER_FREE,
	COMPANY_EBAY,
	COMPANY_MAX,
} brand_emm_num; 

//#define COMPANY_KC_CERTI		0x80
#define COMPANY_TM_CERTI		0x80

#define LCD_LINE_COIN	 		"코인자판기"
#define LCD_LINE_DONGA	 		"동아오츠카"
#define LCD_LINE_PVS	 		"휘닉스벤딩"  	
#define LCD_LINE_LOTTE	 		"LOTTE"
#define LCD_LINE_COCA	 		"코카콜라"
#define LCD_LINE_KORAIL	 		"코레일유통"
#define LCD_LINE_DONGHAK 		"동학식품"
#define LCD_LINE_TMONEY 		"인증"
#define LCD_LINE_UBCN	 		"UBCN"
#define LCD_LINE_LOCKER 		"자동판매기"
#define LCD_LINE_EDWARDS 		"에드워드코리아"
#define LCD_LINE_MINIGILD 		"롯데미니골드"
#define LCD_LINE_WITHME 		"emart24"
#define LCD_LINE_KORAIL_TRAVAL	"코레일관광개발"
#define LCD_LINE_WELSTORY	 	"웰스토리"
#define LCD_LINE_EMART24_KTNG	 "emart24 담배"
#define LCD_LINE_KTNG	 		"KTnG"
#define LCD_LINE_GMART			"Gmart"
#define LCD_LINE_MINISTOP		"미니스톱"배
#define LCD_LINE_SEOUL_MILK	"서울우유"
#define LCD_LINE_KSCC			"한국스마트카드"
#define LCD_LINE_MINISTOP_KTNG  "미니스톱담"
#define LCD_LINE_EMART_EVERDAY	"emart everyday"
#define LCD_LINE_KOLON			"KOLON"
#define LCD_LINE_LOCKER_FREE			"FREE"
#define LCD_LINE_EBAY			"ebay"  // "ebay" "EA Korea"	
#define LCD_LINE_GS25 			"GS25"

#define LCD_LINE_7_ELEVEN		"7-ELEVEN"
#define LCD_LINE_SHILAX			"LG"
#define LCD_LINE_SANDEN		"SANDEN"
#define LCD_LINE_SAECO			"Saeco"
#define LCD_LINE_NEWWORLD 	"뉴월드벤딩시스템"
#define LCD_LINE_HAPPYNAME 	"해피네임"
#define LCD_LINE_CAFETODAY 	"카페투데이"
#define LCD_LINE_TOUCHCAFE 	"TOUCH CAFE"
#define LCD_LINE_NESPRESSO		"NESPRESSO"
#define LCD_LINE_RIDE			"uforus"
#define LCD_LINE_JUNE			"JUNE SYSTEMS"
#define LCD_LINE_ENTERSYS		"ENTER SOLUTION"
#define LCD_LINE_MIRE			"미래자판기연구소"
#define LCD_LINE_ABCO			"(주)앱코"
#define LCD_LINE_GPS			"지피에스"
#define LCD_LINE_SUNGHYUN		"오름독서실"
#define LCD_LINE_OHJIN	 		"오진양행"
#define LCD_LINE_SMCKOREA	 	"SMC KOREA"
#define LCD_LINE_LOHAS		 	"로하스"
#define LCD_LINE_S_LOCKER		"새누라커"
#define LCD_LINE_AGD_FNC		"에프엔씨"
#define LCD_LINE_GK_VEND		"GK 벤딩스"
#define LCD_LINE_CARD_INSERT    "카드투입"
#define LCD_LINE_LOTTI	 		"U SYSTEMS"
#define LCD_LINE_NESTLE	 		"NESTLE"
#define LCD_LINE_LAB101 		"LAB-101"

#define KTC_FK_PATH 			"/app/data/koces_fk_info.dat"
#define KTC_FK_BAK_PATH 		"/app/data/koces_fk_info.dat.bak"
#define KTC_DUKPT_PATH 		"/app/data/koces_dukpt_info.dat"
#define KTC_MARU_PATH    		"/app/data/hash_maru.dat"
#define KEY_LIBDCLI_PATH    		"/app/data/hash_libdcli.dat"
#define KEY_LIBDCRYPT_PATH    	"/app/data/hash_libdcrypt.dat"
#define KEY_LIBDKOCES_PATH    	"/app/data/hash_libdkoces.dat"

#define MSR_READ_COUNT_KORAIL		18000
#define MSR_READ_COUNT_MAX		50000

#define CMD_TTYS_INT			0x1000
#define CMD_TTYS_FIFO			0x1001
#define CMD_TTYS_RD			0x1002
#define CMD_TTYS_BAUD			0x1003
#define CMD_TTYS_PARITY		0x1004
#define CMD_TTYS_STOP			0x1005


#define TTYS_FIFO_1				1
#define TTYS_FIFO_4				4
#define TTYS_FIFO_8				8
#define TTYS_FIFO_12				12

#define B2S(X1,X2)				(X1 | (X2<<8))
#define B2I(X1,X2,X3,X4)		(X1 | (X2<<8) | (X3<<16) | (X4<<24))


///////////////////////////////////////////////////////////////////////
// 
#define EB_SW_9401				0x0194		// 
#define EB_SW_9402				0x0294		// 


///////////////////////////////////////////////////////////////////////
// SAM Key
#define SAM_RealKey				'1'
#define SAM_FakeKey				'0'
#define SAM_NotDefineKey		0xFF


#define PSAM_DG					1
#define PSAM_MYbi				2
#define PSAM_KB					3
#define PSAM_TMONEY			4
#define PSAM_CNC				5
#define PSAM_KCASH				6
#define PSAM_EB					7
#define PSAM_EBM				8
#define PSAM_USAM				9
#define PSAM_HIPASS				10
#define PSAM_HANPAY			11
#define PSAM_BDO				12
#define PSAM_LOTTE_TOT			13
#define PSAM_RAIL_PLUS			14

///////////////////////////////////////////////////////////////////////
//
#define CARD_NotDefine			0xff

#define RF_MybiCard			 	0x00
#define RF_HanaroCard		 	0x02


#define FUN_TRUE				0x01
#define FUN_FALSE				0x00
#define FUN_APDU_ERR			0x02

#define RC_OK					'P'
#define RC_FAIL					'N'

// Return codes
#define RC_SUCCESS				0x00
#define RC_ValidBlock				0x00
#define RC_DATA					0x01
#define RC_PacketError 			0x01	
#define RC_NoCard				0x02	
#define RC_POLL_A				0x02
#define RC_NOT_SUPPORT			0x03
#define RC_InvaildCard 			0x03		
#define RC_MoreCard				0x04	
#define RC_CONTINUE				0x05


#define RC_NO_SEND				0x20
#define RC_TCP_OPEN_FAIL		0x21




#define RC_BLOCK5_ERROR		0x30
#define RC_FLASH				0x30 // Internal use only. do not return this code
#define RC_MFRC5xxError			0x30
#define RC_FLASH_DATA			0x31 // Internal use only. do not return this code

#define RC_CardAccessError		0x32
#define	RC_FAIL32				0x32
#define RC_CARD_AGAIN1			0x33
#define RC_RESTORE_ERR			0x34
#define RC_CANCEL_ERR			0x35
#define RC_RFCardDataReadError	0x35	
#define RC_RFCardDataWriteError	0x36	
#define RC_PayCompleteCard		0x37	
#define RC_PurchaseError			0x38	
#define RC_UnderValue			0x39

#define RC_MIFARE				0x40
#define RC_FlashInitializeError		0x40
#define RC_FlashWriteError		0x41
#define RC_FlashMemoryShortage	0x42	
#define	RC_FAIL42				0x42
#define RC_NoBackupData			0x43

#define RC_SamInitializeError		0x50
#define RC_SamCommError		0x51	
#define	RC_FAIL52				0x52


#define RC_ABTYPE				0x80

#define RC_FIND_CARD			0xD1
#define RC_CARD_SEL				0xD2
#define RC_STARTBIT_ERROR		0xD2
#define RC_RF_APDU				0xD3
#define RC_SAM_ON				0xD4
#define RC_SAM_OFF				0xD5
#define RC_SAM_APDU			0xD6
#define RC_MSG					0xB3

#define RC_CANCEL				0xCD
#define RC_CAT_TIME_ERROR		0xCE

#define RC_Timeout				0xB2


#define RC_INVALID_CARD			0xE1
#define RC_INVALID_PAR_LEN		0xE2		
#define RC_SAM_OPEN_FAILURE	0xE4		
#define RC_SAM_AUTH_FAILURE	0xE5		
#define RC_AUTH_ERROR			0xE6	
#define RC_OVERLAP_TRANS		0xE7	
#define RC_REQ_CANCEL			0xE8	
#define RC_REQ_REPURCHASE		0xE9	
#define RC_READ_BALANCE		0xEA		
#define RC_INVALID_CONDITION	0xEB		
#define RC_REQ_COLLECT			0xEC
#define RC_REQ_LOG				0xED
#define RC_SIGN_FAILURE 		      0xEE
#define RC_NO_BANK_DATA 		0xEF

#define RC_SEQ_ERROR			0xF0
#define RC_EXIT					0xF0		// 
#define RC_CARD_AGAIN			0xF1 // Internal use only. do not return this code
#define RC_FAILURE_CRYPTO		0xF1
#define	RC_RECOVERY1			0xF1
#define RC_NO_CARD				0xF2
#define	RC_RECOVERY2			0xF2
#define RC_MORE_CARDS			0xF3
#define RC_CHANG_CARD			0xF4
#define RC_SAM_CREDIT_ERROR	0xF4
#define RC_INVALID_CMD			0xF5
#define RC_SAM_ERROR			0xF6
#define RC_CRC_ERROR			0xF7
#define RC_CARD_ERROR			0xF8
#define RC_BAL_ERROR			0xF9
#define RC_LOG_OVERFLOW		0xFA
#define RC_WRONG_PIN			0xFA
#define RC_LOG_NOTHING			0xFB
#define RC_PIN_BLOCKED			0XFB
#define RC_BAL_OVER_ERR			0xFC
#define RC_BAL_OVER				0xFC
#define RC_SET_STARTBIT			0XFC
#define RC_TIME_ERROR			0xFD
#define RC_RETRY_ERROR			0xFE
#define RC_BEEP					0xFE
#define RC_FAILURE				0xFF
#define RC_BrokenBlock			0xFF



typedef unsigned char   uchar;
typedef unsigned int   uint;

typedef unsigned char	 BYTE;
typedef unsigned char	 byte;
typedef unsigned char	 UCHAR;
typedef unsigned char* PUCHAR;

typedef unsigned long	ulong;
typedef unsigned long	ULONG;
typedef unsigned long 	DWORD;
typedef unsigned long   dword;

typedef char			CHAR;
typedef char*			PCHAR;

typedef unsigned int	UINT;
typedef unsigned int*	PUINT;

typedef int				INT;
typedef int             BOOL;
typedef int*			PINT;

typedef double			FLOAT;

typedef void			VOID;


typedef unsigned short  word;
typedef unsigned short  WORD;



typedef union {
	unsigned long value;
	BYTE data[4];
} LONG;

typedef union {
	unsigned short value;
	BYTE data[2];
} SHORT;


typedef struct _DEVICE_INFO_ {
	char version[2][32];
	char date_time[64];
	char company[64];
	int power;
	int test_mode;
	int calibration;
	int lcd_type;
	int credit_type;
	int tms_mode;
	int ktc_stauts;
	int ic_status;
	int ic_pause;
	int msg_signal;
	int vertical_error;
	int horizontal_error;
	int guard_error;
	int take_error;
	int c100_error;
	int c500_error;
	int bill_door_error;
	char antenna[4];
} DEVICE_INFO;
extern DEVICE_INFO devinfo;

typedef struct _GLOBAL_INFO
{		
	unsigned char ClrTime[16];

	unsigned char TYPE[64];			// 	
	unsigned char rfu2[64];
	
	int Modem;
	int Company;
	char *pCmpanyName;
	
	int VmType;
	char VmModel[32];		

	char VmUbcn;
	unsigned long DisplayMode;
	unsigned char IC;
	unsigned char VISA;

	char Phone[16];
	char Printer;
} GLOBAL_INFO;
extern GLOBAL_INFO g_GlobalInfo, *g_GlobalpInfo;



#endif /* TYPEDEF_H_ */

