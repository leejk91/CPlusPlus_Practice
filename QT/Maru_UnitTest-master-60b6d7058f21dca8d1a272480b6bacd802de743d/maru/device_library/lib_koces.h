#ifndef _KOCES_DUKPT_H_
#define _KOCES_DUKPT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "auth_koces.h"

/*  공통 header 부분 65 byte*/
typedef struct 
{
   char ApprVer      [ 2]; /* 전문버전 */
   char SvcType      [ 2]; /* 서비스종류 서비스전문표 참조 */
   char TrdType      [ 2]; /* 업무 구분, 업무구분표 참조 */
   char SndType      [ 1]; /* 전송구분, 'S': PG->VAN, 'R': VAN->PG */
   char TermID       [10]; /* 단말기번호 터미널아이디 */
   char TrdDate      [14]; /* 거래일시 YYYYMMDDhhmmss */
   char TrdNo        [10]; /* 거래일련번호 (응답시 반환) */
   char MchData      [20]; /* 가맹점 데이타 (응답시 반환) */
   char AnsCode      [ 4]; /* 응답코드 (요청시는 스페이스) */
  
}  PGFComHead ;

/* 키갱신 요청 */
typedef struct
{
	PGFComHead Header;	/* 공통 헤더 부분 */

	char TmlcNo       [32];		/* 단말인증번호 */
	char VefyReq      [256];	/* 검증 요청 데이터 */	/* 데이터 HEXString */
	char Filler       [50];		/* 여유필드 */
	char cCR          [ 1];		/* CR 0x0d */

}VerifyReqAppr;

/* 키갱신 응답 */
typedef struct
{
	PGFComHead Header;	/* 공통 헤더 부분 */

	char TmlcNo       [32];		/* 단말인증번호 */
	char VefyRes      [96];		/* 검증 응답 데이터 */	/* 데이터 HEXString */
	char Message      [32];		/* 응답 메시지 */
	char Filler       [50];		/* 여유필드 */
	char cCR          [ 1]; /* CR 0x0d */

}VerifyResAppr;


typedef struct 
{		
	unsigned char key_injection;
	
	unsigned char hdata_used;
	unsigned char serial_number[20];
	unsigned char hdata[1024+16];
	unsigned char enc_hash[32];
	size_t enc_hash_len;

	unsigned char ipek[AP_KEY_SIZE];
			
} s_koces_dukpt_info;
extern s_koces_dukpt_info koces_dukpt_info;


typedef struct 
{			
    KOCES_FKINFO fki;
	unsigned char enc_hash[32];
	size_t enc_hash_len;
	
	unsigned char ksn[10];		
	unsigned long ksn_counter;
	unsigned char ksn_1st;
	
	unsigned char time[20];
			
} s_koces_fk_info;
extern s_koces_fk_info koces_fk_info;

typedef struct {
		unsigned char stx;
		unsigned char line_type;
		unsigned char transaction_preamble[4];
		unsigned char terminal_id[10];
		unsigned char certification_number[32];
		unsigned char transaction_date[14];
		unsigned char response_code[4];
		unsigned char response_msg[40];
		unsigned char vefify_response_code[48];
		unsigned char rev[1024];
} __attribute__ ((packed))  koces_download_crypto;

typedef struct _KOCES_DUPKT_REQ_ {
	//unsigned char preamble_num[4];			// an	 거래전문번호  : 2820 : 신용 승인
	//unsigned char cat_id[10];					// an 	단말기 TID
	unsigned char auth_num[32];				// an	단말인증번호
	unsigned char date_time[14];				// an 	거래일시 : YYYYMMDDhhmmss	
	unsigned char data[256];					// an	검증 요청 데이터
} __attribute__ ((packed))  KocesDupktReq, *PKocesDupktReq;
extern KocesDupktReq koces_dupkt_req;

typedef struct _KOCES_DUPKT_RES_ {
	//unsigned char auth_num[32];				// an	단말인증번호
	//unsigned char date_time[14];				// an 	거래일시 : YYYYMMDDhhmmss
	unsigned char data[96];					// an	검증 응답 데이터
} __attribute__ ((packed))  KocesDupktRes, *PKocesDupktRes;
extern KocesDupktRes koces_dupkt_res;

