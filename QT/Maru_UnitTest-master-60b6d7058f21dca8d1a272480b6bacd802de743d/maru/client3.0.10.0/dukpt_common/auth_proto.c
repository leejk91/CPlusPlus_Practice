#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "auth_proto.h"
#include "aes.h"
#include "sha256.h"
#include "hmac.h"
#include "rng.h"
#ifdef _WIN32
#include "strptime.h"
#endif

#ifndef DUKPT_SERVER
#define INLENMAX 256
#endif

static unsigned char *DAMO_DUKPT_Get_Pre_Shared_Key()
{
	static unsigned char pre_shared_key[16]={0x7b, 0x0b, 0x60, 0xe1, 0xe8, 0x66, 0x8e, 0x41,
		0xbf, 0x93, 0x97, 0x82, 0x30, 0x48, 0x30, 0x95};

	return pre_shared_key;
}

int DAMO_DUKPT_Get_Temp_Key(const unsigned char *tid, size_t tid_len,
		unsigned char *c_random, size_t c_random_len, unsigned char *key1, size_t key1_len)
{
	int ret = DAMO_DUKPT_SUCCESS;
	unsigned char in[AP_PSK_TID_RAND_SIZE];
	size_t in_len;
	unsigned char *pre_shared_key;

	if(!tid || !c_random || !key1)
		return DAMO_DUKPT_ERR_AP_GTK_NULL_POINTER;

	if(tid_len < AP_TID_SIZE || c_random_len < AP_RAND_SIZE || key1_len < AP_KEY_SIZE)
		return DAMO_DUKPT_ERR_AP_GTK_INVALID_LEN;

	key1_len = AP_KEY_SIZE;

	pre_shared_key = DAMO_DUKPT_Get_Pre_Shared_Key();

	DAMO_CRYPT_RNG( c_random, AP_RAND_SIZE );

	in_len = AP_PSK_TID_RAND_SIZE;

	memcpy(in, pre_shared_key, AP_PSK_SIZE);
	memcpy(in+AP_PSK_SIZE, tid, AP_TID_SIZE);
	memcpy(in+AP_PSK_TID_SIZE, c_random, AP_RAND_SIZE);

	if(!DAMO_CRYPT_KDF2_SHA256(in, in_len, key1, &key1_len))
	{
		ret = DAMO_DUKPT_ERR_AP_GTK_GET_TKEY_FAIL;
	}

	return ret;
}

int DAMO_DUKPT_Make_Challenge(const unsigned char *tid, size_t tid_len, const unsigned char *c_random, size_t c_random_len,
		const unsigned char *key1, size_t key1_len, unsigned char *send_chal, size_t send_chal_len)
{
	int ret = DAMO_DUKPT_SUCCESS;
	size_t digest_len;

	if(!tid || !c_random || !key1 || !send_chal)
		return DAMO_DUKPT_ERR_AP_MC_NULL_POINTER;

	if(tid_len < AP_TID_SIZE || c_random_len < AP_RAND_SIZE ||
			key1_len < AP_KEY_SIZE || send_chal_len < AP_CHAL_SIZE)
		return DAMO_DUKPT_ERR_AP_MC_INVALID_LEN;

	memcpy(send_chal, tid, AP_TID_SIZE);
	memcpy(send_chal+AP_TID_SIZE, c_random, AP_RAND_SIZE);
	ret = DAMO_CRYPT_HMAC(HMAC_SHA256, key1, AP_RAND_SIZE, send_chal, AP_TID_RAND_SIZE, send_chal+AP_TID_RAND_SIZE, &digest_len);

	return ret;
}

