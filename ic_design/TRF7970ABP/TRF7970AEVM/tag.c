#include "tag.h"
#include <stdlib.h>					// general purpose standard library
#define DBG_NFC	0

u08_t tag_flag;

extern u08_t	buf[300];
extern u08_t	i_reg;
extern u08_t	rx_done;
extern u08_t	tx_buf[23];
extern s16_t	nfc_state;
extern u08_t	reader_mode;
extern u08_t pupi[10];

u08_t	rx_len;
//u08_t	rx_data_pointer;

void Tag14443A(void);
u08_t Tag14443B(u08_t *UID);

void 
TagEmulation(u08_t *pbuf)
{
	u08_t protocol, uid[11], uid_len;
	u08_t i;
	
	reader_mode = 0xFF;	// set for target interrupt handling
	
	tag_flag = 1;
	
	protocol = *pbuf;
	uid_len = *(pbuf+1);
	
	uid[0] = NFCID;
	for(i = 1; i < 11; i++)
	{
		uid[i] = *(pbuf + 1 + i);
	}
	
	Trf797xWriteCont(uid, (uid_len + 1));
	
	while(1)
	{
		buf[0] = ISO_CONTROL;
		switch(protocol)
		{
			case 0x0A: buf[1] = 0x24;
				break;
			case 0x0B: buf[1] = 0x25;
				break;
			default:
				break;
		}
		Trf797xWriteSingle(buf, 2);
                
		buf[0] = RX_SPECIAL_SETTINGS;
		buf[1] = 0x3C;
		Trf797xWriteSingle(&buf[0], 2);
        
		buf[0] = NFC_TARGET_LEVEL;
		buf[1] = NFC_TARGET_LEVEL;
		buf[2] = NFC_LOW_DETECTION;
		buf[3] = BIT0;// 0x01;
		Trf797xReadSingle(&buf[1], 1);		// read the NFCTargetLevel register
		buf[1] |= BIT2 + BIT1 + BIT0;
		
		switch(uid_len)
		{
			case 0x04: buf[1] &= ~(BIT7 + BIT6);
				break;
			case 0x07: buf[1] &= ~BIT7;
				buf[1] |= BIT6;
				break;
			case 0x0A: buf[1] &= ~BIT6;
				buf[1] |= BIT7;
				break;
			default:
				break;
		}
		switch(protocol)
		{
			case 0x0A: buf[1] |= BIT5;
				break;
			case 0x0B: buf[1] &= ~BIT5;
				break;
			default:
				break;
		}
		Trf797xWriteSingle(buf, 2);
		
		buf[0] = ISO_14443B_OPTIONS;	
		buf[1] = ISO_14443B_OPTIONS;
		Trf797xReadSingle(&buf[1], 1);		
		buf[1] |= BIT0;			// set 14443A - 4 compliant bit
		buf[2] = CHIP_STATE_CONTROL;
		buf[3] = 0x21;
		Trf797xWriteSingle(buf, 4);
			
#if DBG_NFC
		buf[0] = TEST_SETTING_1;	// subcarrier output
		buf[1] = 0x40;
		Trf797xWriteSingle(buf, 2);
		buf[0] = MODULATOR_CONTROL;
		buf[1] = MODULATOR_CONTROL;
		Trf797xReadSingle(&buf[1], 1);
		buf[1] |= BIT3;
		Trf797xWriteSingle(buf, 2);
#endif
		
		Trf797xResetIrqStatus();
		Trf797xReset();	
		Trf797xStopDecoders();
		Trf797xRunDecoders();	

		IRQ_CLR;
		IRQ_ON;
		i_reg = 0x01;
		nfc_state = 1;
		//rx_data_pointer = 0;
		rx_len = 0;
		rx_done = 0;
		while(i_reg == 0x01 && !rx_done)		// wait for IRQ
		{
		}
	
		if(i_reg == 0xFF)	// on error skip
		{
			buf[50] = NFC_TARGET_LEVEL;
			buf[51] = NFC_TARGET_LEVEL;
			Trf797xReadSingle(&buf[51], 1);		// read the NFCTargetLevel register
			buf[51] &= ~BIT5;			// reset automatic SDD algorithm
			Trf797xWriteSingle(&buf[50], 2);

		  	switch(protocol)
		  	{
				case 0x0A:
					Tag14443A();
					break;
				case 0x0B:
					Tag14443B(uid);
					break;
				default:
					break;
			}
		}
	
		if (rx_done != 0) 
		{
			break;
		}
	}
}											// TagEmulation

