#ifndef DAMO_PKCRYPT_H
#define DAMO_PKCRYPT_H

#ifndef POLARSSL_CONFIG_H
#include "ps_config.h"
#endif
#ifndef POLARSSL_PK_H
#include "ps_pk.h"
#endif

#define DAMO_PKCRYPT_ERR_PK_FILE_NOT_FOUND 205
#define DAMO_PKCRYPT_ERR_PK_KEY_INVALID_FORMAT 206
#define DAMO_PKCRYPT_ERR_ENCRYPT 305
#define DAMO_PKCRYPT_ERR_DECRYPT 306
#define DAMO_PKCRYPT_ERR_SIGN 405
#define DAMO_PKCRYPT_ERR_VERIFY 406

int DAMO_PKCRYPT_pk_encrypt( 
  char    *pubKeyFilePath, 
  unsigned char    *in, 
  size_t   inLen, 
  unsigned char    *out, 
  size_t  *outLen, 
  int      outMax);

int DAMO_PKCRYPT_pk_decrypt(
  char    *priKeyFilePath, 
  unsigned char    *in, 
  size_t   inLen, 
  unsigned char    *out, 
  size_t  *outLen, 
  int      outMax);

int DAMO_PKCRYPT_pk_sign( 
  char    *priKeyFilePath, 
  unsigned char    *hash, 
  size_t   hashLen, 
  unsigned char    *sign, 
  size_t  *signLen);

int DAMO_PKCRYPT_pk_verify( 
  char    *pubKeyFilePath, 
  unsigned char    *hash, 
  size_t   hashLen, 
  unsigned char    *sign, 
  size_t   signLen);

#endif
