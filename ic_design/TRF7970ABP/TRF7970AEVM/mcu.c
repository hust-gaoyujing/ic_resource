/****************************************************************
* FILENAME: mcu.c
*
* BRIEF: Selects MCU specific functions.
*
* Copyright (C) 2010 Texas Instruments, Inc.
*
* AUTHOR(S): Reiser Peter		DATE: 27 JAN 2010
*
* EDITED BY:
* *
*
****************************************************************/

#include "mcu.h"

void 
McuCounterSet(void)
{
	#ifdef MSP430F23X0
		Msp430f23x0CounterSet();
	#endif
}

void 
McuDelayMillisecond(u32_t n_ms)
{
	#ifdef MSP430F23X0
		Msp430f23x0DelayMillisecond(n_ms);
	#endif
}

void 
McuOscSel(u08_t mode)
{
	#ifdef MSP430F23X0
		Msp430f23x0OscSel(mode);
	#endif
}
