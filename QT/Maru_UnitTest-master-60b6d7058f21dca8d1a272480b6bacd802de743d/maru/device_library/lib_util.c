#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <fcntl.h>
#include <time.h>
#include <ctype.h>
#include <monetary.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>

#include <linux/rtc.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/time.h>

#include "typedef.h"
#include "lib_util.h"

#define LIB_UTIL_DBG 	0
#if (LIB_UTIL_DBG==1)
# define	CPRINTF(fmt,args...) 	printf(fmt, ##args)
# define	CPRINTHEX(fmt,args...)  	PrintHex(fmt, ##args)
# define	CHEXDUMP(fmt,args...)  	HEX_Dump(fmt, ##args)
#else
# define    CPRINTF(...)    (void)(0)
# define    CPRINTHEX(...)  (void)(0)
# define    CHEXDUMP(...)   (void)(0)
#endif

#if 1
void cPrintHex(unsigned char *nAddr, int nLen, char *sFrom, ...)
{
	int i, j=0;
	va_list ap;
	char buf[256];

	va_start(ap,sFrom);
	vsprintf(buf,sFrom,ap);
	va_end(ap);
	
	for(i=0; buf[i]=='\n' || buf[i]=='\r'; i++) {
		printf("%c", buf[i]);
	}
	
	if(strlen(&buf[i])>=15) {
		printf("%s\r\n", &buf[i]);
		printf("%-15s", "");
	
	} else {
		printf("%-15s", &buf[i]);
	}
	for (i = 0; i < nLen;) {
		printf("%02X ", nAddr[i]);
		
		i++; j++; 
		
		if ((j % 16) == 0) {
			j = 0;
			if(i!=nLen) 	printf("\r\n%15s", "");
		}
		else if ((j % 8) == 0) {
			printf("  ");
		} 
	}
	printf("\r\n");
}

#define CHEX_SHORT_DUMP	1
void CHEX_Dump(unsigned char *nAddr, int nLen, char *sFrom, ...) {
	unsigned char *inP, *endP;
	int i, nRemainder;
	va_list ap;
	char buf[128];

	va_start(ap,sFrom);
	vsprintf(buf,sFrom,ap);
	va_end(ap);
	
	
	endP = (unsigned char *)((unsigned long)nAddr+nLen);
	nRemainder = nLen%16;	
	
	for(i=0; buf[i]=='\n' || buf[i]=='\r'; i++) {
		printf("%c", buf[i]);
	}
	
#if CHEX_SHORT_DUMP
	if(strlen(&buf[i])>=15) {
		printf("%s\r\n", &buf[i]);
		printf("%-15s", "");
	
	} else {
		printf("%-15s", &buf[i]);
	}
#else
	//printf("\r\nOffset         HEX Value                                         ASCII value\r\n");
	printf("%-15s 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\r\n", buf);
	//printf("%-15s-----------------------------------------------\r\n", buf);
	//printf("\r\n%-15s", buf);
#endif

	//print out 16byte blocks
	for (inP=nAddr; inP+16 <= endP; inP+=16) {
		// offset output
		#if HEX_SHORT_DUMP
		if(inP!=nAddr) 
		#endif
		printf("0x%08lX     ", (long)(inP-nAddr));

		// 16-byte assign
		for(i=0; i<16; i++) {
			printf("%02X ", inP[i]);
		}
		printf("  ");

		for(i=0; i<16; i++) {
			if(inP[i]>=32 && inP[i]<=125)
				printf("%c", inP[i]);
			else 
				printf(".");
		}
		
		printf("\r\n");

	}

	if(nRemainder) {		
		// offset output
		#if HEX_SHORT_DUMP
		if(inP!=nAddr) 
		#endif
		printf("0x%08lX     ", (long)(inP-nAddr));

		// 16-byte assign
		for(i=0; i<nRemainder; i++) {
			printf("%02X ", inP[i]);
		}

		for(; i<16; i++) {
			printf("   ");
		}
		printf("  ");

		for(i=0; i<nRemainder; i++) {
			if(inP[i]>=32 && inP[i]<=125)
				printf("%c", inP[i]);
			else 
				printf(".");
		}
		
		printf("\r\n");

	}
	
	if(!nLen)	printf("\r\n");
}
#endif


