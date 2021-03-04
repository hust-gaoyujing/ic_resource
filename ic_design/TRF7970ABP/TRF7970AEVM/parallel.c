/****************************************************************
* FILENAME: parallel.c
*
* BRIEF: Contains functions to initialize parallel interface and
* communicate with TRF796x via this interface.
*
* Copyright (C) 2010 Texas Instruments, Inc.
*
* AUTHOR(S): Reiser Peter		DATE: 02 DEC 2010
*
* EDITED BY:
* *
*
****************************************************************/

#include "parallel.h"
#include "trf797x.h"
#include "msp430f23x0.h"


//===============================================================

extern u08_t direct_mode;

//===============================================================

void ParallelStartCondition(void);
void ParallelStopCondition(void);
void ParallelStopContinuous(void);

//===============================================================
// NAME: void ParallelDirectCommand (u08_t *pbuf)
//
// BRIEF: Is used in parallel mode to transmit a Direct Command
// to reader chip.
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
ParallelDirectCommand(u08_t *pbuf)
{	
	ParallelStartCondition();
	// set Address/Command Word Bit Distribution to command
	*pbuf = (0x80 | *pbuf);					// command
	*pbuf = (0x9f &*pbuf);					// command code
	TRF_WRITE = *pbuf;						// send command
	CLK_ON;
	CLK_OFF;

	//
	// Direct Command Stop Condition
	//
	ParallelStopCondition();
	//TRF_WRITE |= 0x80;
	//CLK_ON;
	//TRF_WRITE = 0x00;
	//CLK_OFF;
}

//===============================================================
// NAME: void ParallelDirectMode (void)
//
// BRIEF: Is used in parallel mode to start Direct Mode.
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

void
ParallelDirectMode(void)
{	
	u08_t command [2];
	
	command[0] = CHIP_STATE_CONTROL;
	command[1] = CHIP_STATE_CONTROL;
	ParallelReadSingle(&command[1], 1);
	command[1] |= 0x60;						// RF on and BIT 6 in Chip Status Control Register set
	ParallelWriteSingle(command, 2);
}

//===============================================================
// NAME: void ParallelRawWrite (u08_t *pbuf, u08_t length)
//
// BRIEF: Is used in parallel mode to write direct to the reader
// chip.
//
// INPUTS:
//	Parameters:
//		u08_t		*pbuf		raw data
//		u08_t		length		number of data bytes
//
// OUTPUTS:
//
// PROCESS:	[1] send raw data to reader chip
//
// CHANGE:
// DATE  		WHO	DETAIL
// 24Nov2010	RP	Original Code
//===============================================================

void 
ParallelRawWrite(u08_t *pbuf, u08_t length)
{
	ParallelStartCondition();
	while(length > 0)
	{	TRF_WRITE = *pbuf;					// send command
		CLK_ON;
		CLK_OFF;
		pbuf++;
		length--;
	}
	ParallelStopContinuous();
}

//===============================================================
// NAME: void ParallelReadCont (u08_t *pbuf, u08_t length)
//
// BRIEF: Is used in parallel mode to read a specified number of
// reader chip registers from a specified address upwards.
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
ParallelReadCont(u08_t *pbuf, u08_t length)
{
	ParallelStartCondition();
	// Address/Command Word Bit Distribution
	*pbuf = (0x60 | *pbuf); 				// address, read, continuous
	*pbuf = (0x7f &*pbuf);					// register address
	TRF_WRITE = *pbuf;						// end command
	CLK_ON;
	__no_operation();
	CLK_OFF;
	TRF_DIR_IN;								// read register
	
	while(length > 0)
	{	
		CLK_ON;
		CLK_OFF;
		*pbuf = TRF_READ;
		pbuf++;
		length--;
	}										// while
	ParallelStopContinuous();
}

