#define AP_TS_SIZE 25
#define AP_PSK_SIZE 16
#define AP_TID_SIZE 7
#define AP_RAND_SIZE 16
#define AP_KEY_SIZE 16
#define AP_PSK_TID_SIZE (AP_PSK_SIZE+AP_TID_SIZE)
#define AP_PSK_TID_RAND_SIZE (AP_PSK_SIZE+AP_TID_SIZE+AP_RAND_SIZE)
#define AP_TID_RAND_SIZE (AP_TID_SIZE+AP_RAND_SIZE)
#define AP_KEY_RAND_SIZE (AP_KEY_SIZE+AP_RAND_SIZE)
#define AP_CHAL_SIZE 55
#define AP_RESP_SIZE 48
#define AP_SID_SIZE 16
#define AP_SID_TID_SIZE (AP_SID_SIZE+AP_TID_SIZE)
#define AP_SID_TID_RAND_SIZE (AP_SID_SIZE+AP_TID_SIZE+AP_RAND_SIZE)

/* Return Value */
#define DAMO_DUKPT_SUCCESS                        000000 /* Success */
#define DAMO_DUKPT_ERR_AP_GTK_NULL_POINTER       -100000 /* �ӽ�Ű ������ NULL ������ ���� */
#define DAMO_DUKPT_ERR_AP_GTK_INVALID_LEN        -100001 /* �ӽ�Ű ������ ��ȿ���� �ʴ� ���� */
#define DAMO_DUKPT_ERR_AP_GTK_GET_TKEY_FAIL      -100002 /* �ӽ�Ű ���� ���� */
#define DAMO_DUKPT_ERR_AP_MC_NULL_POINTER        -100003 /* Challenge ������ NULL ������ ���� */
#define DAMO_DUKPT_ERR_AP_MC_INVALID_LEN         -100004 /* Challenge ������ ��ȿ���� �ʴ� ���� */
#define DAMO_DUKPT_ERR_AP_VC_NULL_POINTER        -100005 /* Challenge ������ NULL ������ ���� */
#define DAMO_DUKPT_ERR_AP_VC_INVALID_LEN         -100006 /* Challenge ������ ��ȿ���� �ʴ� ���� */
#define DAMO_DUKPT_ERR_AP_VC_VERI_FAIL           -100007 /* Challenge ���� ���� */
#define DAMO_DUKPT_ERR_AP_MR_NULL_POINTER        -100008 /* Response ������ NULL ������ ���� */
#define DAMO_DUKPT_ERR_AP_MR_INVALID_LEN         -100009 /* Response ������ ��ȿ���� �ʴ� ���� */
#define DAMO_DUKPT_ERR_AP_VR_NULL_POINTER        -100010 /* Response ������ NULL ������ ���� */
#define DAMO_DUKPT_ERR_AP_VR_INVALID_LEN         -100011 /* Response ������ ��ȿ���� �ʴ� ���� */
#define DAMO_DUKPT_ERR_AP_VR_VERI_FAIL           -100012 /* Response ���� ���� */
#define DAMO_DUKPT_ERR_AP_MTCS_NULL_POINTER       -100103 /* TokenCS ������ NULL ������ ���� */
#define DAMO_DUKPT_ERR_AP_MTCS_INVALID_LEN        -100104 /* TokenCS ������ ��ȿ���� �ʴ� ���� */
#define DAMO_DUKPT_ERR_AP_VTCS_NULL_POINTER       -100105 /* TokenCS ������ NULL ������ ���� */
#define DAMO_DUKPT_ERR_AP_VTCS_INVALID_LEN        -100106 /* TokenCS ������ ��ȿ���� �ʴ� ���� */
#define DAMO_DUKPT_ERR_AP_VTCS_VERI_FAIL          -100107 /* TokenCS ���� ���� */
#define DAMO_DUKPT_ERR_AP_MTSC_NULL_POINTER       -100108 /* TokenSC ������ NULL ������ ���� */
#define DAMO_DUKPT_ERR_AP_MTSC_INVALID_LEN        -100109 /* TokenSC ������ ��ȿ���� �ʴ� ���� */
#define DAMO_DUKPT_ERR_AP_VTSC_NULL_POINTER       -100110 /* TokenSC ������ NULL ������ ���� */
#define DAMO_DUKPT_ERR_AP_VTSC_INVALID_LEN        -100111 /* TokenSC ������ ��ȿ���� �ʴ� ���� */
#define DAMO_DUKPT_ERR_AP_VTSC_VERI_FAIL          -100112 /* TokenSC ���� ���� */


