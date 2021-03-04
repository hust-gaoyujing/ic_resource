#ifndef _TARGET_H_
#define _TARGET_H_

//#include <stdio.h>							// standard input/output header
#include "mcu.h"
#include "nfc.h"
#include "trf797x.h"
#include "types.h"
#include "uart.h"

void TargetAnticollision(u08_t *pbuf, u08_t tag);

#endif
