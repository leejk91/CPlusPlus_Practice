#include "hmac.h"

#ifndef DAMO_CRYPT_HMAC_DRBG_H
#define DAMO_CRYPT_HMAC_DRBG_H

/* Return Value */
#define DAMO_CRYPT_SUCCESS                        			   00000 /* Success */
#define DAMO_CRYPT_ERR_HMAC_DRBG_REQUEST_TOO_BIG              -40100 /* �ѹ��� ȣ���� ������ ���� ��û */
#define DAMO_CRYPT_ERR_HMAC_DRBG_INPUT_TOO_BIG                -40101 /* ������ �Է� */
#define DAMO_CRYPT_ERR_HMAC_DRBG_FILE_IO_ERROR                -40102 /* ���Ͽ��� �б�/���� ���� */
#define DAMO_CRYPT_ERR_HMAC_DRBG_ENTROPY_SOURCE_FAILED        -40103 /* ��Ʈ���� �ҽ� ���� */

#define DAMO_CRYPT_HMAC_DRBG_MAX_INPUT_BYTES   256     /* �߰����� Input �ִ� ����Ʈ�� */
#define DAMO_CRYPT_HMAC_DRBG_MAX_REQUEST       1024    /* ȣ��� Request �ִ� ����Ʈ�� */
#define DAMO_CRYPT_HMAC_DRBG_MAX_SEED_BYTES    384     /* Reseed �ִ� ����Ʈ�� */

#define DAMO_CRYPT_HMAC_DRBG_PR_OFF   0   /* No Prediction Resistance */
#define DAMO_CRYPT_HMAC_DRBG_PR_ON    1   /* Prediction Resistance Enabled */

#define DAMO_CRYPT_DIGEST_MAX_SIZE	  64

