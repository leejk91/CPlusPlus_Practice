#include "damo_pkcrypt.h"

int DAMO_PKCRYPT_pk_encrypt( char* pubKeyFilePath, unsigned char* in, size_t inLen, unsigned char* out, size_t* outLen, int outMax)
{
  pk_context pk;
  int ret = -1;

  pk_init(&pk);

  ret = pk_parse_public_keyfile(&pk, pubKeyFilePath);
  if ( ret == POLARSSL_ERR_PK_FILE_IO_ERROR)
  {
    pk_free(&pk);
    return DAMO_PKCRYPT_ERR_PK_FILE_NOT_FOUND;
  }
  else if ( ret == POLARSSL_ERR_PK_KEY_INVALID_FORMAT)
  {
    pk_free(&pk);
    return DAMO_PKCRYPT_ERR_PK_KEY_INVALID_FORMAT;
  }
  else if ( ret != 0)
  {
    pk_free(&pk);
    return ret;
  }

  ret = pk_encrypt( &pk, in,inLen, out, outLen, outMax);
  
  if( ret != 0)
  {
    pk_free(&pk);
    return DAMO_PKCRYPT_ERR_ENCRYPT;
  }

  pk_free(&pk);
  return ret;
}

int DAMO_PKCRYPT_pk_decrypt( char* priKeyFilePath, unsigned char* in, size_t inLen, unsigned char* out, size_t* outLen, int outMax)
{
  pk_context vk;
  int ret = -1;

  pk_init(&vk);

  ret = pk_parse_keyfile(&vk, priKeyFilePath, "");
  if ( ret == POLARSSL_ERR_PK_FILE_IO_ERROR)
  {
    pk_free(&vk);
    return DAMO_PKCRYPT_ERR_PK_FILE_NOT_FOUND;
  }
  else if ( ret == POLARSSL_ERR_PK_KEY_INVALID_FORMAT)
  {
    pk_free(&vk);
    return DAMO_PKCRYPT_ERR_PK_KEY_INVALID_FORMAT;
  }

  ret = pk_decrypt( &vk, in, inLen, out, outLen, outMax);
  
  if ( ret != 0)
  {
    pk_free(&vk);
    return DAMO_PKCRYPT_ERR_DECRYPT;
  }

  pk_free(&vk);
  return ret;
}

int DAMO_PKCRYPT_pk_sign( char* priKeyFilePath, unsigned char* hash, size_t hashLen, unsigned char* sign, size_t* signLen)
{
  pk_context vk;
  int ret = -1;

  pk_init(&vk);
  
  ret = pk_parse_keyfile(&vk, priKeyFilePath, "");
  if ( ret == POLARSSL_ERR_PK_FILE_IO_ERROR)
  {
    pk_free(&vk);
    return DAMO_PKCRYPT_ERR_PK_FILE_NOT_FOUND;
  }
  else if ( ret == POLARSSL_ERR_PK_KEY_INVALID_FORMAT)
  {
    pk_free(&vk);
    return DAMO_PKCRYPT_ERR_PK_KEY_INVALID_FORMAT;
  }

  ret = pk_sign( &vk, POLARSSL_MD_SHA256, hash, hashLen, sign, signLen);

  if ( ret != 0)
  {
    pk_free(&vk);
    return DAMO_PKCRYPT_ERR_SIGN;
  }

  pk_free(&vk);
  return ret;
}

int DAMO_PKCRYPT_pk_verify( char* pubKeyFilePath, unsigned char* hash, size_t hashLen, unsigned char* sign, size_t signLen)
{
  pk_context pk;
  int ret = -1;

   pk_init(&pk);

   ret = pk_parse_public_keyfile(&pk, pubKeyFilePath);
   if ( ret == POLARSSL_ERR_PK_FILE_IO_ERROR)
   {
     pk_free(&pk);
     return DAMO_PKCRYPT_ERR_PK_FILE_NOT_FOUND;
   }
   else if ( ret == POLARSSL_ERR_PK_KEY_INVALID_FORMAT)
   {
     pk_free(&pk);
     return DAMO_PKCRYPT_ERR_PK_KEY_INVALID_FORMAT;
   }
   else if ( ret != 0)
   {
     pk_free(&pk);
     return ret;
   }

   ret = pk_verify( &pk, POLARSSL_MD_SHA256, hash, hashLen, sign, signLen);

   if ( ret != 0)
   {
     pk_free(&pk);
     return DAMO_PKCRYPT_ERR_VERIFY;
   }

   pk_free(&pk);
   return ret;
}
