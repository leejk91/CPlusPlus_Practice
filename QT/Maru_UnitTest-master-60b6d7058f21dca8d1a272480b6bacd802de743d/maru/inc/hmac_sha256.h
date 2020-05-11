#include "sha256.h"

/* Return Value */
#define DAMO_CRYPT_SUCCESS                        00000 /* Success */
#define DAMO_CRYPT_ERR_HMAC_SHA256_NULL_POINTER  -30100 /* HMAC_SHA256���� NULL ������ ���� */

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	HASH_SHA256_CTX hash_ctx;
	unsigned char ipad[64]; /* inner padding */
	unsigned char opad[64]; /* outer padding */
} HMAC_SHA256_CTX;

/* DAMO_CRYPT_HMAC_SHA256
 * description : SHA256��� HMAC �˰��� �Լ�
 *   
 * parameter
 * 1. key(IN) : �Է� Ű
 * 2. key_len(IN) : �Է� Ű ����
 * 3. data(IN) : �Է� ������ ����
 * 4. data_len(IN) : �Է� ������ ����
 * 5. digest(OUT) : ��������Ʈ ����� ������ ������
 *      
 * return value
 *  if successful, ret = 0
 *  otherwise, ret < 0
 */
int DAMO_CRYPT_HMAC_SHA256(const unsigned char *key, size_t key_len, const unsigned char *data, size_t data_len, unsigned char *digest);

#ifdef __cplusplus
}
#endif
