/*************************70?************************************
* FILENAME: trf797x.c
*
* BRIEF: Contains functions to initialize and execute
* communication the TRF796x via the selected interface.
*
* Copyright (C) 2010 Texas Instruments, Inc.
*
* AUTHOR(S): Reiser Peter		DATE: 02 DEC 2010
*
* EDITED BY:
* *
*
****************************************************************/

#include "trf797x.h"
#include "nfc.h"
//#include "iso14443a.h"		// add by Long

//===============================================================

u08_t	command[2];
extern u08_t	tag_flag;
extern u08_t	buf[300];
extern u08_t	i_reg;
#ifdef ENABLE14443A
	extern u08_t	coll_poss;
#endif
extern u08_t	irq_flag;
extern u08_t	rx_error_flag;
extern s08_t	rxtx_state;
extern u08_t	active;
extern s16_t	nfc_state;
extern u08_t	nfc_protocol;
extern u08_t	stand_alone_flag;
extern u08_t	reader_mode;

//===============================================================

void Trf797xISR(u08_t *irq_status);
void Trf797xNFCmodeISR(u08_t *Register);
void Trf797xTargetIST(u08_t *Register);

//===============================================================
//                                                              ;
//===============================================================

void 
Trf797xCommunicationSetup(void)
{
	if(SPIMODE)								// SPI mode given by jumper setting
	{
		SpiSetup();
	}
	else									// parallel mode
	{
		ParallelSetup();	
	}
}

//===============================================================
// NAME: void Trf797xDirectCommand (u08_t *pbuf)
//
// BRIEF: Is used to transmit a Direct Command to Trf797x.
//
// INPUTS:
//	Parameters:
//		u08_t		*pbuf		Direct Command
//
// OUTPUTS:
//
// PROCESS:	[1] transmit Direct Command
//
// CHANGE:
// DATE  		WHO	DETAIL
// 24Nov2010	RP	Original Code
//===============================================================

void 
Trf797xDirectCommand(u08_t *pbuf)
{
	if(SPIMODE)								// SPI mode given by jumper setting
	{
		SpiDirectCommand(pbuf);
	}
	else									// parallel mode
	{
		ParallelDirectCommand(pbuf);	
	}
}

//===============================================================
// NAME: void Trf797xDirectMode (void)
//
// BRIEF: Is used to start Direct Mode.
//
// INPUTS:
//
// OUTPUTS:
//
// PROCESS:	[1] start Direct Mode
//
// NOTE: No stop condition
//
// CHANGE:
// DATE  		WHO	DETAIL
// 24Nov2010	RP	Original Code
//===============================================================

/*void
Trf797xDirectMode(void)
{
	if(SPIMODE)								// SPI mode given by jumper setting
	{
		SpiDirectMode();
	}
	else									// parallel mode
	{
		ParallelDirectMode();	
	}
}*/

//===============================================================
// 02DEC2010	RP	Original Code
//===============================================================

void 
Trf797xDisableSlotCounter(void)
{
	buf[40] = IRQ_MASK;
	buf[41] = IRQ_MASK;				// next slot counter
	Trf797xReadSingle(&buf[41], 1);
	buf[41] &= 0xFE;				// clear BIT0 in register 0x01
	Trf797xWriteSingle(&buf[40], 2);
}

//===============================================================
// 02DEC2010	RP	Original Code
//===============================================================

void
Trf797xEnableSlotCounter(void)
{
	buf[40] = IRQ_MASK;
	buf[41] = IRQ_MASK;				// next slot counter
	Trf797xReadSingle (&buf[41], 1);
	buf[41] |= BIT0;				// set BIT0 in register 0x01
	Trf797xWriteSingle(&buf[40], 2);
}

//===============================================================
// 02DEC2010	RP	Original Code
//===============================================================

void
Trf797xInitialSettings(void)
{	
	u08_t mod_control[2];

	mod_control[0] = SOFT_INIT;
	Trf797xDirectCommand (mod_control);
	mod_control[0] = IDLE;
	Trf797xDirectCommand (mod_control);

	mod_control[0] = MODULATOR_CONTROL;
	mod_control[1] = 0x21;						// 6.78MHz, OOK 100%

	Trf797xWriteSingle(mod_control, 2);

	mod_control[0] = MODULATOR_CONTROL;
	Trf797xReadSingle(mod_control, 1);
}

//===============================================================
// The Interrupt Service Routine determines how the IRQ should  ;
// be handled. The Trf797x IRQ status register is read to       ;
// determine the cause of the IRQ. Conditions are checked and   ;
// appropriate actions taken.                                   ;
//===============================================================

