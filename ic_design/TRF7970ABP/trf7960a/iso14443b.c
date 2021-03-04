/****************************************************************
* FILENAME: iso14443b.c
*
* BRIEF: Contain functions to search ISO14443B standardized tags
* in stand alone mode and execute ISO14443B Anticollision in
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


#include "iso14443b.h"

//===============================================================

extern u08_t buf[300];
extern u08_t i_reg;
extern u08_t irq_flag;
extern u08_t rx_error_flag;
extern s08_t rxtx_state;
extern u08_t stand_alone_flag;
extern u08_t host_control_flag;

//===============================================================

void iso14443bSlotMarkerCommand(u08_t number);

//===============================================================
// NAME: void Iso14443aFindTag(void)
//
// BRIEF: Is used to detect ISO14443B conform tags in stand alone 
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
// NOTE: If ISO14443B conform Tag is detected, ISO14443B LED will
//       be turned on.
//
// CHANGE:
// DATE  		WHO	DETAIL
// 23Nov2010	RP	Original Code
//===============================================================

void
Iso14443bFindTag(void)
{	
	Trf796xTurnRfOn();
	
	Trf796xWriteIsoControl(0x0C);
	
	// When a PICC is exposed to an unmodulated operating field
	// it shall be able to accept a quest within 5 ms.
	// PCDs should periodically present an unmodulated field of at least
	// 5,1 ms duration. (ISO14443-3)
	McuDelayMillisecond(6);

	iso14443bAnticollision(0xB0, 0x04); 				// 16 slots (0xB0, 0x00) for 16 slots

	Trf796xTurnRfOff();

	Trf796xResetIrqStatus();
}

//===============================================================
// NAME: void Iso14443bAnticollision(u08_t command, u08_t slots)
//
// BRIEF: Is used to perform ISO14443B anticollision sequence.
//
// INPUTS:
//	Parameters:
//		U08_t		command		REQB or WUPB
//		u08_t		slots		number of slots
//	
// OUTPUTS:
//
// PROCESS:	[1] PCD sends REQB with number of slots N
//			[2] PICC generates random number R from 1 to N
//			[3] PCD sends Slot-Marker command during every
//			    timeslot
//			[4] PICC respons only if R matches N
//			[5] When multiple PICCs respond, PCD makes a note of
//			    collision and generates a new N. Steps 1 to 4
//			    will be repeated.
//
// NOTE: Collisions returned as “[z]”.
//       Timeouts returned as “[]”.
//
// CHANGE:
// DATE  		WHO	DETAIL
// 23Nov2010	RP	Original Code
//===============================================================

void
iso14443bAnticollision(u08_t command, u08_t slots)
{
	u08_t	i = 0, collision = 0x00, j, found = 0;
	u32_t	k = 0;
	#ifdef ENABLE_HOST
		//u08_t rssi[2];
	#endif
	
	rx_error_flag = 0x00;

	buf[0] = 0x8F;
	buf[1] = 0x91;
	buf[2] = 0x3D;
	buf[3] = 0x00;
	buf[4] = 0x30;
	buf[5] = 0x05;
	buf[6] = 0x00;

	if(slots == 0x04)
	{	
		Trf796xEnableSlotCounter();
	}

	buf[7] = slots;

	if(command == 0xB1)
	{	
		buf[7] |= 0x08; 					// WUPB command else REQB command
	}

	i_reg = 0x01;

	Trf796xRawWrite(&buf[0], 8);

	IRQ_CLR;								// PORT2 interrupt flag clear
	IRQ_ON;

	j = 0;
	while((i_reg == 0x01) && (j < 2))
	{
		j++;
		McuCounterSet();					// TimerA set
		COUNT_VALUE = COUNT_1ms * 2;		// 2ms
		START_COUNTER;
		irq_flag = 0x00;
		while(irq_flag == 0x00)
		{
		}
	}										// wait for end of TX

	i_reg = 0x01;
	
	McuCounterSet();						// TimerA set
	COUNT_VALUE = COUNT_1ms * 2;			// 2ms
	START_COUNTER;

	for(i = 1; i < 17; i++)
	{	
		rxtx_state = 1;						// the response will be stored in buf[1] upwards

		while(i_reg == 0x01)				// wait for RX complete
		{	
			k++;
			if(k == 0xFFF0)
			{	
				i_reg = 0x00;
				rx_error_flag = 0x00;
				break;
			}
		}

		if(rx_error_flag == 0x02)
		{	
			i_reg = rx_error_flag;
		}
		
		if(i_reg == 0xFF)					// recieved SID in buffer
		{	
			if(stand_alone_flag == 1)
			{	
				found = 1;
			}
			else
			{	
				#ifdef ENABLE_HOST
				
					UartPutChar('[');
					for(j = 1; j < rxtx_state; j++)
					{	
						UartPutByte(buf[j]);
					}
					UartPutChar(']');

					//UartPutChar('(');
					//UartPutByte(rssi[0]);
					//UartPutChar(')');
					UartPutCrlf();
				#endif
			}
		}
		else if(i_reg == 0x02)				// collision occured
		{	
			if(stand_alone_flag == 0)
			{	
				#ifdef ENABLE_HOST
					UartPutChar('[');
					UartPutChar('z');
					UartPutChar(']');
				#endif
			}
			collision = 0x01;
		}
		else if(i_reg == 0x00)				// slot timeout
		{	
			if(stand_alone_flag == 0)
			{	
				#ifdef ENABLE_HOST
					//UartPutChar('[');
					//UartPutChar(']');
				#endif
			}
		}
		else
		{
		}

		if((slots == 0x00) || (slots == 0x01) || (slots == 0x02) || ((slots == 0x04) && (i == 16)))
		{	
			break;
		}

		iso14443bSlotMarkerCommand(i);

		i_reg = 0x01;

		if(stand_alone_flag == 0)
		{	
			#ifdef ENABLE_HOST
				//UartPutCrlf();
			#endif
		}
	}										// for

	if(slots == 0x04)
	{	
		Trf796xDisableSlotCounter();
	}

	IRQ_OFF;
	
	if(host_control_flag == 0)
	{
		if(found == 1)
		{	
			LED_14443B_ON;
		}
		else
		{	
			LED_14443B_OFF;
		}
	}

	if(collision)
	{	
		iso14443bAnticollision(0x20, 0x02);	// Call this function for 16 timeslots
	}
}											// iso14443bAnticollision

//===============================================================
// NAME: void Iso14443bSlotMarkerCommand(u08_t number)
//
// BRIEF: Is used to execute ISO14443B Slot-Marker command.
//
// INPUTS:
//	Parameters:
//		U08_t	number		number of slots
//	
// OUTPUTS:
//
// PROCESS:	[1] Execute slot marker command
//
// CHANGE:
// DATE  		WHO	DETAIL
// 23Nov2010	RP	Original Code
//===============================================================

void
iso14443bSlotMarkerCommand(u08_t number)
{
	//This method shows the carrier modulation

	buf[0] = 0x8f;
	buf[1] = 0x91;
	buf[2] = 0x3D;
	buf[3] = 0x00;
	buf[4] = 0x10;
	Trf796xRawWrite(&buf[0], 5);

	buf[5] = 0x3F;
	buf[6] = (number << 4) | 0x05;
	buf[7] = 0x00;
	
	i_reg = 0x01;
	
	Trf796xRawWrite(&buf[5], 3);

	IRQ_CLR;									//PORT2 interrupt flag clear
	IRQ_ON;


	while(i_reg == 0x01)
	{	
		McuCounterSet();					// TimerA set
		COUNT_VALUE = COUNT_1ms * 10;		// 20ms
		START_COUNTER;
		irq_flag = 0x00;
		while(irq_flag == 0x00)
		{
		}									// wait for interrupt
	}
}