typedef struct _KOCES_CASH_REQ_ {
	//unsigned char preamble_num[4];			// an	 거래전문번호  : 2820 : 신용 승인
	//unsigned char cat_id[10];					// an 	단말기 TID
	unsigned char auth_num[32];				// an	단말인증번호
	unsigned char date_time[14];				// an 	거래일시 : YYYYMMDDhhmmss	
	unsigned char ksn[20];					// an	KSN : Special -> Alpha Numeric
	unsigned char crypto_track2[96];			// an	암호화된 Track II : Special -> Alpha Numeric
} __attribute__ ((packed))  KocesCashReq, *PKocesCashReq;
// 변수 앞의 _는 원거래 정보
extern KocesCashReq koces_cash_req;


typedef struct _KOCES_CASH_RES_ {
	//unsigned char preamble_num[4];			//an		거래전문번호  : 2515 : 신용 승인
	//unsigned char cat_id[10];					// an 	단말기 TID
	unsigned char auth_num[32];				// an	단말인증번호
	unsigned char date_time[14];				// an 	거래일시 : YYYYMMDDhhmmss
	unsigned char expire_date[2];				// n		암호키 만료일까지 남은 일수 ( 30일 전부터 전송 )
	unsigned char id_validate_num[40];			// a		출력시 사용될 신분확인번호 정보 ( 국세청 고시사항 기준으로 마스킹된 내역 전송 됨)
} __attribute__ ((packed))  KocesCashRes, *PKocesCashRes;
// 변수 앞의 _는 원거래 정보
extern KocesCashRes koces_cash_res;


typedef struct _KOCES_MSR_REQ_ {
	//unsigned char preamble_num[4];			// an	 거래전문번호  : 2410 : 신용 승인
	//unsigned char cat_id[10];					// an 	단말기 TID
	unsigned char auth_num[32];				// an	단말인증번호
	unsigned char date_time[14];				// an 	거래일시 : YYYYMMDDhhmmss
	unsigned char ksn[20];					// an	KSN : Special -> Alpha Numeric
	unsigned char crypto_track2[96];			// an	암호화된 Track II : Special -> Alpha Numeric
	unsigned char working_key_index[2];		// a		전자서명 또는 비밀번호 사용 거래  외 0 으로 SET
	unsigned char password[16];				// a		비밀번호 입력이 필요한 경우만 사용 (DES암호화)
} __attribute__ ((packed))  KocesMSRReq, *PKocesMSRReq;
// 변수 앞의 _는 원거래 정보
extern KocesMSRReq koces_msr_req;


typedef struct _KOCES_MSR_RES_ {
	//unsigned char preamble_num[4];			//an		거래전문번호  : 2415 : 신용 승인
	//unsigned char cat_id[10];					// an 	단말기 TID
	unsigned char auth_num[32];				// an	단말인증번호
	unsigned char date_time[14];				// an 	거래일시 : YYYYMMDDhhmmss
	unsigned char expire_date[2];				// n		암호키 만료일까지 남은 일수 ( 30일 전부터 전송 )
	unsigned char working_key_index[2];		// a		전자서명 또는 비밀번호 사용 거래  외 0 으로 SET
	unsigned char working_key[16];			// a 
} __attribute__ ((packed))  KocesMSRRes, *PKocesMSRRes;
// 변수 앞의 _는 원거래 정보
extern KocesMSRRes koces_msr_res;




