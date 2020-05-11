/*
 * emvtypedef.h
 *  Created on: 2012. 5. 9.
 *      Author: Administrator
 */

#ifndef EMVTYPEDEF_H_
#define EMVTYPEDEF_H_

#define NEC_EXT
#define MAX_CAP         101
#define MAX_CAPK 		41
#define MAX_REVO 		90
#define MAX_EXCEPT 		1

#define NETWORK_LIMIT   128


/* revocation list */
typedef struct {
    byte RID[5];        /* RID */
    byte index;         /* CA PK Index */
    byte Cert_SN[3];    /* Certificate Serial Number */
} REVOCATION_LIST;

/* RSA key lengths. */
#define MAX_RSA_MODULUS_BITS 2048
#define MAX_RSA_MODULUS_LEN ((MAX_RSA_MODULUS_BITS + 7) / 8)

/* RSA public key. */
typedef struct {
//111224
  byte f_revo;                         /* revocation 여부 flag - 0x31 : 사용, 0x32: revocated */
  byte RID[5];                          /* RID */
  byte index;                           /* index */
  word bits;                            /* length in bits of modulus */
  byte modulus[MAX_RSA_MODULUS_LEN];    /* modulus */
  byte exponent[MAX_RSA_MODULUS_LEN];   /* public exponent */
} R_RSA_PUBLIC_KEY;

/* exception file */
typedef struct {
    byte PAN[11];   /* PAN, [0] : length */
    byte seqnum[2]; /* PAN Sequence Number, [0] : length */
} EXCEPTION_FILE;

