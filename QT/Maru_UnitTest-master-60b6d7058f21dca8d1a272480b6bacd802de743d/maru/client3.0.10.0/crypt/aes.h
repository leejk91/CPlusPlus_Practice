#include <stdio.h>
#include <stdint.h>
#include <string.h>

#if defined(EXPORT_DUKPTCRYPT)
#define LIBSPEC_DUKPTCRYPT  __declspec(dllexport)
#elif defined(IMPORT_DUKPTCRYPT)
#define LIBSPEC_DUKPTCRYPT  __declspec(dllimport)
#else
#define LIBSPEC_DUKPTCRYPT
#endif

#if defined(_MSC_VER) && !defined(EFIX64) && !defined(EFI32)
#include <basetsd.h>
typedef UINT32 uint32_t;
#else
#include <inttypes.h>
#endif

#define DAMO_CRYPT_AES_ENC     1
#define DAMO_CRYPT_AES_DEC     0

/* Return Value */
#define DAMO_CRYPT_SUCCESS                        00000 /* Success */
#define DAMO_CRYPT_ERR_AES_ENC_NULL_POINTER      -10000 /* AES��ȣȭ�� NULL ������ ���� */
#define DAMO_CRYPT_ERR_AES_ENC_INVALID_LEN       -10001 /* AES��ȣȭ�� ��ȿ���� �ʴ� ���� */
#define DAMO_CRYPT_ERR_AES_ENC_INVALID_ALG       -10002 /* AES��ȣȭ�� ��ȿ���� �ʴ� �˰��� */
#define DAMO_CRYPT_ERR_AES_ENC_INVALID_MODE      -10003 /* AES��ȣȭ�� ��ȿ���� �ʴ� ����� */
#define DAMO_CRYPT_ERR_AES_ENC_KEY_SCH_FAIL      -10004 /* AES��ȣȭ�� Ű�����층 ���� */
#define DAMO_CRYPT_ERR_AES_DEC_NULL_POINTER      -10005 /* AES��ȣȭ�� NULL ������ ���� */
#define DAMO_CRYPT_ERR_AES_DEC_INVALID_LEN       -10006 /* AES��ȣȭ�� ��ȿ���� �ʴ� ���� */
#define DAMO_CRYPT_ERR_AES_DEC_INVALID_ALG       -10007 /* AES��ȣȭ�� ��ȿ���� �ʴ� �˰��� */
#define DAMO_CRYPT_ERR_AES_DEC_INVALID_MODE      -10008 /* AES��ȣȭ�� ��ȿ���� �ʴ� ����� */
#define DAMO_CRYPT_ERR_AES_DEC_KEY_SCH_FAIL      -10009 /* AES��ȣȭ�� Ű�����층 ���� */
#define DAMO_CRYPT_ERR_AES_DEC_INVALID_PAD       -10010 /* AES��ȣȭ�� �е����� ���� */

typedef enum {
	AES_128,
	AES_192,
	AES_256
} AES_TYPE;

typedef enum {
	CBC_MODE,
	CFB_MODE
} MODE_TYPE;

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    int nr;                     /*!<  number of rounds  */
    uint32_t *rk;               /*!<  AES round keys    */
    uint32_t buf[68];           /*!<  unaligned data    */
}
AES_CTX;

/* DAMO_CRYPT_AES_Encrypt
 * description : AES�˰��� ��ȣȭ �Լ�
 *   
 * parameter
 * 1. out(OUT) : ��ȣ�� �����͸� ������ ������
 * 2. out_len(OUT) : ��ȣ�� ������ ���̸� ������ ������
 * 3. in(IN) : �� ������
 * 4. in_len(IN) : �� ������ ����
 * 5. key(IN) : �Է� Ű
 * 6. key_len(IN) : �Է� Ű ����
 * 7. alg(IN) : AES�˰��� Ÿ�� (AES_128, AES_192, AES_256)
 * 8. mode(IN) : ��Ͼ�ȣ ���� (CBC_MODE , CFB_MODE)
 *      
 * return value
 *  if successful, ret = 0
 *  otherwise, ret < 0
 */