int DAMO_DUKPT_Verify_Challenge(const unsigned char *recv_chal, size_t recv_chal_len, unsigned char *tid, size_t tid_len,
		unsigned char *c_random, size_t c_random_len, unsigned char *key1, size_t key1_len)
{
	int ret = DAMO_DUKPT_SUCCESS;
	unsigned char digest[32];
	size_t digest_len;
	unsigned char in[AP_PSK_TID_RAND_SIZE];
	size_t in_len;
	unsigned char *pre_shared_key;

	if(!recv_chal || !tid || !c_random || !key1)
		return DAMO_DUKPT_ERR_AP_VC_NULL_POINTER;

	if(recv_chal_len < AP_CHAL_SIZE || tid_len < AP_TID_SIZE ||
			c_random_len < AP_RAND_SIZE || key1_len < AP_KEY_SIZE)
		return DAMO_DUKPT_ERR_AP_VC_INVALID_LEN;

	key1_len = AP_KEY_SIZE;

	pre_shared_key = DAMO_DUKPT_Get_Pre_Shared_Key();

	in_len = AP_PSK_TID_RAND_SIZE;

	memcpy(in, pre_shared_key, AP_PSK_SIZE);
	memcpy(in+AP_PSK_SIZE, recv_chal, AP_TID_RAND_SIZE);

	if(!DAMO_CRYPT_KDF2_SHA256(in, in_len, key1, &key1_len))
	{
		ret = DAMO_DUKPT_ERR_AP_VC_VERI_FAIL;
		goto err;
	}

	DAMO_CRYPT_HMAC(HMAC_SHA256, key1, key1_len, recv_chal, AP_TID_RAND_SIZE, digest, &digest_len);

	if(memcmp(digest, recv_chal+AP_TID_RAND_SIZE, 32))
	{
		ret = DAMO_DUKPT_ERR_AP_VC_VERI_FAIL;
		goto err;
	}

	memcpy(tid, recv_chal, AP_TID_SIZE);
	memcpy(c_random, recv_chal+AP_TID_SIZE, AP_RAND_SIZE);

err:

	return ret;
}

int DAMO_DUKPT_Make_Response(const unsigned char *ipek, size_t ipek_len, const unsigned char *c_random, size_t c_random_len,
		const unsigned char *key1, size_t key1_len, unsigned char *send_resp, size_t send_resp_len)
{
	int i;
	int ret = DAMO_DUKPT_SUCCESS;
	size_t out_len;
	unsigned char key2[AP_KEY_SIZE];
	unsigned char in[AP_KEY_RAND_SIZE];
	unsigned char *pre_shared_key;

	if(!ipek || !c_random || !key1 || !send_resp)
		return DAMO_DUKPT_ERR_AP_MR_NULL_POINTER;

	if(ipek_len < AP_KEY_SIZE || c_random_len < AP_RAND_SIZE ||
			key1_len < AP_KEY_SIZE || send_resp_len < AP_RESP_SIZE)
		return DAMO_DUKPT_ERR_AP_MR_INVALID_LEN;

	pre_shared_key = DAMO_DUKPT_Get_Pre_Shared_Key();

	ret = DAMO_CRYPT_RNG( send_resp, AP_RAND_SIZE );
	if(ret != DAMO_DUKPT_SUCCESS)
	{
		return ret;
	}

	for(i=0; i<AP_KEY_SIZE; i++)
		key2[i] = key1[i]^send_resp[i];

	memcpy(in, ipek, AP_KEY_SIZE);
	memcpy(in+AP_KEY_SIZE, c_random, AP_RAND_SIZE);

	ret = DAMO_CRYPT_AES_Encrypt(send_resp+AP_RAND_SIZE, &out_len, in, AP_KEY_RAND_SIZE, key2, AP_KEY_SIZE, AES_128, CFB_MODE);
  memset(key2, 0x00, sizeof(key2));

	return ret;

}

int DAMO_DUKPT_Verify_Response(const unsigned char *recv_resp, size_t recv_resp_len, const unsigned char *c_random, size_t c_random_len,
		const unsigned char *key1, size_t key1_len, unsigned char *ipek, size_t ipek_len)
{
	int i;
	int ret = DAMO_DUKPT_SUCCESS;
	unsigned char key2[AP_KEY_SIZE];
	unsigned char out[AP_KEY_RAND_SIZE];
	size_t out_len;

	if(!recv_resp || !c_random || !key1 || !ipek)
		return DAMO_DUKPT_ERR_AP_VR_NULL_POINTER;

	if(recv_resp_len < AP_RESP_SIZE || c_random_len < AP_RAND_SIZE ||
			key1_len < AP_KEY_SIZE || ipek_len < AP_KEY_SIZE)
		return DAMO_DUKPT_ERR_AP_VR_INVALID_LEN;

	for(i=0; i<AP_KEY_SIZE; i++)
		key2[i] = key1[i]^recv_resp[i];

	ret = DAMO_CRYPT_AES_Decrypt(out, &out_len, recv_resp+AP_RAND_SIZE, AP_KEY_RAND_SIZE, key2, AP_KEY_SIZE, AES_128, CFB_MODE);
  memset(key2, 0x00, sizeof(key2));

	if(ret != DAMO_DUKPT_SUCCESS)
	{
		return ret;
	}  

	if(memcmp(out+AP_KEY_SIZE, c_random, AP_RAND_SIZE))
	{
		ret = DAMO_DUKPT_ERR_AP_VC_VERI_FAIL;
		goto err;
	}

	memcpy(ipek, out, AP_KEY_SIZE);

err:

	return ret;
}

