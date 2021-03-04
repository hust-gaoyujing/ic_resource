//***************************************************************
//------------<02.Dec.2010 by Peter Reiser>----------------------
//***************************************************************

#ifndef _SPI_H_
#define _SPI_H_

//================================================================

#include <MSP430x23x0.h> 					// prozessor spezific header
//#include <stdio.h>							// standard input/output header
#include "mcu.h"
#include "types.h"

//===============================================================

void SpiDirectCommand(u08_t *pbuf);
void SpiDirectMode(void);
void SpiRawWrite(u08_t *pbuf, u08_t length);
void SpiReadCont(u08_t *pbuf, u08_t length);
void SpiReadSingle(u08_t *pbuf, u08_t length);
void SpiSetup(void);
void SpiUsciExtClkSet(void);
void SpiWriteCont(u08_t *pbuf, u08_t length);
void SpiWriteSingle(u08_t *pbuf, u08_t length);

//===============================================================

#endif
