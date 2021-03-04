//#include "hardware.h"
//#include "TRF7970A_SPI.h"
//#include "anticollision.h"
//#include "globals.h"
//#include "14443.h"
//#include "host.h"
#include "directmode.h"
#include "crypto1_mifare.h"
#include "RFid_functions.h"


unsigned char u8ArrUID[12];
u08_t u8MFCAuth2Challenge[4];

unsigned char RFID_AuthenticateA(unsigned char* pu8MifareKey, unsigned char u8Sec_Block, unsigned char* pu8UID, unsigned char pu8UIDLength)
{
  unsigned char uc8MFCAuthSuccess = 0;

	/* Crypto1 set key */
	MifareResetCipher();
	if(MifareSetKeyCommand(pu8MifareKey, 6))
	{
		  /* Crypto1 authentication step 1 */
		  if(MifareAuth1(KEY_A_SELECT, u8Sec_Block, pu8UID, pu8UIDLength))
		  {
			  u8MFCAuth2Challenge[0] = 0x11; // Our test challenge
			  u8MFCAuth2Challenge[1] = 0x22;
			  u8MFCAuth2Challenge[2] = 0x33;
			  u8MFCAuth2Challenge[3] = 0x44;

			  uc8MFCAuthSuccess = MifareAuth2(u8MFCAuth2Challenge, 4);
		  }
	}/*if (u8FlagSetKeyOK)*/

	return uc8MFCAuthSuccess;
}/*RFID_AuthenticateA()*/

void RFID_AuthenticateB(unsigned char* pu8MifareKey, unsigned char u8Sec_Block, unsigned char* pu8UID)
{
//  unsigned char buf[4];
////  unsigned char* pbuf;
//
////  if (u8FlagScanOK)
//  {
//      //--> 010E000304 C0 xxxxxxxxxxxx 0000
//        /* Crypto1 set key */
//        MifareResetCipher();
//        MifareSetKeyCommand(pu8MifareKey, 6);
//
////        if (u8FlagSetKeyOK)
//        {
//            //--> 010F000304 C1 6000EC91EA1F88 0000
//              /* Crypto1 authentication step 1 */
//              MifareAuth1(0x61,u8Sec_Block,pu8UID, 5); //0x60 = TypeA, 0x61 = TypeB
////              delay_ms(100);
////              if (u8FlagAuth1OK)
//              {
//                      //--> 010C000304 C2 11223344 0000
//                      buf[0] = 0x11; //LeMaBa: check on this, should be random?
//                      buf[1] = 0x22;
//                      buf[2] = 0x33;
//                      buf[3] = 0x44;
//                      pbuf = buf;
////                      MifareAuth2(pbuf, 4);
//              }
//        }/*if (u8FlagSetKeyOK)*/
//  }/*if (u8FlagScanOK)*/


}/*RFID_AuthenticateB()*/




void RFID_ReadBlock(unsigned char u8Sec_Block, unsigned char* pu8BlockData)
{
	unsigned char i;
	unsigned short* transceive_buffer_aux;
	unsigned char uc8TempBuff[4];

	/* encrypted request mode - Read */
	uc8TempBuff[0] = 0x30;
	uc8TempBuff[1] = u8Sec_Block;
	transceive_buffer_aux = MifareTransceiveCommand(&uc8TempBuff[0], 2, 0, 1);

	if (transceive_buffer_aux)
	{
		for (i =0 ; i<16 ; i++)
		{
		  pu8BlockData[i] = (unsigned char)transceive_buffer_aux[i];
		}

		LED_MFC_READ_BLOCK_ON;
	}
	else
	{
		LED_MFC_READ_BLOCK_OFF;
	}
  
}/*RFID_ReadBlock()*/

void RFID_WriteBlock(unsigned char u8Sec_Block, unsigned char* pu8BlockData)
{

//	unsigned short* u16FlagRFidWR;

//	u8FlagWriteOK = 0;

//  if (u8FlagAuth2OK)
  {
  
        //  --> 010A000304 C8 A0 xx 0000
        /* encrypted request mode - Write */
//          buf[0] = 0xA0;
//          buf[1] = u8Sec_Block;
//          u16FlagRFidWR = MifareTransceiveCommand(&buf[0], 2, 0, 0);
//          if (u16FlagRFidWR != 0)
//          {
//
//				//--> 0118000304 C8 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx 0000
//				/* encrypted request mode - Write Data */
//				  u16FlagRFidWR = MifareTransceiveCommand(pu8BlockData, 16, 0, 0);
//				  if (u16FlagRFidWR != 0)
//				  {
////					  u8FlagWriteOK = 1;
//				  }
//				  else
//				  {
//					  _nop();
//				  }
//          }
//          else
//          {
//			  _nop();
//		  }
  }/*if (u8FlagAuth2OK)*/
}/*RFID_WriteBlock()*/
