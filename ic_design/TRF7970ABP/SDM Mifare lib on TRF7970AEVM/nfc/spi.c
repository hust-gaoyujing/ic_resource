/*
 * File Name: spi.c
 *
 * Description: Contains functions to initialize SPI interface using
 * USCI_B0 and communicate with the TRF797x via this interface.
 *
 * Copyright (C) 2014 Texas Instruments Incorporated - http://www.ti.com/
 *
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
*/

#include "spi.h"
#include "trf797x.h"

//===============================================================

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

	//SpiStartCondition(); 					//SCLK High

	while(UCB0STAT & UCBUSY)
	{
	}

	SLAVE_SELECT_HIGH; 						//Stop SPI Mode
	
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
	u08_t pui8Command[2];
	
	pui8Command[0] = TRF797x_STATUS_CONTROL;
	pui8Command[1] = TRF797x_STATUS_CONTROL;
	SpiReadSingle(&pui8Command[1],1);
	pui8Command[1] |= 0x60;						// RF on and BIT 6 in Chip Status Control Register set
	SpiWriteSingle(pui8Command, 2);
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
	//u08_t	j = 0;

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
	
	while(length > 0)
	{	
		while (!(IFG2 & UCB0TXIFG))
		{
		}
		UCB0TXBUF = 0x00; 					// Receive initiated by a dummy TX write
					
		while(UCB0STAT & UCBUSY)
		{
		}
		
		*pbuf = UCB0RXBUF;
		pbuf++;
		length--;
	}
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

		while (!(IFG2 & UCB0TXIFG))			// USCI_B0 TX buffer ready?
		{
		}
		UCB0TXBUF = 0x00; 					// Receive initiated by a dummy TX write???
	
		while(UCB0STAT & UCBUSY)
		{
		}
		
		*pbuf = UCB0RXBUF;
		pbuf++;
		number--;

	}
	while(UCB0STAT & UCBUSY)
	{
		// Do Nothing
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

	TRF_ENABLE_SET;

	IRQ_PIN_SET;
	IRQ_EDGE_SET;								// rising edge interrupt
	
	SpiUsciSet();								// Set the USART

	LED_ALL_OFF;
	LED_PORT_SET;
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
//	UCB0CTL1 |= UCSWRST;                     		// Enable SW reset
//    UCB0CTL0 |= UCCKPH + UCMSB + UCMST + UCSYNC;  	// 3-pin, 8-bit SPI master
//	UCB0CTL0 &= ~UCCKPH;
//
//	UCB0CTL1 |= UCSSEL_2;                     		// SMCLK
//
//	UCB0BR0 = 0x04;									// 1.70 MHz
//	UCB0BR1 = 0;
//
//	P1SEL |= BIT5 + BIT6 + BIT7;					// P1.5,1.6,1.7 UCBOCLK,UCB0SIMO,UCB0SOMI, option select
//	P1SEL2 |= BIT5 + BIT6 + BIT7;					// P1.5,1.6,1.7 UCBOCLK,UCB0SIMO,UCB0SOMI, option select
//
//	SLAVE_SELECT_PORT_SET;  						// P3.0 - Slave Select
//	SLAVE_SELECT_HIGH;     							// Slave Select - inactive ( high)
//
//	UCB0CTL1 &= ~UCSWRST;                     		// **Initialize USCI state machine**

	UCB0CTL1 |= UCSWRST;							// Enable SW reset
	UCB0CTL0 |= UCMSB + UCMST + UCSYNC;				// 3-pin, 8-bit SPI master
	UCB0CTL1 |= UCSSEL_2;							// SMCLK

	UCB0BR0 = 0x04;
	UCB0BR1 = 0;
	SPI_PORT_SEL1 |= SCLK_PIN | MISO_PIN | MOSI_PIN;	// P3.1,3.2,3.3 UCBOCLK,UCB0SIMO,UCB0SOMI, option select

	SLAVE_SELECT_PORT_SET;							// P2.1 - Slave Select
	SLAVE_SELECT_HIGH;								// Slave Select - inactive ( high)

	UCB0CTL1 &= ~UCSWRST;							// **Initialize USCI state
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
	UCB0CTL0 |= UCMSB + UCMST + UCSYNC;				// 3-pin, 8-bit SPI master
	UCB0CTL1 |= UCSSEL_2;							// SMCLK

	UCB0BR0 = 0x04;
	UCB0BR1 = 0;
	SPI_PORT_SEL1 |= SCLK_PIN | MISO_PIN | MOSI_PIN;	// P3.1,3.2,3.3 UCBOCLK,UCB0SIMO,UCB0SOMI, option select

	SLAVE_SELECT_PORT_SET;							// P2.1 - Slave Select
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

		pbuf++;
		length--;
	}
	while(UCB0STAT & UCBUSY)
	{
	}

	if(!g_ui8DirectMode){
		SLAVE_SELECT_HIGH; 						// Stop SPI Mode
	}
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

			pbuf++;
			length--;
		}
	}
	while(UCB0STAT & UCBUSY)
	{
	}

	if(!g_ui8DirectMode){
		SLAVE_SELECT_HIGH; 						// Stop SPI Mode
	}
}
