#include "dukpt.h"

#ifndef DUKPT_SERVER
#include "dukpt_cli_config.h"
#endif

/* #define _DEBUG
 */
#if defined(EXPORT_DUKPTCLIENT)
#define LIBSPEC_DUKPTCLIENT  __declspec(dllexport)
#elif defined(IMPORT_DUKPTCLIENT)
#define LIBSPEC_DUKPTCLIENTT  __declspec(dllimport)
#else
#define LIBSPEC_DUKPTCLIENT
#endif

/* Ŭ���̾�Ʈ���� DUKPT Ű �ʱ�ȭ �� ��ȸ�� Ű ������ ���� �ڵ� */
#define DAMO_DUKPT_SUCCESS                         000000 /* Success */
#define DAMO_DUKPT_ERR_CLI_LIK_NULL_POINTER       -101000 /* DUKPT �ʱ�ȭ Ű �ε��� NULL ������ ���� */
#define DAMO_DUKPT_ERR_CLI_LIK_INVALID_LEN        -101001 /* DUKPT �ʱ�ȭ Ű �ε��� ��ȿ���� �ʴ� ���� */
#define DAMO_DUKPT_ERR_CLI_RPE_NULL_POINTER       -101002 /* DUKPT ��ȸ�� Ű ������ NULL ������ ���� */
#define DAMO_DUKPT_ERR_CLI_RPE_INVALID_LEN        -101003 /* DUKPT ��ȸ�� Ű ������ ��ȿ���� �ʴ� ���� */

#define DAMO_DUKPT_ERR_CLI_CODE_BASE_PK_AUTH_START -1000000
#define DAMO_DUKPT_ERR_CLI_CODE_BASE_PK_AUTH_END   -2000000
#define DAMO_DUKPT_ERR_CLI_CODE_BASE_REQUEST_ENTRY -3000000

#define DAMO_DUKPT_ERR_CLI_INVALID_INPUT_LEN              -1 /* DUKPT �߸��� �Է� ���� */
#define DAMO_DUKPT_ERR_CLI_NULL_POINTER                   -2 /* DUKPT NULL ����Ʈ ���� */
#define DAMO_DUKPT_ERR_CLI_INVALID_INPUT_DATA             -3

#define DAMO_CRYPT_ERR_CLI_PK_AUTH_END_VERIFY_MAC         -4
#define DAMO_DUKPT_ERR_CLI_PK_AUTH_END_VERIFY_KSN         -5
#define DAMO_DUKPT_ERR_CLI_PK_AUTH_END_VERIFY_IPEK_LENGTH -6
#define DAMO_DUKPT_ERR_CLI_PK_AUTH_END_VERIFY_HASH        -7

#define DAMO_DUKPT_ERR_CLI_MAXIMUM_COUNT                -8
#define DAMO_DUKPT_WARNING_CLI_LIMIT_COUNT              -9

