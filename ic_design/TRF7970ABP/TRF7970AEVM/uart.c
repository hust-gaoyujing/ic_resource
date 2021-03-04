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
* EDITED BY: JD Crutchfield 	10 JUN 2013
* *
*
****************************************************************/

#include "uart.h"
#include "trf797x.h"
#include "string.h"

//===============================================================

#if TRIGGER
	extern u08_t trigger;
#endif

#define RX_MAX_BUFF_SIZE 100

//u08_t rx_data[0]_1[20] = 0x00; // = 0xFF;
u08_t rx_data[RX_MAX_BUFF_SIZE];						// RS232 RX data byte
u08_t Write_Pointer = 0;
u08_t Read_Pointer = 0;
u08_t Start_Command_Addr = 0;
u08_t Command_Length = 0;
u08_t Command_Length_H = 0;
u08_t Command_Length_L = 0;


u08_t temp_rx_buff[40];					// Need temp buffer to prevent overwriting
u08_t rx_data_length = 40;
u08_t temp_rx_buff_length = 0;
u08_t rx_data_count = 0;
u08_t rx_done = 0;
u08_t first_UART_RX = 1;
extern u08_t i_reg;
extern u08_t irq_flag;
extern u08_t remote_flag;
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
	u08_t	i = 0;
	u08_t	temp = 0;

	//somehow where getting off by 2 bytes.  I think the issue is that somehow RX_data_count is getting off, (lagging by 2 bytes.)

//	while(rx_data_count < rx_data_length){};  	// Wait for entire packet to be Received.
//
//	temp_rx_buff_length = rx_data_length;
//	memcpy(temp_rx_buff, rx_data, temp_rx_buff_length);  	// copying to temp buffer because RX_data may be overwritten
//															// if another UART command is received before this function ends.
//	while(!UartRxReady){};
	UartRxReady();
	first_UART_RX = 0;		// flag
//	rx_data_count = 0;		// reset for UART RX ISR

	while(rx_data[Read_Pointer] != '0'){}; 		// Confirming correct SOF '0' then '1'
	Read_Pointer++;
	//UartPutChar(rx_data[0]);
	if(Read_Pointer >= RX_MAX_BUFF_SIZE){	// End of Buffer
		Read_Pointer = 0; // Loop back to begining
	}

	while(rx_data[Read_Pointer] != '1'){};
	Read_Pointer++;

	if(Read_Pointer >= RX_MAX_BUFF_SIZE){	// End of Buffer
		Read_Pointer = 0; // Loop back to begining
	}

//	UartPutCrlf();  // start new line

	rx_done = 0;

	pos = 0;								// character position counter - 8bit
	reading = 1;							// flag: reading not yet finished

	for(i=2; i<Command_Length; i ++){		// interating through RX buffer

		if(reading == 0){
			break;
		}
		// sync, wakeup by irq
		rx_done = 0;
		temp = rx_data[Read_Pointer];  		// Preserving Buffer for debugging
		switch(temp){						// process RETURN key
			case '\r':
				__no_operation();
				break;						// ignore CR

			case '\n':
				reading = 0;				// exit read loop
				break;

				// backspace
			case '\b':
				if(pos > 0){					// is there a char to delete?
					pos--;					// remove it from buffer
					UartPutBksp();			// go back and erase on screen
					if(pos & 0x01 > 0){		//(high) even byte
						*pline--;
						*pline &= 0xF0;		// clear lo nibble
					}
				}
				break;

				// other characters
			default:
				if(temp >= 'a')
				{
					temp -= 32;
				}							// change to uppercase

				// discard if not hex
				if((temp < '0') || ((temp > '9') && (temp < 'A')) || (temp > 'F'))
				{
					break;
				}

				// only store characters if buffer has space
				if(pos++ < 2 * 300)
				{
//					UartPutChar(rx_data[i]);	// echo
					if(temp > '9')		// If ASCII A-F then add 9
					{
						temp = (temp & 0x0F) + 9;
					}

					if((pos & 0x01) == 0)	// (low) odd nibble
					{
						*pline += (temp & 0x0F);	// store
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
					else{							// (high) even nibbl
						temp = temp << 4;		// Move to high nibble
						*pline = temp;			// store
					}
				}
				else{
					err_flg = 1;
				}
				break;
		}
		Read_Pointer++;
   		if(Read_Pointer >= RX_MAX_BUFF_SIZE){	// End of Buffer
   			Read_Pointer = 0; // Loop back to begining
   		}
	} // end of for loop

	rx_data_count = 0;  // reset data pointer

	return(err_flg);								// normal exit
}													// UartGetLine


