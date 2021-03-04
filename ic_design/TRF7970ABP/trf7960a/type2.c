/****************************************************************
* FILENAME: type2.c
*
* BRIEF: Contains functions to execute NFC Type 2 commands.
*
* Copyright (C) 2010 Texas Instruments, Inc.
*
* AUTHOR(S): Reiser Peter		DATE: 02 DEC 2010
*
* EDITED BY:
* *
*
****************************************************************/

#include "type2.h"

//===============================================================

extern u08_t buf[300];
extern u08_t i_reg;
extern u08_t irq_flag;
extern u08_t rx_error_flag;
extern s08_t rxtx_state;

//===============================================================

void Type2ReadTwoBlocks(u08_t rd2b);
void Type2WriteTwoBlocks(u08_t wr2b);
void Type2WriteOneBlock(u08_t wr1b);
void Type2ReadFourBlocks(u08_t rd4b);

//===============================================================

void
Type2Command(u08_t *pbuf)
{
	switch (*pbuf)
	{
		case 0x30:
			Type2ReadFourBlocks(*(pbuf+1));
			break;
		case 0xA2:
			Type2WriteOneBlock(*(pbuf+1));
			break;
		case 0x31:
			Type2ReadTwoBlocks(*(pbuf+1));
			break;
		case 0xA1:
			Type2WriteTwoBlocks(*(pbuf+1));
			break;
		default:
			UartSendCString("Unknown NFC Type 2 command.\r\n");
	}
}

//===============================================================
// NAME: void Type2ReadTwoBlocks (u08_t rd2b)
//
// BRIEF: Is used to read 2 Blocks (8 Bytes) out of my-d move
// memory.
//
// INPUTS:
//	Parameters:
//		u08_t		rd2b		address of first block
//
// OUTPUTS:
//
// PROCESS:	[1] send command
//			[2] receive response
//			[3] send response to host
//
// NOTE:	This command is my-d move specific.
//
// CHANGE:
// DATE  		WHO	DETAIL
// 24Nov2010	RP	Original Code
//===============================================================

void
Type2ReadTwoBlocks(u08_t rd2b)
{	
	u08_t i = 0, command[4];
	
	buf[0] = 0x8F; 						// reset FIFO
	buf[1] = 0x91; 						// send with CRC
	buf[2] = 0x3D; 						// write continuous from register 
	buf[3] = 0x00;						// value for register 1D
	buf[4] = 0x20;						// register 1E (# of bytes to be transmitted)
	buf[5] = 0x31;						// RD2B command
	//buf[6+30] = rd2b;						// addressed block
	
	buf[200] = 80;							// tell apart from Ack/Nack
	
	rx_error_flag = 0x00;
	
	command[0]	= SPECIAL_FUNCTION;
	command[1]	= SPECIAL_FUNCTION;
	Trf796xReadSingle(&command[1], 1);
	command[1] |= BIT2;						// enable 4-bit receive
	
	McuCounterSet();						// TimerA set 
	COUNT_VALUE = COUNT_1ms * 5;			// 5ms 
	IRQ_CLR;								// PORT2 interrupt flag clear 
	IRQ_ON;
	
	Trf796xReset();							// FIFO has to be reset before recieving the next response

	Trf796xRawWrite(&buf[0], 7);			// set Special Function Register
	
	i_reg = 0x01;
	START_COUNTER;							// start timer up mode
	irq_flag = 0x00;
	while(irq_flag == 0x00)					// wait for end of TX interrupt
	{
	}
	
	Trf796xWriteSingle(command, 2);			// enable 4-bit receive
	
	rxtx_state = 1;						// the response will be stored in buf[1] upwards
		
	McuCounterSet();						// TimerA set
	COUNT_VALUE = COUNT_1ms * 10;    
	START_COUNTER;							// start timer up mode
	i_reg = 0x01;
	while(i_reg == 0x01)					// wait for interrupt
	{
	}

	if(rx_error_flag == 0x02)
	{	
		i_reg = 0x02;
	}
	
	if(i_reg == 0xFF)						// recieved response
	{	
		UartPutChar('[');
		for(i = 1; i < rxtx_state; i++)
		{
			UartPutByte(buf[i]);
		}
		UartPutChar(']');
	}
	else if(i_reg == 0x02)
	{	

		if(buf[200] < 0x20)					// not acknowledged
		{	
			UartSendCString("[NACK]");
		}
		else								// collision occured
		{
			UartPutChar('[');
			UartPutChar('z');
			UartPutChar(']');
		}
	}
	else if(i_reg == 0x00)					// no responce
	{	
		UartPutChar('[');
		UartPutChar(']');
	}
	else
	{
	}
	command[0]	= SPECIAL_FUNCTION;
	command[1]	= SPECIAL_FUNCTION;
	Trf796xReadSingle(&command[1], 1);
	command[1] &= ~BIT2;
	Trf796xWriteSingle(command, 2);			// disable 4-bit receive
}