#define POLYNOMIAL	0x8005

static unsigned long crc32_table[256];
static unsigned short crc_table[256];
const unsigned short cnCRC_16 = 0x8005;
const unsigned long cnCRC_32 = 0x04c11db7L;

void CRC_GenerateTable( unsigned short aPoly )
{
    unsigned short i, j;
    unsigned short nData;
    unsigned short nAccum;

    for ( i = 0; i < 256; i++ )
    {
        nData = ( unsigned short )( i << 8 );
        nAccum = 0;

        for ( j = 0; j < 8; j++ )
        {
            if ( ( nData ^ nAccum ) & 0x8000 )
                nAccum = ( nAccum << 1 ) ^ aPoly;
            else
                nAccum <<= 1;
            nData <<= 1;
        }

        crc_table[i] = nAccum;
    }
}

unsigned short CRC_Update( unsigned char * aData, unsigned short aSize )
{
    unsigned short i;
    unsigned short nAccum = 0;

    CRC_GenerateTable( cnCRC_16 );

    for ( i = 0; i < aSize; i++ )
        nAccum = ( nAccum << 8 ) ^ ( unsigned short )crc_table[( nAccum >> 8 ) ^ *aData++];
    return nAccum;
}


void chEndian(char* des, char* src, int len)
{
	int i;
	char* pTmp;
	pTmp = (char*)malloc(len);
	memcpy(pTmp, src, len);
	for(i=0;i<len;i++)
		des[i] = pTmp[len-1-i];	
}


unsigned char GetChar(unsigned char bHex)
{
	if (bHex >= 0x0A && bHex <= 0x0F)
		return bHex + 0x37;
	else 
		return bHex + 0x30;
}

unsigned char GetHex(unsigned char bHex)
{
	if (toupper(bHex) >= 'A' && toupper(bHex) <= 'F') {
		return (toupper(bHex) - 0x37);
	}
	else if (bHex >= '0'){
		return bHex - 0x30;
	} else {
		return bHex;
	}
	return bHex;
}

/* unsigned char convert to hex string */
/* 0x00 0x12-> "0012"					*/
void ByteStr2HexStr(unsigned char *bIn, char *sOut, int nSize)
{
	int i;

	for (i = 0; i < nSize; i++) {
		sprintf(&sOut[i*2], "%02X", bIn[i]);
	}
}

void HexStr2Byte(char *pHexStr, int nSize, unsigned char *pByte)
{
	int i, j;
	
	j = 0;

	for (i=0; i<nSize; i=i+2) {
		pByte[j] = GetHex(pHexStr[i]) << 4 | GetHex(pHexStr[i+1]);
		j++;
	}
}

void Str2BCD(char *pDigit, int nSize, unsigned char *pBCD) 
{
	int i=0, j=0;

	if ((nSize % 2) == 0) {
		for (i=0; i<nSize; i=i+2) {
			pBCD[j] = (GetHex(pDigit[i]) << 4) + GetHex(pDigit[i+1]);
			j++;
		}
	} else {
		for (i=0; i<nSize; i=i+2) {
			pBCD[j] = (GetHex(pDigit[i]) << 4);
			if (nSize > (i+1))
				pBCD[j] =  pBCD[j]+ GetHex(pDigit[i+1]);
			else 
				 pBCD[j] =  pBCD[j] + 0x0F;
				
			j++;
		}
	}
}

unsigned char Ascii_To_Hex(unsigned char a)
{
		if((a>='0') && (a <= '9')) return(a-'0');
		if((a>='a') && (a <= 'f')) return(a-'W');	//small
		if((a>='A') && (a <= 'F')) return(a-'7');	//Capital
		else return(0x0F);							
}

