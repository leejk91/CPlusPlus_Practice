/* Return Value */
#define DAMO_CRYPT_SUCCESS                        00000 /* Success */
#define DAMO_CRYPT_ERR_RNG_NULL_POINTER          -40000 /* ���������� NULL ������ ���� */
#define DAMO_CRYPT_ERR_RNG_INTERNAL_INIT_FAIL    -40001 /* ���������� ���� �ʱ�ȭ ���� */
#define DAMO_CRYPT_ERR_RNG_GEN_RAND_FAIL         -40002 /* �������� ���� */

#ifdef __cplusplus
extern "C" {
#endif

/* DAMO_CRYPT_RNG
 * description : ���� ���� �Լ�
 *   
 * parameter
 * 1. out(OUT) : ������ ������ ������ ������
 * 2. out_len(IN) : ������ ������ ����
 *      
 * return value
 *  if successful, ret = 0
 *  otherwise, ret < 0
 */
int DAMO_CRYPT_RNG( unsigned char *out, size_t out_len );

#ifdef __cplusplus
}
#endif
