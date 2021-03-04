#include "nfc.h"
#include "initiator.h"
#include "target.h"

// KOSTA - turn off echo for file transfer
// KOSTA - check file transfer timing. Why 50ms for 200 bits?
// KOSTA - removing baud settings does not turn the circuit to auto
u08_t active;
u08_t nfc_iso;
u08_t tx_buf[23];

extern s16_t nfc_state;
extern u08_t i_reg;
extern u08_t irq_flag;
extern u08_t reader_mode;

extern u08_t buf[300];
extern s08_t rxtx_state;

#define DBG_NFC	0
//----------------------------------------------------------------------
//Performs the initial RF collision avoidance. The modulation
//has to be OOK at start and after it completes it has to
//switch to 10-30% AM modulation.
//----------------------------------------------------------------------
u08_t
NfcRfCollisionAvoidance(u08_t mode)
{
	u08_t command = 0x00, Register = 0xFF, i = 0x00;

	i_reg = 0x01;

	Trf797xResetIrqStatus();	//IRQ status register has to be read
	
	while((Register & 0xC0) != 0x00)	//Check external RF field.
	{
		if(i == 0x10) 		//alow the loop only 15 times	
		{	
			return 0;
		}
		Register = NFC_TARGET_PROTOCOL;   //The register 0x19 bit7 and bit6
		Trf797xReadSingle(&Register, 1);	//If it is below the LOW level
		i++;				//transmition can begin.
	}//while
	
	switch(mode)
	{
		case 0x00:
			command = INITIAL_RF_COLLISION;
	   		break;
		case 0x01:
			command = RESPONSE_RF_COLLISION_N;
			break;
		case 0x02:
			command = RESPONSE_RF_COLLISION_0;
	   		break;
	}//switch
	
	Trf797xDirectCommand(&command);	//performs RF collision avoidance
	while(i_reg == 0x01)		//wait for completition of RF collision avoidance
	{
		McuCounterSet();		//TimerA set
		COUNT_VALUE = 0xF000;	//74ms
		START_COUNTER;		//start timer up mode
		irq_flag = 0x00;
		while(irq_flag == 0x00)
		{
		}
/*		if(i_reg != 0x01)
			break;
		CounterSet();		//TimerA set
		CountValue = 0xF000;	//74ms
		startCounter;		//start timer up mode
		LPM0;
		break;
*/
	}

	switch(i_reg)
	{			//If an error occures during RF collision avoidance
		case 0x00:
			break;	//the function terminates.
		case 0x02:	
			return 0;
		default:
			return 1;
	}//switch	
	/*
	if(mode != 0x00){
		CounterSet();		//TimerA set
		CountValue = 0x352;		//1ms
		startCounter;		//start timer up mode
		LPM0;
	}*/
	return 1;
}//RFCollisionAvoidance


//-------------------------------------------------------
//This is the main NFC send function that has to follow
//the RFAnticollisionAvoidance function - in active mode.
//-------------------------------------------------------
u08_t
NfcSend(u08_t *pbuf)
{
	u08_t length, k = 0;
	
	IRQ_OFF;
	Trf797xReset();

	nfc_state = *pbuf;	//nfc_state global variable is the main transmit counter
	length = *pbuf;	
	if(length > FIFO_LEN)
	{	
		length = FIFO_LEN;
	}

	*(pbuf - 5) = 0x8f;
	*(pbuf - 4) = 0x91;		//buffer setup for FIFO writing
	*(pbuf - 3) = 0x3d;
	*(pbuf - 2) = (char)(nfc_state >> 4);
	*(pbuf - 1) = (char)(nfc_state << 4);
	//RAWwrite(command, 5, 1, 0);	//send the request using RAW writing
	Trf797xRawWrite((pbuf - 5), (length + 5));
	IRQ_ON;
/*
	*(pbuf - 1) = 0x1F;
	WriteCont(pbuf - 1, length + 1);	//send the request using RAW writing
					//Write FIFO_LEN bytes the first time you write to FIFO	*/

	nfc_state = nfc_state - FIFO_LEN;
	i_reg = 0x01;
	pbuf += length;
	
	irq_flag = 0x00;
	
	while(nfc_state > 0)
	{
		McuCounterSet();		//TimerA set
		COUNT_VALUE = 0xf000;	//74ms
		START_COUNTER;		//start timer up mode
		while(irq_flag == 0x00)
		{
		}
		if(irq_flag == 0x03)	// Timer Event
		{
			buf[0] = 0x00;
			buf[1] = 0x01;
			Trf797xWriteSingle(buf, 2);
			Trf797xResetIrqStatus();
			return 0;
		}
		
		if(nfc_state > FIFO_MORE)		//the number of unsent bytes is in the NFCstate global	
		{
			length = FIFO_MORE + 1;		//count variable has to be 10 : 9 bytes for FIFO and 1 address
		}
		else if(nfc_state < 1)
		{	
			break;			//return from interrupt if all bytes have been sent to FIFO
		}
		else
		{
			length = nfc_state + 1;	//all data has been sent out
		}//if
		*(pbuf - 1) = FIFO;		//writes FIFO_MORE or less bytes to FIFO for transmitting
		Trf797xWriteCont(pbuf - 1, length);
		nfc_state = nfc_state - FIFO_MORE;//write FIFO_MORE bytes to FIFO
		pbuf += FIFO_MORE;
	}//while
	i_reg = 0x01;
	while(i_reg != 0x00 && k < 2){		//end of TX recieved
			k++;
			McuCounterSet();		//TimerA set
			COUNT_VALUE = 0xf000;	//74ms
			START_COUNTER;		//start timer up mode
			irq_flag = 0x00;
			while(irq_flag == 0x00)
			{
			}	
	}

	Trf797xResetIrqStatus();
	return 1;
}//NFCSend