void 
Tag14443A(void)
{
	OOK_DIR_IN;

//	if(TagATS()){
//		//if(buf[1] == 0x50)
//			goto EXIT;
//	}

//	PICCTransmissionProtocol();	//PPS is handled in this function

	return;
}				// Tag14443A



u08_t 
Tag14443B(u08_t *uid)
{
    u08_t i, slot, loop = 1, no_slots;
	rx_done = 0;
	
	buf[50] = RX_SPECIAL_SETTINGS;
	buf[51] = 0x3c;
	Trf797xWriteSingle(&buf[50], 2);

	tx_buf[5] = 0x50;		// set up the buffer for 14443B response
	tx_buf[6] = *(uid+1);
	tx_buf[7] = *(uid+2);
	tx_buf[8] = *(uid+3);
	tx_buf[9] = *(uid+4);
	tx_buf[10] = 0x20;
	tx_buf[11] = 0x38;
	tx_buf[12] = 0x19;
	tx_buf[13] = 0x33;
	tx_buf[14] = 0x02;
	tx_buf[15] = 0x21;
	tx_buf[16] = 0x85;
	
	while(buf[1] != 0x05)		// look for REQB or WUPB
	{
		Trf797xStopDecoders();
		Trf797xRunDecoders();	// reset the decoders
	  	nfc_state = 1;		// the response will be stored in buf[1] upwards	
		i_reg = 0x01;
		while(i_reg == 0x01)
		{
			if(rx_done != 0)
			{
				return (1);
			}
		}
	}
	
	while(loop)
	{
	  	if((buf[3] & 0x07))
	  	{
		  	no_slots = 1;
			loop = 0;
			for(i = 0; i < (buf[3] & 0x07); i++)
			{
				no_slots *= 2;
			}
			slot = (char)(rand()) & 0x0F;
			for(i = 0; i < no_slots; i++)
			{
				if(slot == (buf[1] >> 4))	// reply in the calculated slot
				{
				  	NfcFifoSend(12, 0, 0, 0);
				}
				else
				{
					Trf797xStopDecoders();	// reset the decoders for receiving
					Trf797xRunDecoders();	// the Next Slot Command
				}
				i_reg = 0x01;
				nfc_state = 1;
				while(i_reg == 0x01)
				{
					if(rx_done != 0)
					{	
						return(1);
					}
				}
				if(i_reg == 0x03)
				{
					loop = 0;
					break;
				}
				if(buf[1] == 0x05)		// restart anticollision loop
				{
					loop = 1;
					break;
				}
				else if(buf[1] == 0x1D)		// ATTRIB command received
				{
				  	//switch(TagATTRIB()){
					  //case 0: PICCTransmissionProtocol();	//go to level-4
					 // break;
					 // case 1: loop = 1;	//PUPI not matched
					  //break;
					  //case 2: loop = 0;	//transmit error
					  //break;
					//}//switch
				}
				else if(buf[1] == 0x50)		// HLTB received
				{
					loop = 1;
					for(i = 0; i < 4; i++)	// check for matched pupi
					{
						if(buf[2 + i] != pupi[i])
						{
							loop = 0;
							break;
						}
					}
				}
			}
		}
		else
		{
			NfcFifoSend(12, 1, 1, 0);	// reply in first slot
			loop = 0;
		}

	}
	return(0);
}					// Tag14443B

