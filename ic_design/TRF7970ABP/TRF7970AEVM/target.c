#include "target.h"
#include <stdlib.h>					// general purpose standard library

u08_t TargetAtributeResponse(u08_t data);
u08_t TargetSelectedState(void);
void TargetActive(u08_t data);
void TargetPassive(void);

u08_t target_cid[11];
u08_t nfc_protocol;
u08_t block_no_se;
u08_t pupi[10];

extern u08_t buf[300];
extern u08_t i_reg;
extern u08_t irq_flag;
extern u08_t rx_done;
extern u08_t reader_mode;

extern u08_t active;
extern s16_t nfc_state;
extern u08_t nfc_iso;
extern u08_t tx_buf[23];

//------------------------------------------------------------
//Sends the Atribute Response when in TARGET mode.
//------------------------------------------------------------
u08_t
TargetAtributeResponse(u08_t data)
{
	u08_t did, bs, br, pp, to = 0x08, i;

//UartPutChar('A');
//kputchar('A');
	if(data == 0x00)
	{		
		nfc_state = 0x01;	//the request will be stored from buf[1] upwards
		i_reg = 0x01;
		buf[1] = 0x00;
		buf[2] = 0x00;
	
		do	//wait for ATR_REQ
		{
			while(i_reg == 0x01)
			{
				if (rx_done)
				{
					return(0);
				}
			}//while
//UartPutChar('%');
//kputchar('%');
			nfc_state = 0x01;
			i_reg = 0x01;
		}while((buf[2] != 0xD4) && (buf[3] != 0x00));
	}
	else
	{
		if((buf[2] != 0xD4) && (buf[3] != 0x00)) 
		{
			return(0);
		}
	}
//UartPutChar('#');
//kputchar('#');

	//ATR_REQ has been received - store the INITIATOR CID
	did = buf[14];
	bs  = buf[15];
	br  = buf[16];
	pp  = buf[17];
	
	buf[5] = 17;			//the field LEN (3 - 255)
	buf[6] = RES;			//prepare the buffer for response
	buf[7] = ATR;
	for(i = 1; i < 11; i++)
	{
		buf[7 + i] = target_cid[i];
	}
	buf[18] = did;
	buf[19] = bs;
	buf[20] = br;
	buf[21] = to;
	buf[22] = pp;	
	
	if(active)
	{
		if(NfcRfCollisionAvoidance(0x01))
		{
//UartPutChar('C');
//kputchar('C');
			NfcSend(&buf[5]);		//send the response
			return(1);
		}
	}
	else
	{
//UartPutChar('p');
//kputchar('p');
		NfcSend(&buf[5]);		//send the response
		return(1);
	}

//UartPutChar('_');
//kputchar('_');
	return(0);
}//ATR_RES


