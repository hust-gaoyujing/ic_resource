#ifndef CRYPTO1_H_
#define CRYPTO1_H_

//#include "stddef.h"

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#else
typedef unsigned int size_t_rfid;
#endif

#include <stdint.h>

/*
 * This type holds data bytes with associated parity bits.
 * The data is in the low byte while the associated parity bit
 * is in the least-significant bit of the high byte.
 */
typedef uint16_t parity_data_t;

struct _crypto1_state;

struct _crypto1_ops {
	void(*init)(struct _crypto1_state *state, const uint64_t key);
	void(*mutual_1)(struct _crypto1_state *state, const uint32_t uid, const uint32_t card_challenge);
	void(*mutual_2_reader)(struct _crypto1_state *state, parity_data_t *reader_response);
	int (*mutual_3_reader)(struct _crypto1_state *state, const parity_data_t *card_response);
	int (*mutual_2_card)(struct _crypto1_state *state, const parity_data_t *reader_response);
	void(*mutual_3_card)(struct _crypto1_state *state, parity_data_t *card_response);
	void(*transcrypt_bits)(struct _crypto1_state *state, parity_data_t *data, size_t_rfid bytes, size_t_rfid bits);
};

typedef struct _crypto1_state {
	uint64_t lfsr; /* The 48 bit LFSR for the main cipher state and keystream generation */
	uint16_t prng; /* The 16 bit LFSR for the card PRNG state, also used during authentication. */
	
	uint8_t is_card; /* Boolean whether this instance should perform authentication in card mode. */
	
	const struct _crypto1_ops *ops;
} crypto1_state;

enum crypto1_cipher_implementation {
	CRYPTO1_IMPLEMENTATION_CLEAN,
	CRYPTO1_IMPLEMENTATION_OPTIMIZED
};

int  crypto1_new(crypto1_state *state, uint8_t is_card, enum crypto1_cipher_implementation implementation);
void crypto1_init(crypto1_state *state, uint64_t key);
void crypto1_mutual_1(crypto1_state *state, uint32_t uid, uint32_t card_challenge);
int  crypto1_mutual_2(crypto1_state *state, parity_data_t *reader_response);
int  crypto1_mutual_3(crypto1_state *state, parity_data_t *card_response);
void crypto1_transcrypt(crypto1_state *state, parity_data_t *data, size_t_rfid length);
void crypto1_transcrypt_bits(crypto1_state *state, parity_data_t *data, size_t_rfid bytes, size_t_rfid bits);

int _crypto1_new_opt(crypto1_state *state);
int _crypto1_new_clean(crypto1_state *state);

/* Reverse the bit order in the 8 bit value x */
#define rev8(x)			((((x)>>7)&1)^((((x)>>6)&1)<<1)^((((x)>>5)&1)<<2)^((((x)>>4)&1)<<3)^((((x)>>3)&1)<<4)^((((x)>>2)&1)<<5)^((((x)>>1)&1)<<6)^(((x)&1)<<7))
/* Reverse the bit order in the 16 bit value x */
#define rev16(x)		(rev8 (x)^(rev8 (x>> 8)<< 8))
/* Reverse the bit order in the 32 bit value x */
#define rev32(x)		(rev16(x)^(rev16(x>>16)<<16))
/* Return the nth bit from x */
#define bit(x,n)		(((x)>>(n))&1)

/* Convert 4 array entries (a[0], a[1], a[2] and a[3]) into a 32 bit integer, where 
 * a[0] is the MSByte and a[3] is the LSByte */
#define ARRAY_TO_UINT32(a) ( ((uint32_t)((a)[0]&0xff) << 24) | ((uint32_t)((a)[1]&0xff) << 16) | \
	((uint32_t)((a)[2]&0xff) << 8) | ((uint32_t)((a)[3]&0xff) << 0) )
/* Copy an uint32_t i into 4 array entries (a[0], a[1], a[2] and a[3]), where a[0] is the MSByte */
#define UINT32_TO_ARRAY(i, a) do{ \
	(a)[0] = ((i)>>24) & 0xff; \
	(a)[1] = ((i)>>16) & 0xff; \
	(a)[2] = ((i)>> 8) & 0xff; \
	(a)[3] = ((i)>> 0) & 0xff; \
}while(0);

/* Calculate the odd parity bit for one byte of input */ 
#define ODD_PARITY(i) (( (i) ^ (i)>>1 ^ (i)>>2 ^ (i)>>3 ^ (i)>>4 ^ (i)>>5 ^ (i)>>6 ^ (i)>>7 ^ 1) & 0x01)

/* Like UINT32_TO_ARRAY, but put the correct parity in the 8th bit of each array entry (thus the
 * array entries must at least be 16 bit wide) */
#define UINT32_TO_ARRAY_WITH_PARITY(i, a) do{ \
	(a)[0] = ((i)>>24) & 0xff; (a)[0] |= ODD_PARITY((a)[0])<<8; \
	(a)[1] = ((i)>>16) & 0xff; (a)[1] |= ODD_PARITY((a)[1])<<8; \
	(a)[2] = ((i)>> 8) & 0xff; (a)[2] |= ODD_PARITY((a)[2])<<8; \
	(a)[3] = ((i)>> 0) & 0xff; (a)[3] |= ODD_PARITY((a)[3])<<8; \
}while(0);

#endif /*CRYPTO1_H_*/
