//***************************************************************
//------------<02.Dec.2010 by Peter Reiser>----------------------
//***************************************************************

#ifndef _MY_D_H_
#define _MY_D_H_

//================================================================

#include <MSP430x23x0.h> 					// prozessor spezific header
//#include <stdio.h>							// standard input/output header
#include "mcu.h"
#include "trf797x.h"
#include "types.h"
#include "uart.h"

//===============================================================

// if disabled file my-d.c may be excluded from build
#define TYPE2_ENABLE							// should only be enabled for TRF7960A

//===============================================================

void Type2Command(u08_t *pbuf);

//===============================================================

#endif