#ifdef __cplusplus
extern "C" {
#endif

/* HMAC DRBG Context */
typedef struct
{
    HMAC_CTX hmac_ctx;                    					/* HMAC Context */
    unsigned char V[DAMO_CRYPT_DIGEST_MAX_SIZE];
    int reseed_counter;                   					/* Reseed Counter */
    size_t entropy_len;         							/* �� Seed���� ������ ��Ʈ���� ����Ʈ */
    int prediction_resistance;  							/* ���� ���� ��� Ȱ��ȭ(��� ���� ���� ���� �ڵ����� Reseed) */
    int reseed_interval;        							/* Reseed Interval */
    int (*entropy_func)(void *, unsigned char *, size_t);   /* ��Ʈ���� �Լ� */
    void *entropy_ctx;            							/* ��Ʈ���� �Լ��� ���� Context */
} HMAC_DRBG_CTX;


/* DAMO_CRYPT_HMAC_DRBG_Update
 * description : HMAC DRBG State ������Ʈ �Լ�
 *
 * parameter
 * 1. ctx(IN/OUT) : HMAC DRBG Context ������
 * 2. add_data(IN) : State�� ������Ʈ�� �߰����� ������
 * 3. add_data_len(IN) : State�� ������Ʈ�� �߰����� ������ ����
 *
 * return value
 *  no
 */
void DAMO_CRYPT_HMAC_DRBG_Update( HMAC_DRBG_CTX *ctx, const unsigned char *add_data, size_t add_data_len );

/* DAMO_CRYPT_HMAC_DRBG_Reseed
 * description : HMAC DRBG Reseeding �Լ�(��Ʈ���� �ҽ��κ��� ������ ����)
 *
 * parameter
 * 1. ctx(IN/OUT) : HMAC DRBG Context ������
 * 2. add_data(IN) : �߰����� ������
 * 3. add_data_len(IN) : �߰����� ������ ����
 *
 * return value
 *  if successful, ret = 0
 *  otherwise, ret < 0
 */
int DAMO_CRYPT_HMAC_DRBG_Reseed( HMAC_DRBG_CTX *ctx, const unsigned char *add_data, size_t add_data_len );

/* DAMO_CRYPT_HMAC_DRBG_Init
 * description : HMAC DRBG �ʱ�ȭ �Լ�
 *
 * parameter
 * 1. ctx(OUT) : �ʱ�ȭ�� HMAC DRBG Context ������
 * 2. hmac_type(IN) : HMAC �˰��� Ÿ��
 * 3. entropy_func(IN) : ��Ʈ���� ���� �Լ�
 * 4. entropy_ctx(IN) : ��Ʈ���� ���� �Լ� Context
 * 5. add_data(IN) : �߰����� ������(e.g. Device specific identifiers)
 * 6. add_data_len(IN) : �߰����� ������ ����
 *
 * return value
 *  if successful, ret = 0
 *  otherwise, ret < 0
 */
int DAMO_CRYPT_HMAC_DRBG_Init( HMAC_DRBG_CTX *ctx, HMAC_TYPE hmac_type,
		int (*entropy_func)(void *, unsigned char *, size_t), void *entropy_ctx,
		const unsigned char *add_data, size_t add_data_len );

/* DAMO_CRYPT_HMAC_DRBG_Destroy
 * description : HMAC DRBG Context ��ȯ �Լ�
 *
 * parameter
 * 1. ctx(IN/OUT) : HMAC DRBG Context ������
 *
 * return value
 *  no 
 */
void DAMO_CRYPT_HMAC_DRBG_Destroy( HMAC_DRBG_CTX *ctx );

/* DAMO_CRYPT_HMAC_DRBG_Random_With_Add
 * description : HMAC DRBG �������� �Լ�(�߰����� �����͸� �Է�)
 *
 * parameter
 * 1. ctx(IN/OUT) : HMAC DRBG Context ������
 * 2. output(OUT) : ���� �����͸� ä�� ����
 * 3. output_len(IN) : ������ ����
 * 4. add_data(IN) : �߰����� ������
 * 5. add_data_len(IN) : �߰����� ������ ����
 *
 * return value
 *  if successful, ret = 0
 *  otherwise, ret < 0
 */
int DAMO_CRYPT_HMAC_DRBG_Random_With_Add( HMAC_DRBG_CTX *ctx,
		unsigned char *output, size_t output_len,
		const unsigned char *add_data, size_t add_data_len );

/* DAMO_CRYPT_HMAC_DRBG_Random_Without_Add
 * description : HMAC DRBG �������� �Լ�(�߰����� �����͸� �Է����� ����)
 *
 * parameter
 * 1. ctx(IN/OUT) : HMAC DRBG Context ������
 * 2. output(OUT) : ���� �����͸� ä�� ����
 * 3. output_len(IN) : ������ ����
 *
 * return value
 *  if successful, ret = 0
 *  otherwise, ret < 0
 */
int DAMO_CRYPT_HMAC_DRBG_Random_Without_Add( HMAC_DRBG_CTX *ctx,
		unsigned char *output, size_t output_len);

/* DAMO_CRYPT_HMAC_DRBG_Set_Prediction_Resistance
 * description : Prediction Resistance ���� �Լ� (Default: Off)
 *
 * parameter
 * 1. ctx(IN/OUT) : HMAC DRBG Context ������
 * 2. resistance(IN) : Prediction Resistance ������
 *
 * return value
 *  no 
 */
void DAMO_CRYPT_HMAC_DRBG_Set_Prediction_Resistance( HMAC_DRBG_CTX *ctx, int resistance );

/* DAMO_CRYPT_HMAC_DRBG_Set_Entropy_Len 
 * description : ��Ʈ���� ���� ���� �Լ�
 *
 * parameter
 * 1. ctx(IN/OUT) : HMAC DRBG Context ������
 * 2. len(IN) : ��Ʈ���� ���� ������
 *
 * return value
 *  no 
 */
void DAMO_CRYPT_HMAC_DRBG_Set_Entropy_Len( HMAC_DRBG_CTX *ctx, size_t len );

/* DAMO_CRYPT_HMAC_DRBG_Set_Reseed_Interval 
 * description : Reseed �ֱ� ���� �Լ�
 *
 * parameter
 * 1. ctx(IN/OUT) : HMAC DRBG Context ������
 * 2. interval(IN) : Reseed �ֱ� ������
 *
 * return value
 *  no 
 */
void DAMO_CRYPT_HMAC_DRBG_Set_Reseed_Interval( HMAC_DRBG_CTX *ctx, int interval );

#ifdef __cplusplus
}
#endif

#endif
