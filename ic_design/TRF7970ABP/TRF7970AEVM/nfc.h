//***************************************************************
//------------<02.Dec.2010 by Peter Reiser>----------------------
//***************************************************************

#ifndef _NFC_H_
#define _NFC_H_

//#include <MSP430x23x0.h> 					// prozessor spezific header
//#include <stdio.h>							// standard input/output header
#include "mcu.h"
#include "trf797x.h"
#include "types.h"
#include "uart.h"

#define RES			0xD5
#define	ATR			0x01
#define WUP			0x03
#define PSL			0x05
#define DEP 		0x07
#define DSL			0x09
#define RLS			0x0B
#define ACK_HOST 	0x10
#define NAK_HOST 	0x11

//NFC and tag emulation settings-----------------------------
#define	NFC106AC	0x29
#define NFC212AC	0x2A
#define NFC424AC	0x2B
#define	NFC106PA	0x21
#define NFC212PA	0x22
#define NFC424PA	0x23

#define TAG14443A	0x24
#define TAG14443B	0x25
#define TAG15693	0x26
#define TAGFelica	0x27

#define TAG106		0x00
#define TAG212		0x21
#define TAG424		0x42
#define TAG848		0x63

//#define FIFO_LEN	128
//#define FIFO_MORE	96
#define FIFO_LEN	32
#define FIFO_MORE	26

u08_t NfcRfCollisionAvoidance(u08_t mode);
u08_t NfcSend(u08_t *pbuf);
u08_t NfcFifoSend(u08_t length, u08_t broken_bits, u08_t no_tx_crc, u08_t delayed);
u08_t NfcMain(u08_t *pbuf, u08_t tag);

#endif
