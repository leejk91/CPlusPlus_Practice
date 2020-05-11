#define __USE_XOPEN
#define _GNU_SOURCE
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>

#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <time.h>
#include <ctype.h>
#include <monetary.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>

#include "lib_koces.h"
#include "lib_util.h"
#include "lib_ubcvan.h"
#include "typedef.h"
#include "termio.h"
#include "app_config.h"

#include "auth_koces.h"

#define DBG_KTC 1
#define DBG_DUKPT_CLIENT	   1
#if ((DBG_DUKPT_CLIENT==1) && (DBG_KTC))
# define	PRINTF(fmt,args...) 	printf(fmt, ##args)
# define	PRINTHEX(fmt,args...)  	PrintHex(fmt, ##args)
# define	HEXDUMP(fmt,args...)  	HEX_Dump(fmt, ##args)
#else
# define    PRINTF(...)    (void)(0)
# define    PRINTHEX(...)  (void)(0)
# define    HEXDUMP(...)   (void)(0)
#endif

#define FK_BACKUP_EN                     1
#define EMV_2ND_GENERATION_EN	0
#define KOCES_PREAMBLE_MAX_SIZE	128

s_koces_fk_info koces_fk_info;
s_koces_dukpt_info koces_dukpt_info;

KocesDupktReq koces_dupkt_req;
KocesDupktRes koces_dupkt_res;

KocesCashReq koces_cash_req;
KocesCashRes koces_cash_res;

KocesMSRReq koces_msr_req;
KocesMSRRes koces_msr_res;

KocesICReq koces_ic_req;
KocesICRes koces_ic_res;

#if 0
extern EMVL2_DATA EMVL2_App;
#endif

#if 0
static unsigned char koces_approval_bakup[1024];
#endif

static unsigned char ktc_in_hash[32] = {
	0x23, 0x37, 0xef, 0xc6, 0xbf, 0xeb, 0xed, 0x1e, 0xd0, 0xa4, 0xf9, 0x40, 0xf2, 0x89, 0x21, 0x41,
	0x7a, 0x9c, 0xad, 0x79, 0xf7, 0xe9, 0x44, 0xba, 0xb9, 0x22, 0x9d, 0x09, 0x25, 0x34, 0x7f, 0x4a,
};

int ktc_comp_result=0;

static int ktc_data_len=1024;
static char ktc_data[1024] = {0x00, };

static int ktc_hide_data_len=1024;
static char ktc_hide_data[1024] = {0x00, };
static unsigned char ktc_enc_hash[1024] = {0x00, };

static char *ktc_term_serial;
static unsigned char ktc_tid[AP_TID_SIZE] = {0x00, };
static unsigned char ktc_key1[AP_KEY_SIZE] = {0x00, };
static unsigned char ktc_send_token_cs[128] = {0x00, };

static unsigned char ktc_ksn[10]={0x00, };

static size_t ktc_enc_hash_len;
static size_t ktc_tid_len = sizeof(ktc_tid);
static int ktc_ksn_len = sizeof(ktc_ksn);
static size_t ktc_key1_len = sizeof(ktc_key1);
static size_t ktc_send_token_cs_len = sizeof(ktc_send_token_cs);

static KOCES_FUNCPTR stUserFunc;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
static int fd;

int Debug_Open(void)
{
    struct termios newtio;

    fd = open("/dev/ttymxc0", O_RDWR | O_NOCTTY);

    if (fd < 0) {
        printf("Device Open Error : /dev/ttymxc0\n\r");
        perror(__FILE__);
        return -1;
    }
    else{
        //printf("\tDevice Open :/dev/ttymxc0\n\r");
    }

    memset(&newtio, 0, sizeof(newtio));
    newtio.c_iflag = IGNPAR; // non-parity
    newtio.c_oflag = 0;
    newtio.c_cflag = CS8 | CLOCAL | CREAD; // NO-rts/cts
    newtio.c_cflag |= B115200;

    newtio.c_lflag = 0;
    newtio.c_cc[VTIME] = 10; // timeout 0.1초 단위
    newtio.c_cc[VMIN] = 32; // 최소 n 문자 받을 때까진 대기
    tcflush ( fd, TCIFLUSH );
    tcsetattr( fd, TCSANOW, &newtio );

    return fd;
}

void Debug_Close(void)
{
    close(fd);
}

void Dbg_Printf(const char *fmt,...)
{
    va_list ap;
    char buf[256];

    va_start(ap,fmt);
    vsprintf(buf,fmt,ap);
    va_end(ap);
    write(fd, buf, strlen(buf));
}

void PrintHex(unsigned char *nAddr, unsigned long nLen, char *sFrom, ...)
{
    unsigned long i, j=0;
    va_list ap;
    char buf[128];

    va_start(ap,sFrom);
    vsprintf(buf,sFrom,ap);
    va_end(ap);

    for(i=0; buf[i]=='\n' || buf[i]=='\r'; i++) {
        //Dbg_Printf(buf[i]);
        write(fd, &buf[i], 1);
    }

    if(strlen(&buf[i])>=15) {
        Dbg_Printf("%s\r\n", &buf[i]);
        Dbg_Printf("%-15s", "");

    } else {
        Dbg_Printf("%-15s", &buf[i]);
    }
    for (i = 0; i < nLen;) {
        Dbg_Printf("%02X ", nAddr[i]);

        i++; j++;

        if ((j % 16) == 0) {
            j = 0;
            if(i!=nLen) 	Dbg_Printf("\r\n%15s", "");
        }
        else if ((j % 8) == 0) {
            Dbg_Printf("  ");
        }
    }
    Dbg_Printf("\r\n");
}


#define HEX_SHORT_DUMP	1
void HEX_Dump(unsigned char *nAddr, unsigned long nLen, char *sFrom, ...) {
    unsigned char *inP, *endP;
    unsigned long i, nRemainder;
    va_list ap;
    char buf[128];

    va_start(ap,sFrom);
    vsprintf(buf,(const char*)sFrom,ap);
    va_end(ap);

    endP = (unsigned char*)((unsigned long)nAddr+nLen);
    nRemainder = nLen%16;

    for(i=0; buf[i]=='\n' || buf[i]=='\r'; i++) {
        //Dbg_Printf(buf[i]);
        write(fd, &buf[i], 1);
    }

#if HEX_SHORT_DUMP
    if(strlen(&buf[i])>=15) {
        Dbg_Printf("%s\r\n", &buf[i]);
        Dbg_Printf("%-15s", "");

    } else {
        Dbg_Printf("%-15s", &buf[i]);
    }
#else
    //Dbg_Printf(com, "\r\nOffset         HEX Value                                         ASCII value\r\n");
    Dbg_Printf(com, "%-15s 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\r\n", buf);
    //Dbg_Printf(com, "%-15s-----------------------------------------------\r\n", buf);
    //Dbg_Printf(com, "\r\n%-15s", buf);
#endif

    //print out 16byte blocks
    for (inP=nAddr; inP+16 <= endP; inP+=16) {
        // offset output
        #if HEX_SHORT_DUMP
        if(inP!=nAddr)
        #endif
        Dbg_Printf("0x%08lX     ", (long)(inP-nAddr));

        // 16-byte assign
        for(i=0; i<16; i++) {
            Dbg_Printf("%02X ", inP[i]);
        }
        Dbg_Printf("  ");

        for(i=0; i<16; i++) {
            if(inP[i]>=32 && inP[i]<=125)
                Dbg_Printf("%c", inP[i]);
            else
                Dbg_Printf(".");
        }

        Dbg_Printf("\r\n");
    }

    if(nRemainder) {
        // offset output
        #if HEX_SHORT_DUMP
        if(inP!=nAddr)
        #endif
        Dbg_Printf("0x%08lX     ", (long)(inP-nAddr));

        // 16-byte assign
        for(i=0; i<nRemainder; i++) {
            Dbg_Printf("%02X ", inP[i]);
        }

        for(; i<16; i++) {
            Dbg_Printf("   ");
        }
        Dbg_Printf("  ");

        for(i=0; i<nRemainder; i++) {
            if(inP[i]>=32 && inP[i]<=125)
                Dbg_Printf("%c", inP[i]);
            else
                Dbg_Printf(".");
        }

        Dbg_Printf("\r\n");
    }

    if(!nLen)	Dbg_Printf("\r\n");
}

int koces_printf(const char *fmt,...)
{
#if DBG_DUKPT_CLIENT
    va_list ap;
    char buf[256];
    //buf[0]='\r';
    va_start(ap,fmt);
    vsprintf(buf,fmt,ap);
    va_end(ap);
    if(strlen(buf)<255)
    {
        buf[strlen(buf)+2]=0x00;
        buf[strlen(buf)+1]='\n';
        buf[strlen(buf)]='\r';
    }
    write(fd, buf, strlen(buf));
    return (int)strlen(buf);
#else
    return 0;
#endif
}

void Debug_Test(void)
{
    unsigned char sample = 0x52;

    PRINTF("\n\r>> test %02x, %d\n\r", sample, sample);
    PRINTHEX((unsigned char*)ktc_in_hash, 32, "test2 : ", sample);
}

int RNGGenerate(unsigned char *entropy_data, int *out_entropy_data_len)
{
    int entropy_data_len = 0;
    unsigned long l;

    l=(unsigned long)rand();
    memcpy(entropy_data+entropy_data_len, &l, sizeof(l));
    entropy_data_len += sizeof(l);

    *out_entropy_data_len = 256;

    PRINTF("RNGGenerate : *out_entropy_data_len = %d, entropy_data_len = %d\r\n", *out_entropy_data_len, entropy_data_len);

    return KOCES_DUKPT_SUCCESS;
}

int Time2Ctime(unsigned char *OutAddr, int *OutLenth)
{
    int ret = KOCES_DUKPT_ERR_TIME;
    unsigned char *Addr = NULL;
    time_t ts = 0;

    time(&ts);
    Addr = (unsigned char*)ctime(&ts);
    if(Addr == 0x00)
    {
        return ret;
    }
    strncpy((char*)OutAddr, (char*)Addr, strlen((char*)Addr));
    *OutLenth = (int)(strlen((char*)OutAddr));
    ret = KOCES_DUKPT_SUCCESS;

    return ret;
}

int Strtime2Mktime(unsigned char *Input, unsigned int *OutSec)
{
    int ret = KOCES_DUKPT_ERR_TIME;
    struct tm tm;

    printf("Input[0] = %02x, Input[1] = %02x, Input[2] = %02x, Input[3] = %02x, Input[4] = %02x\n", Input[0], Input[1], Input[2], Input[3], Input[4]);

    printf("[%s]\n", Input);
    strptime((const char *)Input, "%a %b %d %H:%M:%S %Y", &tm);
    *OutSec = (unsigned int )mktime(&tm);
    ret = KOCES_DUKPT_SUCCESS;

    printf("*OutSec = %d\n", *OutSec);

    return ret;
}

int Time(unsigned int *OutSec)
{
    int ret = KOCES_DUKPT_ERR_TIME;

    printf("*OutSec = %d\n", *OutSec);

    time((time_t*)OutSec);
    ret = KOCES_DUKPT_SUCCESS;

    printf("*OutSec = %d\n", *OutSec);

    return ret;
}

void koces_self_verification(void)
{
    int ret=0;
    FILE *fp;

    // APP, 보안Lib, HData, EMV
    // Future Key
    ktc_comp_result = 0x07;
    fp = fopen(KTC_DUKPT_PATH, "r");
    if(fp!=NULL) {
        fread(&koces_dukpt_info, sizeof(s_koces_dukpt_info), 1, fp);
        fclose(fp);
    }

    PRINTHEX((unsigned char*)&koces_dukpt_info, 16, "\n\rDUPKT(%s):", KTC_DUKPT_PATH);

    if(koces_dukpt_info.key_injection == UBCN_DUKPT_SUCCESS)
    {
        if(koces_dukpt_info.hdata_used== UBCN_DUKPT_SUCCESS)
        {
            ktc_hide_data_len = 1024;
            memcpy(ktc_hide_data, koces_dukpt_info.hdata, (unsigned long)ktc_hide_data_len);
            fp = fopen(KTC_FK_PATH, "r");
            if(fp!=NULL) {
                fread(&koces_fk_info, sizeof(s_koces_fk_info), 1, fp);
                fclose(fp);
            }

            //hdata
            PRINTHEX(koces_fk_info.ksn, 10, "\n\rKSN(%s):", KTC_FK_PATH);
            ret = koces_hdata_comp_self_protect();

            if(ret>=0) {
                ktc_comp_result &= ~0x01;
            }
            //fk
            ret = koces_fk_comp_self_protect();
            if(ret>=0) {
                ktc_comp_result &= ~0x02;
            }
            //app
            ret = koces_bin_comp_self_protect();
            if(ret>=0) {
                ktc_comp_result &= ~0x04;
            }
            //lib

            PRINTF("\r\n>> ktc_comp_result 0x%02x\r\n", ktc_comp_result);
            memset(ktc_in_hash,0x00,sizeof(ktc_in_hash));
        }

    }

#if 0
    UBC_SetTmsCip();
    if(ktc_comp_result)
        STA_Error(NULL, 1);
    else STA_Ready(NULL), BEEP_Success();
#endif

    PRINTHEX((unsigned char*)&ktc_comp_result, 4, "\n\rKEY R.%08X", ktc_comp_result);
    save_self_verification(ktc_comp_result);//seo 181210 추가 무결성검사시 결과를 저장한다.
}


void koces_init(void)
{
    memset(&stUserFunc, 0x00, sizeof(KOCES_FUNCPTR));

    // Set Function Pointer
    stUserFunc.pPrintf = koces_printf;
    stUserFunc.pRandomGenerator = RNGGenerate;
    stUserFunc.pTime2Ctime = Time2Ctime;
    stUserFunc.pStrtime2Mktime = Strtime2Mktime;
    stUserFunc.pTime = Time;

    KOCES_DUKPT_InitFunc(&stUserFunc, 1);			// 사용자 모드

#if 0
    KOCES_DUKPT_SetDebug((int)g_VcpsInfo.Debug);
#else
    KOCES_DUKPT_SetDebug(1);							// Debug Mode
#endif

    koces_self_verification();

    PRINTHEX((unsigned char *)&ktc_comp_result, 4, "\n\rFK(%s)", KTC_FK_PATH);
    PRINTHEX((unsigned char *)&ktc_comp_result, 4, "\n\rEX(%s)", KTC_MARU_PATH);
    PRINTHEX((unsigned char *)&ktc_comp_result, 4, "\n\rDK(%s)", KTC_DUKPT_PATH);
}

unsigned char koces_get_hdata_used(void)
{
    return koces_dukpt_info.hdata_used;
}

unsigned long koces_get_ksn(unsigned char *ksn)
{
    memcpy(ksn, koces_fk_info.ksn, 10);
    return (unsigned long)koces_fk_info.ksn;
}

/* tid 생성 시 첫 번재 바이트는 '1'~'9', 'a'~'z', 'A'~'Z' 이며 보안키 갱신 시 증가를 하는 것이 원칙이다. */
#define KOCES_DUPKT_1ST_BYTE_LEN	(9+26+26)
static const unsigned char koces_dupkt_1st_byte[KOCES_DUPKT_1ST_BYTE_LEN] = "123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