void Ascii2Hex(unsigned char *inChar, unsigned char *outBit, int size) {
	int i;
	
	for(i=0;i<size;i++) {
		outBit[i] = Ascii_To_Hex(inChar[i]);
	}
	
}

void Hex_To_Ascii(unsigned char c, unsigned char asc[])
{
	unsigned char tmp;

	tmp =  c & 0x0f;
	if(tmp<0x0a)	asc[1] = (tmp+0x30);
	else			asc[1] = (tmp+0x37);
	
	tmp = (c>>4)&0x0f;
	if(tmp<0x0a)	asc[0] = (tmp+0x30);
	else			asc[0] = (tmp+0x37);
}

void Hex2Ascii(unsigned char *pHex, unsigned char *pAscii, int nHexLen) {
	int i;
	for(i=0;i<nHexLen;i++)
		Hex_To_Ascii(pHex[i], &pAscii[i*2]);
}

void BCD2Str(unsigned char *pBCD, int nSize, char *pStr) 
{
	int i;

	for (i=0; i<nSize; i++) {
		pStr[2*i] = GetChar((pBCD[i] & 0xF0) >> 4);
		pStr[2*i + 1] = GetChar(pBCD[i] & 0x0F);
	}
}

int Char2BCD(unsigned char *inChar, unsigned char *outBCD, int size)
{
	int i, j;
	
	for (i=0,j=0; i < size;) {
		outBCD[j]  = 0x00;
		outBCD[j]  = Ascii_To_Hex(inChar[i++]) << 4;

		if(i<size)
		outBCD[j] |= Ascii_To_Hex(inChar[i++]);
		j++;
	}	

	return j;
}

int BCD2Char(unsigned char *inBCD, unsigned char *outChar, int size)
{
	int i, j;	

	for (i=j=0; j < size; j++) {
		outChar[i++] = (inBCD[j]>>4)   > 9 ? (inBCD[j]>>4) + 'A' - 10 : (inBCD[j]>>4) + '0';
		outChar[i++] = (inBCD[j]&0x0f) > 9 ? (inBCD[j]&0x0f) + 'A' - 10: (inBCD[j]&0x0f) + '0';
		//outChar[i++] = (inBCD[j]>>4) + '0';
		//outChar[i++] = (inBCD[j]&0x0f) + '0';
	}	

	return i;
}
void Int2Byte(unsigned int inVal, unsigned char *outVal)
{
	outVal[0] = (unsigned char)((inVal>>8)&0xff);
	outVal[1] = (unsigned char)(inVal&0xff);
}

void InitRandom()
{
	struct timeb tstruct;

	ftime( &tstruct );

	srand(tstruct.time + tstruct.millitm );
	
//	printf("RAND INIT %d\n", tstruct.time  + tstruct.millitm);
}

void GetRandom(unsigned char *pDst, unsigned char nLen)
{
	unsigned char i;

	for(i=0; i<nLen; i++)
		//binary UN 오류 수정(0xFF 도 가능하게)
		//pDst[i] = rand() % 0xFF;
		pDst[i] = rand() % 256;
}


void Swap_4(unsigned char *pHex1, unsigned char *pHex2) 
{
	pHex2[0]=pHex1[3];
	pHex2[1]=pHex1[2];
	pHex2[2]=pHex1[1];
	pHex2[3]=pHex1[0];
}


void Swap_2(unsigned char *pHex1, unsigned char *pHex2) 
{
	pHex2[0]=pHex1[1];
	pHex2[1]=pHex1[0];
}