int DAMO_DUKPT_Make_TokenCS(const unsigned char *svr_info, size_t svr_info_len, const unsigned char *tid, size_t tid_len,
		const unsigned char *c_random, size_t c_random_len, const unsigned char *key1, size_t key1_len,
		unsigned char *send_token_cs, size_t *send_token_cs_len)
{
	int ret = DAMO_DUKPT_SUCCESS;
	size_t pt_len;
	size_t ct_len;
	time_t ts;
	unsigned char *ts_str = NULL;
	size_t ts_str_len;
	size_t offset;

#ifdef DUKPT_SERVER
	unsigned char *pt = NULL;
#else
  unsigned char pt[INLENMAX];
#endif


	if(!svr_info || !tid || !c_random || !key1 || !send_token_cs || !send_token_cs_len)
		return DAMO_DUKPT_ERR_AP_MTCS_NULL_POINTER;

	if(key1_len < AP_KEY_SIZE)
		return DAMO_DUKPT_ERR_AP_MTCS_INVALID_LEN;

	time(&ts);
	ts_str = (unsigned char *)ctime(&ts);
	ts_str_len = strlen((char *)ts_str);

	if(!ts_str_len)
		return DAMO_DUKPT_ERR_AP_MTCS_INVALID_LEN;

	pt_len = ts_str_len+svr_info_len+tid_len+c_random_len;

	if((*send_token_cs_len) < (tid_len+c_random_len+pt_len))
	{
		return DAMO_DUKPT_ERR_AP_MTCS_INVALID_LEN;
	}

#ifdef DUKPT_SERVER
	pt = (unsigned char *)malloc(pt_len);
	if(!pt)
		return DAMO_DUKPT_ERR_AP_MTCS_NULL_POINTER;
#else
  if( INLENMAX < pt_len)
    return DAMO_DUKPT_ERR_AP_MTCS_INVALID_LEN;
#endif


	memcpy(pt, ts_str, ts_str_len);
	offset = ts_str_len;
	memcpy(pt+offset, svr_info, svr_info_len);
	offset += svr_info_len;
	memcpy(pt+offset, tid, tid_len);
	offset += tid_len;
	memcpy(pt+offset, c_random, c_random_len);

	memcpy(send_token_cs, tid, tid_len);
	offset = tid_len;
	memcpy(send_token_cs+offset, c_random, c_random_len);
	offset += c_random_len;

	ret = DAMO_CRYPT_AES_Encrypt(send_token_cs+offset, &ct_len, pt, pt_len, key1, key1_len, AES_128, CFB_MODE);
	(*send_token_cs_len) = offset+ct_len;

	memset(pt, 0x00, pt_len);
#ifdef DUKPT_SERVER
	free(pt);
#endif

	return ret;
}