void koces_dupkt_request(void)
{
#if 1
    int i, nLen=0;//, offset;
    //char *pbuf;
    unsigned char *pdata, buf[256];
    unsigned short crc;

    //int enc_len;
    PKocesDupktReq pKoces;

    // 요청시 반드시 해야 하므로.
#if 0
    if(UBC_LogCheck()) return;
#endif
    ktc_comp_result = 1;

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
    sprintf((char*)VAN_BasicReq.PacketClass,           "%s",   g_NanoPacket[14]);
    // 13 an 1 입력매체 구분코드 , K:KEYIN, S:SWIPE, R:RF, I:IC, X:NON
    VAN_BasicReq.TransactionType = 'K';
    // 14 an 5 단말기 거래일련 번호, 총 거래 Cnt
    sprintf((char*)VAN_BasicReq.TransactionCount,      "%05d", (int)g_TmsLog.TotCnt);
    // 19 an 2 전문 버젼
    sprintf((char*)VAN_BasicReq.PacketVersion,         "%s",   "T1");
    // 1B an 1 전문 암호화 여부 안함
    VAN_BasicReq.Encryption = '0';
    // 1C an 10 예비용
    //sprintf((char*)VAN_BasicReq.RFU1,                  "%010d", ' ');
    // 26 FS																//
    VAN_BasicReq.FS1 = FS;
    // 27 an 40 인식 카드번호
    // 카드일련번호 : "15442020****123456=",  "94100000****0000=********************"( 앞 8자리 이후 4개 정보와 '=' 이후 정보를 '*'로 설정)
    //memcpy(VAN_BasicReq.CSN, track2, strlen(track2));
    // 4F FS
    VAN_BasicReq.FS2 = FS;
    // 50 an 2 할부 개월 수
    sprintf((char*)VAN_BasicReq.Installment,           "%02d", 0);
    // 52 FS
    VAN_BasicReq.FS3 = FS;
    // 53 an 10 승인금액
    //sprintf((char*)VAN_BasicReq.Amount,      			"%010d", m_nAmount);
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
    //memset(VAN_BasicReq.ApproveNum, ' ', 10);
    // 89 FS
    VAN_BasicReq.FS9 = FS;
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    nLen = 0;
    pKoces = (PKocesDupktReq)VAN_BasicReq.AddedInfo;

    //memcpy(pKoces->preamble_num, "2410", 4);
    //nLen += 4;

    //memcpy(pKoces->cat_id, g_VmConfig.CATID, 10);
    //nLen += 10;

    memcpy(pKoces->auth_num, VM_GetKtcApprovalNumber(), 32);
    nLen += 32;

    //BCD2Char(g_Today, pKoces->date_time, 7);
    GetTimeString((time_t)NULL, (char*)pKoces->date_time);
    nLen += 14;

    PRINTF("========================== KOCES_DUKPT_Make_TokenCS() ===========================\r\n");
    {
        int ret;
        char tmp[1024];
        FILE *fp;

        fp = fopen(KTC_DUKPT_PATH, "r");
        if(fp!=NULL) {
            fread(&koces_dukpt_info, sizeof(s_koces_dukpt_info), 1, fp);
            fclose(fp);
            fp = fopen(KTC_FK_PATH, "r");
            if(fp!=NULL) {
                fread(&koces_fk_info, sizeof(s_koces_fk_info), 1, fp);
                fclose(fp);
            }
            else {
#if 0//(FK_BACKUP_EN==1)
                fp = fopen(KTC_FK_BAK_PATH, "r");
                if(fp!=NULL) {
                    fread(&koces_fk_info, sizeof(s_koces_fk_info), 1, fp);
                    fclose(fp);
                    koces_fk_info.ksn_1st +=1;
                }
                else {
                    koces_fk_info.ksn_1st =1;
                }
#endif
            }
        }
        else {
            return ;
        }

        //HEX_Dump(DEBUG_PORT, (u8*)&koces_fk_info, sizeof(s_koces_fk_info), "FK %d", sizeof(s_koces_fk_info));
        //HEX_Dump(DEBUG_PORT, (u8*)&koces_dukpt_info, sizeof(s_koces_dukpt_info), "DUPKT %d", sizeof(s_koces_dukpt_info));

        ktc_term_serial = (char*)koces_dukpt_info.serial_number;
        memcpy(ktc_hide_data,  koces_dukpt_info.hdata, 1024);

        ktc_tid[0] = koces_dupkt_1st_byte[koces_fk_info.ksn_1st%KOCES_DUPKT_1ST_BYTE_LEN];
        ktc_tid[1] = (unsigned char)ktc_term_serial[0];
        ktc_tid[2] = (unsigned char)ktc_term_serial[1];
        Char2BCD((unsigned char*)&ktc_term_serial[2], &ktc_tid[3], 8);

        koces_fk_info.ksn_counter =0;

        memcpy(ktc_ksn, ktc_tid, AP_TID_SIZE);
        ktc_ksn[7]=(unsigned char)(koces_fk_info.ksn_counter>>16);
        ktc_ksn[8]=(unsigned char)(koces_fk_info.ksn_counter>>8);
        ktc_ksn[9]=(unsigned char)(koces_fk_info.ksn_counter>>0);

        memcpy(koces_fk_info.ksn, ktc_ksn, 10);
        memset(ktc_send_token_cs,0x00,sizeof(ktc_send_token_cs));
        /* IN : send_token_cs 의 버퍼크기, OUT : send_token_cs의 실제 크기 */
        ret = KOCES_DUKPT_Make_TokenCS((const unsigned char*)ktc_hide_data, ktc_hide_data_len, ktc_tid, (int)ktc_tid_len, ktc_key1, (int)ktc_key1_len, ktc_send_token_cs, (int *)&ktc_send_token_cs_len );
        PRINTHEX(ktc_tid, ktc_tid_len, "tid[%d] ",ktc_tid_len);
        PRINTHEX(ktc_key1, ktc_key1_len, "ktc_key1[%d]",ktc_key1_len);
        PRINTHEX(ktc_send_token_cs, ktc_send_token_cs_len, "send_token_cs[%d]",ktc_send_token_cs_len);
        if (ret < 0)
        {
            koces_dukpt_info.key_injection = UBCN_DUKPT_CANCEL;
            PRINTF("Make_TokenCS Error!(%d)\r\n", ret);
            return;
        }

        // 검증 요청 데이터 : 길이(3byte, ex.087)+TrsmID(7)+Crandom(16)+KEY1_ENC(25바이트Timpstamp+가변SvrInfo+7바이트TID+16바이트CRandom)
        sprintf(tmp, "%03d", (int)ktc_send_token_cs_len);       /* 통신전문상에서는 Token_cs 데이타 앞에 길이를 넣어줘야함 */
        memcpy(&tmp[3], ktc_send_token_cs, ktc_send_token_cs_len);

        Hex2Ascii((unsigned char*)tmp, pKoces->data, (int)(ktc_send_token_cs_len+3));
    }

    nLen += 256;

    PRINTF("==========================================================================\r\n");

    // 8A FS
    VAN_BasicReq.AddedInfo[nLen++] = FS;
    // 8B an 판매된 Column
    //BCD2Char((uchar*)&g_VmAmountColumn.Col, (uchar*)&VAN_BasicReq.AddedInfo[nLen], 2);	// an 현재 컬럼.
    nLen+=2;
    // 8D
    VAN_BasicReq.AddedInfo[nLen++] = 0x03;

    // 단말기번호 ~ ETX까지의 길이
    pdata = VAN_BasicReq.TID;
    for(i=0; i<(int)sizeof(NanoVANBasicReq) ; i++) {
        if(pdata[i]==0x03) break;
    }
    sprintf((char*)buf,  "%04d", i+1);
    memcpy((char*)VAN_BasicReq.MsgLen, buf, 4);

    // CRC
    crc = (unsigned short)CRC_Update((unsigned char*)&VAN_BasicReq, (unsigned short)Char2Int(VAN_BasicReq.MsgLen, 0)+5);
    Int2Byte(crc, &VAN_BasicReq.AddedInfo[nLen]);

    PRINTHEX((unsigned char*)&VAN_BasicReq, (unsigned long)Char2Int(VAN_BasicReq.MsgLen, 0)+5, "DUPKT REQ %d", Char2Int(VAN_BasicReq.MsgLen, 0)+5);

#if 0
    UBC_Backup(&VAN_BasicReq);
#endif

#endif
}

void koces_dupkt_response(unsigned char *recv_token_sc, size_t recv_token_sc_len)
{
    FILE *fp;
    int i, ret;
    unsigned char ipek[AP_KEY_SIZE] = {0x00, };
    unsigned char token_sc[128];
    int token_sc_len;
    int ipek_len = sizeof(ipek);
    int in_hash_len = 32;
    KOCES_FKINFO fki;
    /* 자체보호 */
    unsigned char digest[32] = {0x00, };

    Char2BCD(recv_token_sc, token_sc, (int)recv_token_sc_len);
    token_sc_len = (int)(recv_token_sc_len/2);

    PRINTHEX(token_sc, (unsigned long)token_sc_len, "\r\nToken_SC");

    PRINTF("========================= KOCES_DUKPT_Verify_TokenSC() =========================\r\n");
    ret = KOCES_DUKPT_Verify_TokenSC((const unsigned char*)ktc_hide_data, ktc_hide_data_len, token_sc, token_sc_len, ktc_ksn, ktc_ksn_len,  ktc_key1,  (int)ktc_key1_len, (KOCES_FKINFO*)&fki);
    PRINTHEX(token_sc, (unsigned long)token_sc_len, "token_sc");
    PRINTHEX(ktc_tid, ktc_tid_len, "tid ");
    PRINTHEX(ktc_key1, ktc_key1_len, "ktc_key1");
    if (ret < KOCES_DUKPT_SUCCESS)
    {
        PRINTF("Verify_TokenCS Error! (%d)\r\n", ret);
        return;
    }
    PRINTF("================================================================================\n\r\n");

    PRINTHEX(fki.KSN, sizeof(fki.KSN), "KSN");
    for (i=0; i<21; i++)
    {
        PRINTHEX(fki.FKSet[i], sizeof(fki.FKSet[i]), "FKSet[%02d]", i);
    }

    // 단말기 내 비휘발성저장소
    koces_fk_info.fki = fki;
    memcpy(koces_dukpt_info.ipek, ipek, AP_KEY_SIZE);

    memset(ipek, 0, sizeof(ipek));
    memset(ipek, 255, sizeof(ipek));
    memset(ipek, 0, sizeof(ipek));

    PRINTHEX(ipek, (unsigned long)ipek_len, "\r\nipek");

    PRINTF("==================================================================================\r\n\r\n");

    PRINTHEX((unsigned char*)ktc_hide_data, (unsigned long)ktc_hide_data_len, "hdata");

    /*******************************************************************************************/
    /************************************* 자체보호 ********************************************/
    /*******************************************************************************************/

    /* 정상 데이터를 Sha-256으로 Hash 값을 추출 - 자체보호 HASH 추출 */
    PRINTF("========================== KOCES_CRYPT_HASH_SHA256(%d) ===========================\r\n\r\n",ktc_hide_data_len);
    //usleep(10000);
    KOCES_CRYPT_HASH_SHA256((const unsigned char*)ktc_hide_data, ktc_hide_data_len, digest);
    //usleep(10000);
    PRINTHEX(digest, 32, "Hash Data(OUT)");

    in_hash_len = sizeof(digest);
    memcpy(ktc_in_hash, digest, (size_t)in_hash_len);

    /* 입력(ktc_in_hash)데이타를 SKey로 암호화하여 줌 - 자체보호 HASH 암호화 */
    PRINTF("===================== KOCES_DUKPT_Make_Salf_Protect_HASH() =====================\r\n");
    ret = KOCES_DUKPT_Make_Self_Protect_HASH((const unsigned char*)ktc_hide_data, ktc_hide_data_len, ktc_enc_hash, (int *)&ktc_enc_hash_len, ktc_in_hash, in_hash_len);
    PRINTHEX(ktc_in_hash, (unsigned long)in_hash_len, "ktc_in_hash(IN)");
    PRINTF("KOCES_DUKPT_Make_Self_Protect_HASH: in_hash_len(IN) : (%d)\r\n", in_hash_len);
    if(ret < 0)
    {
        PRINTF(" Failure!!! : (%d)\r\n", ret);
    }
    PRINTHEX(ktc_enc_hash, ktc_enc_hash_len, "ktc_enc_hash(OUT) %d", ret);
    PRINTF(" Success!!!\r\n");
    PRINTF("================================================================================\r\n\r\n");

    // 단말기 내 비휘발성저장소
    koces_dukpt_info.enc_hash_len = ktc_enc_hash_len;
    memcpy(koces_dukpt_info.enc_hash, ktc_enc_hash, ktc_enc_hash_len);

    koces_fk_make_self_protect();
    koces_bin_make_self_protect();

#if 1 //
    koces_library_make_self_protect();
#endif

    GetTimeString((time_t)NULL, (char *)(koces_fk_info.time));

    koces_dukpt_info.key_injection = UBCN_DUKPT_SUCCESS;
    koces_dukpt_info.hdata_used = UBCN_DUKPT_SUCCESS;

    fp = fopen(KTC_DUKPT_PATH, "wb");
    fwrite(&koces_dukpt_info, sizeof(s_koces_dukpt_info), 1, fp);
    fclose(fp);

    koces_fk_info.ksn_1st +=1;

    fp = fopen(KTC_FK_PATH, "wb");
    fwrite(&koces_fk_info, sizeof(s_koces_fk_info), 1, fp);
    fclose(fp);

    //키 저장후 삭제해야함. 체크요망.
    //memset(koces_dukpt_info.hdata,0x00,1076);
    //memset(koces_fk_info.fki,0x00,sizeof(KOCES_FKINFO)+36);
    //HEX_Dump(DEBUG_PORT, (u8*)&koces_fk_info, sizeof(s_koces_fk_info), "END FK %d", sizeof(s_koces_fk_info));
    //HEX_Dump(DEBUG_PORT, (u8*)&koces_dukpt_info, sizeof(s_koces_dukpt_info), "END DUPKT %d", sizeof(s_koces_dukpt_info));

    koces_self_verification();
}

unsigned long koces_get_ksn_counter(void)
{
    FILE *fp;

    fp = fopen(KTC_FK_PATH, "r");
    if(fp == NULL) {

    }
    else{
        fclose(fp);

        koces_fk_info.ksn_counter  = (unsigned long)(koces_fk_info.ksn[7]&0x1F)<<16;
        koces_fk_info.ksn_counter |= (unsigned long)koces_fk_info.ksn[8]<<8;
        koces_fk_info.ksn_counter |= (unsigned long)koces_fk_info.ksn[9]<<0;

        PRINTHEX(koces_fk_info.ksn, 10, "\r\nkoces_get_ksn_counter(%d)", koces_fk_info.ksn_counter);

        if(koces_fk_info.ksn_counter+1 >= 0x1FFFFF)
        {
            koces_dukpt_info.key_injection = UBCN_DUKPT_INIT;
            koces_dupkt_request();
        }

        koces_fk_make_self_protect();

        fp = fopen(KTC_FK_PATH, "w");
        fwrite(&koces_fk_info, sizeof(s_koces_fk_info), 1, fp);
        fclose(fp);
    }

    return koces_fk_info.ksn_counter;
}