void
Trf797xISR(u08_t *irq_status)
{	
#ifdef ENABLE14443A
	u08_t	len;	
#endif

#if DBG
	UartPutChar('Q');
	UartPutByte(*irq_status);
#endif
	if(*irq_status == 0xA0)			// BIT5 and BIT7
	{								// TX active and only 3 bytes left in FIFO
		i_reg = 0x00;
#if DBG
		UartPutChar('.');			// FIFO filling is done in the transmit function
#endif
	}

	else if(*irq_status == BIT7)
	{								// TX complete
		i_reg = 0x00;
		Trf797xReset();				// reset the FIFO after TX
#if DBG
		UartPutChar('T');
#endif
	}

	else if((*irq_status & BIT1) == BIT1)
	{								// collision error
		i_reg = 0x02;				// RX complete

#ifdef ENABLE14443A
		// change by Long


		coll_poss = COLLISION_POSITION;
		Trf797xReadSingle(&coll_poss, 1);
		buf[rxtx_state] = FIFO;					// write the recieved bytes to the correct place of the
													// buffer;
		if (coll_poss >= 0x20)
		{
			Trf797xReadCont(&buf[rxtx_state], 5);
		}
#endif
		
		Trf797xStopDecoders();						// reset the FIFO after TX

		Trf797xReset();

		Trf797xResetIrqStatus();

		IRQ_CLR;
	}
	else if(*irq_status == BIT6)
	{	// RX flag means that EOF has been recieved
		// and the number of unread bytes is in FIFOstatus regiter	
		if(rx_error_flag == 0x02)
		{
			i_reg = 0x02;
			return;
		}

		*irq_status = FIFO_CONTROL;
		Trf797xReadSingle(irq_status, 1);					// determine the number of bytes left in FIFO

		*irq_status = 0x7F & *irq_status;
		buf[rxtx_state] = FIFO;						// write the recieved bytes to the correct place of the buffer
				
/*TRF_WRITE = 0x00;
TRF_DIR_OUT;
TRF_WRITE = 0x01;
	_NOP();
	_NOP();
	_NOP();
TRF_WRITE = 0x00;*/

		Trf797xReadCont(&buf[rxtx_state], *irq_status);
		rxtx_state = rxtx_state + *irq_status;
		
		/* *irq_status = ISO_CONTROL;
		Trf797xReadSingle(irq_status, 1);
		*irq_status &= 0x1F;
		if(*irq_status > 0x0B)
		{
			*irq_status = TX_LENGTH_BYTE_2;		// determine if there are broken bytes
			Trf797xReadSingle(irq_status, 1);
	
			if((*irq_status & BIT0) == BIT0)
			{
				*irq_status = (*irq_status >> 1) & 0x07;	// mask the first 5 bits
				*irq_status = 8 - *irq_status;
				buf[rxtx_state - 1] &= 0xFF << *irq_status;
			}								// if
		}*/

#if DBG
		UartPutChar('E');
#endif
		Trf797xReset();					// reset the FIFO after last byte has been read out

		i_reg = 0xFF;					// signal to the recieve funnction that this are the last bytes
	}	
	else if(*irq_status == 0x60)
	{									// RX active and 9 bytes allready in FIFO
		i_reg = 0x01;
		buf[rxtx_state] = FIFO;
		Trf797xReadCont(&buf[rxtx_state], 86);	// read 86 bytes from FIFO
		rxtx_state = rxtx_state + 86;
#if DBG
		UartPutChar('F');
#endif
		if((IRQ_PORT & IRQ_PIN) == IRQ_PIN)			// if IRQ pin high
		{
			Trf797xReadIrqStatus(irq_status);
				
			IRQ_CLR;

			if(*irq_status == 0x40)							// end of recieve
			{	
				*irq_status = FIFO_CONTROL;
				Trf797xReadSingle(irq_status, 1);					// determine the number of bytes left in FIFO

				*irq_status = 0x7F & *irq_status;
				buf[rxtx_state] = FIFO;						// write the recieved bytes to the correct place of the buffer
				
/*TRF_WRITE = 0x00;
TRF_DIR_OUT;
TRF_WRITE = 0x01;
	_NOP();
	_NOP();
	_NOP();
TRF_WRITE = 0x00;*/
				
				Trf797xReadCont(&buf[rxtx_state], *irq_status);
				rxtx_state = rxtx_state + *irq_status;

				/* *irq_status = ISO_CONTROL;
				Trf797xReadSingle(irq_status, 1);
				*irq_status &= 0x1F;
				if(*irq_status > 0x0B)
				{
					*irq_status = TX_LENGTH_BYTE_2;				// determine if there are broken bytes
					Trf797xReadSingle(irq_status, 1);				// determine the number of bits
	
					if((*irq_status & BIT0) == BIT0)
					{
						*irq_status = (*irq_status >> 1) & 0x07;	// mask the first 5 bits
						*irq_status = 8 - *irq_status;
						buf[rxtx_state - 1] &= 0xFF << *irq_status;
					}											// if
				}*/

#if DBG
				UartPutChar('E');
#endif
				i_reg = 0xFF;			// signal to the recieve funnction that this are the last bytes
				Trf797xReset();		// reset the FIFO after last byte has been read out
			}
			else if(*irq_status == 0x50)	// end of recieve and error
			{
				i_reg = 0x02;
#if DBG
				UartPutChar('x');
#endif
			}
		}
		else
		{
			Trf797xReadIrqStatus(irq_status);
                                 
			if(irq_status[0] == 0x00)
			{	
				i_reg = 0xFF;
			}
		}
	}
	else if((*irq_status & BIT4) == BIT4)				// CRC error
	{
		if((*irq_status & BIT5) == BIT5)
		{	
			i_reg = 0x01;	// RX active
			rx_error_flag = 0x02;
		}
		if((*irq_status & BIT6) == BIT6)			// 4 Bit receive
		{
			buf[200] = FIFO;						// write the recieved bytes to the correct place of the buffer
		
			Trf797xReadCont(&buf[200], 1);

			Trf797xReset();
		
			i_reg = 0x02;	// end of RX
			rx_error_flag = 0x02;
		}
		else
		{
			i_reg = 0x02;	// end of RX
		}
	}
	else if((*irq_status & BIT2) == BIT2)	// byte framing error
	{
		if((*irq_status & BIT5) == BIT5)
		{
			i_reg = 0x01;	// RX active
			rx_error_flag = 0x02;
		}
		else
			i_reg = 0x02;	// end of RX
	}
	else if((*irq_status == BIT0))
	{						// No response interrupt
		i_reg = 0x00;
#if DBG
		UartPutChar('N');
#endif
	}
	else
	{						// Interrupt register not properly set
		if(stand_alone_flag == 0)
		{
			#ifdef ENABLE_HOST
				UartSendCString("Interrupt error");
				UartPutByte(*irq_status);
			#endif
		}

		i_reg = 0x02;

		Trf797xStopDecoders();	// reset the FIFO after TX

		Trf797xReset();

		Trf797xResetIrqStatus();
		
		IRQ_CLR;
	}
}							// Interrupt Service Routine

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void
Trf797xNFCmodeISR(u08_t *Register)
{
	//bit0 - RF collision avoidance error
	//bit1 - RF collision avoidance OK
	//bit2 - Change in RF field level
	//bit3 - SDD OK
	//bit4 - Communication error
	//bit5 - FIFO high/low
	//bit6 - RX
	//bit7 - TX
	//--------------------------------
	//i_reg is used for signaling to the protocol functions
	//i_reg = 0x00 - OK
	//i_reg = 0x01 - progress
	//i_reg = 0x02 - error
	//i_reg = 0xFF - end of RX
#if DBG
  	UartPutCrlf();
  	UartPutChar('C');
	UartPutByte(*Register);
#endif

	//-------------------------------------------------------------
	if(*Register == BIT0)
	{
		i_reg = 0x02;
	}
	//-------------------------------------------------------------
	else if(*Register == BIT1)
	{
		i_reg = 0x00;
	}
	//-------------------------------------------------------------
	/*else if(*Register == BIT2)
	{	//RF field change
#if DBG
		UartPutChar('R');
#endif
#if !PICCtest
		if(tag_flag && 0)
		{
			*Register = NFC_TARGET_PROTOCOL;
			Trf797xReadSingle(Register, 1);
			if((*Register & 0x80) == 0x00){
				/ *
				kputchar(0x00);
				kputchar(0x01);
				kputchar(0xFF);	
				kputchar(0xFE);
				for(Register = 0; Register < 200; Register++){}
				FCTL1 = 0x33;		//master reset when tag moves out of field
				///
				i_reg = 0x03;
///#if 0
				seClkStop();	//RESET the smart card
				seInit();	//initialize secure element
				seStart();	//
#endif* /
			}//if
		}//if
#endif
	}*/
	//-------------------------------------------------------------
	else if(*Register == BIT3){
		i_reg = 0x00;
	}
	//-------------------------------------------------------------
	else if((*Register & BIT4) == BIT4){
//////////////////////////////////////////   
		*Register = RX_SPECIAL_SETTINGS;		//check filter and gain
		Trf797xReadSingle(Register, 1);
		//Stop decoders:
	    Trf797xStopDecoders();
		i_reg = 0x02;
	}
	//-------------------------------------------------------------
	else if(*Register == BIT7){	//TX complete
		i_reg = 0x00;
		Trf797xReset();
#if DBG
		UartPutChar('T');
#endif
	}
	//-------------------------------------------------------------
	/*else if(*Register == 0xA0){	//TX active and only 3 bytes left in FIFO
		i_reg = 0x00;
#if DBG
		UartPutChar('.');
#endif
	}*/
	//-------------------------------------------------------------
	else if(*Register == 0x40 || *Register == 0x44){	//RX flag means that EOF has been recieved
							//and the number of unread bytes is in FIFOstatus regiter          	
		
//		Register = TXLenghtByte2;		//determine if there are broken bytes
//		ReadSingle(&Register, 1);	
//////////////////////////////////////////   
		*Register = RX_SPECIAL_SETTINGS;		//check filter and gain
		Trf797xReadSingle(Register, 1);
		
		*Register = FIFO_CONTROL;
		Trf797xReadSingle(Register, 1);		//determine the number of bytes left in FIFO
		*Register = 0x7F & *Register;
		
		buf[nfc_state] = FIFO;			//write the recieved bytes to the correct place of the buffer;
		Trf797xReadCont(&buf[nfc_state], *Register);
/*#if 0
UartPutChar('E');
UartPutByte(NFCstate);
UartPutChar('E');
UartPutByte(Register);
UartPutChar('E');
#endif*/
		nfc_state = nfc_state + *Register;
#if DBG
		UartPutChar('E');
#endif
		Trf797xReset();			//reset the FIFO after last byte has been read out

		i_reg = 0xFF;				//signal to the recieve function that this are the last bytes
		Trf797xResetIrqStatus();	
	}
	//------------------------------------------------------------------
	else if((*Register == 0x60) || (*Register == 0x64))	//RX & FIFO interrupt
	{
		i_reg = 0x01;
		//*Register = FIFO_CONTROL;
		/*Trf797xReadSingle(Register, 1);
		Register = Register & 0x7F;
		//do{
		buf[nfc_state] = FIFO;
		Trf797xReadCont(&buf[NFCstate], FIFO_MORE);	//read FIFO_MORE bytes from FIFO
//kputchar('M');
//Put_byte(NFCstate);
//kputchar('M');
//Put_byte(Register);
//kputchar('M');
		nfc_state = nfc_state + FIFO_MORE;
//////
			Register = FIFOStatus;
			ReadSingle(&Register, 1);
			Register = Register & 0x7F;
			if((Register == 0x10) && (P1IN & BIT5)){
				Register = IRQStatus;
				ReadSingle(&Register, 1);
				if(Register == 0x40){
					i_reg = 0xFF;
				}else{
					i_reg = 0x02;
				}
				//break;
			}
		}while(Register & 0x0F > 0x03);*/
//////////////
#if DBG
		UartPutChar('F');
#endif		
        }
        //-----------------------------------------------------------------
        else if(*Register == 0x48){	//SDD complete
		i_reg = 0x00;
#if DBG
		UartPutChar('S');
#endif
	}
	//-----------------------------------------------------------------

}			//Trf797xNFCmodeISR()

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void Trf797xTargetISR(u08_t *Register)
{
  	unsigned char command, i;
// send_cstring("++>\r\n");
#if DBG
	UartPutCrlf();
	UartPutByte(*Register);
#endif
//kputchar('T');
//kputchar('T');
//Put_byte(NFCstate);
//kputchar(',');
//Put_byte(Register);
//kputchar('T');
	if(*Register == 0x40)
	{
		TargetProtocol:
		//determine the NFC target protocol
		command = NFC_TARGET_PROTOCOL;
		Trf797xReadSingle(&command, 1);	//read the NFCtargetprotocol register
#if DBG
		UartPutCrlf();
		UartPutChar('<');
		UartPutByte(command);
		UartPutChar('>');
#endif
		command &= 0x3F;
		buf[50] = ISO_CONTROL;
		if(command == 0x09)
		{
			nfc_protocol = 0x01;
			
			*Register = FIFO_CONTROL;
			Trf797xReadSingle(Register, 1);	//determine the number of bytes left in FIFO
			*Register = 0x7F & *Register;
			buf[nfc_state] = FIFO;	//write the recieved bytes to the correct place of the buffer;
//kputchar('*');
//Put_byte(NFCstate);
//kputchar('*');
//Put_byte(Register);
//kputchar('*');
			Trf797xReadCont(&buf[nfc_state], *Register);
			nfc_state = nfc_state + *Register;
			i_reg = 0xFF;
			
#if DBG
			UartSendCString("14443A - passive.\r\n");
			command = TX_TIMER_EPC_LOW;
			Trf797xReadSingle(&command, 1);
			UartPutCrlf();
			UartPutChar('|');
			UartPutByte(command);
			UartPutChar('|');
			Trf797xResetIrqStatus();
			UartPutByte(command);
			UartPutChar('|');
#endif
		}
		else
		{
			*Register = FIFO_CONTROL;
			Trf797xReadSingle(Register, 1);	//determine the number of bytes left in FIFO
			*Register = 0x7F & *Register;
			buf[nfc_state] = FIFO;	//write the recieved bytes to the correct place of the buffer;
//kputchar('*');
//Put_byte(NFCstate);
//kputchar('*');
//Put_byte(Register);
//kputchar('*');
			Trf797xReadCont(&buf[nfc_state], *Register);
			nfc_state = nfc_state + *Register;
			i_reg = 0xFF;

			if((command == 0x01))	// || (command == 0x00)){	//14443A - active
			{
				buf[51] = NFC106AC;
				Trf797xWriteSingle(&buf[50], 2);
#if DBG
				UartSendCString("14443A - active.\r\n");
#endif
				nfc_protocol = 0x02;
				active = 0xFF;
			}
			else if(command == 0x05)
			{	//14443B - tag emulation
				buf[51] = TAG14443B;
				Trf797xWriteSingle(&buf[50], 2);
#if DBG
				UartSendCString("14443B - tag emulation.\r\n");
#endif
				nfc_protocol = 0x07;
				active = 0x00;
			}
				else if(command == 0x12)
				{
					if(buf[1] == 0x06)	//Felica 212 - passive
					{
						buf[51] = NFC212PA;
						Trf797xWriteSingle(&buf[50], 2);
#if DBG
						UartSendCString("Felica 212 - passive.\r\n");
#endif
						nfc_protocol = 0x03;
						active = 0x00;
  					}
  					else			//Felica 212 - active
					{
						buf[51] = NFC212AC;
						Trf797xWriteSingle(&buf[50], 2);
#if DBG
						UartSendCString("Felica 212 - active.\r\n");
#endif
						nfc_protocol = 0x04;
  						active = 0xFF;
					}//if-else
				}
				else if(command == 0x13)
				{
					if(buf[1] == 0x06)	//Felica 424 - passive
					{
						buf[51] = NFC424PA;
						Trf797xWriteSingle(&buf[50], 2);
#if DBG
						UartSendCString("Felica 424 - passive.\r\n");
#endif
						nfc_protocol = 0x05;
						active = 0x00;
					}
					else			//Felica 424 - active
					{
						buf[51] = NFC424AC;
						Trf797xWriteSingle(&buf[50], 2);
#if DBG
						UartSendCString("Felica 424 - active.\r\n");
#endif
						nfc_protocol = 0x06;
						active = 0xFF;
					}//if-else
				}
			}//if-else
        }
	//}
	else if(*Register == 0x60)
	{
		i_reg = 0x01;
		buf[nfc_state] = FIFO;
		Trf797xReadCont(&buf[nfc_state], FIFO_MORE);	//read FIFO_MORE bytes from FIFO
//kputchar('+');
//Put_byte(NFCstate);
//kputchar('+');
//Put_byte(Register);
//kputchar('+');
			nfc_state = nfc_state + FIFO_MORE;
			Trf797xResetIrqStatus();
        }
        else if((*Register & BIT3) == BIT3)	//SDD finished
        {
          	i_reg = 0xFF;
			for(i = 0; i < 255; i++);
			nfc_protocol = 0x01;
#if DBG
			UartSendCString("14443A - SDD complete.\r\n");
#endif
			Trf797xReset();
			Trf797xStopDecoders();
			Trf797xRunDecoders();
        }	
        //-------------------------------------------------------------
        else if(*Register == 0x44)
        {
			Trf797xReadIrqStatus(Register);
			if(*Register == 0x40)
			{
				goto TargetProtocol;
			}
			else if(*Register == 0x00)
			{
				goto RFfieldChange;
			}
        }
        //-------------------------------------------------------------
	else if(*Register == BIT2)	//RF field change
	{
		RFfieldChange:
            Trf797xReset();
			Trf797xStopDecoders();
			Trf797xRunDecoders();
#if DBG
		UartPutChar('R');
#endif
/*#if !PICCtest
		if(tag_flag){
			Register = NFCTargetProtocol;
			ReadSingle(&Register, 1);
			if((Register & 0x80) == 0x00){
				/ *
				kputchar(0x00);
				kputchar(0x01);
				kputchar(0xFF);	
				kputchar(0xFE);
				for(Register = 0; Register < 200; Register++){}
				FCTL1 = 0x33;		//master reset when tag moves out of field
				* /
				i_reg = 0x03;
#if 0
				seClkStop();	//RESET the smart card
				seInit();	//initialize secure element
				seStart();	//
#endif
			}
		}
#endif*/
	}//if-else
            Trf797xResetIrqStatus();
}//Trf797xTargetISR