typedef struct {
	/*****************/
	/* Data Elements */
	/*****************/

	/******************************/
	/* 각 변수의 첫 Byte는 Length */
	/******************************/
	
	/* Issuer Identification Number (IIN) BF0C or 73 42, SU-32 */
	/* ICC, n 6, 3 */
	byte DE_42[5];
	
	/* Application Identifier (AID) 61 4F */
	/* ICC, b, 5-16 */
	byte DE_4F[18];
	
	/* Application Label 61 or A5 50 */
	/* ICC, ans with the special character limited to space 1-16, 1-16 */
	byte DE_50[18];
	
	/* Track 2 Equivalent Data 70 or 77 57 */
	/* ICC, b, var. up to 19 */
	byte DE_57[21];
	
	/* Application Primary Account Number (PAN) 70 or 77 5A */
	/* ICC, cn(var. up to 19), var. up to 10 */
	byte DE_5A[12];
	
	/* Cardholder Name 70 or 77 5F20 */
	/* ICC, ans 2-26, 2-26 */
	byte DE_5F20[28];
	
	/* Application Expiration Date 70 or 77 5F24 */
	/* ICC, n 6(YYMMDD), 3 */
	byte DE_5F24[5];
	
	/* Application Effective Date 70 or 77 5F25 */
	/* ICC, n 6(YYMMDD), 3 */
	byte DE_5F25[5];
	
	/* Issuer Country Code 70 or 77 5F28 */
	/* ICC, n 3, 2 */
	byte DE_5F28[4];
	
	///* Transaction Currency Code - 5F2A */
	///* Terminal, n 3, 2 */
	//byte DE_5F2A[4];
	
	/* Language Preference A5 5F2D */
	/* ICC, an 2, 2-8 */
	byte DE_5F2D[10];
	byte DE_5F2D_PSE[10];
	
	/* Service Code 70 or 77 5F30 */
	/* ICC, n 3, 2 */
	byte DE_5F30[4];
	
	/* Application Primary Account Number (PAN) Sequence Number 70 or 77 5F34 */
	/* ICC, n 2, 1 */
	byte DE_5F34[3];
	
	///* Transaction Currency Exponent - 5F36 */
	///* Terminal, n 1, 1 */
	//byte DE_5F36[3];
	
	/* Issuer URL A5 5F50 */
	/* Template이 BF0C로 바뀜 */
	/* ICC, ans, var. */
	byte DE_5F50[256];
	
	/* International Bank Account Number (IBAN) BF0C or 73 5F53, SU-32 */
	/* ICC, var., var. up to 34 */
	byte DE_5F53[36];
	
	/* Bank Identifier Code (BIC) BF0C or 73 5F54, SU-32 */
	/* ICC, var., 8 or 11 */
	byte DE_5F54[13];
	
	/* Issuer Country Code (alpha2 format) BF0C or 73 5F55, SU-32 */
	/* ICC, a 2, 2 */
	byte DE_5F55[4];
	
	/* Issuer Country Code (alpha3 format) BF0C or 73 5F56, SU-32 */
	/* ICC, a 3, 3 */
	byte DE_5F56[5];
	
	/* Account Type - 5F57, SU-39 */
	/* Terminal, n 2, 1 */
	byte DE_5F57[3];
	
	/* Application Template 70 or 77 61 */
	/* ICC, b, var. up to 252 */
	//byte DE_61[254];
	
	/* File Control Information (FCI) Template - 6F */
	/* ICC, var., var. up to 252 */
	//byte DE_6F[254];
	
	/* Application Elementary File (AEF) Data Template - 70 */
	//byte DE_70[256];
	
	/* Issuer Script Template 1 - 71 */
	/* Issuer, b, var. */
	//byte DE_71[256];
	
	/* Issuer Script Template 2 - 72 */
	/* Issuer, b, var. */
	//byte DE_72[256];
	
	/* Directory Discretionary Template 61 73 */
	/* ICC, var., var. up to 252 */
	//byte DE_73[254];
	
	/* Response Message Template Format 2 - 77 */
	/* ICC, var., var. */
	//byte DE_77[256];
	
	/* Response Message Template Format 1 - 80 */
	/* ICC, var., var. */
	byte DE_80[256];
	
	/* Amount, Authorised (Binary) - 81 */
	/* Terminal, b, 4 */
	byte DE_81[6];
	
	/* Application Interchange Profile 77 or 80 82 */
	/* ICC, b, 2 */
	byte DE_82[4];
	
	/* Command Template - 83 */
	/* Terminal, b, var. */
	byte DE_83[256];
	
	/* Dedicated File (DF) Name 6F 84 */
	/* ICC, b, 5-16 */
	byte DE_84[18];
	
	/* Issuer Script Command 71 or 72 86 */
	/* Issuer, b, var. up to 261 */
	byte DE_86_71[10][256];
	byte DE_86_72[10][256];
	
	/* Application Priority Indicator 61 or A5 87 */
	/* ICC, b, 1 */
	byte DE_87[3];
	
	/* Short File Identifier (SFI) A5 88 */
	/* ICC, b, 1 */
	byte DE_88[3];
	
	/* Authorisation Code - 89 */
	/* Issuer, As defined by the payment systems, 6 */
	byte DE_89[8];
	
	/* Authorisation Response Code - 8A */
	/* Issuer/Terminal, an 2, 2 */
	byte DE_8A[4];
	
	/* Card Risk Management Data Object List 1 (CDOL1) 70 or 77 8C */
	/* ICC, b, var. up to 252 */
	byte DE_8C[254];
	
	/* Card Risk Management Data Object List 2 (CDOL2) 70 or 77 8D */
	/* ICC, b, var. up to 252 */
	byte DE_8D[254];
	
	/* Cardholder Verification Method (CVM) List 70 or 77 8E */
	/* ICC, b, var. up to 252 */
	byte DE_8E[254];
	
	/* Certification Authority Public Key Index 70 or 77 8F */
	/* ICC, b, 1 */
	byte DE_8F[3];
	
	/* Issuer Public Key Certificate 70 or 77 90 */
	/* ICC, b, N_CA */
	byte DE_90[250];
	
	/* Issuer Authentication Data - 91 */
	/* Issuer, b, 8-16 */
	byte DE_91[18];
	
	/* Issuer Public Key Remainder 70 or 77 92 */
	/* ICC, b, N_I-N_CA+36 */
	byte DE_92[38];
	
	/* Signed Static Application Data 70 or 77 93 */
	/* ICC, b, N_I */
	byte DE_93[250];
	
	/* Application File Locator (AFL) 77 or 80 94 */
	/* ICC, var., var. up to 252 */
	byte DE_94[254];
	
	/* Terminal Verification Results - 95 */
	/* Terminal, b, 5 */
	byte DE_95[7];
	
	/* Transaction Certificate Data Object List (TDOL) 70 or 77 97 */
	/* ICC, b, var. up to 252 */
	byte DE_97[254];
	
	/* Transaction Certificate (TC) Hash Value - 98 */
	/* Terminal, b, 20 */
	byte DE_98[22];
	
	/* Transaction Personal Identification Number (PIN) Data - 99 */
	/* Terminal, b, var. */
	byte DE_99[256];
	
	/* Transaction Date - 9A */
	/* Terminal, n 6(YYMMDD), 3 */
	byte DE_9A[5];
	
	/* Transaction Status Information - 9B */
	/* Terminal, b, 2 */
	byte DE_9B[4];
	
	/* Transaction Type - 9C */
	/* Terminal, n 2, 1 */
	byte DE_9C[3];
	
	/* Directory Definition File (DDF) Name 61 9D */
	/* ICC, b, 5-16 */
	byte DE_9D[18];
	
	///* Acquirer Identifier - 9F01 */
	///* Terminal, n 6-11, 6 */
	//byte DE_9F01[8];
	
	/* Amount, Authorised (Numeric) - 9F02 */
	/* Terminal, n 12, 6 */
	byte DE_9F02[8];
	
	/* Amount, Other (Numeric) - 9F03 */
	/* Terminal, n 12, 6 */
	byte DE_9F03[8];
	
	/* Amount, Other (Binary) - 9F04 */
	/* Terminal, b, 4 */
	byte DE_9F04[6];
	
	/* Application Discretionary Data 70 or 77 9F05 */
	/* ICC, b, 1-32 */
	byte DE_9F05[34];
	
	/* Application Identifier (AID) - 9F06 */
	/* Terminal, b, 5-16 */
	byte DE_9F06[18];
	
	/* Application Usage Control 70 or 77 9F07 */
	/* ICC, b, 2 */
	byte DE_9F07[4];
	
	/* Application Version Number 70 or 77 9F08 */
	/* ICC, b, 2 */
	byte DE_9F08[4];
	
	///* Application Version Number - 9F09 */
	///* Terminal, b, 2 */
	//byte DE_9F09[4];
	
	/* Cardholder Name -Extended 70 or 77 9F0B */
	/* ICC, ans 27-45, 27-45 */
	byte DE_9F0B[47];
	
	/* Issuer Action Code - Default 70 or 77 9F0D */
	/* ICC, b, 5 */
	byte DE_9F0D[7];
	
	/* Issuer Action Code - Denial 70 or 77 9F0E */
	/* ICC, b, 5 */
	byte DE_9F0E[7];
	
	/* Issuer Action Code - Online 70 or 77 9F0F */
	/* ICC, b, 5 */
	byte DE_9F0F[7];
	
	/* Issuer Application Data 77 or 80 9F10 */
	/* ICC, b, var. up to 32 */
	byte DE_9F10[34];
	
	/* Issuer Code Table Index A5 9F11 */
	/* ICC, n 2, 1 */
	byte DE_9F11[3];
	byte DE_9F11_PSE[3];
	
	/* Application Preferred Name 61 or A5 9F12 */
	/* ICC, ans 1-16, 1-16 */
	byte DE_9F12[18];
	
	/* Last Online Application Transaction Counter (ATC) Register - 9F13 */
	/* ICC, b, 2 */
	byte DE_9F13[4];
	
	/* Lower Consecutive Offline Limit 70 or 77 9F14 */
	/* ICC, b, 1 */
	byte DE_9F14[3];
	
	///* Merchant Category Code - 9F15 */
	///* Terminal, n 4, 2 */
	//byte DE_9F15[4];
	
	///* Merchant Identifier - 9F16 */
	///* Terminal, ans 15, 15 */
	//byte DE_9F16[17];
	
	/* Personal Identification Number (PIN) Try Counter - 9F17 */
	/* ICC, b, 1 */
	byte DE_9F17[3];
	
	/* Issuer Script Identifier 71 or 72 9F18 */
	/* Issuer, b, 4 */
	byte DE_9F18[6];
	byte DE_9F18_71[10][6];
	byte DE_9F18_72[10][6];
	
	///* Terminal Country Code - 9F1A */
	///* Terminal, n 3, 2 */
	//byte DE_9F1A[4];
	
	///* Terminal Floor Limit - 9F1B */
	///* Terminal, b, 4 */
	//byte DE_9F1B[6];
	
	///* Terminal Identification - 9F1C */
	///* Terminal, an 8, 8 */
	//byte DE_9F1C[10];
	
	///* Terminal Risk Management Data - 9F1D */
	///* Terminal, b, 1-8 */
	//byte DE_9F1D[10];
	
	///* Interface Device (IFD) Serial Number - 9F1E */
	///* Terminal, an 8, 8 */
	//byte DE_9F1E[10];
	
	/* Track 1 Discretionary Data 70 or 77 9F1F */
	/* ICC, ans, var. */
	byte DE_9F1F[256];
	
	/* Track 2 Discretionary Data 70 or 77 9F20 */
	/* ICC, cn, var. */
	byte DE_9F20[256];
	
	/* Transaction Time - 9F21 */
	/* Terminal, n 6(HHMMSS), 3 */
	byte DE_9F21[5];
	
	/* Certification Authority Public Key Index - 9F22 */
	/* Terminal, b, 1 */
	byte DE_9F22[3];
	
	/* Upper Consecutive Offline Limit 70 or 77 9F23 */
	/* ICC, b, 1 */
	byte DE_9F23[3];
	
	/* Application Cryptogram 77 or 80 9F26 */
	/* ICC, b, 8 */
	byte DE_9F26[10];
	
	/* Cryptogram Information Data 77 or 80 9F27 */
	/* ICC, b, 1 */
	byte DE_9F27[3];
	byte DE_9F27_1st[3];
	
	/* ICC PIN Encipherment Public Key Certificate 70 or 77 9F2D */
	/* ICC, b, N_I */
	byte DE_9F2D[250];
	
	/* ICC PIN Encipherment Public Key Exponent 70 or 77 9F2E */
	/* ICC, b, 1 or 3 */
	byte DE_9F2E[5];
	
	/* ICC PIN Encipherment Public Key Remainder 70 or 77 9F2F */
	/* ICC, b, N_PE-N_I+42 */
	byte DE_9F2F[44];
	
	/* Issuer Public Key Exponent 70 or 77 9F32 */
	/* ICC, b, 1 to 3 */
	byte DE_9F32[5];
	
	/* Terminal Capabilities - 9F33 */
	/* Terminal, b, 3 */
	byte DE_9F33[5];
	
	/* Cardholder Verification Method (CVM) Results - 9F34 */
	/* Terminal, b, 3 */
	byte DE_9F34[5];
	
	/* Terminal Type - 9F35 */
	/* Terminal, n 2, 1 */
	byte DE_9F35[3];
	
	/* Application Transaction Counter (ATC) 77 or 80 9F36 */
	/* ICC, b, 2 */
	byte DE_9F36[4];
	
	/* Unpredictable Number - 9F37 */
	/* Terminal, b, 4 */
	byte DE_9F37[6];

	/* Processing Options Data Object List (PDOL) A5 9F38 */
	/* ICC, b, var. */
	byte DE_9F38[256];
	
	/* Point-of-Service (POS) Entry Mode - 9F39 */
	/* Terminal, n 2, 1 */
	byte DE_9F39[3];
	
	/* Amount, Reference Currency - 9F3A */
	/* Terminal, b, 4 */
	byte DE_9F3A[6];
	
	/* Application Reference Currency 70 or 77 9F3B */
	/* ICC, n 3, 2-8 */
	byte DE_9F3B[10];
	
	///* Transaction Reference Currency - 9F3C */
	///* Terminal, n 3, 2 */
	//byte DE_9F3C[4];
	
	///* Transaction Reference Currency Exponent - 9F3D */
	///* Terminal, n 1, 1 */
	//byte DE_9F3D[3];
	
	/* Additional Terminal Capabilities - 9F40 */
	/* Terminal, b, 5 */
	byte DE_9F40[7];
	
	/* Transaction Sequence Counter - 9F41 */
	/* Terminal, n 4-8, 2-4 */
	byte DE_9F41[6];
	
	/* Application Currency Code 70 or 77 9F42 */
	/* ICC, n 3, 2 */
	byte DE_9F42[4];
	
	/* Application Reference Currency Exponent 70 or 77 9F43 */
	/* ICC, n 1, 1-4 */
	byte DE_9F43[6];
	
	/* Application Currency Exponent - 9F44 */
	/* ICC, n 1, 1 */
	byte DE_9F44[3];
	
	/* Data Authentication Code - 9F45 */
	/* ICC, b, 2 */
	byte DE_9F45[4];
	
	/* ICC Public Key Certificate 70 or 77 9F46 */
	/* ICC, b, N_I */
	byte DE_9F46[250];
	
	/* ICC Public Key Exponent 70 or 77 9F47 */
	/* ICC, b, 1 to 3 */
	byte DE_9F47[5];
	
	/* ICC Public Key Remainder 70 or 77 9F48 */
	/* ICC, b, N_IC-N_I+42 */
	byte DE_9F48[44];
	
	/* Dynamic Data Object List (DDOL) 70 or 77 9F49 */
	/* ICC, b, up to 252 */
	byte DE_9F49[254];
	
	/* Static Data Authentication Tag List - 9F4A */
	/* ICC, --, var. */
	/* Template 70 or 77, Spec의 두곳에 서로 다르게 되어있는데, 70 or 77이 맞는것 같음 */
	/* Book3 p76, p83 */
	byte DE_9F4A[256];
	
	/* Signed Dynamic Application Data - 9F4B */
	/* ICC, b, N_IC */
	/* Template 77 or 80, Spec의 두곳 모두 다르게 되어있는데, 77 or 80이 맞는것 같음 */
	/* Book3 p76, p83 */
	byte DE_9F4B[250];
	
	/* ICC Dynamic Number - 9F4C */
	/* ICC, b, 2-8 */
	 byte DE_9F4C[10];
	
	/* Log Entry BF0C 9F4D, SU-19 */
	/* ICC, b, 2 */
	byte DE_9F4D[4];
	
	///* Merchant Name and Location - 9F4E, SU-19 */
	///* Terminal, ans, var. */
	//byte DE_9F4E[256];
	
	/* Log Format - 9F4F, SU-19 */
	/* ICC, b, var. */
	byte DE_9F4F[256];


	/* Data Elements 중 비휘발성 메모리에 저장해야하는 값 */
	/******************************************************/
	/* Transaction Currency Code - 5F2A */
	/* Terminal, n 3, 2 */
	byte DE_5F2A[4];

	/* Transaction Currency Exponent - 5F36 */
	/* Terminal, n 1, 1 */
	byte DE_5F36[3];

	/* Acquirer Identifier - 9F01 */
	/* Terminal, n 6-11, 6 */
	byte DE_9F01[8];

	/* Application Version Number - 9F09 */
	/* Terminal, b, 2 */
	byte DE_9F09[4];

	/* Merchant Category Code - 9F15 */
	/* Terminal, n 4, 2 */
	byte DE_9F15[4];

	/* Merchant Identifier - 9F16 */
	/* Terminal, ans 15, 15 */
	byte DE_9F16[17];

	/* Terminal Country Code - 9F1A */
	/* Terminal, n 3, 2 */
	byte DE_9F1A[4];

	/* Terminal Floor Limit - 9F1B */
	/* Terminal, b, 4 */
	byte DE_9F1B[6];

	/* Terminal Identification - 9F1C */
	/* Terminal, an 8, 8 */
	byte DE_9F1C[10];

	/* Terminal Risk Management Data - 9F1D */
	/* Terminal, b, 1-8 */
	byte DE_9F1D[10];

	/* Interface Device (IFD) Serial Number - 9F1E */
	/* Terminal, an 8, 8 */
	byte DE_9F1E[10];

	/* Transaction Reference Currency - 9F3C */
	/* Terminal, n 3, 2 */
	byte DE_9F3C[4];

	/* Transaction Reference Currency Exponent - 9F3D */
	/* Terminal, n 1, 1 */
	byte DE_9F3D[3];

	/* Merchant Name and Location - 9F4E, SU-19 */
	/* Terminal, ans, var. */
	byte DE_9F4E[256];
	/******************************************************/

	/* tag가 정의되어 있지 않은 Data Elements */

	/* Issuer Script Results */
	/* Terminal, b, var. */
	byte IsScript_Results[42];


	/* Candidate List */
	//byte cnt_clist;         /* Candidate List의 entry 갯수 */
 	byte sel_app;           /* 최종적으로 선택된 application : Cand_List[sel_app][x] */
	byte Cand_List[5][64];    /* 64자리의 entry 5개 */
		                                /* Cand_List[x][0] 은 '4F' : ADF Name (AID) Length : 5-16 */
		                                /* Cand_List[x][1-16] */
		                                /* Cand_List[x][17]은 '50' : Application Label Length : 1-16 */
		                                /* Cand_List[x][18-33] */
		                                /* Cand_List[x][34]은 '9F12' : Application Preferred Name Length : 1-16 */
		                                /* Cand_List[x][35-50] */
		                                /* Cand_List[x][51]은 '87' : Application Priority Indicator Length : 1 */
		                                /* Cand_List[x][52] */
		                                /* Cand_List[x][53]은 '5F2D' : Language Preference : 2-8 */
		                                /* Cand_List[x][54-61] */
		                                /* Cand_List[x][62]은 '9F11' : Issuer Code Table Index : 1 */
		                                /* Cand_List[x][63] */
		                                /* x = 0, 1, 2, 3, 4 */

	byte PIN_input[14]; /* PIN_input[0] : Length, 0xFF(미입력-초기값), 0x00(PIN없이 입력키만 누름) */

	/* Amount, Authorised = Purchase Amount + Cashback Amount(Amount, Other) */
	dword amount_purchase;      /* 판매 금액 */
	dword amount_authorised;    /* 저장되는 거래금액, decimal point고려해야 함 */
	dword amount_other;         /* 저장되는 Cashbak금액, decimal point고려해야 함 */

	byte f_NotAccepted;     /* 'Not Accepted' message 표시여부 */
	byte f_signature;       /* receipt에 signature line의 유무여부 */

	/* REV_004 */
	byte App_List1[8][40]; 
	byte App_List2[8][40];

	byte f_fallback;        /* FallBack 거래 여부 */

	
	byte f_fonline;         /* 강제온라인 설정여부 */
	byte f_DataCapture;     /* Data Capture, 0x31=BDC(default), 0x32=ODC */
	byte f_onlinePIN;      			/* Online PIN, 0x31=사용(default), 0x32=비사용 */
	byte f_offlinePlainPIN;		/* Offline Plaintext PIN */
	byte f_offlineEncipheredPIN;	/* Offline Enciphered PIN */
	byte f_SignaturePaper;		/* Signature(paper) */

	byte f_FloorLimit;		/* Floor Limit */
	byte f_RandomTransSel;	/* Random Transaction Selection */
	byte f_VelCheck;			/* Velocity Checking */

	byte f_ExceptFile;		/* Exception File */

	byte f_SetAdvice;		/* Advice, 0x31=support, 0x32=not support */

	/* List of AIDs */
	byte cnt_AIDs_Term;     /* Terminal에 있는 AID 갯수 */
	byte AID_Term[20][19];  /* List of AIDs in terminal */

	/* Terminal Action Code(TAC) */
	byte TAC_Denial[7];
	byte TAC_Online[7];
	byte TAC_Default[7];

	/* Default Transaction Certificate Data Object List(TDOL) */
	byte Default_TDOL[20];

	/* Default Dynamic Data Authentication Data Object List(DDOL) */
	byte Default_DDOL[20];

	/* Terminal Floor Limit - 9F1B의 dword type */
	dword Term_FLimit;

	/* Target percentage for random selection */
	//float Tp_rs;
	byte Tp_rs;

	/* Maximum Target percentage for random selection */
	//float MTP_rs;
	 byte MTP_rs;

	/* Threshold value for biased random selection */
	 dword Tv_brs;

	//T=0 or T=1(080715)
	byte f_Protocol;
	
} EMVL2_DATA;

#endif /* EMVTYPEDEF_H_ */
