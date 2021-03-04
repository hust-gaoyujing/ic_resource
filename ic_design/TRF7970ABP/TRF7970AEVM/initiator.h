#ifndef _INITIATOR_H_
#define _INITIATOR_H_

//#include <stdio.h>							// standard input/output header
#include "mcu.h"
#include "nfc.h"
#include "trf797x.h"
#include "types.h"
#include "uart.h"

void InitiatorRequest(u08_t *pbuf);
u08_t InitiatorDepRequest(u08_t *pbuf);

#endif /*INITIATOR_H_*/