//===============================================================
// 02DEC2010	RP	Original Code
//===============================================================

#pragma vector = PORT2_VECTOR
__interrupt void
Trf797xPortB(void)							// interrupt handler
{
	u08_t irq_status[2], iso_control;

	irq_flag = 0x02;

	STOP_COUNTER;							// stop timer mode
	
	do
	{
		IRQ_CLR;							// PORT2 interrupt flag clear
		
		iso_control = ISO_CONTROL;
		Trf797xReadSingle(&iso_control, 1);
		
		Trf797xReadIrqStatus(irq_status);

		// IRQ status register has to be read

		if(*irq_status == 0xA0)				// TX active and only 3 bytes left in FIFO
		{
			break;
		}
		
		if((iso_control & BIT5) != BIT5)			// RFID mode
		{
			Trf797xISR(irq_status);
		}
		else										// NFC mode
		{
			if((iso_control & BIT2) == BIT2)		// tag emulation
			{
				tag_flag = 1;
				Trf797xNFCmodeISR(irq_status);
			}
			else
			{
				if((iso_control & BIT4) == BIT4)	// active mode
				{
					Trf797xNFCmodeISR(irq_status);
				}
				else								// passive mode
				{
					Trf797xTargetISR(irq_status);
				}
			}
		}
		/*switch(reader_mode)
		{
			case 0x00:	
				Trf797xISR(irq_status);				// RFID mode
				break;
	        case 0xF0:
	        	Trf797xTargetISR(irq_status);		// NFC Target mode
	        	break;
			case 0xFF:
				Trf797xNFCmodeISR(irq_status);		// NFC mode
				break;
			default:
				break;
		}*/
		
	} while((IRQ_PORT & IRQ_PIN) == IRQ_PIN);
	//__low_power_mode_off_on_exit();
}

