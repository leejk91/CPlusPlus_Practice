#include <stdio.h>
#include <stdint.h>
#include <string.h>

#if defined(_MSC_VER) && !defined(EFIX64) && !defined(EFI32)
#include <basetsd.h>
typedef UINT32 uint32_t;
#else
#include <inttypes.h>
#endif

#define DAMO_CRYPT_DES_ENC     1
#define DAMO_CRYPT_DES_DEC     0

/* Return Value */
#define DAMO_CRYPT_SUCCESS                        00000 /* Success */
#define DAMO_CRYPT_ERR_DES_ENC_NULL_POINTER      -10100 /* DES암호화시 NULL 포인터 에러 */
#define DAMO_CRYPT_ERR_DES_ENC_INVALID_LEN       -10101 /* DES암호화시 유효하지 않는 길이 */
#define DAMO_CRYPT_ERR_DES_ENC_INVALID_ALG       -10102 /* DES암호화시 유효하지 않는 알고리즘 */
#define DAMO_CRYPT_ERR_DES_ENC_INVALID_MODE      -10103 /* DES암호화시 유효하지 않는 운용모드 */
#define DAMO_CRYPT_ERR_DES_ENC_KEY_SCH_FAIL      -10104 /* DES암호화시 키스케쥴링 실패 */
#define DAMO_CRYPT_ERR_DES_DEC_NULL_POINTER      -10105 /* DES복호화시 NULL 포인터 에러 */
#define DAMO_CRYPT_ERR_DES_DEC_INVALID_LEN       -10106 /* DES복호화시 유효하지 않는 길이 */
#define DAMO_CRYPT_ERR_DES_DEC_INVALID_ALG       -10107 /* DES복호화시 유효하지 않는 알고리즘 */
#define DAMO_CRYPT_ERR_DES_DEC_INVALID_MODE      -10108 /* DES복호화시 유효하지 않는 운용모드 */
#define DAMO_CRYPT_ERR_DES_DEC_KEY_SCH_FAIL      -10109 /* DES복호화시 키스케쥴링 실패 */

#define DES_KEY_SIZE    8

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    int is_enc;
    uint32_t sk[32];
}
DES_CTX;

typedef struct
{
    int is_enc;
    uint32_t sk[96];
}
DES3_CTX;

int DAMO_CRYPT_DES_Set_EKey( DES_CTX *ctx, const unsigned char key[DES_KEY_SIZE] );

int DAMO_CRYPT_DES_Set_DKey( DES_CTX *ctx, const unsigned char key[DES_KEY_SIZE] );

int DAMO_CRYPT_DES3_Set_EKey2( DES3_CTX *ctx, const unsigned char key[DES_KEY_SIZE * 2] );

int DAMO_CRYPT_DES3_Set_DKey2( DES3_CTX *ctx, const unsigned char key[DES_KEY_SIZE * 2] );

int DAMO_CRYPT_DES_Block( DES_CTX *ctx, const unsigned char input[8], unsigned char output[8] );

int DAMO_CRYPT_DES3_Block( DES3_CTX *ctx, const unsigned char input[8], unsigned char output[8] );

int DAMO_CRYPT_DES3_CBC( DES3_CTX *ctx, int is_enc, unsigned char iv[8],
		const unsigned char *input, size_t input_len, unsigned char *output );

#ifdef __cplusplus
}
#endif
