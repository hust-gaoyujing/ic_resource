/****************************************************************
* FILENAME: iso14443a.c
*
* BRIEF: Contain functions to search ISO14443A standardized tags
* in stand alone mode and execute ISO14443A Anticollision in
* remote mode.
*
* Copyright (C) 2010 Texas Instruments, Inc.
*
* AUTHOR(S): Reiser Peter		DATE: 02 DEC 2010
*
* EDITED BY:
* *
*
****************************************************************/

#include "iso14443a.h"

//===============================================================

u08_t	complete_uid[14];
u08_t	coll_poss = 0;
extern u08_t buf[300];
extern u08_t i_reg;
extern u08_t irq_flag;
extern u08_t rx_error_flag;
extern s08_t rxtx_state;
extern u08_t stand_alone_flag;
extern u08_t host_control_flag;

//===============================================================
// NAME: void Iso14443aFindTag(void)
//
// BRIEF: Is used to detect ISO14443A conform tags in stand alone 
// mode.
//
// INPUTS:
//	
// OUTPUTS:
//
// PROCESS:	[1] turn on RF driver
//			[2] do a complete anticollision sequence
//			[3] turn off RF driver
//
// NOTE: If ISO14443A conform Tag is detected, ISO14443A LED will
//       be turned on.
//
// CHANGE:
// DATE  		WHO	DETAIL
// 23Nov2010	RP	Original Code
//===============================================================

void
Iso14443aFindTag(void)
{
	Trf796xTurnRfOn();
	
	Trf796xWriteIsoControl(0x08);
	
	// When a PICC is exposed to an unmodulated operating field
	// it shall be able to accept a quest within 5 ms.
	// PCDs should periodically present an unmodulated field of at least
	// 5,1 ms duration. (ISO14443-3)
	McuDelayMillisecond(6);

	Iso14443aAnticollision(0x00);						// do a complete anticollision sequence as described

	// in ISO14443-3 standard for type A
	Trf796xTurnRfOff();

	Trf796xResetIrqStatus();

}

//===============================================================
// NAME: void Iso14443aLoop(u08_t select, u08_t nvb, u08_t
// 			*uid)
//
// BRIEF: Is used to run through the cascade levels.
//
// INPUTS:
//	Parameters:
//		u08_t		select		indicates cascade level
//		u08_t		nvb			number of valid bits
//		u08_t		*uid		known part of UID
//	
// OUTPUTS:
//	Globals:
//		u08_t		complete_uid[14]	stores UID
//
// PROCESS:	(ISO14443-3)
//
// NOTE: Collisions returned as “(z)”.
//       Timeouts returned as “()”.
//
// CHANGE:
// DATE  		WHO	DETAIL
// 23Nov2010	RP	Original Code
//===============================================================