int DAMO_DUKPT_Verify_TokenCS(int ts_interval, const unsigned char *recv_token_cs, size_t recv_token_cs_len,
		const unsigned char *svr_info, size_t svr_info_len, unsigned char *tid, size_t tid_len, unsigned char *key1, size_t key1_len)
{
	int ret = DAMO_DUKPT_SUCCESS;
	size_t pt_len;
	time_t ts;
	size_t ts_str_len = 25;
	int try_ts_cmp_cnt;
	struct tm tm;
	time_t recv_ts;
	unsigned char in[AP_PSK_TID_RAND_SIZE];
	size_t in_len;
	unsigned char *pre_shared_key;

#ifdef DUKPT_SERVER
	unsigned char *pt = NULL;
#else
  unsigned char pt[INLENMAX];
#endif

	if(!recv_token_cs || !svr_info || !tid || !key1)
	{
		return DAMO_DUKPT_ERR_AP_VTCS_NULL_POINTER;
	}

	if(tid_len < AP_TID_SIZE || key1_len < AP_KEY_SIZE)
	{
		return DAMO_DUKPT_ERR_AP_VTCS_INVALID_LEN;
	}

	pt_len = ts_str_len+svr_info_len+AP_TID_RAND_SIZE;

	if(recv_token_cs_len != (AP_TID_RAND_SIZE+pt_len))
	{
		return DAMO_DUKPT_ERR_AP_VTCS_INVALID_LEN;
	}

	key1_len = AP_KEY_SIZE;

	pre_shared_key = DAMO_DUKPT_Get_Pre_Shared_Key();

	in_len = AP_PSK_TID_RAND_SIZE;

	memcpy(in, pre_shared_key, AP_PSK_SIZE);
	memcpy(in+AP_PSK_SIZE, recv_token_cs, AP_TID_RAND_SIZE);

	if(!DAMO_CRYPT_KDF2_SHA256(in, in_len, key1, &key1_len))
	{
		ret = DAMO_DUKPT_ERR_AP_VTCS_VERI_FAIL;
		goto err;
	}

#ifdef DUKPT_SERVER
	pt = (unsigned char *)malloc(pt_len);

	if(!pt)
	{
		return DAMO_DUKPT_ERR_AP_VTCS_NULL_POINTER;
	}
#else
  if(INLENMAX < pt_len)
    return DAMO_DUKPT_ERR_AP_VTCS_INVALID_LEN;
#endif

	ret = DAMO_CRYPT_AES_Decrypt(pt, &pt_len, recv_token_cs+AP_TID_RAND_SIZE, recv_token_cs_len-AP_TID_RAND_SIZE, key1, key1_len, AES_128, CFB_MODE);
	if(ret != DAMO_DUKPT_SUCCESS)
	{
		goto err;
	}

	if(memcmp(pt+(pt_len-AP_TID_RAND_SIZE), recv_token_cs, AP_TID_RAND_SIZE))
	{
		ret = DAMO_DUKPT_ERR_AP_VTCS_VERI_FAIL;
		goto err;
	}

	strptime(pt, "%a %b %d %H:%M:%S %Y", &tm);
	recv_ts = mktime(&tm);
	recv_ts = (recv_ts/ts_interval)*ts_interval;

	time(&ts);
	ts = (ts/ts_interval)*ts_interval;

	try_ts_cmp_cnt = 0;
	while(1)
	{
		if(recv_ts == ts)
			break;

		if(try_ts_cmp_cnt == 0)
		{
			ts -= ts_interval;
		}
		else if(try_ts_cmp_cnt == 1)
		{
			ts += (ts_interval*2);
		}
		else
		{
			ret = DAMO_DUKPT_ERR_AP_VTCS_VERI_FAIL;
			goto err;
		}

		try_ts_cmp_cnt++;
	}

	if(memcmp(pt+ts_str_len, svr_info, svr_info_len))
	{
		ret = DAMO_DUKPT_ERR_AP_VTCS_VERI_FAIL;
		goto err;
	}

	
	memcpy(tid, recv_token_cs, AP_TID_SIZE);

err:
	if(pt)
	{
		memset(pt, 0x00, pt_len);
#ifdef DUKPT_SERVER
		free(pt);
#endif
	}

	return ret;
}