typedef struct _KOCES_IC_REQ_ {
	//unsigned char preamble_num[4];			//an		거래전문번호  : 2410 : 신용 승인
	//unsigned char cat_id[10];					// an 	단말기 TID
	unsigned char auth_num[32];				// an	단말인증번호
	unsigned char date_time[14];				// an 	거래일시 : YYYYMMDDhhmmss
	unsigned char ksn[20];					// an	KSN : Special -> Alpha Numeric
	unsigned char crypto_track2[96];			// an	암호화된 Track II : Special -> Alpha Numeric
	unsigned char working_key_index[2];		// a		전자서명 또는 비밀번호 사용 거래  외 0 으로 SET
	unsigned char password[16];				// a		비밀번호 입력이 필요한 경우만 사용 (DES암호화)
	unsigned char emv_transaction_type;		// a 	EMV거래구분, 
	unsigned char pos_entry_mode_code[4];		// an      Fall back : 4BCD -> Alpha Numeric
	unsigned char card_sequence_number[4];	// n		3BCD -> Alpha Numeric, card_sequence_number
	unsigned char additional_pos_information[12];	//    1B+up to 10BCD-> Alpha Numeric
	unsigned char DE_9F5B[46];				//  an 	Tag(9F5B)+Len(1)+Data(v20), issuer_script_results
	unsigned char DE_9F26[22];				//  an	Tag(9F26)+Len(1)+Data(8) , Cryptogram(ARQC)
	unsigned char DE_9F27[8]; 				// an 	Tag(9F27)+Len(1)+Data(1), cryptogram_information_data
	unsigned char DE_9F10[70];				// an 	Tag(9F10)+Len(1)+Data(v32), Visa Discretionary Data, issuer_applicaton_data
	unsigned char DE_9F37[14];				// an	Tag(9F37)+Len(1)+Data(4)  , Unpredictable Number
	unsigned char DE_9F36[10]; 				// an 	Tag(9F36)+Len(1)+Data(2)  , Application transaction counter
	unsigned char DE_95[14];					// an       Tag(95)  +Len(1)+Data(5)    , Terminal verification results
	unsigned char DE_9A[10];					// an	Tag(9A)  +Len(1)+Data(3)    , Terminal Transaction date
	unsigned char DE_9C[6];					// an	Tag(9C)  +Len(1)+Data(1)    , Cryptogram Transaction Type
	unsigned char DE_9F02[18];				// an	Tag(9F02)+Len(1)+Data(6)  , Cryptogram Amount
	unsigned char DE_5F2A[10]; 				// an	Tag(5F2A)+Len(1)+Data(2)  , Cryptogram Currency Code
	unsigned char DE_82[8];					// an	Tag(82)  +Len(1)+Data(2)    , Application interchange Profile
	unsigned char DE_9F1A[10];				// an 	Tag(9F1A)+Len(1)+Data(2)  , Terminal Country Code
	unsigned char DE_9F03[18];				// an 	Tag(9F03)+Len(1)+Data(6)  , Cryptogram Cashback, amount_other
	unsigned char DE_9F34[12];				// an 	Tag(9F34)+Len(1)+Data(3)  , Cardholder Verification Method(CVM) Results
	unsigned char DE_9F33[12];				// an	Tag(9F33)+Len(1)+Data(3)  , Terminal Capability Profile
	unsigned char DE_9F35[8];					// an 	Tag(9F35)+Len(1)+Data(1) ,  terminal_type
	unsigned char DE_9F1E[22];				// an	Tag(9F1E)+Len(1)+Data(8)  , Terminal Serial Number, Interface Device(IFD) Serial Number
	unsigned char DE_9F53[8];					// an 	Tag(9F53)+Len(1)+Data(1) ,  Transaction Category Code
	unsigned char DE_84[36];					// an 	Tag(84)  +Len(1)+Data(v16), Dedicated File Name
	unsigned char DE_9F09[10];				// an 	Tag(9F09)+Len(1)+Data(2) , Terminal ApplicationVersion Number
	unsigned char DE_9F41[14];				// an 	Tag(9F41)+Len(1)+Data(v4) , Transaction Sequence Counter
	unsigned char fallback_reason[2];			// 	
} __attribute__ ((packed))  KocesICReq, *PKocesICReq;
// 변수 앞의 _는 원거래 정보
extern KocesICReq koces_ic_req;


typedef struct _KOCES_IC_RES_ {
	unsigned char auth_num[2];				// an	
	unsigned char track2[40];				// an 	거래일시 : YYYYMMDDhhmmss
	unsigned char working_key_index[2];		// a		전자서명 또는 비밀번호 사용 거래  외 0 으로 SET
	unsigned char working_key[16];			// a 
	unsigned char ac2nd;					// an	
	unsigned char ard[52];
	unsigned char iad[24];
	unsigned char issuer_script[520];
} __attribute__ ((packed))  KocesICRes, *PKocesICRes;
// 변수 앞의 _는 원거래 정보
extern KocesICRes koces_ic_res;