//----------------------------------------------------------------------------
//This function handles the requests that are allowed in the selected	
//state. Those are PSL_REQ, DSL_REQ, DEP_REQ and RLS_REQ. Any other request	
//will put the target out of the selcted state.		
//----------------------------------------------------------------------------
u08_t
TargetSelectedState(void)
{
	u08_t did, pni = 0x00, bs, br, pp, to = 0x08;
	//u08_t command;
	u16_t i;
	static unsigned char count = 0;
	
	rx_done = 0;
//kputchar('?');
//kputchar('?');
	nfc_state = 0x01;	//the request will be stored from buf[1] upwards
	i_reg = 0x01;

	while(i_reg == 0x01)
	{
		if (rx_done)
			return 1;
	}
	
	Trf797xResetIrqStatus();
	
	if(i_reg == 0x02)
	{
		buf[8] = 0x50 | (pni & 0x03);   //PFB field NAK	
		buf[5] = 4;	//the field LEN
		buf[6] = RES;   //prepare the reaponse ACK
		buf[7] = DEP;
		if(active)
		{
			NfcRfCollisionAvoidance(0x02);
		}
		NfcSend(&buf[5]);
		return 1;
	}
	
	if(buf[2] != 0xD4)
	{
		return 1;
	}

	switch(buf[3])
	{
		case 0x00: 	//atribute request
		
			did = buf[14];
			bs  = buf[15];
			br  = buf[16];
			pp  = buf[17];
	
			buf[5] = 17;		//the field LEN (3 - 255)
			buf[6] = RES;		//prepare the buffer for response
			buf[7] = ATR;
			for(i = 1; i < 11; i++)
			{
				buf[7 + i] = *(target_cid + i);
			}
			buf[18] = did;
			buf[19] = bs;
			buf[20] = br;
			buf[21] = to;
			buf[22] = pp;	
	
			if(active)
			{
				NfcRfCollisionAvoidance(0x01);
			}
			NfcSend(&buf[5]);	//send the response
		break;
		
		case 0x02:
			break;
			
		case 0x04: 	//parameter selection request
			if((count != 0x00) || (nfc_state != 0x05))	//in case where the recieved request is not complete
			{
				return 1;				//or it is not the first command leave function
			}

			did = buf[3];			//store the requested parameters
//			brS = buf[4];
//			FSL = buf[5];

			buf[5] = 3;
			buf[6] = RES;			//prepare the buffer for response
			buf[7] = PSL;
			
			if(active)
			{
				NfcRfCollisionAvoidance(0x02);	//send the response
			}
			NfcSend(&buf[5]);
			
			count = 0x01;		
			break;
		
		case 0x16: 	//data exchange protocol
			if((buf[4] & 0xE0) == 0x00)
			{
				count = 0x01;
				pni = buf[4] & 0x03;
				UartPutChar('[');			//send the recieved data to the host-
				for(i = 4; i < nfc_state; i++)	//including PFB
				{
					UartPutByte(buf[i]);
				}//for
				UartPutChar(']');
				UartPutCrlf();
				
				//wait for ACK/NAK from host befor sending ACK/NAK to INITIATOR
				//SimpleTimer = 1;
				buf[1] = buf[4] = 0;
				if(! UartGetLine(buf) && (buf[4] == 0xD4) && ((buf[5] & 0x10) == 0x10))
				{
					//SimpleTimer = 0;
// kputchar('y');				
// kputchar('y');				
					if(buf[5] == ACK_HOST)			
						buf[8] = 0x40 | (pni & 0x03);   //PFB field ACK
					else
						buf[8] = 0x50 | (pni & 0x03);   //PFB field NAK
				
					buf[5] = 4;	//the field LEN
					buf[6] = RES;   //prepare the reaponse ACK
					buf[7] = DEP;
					if(active)
					{
						NfcRfCollisionAvoidance(0x02);
					}
					NfcSend(&buf[5]);
				}
				else
				{
					//SimpleTimer = 0;
					/*//reset reader
					P1OUT |= BIT7;
					P1OUT &= ~BIT7;
					buf[0] = 0x00;
					buf[1] = 0x01;
					buf[2] = 0x2A;
					WriteCont(&buf[0], 3);  */
//kputchar('n');
//kputchar('n');
					break;
				}
			}
			else if((buf[4] & 0xE0) == 0x40)	//recieved PDU is ACK/NAK - ignore
			{
			}
			else if((buf[4] & 0xE0) == 0x80)	//recieved PDU is Attention-Target present
			{
				buf[5] = 3;
				buf[6] = RES;
				buf[7] = DEP;
				
				if(active)
				{
					NfcRfCollisionAvoidance(0x02);	//send the response
				}
				NfcSend(&buf[5]);
			}//if
		break;
		
		case 0x08:
			i_reg = 0x00;	//DSL
			buf[5] = 3;
			buf[6] = RES;
			buf[7] = DSL;

			if(active)
			{
				NfcRfCollisionAvoidance(0x02);	//send the response
			}
			NfcSend(&buf[5]);	
			return 0;		//leave selected state and wait for WUP_REQ
		case 0x0A:
			i_reg = 0xFF;	//RLS
			buf[5] = 3;
			buf[6] = RES;
			buf[7] = RLS;

			if(active)
			{
				NfcRfCollisionAvoidance(0x02);	//send the response
			}
			NfcSend(&buf[5]);
			return 0;		//leave selected state and go to initial target mode
	}//switch	

	return 1;
}//TargetSelectedState


//--------------------------------------------------------
//This functions handles the state transitions that are
//defined for NFC TARGET active communication mode.
//--------------------------------------------------------
void
TargetActive(u08_t data)
{
#if DBG_NFC
	buf[50] = TEST_SETTING_1;	//subcarrier output
	buf[51] = 0x40;
	Trf797xWriteSingle(&buf[50], 2);

	buf[50] = RX_SPECIAL_SETTINGS;
	buf[51] = 0x1C;
	Trf797xWriteSingle(&buf[50], 2);

#endif
//kputchar('P');
//Put_byte(data);
//kputchar('P');
//	while(1){	// KOSTA - ne gre ven
		//target state
		if(TargetAtributeResponse(data))	// KOSTA - was while - did not exit
		{
			//selected state	
			UartSendCString("[s1]");
			while(TargetSelectedState()){}
			UartSendCString("[s0]");
			//while
			//if(i_reg == 0xFF || RXdone)	// KOSTA
//				break;			// ???
			data = 0x00;		// ???
		}//while
//		if(RXdone)				// ???
//			return;				// ???
//		data = 0x00;			// ???
//		break;					// VODA
//	}
	
}//TargetActive



