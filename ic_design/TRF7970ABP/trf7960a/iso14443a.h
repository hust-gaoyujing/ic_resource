//***************************************************************
//------------<02.Dec.2010 by Peter Reiser>----------------------
//***************************************************************

#ifndef _ISO14443A_H_
#define _ISO14443A_H_

//================================================================

#include <MSP430x23x0.h> 					// prozessor spezific header
//#include <stdio.h>							// standard input/output header
#include "mcu.h"
#include "trf796x.h"
#include "types.h"
#include "uart.h"

//===============================================================

// if disabled file ISO14443A.c may be excluded from build
#define	ENABLE14443A						// delete to disable standard

//===============================================================

void Iso14443aAnticollision(u08_t reqa);
void Iso14443aFindTag(void);
void Iso14443aLoop(u08_t select, u08_t nvb, u08_t *UID);
u08_t Iso14443aSelectCommand(u08_t select, u08_t *UID);

//===============================================================

#endif