//===============================================================
// NAME: void Type2WriteTwoBlocks (u08_t wr2b)
//
// BRIEF: Is used to program 2 Blocks (8 Bytes) in my-d move
// memory.
//
// INPUTS:
//	Parameters:
//		u08_t		wr2b		address of first block
//
// OUTPUTS:
//
// PROCESS:	[1] send command
//			[2] receive response
//			[3] send response to host
//
// NOTE:	This command is my-d move specific.
//
// CHANGE:
// DATE  		WHO	DETAIL
// 24Nov2010	RP	Original Code
//===============================================================

void
Type2WriteTwoBlocks(u08_t wr2b)
{
	u08_t command[6];
	
	//for(i=14; i>4; i--)						// write parameter in right frameposition
	//{	
	//	buf[i+2] = buf[i];
	//}
	
	buf[0] = 0x8F; 							// reset FIFO
	buf[1] = 0x91; 							// send with CRC
	buf[2] = 0x3D; 							// write continuous from register 
	buf[3] = 0x00;							// value for register 1D
	buf[4] = 0xA0;							// register 1E (# of bytes to be transmitted)
	buf[5] = 0xA1;							// WR2B command
	//buf[6] = wr2b;
	
	buf[200] = 80;							// tell apart from Ack/Nack

	rx_error_flag = 0x00;
	
	command[0]	= SPECIAL_FUNCTION;
	command[1]	= SPECIAL_FUNCTION;
	Trf796xReadSingle(&command[1], 1);
	command[1] |= BIT2;
	Trf796xWriteSingle(command, 2);			// enable 4-bit receive
	
	Trf796xRawWrite(&buf[0], 15);
	
	IRQ_CLR;								// PORT2 interrupt flag clear
	IRQ_ON;
	
	i_reg = 0x01;
	rxtx_state = 1;							// the response will be stored in buf[1] upwards
	
	// wait for end of transmit
	while(i_reg == 0x01)
	{	
		McuCounterSet();
		COUNT_VALUE = COUNT_1ms * 10;		// for 10 ms TIMEOUT
		START_COUNTER;						// start timer up mode
		irq_flag = 0x00;
		while(irq_flag == 0x00)				// wait for interrupt
		{
		}
	}
	
	i_reg = 0x01;
	
	McuCounterSet();						// TimerA set 
	COUNT_VALUE = COUNT_1ms * 6;			// 10ms 
	START_COUNTER;
	i_reg = 0x01;
	
	while(i_reg == 0x01)					// wait for RX complete
	{
	}
	
	if(rx_error_flag == 0x02)
	{	
		i_reg = 0x02;
	}
	
	if(i_reg == 0x02)
	{	
		if(buf[200] < 0x20)					// not acknowledged
		{	
			UartSendCString("[NACK]");
		}
		else if((buf[200] & 0xF0) == 0xA0)	// acknowledged
		{	
			UartSendCString("[ACK]");
		}
		else								// collision occured
		{
			UartPutChar('[');
			UartPutChar('z');
			UartPutChar(']');
		}
	}
	else if(i_reg == 0x00)					// no response
	{		
		UartPutChar('[');
		UartPutChar(']');
	}
	else
	{
	}
	command[0]	= SPECIAL_FUNCTION;
	command[1]	= SPECIAL_FUNCTION;
	Trf796xReadSingle(&command[1], 1);
	command[1] &= ~BIT2;
	Trf796xWriteSingle(command, 2);			// disable 4-bit receive
}

//===============================================================
// NAME: void Type2ReadFourBlocks (u08_t rd4b)
//
// BRIEF: Is used to read 4 Blocks (16 Bytes) out of NFC Type 2
// memory.
//
// INPUTS:
//	Parameters:
//		u08_t		rd4b		address of first block
//
// OUTPUTS:
//
// PROCESS:	[1] send command
//			[2] receive response
//			[3] send response to host
//
// CHANGE:
// DATE  		WHO	DETAIL
// 24Nov2010	RP	Original Code
//===============================================================

