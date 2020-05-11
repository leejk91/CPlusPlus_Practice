#include <string.h>

#define DAMO_CRYPT_ERR_BASE64_BUFFER_TOO_SMALL               -50000  /* Output buffer too small. */
#define DAMO_CRYPT_ERR_BASE64_INVALID_CHARACTER              -50001  /* Invalid character in input. */

#ifdef __cplusplus
extern "C" {
#endif

int DAMO_CRYPT_Base64_Encode( unsigned char *dst, size_t *dlen, const unsigned char *src, size_t slen );

int DAMO_CRYPT_Base64_Decode( unsigned char *dst, size_t *dlen, const unsigned char *src, size_t slen );

#ifdef __cplusplus
}
#endif
