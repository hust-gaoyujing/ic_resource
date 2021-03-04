#include "highdata.h"
#include <MSP430x23x0.h>

u08_t	direct_mode = 0;
extern u08_t buf[300];
extern u08_t irq_flag;

void
HighDataCommunication(u08_t *pbuf)
{
	u08_t	bit, byte;
	
	TRF_DIR_IN;
	//DIRECT_ON;
	MOD_SET;
	OOK_DIR_OUT;
	OOK_ON;		
	direct_mode = 1;
	
	McuDelayMillisecond(1);
	
	/*buf[0] = CHIP_STATE_CONTROL;
	buf[1] = 0x00;
	buf[1] |= BIT6;
	Trf796xWriteSingle(buf, 2);*/

	buf[0] = ISO_CONTROL;
	Trf797xReadSingle(buf, 1);
	//buf[0] = 0x07 + *pbuf;
	buf[0] = 0x02;
	
	Trf797xWriteIsoControl(buf[0]);			// set datarate, direct mode 1
	buf[0] = 0x01;
	Trf797xReadSingle(buf, 1);
	//Trf796xTurnRfOn();
	
	buf[0] = MODULATOR_CONTROL;
	//buf[1] = MODULATOR_CONTROL;
	//Trf796xReadSingle(&buf[1], 1);
	buf[1] = 0x21;
	Trf797xWriteSingle(buf, 2);				// set clock to 13.56 MHz, direct mode
	
	buf[0] = MODULATOR_CONTROL;
	Trf797xReadSingle(buf, 1);
	
	McuDelayMillisecond(1);
	
	buf[0] = CHIP_STATE_CONTROL;
	buf[1] = 0x61; 
	Trf797xWriteSingle(buf, 2);
	
	buf[0] = 0x00;
	Trf797xReadSingle(buf, 1);

	
	for(byte=0; byte<*(pbuf+1); byte++)
	{
		for(bit=0; bit<8; bit++)
		{
			//irq_flag = 0xFF;
			//while(irq_flag == 0xFF);		// wait for interrupt
			//{
			//}
			if((*(pbuf + 2 + byte) & BIT7) == BIT7)
			{
				MOD_ON;
			}
			else
			{
				MOD_OFF;
			}

			*(pbuf + 2 + byte) <<= 1;
		}
	}

	//DIRECT_OFF;
	
	/*Trf797xTurnRfOff();*/
	
	/*buf[0] = MODULATOR_CONTROL;
	buf[1] = MODULATOR_CONTROL;
	Trf797xReadSingle(&buf[1], 1);
	buf[1] = 0x01;
	Trf797xWriteSingle(buf, 2);				// set clock to 6.78 MHz, direct mode
	McuDelayMillisecond(1);*/
	
	buf[0] = CHIP_STATE_CONTROL;
	buf[1] = 0x41;
	Trf797xWriteSingle(buf, 2);
	
	buf[0] = 0x00;
	Trf797xReadSingle(buf, 1);
	
	direct_mode = 0;
	
	UartPutChar('[');
	UartPutChar(']');
}

/*#pragma vector = PORT4_VECTOR
__interrupt void HighDataDirectClock (void)
{
	irq_flag++;
	DIRECT_CLR;
}*/