u08_t 
NfcFifoSend(u08_t length, u08_t broken_bits, u08_t no_tx_crc, u08_t delayed)
{
#if DBG
	u08_t irq_mask[2];
#endif
	u08_t code = 0;

	IRQ_OFF;
#if DBG
        irq_mask[0] = IRQ_MASK;
        irq_mask[1] = IRQ_MASK;
        Trf797xReadSingle(&irq_mask[1], 1);
        irq_mask[1] &= ~BIT2;
        Trf797xWriteSingle(irq_mask, 2);
#endif
	i_reg = 0x01;
	irq_flag = 0x00;
	rxtx_state = 12;	//RXTXstate global wariable is the main transmit counter
	
	tx_buf[0] = 0x8F;
	
    if(no_tx_crc)
    {
		tx_buf[1] = 0x90;	//buffer setup for FIFO writing
    }
	else
	{
		tx_buf[1] = 0x91;	//buffer setup for FIFO writing
	}
	
	if(delayed)
	{
		tx_buf[1] |= BIT1;	//delayed transmit command
	}
	
	tx_buf[2] = 0x3D;
	tx_buf[3] = 0x00;
	tx_buf[4] = 0xC0 | broken_bits;
        //if(lenght > FIFO_LEN)	
		//lenght = FIFO_LEN;
	/*if(length == 0x00 && broken_bits != 0x00)
	{
		length = 1;
		rxtx_state = 1;
	}*/
	Trf797xRawWrite(tx_buf, (length + 5));		//send the command and data to TRF796x
	//Trf797xRawWrite(pbuf, lenght, 0, 1);		//send the request using RAW writing
						//Write FIFO_LEN bytes the first time you write to FIFO	
	IRQ_ON;

	/*RXTXstate = RXTXstate - FIFO_LEN;
	index = FIFO_LEN;

	command[0] = 0x3F;			//FIFO continous
	while(RXTXstate > 0){
          	CounterSet();				//TimerA set
		CountValue = 0x4E20;			//20ms
		startCounter;				//start timer up mode
		LPM0;
                if(TimerEvent){
                  	code = 1;
                  	goto EXIT;
                }
		if(RXTXstate > FIFO_MORE){		//the number of unsent bytes is in the RXTXstate global	
			lenght = FIFO_MORE;		//count variable has to be 10 : 9 bytes for FIFO and 1 address
		}else if(RXTXstate < 1){	
			break;			//return from interrupt if all bytes have been sent to FIFO
		}else{
			lenght = RXTXstate;	//all data has been sent out
		}//if
		RAWwrite(command, 1, 1, 0);	//write in FIFO address
		RAWwrite((pbuf + index), lenght, 0, 1);
		RXTXstate = RXTXstate - FIFO_MORE;	//write FIFO_MORE bytes to FIFO
		index = index + FIFO_MORE;
	}//while*/
	
	nfc_state = 1;							// the response will be stored in buf[1] upwards
	
	/*if((tx_buf[3] == 0x00) && ((tx_buf[3] >> 4) > 4))
	{
		irq_flag = 0x00;		
        McuCounterSet();
		COUNT_VALUE = COUNT_60ms;		//60ms for TIMEOUT
		START_COUNTER;			//start timer up mode
       	while(irq_flag == 0x00)
		{
		}
	}*/
	
	irq_flag = 0x00;	
	McuCounterSet();
	COUNT_VALUE = COUNT_60ms;	//60ms for TIMEOUT
	START_COUNTER;				//start timer up mode
	while(i_reg == 0x01)		//wait for end of TX interrupt or timeout
	{
	}

        if(irq_flag == 0x03)
        {
        	code = 1;
        }

//EXIT:

#if DBG
	irq_mask[0] = IRQ_MASK;
	irq_mask[1] = IRQ_MASK;
	Trf797xReadSingle(&irq_mask[1], 1);
	irq_mask[1] |= BIT2;
	Trf797xWriteSingle(irq_mask, 2);
#endif
	return(code);
}						//NfcFifoSend


