
#include "types.h"

unsigned char RFID_AuthenticateA(unsigned char* pu8MifareKey, unsigned char u8Sec_Block, unsigned char* pu8UID, unsigned char pu8UIDLength);
void RFID_AuthenticateB(unsigned char* pu8MifareKey, unsigned char u8Sec_Block, unsigned char* pu8UID);
void RFID_ReadBlock(unsigned char u8Sec_Block, unsigned char* pu8BlockData);
void RFID_WriteBlock(unsigned char u8Sec_Block, unsigned char* pu8BlockData);
