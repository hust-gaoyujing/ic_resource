//***************************************************************
//------------<02.Dec.2010 by Peter Reiser>----------------------
//***************************************************************

#ifndef _ISO14443B_H_
#define _ISO14443B_H_

//================================================================

//#include <MSP430x23x0.h> 					// prozessor spezific header
//#include <stdio.h>							// standard input/output header
#include "mcu.h"
#include "trf797x.h"
#include "types.h"
#include "uart.h"

//===============================================================

// if disabled file ISO14443B.c may be excluded from build
#define	ENABLE14443B						// delete to disable standard

//===============================================================

// change by Long ---------------

//void iso14443bAnticollision(u08_t command, u08_t slots);
//void Iso14443bFindTag(void);
void iso14443bAnticollision(u08_t command, u08_t n);
void Iso14443bFindTag(void);
u08_t Iso14443b_halt(u08_t *pupi); 		// end of change by Long ------

//===============================================================

#endif
