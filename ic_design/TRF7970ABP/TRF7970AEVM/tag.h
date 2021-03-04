//***************************************************************
//------------<02.Dec.2010 by Peter Reiser>----------------------
//***************************************************************

#ifndef _TAG_H_
#define _TAG_H_

//===============================================================

//#include <MSP430x23x0.h> 					// prozessor spezific header
//#include <stdio.h>							// standard input/output header
#include "mcu.h"
#include "nfc.h"
#include "trf797x.h"
#include "types.h"
#include "uart.h"
#include "nfc.h"

//===============================================================

void TagEmulation(u08_t *pbuf);

//===============================================================

#endif
