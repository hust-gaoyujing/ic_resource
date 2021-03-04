/****************************************************************
* FILENAME: uart.c
*
* BRIEF: Contains functions to initialize UART interface using
* USCI_A0 and communicate with the host via this interface.
*
* Copyright (C) 2010 Texas Instruments, Inc.
*
* AUTHOR(S): Reiser Peter		START DATE: 20 SEP 2010
*
* EDITED BY:
* *
*
****************************************************************/

#include "uart.h"
#include "trf796x.h"

//===============================================================

#if TRIGGER
	extern u08_t trigger;
#endif

u08_t rx_data = 0;						// RS232 RX data byte
u08_t rx_done = 0;
extern u08_t first_spi_data;
extern u08_t i_reg;
extern u08_t irq_flag;
extern u08_t host_control_flag;
extern u08_t stand_alone_flag;
extern u08_t enable;

//===============================================================

u08_t UartNibble2Ascii(u08_t anibble);

//===============================================================
// 02DEC2010	RP	Original Code
//===============================================================

void
UartBaudSet(u08_t mode)
{
	if(mode == 0x00)
	{	
		UCA0BR0 = BAUD0;					// baud rate register
		UCA0BR1 = BAUD1;
	}
	else
	{	
		UCA0BR0 = BAUD0EN;					// baud rate register
		UCA0BR1 = BAUD1EN;
	}
}

//===============================================================
// 02DEC2010	RP	Original Code
//===============================================================

u08_t
UartGetLine(u08_t *pline)
{
	u08_t	reading = 0;
	u08_t	err_flg = 0;					// assume no error
	u08_t	pos = 0;						// counts number of received nibbles
	u08_t	length_byte = 0xff;				// max 256 bytes allowed (512 nibbles)
											// assume max number

	// wait for SOF: '0' followed by '1'
	if(first_spi_data == 0)
	{	
		irq_flag = 0x00;
		while(irq_flag == 0x00);			// sync, continue after irq
		{
		}
		while(rx_data != '0')
		{
		}
		UartPutChar('0');
	}
	else
	{	
		first_spi_data = 0;
	}

	if (stand_alone_flag == 0)
	{	
		irq_flag = 0x00;
		while(irq_flag == 0x00);			// sync, continue after irq
		{
		}
	}
	while(rx_data != '1')
	{
	}

	UartPutChar('1');

	rx_done = 0;

	pos = 0;								// character position counter - 8bit
	reading = 1;							// flag: reading not yet finished
	
	while(reading == 1)						// loop and read characters
	{	
		while(rx_done == 0)
		{
		}
		// sync, wakeup by irq
		rx_done = 0;
		switch(rx_data)						// process RETURN key
		{
			case '\r':
				break;						// ignore CR
	
			case '\n':
				reading = 0;				// exit read loop
				break;
	
			// backspace
			case '\b':
				if(pos > 0)					// is there a char to delete?
				{	
					pos--;					// remove it from buffer
					UartPutBksp();			// go back and erase on screen
					if(pos & 0x01 > 0)		//(high) even byte
					{	
						*pline--;
						*pline &= 0xF0;		// clear lo nibble
					}
				}
				break;
	
			// other characters
			default:
				if(rx_data >= 'a')
				{	
					rx_data -= 32;
				}							// change to uppercase
	
				// discard if not hex
				if((rx_data < '0') || ((rx_data > '9') && (rx_data < 'A')) || (rx_data > 'F'))
				{	
					break;
				}
	
				// only store characters if buffer has space
				if(pos++ < 2 * 300)
				{	
					UartPutChar(rx_data);	// echo
					if(rx_data > '9')		// If ASCII A-F then add 9
					{	
						rx_data = (rx_data & 0x0F) + 9;
					}
	
					if((pos & 0x01) == 0)	// (low) odd nibble
					{	
						*pline += (rx_data & 0x0F);	// store
						if(pos == 2)
						{	
							// just finished receiving 2 nibbles containing number of expected data bytes ;
							// change length_bytes (total number of expected data bytes)
							length_byte = *pline;
						}
						pline++;
						if(((length_byte - 1) * 2) == pos)
						{	
							reading = 0;			// flag loop exit - done
						}
					}
					else							// (high) even nibbl
					{	
						rx_data = rx_data << 4;		// Move to high nibble
						*pline = rx_data;			// store
					}
				}
				else
				{	
					err_flg = 1;
				}
		}
	}
	
	#if TRIGGER
		if(pos != 0)
		{	
			trigger = 1;
		}
	#endif

	return(err_flg);								// normal exit
}													// UartGetLine