/*
//-------------------------------------------------------
//This is the main NFC send function that has to follow
//the RFAnticollisionAvoidance function - in active mode.
//-------------------------------------------------------
char NfcSendLong(unsigned char *pbuf)
{
	unsigned char length, k = 0, command[5];
	
	IRQ_OFF;
	Trf797xReset();

	nfc_state = *pbuf;	//NFCstate global variable is the main transmit counter
	length = *pbuf;	
	if(length > FIFO_LEN)	
		length = FIFO_LEN;
//kputchar('S');
//Put_byte(length);
//kputchar('S');
	
	*(pbuf - 5) = 0x8f;
	*(pbuf - 4) = 0x91;		//buffer setup for FIFO writing
	*(pbuf - 3) = 0x3d;
	*(pbuf - 2) = (char)(NFCstate >> 4);
	*(pbuf - 1) = (char)(NFCstate << 4);
	//RAWwrite(command, 5, 1, 0);	//send the request using RAW writing
	Trf797xRAWwrite((pbuf - 5), (length + 5));
	IRQ_ON;
/ *
	*(pbuf - 1) = 0x1F;
	WriteCont(pbuf - 1, length + 1);	//send the request using RAW writing
					//Write FIFO_LEN bytes the first time you write to FIFO	* /

	nfc_state = nfc_state - FIFO_LEN;
	i_reg = 0x01;
////////////////	pbuf += length;
	
	irq_flag = 0x00;
	
	while(nfc_state > 0)
	{
		McuCounterSet();		//TimerA set
		COUNT_VALUE = 0xf000;	//74ms
		START_COUNTER;		//start timer up mode
		while(irq_flag == 0x00)
		{
		}
		if(irq_flag == 0x03)	// Timer Event
		{
			buf[0] = 0x00;
			buf[1] = 0x01;
			WriteSingle(buf, 2);
			Trf797xReset();
			return 0;
		}
		
		if(nfc_state > FIFO_MORE)		//the number of unsent bytes is in the NFCstate global	
		{
			length = FIFO_MORE + 1;		//count variable has to be 10 : 9 bytes for FIFO and 1 address
		}
		else if(nfc_state < 1)
		{	
			break;			//return from interrupt if all bytes have been sent to FIFO
		}
		else
		{
			length = nfc_state + 1;	//all data has been sent out
		}//if
		*(pbuf - 1) = FIFO;		//writes FIFO_MORE or less bytes to FIFO for transmitting
		Trf797xWriteCont(pbuf - 1, length);
		nfc_state = nfc_state - FIFO_MORE;//write FIFO_MORE bytes to FIFO
	/////////////////////////////////////////	pbuf += FIFO_MORE;
	}//while
	i_reg = 0x01;
	while(i_reg != 0x00 && k < 2)		//end of TX recieved
	{
			k++;
			McuCounterSet();		//TimerA set
			COUNT_VALUE = 0xf000;	//74ms
			START_COUNTER;		//start timer up mode
			irq_flag = 0x00;
			while(irq_flag == 0x00)
			{
			}	
	}

	Trf797xReset();
//kputchar('=');
//kputchar('=');
	return 1;
}//NFCSend */