//------------------------------------------------------------
//Set NFC pasive target for 212 and 424kbps - Felica type.
//Sets the FPGA and the reader for tag emulation.
//This function performs the Felica type anticollision
//protocol - max. 16 timeslots delimited only with time.
//------------------------------------------------------------
void
TargetPassive(void)
{
  	u08_t slot, i, no_slots;
#if DBG_NFC
	buf[50] = TEST_SETTINGS_1;	//subcarrier output
	buf[51] = 0x40;
	Trf797xWriteSingle(&buf[50], 2);
	buf[50] = MODULATOR_CONTROL;	//analog output
	buf[51] = MODULATOR_CONTROL;
	Trf797xReadSingle(&buf[51], 1);
	buf[51] |= BIT3;
	Trf797xWriteSingle(&buf[50], 2);
#endif
	no_slots = buf[5];
	slot = (char)(rand()) & no_slots;
	
	tx_buf[0] = 0x12;
	tx_buf[1] = 0x01;
	for(i = 0; i < 8; i++)
	{
		tx_buf[i + 2] = target_cid[i];
	}
	for(i = 0; i < 8; i++)		//8 bytes of pad
	{
	  	tx_buf[10 + i] = 0x00;
	}

	irq_flag = 0x00;
	McuCounterSet();
	COUNT_VALUE = 1100;		//2.4ms for TIMEOUT
	START_COUNTER;		//start timer up mode
	while(irq_flag != 0x03)
	{
	}

	for(i = 0; i < (no_slots + 1); i++)
	{
		if(i == slot)
		{			//send the reply in the calculated timeslot
			NfcFifoSend(18, 0, 0, 0);
			break;
		}

		irq_flag = 0x00;
		McuCounterSet();
		COUNT_VALUE = 340;		//1.2ms for TIMEOUT
		START_COUNTER;		//start timer up mode
		while(!irq_flag != 0x03)
		{
		}
	}//for
}//NFCPassiveTarget()