//===============================================================
// convert a nibble to ASCII hex byte                           ;
// output a binary coded byte as two hex coded ascii bytes      ;
// 02DEC2010	RP	Original Code
//===============================================================

u08_t
UartNibble2Ascii(u08_t anibble)
{
	u08_t	ascii_out = anibble;

	if(anibble > 9)							// If req ASCII A-F then add 7(hex)
	{	
		ascii_out = ascii_out + 0x07;
	}

	// Add offset to convert to Ascii
	ascii_out = ascii_out + 0x30;

	return(ascii_out);
}

//===============================================================
// 02DEC2010	RP	Original Code
//===============================================================

void
UartPutBksp(void)
{
	UartPutChar('\b');
	UartPutChar(' ');
	UartPutChar('\b');
}

//===============================================================
// 02DEC2010	RP	Original Code
//===============================================================

void
UartPutByte(u08_t abyte)
{
	u08_t	temp1 = 0, temp2 = 0;

	temp1 = (abyte >> 4) & 0x0F;			// get high nibble
	temp2 = UartNibble2Ascii(temp1);		// convert to ASCII
	UartPutChar(temp2);						// output */

	temp1 = abyte & 0x0F;					// get low nibble
	temp2 = UartNibble2Ascii(temp1);		// convert to ASCII
	UartPutChar(temp2);						// output
}

//===============================================================
// 02DEC2010	RP	Original Code
//===============================================================

void
UartPutChar(u08_t tx_char)
{
	while(!(IFG2 & UCA0TXIFG))				// till UART Transmit buffer is empty
	{
	}

	UCA0TXBUF = tx_char;						// send the character
}

//===============================================================
// 02DEC2010	RP	Original Code
//===============================================================

void
UartPutCrlf(void)
{
	UartPutChar('\r');
	UartPutChar('\n');
}

//===============================================================
// 02DEC2010	RP	Original Code
//===============================================================

void
UartPutSpace(void)
{
	UartPutChar(' ');
}

//===============================================================
// Send a specified number of bytes from buffer to host         ;
// 02DEC2010	RP	Original Code
//===============================================================

void
UartResponse(u08_t *pbuf, u08_t length)
{
	while(length > 0)
	{	
		UartPutChar('[');
		UartPutByte(*pbuf);
		UartPutChar(']');
		pbuf++;
		length--;
	}
	UartPutCrlf();
}

//===============================================================
//	Common Interrupt RX Vector for USCIA - UART & USCIB - SPI   ;
// 02DEC2010	RP	Original Code
//===============================================================

#pragma vector = USCIAB0RX_VECTOR
__interrupt void
UartRxHandler (void)
{
   if (IFG2 & UCA0RXIFG)					// UART
   {
   		rx_data = UCA0RXBUF;
		rx_done = 1;
		irq_flag = 0x01;
		if(enable == 0)						// If Enable == 0 settings must be done
		{	
			TRF_ENABLE;
			//UartBaudSet(0x01);
			//McuOscSel(0x01);
			Trf796xInitialSettings();
			UartSendCString("Reader enabled.");
			enable = 1;
		}
		//__low_power_mode_off_on_exit();

		if(first_spi_data == 1)
		{	
			IRQ_OFF;

			#ifdef ENABLE_HOST
				host_control_flag = 1;			// shows, that host was detected
											// system in remote mode
											
				LED_ALL_OFF;
				LED_POWER_ON;
			#endif
			IFG2 |= ~UCA0RXIFG;
		}
   }
}

//===============================================================
// Send a character string to USART                             ;
// 02DEC2010	RP	Original Code
//===============================================================

void
UartSendCString(char *pstr)
{
	while(*pstr != '\0')
	{	
		UartPutChar(*pstr++);
	}
}

//===============================================================
// communication with host via USCI_A0                          ;
// 02DEC2010	RP	Original Code
//===============================================================

void
UartSetup (void)							// uses USCI_A0
{
	P3SEL |= BIT4 + BIT5;					// P3.4, P3.5 - UART mode
	P3DIR |= BIT4;							// P3.4 - output direction

	UCA0CTL1 |= UCSWRST;					// disable UART

	UCA0CTL0 = 0x00;

	UCA0CTL1 |= UCSSEL_2;      				// SMCLK
	UCA0BR0 = BAUD0;            			// 115200
	UCA0BR1 = BAUD1;
	UCA0MCTL = 0;               			// Modulation UCBRSx = 5
	UCA0CTL1 &= ~UCSWRST;       			// **Initialize USCI state machine**
	IE2 |= UCA0RXIE;            			// Enable USCI_A0 RX interrupt
}
