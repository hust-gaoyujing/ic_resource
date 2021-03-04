/*
 * mfc_app.c
 *
 *  Created on: Nov 13, 2014
 *      Author: a0273119
 */

#include "tag_header.h"
//#include "mfc_app.h"
//#include "crypto1_mifare.h"

bool g_bNdefWaitForRsp;
uint8_t MFC_A_Key[6] = MFC_DEFAULT_KEY;
uint8_t Auth_2_Challange[4] = AUTH_2_CHALLANGE;

tMFCStateMachine g_eMFCStatus;
//extern crypto1_state cipher_state;
//extern crypto1_state * const ch;

//*************MIFARE Classic Code ***************

uint16_t g_ui16MifareTx[30] = {0x160,0x100,0x1F5,0x17B};
//uint16_t g_ui16MifareTx[30] = {0x55,0x121,0x143,0x065};

//static parity_data_t auth1_command[4];
//static parity_data_t auth1_response[5];

void MFC_process(void)
{
	uint8_t ui8RxByteLength = 0;
	uint8_t ui8RxBitLength = 0;
	uint16_t * pui8RxData = 0;

	uint8_t ui8TxByteLength = 0;

	crypto1_state * sCryptoState;

	uint8_t ui8RxSuccessFlag = 1;
	tMifareConnectionStatus eMifareState;

	tUidLength ui8UidLength = NFC_A_UID_INCOMPLETE;
	uint8_t * pui8UidData = 0;
//	uint32_t ui32Uid, ui32Auth1Challange;

	uint8_t ui8DoubleUIDFlag = 0;

	// Waiting for response
	if(g_bNdefWaitForRsp == true)
	{
		switch(g_eMFCStatus)
		{
		case MFC_WRITE_INIT:
			if(MIFARE_getCommStatus() == MIFARE_RECEIVE_SUCCESS)
			{
				// Do something, AKA go to the next state of auth (i.e.)
//				g_eMFCStatus = MFC_DISCONNECTED;
				g_bNdefWaitForRsp = false;
			}
			else
			{
				g_eMFCStatus = MFC_DISCONNECTED;
				g_bNdefWaitForRsp = false;
			}
			break;
		case MFC_DISCONNECTED:
			break;
		case MFC_AUTH1:
			eMifareState = MIFARE_getCommStatus();

			if(eMifareState == MIFARE_RECEIVE_SUCCESS){  	// check receive state
				MIFARE_getPacketStatus(&pui8RxData, &ui8RxByteLength, &ui8RxBitLength);	// Recieve Data

//				pui8RxData[0] = 0x1A3;
//				pui8RxData[1] = 0x169;
//				pui8RxData[2] = 0x076;
//				pui8RxData[3] = 0x14E;

				if(ui8RxByteLength != 4 || ui8RxBitLength != 0){  		// Check length
					ui8RxSuccessFlag = 0;
//					g_eMFCStatus = MFC_DISCONNECTED;
//					g_bNdefWaitForRsp = false;
				}
				else if(!CheckParity(pui8RxData, ui8RxByteLength)){  	// Check Parity
					ui8RxSuccessFlag = 0;
//					g_eMFCStatus = MFC_DISCONNECTED;
//					g_bNdefWaitForRsp = false;
				}



				g_bNdefWaitForRsp = false;
			}
			else if(eMifareState == MIFARE_RECEIVE_FAIL){			// wait or fail
//				g_eMFCStatus = MFC_DISCONNECTED;
				ui8RxSuccessFlag = 0;

			}
			else{
				while(1);
			}

			//TODO add Receive on going state
			if(ui8RxSuccessFlag){
				g_eMFCStatus = MFC_AUTH2;
			}
			else{		// failed somewhere
				g_eMFCStatus = MFC_DISCONNECTED;

			}
			g_bNdefWaitForRsp = false;

			break;
		case MFC_AUTH2:
			break;
		case MFC_AUTH3:

			eMifareState = MIFARE_getCommStatus();

			if(eMifareState == MIFARE_RECEIVE_SUCCESS){  	// check receive state
				MIFARE_getPacketStatus(&pui8RxData, &ui8RxByteLength, &ui8RxBitLength);	// Recieve Data

				if(ui8RxByteLength != 4 || ui8RxBitLength != 0){  		// Check length
					ui8RxSuccessFlag = 0;
//					g_eMFCStatus = MFC_DISCONNECTED;
//					g_bNdefWaitForRsp = false;
				}
//				else if(!CheckParity(pui8RxData, ui8RxByteLength)){  	// Check Parity
//					ui8RxSuccessFlag = 0;
////					g_eMFCStatus = MFC_DISCONNECTED;
////					g_bNdefWaitForRsp = false;
//				}
				else{
					sCryptoState = getCryptoState();
					if(!crypto1_mutual_3(sCryptoState, pui8RxData)) {  // Check response: Auth 3
					//!! Invalid card response. Abort
						ui8RxSuccessFlag = 0;
					}
				}



				g_bNdefWaitForRsp = false;
			}
			else if(eMifareState == MIFARE_RECEIVE_FAIL){			// wait or fail
//				g_eMFCStatus = MFC_DISCONNECTED;
				ui8RxSuccessFlag = 0;

			}
			else if(eMifareState == MIFARE_RECEIVE_ONGOING){			// wait

			}
			else{
				while(1); //trap
//				__no_operation();
			}

			//TODO add Receive on going state
			if(ui8RxSuccessFlag){
				g_eMFCStatus = MFC_READ_BLOCK;
			}
			else{		// failed somewhere
				g_eMFCStatus = MFC_DISCONNECTED;

			}
			g_bNdefWaitForRsp = false;

			break;

		case MFC_READ_BLOCK:

			g_bNdefWaitForRsp = false;  // Clear "waiting for receive flag"

			eMifareState = MIFARE_getCommStatus();

			if(eMifareState == MIFARE_RECEIVE_SUCCESS){  	// check receive state
				MIFARE_getPacketStatus(&pui8RxData, &ui8RxByteLength, &ui8RxBitLength);	// Get Recieve Data

				sCryptoState = getCryptoState();		// Get Crypto state
				crypto1_transcrypt_bits(sCryptoState, pui8RxData, ui8RxByteLength, ui8RxBitLength);	// Decode bits

				if(ui8RxByteLength > 2){  		// Check length
					if(!CheckParity(pui8RxData, ui8RxByteLength)){	// Check Parity
						ui8RxSuccessFlag = 0;
					}
					if(!CheckCRC(pui8RxData, ui8RxByteLength)) {	// Check CRC
						ui8RxSuccessFlag = 0;
					}
				}
			}
			else if(eMifareState == MIFARE_RECEIVE_ONGOING){			// wait
				g_bNdefWaitForRsp = true;			// Still waiting for Receive
			}
			else{
				ui8RxSuccessFlag = 0;
			}

			// For now, just exit state machine
			g_eMFCStatus = MFC_DISCONNECTED;
			g_bNdefWaitForRsp = false;
//
//			if(ui8RxSuccessFlag){
//				g_eMFCStatus = MFC_READ_BLOCK;
//			}
//			else{		// failed somewhere
//				g_eMFCStatus = MFC_DISCONNECTED;
//
//			}


			break;

		}
	}



	if(g_bNdefWaitForRsp == false) // transmitting
	{
		switch(g_eMFCStatus)
		{
		case MFC_WRITE_INIT:
	        MifareResetCipher();
	        if(MifareSetKeyCommand(MFC_A_Key, 6)){
	        	g_eMFCStatus = MFC_AUTH1;
	        }
	        else{
	        	g_eMFCStatus = MFC_DISCONNECTED;
	        }

//			Mifare_sendRawData(g_ui16MifareTx,0x04,true,true);
//			g_bNdefWaitForRsp = true;
			break;
		case MFC_DISCONNECTED:
			MifareResetCipher();
			NFC_triggerRWProtocolError();
			break;

		case MFC_AUTH1:
			// calculate packet  *****
			// TODO make this into a function, and pass in key selece and block.

			g_ui16MifareTx[0] = KEY_A_SELECT_CMD;	// Auth Key Select
			g_ui16MifareTx[1] = 0X00; 			// BLOCK NUMBER
			ui8TxByteLength = 2;

			CalculateParity(g_ui16MifareTx, ui8TxByteLength);
			ui8TxByteLength = CalculateCRC(g_ui16MifareTx, ui8TxByteLength);

			// Transmit  *********
			if(MIFARE_sendRawData(g_ui16MifareTx, ui8TxByteLength, true, true)){
				g_bNdefWaitForRsp = true;  // go receive response
			}
			else{
				g_eMFCStatus = MFC_DISCONNECTED;
			}
			break;

		case MFC_AUTH2:
			NFC_A_getNfcAId(&pui8UidData, &ui8UidLength);  // Get UID

			if(ui8UidLength != NFC_A_UID_DOUBLE){
//				ui32Uid = ARRAY_TO_UINT32(pui8UidData);  		// 4 byte UID
				ui8DoubleUIDFlag = 0;
			}
			else{          					   		// 7 byte UID
				ui8DoubleUIDFlag = 1;
//				ui32Uid = ((uint32_t)((pui8UidData)[4]&0xff) << 24) | ((uint32_t)((pui8UidData)[5]&0xff) << 16) | \
//					   ((uint32_t)((pui8UidData)[6]&0xff) << 8) | ((uint32_t)((pui8UidData)[7]&0xff) << 0);
			}

//			ui32Auth1Challange = ARRAY_TO_UINT32(pui8RxData);		// calculate Auth 2 challenge
//			crypto1_mutual_1_WA(ui32Uid, ui32Auth1Challange);		// encrypt challenge

//			UINT32_TO_ARRAY_WITH_PARITY( ARRAY_TO_UINT32(Auth_2_Challange), g_ui16MifareTx);
//			crypto1_mutual_2_WA(g_ui16MifareTx);
//			crypto1_mutual_2(ch, g_ui16MifareTx);

			MifareAuth2(Auth_2_Challange, 4, g_ui16MifareTx, ui8DoubleUIDFlag, pui8UidData, pui8RxData);

//			void MifareAuth2(  parity_data_t *auth1_resp )

			if(MIFARE_sendRawData(g_ui16MifareTx, 0x08, true, true)){
				g_eMFCStatus = MFC_AUTH3;
				g_bNdefWaitForRsp = true;  // go receive response
			}
			else{
				g_eMFCStatus = MFC_DISCONNECTED;
			}
			break;
		case MFC_AUTH3:
			break;
		case MFC_READ_BLOCK:
			// Need Block Number
			g_ui16MifareTx[0] = MFC_READ_BLOCK_CMD;	// MFC Command
			g_ui16MifareTx[1] = 0X00; 			// BLOCK NUMBER
//			ui8TxByteLength = 2;
//
//			CalculateParity((parity_data_t *)g_ui16MifareTx, ui8TxByteLength);
//			ui8TxByteLength = CalculateCRC(g_ui16MifareTx, ui8TxByteLength);

			MifareTransceiveCommand(g_ui16MifareTx, 2, 0, 1);

			if(MIFARE_sendRawData(g_ui16MifareTx, 4, true, true)){
				g_eMFCStatus = MFC_READ_BLOCK;
				g_bNdefWaitForRsp = true;  // go receive response
			}
			else{
				g_eMFCStatus = MFC_DISCONNECTED;
			}

			break;
		case MFC_WRITE_BLOCK:
			break;

		}
	}
}




