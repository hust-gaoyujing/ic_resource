/****************************************************************
* FILENAME: spi.c
*
* BRIEF: Contains functions to initialize SPI interface using
* USCI_B0 and communicate with the TRF796x via this interface.
*
* Copyright (C) 2010 Texas Instruments, Inc.
*
* AUTHOR(S): Reiser Peter		DATE: 02 DEC 2010
*
* CHANGES:
* REV.	DATE		WHO	DETAIL
* 00	02Dec2010	RP	Orginal Version
* 01	07Dec2010	RP	Changed SPI clock frequency from 6.78 MHz
* 						to 1.70 MHz in SpiUsciExtClkSet() and
* 						also reduced frequency in SpiUsciSet()
* 01	07Dec2010	RP	integrated wait while busy loops in
* 						spi-communication
*
****************************************************************/

#include "spi.h"
#include "trf796x.h"

//===============================================================

u08_t	temp = 0;

extern u08_t direct_mode;

//===============================================================

void SpiStartCondition(void);
void SpiUsciSet(void);

//===============================================================
// NAME: void SpiDirectCommand (u08_t *pbuf)
//
// BRIEF: Is used in SPI mode to transmit a Direct Command to
// reader chip.
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
// 07Dec2010	RP	integrated wait while busy loops
//===============================================================

void 
SpiDirectCommand(u08_t *pbuf)
{	
	SLAVE_SELECT_LOW; 						// Start SPI Mode
	// set Address/Command Word Bit Distribution to command
	*pbuf = (0x80 | *pbuf);					// command
	*pbuf = (0x9f &*pbuf);					// command code
	while (!(IFG2 & UCB0TXIFG))				// USCI_B0 TX buffer ready?
	{
	}
	UCB0TXBUF = *pbuf;             			// Previous data to TX, RX

	while(UCB0STAT & UCBUSY)
	{
	}

	temp = UCB0RXBUF;

	SpiStartCondition(); 					//SCLK High

	while(UCB0STAT & UCBUSY)
	{
	}

	SLAVE_SELECT_HIGH; 						//Stop SPI Mode
	
	P3SEL |=  BIT3;      					//Revert Back
}

//===============================================================
// NAME: void SpiDirectMode (void)
//
// BRIEF: Is used in SPI mode to start Direct Mode.
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
// 07Dec2010	RP	integrated wait while busy loops
//===============================================================

void
SpiDirectMode(void)
{		
	u08_t command [2];
	
	command[0] = CHIP_STATE_CONTROL;
	command[1] = CHIP_STATE_CONTROL;
	SpiReadSingle(&command[1],1);
	command[1] |= 0x60;						// RF on and BIT 6 in Chip Status Control Register set
	SpiWriteSingle(command, 2);
}  	

//===============================================================
// NAME: void SpiRawWrite (u08_t *pbuf, u08_t length)
//
// BRIEF: Is used in SPI mode to write direct to the reader chip.
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
// 07Dec2010	RP	integrated wait while busy loops
//===============================================================

void 
SpiRawWrite(u08_t *pbuf, u08_t length)
{	
	SLAVE_SELECT_LOW; 						//Start SPI Mode
	while(length > 0)
	{	while (!(IFG2 & UCB0TXIFG));		// USCI_B0 TX buffer ready?
		{	
		}

		UCB0TXBUF = *pbuf;				// Previous data to TX, RX

		while(UCB0STAT & UCBUSY)
		{
		}

		temp=UCB0RXBUF;

		pbuf++;
		length--;
	}
	while(UCB0STAT & UCBUSY)
	{
	}
	SLAVE_SELECT_HIGH; 						// Stop SPI Mode		
}

//===============================================================
// NAME: void SpiReadCont (u08_t *pbuf, u08_t length)
//
// BRIEF: Is used in SPI mode to read a specified number of
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
// 07Dec2010	RP	integrated wait while busy loops
//===============================================================

