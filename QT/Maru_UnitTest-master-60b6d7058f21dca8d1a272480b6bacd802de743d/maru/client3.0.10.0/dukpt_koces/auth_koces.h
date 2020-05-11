#include "auth_proto.h"
#include "dukpt_cli.h"

#define AP_SID_SIZE 16
#define AP_TID_SIZE 7
#define AP_RAND_SIZE 16
#define AP_HIDE_DATA_SIZE 1024
#define AP_

/* Return Value */
#define KOCES_DUKPT_SUCCESS                        000000 /* Success */
#define KOCES_DUKPT_ERR_AP_HIDE_INVALID_LEN       -900001 /* HIDE STRING의 유효하지 않는 길이 */
#define KOCES_DUKPT_ERR_AP_HASH_LEN_COMP          -900002 /* HASH 비교시 HASH 길이 오류 */
#define KOCES_DUKPT_ERR_AP_HASH_DIFF              -900003 /* HASH값 비교시 상이할 때 */
#define KOCES_DUKPT_ERR_AP_LRC                    -900004 /* LRC 오류 */
#define KOCES_DUKPT_ERR_HDATA_LEN                 -900005 /* HDATA 길이 오류 */

#ifdef __cplusplus
extern "C" {
#endif

/* KOCES_DUKPT_ReGen_Hide_Data
 * description : 키 숨김 데이타의 주입 과정 중 주입 프로그램에서 전달된 데이타를 단말기에 저장용 데이타로 재 생성 - 숨김 데이타 주입시 필수로 1회만 실행해야 함
 *               (내부적으로 DAMO_DUKPT_Verify_SID와 동일한 기능을 수행 후 Storage_Key(SKey)를 생성하여 숨김 데이타 내부에 저장한다)
 * parameter
 * 1. hide_data(IN) : 키 숨김 데이타
 * 2. hide_data_len(IN) : 키 숨김 데이타의 길이 (1024바이트)
 * 3. serial_num(IN) : 단말기 Serial Number
 * 4. serial_num_len(IN) : 단말기 Serial Number의 길이 (10Byte)
 *
 * return value
 *  if successful, ret = 0
 *  otherwise, ret < 0
 */
int KOCES_DUKPT_ReGen_Hide_Data(unsigned char *hide_data, size_t hide_data_len, unsigned char *serial_num, size_t serial_num_len);

/* KOCES_DUKPT_Make_TokenCS
 * description : 클라이언트가 서버에 전송하는 Timestamp기반 인증 토큰을 생성하는 함수 (KOCES Version)
 *
 * parameter
 * 1. hide_data(IN) : 키 숨김 데이타
 * 2. hide_data_len(IN) : 키 숨김 데이타의 길이 (1024바이트)
 * 3. tid(IN) : TID(클라이언트 식별자)
 * 4. tid_len(IN) : TID 길이
 * 5. key1(OUT) : 인증토큰을 생성하는데 사용되는 키
 * 6. key1_len(OUT) : key1 길이
 * 7. send_token_cs(OUT) : 전송할 토큰 데이터 
 * 8. send_token_cs_len(INOUT) : send_token_cs버퍼 길이(IN), 토큰 데이터 길이(OUT)
 *
 * return value
 *  Response 데이터 검증 성공시 , ret = 0
 *  실패시, ret < 0
 */
int KOCES_DUKPT_Make_TokenCS(const unsigned char *hide_data, size_t hide_data_len, const unsigned char *tid, size_t tid_len,
        unsigned char *key1, size_t key1_len, unsigned char *send_token_cs, size_t *send_token_cs_len);

/* KOCES_DUKPT_Verify_TokenSC
 * description : 클라이언트에서 서버가 전송하는 Timestamp기반 인증 토큰을 검증하는 함수
 *
 * parameter
 * 1. recv_token_sc(IN) : 클라이언트가 수신한 Timestamp기반 인증 토큰 
 * 2. recv_token_sc_len(IN) : 클라이언트가 수신한 Timestamp기반 인증 토큰 길이
 * 3. tid(IN) : TID(클라이언트 식별자)  
 * 4. tid_len(IN) : TID 길이
 * 5. key1(OUT) : 인증 토큰을 검증하는데 사용되는 키
 * 6. key1_len(INOUT) : key1 길이
 * 7. ipek(OUT) : 클라이언트가 사용할 ipek(초기 PIN 암호화키)
 * 8. ipek_len(IN) : ipek버퍼 길이
 *
 * return value
 *  Response 데이터 검증 성공시 , ret = 0
 *  실패시, ret < 0
 */
int KOCES_DUKPT_Verify_TokenSC(const unsigned char *recv_token_sc, size_t recv_token_sc_len,
		const unsigned char *tid, size_t tid_len, const unsigned char *key1, size_t key1_len, unsigned char *ipek, size_t ipek_len);

/* KOCES_DUKPT_Load_Initial_Key
 * description : DUKPT 초기화 키 로딩 함수
 *   
 * parameter
 * 1. ipek(IN) : 16바이트 초기 PIN 암호화 키
 * 2. ipek_len(IN) : 초기 PIN 암호화 키 길이
 * 3. ksn(IN) : 10 바이트 Key Serial Number ( [59비트 Initial Key Serial Number]||[21비트 0] )
 * 4. ksn_len(IN) : Key Serial Number 길이
 *      
 * return value
 *  if successful, ret = 0
 *  otherwise, ret < 0
 */
int KOCES_DUKPT_Load_Initial_Key(const unsigned char *ipek, size_t ipek_len, const unsigned char *ksn, size_t ksn_len);

/* KOCES_DUKPT_Export_Future_Key_Info
 * description : KSN과 Future Key Set을 가져오는 함수
 *   
 * parameter
 * 1. hide_data(IN) : 키 숨김 데이타
 * 2. hide_data_len(IN) : 키 숨김 데이타의 길이 (1024바이트)
 * 3. fki(OUT) : KSN과 Future Key Set를 저장할 구조체 포인터 
 *      
 * return value
 *  if successful, ret = 0
 *  otherwise, ret < 0
 */
int KOCES_DUKPT_Export_Future_Key_Info(const unsigned char *hide_data, size_t hide_data_len, DukptFutureKeyInfo *fki);

/* KOCES_DUKPT_Import_Future_Key_Info
 * description : KSN과 Future Key Set을 설정하는 함수
 *   
 * parameter
 * 1. hide_data(IN) : 키 숨김 데이타
 * 2. hide_data_len(IN) : 키 숨김 데이타의 길이 (1024바이트)
 * 3. fki(IN) : 설정할 KSN과 Future Key Set의 구조체 
 *      
 * return value
 *  if successful, ret = 0
 *  otherwise, ret < 0
 */
int KOCES_DUKPT_Import_Future_Key_Info(const unsigned char *hide_data, size_t hide_data_len, DukptFutureKeyInfo *fki);

/* KOCES_DUKPT_AES_Encrypt
 * description : DUKPT AES 암호화 함수(fki import --> DUKPT TK 생성 --> AES CBC 128bit enc --> fki export)
 *   
 * parameter
 * 1. hide_data(IN) : 키 숨김 데이타
 * 2. hide_data_len(IN) : 키 숨김 데이타의 길이 (1024바이트)
 * 3. enc(OUT) : 암호문 데이터를 저장할 포인터
 * 4. enc_len(OUT) : 암호문 데이터 길이를 저장할 포인터
 * 5. KSN(OUT) : KSN 데이터를 저장할 포인터
 * 6. KSN_len(OUT) : KSN 데이터 길이를 저장할 포인터
 * 7. org(IN) : 평문 데이터
 * 8. org_len(IN) : 평문 데이터 길이
 * 9. fki(IN/OUT) : 설정할 KSN과 Future Key Set의 구조체
 *            
 * return value
 *  if successful, ret = 0
 *  otherwise, ret < 0
 */
int KOCES_DUKPT_AES_Encrypt(const unsigned char *hide_data, size_t hide_data_len, unsigned char *enc, size_t *enc_len,
        unsigned char *KSN, size_t *KSN_len, const unsigned char *org, size_t org_len, DukptFutureKeyInfo *fki);

/* KOCES_DUKPT_Make_Salf_Protect_HASH
 * description : 자체보호용 HASH를 Storage Key를 이용하여 암호화하는 함수
 *
 * parameter
 * 1. hide_data(IN) : 키 숨김 데이타
 * 2. hide_data_len(IN) : 키 숨김 데이타의 길이 (1024바이트)
 * 3. out(OUT) : 암호화된 HASH
 * 4. out_len(OUT) : 암호화된 HASH의 길이
 * 5. in(IN) : HASH 원본 데이타
 * 6. in_len(IN) : HASH 원본 데이타의 길이
 *
 * return value
 *  if successful, ret = 0
 *  otherwise, ret < 0
 */
int KOCES_DUKPT_Make_Self_Protect_HASH(const unsigned char *hide_data, size_t hide_data_len,
        unsigned char *out, size_t *out_len, const unsigned char *in, size_t in_len);

/* KOCES_DUKPT_Salf_Protect_HASH_Comp
 * description : 자체보호용 암호화된 HASH와의 비교 함수
 *
 * parameter
 * 1. hide_data(IN) : 키 숨김 데이타
 * 2. hide_data_len(IN) : 키 숨김 데이타의 길이 (1024바이트)
 * 3. enc_hash(IN) : 암호화된 HASH
 * 4. enc_hash_len(IN) : 암호화된 HASH의 길이
 * 5. in_hash(IN) : 비교하고자 하는 HASH 데이타
 * 6. in_hash_len(IN) : 비교하고자 하는 HASH 데이타의 길이
 *
 * return value
 *  if successful, ret = 0
 *  otherwise, ret < 0
 */
int KOCES_DUKPT_Comp_Self_Protect_HASH(const unsigned char *hide_data, size_t hide_data_len,
        const unsigned char *enc_hash, size_t enc_hash_len, const unsigned char *in_hash, size_t in_hash_len);

#ifdef __cplusplus
}
#endif