//===============================================================
// NAME: void ParallelReadSingle (u08_t *pbuf, u08_t number)
//
// BRIEF: Is used in parallel mode to read specified reader
// chip registers.
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
ParallelReadSingle(u08_t *pbuf, u08_t number)
{	
	ParallelStartCondition();
	while(number > 0)
	{	
		// Address/Command Word Bit Distribution
		*pbuf = (0x40 | *pbuf); 			// address, read, single
		*pbuf = (0x5f &*pbuf);				// register address

		TRF_WRITE = *pbuf;					// send command
		CLK_ON;
		CLK_OFF;

		TRF_DIR_IN;							// read register
		CLK_ON;
		CLK_OFF;
		*pbuf = TRF_READ;					// read data at falling edge.

		TRF_WRITE = 0x00;
		TRF_DIR_OUT;

		pbuf++;
		number--;
	}
	ParallelStopCondition();
}

//===============================================================
// Settings for Parallel Mode                                   ;
// 02DEC2010	RP	Original Code
//===============================================================

void
ParallelSetup(void)
{
	ENABLE_SET;
	
	TRF_DIR_OUT;							// P4 output
	TRF_FUNC;
	TRF_WRITE = 0x00;						// P4 set to 0 - choose parallel inteface for the TRF796x

	CLK_OFF;
	CLK_P_OUT_SET;							// DATA_CLK on P3.3

	IRQ_PIN_SET;
	IRQ_EDGE_SET;								// rising edge interrupt

	LED_ALL_OFF;
	LED_PORT_SET;
}

//===============================================================
// 02DEC2010	RP	Original Code
//===============================================================

void 
ParallelStartCondition(void)
{	
	TRF_WRITE = 0x00;
	CLK_ON;
	TRF_WRITE = 0xff;
	CLK_OFF;
}

//===============================================================
// 02DEC2010	RP	Original Code
//===============================================================

void
ParallelStopCondition(void)
{
	if(direct_mode == 0)
	{
		TRF_WRITE = 0x80;
		CLK_ON;
		TRF_WRITE = 0x00;
		CLK_OFF;
	}
}

//===============================================================
// 02DEC2010	RP	Original Code
//===============================================================

void
ParallelStopContinuous(void)				// stop condition for continuous mode
{
	TRF_WRITE = 0x00;
	TRF_DIR_OUT;
	TRF_WRITE = 0x80;
	__no_operation();
	TRF_WRITE = 0x00;
}

//===============================================================
// NAME: void ParallelWriteCont (u08_t *pbuf, u08_t length)
//
// BRIEF: Is used in parallel mode to write to a specific number
// of reader chip registers from a specific address upwards.
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
// 24Nov2010	RP	Original Code
//===============================================================

void
ParallelWriteCont(u08_t *pbuf, u08_t length)
{
	ParallelStartCondition();
	// Address/Command Word Bit Distribution
	*pbuf = (0x20 | *pbuf); 				// address, write, continuous
	*pbuf = (0x3f &*pbuf);					// register address
	while(length > 0)
	{	
		TRF_WRITE = *pbuf;					// send command
		CLK_ON;
		CLK_OFF;
		pbuf++;
		length--;
	}
	ParallelStopContinuous();
}

//===============================================================
// NAME: void ParallelWriteSingle (u08_t *pbuf, u08_t length)
//
// BRIEF: Is used in parallel mode to write data to specified
// reader chip registers.
//
// INPUTS:
//	Parameters:
//		u08_t	*pbuf	addresses of the registers followed by
//						the contents to write
//		u08_t	length	2 * number of the registers
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
ParallelWriteSingle(u08_t *pbuf, u08_t length)
{
	u08_t	i;
	
	ParallelStartCondition();
		
	// Address/Command Word Bit Distribution
	// address, write, single (fist 3 bits = 0)
	*pbuf = (0x1f &*pbuf);				// register address
	for(i = 0; i < 2; i++)
	{	
		TRF_WRITE = *pbuf;				// send command and data
		CLK_ON;
		CLK_OFF;
		pbuf++;
	}

	ParallelStopCondition();
}