u08_t UartRxReady(){
	u08_t Ready = 0;

	u08_t Gui_P2P_workaround_flag = 0;   //WORKAROUND

	do{    //WORKAROUND
		Gui_P2P_workaround_flag = 0;   //WORKAROUND

		Start_Command_Addr = Read_Pointer;

		while(Start_Command_Addr == Write_Pointer);  			// No Command Received.  Wait.

		if( !((Start_Command_Addr+1) >= RX_MAX_BUFF_SIZE) ){  // Finding Packet Length, waiting on 2nd character
			while((Start_Command_Addr+1) == Write_Pointer);

			Gui_P2P_workaround_flag = ~rx_data[Start_Command_Addr+1];  //WORKAROUND
		}
		else{
			while((Start_Command_Addr+1)-RX_MAX_BUFF_SIZE == Write_Pointer);

			Gui_P2P_workaround_flag = ~rx_data[Start_Command_Addr+1 -RX_MAX_BUFF_SIZE];  //WORKAROUND
		}


		if( !((Start_Command_Addr+2) >= RX_MAX_BUFF_SIZE) ){
			while((Start_Command_Addr+2) == Write_Pointer);

			Command_Length_H = rx_data[Start_Command_Addr+2];
		}
		else{
			while((Start_Command_Addr+2)-RX_MAX_BUFF_SIZE == Write_Pointer);

			Command_Length_H = rx_data[Start_Command_Addr+2 -RX_MAX_BUFF_SIZE];
		}

		if(Gui_P2P_workaround_flag && (Command_Length_H == 'F')){   //WORKAROUND)
			Read_Pointer += 3;
			UartPutChar('0');
			if(Read_Pointer > RX_MAX_BUFF_SIZE){
				Read_Pointer -= RX_MAX_BUFF_SIZE;
			}
		}
	}while((Gui_P2P_workaround_flag) && (Command_Length_H == 'F'));  // trying to handle 00F send by P2P tab.


	if( !((Start_Command_Addr+3) >= RX_MAX_BUFF_SIZE) ){
		while((Start_Command_Addr+3) == Write_Pointer);

		Command_Length_L = rx_data[Start_Command_Addr+3];
	}
	else{
		while((Start_Command_Addr+3)-RX_MAX_BUFF_SIZE == Write_Pointer);

		Command_Length_L = rx_data[Start_Command_Addr+3 -RX_MAX_BUFF_SIZE];
	}

	// Calculate Length
	if(Command_Length_L > '9'){						// If ASCII A-F then add 9
		Command_Length_L = Command_Length_L + 9;
	}

	if(Command_Length_H > '9'){						// If ASCII A-F then add 9
		Command_Length_H = Command_Length_H + 9;
	}

	Command_Length = (((Command_Length_H << 4) & 0xF0) + (Command_Length_L & 0x0F)) *2;	// twice as many actual bytes


	// Waiting to receive entire command.
	if((Start_Command_Addr + Command_Length) < RX_MAX_BUFF_SIZE){  // command doesn't wrap
		if(Start_Command_Addr < Write_Pointer){					// checking if the Write Pointer has already wrapped, if so, command already ready
			while(Write_Pointer < (Start_Command_Addr + Command_Length)){};
		}
	}
	else{	//for wrap when at end of buffer
		while(Start_Command_Addr < Write_Pointer){}; // Wait for Write_Pointer to wrap
		while(Write_Pointer < (Start_Command_Addr + Command_Length - RX_MAX_BUFF_SIZE)){};
	}

	return Ready;
}
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

//void
//UartPutSpace(void)
//{
//	UartPutChar(' ');
//}

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
	if (IFG2 & UCA0RXIFG)					// UART RX?
	{
   		rx_data[Write_Pointer] = UCA0RXBUF;
//   		UartPutChar(rx_data[Write_Pointer]);	// echo
   		rx_done = 1;

   		Write_Pointer++;
   		if(Write_Pointer == RX_MAX_BUFF_SIZE){	// End of Buffer
   			Write_Pointer = 0; // Loop back to begining
   		}

		irq_flag = 0x01;
		if(enable == 0)						// If Enable == 0 settings must be done
		{	
			TRF_ENABLE;
			Trf797xInitialSettings();
			enable = 1;
		}

		if(first_UART_RX == 1)
		{	
			IRQ_OFF;

			#ifdef ENABLE_HOST
				remote_flag = 1;			// shows, that host was detected
											// system in remote mode
				LED_ALL_OFF;
				LED_POWER_ON;
			#endif

		}
		IFG2 |= ~UCA0RXIFG;
	}
}

//===============================================================
// Send a character string to USART
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