int DAMO_DUKPT_Server_Verify_TokenCS (unsigned char *sid, size_t sid_len, int ts_interval, const unsigned char *recv_token_cs, size_t recv_token_cs_len,
                                      const unsigned char *svr_info, size_t svr_info_len, unsigned char *tid, size_t tid_len, unsigned char *key1, size_t key1_len)
{
  int ret = DAMO_DUKPT_SUCCESS;
  size_t pt_len;
  time_t ts;
  size_t ts_str_len = 25;
  int try_ts_cmp_cnt;
  struct tm tm;
  time_t recv_ts;
  unsigned char in[AP_SID_TID_RAND_SIZE];
  size_t in_len;

#ifdef DUKPT_SERVER
  unsigned char *pt = NULL;
#else
  unsigned char pt[INLENMAX];
#endif
  
  if(!recv_token_cs || !svr_info || !tid || !key1)
  {
    return DAMO_DUKPT_ERR_AP_VTCS_NULL_POINTER;
  }

  if(tid_len < AP_TID_SIZE || key1_len < AP_KEY_SIZE)
  {
    return DAMO_DUKPT_ERR_AP_VTCS_INVALID_LEN;
  }

  pt_len = ts_str_len+svr_info_len+AP_TID_RAND_SIZE;

  if(recv_token_cs_len != (AP_TID_RAND_SIZE+pt_len))
  {
    return DAMO_DUKPT_ERR_AP_VTCS_INVALID_LEN;
  }

  key1_len = AP_KEY_SIZE;

  /*KTC인증지적으로 주석처리
  pre_shared_key = DAMO_DUKPT_Get_Pre_Shared_Key();
  memcpy(in, pre_shared_key, AP_PSK_SIZE);
  */

  in_len = AP_SID_TID_RAND_SIZE;

  memcpy(in, sid, AP_SID_SIZE);
  memcpy(in+AP_SID_SIZE, recv_token_cs, AP_TID_RAND_SIZE);

  if(!DAMO_CRYPT_KDF2_SHA256(in, in_len, key1, &key1_len))
  {
    ret = DAMO_DUKPT_ERR_AP_VTCS_VERI_FAIL;
    goto err;
  }

#ifdef DUKPT_SERVER
  pt = (unsigned char *)malloc(pt_len);

  if(!pt)
  {
    return DAMO_DUKPT_ERR_AP_VTCS_NULL_POINTER;
  }
#else
  if(INLENMAX < pt_len)
    return DAMO_DUKPT_ERR_AP_VTCS_INVALID_LEN;
#endif

  ret = DAMO_CRYPT_AES_Decrypt(pt, &pt_len, recv_token_cs+AP_TID_RAND_SIZE, recv_token_cs_len-AP_TID_RAND_SIZE, key1, key1_len, AES_128, CFB_MODE);
  if(ret != DAMO_DUKPT_SUCCESS)
  {
    goto err;
  }

  if(memcmp(pt+(pt_len-AP_TID_RAND_SIZE), recv_token_cs, AP_TID_RAND_SIZE))
  {
    ret = DAMO_DUKPT_ERR_AP_VTCS_VERI_FAIL;
    goto err;
  }

  strptime(pt, "%a %b %d %H:%M:%S %Y", &tm);
  recv_ts = mktime(&tm);
  recv_ts = (recv_ts/ts_interval)*ts_interval;

  time(&ts);
  ts = (ts/ts_interval)*ts_interval;

  try_ts_cmp_cnt = 0;
  while(1)
  {
    if(recv_ts == ts)
      break;

    if(try_ts_cmp_cnt == 0)
    {
      ts -= ts_interval;
    }
    else if(try_ts_cmp_cnt == 1)
    {
      ts += (ts_interval*2);
    }
    else
    {
      ret = DAMO_DUKPT_ERR_AP_VTCS_VERI_FAIL;
      goto err;
    }

    try_ts_cmp_cnt++;
  }

  if(memcmp(pt+ts_str_len, svr_info, svr_info_len))
  {
    ret = DAMO_DUKPT_ERR_AP_VTCS_VERI_FAIL;
    goto err;
  }

  memcpy(tid, recv_token_cs, AP_TID_SIZE);

err:
  if(pt)
  {
    memset(pt, 0x00, pt_len);
#ifdef DUKPT_SERVER
    free(pt);
#endif
  }

  return ret;
}