#define UBCN_DUKPT_INIT	 	'I'
#define UBCN_DUKPT_CANCEL 	'X'
#define UBCN_DUKPT_SUCCESS 	'O'

extern int ktc_comp_result;

void Debug_Test(void);

void Dbg_Printf(const char *fmt,...);
void PrintHex(unsigned char *nAddr, unsigned long nLen, char *sFrom, ...);
void HEX_Dump(unsigned char *nAddr, unsigned long nLen, char *sFrom, ...);

extern int Debug_Open(void);
void Debug_Close(void);
extern void koces_init(void);

extern int koces_init_dupkt_injection(unsigned char *pdata, int recvlen);

extern void koces_dupkt_request(void);
extern void koces_dupkt_response(unsigned char *recv_token_sc, size_t recv_token_sc_len);

extern void koces_cash_request(int nType, unsigned long nAmount, char *pNumber);
extern void koces_cash_response(unsigned char *recvbuf, size_t recvlen);

extern void koces_credit_request(int nType, unsigned long nAmount);
extern void koces_credit_response(unsigned char *recvbuf, size_t recvlen);

extern void koces_ic_request(int nType, unsigned long nAmount);
extern unsigned char koces_ic_response(unsigned char *recvbuf, size_t recvlen);

extern unsigned long koces_get_ksn(unsigned char *ksn);
extern unsigned long koces_get_ksn_counter(void);

extern int koces_fk_make_self_protect(void);
extern int koces_bin_make_self_protect(void);
extern int koces_libdcli_make_self_protect(void);
extern int koces_libdcrypt_make_self_protect(void);
extern int koces_libdkoces_make_self_protect(void);
extern int koces_library_make_self_protect(void);

extern int koces_hdata_comp_self_protect(void);
extern int koces_fk_comp_self_protect(void);
extern int koces_bin_comp_self_protect(void);
extern int koces_libdcli_comp_self_protect(void);
extern int koces_libdcrypt_comp_self_protect(void);
extern int koces_libdkoces_comp_self_protect(void);
extern int koces_library_comp_self_protect(void);

extern void koces_key_task(void);
extern void koces_self_verification(void);

extern void koces_ic_request(int nType, unsigned long nAmount);
extern unsigned char koces_ic_response(unsigned char *recvbuf, size_t recvlen);

#if 0
extern unsigned char koces_get_hdata_used(void);
extern int koces_hdata_removal(void);
extern int koces_hdata_insert_ack(void);
extern int koces_hdata_insert_request(char *serial_number);
extern int koces_hdata_insert_response(unsigned char *pdata, int recvlen);


extern int koces_mutual_authentication(void);
extern void koces_dupkt_request(void);
extern unsigned char koces_dupkt_status(void);
extern int koces_dupkt_credit_preamble(int tranasction_type, int key_in, char *track2, int track2_len, unsigned long amount);

extern void koces_cash_request(int nType, unsigned long nAmount, char *pNumber);
extern void koces_ic_request(int nType, unsigned long nAmount);
extern void koces_credit_request(int nType, unsigned long nAmount);
extern void koces_fallback_request(int nType, unsigned long nAmount, unsigned char nReason);
extern void koces_cash_response(unsigned char *recvbuf, size_t recvlen);
extern unsigned char koces_ic_response(unsigned char *recvbuf, size_t recvlen);
extern void koces_credit_response(unsigned char *recvbuf, size_t recvlen);
extern unsigned long koces_get_ksn(unsigned char *ksn);
extern unsigned long koces_get_ksn_counter(void);
extern char *koces_get_dupkt_renewal_time(void);

extern int koces_set_newzen_hash(void);
extern int koces_newzen_verification(void);

extern int koces_libdcli_verification(void);
extern int koces_libdcrypt_verification(void);
extern int koces_libdkoces_verification(void);

extern int koces_hdata_comp_self_protect(void);
extern int koces_fk_comp_self_protect(void);
extern int koces_fk_make_self_protect(void);

extern unsigned char koces_2nd_ac_check(void);
#endif
#ifdef __cplusplus
}
#endif

#endif