void
SpiReadCont(u08_t *pbuf, u08_t length)
{	
	u08_t	j = 0;
	
	SLAVE_SELECT_LOW; 							//Start SPI Mode
	// Address/Command Word Bit Distribution
	*pbuf = (0x60 | *pbuf); 					// address, read, continuous
	*pbuf = (0x7f &*pbuf);						// register address
	while (!(IFG2 & UCB0TXIFG))					// USCI_B0 TX buffer ready?
	{
	}
	UCB0TXBUF = *pbuf;  						// Previous data to TX, RX
	
	while(UCB0STAT & UCBUSY)
	{
	}
	
	temp = UCB0RXBUF;
	UCB0CTL0 &= ~UCCKPH;
	
	if(*pbuf != 0x6C)							// execute only when IRQRead is not called
	{	
		if (length != 0x1F)
		{	
			for (j=0;j<2;j++)
			{	
				while (!(IFG2 & UCB0TXIFG))		// USCI_B0 TX buffer ready?
				{
				}
				UCB0TXBUF = 0x00; 				// Receive initiated by a dummy TX write
						
				while(UCB0STAT & UCBUSY)
				{
				}
				_NOP();
				_NOP();
				temp = UCB0RXBUF;
			}
		}
	}
	while(length > 0)
	{	
		while (!(IFG2 & UCB0TXIFG))
		{
		}
		UCB0TXBUF = 0x00; 					// Receive initiated by a dummy TX write
					
		while(UCB0STAT & UCBUSY)
		{
		}
		_NOP();
		_NOP();
		*pbuf = UCB0RXBUF;
		pbuf++;
		length--;
	}
	UCB0CTL0 |= UCCKPH;
	while(UCB0STAT & UCBUSY)
	{
	}
	SLAVE_SELECT_HIGH; 						// Stop SPI Mode

}

//===============================================================
// NAME: void SpiReadSingle (u08_t *pbuf, u08_t number)
//
// BRIEF: Is used in SPI mode to read specified reader chip
// registers.
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
// 07Dec2010	RP	integrated wait while busy loops
//===============================================================

void
SpiReadSingle(u08_t *pbuf, u08_t number)
{			
	SLAVE_SELECT_LOW; 						// Start SPI Mode
	while(number > 0)
	{	
		// Address/Command Word Bit Distribution
		*pbuf = (0x40 | *pbuf); 			// address, read, single
		*pbuf = (0x5f & *pbuf);				// register address
	
		while (!(IFG2 & UCB0TXIFG))			// USCI_B0 TX buffer ready?
		{
		}
		UCB0TXBUF = *pbuf;					// Previous data to TX, RX
	
		while(UCB0STAT & UCBUSY)
		{
		}

 		temp=UCB0RXBUF;
	
		UCB0CTL0 &= ~UCCKPH;
	
		while (!(IFG2 & UCB0TXIFG))			// USCI_B0 TX buffer ready?
		{
		}
		UCB0TXBUF = 0x00; 					// Receive initiated by a dummy TX write???
	
		while (!(IFG2 & UCB0RXIFG))			// USCI_B0 RX buffer ready?
		{
		}
		_NOP();
		_NOP();
		*pbuf = UCB0RXBUF;
		pbuf++;
		number--;

		UCB0CTL0 |= UCCKPH;
	}
	while(UCB0STAT & UCBUSY)
	{
	}
	SLAVE_SELECT_HIGH; 						// Stop SPI Mode
}

//===============================================================
// Settings for SPI Mode                                        ;
// 02DEC2010	RP	Original Code
//===============================================================

void
SpiSetup(void)
{	
	ENABLE_SET;
	
	IRQ_PIN_SET;
	IRQ_EDGE_SET;								// rising edge interrupt
	
	SpiUsciSet();								// Set the USART

	LED_ALL_OFF;
	LED_PORT_SET;
}

//===============================================================
// 02DEC2010	RP	Original Code
//===============================================================

void
SpiStartCondition(void)					//Make the SCLK High
{
	P3SEL &= ~ BIT3;
	P3DIR |= BIT3;
	P3OUT |= BIT3; 			//Make SCLK High
}

//===============================================================
// NAME: void SpiUsciExtClkSet (void)
//
// BRIEF: Is used to switch SPI data clock from DCO to more
// stabile extern clock
//
// INPUTS:
//
// OUTPUTS:
//
// PROCESS:	[1] switch SPI data clock
//
// CHANGE:
// DATE  		WHO	DETAIL
// 24Nov2010	RP	Original Code
// 07Dec2010	RP	changed SPI clock from 6.78MHz to 1.70MHz
//===============================================================

