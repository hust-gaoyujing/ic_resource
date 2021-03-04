#ifndef CRYPTO1_MIFARE_H_
#define CRYPTO1_MIFARE_H_


#include "types.h"

#define KEY_A_SELECT 0x60
#define KEY_B_SELECT 0x61

extern u08_t MifareSetKeyCommand(unsigned char *pbuf, unsigned char length);
extern u08_t MifareAuth1(unsigned char key_select, unsigned char block, unsigned char *uid, unsigned char length);
extern u08_t MifareAuth2(unsigned char *random, unsigned char length);
extern unsigned short* MifareTransceiveCommand(unsigned char *pbuf, unsigned short numbytes, char noCRC, unsigned char R_nW);
//extern void MifareTransceiveCommand(unsigned char *pbuf, unsigned short numbytes, char noCRC);
extern void MifareResetCipher(void);

#endif