unsigned char bcd2hex(unsigned char *pDes, unsigned char *pSrc)
{
	unsigned long l=0;
	unsigned char *pl;

	l += ((pSrc[0]&0xF0)>>4)	*10000000;
	l += (pSrc[0]&0x0F)		*1000000;
	l += ((pSrc[1]&0xF0)>>4)	*100000;
	l += (pSrc[1]&0x0F)		*10000;
	l += ((pSrc[2]&0xF0)>>4)	*1000;
	l += (pSrc[2]&0x0F)		*100;
	l += ((pSrc[3]&0xF0)>>4)	*10;
	l += (pSrc[3]&0x0F)		*1;
	pl = (unsigned char*)&l;
	Swap_4(pl, pDes);
	return 0;
}
int ByteToInt(unsigned char *pIn) 
{	
	//     B2I(X1,X2,X3,X4)		(X1 | (X2<<8) | (X3<<16) | (X4<<24))
	return B2I(pIn[3],pIn[2],pIn[1],pIn[0]);
}

int Char2Int(unsigned char *pIn, int nType) 
{	
	if(nType) // little endian
		return ( (pIn[0]&0x0f) + (pIn[1]&0x0f)*10 + (pIn[2]&0x0f)*100 + (pIn[3]&0x0f)*1000 );
	else
		return ( (pIn[3]&0x0f) + (pIn[2]&0x0f)*10 + (pIn[1]&0x0f)*100 + (pIn[0]&0x0f)*1000 );
}

void Util_ChangeEndian(unsigned char *pDes, unsigned char *pSrc, int nLen)
{
	int i;
	
	for(i=0;i<nLen; i++)		pDes[i] = pSrc[nLen-i-1];
	
}

void GetTimeString(time_t tTime, char *pRet)
{
	struct tm tmTime;

	if(0 == tTime)
	{
		tTime = time(NULL);
	}

	localtime_r(&tTime, &tmTime);

	sprintf(pRet, "%04d%02d%02d%02d%02d%02d", tmTime.tm_year + 1900, tmTime.tm_mon + 1, tmTime.tm_mday,
		tmTime.tm_hour, tmTime.tm_min, tmTime.tm_sec);
    //printf(">> %s\n\r", pRet);
}

void GetTimeBCD(time_t tTime, char *pRet)
{
	unsigned char buf[32];

    GetTimeString(tTime, (char *)buf);
    Str2BCD((char *)buf, 14, (unsigned char*)pRet);
}

clock_t CheckExpireTick(clock_t tStart)
{
	clock_t tCur;
	clock_t tDiff;

	tCur = times(NULL);

	if((clock_t)-1 == tCur)
	{
		return 0;
	}

	if(tCur >= tStart)			// 현재 시간이 기준 시간보다 큰 경우, 현재시간에서 기준 시간을 뺌
	{
		tDiff = labs(tCur - tStart);
	}
	else		// 현재 시간이 기준 시간보다 작을 경우, 기준 시간에서 현재 시간을 뺌.
	{
		tDiff = labs(tStart - tCur);
	}

	return tDiff;
}


/**
	@brief		아스키 데이터를 BCD 데이터로 변환하는 함수
	@param[in]	str		아스키 데이터
	@param[in]	size	아스키 데이터(str)의 크기
	@param[out]	pBCD	BCD 데이터로 변환한 데이터
	@return		BCD 로 변환한 길이
*/
int Ascii2BCD(unsigned char *str,int size, unsigned char *pBCD)
{
	int i;

	for(i = 0; i < size ; i += 2) 
	{
		if((*str >= 'A') && (*str <= 'F'))
			*pBCD = (*str - 'A') + 10;
		else if((*str >= 'a') && (*str <= 'f'))
			*pBCD = (*str - 'a') + 10;
		else if((*str >= '0') && (*str <= '9'))
			*pBCD = (*str - '0');
		*pBCD <<= 4;

		str++;
		if((*str >= 'A') && (*str <= 'F'))
			*pBCD += (*str - 'A') + 10;
		else if((*str >= 'a') && (*str <= 'f'))
			*pBCD += (*str - 'a') + 10;
		else if((*str >= '0') && (*str <= '9'))
			*pBCD += (*str - '0');
		pBCD++;
		str++;
	}

	return size;
}