//===============================================================
// NAME: void Trf797xRawWrite (u08_t *pbuf, u08_t length)
//
// BRIEF: Is used to write direct to the Trf797x.
//
// INPUTS:
//	Parameters:
//		u08_t		*pbuf		raw data
//		u08_t		length		number of data bytes
//
// OUTPUTS:
//
// PROCESS:	[1] send raw data to Trf797x
//
// CHANGE:
// DATE  		WHO	DETAIL
// 24Nov2010	RP	Original Code
//===============================================================

void 
Trf797xRawWrite(u08_t *pbuf, u08_t length)
{
	if(SPIMODE)								// SPI mode given by jumper setting
	{
		SpiRawWrite(pbuf, length);
	}
	else									// parallel mode
	{
		ParallelRawWrite(pbuf, length);	
	}
}

//===============================================================
// 02DEC2010	RP	Original Code
//===============================================================

void
Trf797xReConfig(void)
{
	if(SPIMODE)
	{	
		SpiUsciExtClkSet();       			// Set the USART
    }
}

//===============================================================
// NAME: void Trf797xReadCont (u08_t *pbuf, u08_t length)
//
// BRIEF: Is used to read a specified number of Trf797x registers
// from a specified address upwards.
//
// INPUTS:
//	Parameters:
//		u08_t		*pbuf		address of first register
//		u08_t		length		number of registers
//
// OUTPUTS:
//
// PROCESS:	[1] read registers
//			[2] write contents to *pbuf
//
// CHANGE:
// DATE  		WHO	DETAIL
// 24Nov2010	RP	Original Code
//===============================================================