int DAMO_DUKPT_Make_TokenSC(const unsigned char *tid, size_t tid_len,
		const unsigned char *ipek, size_t ipek_len, const unsigned char *key1, size_t key1_len,
		unsigned char *send_token_sc, size_t *send_token_sc_len)
{
	int ret = DAMO_DUKPT_SUCCESS;
	size_t pt_len;
	size_t ct_len;
	time_t ts;
	unsigned char *ts_str = NULL;
	size_t ts_str_len;
	size_t offset;

#ifdef DUKPT_SERVER
	unsigned char *pt = NULL;
#else
  unsigned char pt[INLENMAX];
#endif

	if(!tid || !ipek || !key1 || !send_token_sc || !send_token_sc_len)
		return DAMO_DUKPT_ERR_AP_MTSC_NULL_POINTER;

	if(key1_len < AP_KEY_SIZE)
		return DAMO_DUKPT_ERR_AP_MTSC_INVALID_LEN;

	time(&ts);
	ts_str = (unsigned char *)ctime(&ts);
	ts_str_len = strlen((char *)ts_str);

	if(!ts_str_len)
		return DAMO_DUKPT_ERR_AP_MTSC_INVALID_LEN;

	pt_len = ts_str_len+tid_len+ipek_len;

	if((*send_token_sc_len) < pt_len)
	{
		return DAMO_DUKPT_ERR_AP_MTSC_INVALID_LEN;
	}

#ifdef DUKPT_SERVER
	pt = (unsigned char *)malloc(pt_len);

	if(!pt)
		return DAMO_DUKPT_ERR_AP_MTSC_NULL_POINTER;
#else
  if(INLENMAX < pt_len)
    return DAMO_DUKPT_ERR_AP_MTSC_INVALID_LEN;
#endif

	memcpy(pt, ts_str, ts_str_len);
	offset = ts_str_len;
	memcpy(pt+offset, tid, tid_len);
	offset += tid_len;
	memcpy(pt+offset, ipek, ipek_len);

	ret = DAMO_CRYPT_AES_Encrypt(send_token_sc, &ct_len, pt, pt_len, key1, key1_len, AES_128, CFB_MODE);
	(*send_token_sc_len) = ct_len;

	memset(pt, 0x00, pt_len);
#ifdef DUKPT_SERVER
	free(pt);
#endif

	return ret;
}

int DAMO_DUKPT_Verify_TokenSC(int ts_interval, const unsigned char *recv_token_sc, size_t recv_token_sc_len,
		const unsigned char *tid, size_t tid_len, const unsigned char *key1, size_t key1_len, unsigned char *ipek, size_t ipek_len)
{
	int ret = DAMO_DUKPT_SUCCESS;
	size_t pt_len;
	time_t ts;
	size_t ts_str_len = 25;
	int try_ts_cmp_cnt;
	struct tm tm;
	time_t recv_ts;

#ifdef DUKPT_SERVER
	unsigned char *pt = NULL;
#else
  unsigned char pt[INLENMAX];
#endif

	if(!recv_token_sc || !tid || !key1 || !ipek)
	{
		return DAMO_DUKPT_ERR_AP_VTSC_NULL_POINTER;
	}

	if(tid_len < AP_TID_SIZE || key1_len < AP_KEY_SIZE || ipek_len < AP_KEY_SIZE)
	{
		return DAMO_DUKPT_ERR_AP_VTSC_INVALID_LEN;
	}

	pt_len = ts_str_len+tid_len+AP_KEY_SIZE;

	if(recv_token_sc_len != pt_len)
	{
		return DAMO_DUKPT_ERR_AP_VTSC_INVALID_LEN;
	}

#ifdef DUKPT_SERVER
	pt = (unsigned char *)malloc(pt_len);

	if(!pt)
	{
		return DAMO_DUKPT_ERR_AP_VTSC_NULL_POINTER;
	}
#else
  if(INLENMAX < pt_len)
    return DAMO_DUKPT_ERR_AP_VTSC_INVALID_LEN;
#endif

	ret = DAMO_CRYPT_AES_Decrypt(pt, &pt_len, recv_token_sc, recv_token_sc_len, key1, key1_len, AES_128, CFB_MODE);
	if(ret != DAMO_DUKPT_SUCCESS)
	{
		return ret;
	}

	strptime(pt, "%a %b %d %H:%M:%S %Y", &tm);
	recv_ts = mktime(&tm);
	recv_ts = (recv_ts/ts_interval)*ts_interval;

	time(&ts);
	ts = (ts/ts_interval)*ts_interval;

	try_ts_cmp_cnt = 0;
	while(1)
	{
		if(recv_ts == ts)
			break;

		if(try_ts_cmp_cnt == 0)
		{
			ts -= ts_interval;
		}
		else if(try_ts_cmp_cnt == 1)
		{
			ts += (ts_interval*2);
		}
		else
		{
			ret = DAMO_DUKPT_ERR_AP_VTSC_VERI_FAIL;
			goto err;
		}

		try_ts_cmp_cnt++;
	}

	if(memcmp(pt+ts_str_len, tid, tid_len))
	{
		ret = DAMO_DUKPT_ERR_AP_VTSC_VERI_FAIL;
		goto err;
	}
	
	memcpy(ipek, pt+ts_str_len+tid_len, AP_KEY_SIZE);

err:
	if(pt)
	{
		memset(pt, 0x00, pt_len);
#ifdef DUKPT_SERVER
		free(pt);
#endif
	}

	return ret;
}
