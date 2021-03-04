#include <string.h>
#include "crypto1.h"
#include "crypto1_mifare.h"

#include "spi.h"
//#include "globals.h"
#include "directmode.h"

static crypto1_state cipher_state;
static crypto1_state * const psMifareCipher = &cipher_state;
static int cipher_active = 0;

/* Initialize the crypto1 cipher and set a key (6 bytes) */
u08_t MifareSetKeyCommand(unsigned char *pui8MifareKey, unsigned char ui8Length)
{
	u08_t u8FlagSetKeyOK = 1;
	unsigned char i=0;
	uint64_t key = 0;

  if(!crypto1_new(psMifareCipher, 0, CRYPTO1_IMPLEMENTATION_CLEAN)) {
	  u8FlagSetKeyOK = 0;		//    Initialization failed
  }

  if(ui8Length == 6){
	  for(i=0; i<6; i++) {
		 key = (key << 8) | pui8MifareKey[i];
	  }
	  crypto1_init(psMifareCipher, key);
  }
  else{
	  u8FlagSetKeyOK = 0;		//   !! Key wrong length. Abort
  }

  return u8FlagSetKeyOK;
}

static parity_data_t auth1_command[4];
static parity_data_t auth1_response[5];

/* Perform first step of authentication (reader side) with given key A or B,
 *  block number and uid */
u08_t MifareAuth1(unsigned char key_select, unsigned char block, unsigned char *_uid, unsigned char length)
{
	unsigned char Double_UID = 0;
	unsigned short recv_bytes = 5;
	unsigned char recv_bits = 0, ucFail_Flag = 0;
	u08_t u8FlagAuth1OK = 0;
	uint32_t uid, tag_challange;

	auth1_command[0] = key_select;
	auth1_command[1] = block;
	CalculateParity(auth1_command, 2);
	CalculateCRC(auth1_command, 2);

	if(length > 4){  // checking for Double Size UID
	    Double_UID = 1;
	}

	DirectModeTransceive( DM_TX_PARITY | DM_RX_PARITY,
						   auth1_command, 4, 0,
						   auth1_response, &recv_bytes, &recv_bits);

	__no_operation();
	if((recv_bytes != 4) || (recv_bits != 0)) {
		ucFail_Flag = 1;		//expected 4 bytes and 0 bits. Abort
	}

	if(!CheckParity(auth1_response, 4)) {
		ucFail_Flag = 1;
	}

	if(ucFail_Flag){
		MifareResetCipher();
		LEDAuth1OFF;
		LEDAuth2OFF;
		LEDAuth3OFF;
		LED_MFC_READ_BLOCK_OFF;
	}
	else{ // Tag response is good.
		LEDAuth1ON;

		if(!Double_UID){
			uid = ARRAY_TO_UINT32(_uid);  // 4 byte UID
		}
		else{           // 7 byte UID
			uid = ((uint32_t)((_uid)[5]&0xff) << 24) | ((uint32_t)((_uid)[6]&0xff) << 16) | \
				   ((uint32_t)((_uid)[7]&0xff) << 8) | ((uint32_t)((_uid)[8]&0xff) << 0);
		}

		tag_challange = ARRAY_TO_UINT32(auth1_response);

		crypto1_mutual_1(psMifareCipher, uid, tag_challange);

		u8FlagAuth1OK = 1;
	}

	return u8FlagAuth1OK;
}

static parity_data_t auth2_command[8];
static parity_data_t auth2_response[5];

/* Perform second step of authentication (reader side), use given random number
 *  for the reader nonce. */
u08_t MifareAuth2(unsigned char *random, unsigned char length)
{
	unsigned short recv_bytes = 5;
	unsigned char recv_bits = 0, ucFail_Flag = 0;
	unsigned char u8FlagAuth2OK = 0;

	if(length < 4) {		//!! Random too short. Abort
		MifareResetCipher();
		ucFail_Flag = 1;
	}
  
	UINT32_TO_ARRAY_WITH_PARITY( ARRAY_TO_UINT32(random), auth2_command);

	crypto1_mutual_2(psMifareCipher, auth2_command);

	DirectModeTransceive( DM_TX_PARITY | DM_RX_PARITY,
							auth2_command, 8, 0,
							auth2_response, &recv_bytes, &recv_bits);
	__no_operation();
	if(recv_bytes != 4 || recv_bits != 0) {
		 ucFail_Flag = 1;
	}
	else{  // Auth 2 success (received response correct lenght)
		LEDAuth2ON;

		if(crypto1_mutual_3(psMifareCipher, auth2_response)) {  // Check response: Auth 3
			LEDAuth3ON;
		}
		else{
			ucFail_Flag = 1;
		}
	}

	if( ucFail_Flag ){
		MifareResetCipher();
		LEDAuth2OFF;
		LEDAuth3OFF;
		LED_MFC_READ_BLOCK_OFF;
	}
	else{	//Authentication ok. Success
		cipher_active = 1;

		u8FlagAuth2OK = STATUS_SUCCESS;
	}

	return u8FlagAuth2OK;
}

static parity_data_t transceive_buffer[19];
/* Host command. Encrypt and transmit given buffer, receive and decrypt response
 *  then print response. Behaves much like RequestCommand() except for the
 *  possibility to send incomplete bytes. Command or Response shall not exceed 
 *  18 bytes (including CRC). Addition: Incomplete received bytes (broken bits)
 *  are printed in a second pair of []. */
unsigned short* MifareTransceiveCommand(unsigned char *pbuf, unsigned short numbytes, char noCRC, unsigned char R_nW)
{
	int i;
  if(!cipher_active) {
//!! cipher not initialized. Abort
    return 0;
  }
  
  if( (noCRC && numbytes > 18) || (!noCRC && numbytes > 16) ) {
//!! numbytes too big. Abort
    return 0;
  }
  
  for(i=0; i<numbytes; i++)
    transceive_buffer[i] = pbuf[i];
  CalculateParity(transceive_buffer, numbytes);
  
  if(!noCRC)
    numbytes = CalculateCRC(transceive_buffer, numbytes);
  
//  LEDspecialINVERT;
  crypto1_transcrypt(psMifareCipher, transceive_buffer, numbytes);
//  LEDspecialINVERT;
  
  unsigned short recv_bytes = 19;
  unsigned char recv_bits = 0;
  DirectModeTransceive(DM_TX_PARITY | DM_RX_PARITY,
                       transceive_buffer, numbytes, 0,
                       transceive_buffer, &recv_bytes, &recv_bits);
  
//  LEDspecialINVERT
  crypto1_transcrypt_bits(psMifareCipher, transceive_buffer, recv_bytes, recv_bits);
//  LEDspecialINVERT;
  
  if(recv_bytes > 2) {
    if(!CheckParity(transceive_buffer, recv_bytes)) {
//      !! parity error. Abort
      MifareResetCipher();
      return 0;
    }
    
    if(!CheckCRC(transceive_buffer, recv_bytes)) {
//!! CRC error. Abort
      MifareResetCipher();
      return 0;
    }
    
    recv_bytes -= 2;
  }
  
  if( (recv_bytes == 0) && (recv_bits == 0) )
  {
	  _nop();
	  return 0;
  }


  return (unsigned short*)transceive_buffer;
}

void MifareResetCipher(void)
{
  memset(&cipher_state, 0, sizeof(cipher_state));
  cipher_active = 0;
}