void
Trf797xReadCont(u08_t *pbuf, u08_t length)
{
	if(SPIMODE)								// SPI mode given by jumper setting
	{
		SpiReadCont(pbuf, length);
	}
	else									// parallel mode
	{
		ParallelReadCont(pbuf, length);	
	}
}

//===============================================================
// 02DEC2010	RP	Original Code
//===============================================================

void
Trf797xReadIrqStatus(u08_t *pbuf)
{
	*pbuf = IRQ_STATUS;
	*(pbuf + 1) = IRQ_MASK;
	
	if(SPIMODE)								// SPI mode given by jumper setting
	{
		Trf797xReadCont(pbuf, 2);			// read second reg. as dummy read
	}
	else									// parallel mode
	{
		Trf797xReadSingle(pbuf, 1);
	}
}

//===============================================================
// NAME: void Trf797xReadSingle (u08_t *pbuf, u08_t number)
//
// BRIEF: Is used to read specified Trf797x registers.
//
// INPUTS:
//	Parameters:
//		u08_t		*pbuf		addresses of the registers
//		u08_t		number		number of the registers
//
// OUTPUTS:
//
// PROCESS:	[1] read registers
//			[2] write contents to *pbuf
//
// CHANGE:
// DATE  		WHO	DETAIL
// 24Nov2010	RP	Original Code
//===============================================================

