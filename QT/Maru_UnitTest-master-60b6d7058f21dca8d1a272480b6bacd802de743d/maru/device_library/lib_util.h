#ifndef __LIB_UTIL_H__
#define __LIB_UTIL_H__

#include <time.h>
#include <sys/timeb.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SEMA_KEY_1	1111
#define SEMA_KEY_2	1112
#define SEMA_KEY_3	1113


typedef union semun {
   	int val;
   	struct semid_ds *buf;
   	unsigned short int *array;
} semum_arg;


#if 0
extern void PrintHex(unsigned char *nAddr, int nLen, char *sFrom, ...);
extern void HEX_Dump(unsigned char *nAddr, int nLen, char *sFrom, ...);
#endif

extern void CRC_GenerateTable( unsigned short aPoly );
extern unsigned short CRC_Update(unsigned char *data_blk_ptr, unsigned short data_blk_size);

extern void chEndian(char* des, char* src, int len);
extern void HexStr2Byte(char *pHexStr, int nSize, unsigned char *pByte);
extern void Int2Byte(unsigned int inVal, unsigned char *outVal);
extern int Char2BCD(unsigned char *inChar, unsigned char *outBCD, int size);
extern int BCD2Char(unsigned char *inBCD, unsigned char *outChar, int size);
extern unsigned char bcd2hex(unsigned char *pDes, unsigned char *pSrc);
extern int ByteToInt(unsigned char *pIn) ;
extern void Hex_To_Ascii(unsigned char c, unsigned char asc[]);
extern void Hex2Ascii(unsigned char *pHex, unsigned char *pAscii, int nHexLen);
extern int Char2Int(unsigned char *pIn, int nType) ;

extern void Swap_4(unsigned char *pHex1, unsigned char *pHex2);
extern void Swap_2(unsigned char *pHex1, unsigned char *pHex2);

extern void Util_ChangeEndian(unsigned char *pDes, unsigned char *pSrc, int nLen);

extern void GetTimeString(time_t tTime, char *pRet);
extern void GetTimeBCD(time_t tTime, char *pRet);
extern clock_t CheckExpireTick(clock_t tStart);

extern int Ascii2BCD(unsigned char *str,int size, unsigned char *pBCD);
extern time_t GetBCDDateTimeTick(unsigned char *pDate);

#if 0
extern int util_strptime(time_t t);
extern void util_usb_log(char *fmt,...);

#endif

int set_systemtime(char *pDateTime);

#ifdef __cplusplus
}
#endif

#endif	// __LIB_UTIL_H__