/**
	@brief		BCD to binary convert
	@param[in]	pBCD	source data
	@param[in]	size	length of pBCD
	@return				convert data
*/
unsigned long BCD2Bin(unsigned char *pBCD, int size)
{
	unsigned long  dwVal = 0L;
	int i;

	for(i = 0; i < size; ++i)
	{
		dwVal *= 10L;
		dwVal += ((pBCD[i] >> 4) & 0x0F);
		dwVal *= 10L;
		dwVal += (pBCD[i] & 0x0F);
	}

	return (dwVal);
}

/**
	@brief		convert BCD date time to time_t
	@param[in]	pDate		BCD date. YYYYMMDDhhmmss
	@return					time tick
*/
time_t GetBCDDateTimeTick(unsigned char *pDate)
{
	time_t tRet;
	struct tm tmDate;

	memset(&tmDate, 0, sizeof(tmDate));

	tmDate.tm_year = BCD2Bin(pDate, 2);
	if(tmDate.tm_year >= 1900)
	{
		tmDate.tm_year -= 1900;
	}
	
	tmDate.tm_mon = BCD2Bin(&pDate[2], 1);
	if(tmDate.tm_mon > 0)
	{
		tmDate.tm_mon -= 1;
	}

	tmDate.tm_mday = BCD2Bin(&pDate[3], 1);
	tmDate.tm_hour = BCD2Bin(&pDate[4], 1);
	tmDate.tm_min = BCD2Bin(&pDate[5], 1);
	tmDate.tm_sec = BCD2Bin(&pDate[6], 1);

	tRet = mktime(&tmDate);
	if(tRet < 0)
	{
		tRet = 0;
	}

	return tRet;
}

#if 0
const char *test_week[7]= {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};

int util_strptime(time_t t)
{
	char buf[100]; 
	struct tm *timeptr, result;

	timeptr = localtime(&t);
	strftime(buf, sizeof(buf), "%a %m/%d/%Y %r", timeptr);

	if (strptime(buf, "%a %m/%d/%Y %r",&result) == NULL) {
		printf("\nstrptime failed\n");
		return -1;
	}
	else {
		printf("tm_hour :  %d\n",result.tm_hour);
		printf("tm_min  :  %d\n",result.tm_min);
		printf("tm_sec  :  %d\n",result.tm_sec);
		printf("tm_mon  :  %d\n",result.tm_mon);
		printf("tm_mday :  %d\n",result.tm_mday);
		printf("tm_year :  %d\n",result.tm_year+1900);		/* Year (calendar year minus 1900) */
		printf("tm_yday :  %d/365\n",result.tm_yday);		/* Day of year (0--365) */
		printf("tm_wday :  %s\n",test_week[result.tm_wday]);	/* Weekday (0--6; Sunday = 0) */
    	}
 
    	return 0;
}


int Time2Ctime(unsigned char *OutAddr, int *OutLenth)
{
	int ret = -1;
	unsigned char *Addr = NULL;
	time_t ts = 0;

	time(&ts);
	Addr = (unsigned char*)ctime(&ts);
	if(Addr == 0x00)
	{
		return ret;
	}
	strncpy((char*)OutAddr, (char*)Addr, strlen((char*)Addr));
	*OutLenth = strlen((char*)OutAddr);
	ret = RC_SUCCESS;

	return ret;
}

int Strtime2Mktime(unsigned char *Input, unsigned int *OutSec)
{
	int ret = -1;
	struct tm tm;

	PrintHex(Input, 4, "Input");
	
	strptime(Input, "%a %b %d %H:%M:%S %Y", &tm);
	*OutSec = mktime(&tm);
	ret = RC_SUCCESS;

	printf("*OutSec = %d\n", *OutSec);

	return ret;
}
#endif