LIBSPEC_DUKPTCRYPT
int DAMO_CRYPT_AES_Encrypt(unsigned char *out, size_t *out_len,
		const unsigned char *in, size_t in_len, const unsigned char *key, size_t key_len, AES_TYPE alg, MODE_TYPE mode);

/* DAMO_CRYPT_AES_Decrypt
 * description : AES�˰��� ��ȣȭ �Լ�
 *   
 * parameter
 * 1. out(OUT) : �� �����͸� ������ ������
 * 2. out_len(OUT) : �� ������ ���̸� ������ ������
 * 3. in(IN) : ��ȣ�� ������
 * 4. in_len(IN) : ��ȣ�� ������ ����
 * 5. key(IN) : �Է� Ű
 * 6. key_len(IN) : �Է� Ű ����
 * 7. alg(IN) : AES�˰��� Ÿ�� (AES_128, AES_192, AES_256)
 * 8. mode(IN) : ��Ͼ�ȣ ���� (CBC_MODE , CFB_MODE)
 *      
 * return value
 *  if successful, ret = 0
 *  otherwise, ret < 0
 */
int DAMO_CRYPT_AES_Decrypt(unsigned char *out, size_t *out_len,
		const unsigned char *in, size_t in_len, const unsigned char *key, size_t key_len, AES_TYPE alg, MODE_TYPE mode);

/* DAMO_CRYPT_AES_EncryptEx
 * description : AES�˰��� ��ȣȭ �Լ�, Initial Vector�� ������ �� �ִ�.
 *   
 * parameter
 * 1. out(OUT) : ��ȣ�� �����͸� ������ ������
 * 2. out_len(OUT) : ��ȣ�� ������ ���̸� ������ ������
 * 3. in(IN) : �� ������
 * 4. in_len(IN) : �� ������ ����
 * 5. key(IN) : �Է� Ű
 * 6. key_len(IN) : �Է� Ű ����
 * 7. alg(IN) : AES�˰��� Ÿ�� (AES_128, AES_192, AES_256)
 * 8. mode(IN) : ��Ͼ�ȣ ���� (CBC_MODE , CFB_MODE)
 * 9. ivec(IN) : ��Ͼ�ȣ Initial Vector ( 16byte )
 *      
 * return value
 *  if successful, ret = 0
 *  otherwise, ret < 0
 */
LIBSPEC_DUKPTCRYPT
int DAMO_CRYPT_AES_EncryptEx(unsigned char *out, size_t *out_len,
		const unsigned char *in, size_t in_len, const unsigned char *key, size_t key_len, AES_TYPE alg, MODE_TYPE mode, unsigned char *ivec);

/* DAMO_CRYPT_AES_DecryptEx
 * description : AES�˰��� ��ȣȭ �Լ�, Initial Vector�� ������ �� �ִ�.
 *   
 * parameter
 * 1. out(OUT) : �� �����͸� ������ ������
 * 2. out_len(OUT) : �� ������ ���̸� ������ ������
 * 3. in(IN) : ��ȣ�� ������
 * 4. in_len(IN) : ��ȣ�� ������ ����
 * 5. key(IN) : �Է� Ű
 * 6. key_len(IN) : �Է� Ű ����
 * 7. alg(IN) : AES�˰��� Ÿ�� (AES_128, AES_192, AES_256)
 * 8. mode(IN) : ��Ͼ�ȣ ���� (CBC_MODE , CFB_MODE)
 * 9. ivec(IN) : ��Ͼ�ȣ Initial Vector ( 16byte )
 *      
 * return value
 *  if successful, ret = 0
 *  otherwise, ret < 0
 */
int DAMO_CRYPT_AES_DecryptEx(unsigned char *out, size_t *out_len,
		const unsigned char *in, size_t in_len, const unsigned char *key, size_t key_len, AES_TYPE alg, MODE_TYPE mode, unsigned char *ivec);
#ifdef __cplusplus
}
#endif