void
Trf797xReadSingle(u08_t *pbuf, u08_t number)
{
	if(SPIMODE)								// SPI mode given by jumper setting
	{
		SpiReadSingle(pbuf, number);
	}
	else									// parallel mode
	{
		ParallelReadSingle(pbuf, number);	
	}
}

//===============================================================
// resets FIFO                                                  ;
// 02DEC2010	RP	Original Code
//===============================================================

void
Trf797xReset(void)
{
	command[0] = RESET;
	Trf797xDirectCommand(command);
}

//===============================================================
// resets IRQ Status                                            ;
// 02DEC2010	RP	Original Code
//===============================================================

void
Trf797xResetIrqStatus(void)
{
	u08_t irq_status[4];
	irq_status[0] = IRQ_STATUS;
	irq_status[1] = IRQ_MASK;
	
	if(SPIMODE)								// SPI mode given by jumper setting
	{
		Trf797xReadCont(irq_status, 2);		// read second reg. as dummy read
	}
	else									// parallel mode
	{
		Trf797xReadSingle(irq_status, 1);
	}
}

//===============================================================
//                                                              ;
// 02DEC2010	RP	Original Code
//===============================================================

void
Trf797xRunDecoders(void)
{
	command[0] = RUN_DECODERS;
	Trf797xDirectCommand(command);
}

