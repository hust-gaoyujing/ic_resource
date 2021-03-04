//***************************************************************
//------------<02.Dec.2010 by Peter Reiser>----------------------
//***************************************************************

#ifndef _ISO14443A_H_
#define _ISO14443A_H_

//================================================================

//#include <MSP430x23x0.h> 					// prozessor spezific header
//#include <stdio.h>							// standard input/output header
#include "mcu.h"
#include "trf797x.h"
#include "types.h"
#include "uart.h"

//===============================================================

// if disabled file ISO14443A.c may be excluded from build
#define	ENABLE14443A						// delete to disable standard

// command
#define REQA	0x26
#define WUPA	0x52

#define NVB_INIT 0x20
#define NVB_FULL 0x70
#define LEVEL1	0x93
#define LEVEL2	0x95
#define LEVEL3	0x97
#define RX_CRC		0x08
#define NO_RX_CRC	0x88
//===============================================================

void Iso14443aAnticollision(u08_t reqa);
void Iso14443aFindTag(void);
void Iso14443aLoop(u08_t select, u08_t nvb, u08_t *UID);

//change by Long ---------------
//u08_t Iso14443aSelectCommand(u08_t select, u08_t *UID);
void Iso14443a_command(u08_t command);
void Iso14443a_halt();
u08_t Iso14443aSelectCommand(u08_t select, u08_t nvb, u08_t *uid);
void Iso14443_config(u08_t crc);
void ISO14443IRQWaitTimeout(u08_t txtimeout, u08_t rxtimeout);
// end of change by Long ----------------------------------------

//===============================================================

#endif