#if 0
int getCmdLine(char *file, char *buf) 
{
    FILE *srcFp;
	srcFp = fopen(file, "r");            //   /proc/pid/cmdline에 이름이 있습니다.

	if(srcFp != NULL) {
		memset(buf, 0, sizeof(buf));
		fgets(buf, 256, srcFp);
		
		fclose(srcFp);
	}

    return 1;
}
#endif

#if 0
DIR *dir;                       //  /proc/pid/ 를 가리킬 DIR* 변수
struct dirent *entry;        // 각 파일의 inode를 통해 파일을 선택할 dirent 구조체
struct stat fileStat;          // 파일의 정보를 담는 구조체

char cmdLine[256];
char tempPath[256];
static int mutex_util_ps_list = 0;
int util_ps_list(char *ps_list) 
{
	int pid, chkpid;                         // 프로세스는 /proc 디렉토리에 자신의 pid로 파일을 담아 둡니다.
	
	while(mutex_util_ps_list>0) continue;	
		
	chkpid=0;	
	mutex_util_ps_list=1;
	if(mutex_util_ps_list) 
	{
		dir = opendir("/proc");   //  /proc이란 디렉토리 스트름이 대한 포인터가 반환되었습니다.	

		//printf("dir : %d\r\n", dir);

		if(dir > 0) {
			while ((entry = readdir(dir)) > 0) 	//  proc에 존재하는 파일들을 차례대로 읽습니다.
			{   
				//printf("d_name : %s\r\n", entry->d_name);
				
				lstat(entry->d_name, &fileStat);          // DIR*가 가리키는 파일의 state 정보를 가져온다.

				if (!S_ISDIR(fileStat.st_mode))            // is dir? 디렉토리인지 확인한다.
				continue;                                    	// 프로세스는 /proc에 자신의 pid로 디렉토리를   만드는 점을 안다면 이해하실거라 생각합니다.

				pid = atoi(entry->d_name);          	// 프로세스(디렉토리)인것을 확인하면, 숫자로 반환한다.
				if (pid <= 0) continue;                       // 숫자가 아니라면 다시 continue;

				sprintf(tempPath, "/proc/%d/cmdline", pid); 	// cmdline :: 프로세스 이름이 적힌파일
				getCmdLine(tempPath, cmdLine);     			// /proc/pid/cmdline에서 프로세스의 이름을
				                                     					// 가져오는 함수로 보냅니다. 아래에 정의되어있습니다.																		
				if(strnlen(cmdLine, sizeof(cmdLine))) 
				{				
					if(strstr(cmdLine, ps_list)) {
						chkpid = pid;
						printf("\t[%d] %s\r\n", pid, cmdLine);
						break;
					}
				}
				else pid = 0;
			}
			closedir(dir);		
		}
	}
	
	//printf("end\r\n\n");
	
	mutex_util_ps_list=0;	
	return chkpid ? pid : 0;
}


void util_visa_bin(char *pBuf, char *pBin) 
{
	static char pValue[1024];
	char pTAG[1024];
	int	nLen;
	
	nLen = VCPS_GetData(pBuf, pTAG, pValue, sizeof(pValue));
	nLen = nLen/2;	// CHAR 인 경우 VCPS_GetData 에서 값을 2로 ... 확인 팔요
	
	if(!strcmp(pTAG, "para")) {		
		HEXDUMP(pValue, nLen, "%s", pTAG);
	}
	else 
	if(!strcmp(pTAG, "info")) {		
		HEXDUMP(pValue, nLen, "%s", pTAG);
		memcpy(pBin, pValue, nLen);
	}
}