//----------------------------------------------//
//The NFCActiveMain function calls the other NFC//
//functions for Felica (active) mode.		//
//----------------------------------------------//
u08_t
NfcMain(u08_t *pbuf, u08_t tag)
{
	u08_t i = 0, command[2];
	u08_t code = 0;

//#if DBG_NFC
//	command[0] = TEST_SETTINGS_1;	//subcarrier output
//	command[1] = 0x40;
//	Trf797xWriteSingle(command, 2);
//
//	command[0] = MODULATOR_CONTROL;	//analog output
//    command[1] = MODULATOR_CONTROL;
//	Trf797xReadSingle(&command[1], 1);
//	command[1] |= BIT3;
//	Trf797xWriteSingle(command, 2);
//#endif
	reader_mode = 0xFF;

	//rxdataPointer = 0;
	//RXlen = 0;

	Trf797xResetIrqStatus();	//function call for single address read
	IRQ_ON;
/*
        if ((*(pbuf + 2) == 0x00) || (*(pbuf + 2) == 0x02) || (*(pbuf + 2) == 0x04) || (*(pbuf + 2) == 0x06) || (*(pbuf + 2) == 0x08) || (*(pbuf + 2) == 0x0A) || (*(pbuf + 2) == 0x16))
        {
          command[0] = ISOControl;
	  command[1] = NFCIso;
	  WriteSingle(command, 2);
        }
*/
	switch(*(pbuf + 2))
	{
		case 0x00:	//ATR_REQ
			//send_cstring("ATR\r\n");
			do
			{
				i++;
				if(i > 3)
				{
					code = 1;
					goto EXIT;
				}
				InitiatorRequest(pbuf);
			}while((i_reg != 0xFF) && (buf[1] != 0xD5) && (buf[2] != 0x01));
			break;
			
		case 0x02:	//WUP_REQ
			//send_cstring("WUP\r\n");
			do
			{
				i++;
				if(i > 3)
				{
					code = 1;
					goto EXIT;
				}
				InitiatorRequest(pbuf);
			}while((i_reg != 0xFF) && (buf[1] != 0xD5) && (buf[2] != 0x03));
			break;
			
		case 0x04:	//PSL_REQ
			//send_cstring("PSL\r\n");
			do
			{
				i++;
				if(i > 3)
				{
					code = 1;
					goto EXIT;
				}
				InitiatorRequest(pbuf);
			}while((i_reg != 0xFF) && (buf[1] != 0xD5) && (buf[2] != 0x05));
			break;
		
		case 0x06:	//DEP_REQ
			//send_cstring("DEP\r\n");
			while(!InitiatorDepRequest(pbuf))
			{
				i++;
				if(i > 1)
				{
					code = 1;
					goto EXIT;
				}
			}//while
			break;
			
		case 0x08:	//DSL_REQ
			//send_cstring("DSL\r\n");
			do
			{
				i++;
				if(i > 3)
				{
					code = 1;
					goto EXIT;
				}
				InitiatorRequest(pbuf);
			}while((i_reg != 0xFF) && (buf[1] != 0xD5) && (buf[2] != 0x09));
			break;
			
		case 0x0A:	//RLS_REQ
			//send_cstring("RLS\r\n");
			do
			{
				i++;
				if(i > 3)
				{
					code = 1;
					goto EXIT;
				}
				InitiatorRequest(pbuf);
			}while((i_reg != 0xFF) && (buf[1] != 0xD5) && (buf[1] != 0x0B));
			break;
			
		case 0x0C:	//Select target mode, CID is recieved with the command
			//send_cstring("target\r\n");
			nfc_iso = ISO_CONTROL;	
			Trf797xReadSingle(&nfc_iso, 1);
			reader_mode = 0xF0;	//set for target iterrupt handling
			TargetAnticollision(pbuf, tag);
			break;
			
		case 0x16:	//Data excahange
//kputchar('v');				
//kputchar('v');				
			do
			{
				i++;
				if(i > 1)
				{
					code = 1;
					goto EXIT;
				}
//kputchar('x');				
//kputchar('x');				
				InitiatorRequest(pbuf);
//kputchar('w');
//Put_byte(NFCstate);
//kputchar('w');
//Put_byte(i_reg);
//Put_byte(buf[1]);
//Put_byte(buf[2]);
//kputchar('w');				
			}while((i_reg != 0xFF) && (buf[1] != 0xD5) && (buf[2] != 0x09));
//kputchar('z');				
//kputchar('z');				
			break;
			
		case 0xF0:	//select initiator for passive operation
		  	active = 0x00;
                        //NFCIso = ISOControl;	
	                //ReadSingle(&NFCIso, 1);
#if DBG_NFC
 			command[0] = TEST_SETTINGS_1;	//subcarrier output
			command[1] = 0x40;
			Trf797xWriteSingle(command, 2);

			command[0] = MODULATOR_CONTROL;	//analog output
       			command[1] = MODULATOR_CONTROL;
			Trf797xReadSingle(&command[1], 1);
			command[1] |= BIT3;
			Trf797xWriteSingle(command, 2);
#endif
			break;

		case 0xF1: 	//select initiator for active operation
		  	active = 0xFF;
                        //NFCIso = ISOControl;	
	                //ReadSingle(&NFCIso, 1);
			//send_cstring("active\r\n");
			command[0] = CHIP_STATE_CONTROL;
			command[1] = 0x01;
			Trf797xWriteSingle(command, 2);
			break;
			
		default:
			break;
			
	}//switch
	IRQ_OFF;
	UartSendCString("[00]");
	return(code);
EXIT:
	IRQ_OFF;
	UartSendCString("[]");
	return(code);
}//NfcMain


