#include <string.h>

#if defined(_MSC_VER) && !defined(EFIX64) && !defined(EFI32)
#include <basetsd.h>
typedef UINT32 uint32_t;
#else
#include <inttypes.h>
#endif

/* Return Value */
#define DAMO_CRYPT_SUCCESS                        00000 /* Success */
#define DAMO_CRYPT_ERR_HASH_SHA256_NULL_POINTER  -20100 /* HASH SHA256���� NULL ������ ���� */

#ifdef __cplusplus
extern "C" {
#endif

/* SHA256 Context */
typedef struct
{
    uint32_t total[2]; /* ó���� ����Ʈ�� */
    uint32_t state[8]; /* �ؽ� �߰� ���� */
    unsigned char buffer[64]; /* ó���� ������ ��� */
}
HASH_SHA256_CTX;

void DAMO_CRYPT_HASH_SHA256_Init( HASH_SHA256_CTX *ctx );

void DAMO_CRYPT_HASH_SHA256_Update( HASH_SHA256_CTX *ctx, const unsigned char *data, size_t data_len );

void DAMO_CRYPT_HASH_SHA256_Final( HASH_SHA256_CTX *ctx, unsigned char digest[32] );

/* DAMO_CRYPT_HASH_SHA256
 * description : SHA256 �ؽ� �˰��� �Լ�
 * 
 * parameter
 * 1. data(IN) : �Է� ������ ������
 * 2. data_len(IN) : �Է� ������ ����
 * 3. digest(OUT) : ��������Ʈ ����� ������ ������
 *
 * return value
 *  if successful, ret = 0
 *  otherwise, ret < 0
 */
int DAMO_CRYPT_HASH_SHA256( const unsigned char *data, size_t data_len, unsigned char digest[32] );

void *DAMO_CRYPT_KDF2_SHA256( const void *in, size_t in_len, void *out, size_t *out_len );

#ifdef __cplusplus
}
#endif
