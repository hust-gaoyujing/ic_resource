#include "felica.h"

extern u08_t	buf[300];
extern u08_t	i_reg;
extern u08_t	irq_flag;
extern u08_t	stand_alone_flag;
extern s08_t	rxtx_state;

void
FindFelica(void)
{
	Trf797xTurnRfOn();
	
	Trf797xWriteIsoControl(0x1A);
	
	McuDelayMillisecond(6);

	FelicaPolling(0x00);						// do a complete anticollision sequence as described

	// in ISO14443-3 standard for type A
	Trf797xTurnRfOff();

	Trf797xResetIrqStatus();

}

//------------------------------------------------------//
//The Polling command can use any number of slots	//
//between 1 and 16 (00h - 0fh) witch is specified in the//
//'slotNo' variable.					//
//The slot time Ts is defined as (256 x 64 / fc) and the//
//delay Td between the Polling command and the first 	//
//timeslot is defined as (512 x 64 / fc).		//
//Td = 2.42ms = 2051 = 0x0803 for CounterSet()		//
//Ts = 1.21ms = 1025 = 0x0401 for CounterSet()		//
//------------------------------------------------------//
void
FelicaPolling(u08_t slot_no)
{
	u08_t i, j, command;
	
	IRQ_CLR;			//PORT2 interrupt flag clear
	
	buf[0] = 0x8f;
	buf[1] = 0x91;		//send with CRC
	buf[2] = 0x3d;		//write continous from 1D
	buf[3] = 0x00;		
	buf[4] = 0x60;	
	buf[5] = 0x06;
	buf[6] = POLLING;	//anticollision command code
	buf[7] = 0xFF;
	buf[8] = 0xFF;
	buf[9] = 0x00;
	buf[10] = slot_no;
	
	i_reg = 0x01;
	Trf797xRawWrite(&buf[0], 11);	//writing to FIFO
	IRQ_ON;

	j = 0;
	while((i_reg == 0x01) && (j < 2))
	{
		j++;
		McuCounterSet();
		COUNT_VALUE = COUNT_1ms * 10;//10ms for TIMEOUT
		START_COUNTER;		//start timer up mode
		irq_flag = 0x00;
		while(irq_flag == 0x00)
		{
        }
	}
        command = ADJUST_GAIN;
        Trf797xDirectCommand(&command);
	
	for(i = 0; i < slot_no + 1; i++){
		rxtx_state = 1;		//prepare the global counter		
			
		i_reg = 0x01;
                j = 0;

		while((i_reg == 0x01) && (j < 2))
		{
			j++;
			McuCounterSet();		//TimerA set
			if(i == 0)
			{
				COUNT_VALUE = 0x0bfe;//2.42ms + 1.21ms (0x0803) 0x0b00;
			}
			else
			{
				COUNT_VALUE = 0x03fd;//1.21ms  (0x0401) 0x0270;
			}
			START_COUNTER;	
			irq_flag = 0x00;
			while(irq_flag == 0x00)
			{
			}
		} 	//wait for response or timer interrupt
		
		if(stand_alone_flag == 0)
		{
			if(i_reg == 0xFF)			//recieved UID in buffer
			{
				#ifdef ENABLE_HOST
					UartPutChar('[');
					for(j = 1; j < rxtx_state; j++)
					{
						UartPutByte(buf[j]);
					}//for
					UartPutChar(']');
				#endif
			}
			else if(i_reg == 0x02)		//collision occured
			{
				#ifdef ENABLE_HOST
					UartPutChar('[');
					UartPutChar('z');
					UartPutChar(']');
				#endif
			}
			else if(i_reg == 0x00)		//timer interrupt
			{
				#ifdef ENABLE_HOST
					UartPutChar('[');
					UartPutChar(']');
				#endif
			}else;
		}
		else
		{
			if(i_reg == 0xFF)
			{
				LED_OPEN1_ON;
			}
			else
			{
				LED_OPEN1_OFF;
			}
		}
		
        Trf797xStopDecoders();
		Trf797xReset();			//FIFO has to be reset before recieving the next response
		Trf797xRunDecoders();
	}//for
        IRQ_OFF;
}//FelicaPolling

