#include "initiator.h"

extern u08_t active;
extern s16_t nfc_state;

extern u08_t buf[300];
extern u08_t irq_flag;
extern u08_t i_reg;

//----------------------------------------------//
//Sends a NFC command and waits for response.	//
//----------------------------------------------//
void
InitiatorRequest(u08_t *pbuf)
{

	u08_t command[2];
	
        IRQ_ON;
	if(active)
	{
		if (! NfcRfCollisionAvoidance(0x00))	
		{
			Trf797xReset();
			return;
		}
	}

	NfcSend(pbuf);//NFCSendLONG(pbuf);
	IRQ_OFF;
	Trf797xReset();
	Trf797xStopDecoders();		// Reset Decoders
	Trf797xRunDecoders();
	command[0] = IRQ_STATUS;		//IRQ status register address
	Trf797xReadSingle(command, 1);	//function call for single address read
        
        
        command[0] = MODULATOR_CONTROL;	//analog output
        command[1] = MODULATOR_CONTROL;
	Trf797xReadSingle(&command[1], 1);
	command[1] |= BIT3;
	Trf797xWriteSingle(command, 2);
        if(active)
        {
            command[0] = CHIP_STATE_CONTROL;	//analog output
            command[1] = CHIP_STATE_CONTROL;
	    	Trf797xReadSingle(&command[1], 1);
	    	command[1] |= BIT1;
	   		Trf797xWriteSingle(command, 2);
        }  
        
	nfc_state = 0;		//save the recieved response after the host bufer
	i_reg = 0x01;
        IRQ_ON;
	while(i_reg == 0x01)
	{		//wait for end of RX or timer IRQ
		McuCounterSet();		//TimerA set
		COUNT_VALUE = 0xCE20;	//20ms 0x4E20;
		START_COUNTER;		//start timer up mode
		irq_flag = 0x00;
		while(irq_flag == 0x00)
		{
		}
	}//while
        IRQ_OFF;
	Trf797xStopDecoders();
}//InitiatorRequest


//---------------------------------------------------
//Sends the data exchange protocol request and
//examines the response for ACK/NAK and PNI.
//---------------------------------------------------
u08_t
InitiatorDepRequest(u08_t *pbuf)
{
	unsigned char length, packet_number, k;

	packet_number = *(pbuf + 3) & 0x03;
	length = *pbuf;

	//perform RFCollision avoidance only for active mode
	if(active)
	{
		NfcRfCollisionAvoidance(0x02);
	}

	if(!NfcSend(pbuf))
	{
		i_reg = 0x02;
		goto FINISH;
	}

	nfc_state = length + 5;		//save the recieved response after the host bufer
	//i_reg = 0x01;
	i_reg = 0x00;
	irq_flag = 0x00;

	//while(i_reg == 0x01 && TimerEvent == 0){
	McuCounterSet();		//TimerA set
	COUNT_VALUE = 0xf000;	//74ms
	START_COUNTER;		//start timer up mode
	while(irq_flag == 0x00)
	{
	}
//	}

	i_reg = 0x01;
	k = 0;
	while(i_reg == 0x01 || i_reg == 0x00)
	{
		McuCounterSet();		//TimerA set
		COUNT_VALUE = 0xf000;	//74ms
		START_COUNTER;		//start timer up mode
		while(irq_flag == 0x00);
		k++;
		if(k > 2)
			break;
	}


FINISH:

	if(i_reg == 0xFF){	     //if response is in buffer
		if(((buf[length + 8] & BIT4) == 0x00) && (packet_number == (buf[length + 8] & 0x03)))
		{
			return(1);      //recieved ACK with correct PNI
		}
		else
		{
			return(0);
		}
	}
	else
	{
		return(0);
	}
}//InitiatorDepRequest