#ifdef __cplusplus
extern "C" {
#endif

/* DAMO_DUKPT_Get_Temp_Key
 * description : �ӽ�Ű ���� �Լ�
 *
 * parameter
 * 1. tid(IN) : KSN ���� 7����Ʈ
 * 2. tid_len(IN) : tid���� ����
 * 3. c_random(OUT) : Ŭ���̾�Ʈ�� �����ϴ� 16����Ʈ ����������
 * 4. c_random_len(IN) : c_random���� ����
 * 5. key1(OUT) : Challenge�����͸� �����ϴµ� ���� 16����Ʈ �ӽ�Ű ������
 * 6. key1_len(IN) : key1���� ����
 *
 * return value
 *  if successful, ret = 0
 *  otherwise, ret < 0
 */
int DAMO_DUKPT_Get_Temp_Key(const unsigned char *tid, size_t tid_len,
		unsigned char *c_random, size_t c_random_len, unsigned char *key1, size_t key1_len);

/* DAMO_DUKPT_Make_Challenge
 * description : Ŭ���̾�Ʈ�� ������ ������ Challenge�����͸� �����ϴ� �Լ�
 *
 * parameter
 * 1. tid(IN) : KSN ���� 7����Ʈ
 * 2. tid_len(IN) : tid���� ����
 * 3. c_random(IN) : Ŭ���̾�Ʈ�� ������ 16����Ʈ ����������
 * 4. c_random_len(IN) : c_random���� ����
 * 5. key1(IN) : Challenge�����͸� �����ϴµ� ���Ǵ� 16����Ʈ �ӽ�Ű ������
 * 6. key1_len(IN) : key1���� ����
 * 4. send_chal(OUT) : ������ ������ 55����Ʈ Challenge������ => [7����Ʈ TID]+[16����Ʈ CRandom]+[32����Ʈ KEY1_HMAC(TID+CRandom)]
 * 5. send_chal_len(IN) : send_chal���� ����
 *
 * return value
 *  if successful, ret = 0
 *  otherwise, ret < 0
 */
int DAMO_DUKPT_Make_Challenge(const unsigned char *tid, size_t tid_len, const unsigned char *c_random, size_t c_random_len,
		const unsigned char *key1, size_t key1_len, unsigned char *send_chal, size_t send_chal_len);

/* DAMO_DUKPT_Verify_Challenge
 * description : �������� Ŭ���̾�Ʈ�� ������ Challenge�����͸� �����ϴ� �Լ�
 *
 * parameter
 * 1. recv_chal(IN) : ������ Ŭ���̾�Ʈ�κ���  ������ 55����Ʈ Challenge������ => [7����Ʈ TID]+[16����Ʈ CRandom]+[32����Ʈ KEY1_HMAC(TID+CRandom)]
 * 2. recv_chal_len(IN) : recv_chal���� ����
 * 3. tid(OUT) : Challenge�����ͷκ��� ����� ���� 7����Ʈ TID
 * 4. tid_len(IN) : tid���� ����
 * 5. c_random(OUT) : Challenge�����ͷκ��� ����� 16����Ʈ Ŭ���̾�Ʈ ����������
 * 6. c_random_len(IN) : c_random���� ����
 * 7. key1(OUT) : Response������ ��ȣȭ Ű�� �����ϴµ� ���Ǵ� 16����Ʈ �ӽ�Ű ������
 * 8. key1_len(IN) : key1���� ����
 *
 * return value
 *  Challenge ������ ���� ������ , ret = 0
 *  ���н�, ret < 0
 */
int DAMO_DUKPT_Verify_Challenge(const unsigned char *recv_chal, size_t recv_chal_len, unsigned char *tid, size_t tid_len,
		unsigned char *c_random, size_t c_random_len, unsigned char *key1, size_t key1_len);

/* DAMO_DUKPT_Make_Response
 * description : ������ Ŭ���̾�Ʈ�� ������ Response�����͸� �����ϴ� �Լ�
 *
 * parameter
 * 1. ipek(IN) : Ŭ���̾�Ʈ�� ������ 16����Ʈ ipek(�ʱ� PIN ��ȣȭŰ)
 * 2. ipek_len(IN) : ipek���� ����
 * 3. c_random(IN) : Challenge�����ͷκ��� ����� 16����Ʈ Ŭ���̾�Ʈ ����������
 * 4. c_random_len(IN) : c_random���� ����
 * 5. key1(IN) : Response������ ��ȣȭ Ű�� �����ϴµ� ���Ǵ� 16����Ʈ �ӽ�Ű ������
 * 6. key1_len(IN) : key1���� ����
 * 7. send_resp(OUT) : Ŭ���̾�Ʈ�� ������ 48����Ʈ Response������ => [16����Ʈ SRandom]+[32����Ʈ KEY2_ENC(IPEK+CRandom)]
 * 8. send_resp_len(IN) : send_resp���� ���� 
 *
 *  if successful, ret = 0
 *  otherwise, ret < 0
 */
int DAMO_DUKPT_Make_Response(const unsigned char *ipek, size_t ipek_len, const unsigned char *c_random, size_t c_random_len,
		const unsigned char *key1, size_t key1_len, unsigned char *send_resp, size_t send_resp_len);

/* DAMO_DUKPT_Verifiy_Response
 * description : Ŭ���̾�Ʈ���� ������ ������ Response�����͸� �����ϴ� �Լ�
 *
 * parameter
 * 1. recv_resp(IN) : Ŭ���̾�Ʈ�� �����κ��� ������ 48����Ʈ Response������ => [16����Ʈ SRandom]+[32����Ʈ KEY2_ENC(IPEK+CRandom)]
 * 2. recv_resp_len(IN) : recv_resp���� ���� 
 * 3. c_random(IN) : Ŭ���̾�Ʈ�� ������ 16����Ʈ ���������� 
 * 4. c_random_len(IN) : c_random���� ����
 * 5. key1(IN) : Response������ ��ȣȭ Ű�� �����ϴµ� ���Ǵ� 16����Ʈ �ӽ�Ű ������
 * 6. key1_len(IN) : key1���� ����
 * 7. ipek(OUT) : Ŭ���̾�Ʈ�� ����� ipek(�ʱ� PIN ��ȣȭŰ)
 * 8. ipek_len(IN) : ipek���� ����
 *
 * return value
 *  Response ������ ���� ������ , ret = 0
 *  ���н�, ret < 0
 */
int DAMO_DUKPT_Verify_Response(const unsigned char *recv_resp, size_t recv_resp_len, const unsigned char *c_random, size_t c_random_len,
		const unsigned char *key1, size_t key1_len, unsigned char *ipek, size_t ipek_len);

/* DAMO_DUKPT_Make_TokenCS
 * description : Ŭ���̾�Ʈ�� ������ �����ϴ� Timestamp��� ���� ��ū�� �����ϴ� �Լ�
 *
 * parameter
 * 1. svr_info(IN) : ���� �ĺ��� ���� 
 * 2. svr_info_len(IN) : ���� �ĺ��� ���� ����
 * 3. tid(IN) : TID(Ŭ���̾�Ʈ �ĺ���)
 * 4. tid_len(IN) : TID ����
 * 5. c_random(IN) : Ŭ���̾�Ʈ�� ������ 16����Ʈ ����������
 * 6. c_random_len(IN) : c_random���� ����
 * 7. key1(IN) : ������ū�� �����ϴµ� ���Ǵ� Ű
 * 8. key1_len(IN) : key1 ����
 * 9. send_token_cs(OUT) : ������ ��ū ������ 
 * 10. send_token_cs_len(INOUT) : send_token_cs���� ����(IN), ��ū ������ ����(OUT)
 *
 * return value
 *  Response ������ ���� ������ , ret = 0
 *  ���н�, ret < 0
 */
int DAMO_DUKPT_Make_TokenCS(const unsigned char *svr_info, size_t svr_info_len, const unsigned char *tid, size_t tid_len,
		const unsigned char *c_random, size_t c_random_len, const unsigned char *key1, size_t key1_len,
		unsigned char *send_token_cs, size_t *send_token_cs_len);

/* DAMO_DUKPT_Verify_TokenCS
 * description : �������� Ŭ���̾�Ʈ�� �����ϴ� Timestamp��� ���� ��ū�� �����ϴ� �Լ�
 *
 * parameter
 * 1. ts_interval(IN) : ���� ��ū�� ������ Timestamp�� ��� �ð� ����(����: ��) 
 * 2. recv_token_cs(IN) : ������ ������ Timestamp��� ���� ��ū 
 * 3. recv_token_cs_len(IN) : ������ ������ Timestamp��� ���� ��ū ����
 * 4. svr_info(IN) : ���� �ĺ��� ����
 * 5. svr_info_len(IN) : ���� �ĺ��� ���� ����
 * 6. tid(OUT) : ���� ��ū���κ��� ����� ���� 7����Ʈ TID 
 * 7. tid_len(IN) : tid���� ����
 * 8. key1(OUT) : ���� ��ū�� �����ϴµ� ���Ǵ� Ű
 * 9. key1_len(IN) : key1���� ����
 *
 * return value
 *  Response ������ ���� ������ , ret = 0
 *  ���н�, ret < 0
 */
int DAMO_DUKPT_Verify_TokenCS(int ts_interval, const unsigned char *recv_token_cs, size_t recv_token_cs_len,
		const unsigned char *svr_info, size_t svr_info_len, unsigned char *tid, size_t tid_len, unsigned char *key1, size_t key1_len);
int DAMO_DUKPT_Server_Verify_TokenCS (unsigned char *sid, size_t sid_len, int ts_interval, const unsigned char *recv_token_cs, size_t recv_token_cs_len,
		 const unsigned char *svr_info, size_t svr_info_len, unsigned char *tid, size_t tid_len, unsigned char *key1, size_t key1_len);

/* DAMO_DUKPT_Make_TokenSC 
 * description : ������ Ŭ���̾�Ʈ�� �����ϴ� Timestamp��� ���� ��ū�� �����ϴ� �Լ�
 *
 * parameter
 * 1. tid(IN) : TID(Ŭ���̾�Ʈ �ĺ���)
 * 2. tid_len(IN) : TID ����
 * 3. ipek(IN) : Ŭ���̾�Ʈ�� ����� ipek(�ʱ� PIN ��ȣȭŰ)
 * 4. ipek_len(IN) : ipek���� ����
 * 5. key1(IN) : ���� ��ū�� �����ϴµ� ���Ǵ� Ű
 * 6. key1_len(IN) : key1 ����
 * 7. send_token_sc(OUT) : ������ ��ū ������ 
 * 8. send_token_sc_len(INOUT) : send_token_sc���� ����(IN), ��ū ������ ����(OUT)
 *
 * return value
 *  Response ������ ���� ������ , ret = 0
 *  ���н�, ret < 0
 */
int DAMO_DUKPT_Make_TokenSC(const unsigned char *tid, size_t tid_len,
		const unsigned char *ipek, size_t ipek_len, const unsigned char *key1, size_t key1_len,
		unsigned char *send_token_sc, size_t *send_token_sc_len);

/* DAMO_DUKPT_Verify_TokenSC
 * description : Ŭ���̾�Ʈ���� ������ �����ϴ� Timestamp��� ���� ��ū�� �����ϴ� �Լ�
 *
 * parameter
 * 1. ts_interval(IN) : ���� ��ū�� ������ Timestamp�� ��� �ð� ����(����: ��) 
 * 2. recv_token_sc(IN) : Ŭ���̾�Ʈ�� ������ Timestamp��� ���� ��ū 
 * 3. recv_token_sc_len(IN) : Ŭ���̾�Ʈ�� ������ Timestamp��� ���� ��ū ����
 * 4. tid(IN) : TID(Ŭ���̾�Ʈ �ĺ���)  
 * 5. tid_len(IN) : TID ����
 * 6. key1(IN) : ���� ��ū�� �����ϴµ� ���Ǵ� Ű
 * 7. key1_len(IN) : key1 ����
 * 8. ipek(OUT) : Ŭ���̾�Ʈ�� ����� ipek(�ʱ� PIN ��ȣȭŰ)
 * 9. ipek_len(IN) : ipek���� ����
 *
 * return value
 *  Response ������ ���� ������ , ret = 0
 *  ���н�, ret < 0
 */
int DAMO_DUKPT_Verify_TokenSC(int ts_interval, const unsigned char *recv_token_sc, size_t recv_token_sc_len,
		const unsigned char *tid, size_t tid_len, const unsigned char *key1, size_t key1_len, unsigned char *ipek, size_t ipek_len);

#ifdef __cplusplus
}
#endif