//===============================================================
// 02DEC2010	RP	Original Code
//===============================================================

void
Trf797xStopDecoders(void)
{
	command[0] = STOP_DECODERS;
	Trf797xDirectCommand(command);
}

//===============================================================
// 02DEC2010	RP	Original Code
//===============================================================

void
Trf797xTransmitNextSlot(void)
{
	command[0] = TRANSMIT_NEXT_SLOT;
	Trf797xDirectCommand(command);
}

//===============================================================
// 02DEC2010	RP	Original Code
//===============================================================

void
Trf797xTurnRfOff(void)
{
	command[0] = CHIP_STATE_CONTROL;
	command[1] = CHIP_STATE_CONTROL;
	Trf797xReadSingle(&command[1], 1);
	command[1] &= 0x1F;
	Trf797xWriteSingle(command, 2);
}

//===============================================================
// 02DEC2010	RP	Original Code
//===============================================================

void
Trf797xTurnRfOn(void)
{
	command[0] = CHIP_STATE_CONTROL;
	command[1] = CHIP_STATE_CONTROL;
	Trf797xReadSingle(&command[1], 1);
	command[1] &= 0x3F;
	command[1] |= 0x20;
	Trf797xWriteSingle(command, 2);
}

//===============================================================
// NAME: void SpiWriteCont (u08_t *pbuf, u08_t length)
//
// BRIEF: Is used to write to a specific number of reader chip
// registers from a specific address upwards.
//
// INPUTS:
//	u08_t	*pbuf	address of first register followed by the
//					contents to write
//	u08_t	length	number of registers + 1
//
// OUTPUTS:
//
// PROCESS:	[1] write to the registers
//
// CHANGE:
// DATE  		WHO	DETAIL
//===============================================================

void
Trf797xWriteCont(u08_t *pbuf, u08_t length)
{
	if(SPIMODE)								// SPI mode given by jumper setting
	{
		SpiWriteCont(pbuf, length);
	}
	else									// parallel mode
	{
		ParallelWriteCont(pbuf, length);	
	}
}

//===============================================================
// 02DEC2010	RP	Original Code
//===============================================================