void koces_fallback_request(int nType, unsigned long nAmount, unsigned char nReason)
{
#if 0
    int i, nLen=0, offset, track2_len;
    char track2[128], tmp[128];
    char *pbuf;
    unsigned char *pdata, buf[256];
    unsigned char ksn[10];
    unsigned short crc;

    size_t ksn_len, enc_len;
    PKocesICReq pKoces;

    if(( (nType==1) || (nType==6) ) && UBC_LogCheck()) return;

    memset((char*)&VAN_BasicReq, ' ', sizeof(NanoVANBasicReq));

    // 00
    VAN_BasicReq.stx = 0x02;
    // 01  an 4 단말기번호 - ETX 길이
    //sprintf((char*)VAN_BasicReq.MsgLen,                "%04d", );
    // 05  an 10 단말기 번호 "1234567890"
    sprintf((char*)VAN_BasicReq.TID,                   "%s",   g_VcpsInfo.TID);
    // 0F an 2 거래(업무) 구분
    sprintf((char*)VAN_BasicReq.TransactionClass,      "%s",   g_NanoTransaction[1]);
    // 11 an 2 전문 구분, 수집
    sprintf((char*)VAN_BasicReq.PacketClass,           "%s",   g_NanoPacket[nType]);
    // 13 an 1 입력매체 구분코드 , K:KEYIN, S:SWIPE, R:RF, I:IC, X:NON
    VAN_BasicReq.TransactionType = 'I';   // RF_EMV
    // 14 an 5 단말기 거래일련 번호, 총 거래 Cnt
    if( (nType==1) || (nType==6) )
        sprintf((char*)VAN_BasicReq.TransactionCount,      "%05d", g_TmsLog.TotCnt);
    else
        sprintf((char*)VAN_BasicReq.TransactionCount,      "%05d", m_nNanoTotCnt);

    // 19 an 2 전문 버젼
    sprintf((char*)VAN_BasicReq.PacketVersion,         "%s",   "T1");
    // 1B an 1 전문 암호화 여부 안함
    VAN_BasicReq.Encryption = '0';
    // 1C an 10 예비용
    memset((char*)VAN_BasicReq.RFU1, ' ', 10);
    // 26 FS																//
    VAN_BasicReq.FS1 = FS;
    // 27 an 40 인식 카드번호

    // Track II Data : Track2 정보는 3rd와 '=' 이후 전부 '*'로 마스킹해서 전송
    //ex) 유효 Track2 정보가  "9400123412341234=19121234123451234567"(37byte) 일 경우 "940012**********=1*******************   "(40byte)
    // 암호화 대상 정보는 유효 Track2 정보와 동일하며 암호화한 데이터가 48byte 보다 작을 경우 LEFT-JUSTIFIED, 스페이스 패딩해야 함.
    memset(track2, 0x20, sizeof(track2));
    if( (nType==1) || (nType==6) ) {
        memcpy(track2, g_MsrInfo.nTrack2, g_MsrInfo.nCnt2);
    }
    else {
        memcpy(track2, m_nMsrInfo.nTrack2, m_nMsrInfo.nCnt2);
        UBC_TrackBackupDataClear();
    }

    track2_len = 40;

    memset(tmp, '*', track2_len);
    tmp[track2_len] = 0;

    memcpy(tmp, track2, 6);
    pbuf = strstr((char *)track2, "=");

    if(!pbuf) return;

    PRINTHEX(track2, 64, "t2 in address : 0x%08X", track2);

    offset = (unsigned long)pbuf-(unsigned long)track2;

    if(offset==16) tmp[offset-4] = *(pbuf-4);
    tmp[offset-3] = *(pbuf-3);
    tmp[offset-2] = *(pbuf-2);
    tmp[offset-1] = *(pbuf-1);
    tmp[offset]    = *(pbuf);
    tmp[offset+5] = *(pbuf+5);

    memcpy(VAN_BasicReq.CSN, tmp, 40);

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
    case 0x06 :
        m_nAmount = 0;
        sprintf((char*)VAN_BasicReq.Amount,      			"%010d", nAmount);
        break;
    default :
        sprintf((char*)VAN_BasicReq.Amount,      			"%010d", nAmount);
        UBC_SetTrack2Backup(nAmount);
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
        for(i=0; i<8; i++)  VAN_BasicReq.ApproveDate[i] = VAN_BasicRes.Date[i];
        // 7E FS
        VAN_BasicReq.FS8 = FS;
        // 7F an 10 원거래 승인 번호
        for(i=0; i<8; i++)  VAN_BasicReq.ApproveNum[i] = VAN_BasicRes.ApproveNum[i];
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

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////
    nLen = 0;
    pKoces = (PKocesICReq)VAN_BasicReq.AddedInfo;

    memset(VAN_BasicReq.AddedInfo, 0x30, sizeof(VAN_BasicReq.AddedInfo));

    //memcpy(pKoces->preamble_num, "2410", 4);
    //nLen += 4;

    //memcpy(pKoces->cat_id, g_VmConfig.CATID, 10);
    //nLen += 10;

    // 단말인증번호
    memcpy(pKoces->auth_num, VM_GetKtcApprovalNumber(), 32);
    nLen += 32;

    // 거래일시
    BCD2Char(g_Today, pKoces->date_time, 7);
    nLen += 14;

    // 암호화된 Track II Data
    ksn_len = 10;
    memcpy(ksn, koces_fk_info.ksn, sizeof(ksn));
    memset(tmp, 0x20, sizeof(tmp));
    memcpy(tmp, track2, track2_len);
    KOCES_DUKPT_AES_Encrypt(koces_dukpt_info.hdata, 1024, buf, (int*)&enc_len, ksn, (int*)&ksn_len, (const unsigned char*)tmp, 40, &koces_fk_info.fki);
    Hex2Ascii(buf, pKoces->crypto_track2, enc_len);
    nLen += 96;

    PRINTHEX(pKoces->crypto_track2, enc_len*2, "t2 cryto in address : 0x%08X", pKoces->crypto_track2);

    // KSN Copy.
    Hex2Ascii(ksn, pKoces->ksn, ksn_len);
    memcpy(koces_fk_info.ksn, ksn, sizeof(ksn));
    nLen += 20;
    PRINTHEX(koces_fk_info.ksn, 10, "t2 cryto ksn");
    // Working Key Index
    Hex2Ascii("\00", pKoces->working_key_index, 1);
    nLen += 2;

    // 비밀번호
    memset(pKoces->password, ' ', 16);
    nLen += 16;

    // EMV거래구분
    pKoces->emv_transaction_type = 'F';
    nLen += 1;

    // POS Entry Mode Code
    // 일반 IC 거래 시 4BCD 형태로 카드에서 전달된 정보를 전송
    // FallBack 거래 시 반드시 PosEntryMode[0]=0x90, PosEntryMode[1]=0x10 로 설정,
    // 기타 IC 관련 필드("Card Sequence Number"~"Transaction Sequence Counter")는 NULL로 설정.
    Hex2Ascii((unsigned char*)"\x90\x10" , pKoces->pos_entry_mode_code, 2);
    nLen += 4;

    // Card Sequence Number, 5f34
//	Hex2Ascii(EMVL2_App.DE_5F34 , pKoces->card_sequence_number, EMVL2_App.DE_5F34[0]);
    nLen += 4;

    // Additional POS information
    //Hex2Ascii((unsigned char*)"      ", pKoces->additional_pos_information, 6);
    nLen += 12;

    // Issuer Script Results
    tmp[0]=0x9F;
    tmp[1]=0x5B;
//	tmp[2]=0x00;
//	Hex2Ascii(tmp, pKoces->DE_9F5B, 3);
    nLen += 46;

    // Application Cryptogram (AC)
    tmp[0]=0x9F;
    tmp[1]=0x26;
//	memcpy(tmp+2, EMVL2_App.DE_9F26, EMVL2_App.DE_9F26[0]+1);
//	Hex2Ascii(tmp, pKoces->DE_9F26, EMVL2_App.DE_9F26[0]+3);
    nLen += 22;

    // Cryptogram InformationData
    tmp[0]=0x9F;
    tmp[1]=0x27;
//	memcpy(tmp+2, EMVL2_App.DE_9F27, EMVL2_App.DE_9F27[0]+1);
//	Hex2Ascii(tmp, pKoces->DE_9F27, EMVL2_App.DE_9F27[0]+3);
    nLen += 8;

    // Issuer Application Data  (IAD)
    tmp[0]=0x9F;
    tmp[1]=0x10;
//	memcpy(tmp+2, EMVL2_App.DE_9F10, EMVL2_App.DE_9F10[0]+1);
//	Hex2Ascii(tmp, pKoces->DE_9F10, EMVL2_App.DE_9F10[0]+3);
    nLen += 70;

    // Unpredictable Number
    tmp[0]=0x9F;
    tmp[1]=0x37;
//	memcpy(tmp+2, EMVL2_App.DE_9F37, EMVL2_App.DE_9F37[0]+1);
//	Hex2Ascii(tmp, pKoces->DE_9F37, EMVL2_App.DE_9F37[0]+3);
    nLen += 14;

    // Application Transaction Counter (ATC)
    tmp[0]=0x9F;
    tmp[1]=0x36;
//	memcpy(tmp+2, EMVL2_App.DE_9F36, EMVL2_App.DE_9F36[0]+1);
//	Hex2Ascii(tmp, pKoces->DE_9F36, EMVL2_App.DE_9F37[0]+3);
    nLen += 10;

    // Terminal Verification Result (TVR)
    tmp[0]=0x95;
//	memcpy(tmp+1, EMVL2_App.DE_95, EMVL2_App.DE_95[0]+1);
//	Hex2Ascii(tmp, pKoces->DE_95, EMVL2_App.DE_95[0]+2);
    nLen += 14;

    // Transaction Date
    tmp[0]=0x9A;
//	memcpy(tmp+1, EMVL2_App.DE_9A, EMVL2_App.DE_9A[0]+1);
//	Hex2Ascii(tmp, pKoces->DE_9A, EMVL2_App.DE_9A[0]+2);
    nLen += 10;

    // Transaction Type
    tmp[0]=0x9C;
//	memcpy(tmp+1, EMVL2_App.DE_9C, EMVL2_App.DE_9C[0]+1);
//	Hex2Ascii(tmp, pKoces->DE_9C, EMVL2_App.DE_9C[0]+2);
    nLen += 6;

    // Transaction Amount
    tmp[0]=0x9F;
    tmp[1]=0x02;
//	memcpy(tmp+2, EMVL2_App.DE_9F02, EMVL2_App.DE_9F02[0]+1);
//	Hex2Ascii(tmp, pKoces->DE_9F02, EMVL2_App.DE_9F02[0]+3);
    nLen += 18;

    // Transaction Currency Code
    tmp[0]=0x5F;
    tmp[1]=0x2A;
//	memcpy(tmp+2, EMVL2_App.DE_5F2A, EMVL2_App.DE_5F2A[0]+1);
//	Hex2Ascii(tmp, pKoces->DE_5F2A, EMVL2_App.DE_5F2A[0]+3);
    nLen += 10;

    // Application Interchange Profile(AIP)
    tmp[0]=0x82;
//	memcpy(tmp+1, EMVL2_App.DE_82, EMVL2_App.DE_82[0]+1);
//	Hex2Ascii(tmp, pKoces->DE_82, EMVL2_App.DE_82[0]+2);
    nLen += 8;

    // Terminal Country Code
    tmp[0]=0x9F;
    tmp[1]=0x1A;
//	memcpy(tmp+2, EMVL2_App.DE_9F1A, EMVL2_App.DE_9F1A[0]+1);
//	Hex2Ascii(tmp, pKoces->DE_9F1A, EMVL2_App.DE_9F1A[0]+3);
    nLen += 10;

    // Amount Other
    tmp[0]=0x9F;
    tmp[1]=0x03;
//	memcpy(tmp+2, EMVL2_App.DE_9F03, EMVL2_App.DE_9F03[0]+1);
//	Hex2Ascii(tmp, pKoces->DE_9F03, EMVL2_App.DE_9F03[0]+3);
    nLen += 18;

    // Cardholder Verification Method(CVM) Results
    tmp[0]=0x9F;
    tmp[1]=0x34;
//	memcpy(tmp+2, EMVL2_App.DE_9F34, EMVL2_App.DE_9F34[0]+1);
//	Hex2Ascii(tmp, pKoces->DE_9F34, EMVL2_App.DE_9F34[0]+3);
    nLen += 12;

    // Terminal Capabilities
    tmp[0]=0x9F;
    tmp[1]=0x33;
//	memcpy(tmp+2, EMVL2_App.DE_9F33, EMVL2_App.DE_9F33[0]+1);
//	Hex2Ascii(tmp, pKoces->DE_9F33, EMVL2_App.DE_9F33[0]+3);
    nLen += 12;

    // Terminal Type
    tmp[0]=0x9F;
    tmp[1]=0x35;
//	memcpy(tmp+2, EMVL2_App.DE_9F35, EMVL2_App.DE_9F35[0]+1);
//	Hex2Ascii(tmp, pKoces->DE_9F35, EMVL2_App.DE_9F35[0]+3);
    nLen += 8;

    // Interface Device(IFD) Serial Number
    tmp[0]=0x9F;
    tmp[1]=0x1E;
//	memcpy(tmp+2, EMVL2_App.DE_9F1E, EMVL2_App.DE_9F1E[0]+1);
//	Hex2Ascii(tmp, pKoces->DE_9F1E, EMVL2_App.DE_9F1E[0]+3);
    nLen += 22;

    // Transaction Category Code
    tmp[0]=0x9F;
    tmp[1]=0x53;
//	//memcpy(tmp+2, EMVL2_App.DE_9F53, 2);
//	memcpy(tmp+2, "\x01\x00", 2);
//	Hex2Ascii(tmp, pKoces->DE_9F53, 4);
    nLen += 8;

    // Dedicated File Name
    tmp[0]=0x84;
//	memcpy(tmp+1, EMVL2_App.DE_84, EMVL2_App.DE_84[0]+1);
//	Hex2Ascii(tmp, pKoces->DE_84, EMVL2_App.DE_84[0]+2);
    nLen += 36;

    // Terminal ApplicationVersion Number
    tmp[0]=0x9F;
    tmp[1]=0x09;
//	memcpy(tmp+2, EMVL2_App.DE_9F09, EMVL2_App.DE_9F09[0]+1);
//	Hex2Ascii(tmp, pKoces->DE_9F09, EMVL2_App.DE_9F09[0]+3);
    nLen += 10;

    // Transaction Sequence Counter
    tmp[0]=0x9F;
    tmp[1]=0x41;
//	memcpy(tmp+2, EMVL2_App.DE_9F41, EMVL2_App.DE_9F41[0]+1);
//	Hex2Ascii(tmp, pKoces->DE_9F41, EMVL2_App.DE_9F41[0]+3);
    nLen += 14;

    // Fallback 사유
    // 01 : Chip 전원을 넣었으나 응답이 없는 경우
    // 02 : 상호지원 application이 없을 경우
    // 03 : 칩데이터 읽기 실패
    // 04 : Mandatory 데이터 미포함
    // 05 : CVM 커맨드 응답실패
    // 06 : EMV 커맨드 잘못설정
    // 07 : 터미널 오작동
    Hex_To_Ascii(nReason , pKoces->fallback_reason);
    nLen+=2;

    // 8A FS
    VAN_BasicReq.AddedInfo[nLen++] = FS;
    // 8B an 판매된 Column
    BCD2Char((uchar*)&g_VmAmountColumn.Col, (uchar*)&VAN_BasicReq.AddedInfo[nLen], 2);	// an 현재 컬럼.
    nLen+=2;
    // 8D
    VAN_BasicReq.AddedInfo[nLen++] = 0x03;

    // 단말기번호 ~ ETX까지의 길이
    pdata = VAN_BasicReq.TID;
    for(i=0; i<sizeof(NanoVANBasicReq) ; i++) {
        if(pdata[i]==0x03) break;
    }
    sprintf((char*)buf,  "%04d", i+1);
    memcpy((char*)VAN_BasicReq.MsgLen, buf, 4);

    // CRC
    crc = (unsigned short)CRC_Update((unsigned char*)&VAN_BasicReq, Char2Int(VAN_BasicReq.MsgLen, 0)+5);
    Int2Byte(crc, &VAN_BasicReq.AddedInfo[nLen]);

    PRINTHEX((unsigned char*)&VAN_BasicReq, Char2Int(VAN_BasicReq.MsgLen, 0)+5, "EMV REQ %d", Char2Int(VAN_BasicReq.MsgLen, 0)+5);

    g_NanoVANBasicReq = VAN_BasicReq;

    memset(tmp, 0x00, sizeof(tmp));
    memset(tmp, 0xFF,sizeof(tmp));
    memset(tmp, 0x00, sizeof(tmp));

    memset(buf, 0x00, sizeof(buf));
    memset(buf, 0xFF,sizeof(buf));
    memset(buf, 0x00, sizeof(buf));

    memset(track2, 0x00, sizeof(track2));
    memset(track2, 0xFF, sizeof(track2));
    memset(track2, 0x00, sizeof(track2));

    PRINTHEX(track2, 64, "t2 clr address : 0x%08X", track2);

    switch(nType) {
    case 0x08 : // 망상취소
        UBC_TmsBackUpProcess(-1);
        UBC_Backup(&VAN_BasicReq);
        break;
    case 0x02 : // 취소
    case 0x06 : // 조회
        case 0x01 :
        UBC_TmsBackUpProcess(nAmount);
        UBC_Backup(&VAN_BasicReq);

        UBC_ReadVanFlash(0);
        break;
    }
#endif
}

