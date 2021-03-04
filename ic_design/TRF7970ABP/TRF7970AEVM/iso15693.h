//***************************************************************
//------------<02.Dec.2010 by Peter Reiser>----------------------
//***************************************************************

#ifndef _ISO15693_H_
#define _ISO15693_H_

//================================================================

//#include <MSP430x23x0.h> 					// prozessor spezific header
//#include <stdio.h>							// standard input/output header
#include "mcu.h"
#include "trf797x.h"
#include "types.h"
#include "uart.h"

//===============================================================

#define SIXTEEN_SLOTS	0x06
#define ONE_SLOT		0x26

//===============================================================

// if disabled file ISO15693.c may be excluded from build
#define	ENABLE15693							// delete to disable standard

//===============================================================

void Iso15693FindTag(void);
void Iso15693Anticollision(u08_t *mask, u08_t length);

//===============================================================

#endif
