#include <stdio.h>
#include <string.h>
#include "x9tr31.h"
#include "des.h"

#ifdef DUKPT_SERVER
#include "dukpt_server_module.h"
#endif

void DAMO_X9TR31_XOR(unsigned char *in, int dataSize, unsigned char *out)
{
  int i;
  for(i = 0; i < dataSize; i++)
  {
    out[i] ^= in[i];
  }
}

void DAMO_X9TR31_leftBitShift(unsigned char *in, int dataSize, unsigned char *out)
{
  int i=0;

  memset(out, 0x00, dataSize);

  if ( dataSize>0)
    out[0] = in[0] << 1;

  for ( i=1; i<dataSize; i++ )
  {
    if ( in[i] & 0x80 ) 
      out[i-1] += 1;
    out[i] = in[i] << 1;
  }
}

void DAMO_X9TR31_GetSubKey(unsigned char *keyBlockProtectionKey, unsigned char *K1, unsigned char *K2, int compatibility)
{
  unsigned char R64[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1B };

  unsigned char S[8] = {0};

  unsigned char inputData[8] = {0 };
  DES3_CTX ctx;

  DAMO_CRYPT_DES3_Set_EKey2( &ctx, keyBlockProtectionKey );
  DAMO_CRYPT_DES3_CBC( &ctx, DAMO_CRYPT_DES_ENC, inputData, inputData, sizeof(inputData), S );    

  DAMO_X9TR31_leftBitShift(S, 8, K1);

  if ( (S[0] & 0x80) || compatibility )
    DAMO_X9TR31_XOR(R64, 8, K1);

  DAMO_X9TR31_leftBitShift(K1, 8, K2);

  if ( (K1[0] & 0x80) || compatibility )
    DAMO_X9TR31_XOR(R64, 8, K2);
}

void DAMO_X9TR31_GetKeyBlockEncryptionKey(unsigned char *keyBlockProtectionKey, unsigned char *subKey, unsigned char *encryptionKey)
{
  unsigned char inputData1[8] = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80};
  unsigned char inputData2[8] = {0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80};
  DES3_CTX ctx;

  DAMO_CRYPT_DES3_Set_EKey2( &ctx, keyBlockProtectionKey );
  DAMO_CRYPT_DES3_CBC( &ctx, DAMO_CRYPT_DES_ENC, subKey, inputData1, sizeof(inputData1), encryptionKey );    
  DAMO_CRYPT_DES3_CBC( &ctx, DAMO_CRYPT_DES_ENC, subKey, inputData2, sizeof(inputData2), &encryptionKey[8] );    
}

void DAMO_X9TR31_GetKeyBlockMACKey(unsigned char *keyBlockProtectionKey, unsigned char *subKey, unsigned char *MACKey)
{
  unsigned char inputData1[8] = {0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x80};
  unsigned char inputData2[8] = {0x02, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x80};
  DES3_CTX ctx;

  DAMO_CRYPT_DES3_Set_EKey2( &ctx, keyBlockProtectionKey );
  DAMO_CRYPT_DES3_CBC( &ctx, DAMO_CRYPT_DES_ENC, subKey, inputData1, sizeof(inputData1), MACKey );    
  DAMO_CRYPT_DES3_CBC( &ctx, DAMO_CRYPT_DES_ENC, subKey, inputData2, sizeof(inputData2), &MACKey[8] );    
}

void DAMO_X9TR31_Get_Enc_Mac_Key(unsigned char *keyBlockProtectionKey, unsigned char *encryptionKey, unsigned char *MACKey)
{
  unsigned char K1[8];
  unsigned char K2[8];

#ifdef DUKPT_SERVER
  if( initValues.msbCheck == DAMO_TR31_STD )
  {
	DAMO_X9TR31_GetSubKey(keyBlockProtectionKey, K1, K2, 0);
  }
  else
  {
    DAMO_X9TR31_GetSubKey(keyBlockProtectionKey, K1, K2, 1);
  }
  
  
#elif defined(DUKPT_CLIENT_TR31_STD)
  DAMO_X9TR31_GetSubKey(keyBlockProtectionKey, K1, K2, 0);
#else
  DAMO_X9TR31_GetSubKey(keyBlockProtectionKey, K1, K2, 1);
#endif

  DAMO_X9TR31_GetKeyBlockEncryptionKey(keyBlockProtectionKey, K1, encryptionKey);
  DAMO_X9TR31_GetKeyBlockMACKey(keyBlockProtectionKey, K1, MACKey);
}
