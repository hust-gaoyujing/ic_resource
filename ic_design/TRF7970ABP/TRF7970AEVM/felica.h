//***************************************************************
//------------<02.Dec.2010 by Peter Reiser>----------------------
//***************************************************************

#ifndef _FELICA_H_
#define _FELICA_H_

//===============================================================

//#include <MSP430x23x0.h> 					// prozessor spezific header
//#include <stdio.h>							// standard input/output header
#include "mcu.h"
#include "trf797x.h"
#include "types.h"
#include "uart.h"

//===============================================================

#define	ENABLE_FELICA

//===============================================================

#define POLLING	0x00						// polling command

//===============================================================

void FelicaPolling(u08_t slot_no);
void FindFelica(void);

//===============================================================

#endif