void
Trf797xWriteIsoControl(u08_t iso_control)
{
	u08_t write[4];
	
	if((iso_control & BIT5) == BIT5)
	{	
		return;
	}
	
	write[0] = ISO_CONTROL;
	write[1] = iso_control;
	write[1] &= ~BIT5;
	Trf797xWriteSingle(write, 2);

	iso_control &= 0x1F;
	
	/*write[0] = IRQ_MASK;
	write[1] = 0x7E;
	Trf797xWriteSingle(write, 2);
	
	if(iso_control < 0x0C)					// ISO14443A or ISO15693
	{
		write[0] = MODULATOR_CONTROL;
		write[1] = 0x21;					// OOK 100% 6.78 MHz
		Trf797xWriteSingle(write, 2);
	}
	else									// ISO14443B
	{
		write[0] = MODULATOR_CONTROL;
		write[1] = 0x20;					// ASK 10% 6.78 MHz
		Trf797xWriteSingle(write, 2);
	}
	
	if(iso_control < 0x08)					// ISO15693
	{	
		write[0] = TX_PULSE_LENGTH_CONTROL;
		write[1] = 0x80;					// 9.44 us
		Trf797xWriteSingle(write, 2);
		
		if((iso_control < 0x02) || (iso_control == 0x04) || (iso_control == 0x05)) // low data rate
		{
			write[0] = RX_NO_RESPONSE_WAIT_TIME;
			write[1] = 0x30;				// 1812 us
			Trf797xWriteSingle(write, 2);
		}
		else
		{
			write[0] = RX_NO_RESPONSE_WAIT_TIME;
			write[1] = 0x14;				// 755 us
			Trf797xWriteSingle(write, 2);
		}
		
		write[0] = RX_WAIT_TIME;
		write[1] = 0x1F;					// 293 us
		Trf797xWriteSingle(write, 2);
		
		write[0] = RX_SPECIAL_SETTINGS;
		write[1] = RX_SPECIAL_SETTINGS;
		Trf797xReadSingle(&write[1], 1);
		write[1] &= 0x0F;
		write[1] |= 0x40;					// bandpass 200 kHz to 900 kHz 
		Trf797xWriteSingle(write, 2);
		
		write[0] = SPECIAL_FUNCTION;
		write[1] = SPECIAL_FUNCTION;
		Trf797xReadSingle(&write[1], 1);
		write[1] &= ~BIT4;
		Trf797xWriteSingle(write, 2);
	}
	else									// ISO14443
	{
		if(iso_control < 0x0C)				// ISO14443A
		{			
			write[0] = TX_PULSE_LENGTH_CONTROL;
			write[1] = 0x20;					// 2.36 us
			Trf797xWriteSingle(write, 2);
		}
		else
		{
			write[0] = TX_PULSE_LENGTH_CONTROL;
			write[1] = 0x00;					// 2.36 us
			Trf797xWriteSingle(write, 2);
		}
		write[0] = RX_NO_RESPONSE_WAIT_TIME;
		write[1] = 0x0E;					// 529 us
		Trf797xWriteSingle(write, 2);
		
		write[0] = RX_WAIT_TIME;
		write[1] = 0x07;					// 66 us
		Trf797xWriteSingle(write, 2);
		
		write[0] = RX_SPECIAL_SETTINGS;
		write[1] = RX_SPECIAL_SETTINGS;
		Trf797xReadSingle(&write[1], 1);
		write[1] &= 0x0F;					// bandpass 450 kHz to 1.5 MHz
		write[1] |= 0x20;
		Trf797xWriteSingle(write, 2);
		
		write[0] = SPECIAL_FUNCTION;
		write[1] = SPECIAL_FUNCTION;
		Trf797xReadSingle(&write[1], 1);
		write[1] &= ~BIT4;
		Trf797xWriteSingle(write, 2);
	}*/
}

//===============================================================
// NAME: void Trf797xWriteSingle (u08_t *pbuf, u08_t length)
//
// BRIEF: Is used to write to specified reader chip registers.
//
// INPUTS:
//	u08_t	*pbuf	addresses of the registers followed by the
//					contents to write
//	u08_t	length	number of registers * 2
//
// OUTPUTS:
//
// PROCESS:	[1] write to the registers
//
// CHANGE:
// DATE  		WHO	DETAIL
// 24Nov2010	RP	Original Code
//===============================================================

void
Trf797xWriteSingle(u08_t *pbuf, u08_t length)
{
	if(SPIMODE)								// SPI mode given by jumper setting
	{
		SpiWriteSingle(pbuf, length);
	}
	else									// parallel mode
	{
		ParallelWriteSingle(pbuf, length);	
	}
}