void
Type2ReadFourBlocks(u08_t rd4b)
{
	u08_t i = 0, command[6];
	
	buf[0] = 0x8F; 							// reset FIFO
	buf[1] = 0x91; 							// send with CRC
	buf[2] = 0x3D; 							// write continuous from register 
	buf[3] = 0x00;							// value for register 1D
	buf[4] = 0x20;							// register 1E (# of bytes to be transmitted)
	buf[5] = 0x30;							// RD4B command
	//buf[6] = rd4b;							// addressed block
	
	buf[200] = 80;							// tell apart from Ack/Nack
	
	rx_error_flag = 0x00;
	
	command[0]	= SPECIAL_FUNCTION;
	command[1]	= SPECIAL_FUNCTION;
	Trf796xReadSingle(&command[1], 1);
	command[1] |= BIT2;						// enable 4-bit receive
	
	McuCounterSet();						// TimerA set 
	COUNT_VALUE = COUNT_1ms * 5;			// 5ms 
	IRQ_CLR;								// PORT2 interrupt flag clear 
	IRQ_ON;
	
	Trf796xReset();							// FIFO has to be reset before recieving the next response
	
	Trf796xRawWrite(&buf[0], 7);			// set Special Function Register
	
	i_reg = 0x01;
	START_COUNTER;							// start timer up mode
	irq_flag = 0x00;
	while(irq_flag == 0x00)					// wait for end of TX interrupt
	{
	}
	
	Trf796xWriteSingle(command, 2);			// enable 4-bit receive
	
	rxtx_state = 1;							// the response will be stored in buf[1] upwards
		
	McuCounterSet();				// TimerA set
	COUNT_VALUE = COUNT_1ms * 10;    
	START_COUNTER;							// start timer up mode
	i_reg = 0x01;
	while(i_reg == 0x01)					// wait for RX complete
	{
	}
	
	if(rx_error_flag == 0x02)
	{	
		i_reg = 0x02;
	}

	if(i_reg == 0xFF)						// recieved response
	{	
		UartPutChar('[');
		for(i = 1; i < rxtx_state; i++)
		{
			UartPutByte(buf[i]);
		}
		UartPutChar(']');
	}
	else if(i_reg == 0x02)					// collision occured
	{	
		if(buf[200] < 0x20)					// not acknowledged
		{	
			UartSendCString("[NACK]");
		}
		else								// collision occured
		{	
			UartPutChar('[');
			UartPutChar('z');
			UartPutChar(']');
		}
	}
	else if(i_reg == 0x00)					// no responce
	{	
		UartPutChar('[');
		UartPutChar(']');
	}
	else
	{
	}
	command[0]	= SPECIAL_FUNCTION;
	command[1]	= SPECIAL_FUNCTION;
	Trf796xReadSingle(&command[1], 1);
	command[1] &= ~BIT2;
	Trf796xWriteSingle(command, 2);			// disable 4-bit receive
}

//===============================================================
// NAME: void Type2WriteOneBlock (u08_t wr1b)
//
// BRIEF: Is used to program 1 Block (4 Bytes) in NFC Type 2
// memory. This command should be used to programm OTB block and
// Locking Bytes as well.
//
// INPUTS:
//	Parameters:
//		u08_t		wr1b		address of block
//
// OUTPUTS:
//
// PROCESS:	[1] send command
//			[2] receive response
//			[3] send response to host
//
// CHANGE:
// DATE  		WHO	DETAIL
// 24Nov2010	RP	Original Code
//===============================================================

void
Type2WriteOneBlock(u08_t wr1b)
{	
	u08_t command[10];
	
	//for(i=10; i>4; i--)						// write parameter in right frameposition
	//{	
	//	buf[i+2] = buf[i];
	//}
	
	buf[0] = 0x8F; 							// reset FIFO
	buf[1] = 0x91; 							// send with CRC
	buf[2] = 0x3D; 							// write continuous from register 
	buf[3] = 0x00;							// value for register 1D
	buf[4] = 0x60;							// register 1E (# of bytes to be transmitted)
	buf[5] = 0xA2;							// WR2B command
	//buf[6] = wr1b;
	
	buf[200] = 80;							// tell apart from Ack/Nack
	
	rx_error_flag = 0x00;
	
	command[0]	= SPECIAL_FUNCTION;
	command[1]	= SPECIAL_FUNCTION;
	Trf796xReadSingle(&command[1], 1);
	command[1] |= BIT2;
	Trf796xWriteSingle(command, 2);			// enable 4-bit receive
	
	Trf796xRawWrite(&buf[0], 11);
	
	IRQ_CLR;								// PORT2 interrupt flag clear
	IRQ_ON;
	
	i_reg = 0x01;
	rxtx_state = 1;							// the response will be stored in buf[1] upwards
	
	// wait for end of transmit
	while(i_reg == 0x01)
	{	
		McuCounterSet();
		COUNT_VALUE = COUNT_1ms * 5;		// for 10 ms TIMEOUT
		START_COUNTER;						// start timer up mode
		irq_flag = 0x00;
		while(irq_flag == 0x00)				// wait for interrupt
		{
		}
	}
	
	i_reg = 0x01;
	
	McuCounterSet();					// TimerA set 
	COUNT_VALUE = COUNT_1ms * 6;			// 6ms 
	START_COUNTER;
	
	while(i_reg == 0x01)					// wait for RX complete
	{
	}
	
	if(rx_error_flag == 0x02)
	{	
		i_reg = 0x02;
	}

	if(i_reg == 0x02)
	{	
		if(buf[200] < 0x20)					// not acknowledged
		{	
			UartSendCString("[NACK]");
		}
		else if((buf[200] & 0xF0) == 0xA0)	// acknowledged
		{	
			UartSendCString("[ACK]");
		}
		else								// collision occured
		{
			UartPutChar('[');
			UartPutChar('z');
			UartPutChar(']');
		}
	}
	else if(i_reg == 0x00)					// no response
	{		
			UartPutChar('[');
			UartPutChar(']');
	}
	else
	{
	}
	command[0]	= SPECIAL_FUNCTION;
	command[1]	= SPECIAL_FUNCTION;
	Trf796xReadSingle(&command[1], 1);
	command[1] &= ~BIT2;
	Trf796xWriteSingle(command, 2);			// disable 4-bit receive
}