#ifdef __cplusplus
extern "C" {
#endif

/* DAMO_DUKPT_Load_Initial_Key
 * description : DUKPT �ʱ�ȭ Ű �ε� �Լ�
 *   
 * parameter
 * 1. ipek(IN) : 16����Ʈ �ʱ� PIN ��ȣȭ Ű
 * 2. ipek_len(IN) : �ʱ� PIN ��ȣȭ Ű ����
 * 3. ksn(IN) : 10 ����Ʈ Key Serial Number ( [59��Ʈ Initial Key Serial Number]||[21��Ʈ 0] )
 * 4. ksn_len(IN) : Key Serial Number ����
 *      
 * return value
 *  if successful, ret = 0
 *  otherwise, ret < 0
 */
LIBSPEC_DUKPTCLIENT
int DAMO_DUKPT_Load_Initial_Key(const unsigned char *ipek, size_t ipek_len,
		const unsigned char *ksn, size_t ksn_len);

/* DAMO_DUKPT_Request_Pin_Entry
 * description : ��ȣȭ�� PIN ������ ���� �� MAC Key/Data Encryption Key ���� �Լ�
 *   
 * parameter
 * 1. pin(IN) : �Է��� PIN ������
 * 2. pin_len(IN) : �Է��� PIN ������ ����
 * 3. account_num(IN) : ī�� ��ȣ ������
 * 4. account_num_len(IN) : ī�� ��ȣ ������ ����
 * 5. flags(IN) : PIN ��ȣȭ Ű / Data MAC Ű / Data ��ȣȭ Ű ��뿩�� �÷���
 * 6. pEntry(OUT) : ��ȣȭ�� PIN ������ �� MAC KEY �׸��� Data Encryption Key�� ������ ����ü ������
 *      
 * return value
 *  if successful, ret = 0
 *  otherwise, ret < 0
 */
LIBSPEC_DUKPTCLIENT
int DAMO_DUKPT_Request_Pin_Entry(const unsigned char *pin, size_t pin_len,
		const unsigned char *account_num, size_t account_num_len, int flags, DukptPinEntry *pEntry);

/* DAMO_DUKPT_Export_Future_Key_Info
 * description : KSN�� Future Key Set�� �������� �Լ�
 *   
 * parameter
 * 1. fki(OUT) : KSN�� Future Key Set�� ������ ����ü ������ 
 *      
 * return value
 *  if successful, ret = 0
 *  otherwise, ret < 0
 */
LIBSPEC_DUKPTCLIENT
int DAMO_DUKPT_Export_Future_Key_Info(DukptFutureKeyInfo *fki);
LIBSPEC_DUKPTCLIENT
int DAMO_DUKPT_Export_Future_Key_Info_Ek(unsigned char *external_key, DukptFutureKeyInfo *fki);

/* DAMO_DUKPT_Import_Future_Key_Info
 * description : KSN�� Future Key Set�� �����ϴ� �Լ�
 *   
 * parameter
 * 1. fki(IN) : ������ KSN�� Future Key Set�� ����ü 
 *      
 * return value
 *  if successful, ret = 0
 *  otherwise, ret < 0
 */
LIBSPEC_DUKPTCLIENT
int DAMO_DUKPT_Import_Future_Key_Info(DukptFutureKeyInfo *fki);
LIBSPEC_DUKPTCLIENT
int DAMO_DUKPT_Import_Future_Key_Info_Ek(unsigned char *external_key, DukptFutureKeyInfo *fki);

/* DAMO_DUKPT_Power_On_Reset
 * description : ����ý� Future Key Set�� ���� Encryption Counter�� Reset��Ű�� �Լ�
 */
void DAMO_DUKPT_Power_On_Reset();

/* DAMO_DUKPT_AES_Encrypt
 * description : DUKPT AES ��ȣȭ �Լ�(fki import --> DUKPT TK ���� --> AES CBC 128bit enc --> fki export)
 *   
 * parameter
 * 1. enc(OUT) : ��ȣ�� �����͸� ������ ������
 * 2. enc_len(OUT) : ��ȣ�� ������ ���̸� ������ ������
 * 3. KSN(OUT) : KSN �����͸� ������ ������
 * 4. KSN_len(OUT) : KSN ������ ���̸� ������ ������
 * 5. org(IN) : �� ������
 * 6. org_len(IN) : �� ������ ����
 * 7. fki(IN/OUT) : ������ KSN�� Future Key Set�� ����ü
 *            
 * return value
 *  if successful, ret = 0
 *  otherwise, ret < 0
 */
int DAMO_DUKPT_AES_Encrypt(unsigned char *enc, size_t *enc_len, unsigned char *KSN, size_t *KSN_len, const unsigned char *org, size_t org_len, DukptFutureKeyInfo *fki);

/* DAMO_DUKPT_Client_PK_Auth_Start
 * description : �������� ���� �����Ϳ� ����Ű�� ����� �����͸� �Է¹޾� ������ �� IPEK�� ��ȣȭ�ϱ� ���� Ű�� �����ϱ� ���� Ű�� ����Ű�� ��ȣȭ�ϴ� �Լ�
 *   
 * parameter
 * 1. publicKeyFilePath(IN) : ����Ű ���� ��ġ, pem ������ �Է¹���. ��) /home/user/public.pem 
 * 2. SrandData(IN)         : �����ʿ��� ���� ������ ������, HEX
 * 3. HashData(IN)          : �����ʿ��� ���� ������ �������� SHA256 �ؽ� ������, HEX
 * 4. SignValue(IN)         : �����ʿ��� ���� ������ �������� ����Ű ���� ������, HEX
 * 5. OutRandValue(OUT)     : IPEK�� ��ȣȭ�ϱ� ���� Ű�� �����ϱ� ���� ���� Ű�� ������ ������, RAW
 * 6. OutEncRandValue(OUT)  : IPEK�� ��ȣȭ�ϱ� ���� Ű�� �����ϱ� ���� ���� Ű�� ����Ű�� ��ȣȭ�Ͽ� ������ ������, HEX
 *            
 * return value
 *  if successful, ret = 0
 *  otherwise, ret < 0
 */
LIBSPEC_DUKPTCLIENT
int DAMO_DUKPT_Client_PK_Auth_Start(unsigned char *publicKeyFilePath, unsigned char *SrandData, unsigned char *HashData, unsigned char *SignValue, unsigned char OutRandValueByte[16], unsigned char *OutEncRandValue);

/* DAMO_DUKPT_Client_PK_Auth_End
 * description : �������� ���� IPEK X9.TR32 ��ȯ ������(��ȣȭ �� MAC )�� ��ȣȭ�Ͽ� IPEK�� ��� �Լ�
 *   
 * parameter
 * 1. publicKeyFilePath(IN) : ����Ű ���� ��ġ, pem ������ �Է¹���. ��) /home/user/public.pem 
 * 2. EncIpekValue(IN)      : �����ʿ��� ���� IPEK X9 TR31 ��ȯ ������, HEX
 * 3. ksn(IN)               : ������ ��û�� ksn ������, RAW
 * 4. RandValue(IN)         : DAMO_DUKPT_Client_Verify_Encrypt���� ������ IPEK�� ��ȣȭ�ϱ� ���� Ű�� �����ϱ� ���� ���� ������, RAW
 * 5. IpekValue(OUT)        : EncIpekValue�� X9 TR31 ��ȣȭ �� MAC �����Ͽ� ���� IPEK�� ������ ������, RAW
 *            
 * return value
 *  if successful, ret = 0
 *  otherwise, ret < 0
 */
LIBSPEC_DUKPTCLIENT
int DAMO_DUKPT_Client_PK_Auth_End(unsigned char *publicKeyFilePath, unsigned char *EncIpekValue, unsigned char *ksn, unsigned char randValueByte[16], unsigned char IpekValue[16]);

#ifdef __cplusplus
}
#endif