void
Iso14443aLoop(u08_t select, u08_t nvb, u08_t *uid)
{
	u08_t	i = 0, length = 0, new_uid[4], more = 0;
	u08_t	nvbytes = 0, nvbits = 0, xbits = 0, found = 0;
	#ifdef ENABLE_HOST
		u08_t rssi[2];
	#endif


	buf[50] = ISO_CONTROL;					// disable RX CRC calculation
	buf[51] = 0x88;
	Trf796xWriteIsoControl(0x88);

	buf[0] = 0x1A;
	buf[1] = 0x40;
	Trf796xWriteSingle(buf, 2);

	rx_error_flag = 0;
	coll_poss = 0;

	length = 5 + (nvb >> 4);
	if((nvb & 0x0F) != 0x00)
	{
		length++;
		nvbytes = (nvb >> 4) - 2;			// the number of known valid bytes
		xbits = nvb & 0x07;					// the number of known valid bits

		// Both are used in the UID calculation
		for(i = 0; i < xbits; i++)
		{
			nvbits = nvbits << 1;
			nvbits = nvbits + 1;
		}
	}

	buf[0] = 0x8F;							// prepare the SELECT command
	if(nvb == 0x70)							// select command, otherwise anticollision command
	{
		buf[1] = 0x91;						// transmit with CRC
	}
	else
	{
		buf[1] = 0x90;
	}
	buf[2] = 0x3D;
	buf[3] = 0x00;
	buf[4] = nvb & 0xF0;					// number of complete bytes
	if((nvb & 0x07) != 0x00)
	{	
		buf[4] |= ((nvb & 0x07) << 1) + 1; 	// number of broken bits
	}
	buf[5] = select;						// can be 0x93, 0x95 or 0x97
	buf[6] = nvb;							// number of valid bits
	buf[7] = *uid;
	buf[8] = *(uid + 1);
	buf[9] = *(uid + 2);
	buf[10] = *(uid + 3);

	Trf796xRawWrite(&buf[0], length);

	rxtx_state = 1;							// the response will be stored in buf[1] upwards

	i_reg = 0x01;
	while(i_reg != 0x00)
	{
		McuCounterSet();
		COUNT_VALUE = COUNT_1ms * 2;		// 2ms for TIMEOUT
		irq_flag = 0x00;
		START_COUNTER;						// start timer up mode
		while(irq_flag == 0x00)				// wait for interrupt
		{
		}
	}										// wait for end of TX

	i_reg = 0x01;
	i = 0;
	while((i_reg == 0x01) && (i < 2))		// wait for end of RX or timeout
	{
		i++;
		McuCounterSet();
		COUNT_VALUE = COUNT_1ms * 4;		// 4ms for TIMEOUT
		irq_flag = 0x00;
		START_COUNTER;						// start timer up mode
		while(irq_flag == 0x00)
		{
		}									// wait for interrupt
	}

	if(rx_error_flag == 0x02)
	{	
		i_reg = 0x02;
	}

	if(i_reg == 0xff)						// if data received
	{	
		if(stand_alone_flag == 0)
		{	
			#ifdef ENABLE_HOST
				UartPutChar('(');
				for(i = 1; i < 6; i++)
				{	
					UartPutByte(buf[i]);
				}
				UartPutChar(')');
			#endif
		}

		switch(select)
		{
		case 0x93:										// cascade level 1
			if((buf[1] == 0x88) || (*uid == 0x88))		// UID not complete
			{	
				if(nvbytes > 0)
				{	
					for(i = 0; i < 4; i++)
					{
						// Combine known bytes and recieved bytes to a whole UID.
						if(i < (nvbytes - 1)) 
						{	
							complete_uid[i] = *(uid + i + 1);
						}
						// Combine the broken bits to a whole byte.
						else if(i == (nvbytes - 1))
						{	
							complete_uid[i] = (buf[i + 2 - nvbytes] &~nvbits) | (*(uid + i + 1) & nvbits);
						}
						// Add the recieved whole bytes.
						else
						{	
							complete_uid[i] = buf[i + 2 - nvbytes];
						}
					}
				}
				else
				{
					complete_uid[0] = (buf[2] &~nvbits) | (*uid & nvbits);
					for(i = 0; i < 4; i++)
					{	
						complete_uid[i + 1] = buf[i + 3];
					}
				}										// if-else

				buf[1] = 0x88;
				for(i = 0; i < 4; i++)
				{	
					buf[i + 2] = complete_uid[i];
				}

				Iso14443aSelectCommand(select, &buf[1]);
				nvb = 0x20;
				more = 1;
			}
			else
			{											// UID
														// complete;
														// send UID to host
				if(stand_alone_flag == 1)
				{	
					found = 1;
				}
				else
				{	
					#ifdef ENABLE_HOST
					
						rssi[0] = RSSI_LEVELS;			// read RSSI levels
						Trf796xReadSingle(rssi, 1);
					
						UartPutChar('[');
						if(nvbytes > 0)
						{	
							for(i = 0; i < 4; i++)
							{	
								if(i < (nvbytes - 1))	// Combine the known bytes and the
								{	
									UartPutByte(*(uid + i + 1));	// recieved bytes to a whole UID.
								}
								else if(i == (nvbytes - 1))
								{	
									UartPutByte((buf[i + 2 - nvbytes] &~nvbits) | (*(uid + i + 1) & nvbits));
								}
								else
								{	
									UartPutByte(buf[i + 2 - nvbytes]);
								}
							}
						}
						else
						{	
							UartPutByte((buf[1] &~nvbits) | (*uid & nvbits));
							for(i = 0; i < 4; i++)
							{	
								UartPutByte(buf[i + 2]);
							}
						}								// if
						UartPutChar(']');
						UartPutChar('(');
						UartPutByte(rssi[0]);
						UartPutChar(')');
					#endif
				}
			}

			select = 0x95;
			break;

		case 0x95:										// cascade level 2
			if(buf[1] == 0x88)
			{	
				for(i = 0; i < 4; i++)
				{	
					complete_uid[i + 4] = buf[i + 2];
				}

				Iso14443aSelectCommand(select, &buf[1]);
				more = 1;
			}
			else
			{											// UID
														// complete;
														// send UID to host
				for(i = 0; i < 5; i++)
				{	
					complete_uid[i + 4] = buf[i + 1];
				}

				if(stand_alone_flag == 1)
				{	
					found = 1;
				}
				else
				{	
					#ifdef ENABLE_HOST
					
						rssi[0] = RSSI_LEVELS;			// read RSSI levels
						Trf796xReadSingle(rssi, 1);
					
						UartPutChar('[');
						for(i = 0; i < 3; i++)			// send UID level 1
						{	
							UartPutByte(complete_uid[i]);
						}
						UartPutByte(complete_uid[3]);	// send BCC for UID 1
	
						for(i = 4; i < 8; i++)			// send UID level 2
						{	
							UartPutByte(complete_uid[i]);
						}
						UartPutByte(complete_uid[8]);	// send BCC for UID 1
						UartPutChar(']');
						UartPutChar('(');
						UartPutByte(rssi[0]);
						UartPutChar(')');
					#endif
				}
			}

			select = 0x97;
			break;

		case 0x97:										// cascade level 3
			for(i = 0; i < 5; i++)
			{											// UID is
										 				// complete;
														// send UID to host
				complete_uid[i + 8] = buf[i + 1];
			}

			if(stand_alone_flag == 1)
			{	
				found = 1;
			}
			else
			{	
				#ifdef ENABLE_HOST
				
					rssi[0] = RSSI_LEVELS;				// read RSSI levels
					Trf796xReadSingle(rssi, 1);
				
					UartPutChar('[');
					for(i = 0; i < 3; i++)				// send UID level 1
					{	
						UartPutByte(complete_uid[i]);
					}
					UartPutByte(complete_uid[3]);		// send BCC for UID 1
	
					for(i = 4; i < 7; i++)				// send UID level 2
					{	
						UartPutByte(complete_uid[i]);
					}
					UartPutByte(complete_uid[7]);		// send BCC for UID 2
	
					for(i = 8; i < 12; i++)				// send UID level 3
					{	
						UartPutByte(complete_uid[i]);
					}
					UartPutByte(complete_uid[12]);		// send BCC for UID 3
					UartPutChar(',');
					UartPutChar(']');
					UartPutChar('(');
					UartPutByte(rssi[0]);
					UartPutChar(')');
				#endif
			}
			break;
		}												// switch
			
	}
	else if(i_reg == 0x02)								// collision occured
	{	
		if(stand_alone_flag == 0)
		{	
			#ifdef ENABLE_HOST
				UartPutChar('(');
				UartPutChar('z');
				UartPutChar(')');
			#endif
		}
	}
	else if(i_reg == 0x00)							// timer interrupt
	{	
		if(stand_alone_flag == 0)
		{	
			#ifdef ENABLE_HOST
				UartPutChar('(');
				UartPutChar(')');
			#endif
		}
	}
	else
	{
	}

	if(i_reg == 0x02)									// if collision occured go into anticollision
	{	
		coll_poss++;									// reader returns coll_poss - 1
		for(i = 1; i < 5; i++)
		{	
			new_uid[i - 1] = buf[i];					// Copy new UID from local buffer and
		}												// assign number of valid bits to nvb
	
		McuCounterSet();
		COUNT_VALUE = COUNT_1ms * 1.2;					// 1.2ms for TIMEOUT
		START_COUNTER;									// start timer up mode
		i_reg = 0x01;
		while(i_reg == 0x01)
		{
		}												// wait for end of RX or timeout

		Iso14443aLoop(select, coll_poss, new_uid);		// recursive call for anticollision procedure
	}

	if(more == 1)
	{	
		// perform anticollison command for 7 or 10 - byte UID - recursive call for cascade levels
		Iso14443aLoop(select, nvb, uid);				// only the select field is different, everything else is the same
		if(stand_alone_flag == 1)
		{	
			found = 1;
		}
	}
	if(host_control_flag == 0)
	{
		if(found == 1)
		{	
			LED_14443A_ON;
		}
		else
		{	
			LED_14443A_OFF;
		}
	}
}														// Iso14443aLoop

