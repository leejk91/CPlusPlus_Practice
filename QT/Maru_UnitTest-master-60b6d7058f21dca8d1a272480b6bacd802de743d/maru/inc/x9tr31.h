#ifndef X9TR31_H_
#define X9TR31_H_

#ifdef __cplusplus
extern "C" {
#endif


#define ENCRYPT		1
#define DECRYPT		0

void DAMO_X9TR31_Get_Enc_Mac_Key(unsigned char *keyBlockProtectionKey, unsigned char *encryptionKey, unsigned char *MACKey);


#ifdef __cplusplus
}
#endif


#endif /* X9TR31_H_ */