void
SpiUsciExtClkSet(void)	  							//Uses USCI_B0
{
	UCB0CTL1 |= UCSWRST;                     		// Enable SW reset
    UCB0CTL0 |= UCCKPH + UCMSB + UCMST + UCSYNC;  	// 3-pin, 8-bit SPI master


	UCB0CTL1 |= UCSSEL_2;                     		// SMCLK

	UCB0BR0 |= 4;									// SPI clock frequency = SMCLK / 4 (1.70 MHz)
	UCB0BR1 = 0;

	P3SEL |= BIT1 + BIT2 + BIT3;                   	// P3.1, 3.2, 3.3 UCB0SIMO,UCB0SOMI,UCBOCLK option select

	SLAVE_SELECT_PORT_SET;  						// P3.0 - Slave Select
	SLAVE_SELECT_HIGH;     							// Slave Select - inactive ( high)

	UCB0CTL1 &= ~UCSWRST;                     		// **Initialize USCI state machine**
}

//===============================================================
// NAME: void SpiUsciSet (void)
//
// BRIEF: Is used to set USCI B0 for SPI communication
//
// INPUTS:
//
// OUTPUTS:
//
// PROCESS:	[1] make settings
//
// CHANGE:
// DATE  		WHO	DETAIL
// 24Nov2010	RP	Original Code
// 07Dec2010	RP	reduced SPI clock frequency
//===============================================================

void
SpiUsciSet(void)									//Uses USCI_B0
{
	UCB0CTL1 |= UCSWRST;							// Enable SW reset
	UCB0CTL0 |= UCCKPH + UCMSB + UCMST + UCSYNC;	// 3-pin, 8-bit SPI master
	UCB0CTL1 |= UCSSEL_2;							// SMCLK

	UCB0BR0 |= 4;									// SPI clock frequency = SMCLK / 4
	UCB0BR1 = 0;	
	P3SEL |= BIT1 + BIT2 + BIT3;					// P3.1,3.2,3.3 UCB0SIMO,UCB0SOMI,UCBOCLK option select

	SLAVE_SELECT_PORT_SET;							// P3.0 - Slave Select
	SLAVE_SELECT_HIGH;								// Slave Select - inactive ( high)

	UCB0CTL1 &= ~UCSWRST;							// **Initialize USCI state machine**
}


//===============================================================
// NAME: void SpiWriteCont (u08_t *pbuf, u08_t length)
//
// BRIEF: Is used in SPI mode to write to a specific number of
// reader chip registers from a specific address upwards.
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
// 07Dec2010	RP	integrated wait while busy loops
//===============================================================

void
SpiWriteCont(u08_t *pbuf, u08_t length)
{	
	SLAVE_SELECT_LOW; 						// Start SPI Mode
	// Address/Command Wort Bit Distribution
	*pbuf = (0x20 | *pbuf); 				// address, write, continuous
	*pbuf = (0x3f &*pbuf);					// register address
	while(length > 0)
	{	
		while (!(IFG2 & UCB0TXIFG))			// USCI_B0 TX buffer ready?
		{
		}
		UCB0TXBUF = *pbuf;					// Previous data to TX, RX
		while(UCB0STAT & UCBUSY)
		{
		}
		temp = UCB0RXBUF;

		pbuf++;
		length--;
	}
	while(UCB0STAT & UCBUSY)
	{
	}
	SLAVE_SELECT_HIGH; 						// Stop SPI Mode
}

//===============================================================
// NAME: void SpiWriteSingle (u08_t *pbuf, u08_t length)
//
// BRIEF: Is used in SPI mode to write to a specified reader chip
// registers.
//
// INPUTS:
//	u08_t	*pbuf	addresses of the registers followed by the
//					contends to write
//	u08_t	length	number of registers * 2
//
// OUTPUTS:
//
// PROCESS:	[1] write to the registers
//
// CHANGE:
// DATE  		WHO	DETAIL
// 24Nov2010	RP	Original Code
// 07Dec2010	RP	integrated wait while busy loops
//===============================================================

void
SpiWriteSingle(u08_t *pbuf, u08_t length)
{
	u08_t	i = 0;
		
	SLAVE_SELECT_LOW; 						// Start SPI Mode

	while(length > 0)
	{	
		// Address/Command Word Bit Distribution
		// address, write, single (fist 3 bits = 0)
		*pbuf = (0x1f &*pbuf);				// register address
		for(i = 0; i < 2; i++)
		{	
			while (!(IFG2 & UCB0TXIFG))		// USCI_B0 TX buffer ready?
			{
			}
			UCB0TXBUF = *pbuf;  			// Previous data to TX, RX

			while(UCB0STAT & UCBUSY)
			{
			}

			temp = UCB0RXBUF;

			pbuf++;
			length--;
		}
	}
	while(UCB0STAT & UCBUSY)
	{
	}
	if(direct_mode == 0x00)
	{
		SLAVE_SELECT_HIGH; 					// Stop SPI Mode
	}
}