//===============================================================
// NAME: void Iso14443aAnticollision(u08_t reqa)
//
// BRIEF: Is used to start the ISO14443A Anticollision Loop.
//
// INPUTS:
//	Parameters:
//		u08_t		reqa		REQA or WUPA
//	
// OUTPUTS:
//
// PROCESS:	[1] send REQA or WUPA command
//			[2] receive ATQA
//			[3] perform bit frame anticollison loop
//
// NOTE: Collisions returned as “(z)”.
//       Timeouts returned as “()”.
//
// CHANGE:
// DATE  		WHO	DETAIL
// 23Nov2010	RP	Original Code
//===============================================================

void
Iso14443aAnticollision(u08_t reqa)
{
	u08_t	i = 0, select = 0x93, nvb = 0x20;

	buf[0] = ISO_CONTROL;
	buf[1] = 0x88;							// recieve with no CRC
	Trf796xWriteIsoControl(0x88);
	
	buf[0] = 0x1A;
	buf[1] = 0x40;
	Trf796xWriteSingle(buf, 2);

	if(reqa)
	{	
		buf[5] = 0x26; 						// send REQA command
	}
	else
	{	
		buf[5] = 0x52;						// send WUPA command
	}

    buf[0] = 0x8F;
    buf[1] = 0x90;
    buf[2] = 0x3D;
    buf[3] = 0x00;
    buf[4] = 0x0F;

    rxtx_state = 1;
    
    Trf796xRawWrite(&buf[0], 6);
    
    IRQ_CLR;								// PORT2 interrupt flag clear
	IRQ_ON;
	
	i_reg = 0x01;
	
	rxtx_state = 1;
	
	// wait for end of transmit
	while(i_reg == 0x01)
	{	McuCounterSet();
		COUNT_VALUE = COUNT_1ms * 2;		// for 2 ms TIMEOUT
		START_COUNTER;						// start timer up mode
		irq_flag = 0x00;
		while(irq_flag == 0x00)
		{
		}									// wait for interrupt
	}
	
	i_reg = 0x01;
	
	McuCounterSet();
	COUNT_VALUE = COUNT_1ms * 4; 			// for 4 ms TIMEOUT
	START_COUNTER;							// start timer up mode 
	
	while(i_reg == 0x01)					// wait for RX complete
	{
	}
	
	if (i_reg == 0xFF)						// received response
	{
		UartPutChar('(');
		for(i = 1; i < rxtx_state; i++)
		{	
			UartPutByte(buf[i]);
		}
		UartPutChar(')');
	}
	else if (i_reg == 0x02)					// collision occured
	{
		UartPutChar('(');
		UartPutChar('z');
		UartPutChar(')');
	}
	else if (i_reg == 0x00)					// timer interrupt
	{
		UartPutChar('(');
		UartPutChar(')');
	}
	
	IRQ_CLR;								// PORT2 interrupt flag clear
	IRQ_ON;

	if(i_reg == 0xFF || i_reg == 0x02)
	{	for(i = 40; i < 45; i++)
		{	
			buf[i] = 0x00;
		}
		Iso14443aLoop(select, nvb, &buf[40]);
		if(stand_alone_flag == 1)
		{	
			LED_14443A_ON;
		}
	}
	else
	{	
		LED_14443A_OFF;
	}

	buf[0] = ISO_CONTROL;
	buf[1] = 0x08;							// recieve with no CRC
	Trf796xWriteIsoControl(0x08);
	IRQ_OFF;
}											// Iso14443aAnticollision