unsigned char util_visa_list(char *pBin)
{
	FILE *fp;
	CHAR ReadBuf[1024], *pPos;
	CHAR Title[10], AttrKey[10], AttrValue[100], VisaBin[1024];
	unsigned char ret = RC_FAILURE;

	if(GetCreditRange() != CREDIT_VISA_ONLY)  {
		return RC_SUCCESS;
	}

	PRINTF("# Load VISA Parameter Configuration\n");
			
	fp = fopen("/usr/rfpinpad/log/visa_bin_list.dat", "r");
	if(fp == NULL) {		
		PRINTF("Config File Open Error\n");		
		return RC_FAILURE;
	}
	
	HEX_Dump(pBin, 6, "VISA");

	while (fp != NULL) 
	{		
		memset(ReadBuf,	0x00, sizeof(ReadBuf));
		memset(Title,	0x00, sizeof(Title));

		if (fgets(ReadBuf, sizeof(ReadBuf), fp) != NULL) {
			if (  ReadBuf[0] == 0x00 ||ReadBuf[0] == 0x20 ||ReadBuf[0] == 0x0a)
			{
					break;
			}
			
			if(ReadBuf[0] == '#')
				continue;
			pPos = ReadBuf;
			pPos = CONF_GetTitle(ReadBuf, Title);
			
			if (pPos == NULL)
			{
				break;
			}
			
			if (strcmp(Title, "CONF") == 0) {
				pPos = CONF_GetAttribute(pPos, AttrKey, AttrValue);
				pPos = CONF_GetAttribute(pPos, AttrKey, AttrValue);
			} 			
			else if (strcmp(Title, "PARA") == 0) {
				util_visa_bin(pPos, VisaBin);
				if(!memcmp(VisaBin, pBin, 6)) {
					HEX_Dump(pBin, 6, "IN");
					ret = RC_SUCCESS;
					break;
				}
			} 
		}
	}

	fclose(fp);
	
	return ret;
}

unsigned char util_amex_list(char *pBin)
{
 	unsigned char ret=RC_FAILURE;

	if(!memcmp(pBin, "37", 2)) ret = RC_SUCCESS;

	return ret;
}
#endif


int set_systemtime(char *pDateTime)
{

    int              rtc;
    struct rtc_time  rtc_time_data;
    struct tm        tm_src;
    struct timeval   tv = { 0, 0 };
    unsigned long year;
    unsigned short mon, day, hour, min, sec;
    unsigned char buf[10];

    memset( buf, 0x00, sizeof( buf));
    memcpy( buf, pDateTime,   4);
    year= atoi( buf);
    memset( buf, 0x00, sizeof( buf));
    memcpy( buf, pDateTime+4, 2);
    mon= atoi( buf);
    memset( buf, 0x00, sizeof( buf));
    memcpy( buf, pDateTime+6, 2);
    day= atoi( buf);
    memset( buf, 0x00, sizeof( buf));
    memcpy( buf, pDateTime+8, 2);
    hour= atoi( buf);
    memset( buf, 0x00, sizeof( buf));
    memcpy( buf, pDateTime+10, 2);
    min= atoi( buf);
    memset( buf, 0x00, sizeof( buf));
    memcpy( buf, pDateTime+12, 2);
    sec= atoi( buf);



    tm_src.tm_year = rtc_time_data.tm_year = year - 1900;
    tm_src.tm_mon  = rtc_time_data.tm_mon  = mon  - 1;
    tm_src.tm_mday = rtc_time_data.tm_mday = day;
    tm_src.tm_hour = rtc_time_data.tm_hour = hour;
    tm_src.tm_min  = rtc_time_data.tm_min  = min;
    tm_src.tm_sec  = rtc_time_data.tm_sec  = sec;


    tv.tv_sec = mktime( &tm_src );

    settimeofday ( &tv, NULL );

//    check_rtc_devicefile();

 //   rtc = open ( "/dev/rtc", O_WRONLY );
 //   if( rtc < 0 )
 //   {
 //       perror( "/dev/rtc open error" );
 //       return -1;
 //   }

//    if ( ioctl ( rtc, RTC_SET_TIME, &rtc_time_data ) < 0 )
//    {
//        perror( "/dev/rtc rtc write time error" );
 //       close( rtc );
 //       return -1;
 //   }

 //   close( rtc );

   return 0;

}

