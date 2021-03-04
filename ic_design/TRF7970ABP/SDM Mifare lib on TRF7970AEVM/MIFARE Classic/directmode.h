#ifndef __DIRECTMODE_H__
#define __DIRECTMODE_H__

#include "crypto1.h"
#include "mcu.h"

/* Input to DirectModeTransceive is an opaque stream of bits, grouped as 8
 *  bits into one byte. LSBit should be sent first. */
#define DIRECT_MODE_SEND_OPAQUE 0

/* Input to DirectModeTransceive is an array of bytes with associated parity
 *  bit, stored as 16 bit words. The lower 8 bits in each word are the byte
 *  (LSBit should be sent first), the lowest bit of the upper 8 bits is the
 *  parity bit. The remaining 7 bits are ignored.
 */
#define DM_TX_PARITY 1

/* Output from DirectModeTransceive should be an opaque bit stream, grouped as
 *  8 bits into one byte, LSBit was received first. */
#define DIRECT_MODE_RECV_OPAQUE 0


/* define this to use TX Data SDM pin to display sample edges. */
#define DM1_RX_DEBUGGING_SIGNAL


/* Output from DirectModeTransceive should be an array of bytes with associated
 *  parity bit. */
#define DM_RX_PARITY 2

#define DIRECT_MODE_SEND_MASK 1
#define DIRECT_MODE_RECV_MASK 2

extern void DirectModeTransceive(int mode,
                    void *send_buf, unsigned short send_bytes, unsigned char send_bits, 
                    void *recv_buf, unsigned short *recv_bytes, unsigned char *recv_bits);
//extern void DirectModeTransceiveCommand(unsigned char *pbuf, unsigned short numbytes, unsigned char numbits);
//unsigned char DirectModeReceiveBits(void);
void DirectModeReceiveDecode(int parity_mode,void *recv_buf, unsigned short *recv_bytes, unsigned char *recv_bits);

extern int CheckParity(const parity_data_t * const data, const size_t_rfid len);
extern void CalculateParity(parity_data_t * const data, const size_t_rfid len);
extern int CheckCRC(const parity_data_t * const data, const size_t_rfid len);
extern size_t_rfid CalculateCRC(parity_data_t * const data, const size_t_rfid len);
extern u08_t g_ui8TimeoutFlag;

void Mifare_SDM_Transmit(unsigned int *Data_buf, unsigned short Byte_Length, unsigned char ui8TxParity);
void Mifare_SDM_Exit(void);
void Mifare_SDM_config(void);
void Mifare_SDM_Enter(void);

void Mifare_DM1_Enter(void);
void Mifare_DM1_Exit(void);
void Mifare_DM1_Recieve(unsigned int *Data_buf, unsigned short *byte_count, unsigned char *bit_count, unsigned char Parity_flag);

#endif