//===============================================================
// NAME: void Iso14443aSelectCommand(u08_t select, u08_t *uid)
//
// BRIEF: Is used to execute the ISO14442A select commands.
//
// INPUTS:
//	Parameters:
//		U08_t		select	selected cascade level
//		u08_t		*uid		selected uid
//	
// OUTPUTS:
//	Return:
//		Type:		u08_t	Error Code:
//		Values:		0		timeout or collision
//					1		normal response
//
// PROCESS:	[1] send select command
//			[2] receive response
//			[3] send response to host
//
// NOTE: Collisions returned as “[z]”.
//       Timeouts returned as “[]”.
//
// CHANGE:
// DATE  		WHO	DETAIL
// 23Nov2010	RP	Original Code
//===============================================================

u08_t
Iso14443aSelectCommand(u08_t select, u08_t *uid)
{
	u08_t	j = 0;

	buf[50] = ISO_CONTROL;					// enable RX CRC calculation
	buf[51] = 0x08;
	Trf796xWriteIsoControl(0x08);

	for(j = 0; j < 5; j++)
	{	
		buf[j + 7] = *(uid + j);
	}
	
	buf[0] = 0x8F;
	buf[1] = 0x91;								// buffer setup for FIFO writing
	buf[2] = 0x3D;
	buf[3] = 0x00;
	buf[4] = 0x70;
	buf[5] = select;
	buf[6] = 0x70;

	Trf796xRawWrite(buf, 12);					// send the request using RAW writing

	// Write 12 bytes the first time you write to FIFO
	i_reg = 0x01;
	rxtx_state = 1;								// the response will be stored in buf[1] upwards

	// wait for end of transmit

	while(i_reg == 0x01)
	{
	}

	i_reg = 0x01;

	McuCounterSet();
	COUNT_VALUE = COUNT_1ms * 2;			// 10ms for TIMEOUT
	START_COUNTER;							// start timer up mode

	while(i_reg == 0x01)
	{
	}										// wait for RX complete

	if(stand_alone_flag == 0)
	{	
		#ifdef ENABLE_HOST
			if(i_reg == 0xFF)				// recieved response
			{	
				if((buf[1] & BIT2) == BIT2)	// UID not complete
				{	
					UartPutChar('(');
					for(j = 1; j < rxtx_state; j++)
					{	
						UartPutByte(buf[j]);
					}
					UartPutChar(')');
					return 1;
				}
				else						// UID complete
				{	
					UartPutChar('[');
					for(j = 1; j < rxtx_state; j++)
					{	
						UartPutByte(buf[j]);
					}
	
					UartPutChar(']');
					return 0;
				}
			}
			else if(i_reg == 0x02)			// collision occured
			{	
				UartPutChar('[');
				UartPutChar('z');
				UartPutChar(']');
			}
			else if(i_reg == 0x00)			// timer interrupt
			{	
				UartPutChar('[');
				UartPutChar(']');
			}
			else
			{
			}
		#endif
	}										// end if(stand_alone_flag == 0)
	return 0;
}											// Iso14443aSelectCommand