//-------------------------------------------------------
//Performs anticollision and sets the target device
//for correct operation. Has to check if the
//INITIATOR device wants to communicate in ACTIVE mode,
//PASSIVE mode and at whitch speed.
//-------------------------------------------------------
void
TargetAnticollision(u08_t *pbuf, u08_t tag)
{
  	u08_t i = 0;
//	static unsigned char first = 1;

		buf[0] = NFC_TARGET_LEVEL;
		buf[1] = NFC_TARGET_LEVEL;
	    buf[2] = NFC_LOW_DETECTION;
	    buf[3] = BIT0;//0x01;
		Trf797xReadSingle(&buf[1], 1);		//read the NFCTargetLevel register
		buf[1] |= BIT5 + BIT2 + BIT1 + BIT0;	//set automatic SDD algorithm
		Trf797xWriteSingle(buf, 4);        


	while(1)
	{
		reader_mode = 0xF0;	//set for target interrupt handling
		IRQ_OFF;
		//ResetTRF(); /////////////////////////////
		OOK_DIR_IN;
		nfc_protocol = 0x00;
		block_no_se = 1;
            //    if ( NFCIso == 0x21)
            //    {
		buf[0] = ISO_CONTROL;
		buf[1] = 0x01;			//set NFC, passive, target
		Trf797xWriteSingle(buf, 2);
		buf[0] = CHIP_STATE_CONTROL;
		buf[1] = 0x01;
		Trf797xWriteSingle(buf, 2);
                
		buf[0] = ISO_CONTROL;
		buf[1] = 0x21;			//set NFC, passive, target
		Trf797xWriteSingle(buf, 2);
		buf[0] = CHIP_STATE_CONTROL;
		buf[1] = nfc_iso;
		Trf797xWriteSingle(buf, 2);

	        buf[0] = RX_SPECIAL_SETTINGS;
	        buf[1] = 0x1C;
	        Trf797xWriteSingle(&buf[0], 2);
            //    } //  if ( NFCIso == 0x21)
                
		buf[0] = NFC_TARGET_LEVEL;
		buf[1] = NFC_TARGET_LEVEL;
	        buf[2] = NFC_LOW_DETECTION;
	        buf[3] = BIT0;//0x01;
		Trf797xReadSingle(&buf[1], 1);		//read the NFCTargetLevel register
		buf[1] |= BIT2 + BIT1 + BIT0;	
                if ( nfc_iso == 0x21) buf[1] |= BIT5; //set automatic SDD algorithm
                else buf[1] &= ~BIT5;
		Trf797xWriteSingle(buf, 4);

	for(i = 1; i < 11; i++)
	{
		target_cid[i] = *(pbuf + 2 + i);
		if(i < 5)
		{
			pupi[i - 1] = *(pbuf + 2 + i);
		}
	}
	*(pbuf + 2) = NFCID;
	target_cid [0] = NFCID | BIT5; 	//NFCID register continous mode
	//RAWwrite(&address, 1, 1, 0);	//write the UID to the NFCID register
	Trf797xRawWrite(target_cid, 11);
	                
        
                
		buf[0] = ISO_14443B_OPTIONS;	
		buf[1] = ISO_14443B_OPTIONS;
		Trf797xReadSingle(&buf[1], 1);		
		//buf[1] |= BIT0;			//set 14443A - 4 compliant bit
		Trf797xWriteSingle(buf, 2);

		//WriteCont(pbuf + 2, 5);	//write the NFC CID to the NFCID register (0x17)

#if DBG_NFC
		buf[0] = TEST_SETTINGS_1;		//subcarrier output
		buf[1] = 0x40;
		Trf797xWriteSingle(buf, 2);

		buf[0] = MODULATOR_CONTROL;	//analog output
       	buf[1] = MODULATOR_CONTROL;
		Trf797xReadSingle(&buf[1], 1);
		buf[1] |= BIT3;
		Trf797xWriteSingle(buf, 2);
#endif
		buf[0] = RX_SPECIAL_SETTINGS;	
       	buf[1] = 0x1C;
		Trf797xWriteSingle(buf, 2);

		buf[0] = IRQ_STATUS;	//IRQ status register address
		Trf797xReadSingle(buf, 1);	//function call for single address read
		Trf797xReset();
		Trf797xStopDecoders();
		Trf797xRunDecoders();

		IRQ_CLR;
		IRQ_ON;
		i_reg = 0x01;
		nfc_state = 1;
		//rxdataPointer = 0;
		//rx_len = 0;
		rx_done = 0;
		while(i_reg == 0x01)		//wait for IRQ
		{
			if (rx_done)
			{	
				break;
			}
		}

		//if(nfc_protocol != 0x00)
		reader_mode = 0xFF;	//NFC operation

		if(i_reg == 0xFF)	//on error skip
		{
			buf[50] = NFC_TARGET_LEVEL;
			buf[51] = NFC_TARGET_LEVEL;
			Trf797xReadSingle(&buf[51], 1);		//read the NFCTargetLevel register
			buf[51] &= ~BIT5;			//reset automatic SDD algorithm
			Trf797xWriteSingle(&buf[50], 2);
			
			buf[50] = ISO_CONTROL;
			Trf797xReadSingle(&buf[50], 1);
			if((buf[50] & BIT2) == BIT2)
			{
				tag = 0x01;
			}
			
		  	switch(nfc_protocol)
		  	{
			  	case 0x01:
			  		if(tag == 0x01)
			  		{
			  			break;
			  		}
					TargetActive(0x00);
					break;

				case 0x07:
					break;

				case 0x02:
					if(tag == 0x01)
			  		{
			  			break;
			  		}
					TargetActive(0xFF);
					break;

				case 0x03:
					if(tag == 0x01)
			  		{
			  			break;
			  		}
					do{
			  			TargetPassive();
						i_reg = 0x01;
						nfc_state = 1;
						while(i_reg == 0x01)		//wait for IRQ
						{
						}
					}while(buf[1] == 0x06);
					TargetActive(0xFF);
			  		break;

				case 0x04:
			  		if(tag == 0x01)
			  		{
			  			break;
			  		}
					TargetActive(0xFF);
			  		break;

			    case 0x05:
			    	if(tag == 0x01)
			  		{
			  			break;
			  		}
					do{
			  			TargetPassive();
						i_reg = 0x01;
						nfc_state = 1;
						while(i_reg == 0x01)		//wait for IRQ
						{
						}
					}while(buf[1] == 0x06);
			       	 	TargetActive(0xFF);
			  	break;

			     	case 0x06: if(tag)
			     	{
			     		break;
			     	}
					TargetActive(0xFF);
			  	break;
			}//switch
		}//if
		if (rx_done)
		{
			break;
		}
	}//while(1)
}//TargetAnticollision
