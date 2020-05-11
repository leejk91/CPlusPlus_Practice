#ifndef DUKPT_H_
#define DUKPT_H_
#include <stdlib.h>
#include <string.h>

typedef struct {
	unsigned char ksn[10]; /* key serial number(encryption counter ����) */
	unsigned char enc_pin_block[8]; /* encrypted pin block */
	unsigned char pin_enc_key[16]; /* pin enc key */
	unsigned char req_mac_key[16]; /* request mac key */
	unsigned char res_mac_key[16]; /* response mac key */
	unsigned char req_enc_key[16]; /* request enc key */
	unsigned char res_enc_key[16]; /* response enc key */
} DukptPinEntry;

typedef struct {
	unsigned char key_serial_number[17];
	unsigned char future_key_register[21][25];
} DukptFutureKeyInfo;

enum DAMO_DUKPT_PADDING {
	DAMO_PKCS7_PADDING=0,
	DAMO_ZERO_PADDING,
};
enum DAMO_DUKPT_MSB {
	DAMO_TR31_NOT_STD=0,
	DAMO_TR31_STD,
};
enum DAMO_DUKPT_CLIENT_WORD {
	DAMO_DUKPT_WORD_HEX=0,
	DAMO_DUKPT_WORD_STRING,
};

/* �ʿ��� Ű�� ���� �÷���(DAMO_DUKPT_Request_Pin_Entry�Լ����� ���) */
#define DAMO_DUKPT_FLAG_USE_NOT_KEY		  0x0000 /* ��� Ű�� ������� ���� */
#define DAMO_DUKPT_FLAG_USE_PIN_ENC_KEY	  0x0001 /* PIN ��ȣȭŰ ��� */
#define DAMO_DUKPT_FLAG_USE_DATA_MAC_KEY  0x0002 /* DATA MACŰ ��� */
#define DAMO_DUKPT_FLAG_USE_DATA_ENC_KEY  0x0004 /* DATA ��ȣȭŰ ��� */
#define DAMO_DUKPT_FLAG_USE_ALL_KEY		  0x0007 /* ��� Ű ��� */
#endif