void koces_credit_request(int nType, unsigned long nAmount)
{
#if 0
    int i, nLen=0, offset, track2_len;
    char track2[128], tmp[128];
    char *pbuf;
    unsigned char *pdata, buf[256];
    unsigned char ksn[10];
    unsigned short crc;

    int ksn_len=0, enc_len;
    PKocesMSRReq pKoces;

    if(((nType==1) || (nType==6))  && UBC_LogCheck()) return;

    switch(nType) {
    case 0x08 : // 망상 취소
        if(m_nCraditType==1) {
            koces_fallback_request(nType, nAmount, 1);
            return;
        }
        break;
    case 0x02 :	// 취소
    case 0x06 :	// 조회
    default :		// 승인
        if(g_CraditType==1) {
            koces_fallback_request(nType, nAmount, 1);
            return;
        }
        break;
    }

    memset((char*)&VAN_BasicReq, ' ', sizeof(NanoVANBasicReq));
    // 00
    VAN_BasicReq.stx = 0x02;
    // 01  an 4 단말기번호 - ETX 길이
    //sprintf((char*)VAN_BasicReq.MsgLen,                "%04d", );
    // 05  an 10 단말기 번호 "1234567890"
    sprintf((char*)VAN_BasicReq.TID,                   "%s",   g_VcpsInfo.TID);
    // 0F an 2 거래(업무) 구분
    sprintf((char*)VAN_BasicReq.TransactionClass,      "%s",   g_NanoTransaction[1]);
    // 11 an 2 전문 구분, 수집
    sprintf((char*)VAN_BasicReq.PacketClass,           "%s",   g_NanoPacket[nType]);
    // 13 an 1 입력매체 구분코드 , K:KEYIN, S:SWIPE, R:RF, I:IC, X:NON

    switch(nType) {
    case 0x08 : // 망상 취소
        switch(m_nCraditType) {
        case 1 :	VAN_BasicReq.TransactionType = 'S'; break;	// MSR
        case 2 :
        case 3 :	VAN_BasicReq.TransactionType = 'E'; break;	// KEY IN
        }
        // 14 an 5 단말기 거래일련 번호, 총 거래 Cnt
        sprintf((char*)VAN_BasicReq.TransactionCount,      "%05d", m_nNanoTotCnt);
        break;
    case 0x02 :
    case 0x06 :
    default :
        switch(g_CraditType) {
        case 1 :	VAN_BasicReq.TransactionType = 'S'; break;	// MSR
        case 2 :
        case 3 :	VAN_BasicReq.TransactionType = 'E'; break;	// KEY IN
        }
        // 14 an 5 단말기 거래일련 번호, 총 거래 Cnt
        sprintf((char*)VAN_BasicReq.TransactionCount,      "%05d", g_TmsLog.TotCnt);
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

    // Track II Data : Track2 정보는 3rd와 '=' 이후 전부 '*'로 마스킹해서 전송
    //ex) 유효 Track2 정보가  "9400123412341234=19121234123451234567"(37byte) 일 경우 "940012**********=1*******************   "(40byte)
    // 암호화 대상 정보는 유효 Track2 정보와 동일하며 암호화한 데이터가 48byte 보다 작을 경우 LEFT-JUSTIFIED, 스페이스 패딩해야 함.
    memset(track2, 0x20, sizeof(track2));
    if((nType==1) || (nType==2) || (nType==6))  {
        switch(g_CraditType) {
        case 1 :
        case 2 :
        case 3 :
            if((nType==2) && u_van_receipt.force)
            {
                memcpy(track2, u_van_receipt.CSN, strlen((const char*)u_van_receipt.CSN));
                g_MsrInfo.nCnt2 = strlen((const char*)u_van_receipt.CSN);
                memcpy(g_MsrInfo.nTrack2, track2, g_MsrInfo.nCnt2);
            }
            else {
                if(g_CraditType==1) memcpy(track2, g_MsrInfo.nTrack2, g_MsrInfo.nCnt2);
                else UBC_GetKbTrack2((unsigned char*)track2);
            }
            break;
        default :
            break;
        }
    }
    else {
        memcpy(track2, m_nMsrInfo.nTrack2, m_nMsrInfo.nCnt2);
    }

    track2_len = 40;

    memset(tmp, '*', track2_len);
    tmp[track2_len] = 0;

    memcpy(tmp, track2, 6);
    pbuf = strstr((char *)track2, "=");


    if(!pbuf) return;

    PRINTHEX((unsigned char *)track2, 64, "t2 in address : 0x%08X", track2);

    offset = (unsigned long)pbuf-(unsigned long)track2;

    if(offset==16) tmp[offset-4] = *(pbuf-4);
    tmp[offset-3] = *(pbuf-3);
    tmp[offset-2] = *(pbuf-2);
    tmp[offset-1] = *(pbuf-1);
    tmp[offset]    = *(pbuf);
    tmp[offset+5] = *(pbuf+5);

    memcpy(VAN_BasicReq.CSN, tmp, 40);

    // 4F FS
    VAN_BasicReq.FS2 = FS;
    // 50 an 2 할부 개월 수
    sprintf((char*)VAN_BasicReq.Installment,           "%02d", 0);
    // 52 FS
    VAN_BasicReq.FS3 = FS;
    // 53 an 10 승인금액
    switch(nType) {
    case 0x08 : // 망상 취소
        if(!m_nAmount) return;
        sprintf((char*)VAN_BasicReq.Amount,      			"%010d", m_nAmount);
        m_nAmount = 0;
        UBC_TrackBackupDataClear();
        break;
    case 0x02 :
    case 0x06 :
        sprintf((char*)VAN_BasicReq.Amount,      			"%010d", nAmount);
        m_nAmount = 0;
        break;
    default :
        sprintf((char*)VAN_BasicReq.Amount,      			"%010d", nAmount);
        UBC_SetTrack2Backup(nAmount);
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
        for(i=0; i<8; i++) {
            if(u_van_receipt.Date[i]>='0' && u_van_receipt.Date[i]<='9') VAN_BasicReq.ApproveDate[i] = u_van_receipt.Date[i];
            else VAN_BasicReq.ApproveDate[i]  = 0x20;
        }
        // 7E FS
        VAN_BasicReq.FS8 = FS;
        // 7F an 10 원거래 승인 번호
        for(i=0; i<10; i++) {
            if(u_van_receipt.ApproveNum[i]>='0' && u_van_receipt.ApproveNum[i]<='9') VAN_BasicReq.ApproveNum[i] = u_van_receipt.ApproveNum[i];
            else VAN_BasicReq.ApproveNum[i]   = 0x20;
        }
        break;
    case 0x06 : //
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
    nLen = 0;
    pKoces = (PKocesMSRReq)VAN_BasicReq.AddedInfo;

    memset(VAN_BasicReq.AddedInfo, 0x30, sizeof(VAN_BasicReq.AddedInfo));

    //memcpy(pKoces->preamble_num, "2410", 4);
    //nLen += 4;

    //memcpy(pKoces->cat_id, g_VmConfig.CATID, 10);
    //nLen += 10;

    memcpy(pKoces->auth_num, VM_GetKtcApprovalNumber(), 32);
    nLen += 32;

    BCD2Char(g_Today, pKoces->date_time, 7);
    nLen += 14;

    ksn_len = 10;
    if(nType==2)
    {
        // 암호화된 Track II Data
        memset(pKoces->crypto_track2, ' ', 96);
        nLen += 96;

        // KSN Copy.
        Hex2Ascii(koces_fk_info.ksn, pKoces->ksn, ksn_len);
        nLen += 20;
    }
    else {
        // 암호화된 Track II Data

        PRINTHEX(koces_fk_info.ksn, 10, "t2 ksn");

        memcpy(ksn, koces_fk_info.ksn, 10);
        memset(tmp, 0x20, sizeof(tmp));
        memcpy(tmp, track2, track2_len);
        KOCES_DUKPT_AES_Encrypt(koces_dukpt_info.hdata, 1024, buf, &enc_len, ksn, &ksn_len, tmp, 40, &koces_fk_info.fki);
        Hex2Ascii(buf, pKoces->crypto_track2, enc_len);
        nLen += 96;

        PRINTHEX(pKoces->crypto_track2, enc_len*2, "t2 cryto in address : 0x%08X", pKoces->crypto_track2);

        // KSN Copy.
        Hex2Ascii(ksn, pKoces->ksn, ksn_len);
        memcpy(koces_fk_info.ksn, ksn, sizeof(ksn));
        nLen += 20;

        PRINTHEX(koces_fk_info.ksn, 10, "t2 cryto ksn");
    }

    // Working Key Index
    Hex2Ascii("\00", pKoces->working_key_index, 1);
    nLen += 2;

    // 비밀번호
    memset(pKoces->password, ' ', 16);
    nLen += 16;

    // 8A FS
    VAN_BasicReq.AddedInfo[nLen++] = FS;
    // 8B an 판매된 Column
    BCD2Char((uchar*)&g_VmAmountColumn.Col, (uchar*)&VAN_BasicReq.AddedInfo[nLen], 2);	// an 현재 컬럼.
    nLen+=2;
    // 8D
    VAN_BasicReq.AddedInfo[nLen++] = 0x03;

    // 단말기번호 ~ ETX까지의 길이
    pdata = VAN_BasicReq.TID;
    for(i=0; i<sizeof(NanoVANBasicReq) ; i++) {
        if(pdata[i]==0x03) break;
    }
    sprintf((char*)buf,  "%04d", i+1);
    memcpy((char*)VAN_BasicReq.MsgLen, buf, 4);

    // CRC
    crc = (unsigned short)CRC_Update((unsigned char*)&VAN_BasicReq, Char2Int(VAN_BasicReq.MsgLen, 0)+5);
    Int2Byte(crc, &VAN_BasicReq.AddedInfo[nLen]);

    PRINTHEX((unsigned char*)&VAN_BasicReq, Char2Int(VAN_BasicReq.MsgLen, 0)+5, "\r\nKTC >> %d", Char2Int(VAN_BasicReq.MsgLen, 0)+5);

    memset(tmp, 0x00, sizeof(tmp));
    memset(tmp, 0xFF,sizeof(tmp));
    memset(tmp, 0x00, sizeof(tmp));

    memset(buf, 0x00, sizeof(buf));
    memset(buf, 0xFF,sizeof(buf));
    memset(buf, 0x00, sizeof(buf));

    memset(track2, 0x00, sizeof(track2));
    memset(track2, 0xFF, sizeof(track2));
    memset(track2, 0x00, sizeof(track2));

    PRINTHEX(track2, 64, "t2 clr address : 0x%08X", track2);

    switch(nType) {
    case 0x08 : // 망상취소
        UBC_TmsBackUpProcess(-1);
        UBC_Backup(&VAN_BasicReq);
        break;
    case 0x02 : // 취소
    case 0x06 : // 조회
        case 0x01 :
        UBC_TmsBackUpProcess(nAmount);
        UBC_Backup(&VAN_BasicReq);

        UBC_ReadVanFlash(0);
        break;
    }
#endif
}

void koces_credit_response(unsigned char *recvbuf, size_t recvlen)
{
#if 0
    unsigned char *pT2=&recvbuf[2];
    int i;

    PRINTHEX(recvbuf, recvlen, "ADD << %d", recvlen);
    // "123456******0000"
    for(i=6; i<12; i++) pT2[i] = '*';

    UBC_SetReceipt();
#endif
}

void koces_cash_request(int nType, unsigned long nAmount, char *pNumber)
{
#if 0
    int i, nLen=0, offset, track2_len=40;
    char track2[128], tmp[128];
    char *pbuf;
    unsigned char *pdata, buf[256];
    unsigned char ksn[10];
    unsigned short crc;

    size_t ksn_len=0, enc_len;
    PKocesMSRReq pKoces;

    memset(track2, 0x00, sizeof(track2));
    memset((char*)&VAN_BasicReq, ' ', sizeof(NanoVANBasicReq));
    // 00
    VAN_BasicReq.stx = 0x02;
    // 01  an 4 단말기번호 - ETX 길이
    //sprintf((char*)VAN_BasicReq.MsgLen,                "%04d", );
    // 05  an 10 단말기 번호 "1234567890"
    sprintf((char*)VAN_BasicReq.TID,                   "%s",   g_VcpsInfo.TID);
    // 0F an 2 거래(업무) 구분
    sprintf((char*)VAN_BasicReq.TransactionClass,      "%s",   g_NanoTransaction[2]);
    // 11 an 2 전문 구분, 수집
    sprintf((char*)VAN_BasicReq.PacketClass,           "%s",   g_NanoPacket[nType]);
    // 13 an 1 입력매체 구분코드 , K:KEYIN, S:SWIPE, R:RF, I:IC, X:NON
    VAN_BasicReq.TransactionType = 'K';
    // 14 an 5 단말기 거래일련 번호, 총 거래 Cnt
    if(nType==1)
        sprintf((char*)VAN_BasicReq.TransactionCount,      "%05d", g_TmsLog.TotCnt);
    else
        sprintf((char*)VAN_BasicReq.TransactionCount,      "%05d", m_nNanoTotCnt);
    // 19 an 2 전문 버젼
    sprintf((char*)VAN_BasicReq.PacketVersion,         "%s",   "T1");
    // 1B an 1 전문 암호화 여부 안함
    VAN_BasicReq.Encryption = '0';
    // 1C an 10 예비용
    //sprintf((char*)VAN_BasicReq.RFU1,                  "%010d", ' ');
    // 26 FS																//
    VAN_BasicReq.FS1 = FS;
    // 27 an 40 인식 카드번호

    // 카드일련번호 : "15442020****123456=",  "94100000****0000=********************"( 앞 8자리 이후 4개 정보와 '=' 이후 정보를 '*'로 설정)
    if(nType==1) {
        strcpy(track2, pNumber);
        strcpy(tmp, track2);

        switch(strlen(track2)) {
        case 11 : // 휴대전화번호 : "010****1234"
            for(i=3 ; i<7; i++) tmp[i]='*';
            break;
        case 10 :
            if(track2[0] == '0') {
                // 휴대전화번호 : "019****123"
                for(i=3 ; i<7; i++) tmp[i]='*';
                break;
            }
            // 사업자등록번호 : "12****1234"
            //for(i=2 ; i<6; i++) tmp[i]='*';
            //break;
        case 13 : // 주민등록번호 : "710011*******"
            //for(i=6 ; i<14; i++) tmp[i]='*';
            //break;
        default : // 자동 발급
            strcpy(track2, "0100001234");
            strcpy(tmp, "010****234");
            break;
        }

        g_MsrInfo.nCnt2 = strlen(track2);
        memcpy(g_MsrInfo.nTrack2, track2, strlen(track2));
    }
    else {
        memcpy(track2, m_nMsrInfo.nTrack2, m_nMsrInfo.nCnt2);
        memcpy(tmp, m_nMsrInfo.nTrack2, m_nMsrInfo.nCnt2);
        for(i=3 ; i<7; i++) tmp[i]='*';
    }

    memcpy(VAN_BasicReq.CSN, tmp, strlen(track2));


    // 4F FS
    VAN_BasicReq.FS2 = FS;
    // 50 an 2 할부 개월 수, 01 : 소득공제 02 : 사업자 지출증빙 .
    sprintf((char*)VAN_BasicReq.Installment,           "%02d", 1);
    // 52 FS
    VAN_BasicReq.FS3 = FS;
    // 53 an 10 승인금액
    switch(nType) {
    case 0x02 :
    case 0x08 : // 망상 취소
        if(!m_nAmount) return;
        sprintf((char*)VAN_BasicReq.Amount,      			"%010d", m_nAmount);
        m_nAmount = 0;
        UBC_TrackBackupDataClear();
        break;
    default :
        sprintf((char*)VAN_BasicReq.Amount,      			"%010d", nAmount);
        UBC_SetTrack2Backup(nAmount);
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
        for(i=0; i<8; i++) VAN_BasicReq.ApproveDate[i] = VAN_BasicRes.Date[i];
        // 7E FS
        VAN_BasicReq.FS8 = FS;
        // 7F an 10 원거래 승인 번호
        for(i=0; i<10; i++)VAN_BasicReq.ApproveNum[i]  = VAN_BasicRes.ApproveNum[i];
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
    nLen = 0;
    pKoces = (PKocesMSRReq)VAN_BasicReq.AddedInfo;

    memset(VAN_BasicReq.AddedInfo, 0x30, sizeof(VAN_BasicReq.AddedInfo));

    //memcpy(pKoces->preamble_num, "2410", 4);
    //nLen += 4;

    //memcpy(pKoces->cat_id, g_VmConfig.CATID, 10);
    //nLen += 10;

    memcpy(pKoces->auth_num, VM_GetKtcApprovalNumber(), 32);
    nLen += 32;

    BCD2Char(g_Today, pKoces->date_time, 7);
    nLen += 14;

    ksn_len = 10;
    if(nType==2)
    {
        memset(pKoces->crypto_track2, ' ', 96);
        nLen += 96;

        // KSN Copy.
        Hex2Ascii(koces_fk_info.ksn, pKoces->ksn, ksn_len);
        nLen += 20;
    }
    else
    if(nType==8)
    {
        // 암호화된 Track II Data
        // KSN Copy.
        memcpy(pKoces->crypto_track2, koces_approval_bakup, 96);
        nLen += 96;

        memcpy(pKoces->ksn, koces_approval_bakup+96, 20);
        nLen += 20;
    }
    else {
        // 암호화된 Track II Data
        memcpy(ksn, koces_fk_info.ksn, sizeof(ksn));

        memset(tmp, 0x20, sizeof(tmp));
        memcpy(tmp, track2, track2_len);
        KOCES_DUKPT_AES_Encrypt(koces_dukpt_info.hdata, 1024, buf, &enc_len, ksn, &ksn_len, tmp, 40, &koces_fk_info.fki);
        Hex2Ascii(buf, pKoces->crypto_track2, enc_len);
        nLen += 96;

        memcpy(koces_approval_bakup, pKoces->crypto_track2, 96);

        // KSN Copy.
        Hex2Ascii(ksn, pKoces->ksn, ksn_len);
        memcpy(koces_fk_info.ksn, ksn, sizeof(ksn));
        nLen += 20;

        memcpy(koces_approval_bakup+96, pKoces->ksn, 20);
    }

    // Working Key Index
    Hex2Ascii("\00", pKoces->working_key_index, 1);
    nLen += 2;

    // 비밀번호
    memset(pKoces->password, ' ', 16);
    nLen += 16;

    // 8A FS
    VAN_BasicReq.AddedInfo[nLen++] = FS;
    // 8B an 판매된 Column
    BCD2Char((uchar*)&g_VmAmountColumn.Col, (uchar*)&VAN_BasicReq.AddedInfo[nLen], 2);	// an 현재 컬럼.
    nLen+=2;
    // 8D
    VAN_BasicReq.AddedInfo[nLen++] = 0x03;

    // 단말기번호 ~ ETX까지의 길이
    pdata = VAN_BasicReq.TID;
    for(i=0; i<sizeof(NanoVANBasicReq) ; i++) {
        if(pdata[i]==0x03) break;
    }
    sprintf((char*)buf,  "%04d", i+1);
    memcpy((char*)VAN_BasicReq.MsgLen, buf, 4);

    // CRC
    crc = (unsigned short)CRC_Update((unsigned char*)&VAN_BasicReq, Char2Int(VAN_BasicReq.MsgLen, 0)+5);
    Int2Byte(crc, &VAN_BasicReq.AddedInfo[nLen]);

    PRINTHEX(DEBUG_PORT, (unsigned char*)&VAN_BasicReq, Char2Int(VAN_BasicReq.MsgLen, 0)+5, "CASH REQ %d", Char2Int(VAN_BasicReq.MsgLen, 0)+5);

    switch(nType) {
    case 0x02 : // 취소
    case 0x08 : // 망상취소
        UBC_TmsBackUpProcess(-1);
        UBC_Backup(&VAN_BasicReq);
        break;
        case 0x01 :
        UBC_TmsBackUpProcess(nAmount);
        UBC_Backup(&VAN_BasicReq);
        break;
    }
#endif
}

void koces_cash_response(unsigned char *recvbuf, size_t recvlen)
{
//	play_wav("sound/transaction_complete.wav");
}


//------------------------------------------------------------------------------------------------------------------
//koces_dupkt_response
//KOCES_DUKPT_Make_Self_Protect_HASH

int koces_hdata_comp_self_protect(void)
{
    int ret=-1;
    FILE *fp;
    unsigned char digest[32] = {0x00, };

    size_t in_hash_len = 32;
    fp = fopen(KTC_DUKPT_PATH, "r");
    if(fp!=NULL) {
        fread(&koces_dukpt_info, sizeof(s_koces_dukpt_info), 1, fp);
        fclose(fp);
    }
    ktc_enc_hash_len = koces_dukpt_info.enc_hash_len;
    if(ktc_enc_hash_len>32)
    {
        ret = -2;
        PRINTF("Failure!!! : (%d)\r\n", ret);
        goto f_end;
    }

    memcpy(ktc_enc_hash, koces_dukpt_info.enc_hash, ktc_enc_hash_len);

    /* 정상 데이터를 Sha-256으로 Hash 값을 추출 - 자체보호 HASH 추출 */
    PRINTF("========================== KOCES_HDATA_HASH_SHA256(%d) ===========================\r\n",ktc_hide_data_len);
    //usleep(10000);
    KOCES_CRYPT_HASH_SHA256((const unsigned char *)ktc_hide_data, ktc_hide_data_len, digest);
    //usleep(10000);
    PRINTHEX(digest, 32, "Hash Data(OUT)");
    PRINTF("================================================================================\n\r\n");

    in_hash_len = sizeof(digest);
    memcpy(ktc_in_hash, digest, in_hash_len);

    /* 입력(ktc_in_hash)데이타와 이전의 암호화된 데이타를 비교 - 자체보호 검증용으로 사용 */
    PRINTF("===================== KOCES_HDATA_Comp_Self_Protect_HASH() =====================\r\n");
    ret = KOCES_DUKPT_Comp_Self_Protect_HASH((const unsigned char*)ktc_hide_data, ktc_hide_data_len, ktc_enc_hash, (int)ktc_enc_hash_len, ktc_in_hash, (int)in_hash_len);
    PRINTHEX(ktc_in_hash, in_hash_len, "in_hash(IN) : (%d)", in_hash_len);
    PRINTHEX(ktc_enc_hash, ktc_enc_hash_len, "enc_hash(IN)");
    if(ret < 0)
    {
        PRINTF("Failure!!! : (%d)\r\n", ret);
        goto f_end;
    }

    PRINTF("Success!!!\r\n");
    PRINTF("================================================================================\n\r\n");
    /*******************************************************************************************/

f_end :
    if(ret<0) {
        koces_dukpt_info.key_injection = UBCN_DUKPT_INIT;
        koces_dukpt_info.hdata_used   = UBCN_DUKPT_INIT;
        fp = fopen(KTC_DUKPT_PATH, "wb");
        fwrite(&koces_dukpt_info, sizeof(s_koces_dukpt_info), 1, fp);
        fclose(fp);
    }

    memset(digest,0x00,sizeof(digest));
    return ret;
}

int koces_fk_make_self_protect(void)
{
    int ret = -1;
    unsigned char digest[32] = {0x00, };
    size_t in_hash_len = 32;

    ktc_data_len = sizeof(KOCES_FKINFO);
    memcpy(ktc_data, (unsigned char *)&koces_fk_info.fki, (size_t)ktc_data_len);

     /* 정상 데이터를 Sha-256으로 Hash 값을 추출 - 자체보호 HASH 추출 */
    PRINTF("=========================== KOCES_FK_HASH_SHA256(%d) ===========================\r\n",ktc_data_len);
    //usleep(10000);
    KOCES_CRYPT_HASH_SHA256((const unsigned char*)ktc_data, ktc_data_len, digest);
    //usleep(10000);
    PRINTHEX(digest, 32, "Hash Data(OUT)");

    in_hash_len = sizeof(digest);
    memcpy(ktc_in_hash, digest, in_hash_len);

    /* 입력(ktc_in_hash)데이타를 SKey로 암호화하여 줌 - 자체보호 HASH 암호화 */
    PRINTF("====================== KOCES_FK_Make_Salf_Protect_HASH() ======================\r\n");
    ret = KOCES_DUKPT_Make_Self_Protect_HASH((const unsigned char*)ktc_hide_data, ktc_hide_data_len, ktc_enc_hash, (int *)&ktc_enc_hash_len, ktc_in_hash, (int)in_hash_len);

    if(ret < 0)
    {
        PRINTF(" Failure!!! : (%d)\r\n", ret);
        goto f_end;
    }
    PRINTHEX(ktc_in_hash, in_hash_len, "ktc_in_hash(IN) %d", ret);
    PRINTHEX(ktc_enc_hash, ktc_enc_hash_len, "ktc_enc_hash(OUT)");
    PRINTF("================================================================================\n\r\n");

    // 단말기 내 비휘발성저장소
    koces_fk_info.enc_hash_len = ktc_enc_hash_len;
    memcpy(koces_fk_info.enc_hash, ktc_enc_hash, ktc_enc_hash_len);

f_end :

    return ret;
}

int koces_fk_comp_self_protect(void)
{
    FILE *fp;
    int ret = -1;    
    unsigned char digest[32] = {0x00, };
    size_t in_hash_len = 32;

    fp = fopen(KTC_FK_PATH, "r");
    if(fp!=NULL) {
        fread(&koces_fk_info, sizeof(s_koces_fk_info), 1, fp);
        fclose(fp);
    }

    ktc_data_len = sizeof(KOCES_FKINFO);
    memcpy(ktc_data, (unsigned char *)&koces_fk_info.fki, (size_t)ktc_data_len);

    ktc_enc_hash_len = koces_fk_info.enc_hash_len;

    if(ktc_enc_hash_len > 32) {
        ret = -2;
        PRINTF("Failure!!! : (%d)\r\n", ret);
        goto f_end;
    }

    memcpy(ktc_enc_hash, koces_fk_info.enc_hash, ktc_enc_hash_len);

    /* 정상 데이터를 Sha-256으로 Hash 값을 추출 - 자체보호 HASH 추출 */
    PRINTF("============================ KOCES_FK_HASH_SHA256(%d) =============================\r\n",ktc_data_len);
    //usleep(10000);
    KOCES_CRYPT_HASH_SHA256((const unsigned char *)ktc_data, ktc_data_len, digest);
    //usleep(10000);
    PRINTHEX(digest, 32, "Hash Data(OUT)");

    in_hash_len = sizeof(digest);
    memcpy(ktc_in_hash, digest, in_hash_len);

    /* 입력(ktc_in_hash)데이타와 이전의 암호화된 데이타를 비교 - 자체보호 검증용으로 사용 */
    PRINTF("====================== KOCES_FK_Comp_Self_Protect_HASH() ======================\r\n");
    ret = KOCES_DUKPT_Comp_Self_Protect_HASH((const unsigned char*)ktc_hide_data, ktc_hide_data_len, ktc_enc_hash, (int)ktc_enc_hash_len, ktc_in_hash, (int)in_hash_len);
    PRINTHEX(ktc_in_hash, in_hash_len, "in_hash(IN) : (%d)", in_hash_len);
    PRINTHEX(ktc_enc_hash, ktc_enc_hash_len, "enc_hash(IN) : (%d)", ktc_enc_hash_len);

    if(ret < 0)
    {
        PRINTF("Failure!!! : (%d)\r\n", ret);
        goto f_end;
    }

    PRINTF(" Success!!!\r\n");
    PRINTF("================================================================================\n\r\n");
    /*******************************************************************************************/
f_end :
    memset(digest,0x00,sizeof(digest));
    return ret;
}

int koces_bin_make_self_protect(void)
{
    FILE *fp;
    int ret=-1;
    unsigned char digest[32];
    unsigned char *data;
    size_t data_len;
    int in_hash_len = 32;

    fp = fopen("/app/maru", "r");
    fseek(fp,0,SEEK_END);       // 파일의 끝으로 파일 position 이동
    data_len = (size_t)ftell(fp);    	// 현재 positon 출력
    rewind(fp);					// 스트림을 처음으로 되돌린다.
    data = malloc(data_len+1);
    fread(data, 1, data_len, fp);

    /* 정상 데이터를 Sha-256으로 Hash 값을 추출 - 자체보호 HASH 추출 */
    PRINTF("\r\n========================== NEWZEN_Make_HASH_SHA256(%d)  ===========================\r\n",data_len);
    //usleep(10000);
    KOCES_CRYPT_HASH_SHA256(data, (int)data_len, digest);
    //usleep(10000);

    free(data);
    fclose(fp);

    in_hash_len = sizeof(digest);
    memcpy(ktc_in_hash, digest, (size_t)in_hash_len);
    PRINTHEX(ktc_in_hash, 32, "Hash Data(IN)");
    /* 입력(ktc_in_hash)데이타와 이전의 암호화된 데이타를 비교 - 자체보호 검증용으로 사용 */
    PRINTF("======================== NEWZEN_Make_Self_Protect_HASH() ========================\r\n");
    ret = KOCES_DUKPT_Make_Self_Protect_HASH((const unsigned char *)ktc_hide_data, ktc_hide_data_len, ktc_enc_hash, (int *)&ktc_enc_hash_len, ktc_in_hash, in_hash_len);
    PRINTHEX(ktc_in_hash, (unsigned long)in_hash_len, "in_hash(IN)(%d)", in_hash_len);
    PRINTHEX(ktc_enc_hash, ktc_enc_hash_len, "enc_hash(IN) %d", ret);

    if(ret < 0)
    {
        PRINTF("Failure!!! : (%d)\r\n", ret);
        goto f_end;
    }

    PRINTF("ret(OUT) : (%d)\r\n", ret);
    PRINTF(" Success!!!\r\n");

    fp = fopen(KTC_MARU_PATH, "w");
    if(fp!=NULL) {
        fwrite(ktc_enc_hash, 1, ktc_enc_hash_len, fp);
        fclose(fp);
    }

f_end :
    PRINTF("================================================================================\n\r\n");
    return ret;
}

int koces_bin_comp_self_protect(void)
{
    FILE *fp;
    int ret=-1;
    unsigned char digest[32];
    unsigned char *data;
    size_t data_len;
    int in_hash_len = 32;

    fp = fopen(KTC_MARU_PATH, "r");
    if(fp!=NULL) {
        ktc_enc_hash_len = sizeof(digest);
        fread(ktc_enc_hash, 1, ktc_enc_hash_len, fp);
        fclose(fp);
    }
    else {
        PRINTF("Failure!!! : (%d)\n", ret);
        goto f_end;
    }

    fp = fopen("/app/maru", "r");
    fseek(fp,0,SEEK_END);				// 파일의 끝으로 파일 position 이동
    data_len = (size_t)ftell(fp);    			// 현재 positon 출력
    rewind(fp);							// 스트림을 처음으로 되돌린다.

    data = malloc(data_len+1);
    fread(data, 1, data_len, fp);

    PRINTF("========================== NEWZEN_Comp_HASH_SHA256(%d)  ===========================\r\n",data_len);
    //usleep(10000);
    KOCES_CRYPT_HASH_SHA256(data, (int)data_len, digest);
    //usleep(10000);

    free(data);
    fclose(fp);

    in_hash_len = sizeof(digest);
    memcpy(ktc_in_hash, digest, (size_t)in_hash_len);
    PRINTHEX(digest, 32, "Hash Data(OUT)");
    /* 입력(ktc_in_hash)데이타와 이전의 암호화된 데이타를 비교 - 자체보호 검증용으로 사용 */
    PRINTF("======================= NEWZEN_Comp_Self_Protect_HASH() =========================\r\n");
    ret = KOCES_DUKPT_Comp_Self_Protect_HASH((const unsigned char*)ktc_hide_data, ktc_hide_data_len, ktc_enc_hash, (int)ktc_enc_hash_len, ktc_in_hash, in_hash_len);
    PRINTHEX(ktc_in_hash, (unsigned long)in_hash_len, "in_hash(IN) : (%d)", in_hash_len);
    PRINTHEX(ktc_enc_hash, ktc_enc_hash_len, "enc_hash(IN)");

    if(ret < 0)
    {
        PRINTF("Failure!!! : (%d)\r\n", ret);
        goto f_end;
    }
    PRINTF(" Success!!!\r\n");

f_end :
    PRINTF("=================================================================================\n\r\n");
    memset(digest,0x00,sizeof(digest));
    return ret;
}

int koces_libdcli_make_self_protect(void)
{
    FILE *fp;
    int ret;
    unsigned char digest[32];
    unsigned char *data;
    size_t data_len;
    size_t in_hash_len = 32;

    fp = fopen("/app/lib/libDCli_v3.0.10.5.so", "rb");

    fseek(fp,0,SEEK_END);					// 파일의 끝으로 파일 position 이동
    data_len = (size_t)ftell(fp);    					// 현재 positon 출력
    rewind(fp);							// 스트림을 처음으로 되돌린다.

    data = malloc(data_len+1);

    fread(data, 1, data_len, fp);

    /* 정상 데이터를 Sha-256으로 Hash 값을 추출 - 자체보호 HASH 추출 */
    PRINTF("\r\n========================== libDCli_HASH_SHA256()  ===========================\r\n");

    KOCES_CRYPT_HASH_SHA256(data, (int)data_len, digest);

    free(data);
    fclose (fp);

    in_hash_len = sizeof(digest);
    memcpy(ktc_in_hash, digest, in_hash_len);

    /* 입력(ktc_in_hash)데이타와 이전의 암호화된 데이타를 비교 - 자체보호 검증용으로 사용 */
    PRINTF("===================== libDCli_Make_Self_Protect_HASH() =====================\n");
    ret = KOCES_DUKPT_Make_Self_Protect_HASH((const unsigned char *)ktc_hide_data, ktc_hide_data_len, ktc_enc_hash, (int *)&ktc_enc_hash_len, ktc_in_hash, (int)in_hash_len);
    HEXDUMP(ktc_enc_hash, ktc_enc_hash_len, "enc_hash(IN) : (%d)", ktc_enc_hash_len);
    HEXDUMP(ktc_in_hash, in_hash_len, "in_hash(IN)(%d)", in_hash_len);

    if(ret < 0)
    {
        PRINTF("Failure!!! : (%d)\n", ret);
        goto f_end;
    }

    PRINTF("ret(OUT) : (%d)\n", ret);
    PRINTF("Success!!!\n");

    fp = fopen(KEY_LIBDCLI_PATH, "wb");
    if(fp!=NULL) {
        fwrite(ktc_enc_hash, 1, ktc_enc_hash_len, fp);
        fclose(fp);
    }

f_end :
    PRINTF("================================================================================\n\n");

    return ret;
}

int koces_libdcli_comp_self_protect(void)
{
    FILE *fp;
    int ret=-1;
    unsigned char digest[32];
    unsigned char *data;
    size_t data_len;
    size_t in_hash_len = 32;

    fp = fopen(KEY_LIBDCLI_PATH, "rb");
    if(fp!=NULL) {
        ktc_enc_hash_len = sizeof(digest);
        fread(ktc_enc_hash, 1, ktc_enc_hash_len, fp);
        fclose(fp);
    }
    else {
        PRINTF("Failure!!! : (%d)\n", ret);
        goto f_end;
    }

    fp = fopen("/app/lib/libDCli_v3.0.10.5.so", "rb");

    fseek(fp,0,SEEK_END);				// 파일의 끝으로 파일 position 이동
    data_len = (size_t)ftell(fp);    			// 현재 positon 출력
    rewind(fp);							// 스트림을 처음으로 되돌린다.

    data = malloc(data_len+1);
    fread(data, 1, data_len, fp);

    /* 정상 데이터를 Sha-256으로 Hash 값을 추출 - 자체보호 HASH 추출 */
    PRINTF("============================ libDCli_HASH_SHA256()  =============================\r\n");

    ret = KOCES_CRYPT_HASH_SHA256(data, (int)data_len, digest);
    free(data);
    fclose (fp);

    if(ret < 0)
    {
        PRINTF("Failure!!! : (%d)\n", ret);
        goto f_end;
    }

    in_hash_len = sizeof(digest);
    memcpy(ktc_in_hash, digest, in_hash_len);
    HEXDUMP(ktc_in_hash, 32, "Hash Data(OUT)");
    PRINTF("Success!!!\n");
    PRINTF("================================================================================\n\n");

    /* 입력(ktc_in_hash)데이타와 이전의 암호화된 데이타를 비교 - 자체보호 검증용으로 사용 */
    PRINTF("======================= libDCli_Comp_Self_Protect_HASH() =======================\n");
    ret = KOCES_DUKPT_Comp_Self_Protect_HASH((const unsigned char *)ktc_hide_data, ktc_hide_data_len, ktc_enc_hash, (int)ktc_enc_hash_len, ktc_in_hash, (int)in_hash_len);
    HEXDUMP(ktc_enc_hash, ktc_enc_hash_len, "enc_hash(IN) : (%d)", ktc_enc_hash_len);
    HEXDUMP(ktc_in_hash, in_hash_len, "in_hash(IN) : (%d)", in_hash_len);

    if(ret < 0)
    {
        PRINTF("Failure!!! : (%d)\n", ret);
        goto f_end;
    }
    //HEXDUMP(digest, 32, "Hash Data(OUT)");
    PRINTF("Success!!!\n");


f_end :
    PRINTF("================================================================================\n\n");

    return ret;
}

int koces_libdcrypt_make_self_protect(void)
{
    FILE *fp;
    int ret;
    unsigned char digest[32];
    unsigned char *data;
    size_t data_len;
    size_t in_hash_len = 32;

    fp = fopen("/app/lib/libDCrypt_v3.0.10.5.so", "rb");

    fseek(fp,0,SEEK_END);					// 파일의 끝으로 파일 position 이동
    data_len = (size_t)ftell(fp);    					// 현재 positon 출력
    rewind(fp);							// 스트림을 처음으로 되돌린다.

    data = malloc(data_len+1);

    fread(data, 1, data_len, fp);

    /* 정상 데이터를 Sha-256으로 Hash 값을 추출 - 자체보호 HASH 추출 */
    PRINTF("\r\n========================== libDCrypt_HASH_SHA256()  ===========================\r\n");

    KOCES_CRYPT_HASH_SHA256(data, (int)data_len, digest);

    free(data);
    fclose (fp);

    in_hash_len = sizeof(digest);
    memcpy(ktc_in_hash, digest, in_hash_len);

    /* 입력(ktc_in_hash)데이타와 이전의 암호화된 데이타를 비교 - 자체보호 검증용으로 사용 */
    PRINTF("===================== libDCrypt_Make_Self_Protect_HASH() =====================\n");
    ret = KOCES_DUKPT_Make_Self_Protect_HASH((const unsigned char*)ktc_hide_data, ktc_hide_data_len, ktc_enc_hash, (int *)&ktc_enc_hash_len, ktc_in_hash, (int)in_hash_len);
    HEXDUMP(ktc_enc_hash, ktc_enc_hash_len, "enc_hash(IN) : (%d)", ktc_enc_hash_len);
    HEXDUMP(ktc_in_hash, in_hash_len, "in_hash(IN)(%d)", in_hash_len);

    if(ret < 0)
    {
        PRINTF("Failure!!! : (%d)\n", ret);
        goto f_end;
    }

    PRINTF("ret(OUT) : (%d)\n", ret);
    PRINTF("Success!!!\n");

    fp = fopen(KEY_LIBDCRYPT_PATH, "wb");
    if(fp!=NULL) {
        fwrite(ktc_enc_hash, 1, ktc_enc_hash_len, fp);
        fclose(fp);
    }

f_end :
    PRINTF("================================================================================\n\n");

    return ret;
}

int koces_libdcrypt_comp_self_protect(void)
{
    FILE *fp;
    int ret=-1;
    unsigned char digest[32];
    unsigned char *data;
    size_t data_len;
    size_t in_hash_len = 32;

    fp = fopen(KEY_LIBDCRYPT_PATH, "rb");
    if(fp!=NULL) {
        ktc_enc_hash_len = sizeof(digest);
        fread(ktc_enc_hash, 1, ktc_enc_hash_len, fp);
        fclose(fp);
    }
    else {
        PRINTF("Failure!!! : (%d)\n", ret);
        goto f_end;
    }

    fp = fopen("/app/lib/libDCrypt_v3.0.10.5.so", "rb");

    fseek(fp,0,SEEK_END);				// 파일의 끝으로 파일 position 이동
    data_len = (size_t)ftell(fp);    			// 현재 positon 출력
    rewind(fp);							// 스트림을 처음으로 되돌린다.

    data = malloc(data_len+1);
    fread(data, 1, data_len, fp);

    /* 정상 데이터를 Sha-256으로 Hash 값을 추출 - 자체보호 HASH 추출 */
    PRINTF("============================ libDCrypt_HASH_SHA256()  =============================\r\n");

    ret = KOCES_CRYPT_HASH_SHA256(data, (int)data_len, digest);

    free(data);
    fclose (fp);

    if(ret < 0)
    {
        PRINTF("Failure!!! : (%d)\n", ret);
        goto f_end;
    }

    in_hash_len = sizeof(digest);
    memcpy(ktc_in_hash, digest, in_hash_len);
    HEXDUMP(ktc_in_hash, 32, "Hash Data(OUT)");
    PRINTF("Success!!!\n");
    PRINTF("================================================================================\n\n");

    /* 입력(ktc_in_hash)데이타와 이전의 암호화된 데이타를 비교 - 자체보호 검증용으로 사용 */
    PRINTF("======================= libDCrypt_Comp_Self_Protect_HASH() =======================\n");
    ret = KOCES_DUKPT_Comp_Self_Protect_HASH((const unsigned char *)ktc_hide_data, ktc_hide_data_len, ktc_enc_hash, (int)ktc_enc_hash_len, ktc_in_hash, (int)in_hash_len);
    HEXDUMP(ktc_enc_hash, ktc_enc_hash_len, "enc_hash(IN) : (%d)", ktc_enc_hash_len);
    HEXDUMP(ktc_in_hash, in_hash_len, "in_hash(IN) : (%d)", in_hash_len);

    if(ret < 0)
    {
        PRINTF("Failure!!! : (%d)\n", ret);
        goto f_end;
    }
    //HEXDUMP(digest, 32, "Hash Data(OUT)");
    PRINTF("Success!!!\n");


f_end :
    PRINTF("================================================================================\n\n");

    return ret;
}

int koces_libdkoces_make_self_protect(void)
{
    FILE *fp;
    int ret;
    unsigned char digest[32];
    unsigned char *data;
    size_t data_len;
    size_t in_hash_len = 32;

    fp = fopen("/app/lib/libDKoces_v1.0.0.7.so", "rb");

    fseek(fp,0,SEEK_END);					// 파일의 끝으로 파일 position 이동
    data_len = (size_t)ftell(fp);    					// 현재 positon 출력
    rewind(fp);							// 스트림을 처음으로 되돌린다.

    data = malloc(data_len+1);

    fread(data, 1, data_len, fp);

    /* 정상 데이터를 Sha-256으로 Hash 값을 추출 - 자체보호 HASH 추출 */
    PRINTF("\r\n========================== libDKoces_HASH_SHA256()  ===========================\r\n");

    KOCES_CRYPT_HASH_SHA256(data, (int)data_len, digest);

    free(data);
    fclose (fp);

    in_hash_len = sizeof(digest);
    memcpy(ktc_in_hash, digest, in_hash_len);

    /* 입력(ktc_in_hash)데이타와 이전의 암호화된 데이타를 비교 - 자체보호 검증용으로 사용 */
    PRINTF("=====================libDKoces_Make_Self_Protect_HASH() =====================\n");
    ret = KOCES_DUKPT_Make_Self_Protect_HASH((const unsigned char*)ktc_hide_data, ktc_hide_data_len, ktc_enc_hash, (int *)&ktc_enc_hash_len, ktc_in_hash, (int)in_hash_len);
    HEXDUMP(ktc_enc_hash, ktc_enc_hash_len, "enc_hash(IN) : (%d)", ktc_enc_hash_len);
    HEXDUMP(ktc_in_hash, in_hash_len, "in_hash(IN) : (%d)", in_hash_len);

    if(ret < 0)
    {
        PRINTF("Failure!!! : (%d)\n", ret);
        goto f_end;
    }

    PRINTF("ret(OUT) : (%d)\n", ret);
    PRINTF("Success!!!\n");

    fp = fopen(KEY_LIBDKOCES_PATH, "wb");
    if(fp!=NULL) {
        fwrite(ktc_enc_hash, 1, ktc_enc_hash_len, fp);
        fclose(fp);
    }

f_end :
    PRINTF("================================================================================\n\n");

    return ret;
}

int koces_libdkoces_comp_self_protect(void)
{
    FILE *fp;
    int ret=-1;
    unsigned char digest[32];
    unsigned char *data;
    size_t data_len;
    size_t in_hash_len = 32;

    fp = fopen(KEY_LIBDKOCES_PATH, "rb");
    if(fp!=NULL) {
        ktc_enc_hash_len = sizeof(digest);
        fread(ktc_enc_hash, 1, ktc_enc_hash_len, fp);
        fclose(fp);
    }
    else {
        PRINTF("Failure!!! : (%d)\n", ret);
        goto f_end;
    }

    fp = fopen("/app/lib/libDKoces_v1.0.0.7.so", "rb");

    fseek(fp,0,SEEK_END);			// 파일의 끝으로 파일 position 이동
    data_len = (size_t)ftell(fp);    			// 현재 positon 출력
    rewind(fp);							// 스트림을 처음으로 되돌린다.

    data = malloc(data_len+1);
    fread(data, 1, data_len, fp);

    /* 정상 데이터를 Sha-256으로 Hash 값을 추출 - 자체보호 HASH 추출 */
    PRINTF("============================ libDKoces_HASH_SHA256()  =============================\r\n");

    ret = KOCES_CRYPT_HASH_SHA256(data, (int)data_len, digest);
    free(data);
     fclose (fp);

    if(ret < 0)
    {
        PRINTF("Failure!!! : (%d)\n", ret);
        goto f_end;
    }

    in_hash_len = sizeof(digest);
    memcpy(ktc_in_hash, digest, in_hash_len);
    HEXDUMP(ktc_in_hash, 32, "Hash Data(OUT)");
    PRINTF("Success!!!\n");
    PRINTF("================================================================================\n\n");

    /* 입력(ktc_in_hash)데이타와 이전의 암호화된 데이타를 비교 - 자체보호 검증용으로 사용 */
    PRINTF("======================= libDKoces_Comp_Self_Protect_HASH() =======================\n");
    ret = KOCES_DUKPT_Comp_Self_Protect_HASH((const unsigned char*)ktc_hide_data, ktc_hide_data_len, ktc_enc_hash, (int)ktc_enc_hash_len, ktc_in_hash, (int)in_hash_len);
    HEXDUMP(ktc_enc_hash, ktc_enc_hash_len, "enc_hash(IN) : (%d)", ktc_enc_hash_len);
    HEXDUMP(ktc_in_hash, in_hash_len, "in_hash(IN) : (%d)", in_hash_len);

    if(ret < 0)
    {
        PRINTF("Failure!!! : (%d)\n", ret);
        goto f_end;
    }
    //HEXDUMP(digest, 32, "Hash Data(OUT)");
    PRINTF("Success!!!\n");


f_end :
    PRINTF("================================================================================\n\n");

    return ret;
}

int koces_library_make_self_protect(void)
{
    int ret = 0;

    // koces
    ret = koces_libdcli_make_self_protect();
    if(ret < 0) return -1;
    ret = koces_libdcrypt_make_self_protect();
    if(ret < 0) return -1;
    ret = koces_libdkoces_make_self_protect();
    if(ret < 0) return -1;

    // emv

    return 0;
}

int koces_library_comp_self_protect(void)
{
    int ret = 0;

    // koces
    ret = koces_libdcli_comp_self_protect();
    if(ret < 0) return -1;
    ret = koces_libdcrypt_comp_self_protect();
    if(ret < 0) return -1;
    ret = koces_libdkoces_comp_self_protect();
    if(ret < 0) return -1;

    // emv

    return 0;
}


//--------------------------------------------------------------------------------------------------------------------

//int koces_hdata_insert_response(unsigned char *pdata, int recvlen)
int koces_init_dupkt_injection(unsigned char *pdata, int recvlen)
{
    int ret;
    FILE *fp;

    PRINTF("KOCES : HDATA INJECTION.\n\r");
    memset(&koces_fk_info, 0, sizeof(s_koces_fk_info));
    fp = fopen(KTC_FK_PATH, "wb");
    if(fp!=NULL) {
        fwrite(&koces_fk_info, sizeof(s_koces_fk_info), 1, fp);
        fclose(fp);
    }

    memset(&ktc_enc_hash, 0, sizeof(ktc_enc_hash));
    fp = fopen(KTC_MARU_PATH, "wb");
    if(fp!=NULL) {
        fwrite(ktc_enc_hash, 1, ktc_enc_hash_len, fp);
        fclose(fp);
    }

    memset(koces_dukpt_info.hdata, 0, sizeof(koces_dukpt_info.hdata));
    fp = fopen(KTC_DUKPT_PATH, "wb");
    if(fp!=NULL) {
        fwrite(&koces_dukpt_info, sizeof(s_koces_dukpt_info), 1, fp);
        fclose(fp);
    }

    koces_dukpt_info.key_injection = UBCN_DUKPT_INIT;
    koces_dukpt_info.hdata_used   = UBCN_DUKPT_INIT;

    memset(koces_dukpt_info.serial_number, 0, sizeof(koces_dukpt_info.serial_number));
    memset(koces_dukpt_info.hdata, 0, sizeof(koces_dukpt_info.hdata));

    memcpy(koces_dukpt_info.serial_number, pdata, 10);
    memcpy(koces_dukpt_info.hdata, pdata+10, (size_t)(recvlen-10));
    PRINTF("===================== ===== KOCES_DUKPT_Init_Hide_Data() ====== ===================\r\n");
    PRINTHEX(koces_dukpt_info.hdata, 1024, "HDATA %d", 1024);
    PRINTF("========================== KOCES_DUKPT_ReGen_Hide_Data() =========================\r\n");
    /* 단말기(리더기)에 KOCES에서 제공한 HData(1024Byte) 주입시 1회만 시행 후 삭제(REWRITE). */
    ret = KOCES_DUKPT_ReGen_Hide_Data(koces_dukpt_info.hdata, 1024, koces_dukpt_info.serial_number, (int)strlen((const char*)koces_dukpt_info.serial_number));
    if (ret < 0) {
            PRINTF("Error: (%d)\r\n", ret);
            return RC_FAILURE;
    }
    /* SID(SecureID), SKey 추출,
       최종적으로 만들어진 1024Byte의 은닉화 된 데이터를 단말기의 비휘발성 메모리에 저장. */

    koces_dukpt_info.key_injection  = UBCN_DUKPT_SUCCESS;
    fp = fopen(KTC_DUKPT_PATH, "w");
    if(fp != NULL){
        fwrite(&koces_dukpt_info, sizeof(s_koces_dukpt_info), 1, fp);
        fclose(fp);
    }
    koces_fk_info.ksn_counter = 1;
    if( koces_fk_info.ksn_1st>sizeof(koces_dupkt_1st_byte) ) koces_fk_info.ksn_1st = 0;
    fp = fopen(KTC_FK_PATH, "w");
    if(fp!=NULL) {
        fwrite(&koces_fk_info, sizeof(s_koces_fk_info), 1, fp);
        fclose(fp);
    }

    //PRINTHEX((unsigned char*)&koces_fk_info, sizeof(s_koces_fk_info), "FK :");
    PRINTHEX(koces_dukpt_info.hdata, 1024, "HDATA %d", 1024);
    PRINTF("Success!!!\r\n");
    PRINTHEX(koces_dukpt_info.serial_number, 10, "CID(%c)", koces_dukpt_info.hdata_used);
    PRINTF("================================================================================\r\n");

    return RC_DATA;
}


void koces_ic_request(int nType, unsigned long nAmount)
{
#if 0
    int i, nLen=0, offset, track2_len;
    char track2[128], tmp[128];
    char *pbuf;
    unsigned char *pdata, buf[256];
    unsigned char ksn[10];
    unsigned short crc;

    size_t ksn_len=0, enc_len;
    PKocesICReq pKoces;

    if((nType==1) && UBC_LogCheck()) return;

    memset((char*)&VAN_BasicReq, ' ', sizeof(NanoVANBasicReq));

    // 00
    VAN_BasicReq.stx = 0x02;
    // 01  an 4 단말기번호 - ETX 길이
    //sprintf((char*)VAN_BasicReq.MsgLen,                "%04d", );
    // 05  an 10 단말기 번호 "1234567890"
    sprintf((char*)VAN_BasicReq.TID,                   "%s",   g_VcpsInfo.TID);
    // 0F an 2 거래(업무) 구분
    sprintf((char*)VAN_BasicReq.TransactionClass,      "%s",   g_NanoTransaction[1]);
    // 11 an 2 전문 구분, 수집
    sprintf((char*)VAN_BasicReq.PacketClass,           "%s",   g_NanoPacket[nType]);
    // 13 an 1 입력매체 구분코드 , K:KEYIN, S:SWIPE, R:RF, I:IC, X:NON
    VAN_BasicReq.TransactionType = 'I';   // RF_EMV
    // 14 an 5 단말기 거래일련 번호, 총 거래 Cnt
    if(nType==1)
        sprintf((char*)VAN_BasicReq.TransactionCount,      "%05d", g_TmsLog.TotCnt);
    else
        sprintf((char*)VAN_BasicReq.TransactionCount,      "%05d", m_nNanoTotCnt);

    // 19 an 2 전문 버젼
    sprintf((char*)VAN_BasicReq.PacketVersion,         "%s",   "T1");
    // 1B an 1 전문 암호화 여부 안함
    VAN_BasicReq.Encryption = '0';
    // 1C an 10 예비용
    memset((char*)VAN_BasicReq.RFU1, ' ', 10);
    // 26 FS																//
    VAN_BasicReq.FS1 = FS;
    // 27 an 40 인식 카드번호

    // Track II Data : Track2 정보는 3rd와 '=' 이후 전부 '*'로 마스킹해서 전송
    //ex) 유효 Track2 정보가  "9400123412341234=19121234123451234567"(37byte) 일 경우 "940012******1234=1*******************   "(40byte)
    // 암호화 대상 정보는 유효 Track2 정보와 동일하며 암호화한 데이터가 48byte 보다 작을 경우 LEFT-JUSTIFIED, 스페이스 패딩해야 함.
    memset(track2, 0x20, sizeof(track2));
    if(nType==1) {
        track2_len = 40;

        memset(track2, 0, sizeof(track2));
        Hex2Ascii(&EMVL2_App.DE_57[1],  track2,  EMVL2_App.DE_57[0]);

        for(i=0; i<track2_len && track2[i]!='D'; i++);
        track2[i]='=';
        for(; i<track2_len && track2[i]!='F'; i++);
        track2[i]=' ';

        g_MsrInfo.nCnt2 = track2_len = i;
        memcpy(g_MsrInfo.nTrack2, track2, g_MsrInfo.nCnt2);
    }
    else {
        track2_len = m_nMsrInfo.nCnt2;
        memcpy(track2, m_nMsrInfo.nTrack2, m_nMsrInfo.nCnt2);
        UBC_TrackBackupDataClear();
    }

    memset(tmp, '*', 40);
    tmp[40] = 0;

    memcpy(tmp, track2, 6);
    pbuf = strstr((char *)track2, "=");

    if(!pbuf) return;

    PRINTHEX(DEBUG_PORT,track2, 64, "t2 in address : 0x%08X", track2);

    offset = (unsigned long)pbuf-(unsigned long)track2;

    tmp[offset-4] = *(pbuf-4);
    tmp[offset-3] = *(pbuf-3);
    tmp[offset-2] = *(pbuf-2);
    tmp[offset-1] = *(pbuf-1);
    tmp[offset]    = *(pbuf);
    tmp[offset+5] = *(pbuf+5);

    memcpy(VAN_BasicReq.CSN, tmp, 40);


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
        UBC_SetTrack2Backup(nAmount);
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
        for(i=0; i<8; i++) VAN_BasicReq.ApproveDate[i] = VAN_BasicRes.Date[i];
        // 7E FS
        VAN_BasicReq.FS8 = FS;
        // 7F an 10 원거래 승인 번호
        for(i=0; i<10; i++)VAN_BasicReq.ApproveNum[i]  = VAN_BasicRes.ApproveNum[i];
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
    nLen = 0;
    pKoces = (PKocesICReq)VAN_BasicReq.AddedInfo;

    memset(VAN_BasicReq.AddedInfo, 0x30, sizeof(VAN_BasicReq.AddedInfo));

    //memcpy(pKoces->preamble_num, "2410", 4);
    //nLen += 4;

    //memcpy(pKoces->cat_id, g_VmConfig.CATID, 10);
    //nLen += 10;

    // 단말인증번호
    memcpy(pKoces->auth_num, VM_GetKtcApprovalNumber(), 32);
    nLen += 32;
    //PRINTF(DEBUG_PORT,"VM_GetKtcApprovalNumber\r\n");
    // 거래일시
    //GetTimeString((time_t)NULL, pKoces->date_time);
    GetTimeString(pKoces->date_time);
    nLen += 14;
    //PRINTF(DEBUG_PORT,"GetTimeString\r\n");

    ksn_len = 10;
    if(nType==2)
    {
        // 암호화된 Track II Data
        memset(pKoces->crypto_track2, ' ', 96);
        nLen += 96;

        // KSN Copy.
        Hex2Ascii(koces_fk_info.ksn, pKoces->ksn, ksn_len);
        nLen += 20;
    }
    else {
        // 암호화된 Track II Data
        memcpy(ksn, koces_fk_info.ksn, sizeof(ksn));
        memset(tmp, 0x20, sizeof(tmp));
        memcpy(tmp, track2, track2_len);

        for(i=0; i<track2_len && tmp[i]!='='; i++);
        //tmp[i]='D';
        //tmp[track2_len]='F';

        //PRINTHEX(DEBUG_PORT,tmp, 40, "IC TII");

        KOCES_DUKPT_AES_Encrypt(koces_dukpt_info.hdata, 1024, buf, &enc_len, ksn, &ksn_len, tmp, 40, &koces_fk_info.fki);
        Hex2Ascii(buf, pKoces->crypto_track2, enc_len);
        nLen += 96;

        PRINTHEX(DEBUG_PORT,pKoces->crypto_track2, enc_len, "t2 cryto in address : 0x%08X", pKoces->crypto_track2);

        // KSN Copy.
        Hex2Ascii(ksn, pKoces->ksn, ksn_len);
        memcpy(koces_fk_info.ksn, ksn, sizeof(ksn));
        nLen += 20;

        PRINTHEX(DEBUG_PORT, koces_fk_info.ksn, 10, "t2 cryto ksn");
    }


    // Working Key Index
    Hex2Ascii("\00", pKoces->working_key_index, 1);
    nLen += 2;

    // 비밀번호
    memset(pKoces->password, ' ', 16);
    nLen += 16;

    if(nType!=2) {

        // EMV거래구분
        pKoces->emv_transaction_type = koces_emv_sevice_type();
        nLen += 1;

        // POS Entry Mode Code
        // 일반 IC 거래 시 4BCD 형태로 카드에서 전달된 정보를 전송
        // FallBack 거래 시 반드시 PosEntryMode[0]=0x90, PosEntryMode[1]=0x10 로 설정,
        // 기타 IC 관련 필드("Card Sequence Number"~"Transaction Sequence Counter")는 NULL로 설정.
        Hex2Ascii((unsigned char*)"\x05\x10" , pKoces->pos_entry_mode_code, 2);
        nLen += 4;

        // Card Sequence Number, 5f34
        PRINTHEX(DEBUG_PORT,EMVL2_App.DE_5F34, sizeof(EMVL2_App.DE_5F34), "DE_5F34");
        Hex2Ascii(&EMVL2_App.DE_5F34[1], pKoces->card_sequence_number+2, EMVL2_App.DE_5F34[0]);
        //Hex2Ascii((unsigned char*)"\x00\x01", pKoces->card_sequence_number, 2);

        nLen += 4;

        // Additional POS information
        //Hex2Ascii((unsigned char*)"      ", pKoces->additional_pos_information, 6);
        Hex2Ascii((unsigned char*)"\x05\x05\x00\x00\x10\x00", pKoces->additional_pos_information, 6);
        nLen += 12;

        // Issuer Script Results
        tmp[0]=0x9F;
        tmp[1]=0x5B;
        tmp[2]=0x00;
        //memcpy(tmp+2, EMVL2_App.DE_9F5B, 9);
        Hex2Ascii(tmp, pKoces->DE_9F5B, 3);
        nLen += 46;

        // Application Cryptogram (AC)
        tmp[0]=0x9F;
        tmp[1]=0x26;
        memcpy(tmp+2, EMVL2_App.DE_9F26, EMVL2_App.DE_9F26[0]+1);
        Hex2Ascii(tmp, pKoces->DE_9F26, EMVL2_App.DE_9F26[0]+3);
        nLen += 22;

        // Cryptogram InformationData
        tmp[0]=0x9F;
        tmp[1]=0x27;
        memcpy(tmp+2, EMVL2_App.DE_9F27, EMVL2_App.DE_9F27[0]+1);
        Hex2Ascii(tmp, pKoces->DE_9F27, EMVL2_App.DE_9F27[0]+3);
        nLen += 8;

        // Issuer Application Data  (IAD)
        tmp[0]=0x9F;
        tmp[1]=0x10;
        memcpy(tmp+2, EMVL2_App.DE_9F10, EMVL2_App.DE_9F10[0]+1);
        Hex2Ascii(tmp, pKoces->DE_9F10, EMVL2_App.DE_9F10[0]+3);
        nLen += 70;

        // Unpredictable Number
        tmp[0]=0x9F;
        tmp[1]=0x37;
        memcpy(tmp+2, EMVL2_App.DE_9F37, EMVL2_App.DE_9F37[0]+1);
        Hex2Ascii(tmp, pKoces->DE_9F37, EMVL2_App.DE_9F37[0]+3);
        nLen += 14;

        // Application Transaction Counter (ATC)
        tmp[0]=0x9F;
        tmp[1]=0x36;
        memcpy(tmp+2, EMVL2_App.DE_9F36, EMVL2_App.DE_9F36[0]+1);
        Hex2Ascii(tmp, pKoces->DE_9F36, EMVL2_App.DE_9F37[0]+3);
        nLen += 10;

        // Terminal Verification Result (TVR)
        tmp[0]=0x95;
        memcpy(tmp+1, EMVL2_App.DE_95, EMVL2_App.DE_95[0]+1);
        Hex2Ascii(tmp, pKoces->DE_95, EMVL2_App.DE_95[0]+2);
        nLen += 14;

        // Transaction Date
        tmp[0]=0x9A;
        memcpy(tmp+1, EMVL2_App.DE_9A, EMVL2_App.DE_9A[0]+1);
        Hex2Ascii(tmp, pKoces->DE_9A, EMVL2_App.DE_9A[0]+2);
        nLen += 10;

        // Transaction Type
        tmp[0]=0x9C;
        memcpy(tmp+1, EMVL2_App.DE_9C, EMVL2_App.DE_9C[0]+1);
        Hex2Ascii(tmp, pKoces->DE_9C, EMVL2_App.DE_9C[0]+2);
        nLen += 6;

        // Transaction Amount
        tmp[0]=0x9F;
        tmp[1]=0x02;
        memcpy(tmp+2, EMVL2_App.DE_9F02, EMVL2_App.DE_9F02[0]+1);
        Hex2Ascii(tmp, pKoces->DE_9F02, EMVL2_App.DE_9F02[0]+3);
        nLen += 18;

        // Transaction Currency Code
        tmp[0]=0x5F;
        tmp[1]=0x2A;
        memcpy(tmp+2, EMVL2_App.DE_5F2A, EMVL2_App.DE_5F2A[0]+1);
        Hex2Ascii(tmp, pKoces->DE_5F2A, EMVL2_App.DE_5F2A[0]+3);
        nLen += 10;

        // Application Interchange Profile(AIP)
        tmp[0]=0x82;
        memcpy(tmp+1, EMVL2_App.DE_82, EMVL2_App.DE_82[0]+1);
        Hex2Ascii(tmp, pKoces->DE_82, EMVL2_App.DE_82[0]+2);
        nLen += 8;

        // Terminal Country Code
        tmp[0]=0x9F;
        tmp[1]=0x1A;
        memcpy(tmp+2, EMVL2_App.DE_9F1A, EMVL2_App.DE_9F1A[0]+1);
        Hex2Ascii(tmp, pKoces->DE_9F1A, EMVL2_App.DE_9F1A[0]+3);
        nLen += 10;

        // Amount Other
        tmp[0]=0x9F;
        tmp[1]=0x03;

        EMVL2_App.DE_9F03[0] = 6;
        memset(&EMVL2_App.DE_9F03[1], 0, 6);

        memcpy(tmp+2, EMVL2_App.DE_9F03, EMVL2_App.DE_9F03[0]+1);
        Hex2Ascii(tmp, pKoces->DE_9F03, EMVL2_App.DE_9F03[0]+3);
        nLen += 18;

        // Cardholder Verification Method(CVM) Results
        tmp[0]=0x9F;
        tmp[1]=0x34;
        memcpy(tmp+2, EMVL2_App.DE_9F34, EMVL2_App.DE_9F34[0]+1);
        Hex2Ascii(tmp, pKoces->DE_9F34, EMVL2_App.DE_9F34[0]+3);
        nLen += 12;

        // Terminal Capabilities
        tmp[0]=0x9F;
        tmp[1]=0x33;
        memcpy(tmp+2, EMVL2_App.DE_9F33, EMVL2_App.DE_9F33[0]+1);
        Hex2Ascii(tmp, pKoces->DE_9F33, EMVL2_App.DE_9F33[0]+3);
        nLen += 12;

        // Terminal Type
        tmp[0]=0x9F;
        tmp[1]=0x35;
        memcpy(tmp+2, EMVL2_App.DE_9F35, EMVL2_App.DE_9F35[0]+1);
        Hex2Ascii(tmp, pKoces->DE_9F35, EMVL2_App.DE_9F35[0]+3);
        nLen += 8;

        // Interface Device(IFD) Serial Number
        tmp[0]=0x9F;
        tmp[1]=0x1E;
        memcpy(tmp+2, EMVL2_App.DE_9F1E, EMVL2_App.DE_9F1E[0]+1);
        Hex2Ascii(tmp, pKoces->DE_9F1E, EMVL2_App.DE_9F1E[0]+3);
        nLen += 22;

        // Transaction Category Code
        tmp[0]=0x9F;
        tmp[1]=0x53;
        //memcpy(tmp+2, EMVL2_App.DE_9F53, 2);
        memcpy(tmp+2, "\x01\x00", 2);
        Hex2Ascii(tmp, pKoces->DE_9F53, 4);
        nLen += 8;

        // Dedicated File Name
        tmp[0]=0x84;
        memcpy(tmp+1, EMVL2_App.DE_84, EMVL2_App.DE_84[0]+1);
        Hex2Ascii(tmp, pKoces->DE_84, EMVL2_App.DE_84[0]+2);
        nLen += 36;

        // Terminal ApplicationVersion Number
        tmp[0]=0x9F;
        tmp[1]=0x09;
        memcpy(tmp+2, EMVL2_App.DE_9F09, EMVL2_App.DE_9F09[0]+1);
        Hex2Ascii(tmp, pKoces->DE_9F09, EMVL2_App.DE_9F09[0]+3);
        nLen += 10;

        // Transaction Sequence Counter
        tmp[0]=0x9F;
        tmp[1]=0x41;
        memcpy(tmp+2, EMVL2_App.DE_9F41, EMVL2_App.DE_9F41[0]+1);
        Hex2Ascii(tmp, pKoces->DE_9F41, EMVL2_App.DE_9F41[0]+3);
        nLen += 14;

        // Fallback 사유
        // 01 : Chip 전원을 넣었으나 응답이 없는 경우
        // 02 : 상호지원 application이 없을 경우
        // 03 : 칩데이터 읽기 실패
        // 04 : Mandatory 데이터 미포함
        // 05 : CVM 커맨드 응답실패
        // 06 : EMV 커맨드 잘못설정
        // 07 : 터미널 오작동
        Hex2Ascii("\x00" , pKoces->fallback_reason, 1);
        nLen+=2;

    }

    // 8A FS
    VAN_BasicReq.AddedInfo[nLen++] = FS;
    // 8B an 판매된 Column
    BCD2Char((uchar*)&g_VmAmountColumn.Col, (uchar*)&VAN_BasicReq.AddedInfo[nLen], 2);	// an 현재 컬럼.
    nLen+=2;
    // 8D
    VAN_BasicReq.AddedInfo[nLen++] = 0x03;

    // 단말기번호 ~ ETX까지의 길이
    pdata = VAN_BasicReq.TID;
    for(i=0; i<sizeof(NanoVANBasicReq) ; i++) {
        if(pdata[i]==0x03) break;
    }
    sprintf((char*)buf,  "%04d", i+1);
    memcpy((char*)VAN_BasicReq.MsgLen, buf, 4);

    // CRC
    crc = (unsigned short)CRC_Update((unsigned char*)&VAN_BasicReq, Char2Int(VAN_BasicReq.MsgLen, 0)+5);
    Int2Byte(crc, &VAN_BasicReq.AddedInfo[nLen]);

    PRINTHEX(DEBUG_PORT, (unsigned char*)&VAN_BasicReq, Char2Int(VAN_BasicReq.MsgLen, 0)+5, "EMV REQ %d", Char2Int(VAN_BasicReq.MsgLen, 0)+5);

    g_NanoVANBasicReq = VAN_BasicReq;

    //민감한 개인정보 완전삭제 추가.
    memset(buf, 0x00, sizeof(buf));
    memset(buf, 0xFF,sizeof(buf));
    memset(buf, 0x00, sizeof(buf));

    memset(EMVL2_App.DE_57, 0x00, sizeof(EMVL2_App.DE_57));
    memset(EMVL2_App.DE_57, 0xFF,sizeof(EMVL2_App.DE_57));
    memset(EMVL2_App.DE_57, 0x00, sizeof(EMVL2_App.DE_57));


    memset(tmp, 0x00, sizeof(tmp));
    memset(tmp, 0xFF,sizeof(tmp));
    memset(tmp, 0x00, sizeof(tmp));

    memset(track2, 0x00, sizeof(track2));
    memset(track2, 0xFF, sizeof(track2));
    memset(track2, 0x00, sizeof(track2));

    PRINTHEX(DEBUG_PORT,track2, 64, "t2 clr address : 0x%08X", track2);

    switch(nType) {
    case 0x02 : // 취소
    case 0x08 : // 망상취소
        UBC_TmsBackUpProcess(-1);
        UBC_Backup(&VAN_BasicReq);
//		PRINTHEX(DEBUG_PORT, (unsigned char*)&g_NanoVANBasicReq, nLen+2, "취소 >> %d", nLen);
        break;
    case 0x06 : // 조회
        break;
    case 0x01 :
        UBC_TmsBackUpProcess(nAmount);
        //UBC_BackupVolatile();
        UBC_Backup(&VAN_BasicReq);

        //VM_ReadVanFlash();
        UBC_ReadVanFlash(0);
        break;
    }
#endif

}

unsigned char koces_ic_response(unsigned char *recvbuf, size_t recvlen)
{
#if 0
    int len_ac2nd, len_ard, len_iad, len_is, nOutDataLen=0;
    byte pOutData[256], ret;

    PKocesICRes pRes = (PKocesICRes)recvbuf;
    //PKocesICReq pReq = (PKocesICReq)g_CurrVANBasicReq.AddedInfo;
    PKocesICReq pReq = (PKocesICReq)g_NanoVANBasicReq.AddedInfo;
    PRINTHEX(DEBUG_PORT,recvbuf, recvlen, "ic_res %d", recvlen);

    if(pReq->emv_transaction_type == 'F') return RC_SUCCESS;


    Char2BCD(pRes->ard, koces_ic_res.ard, sizeof(koces_ic_res.ard));
    Char2BCD(pRes->iad, koces_ic_res.iad, sizeof(koces_ic_res.iad));
    Char2BCD(pRes->issuer_script, koces_ic_res.issuer_script, sizeof(koces_ic_res.issuer_script));

    len_ard = koces_ic_res.ard[0];
    len_iad = sizeof(koces_ic_res.iad)/2;
    len_is  = sizeof(koces_ic_res.issuer_script)/2;

    PRINTHEX(DEBUG_PORT,&koces_ic_res.ac2nd, 1, "ac2nd");
    PRINTHEX(DEBUG_PORT,&koces_ic_res.ard[1], koces_ic_res.ard[0], "ard");
    PRINTHEX(DEBUG_PORT,koces_ic_res.iad, len_iad, "iad");
    PRINTHEX(DEBUG_PORT,koces_ic_res.issuer_script, len_is, "issuer_script");

    #if (EMV_2ND_GENERATION_EN==1)
    {
        ret= EMV_step6("00", koces_ic_res.ard ,koces_ic_res.iad, koces_ic_res.issuer_script, 0x30, pOutData, &nOutDataLen);
        if(ret != RC_SUCCESS) {
            EMV_Reset();
            koces_ic_request(8, g_CraditAmount);
            ret = RC_FAILURE;
        }
        PRINTHEX(DEBUG_PORT,pOutData, nOutDataLen, "EMV_step6.%02X", ret);
    }
    #else
    {
        ret = RC_SUCCESS;
    }
    #endif

    //app_contact_Deactivate();
    TRD_PowerOff(0);
    return ret;
#endif
}

