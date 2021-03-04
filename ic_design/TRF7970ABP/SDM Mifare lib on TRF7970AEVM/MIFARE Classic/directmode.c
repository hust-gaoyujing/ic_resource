#include "directmode.h"
#include "spi.h"
//#include "globals.h"
#include "crypto1.h"
#include "trf797x.h"


/* Debugging/experimental defines */
//#define DIRECT_1
//#define DIS_EN_RX
#define LEAVE_ENTER_DIRECT_MODE
//#define PRINT_INTERRUPTS
#define RECEIVE_TIMEOUT 25000


//#ifdef SYS_CLK_13
#define COMPARE_FUZZ 13 //next test
#define EIGTH_BIT_DURATION 28
#define MOD_DURATION 56  // QUARTER BIT
#define HALF_BIT_DURATION 113
#define SEVEN_EIGTHS_BIT_DURATION	198
#define FULL_BIT_DURATION 226

#define AVERAGE_HIGH FULL_BIT_DURATION
#define AVERAGE_LOW  FULL_BIT_DURATION - 1

//#define COMPARE_FUZZ 8 //next test
//#define EIGTH_BIT_DURATION 16
//#define MOD_DURATION 32  // QUARTER BIT
//#define HALF_BIT_DURATION 64
//#define SEVEN_EIGTHS_BIT_DURATION	112
//#define FULL_BIT_DURATION 128

#define WAIT_FOR_BIT_START  while(TA2R > COMPARE_FUZZ);
#define BIT_ONE_EIGTH   	EIGTH_BIT_DURATION
#define BIT_ONE_QUARTER   	(0+MOD_DURATION )
#define BIT_HALF          	(HALF_BIT_DURATION )
#define BIT_THREE_QUARTER 	(HALF_BIT_DURATION+MOD_DURATION)
#define BIT_SEVEN_EIGTHS	SEVEN_EIGTHS_BIT_DURATION
#define WAIT_FOR(time)      while(TA2R < time);
#define WRAP_WAIT_FOR(time) {while(TA2R >= time); while(TA2R < time);}



unsigned char g_ui8DirectMode = 0;

//unsigned char g_ui8TimeOutFlag;


extern u08_t buf[128];

//void DirectModeReceiveFIX(int mode,
//                    void *send_buf, unsigned short *send_bytes, unsigned char *send_bits);



// unsigned short dst_len;

/* Problem: We need to detect a 847kHz signal on port 4, which is not interrupt
 *  capable.
 * Solution: Use inline assembler to do a BIT on P4 (sets the carry bit depending
 *  on whether the given bit in P4 is set or not) and then RRC which is a right
 *  shift through carry (e.g. the MSBit is set to the carry bit). 16 BIT/RRC
 *  instructions should give use 16 samples in one word with only 2 instructions
 *  per sample. Only after gathering 16 samples we need to store them away (INC
 *  and MOV), adding 2 (or 4, if we want compare and branch) instructions every 
 *  16 samples.
 *
 * In order to more easily compare these ideas with the generated assembler code
 *  this function is in another file (directmode_asm.c) which can be set to also
 *  output an assembler listing.
 */

////Mekre: These two functions are identical.  They just sample different pins.
////The SPI version samples MISO and the parallel version samples P4.6.  These
////pins correspond to I/O_6 of the TRF.
//extern void SPIDirectModeReceive_asm(uint16_t *data, int len);//Mekre

/* 220 words are 3520 samples, at approx. 2MHz sampling rate that's > 1.7ms and
 *  equaling, at 106kbps, approx 180 bits or 20 bytes with parity */
//static uint8_t samplebuf_[220] = {0};

//static parity_data_t databuf[20] = {0};



void DirectModeTransceive(int pui16ParityFlags,
                    void *pui16TxData, unsigned short pui8TxLength, unsigned char send_bits,
                    void *pui16RxData, unsigned short *pui8RxLengthBytes, unsigned char *pui8RxLengthBits)
{
	// Transmiting via SDM
	Mifare_SDM_config();
	Mifare_SDM_Enter();       		//Enter Special Direct Mode
	Mifare_SDM_Transmit(pui16TxData, pui8TxLength, (pui16ParityFlags & DM_TX_PARITY));
	Mifare_SDM_Exit();

	// Receiving with DM1
	Mifare_DM1_Enter();
	Mifare_DM1_Recieve(pui16RxData, pui8RxLengthBytes, pui8RxLengthBits, (pui16ParityFlags & DM_RX_PARITY));
	Mifare_DM1_Exit();
}



///* Host command. Send given buffer in opaque mode, receive response in parity
// *  mode, print response */
//void DirectModeTransceiveCommand(unsigned char *pbuf, unsigned short numbytes, unsigned char numbits)
//{
//  parity_data_t *dst = databuf;
// dst_len = sizeof(databuf)/sizeof(databuf[0]);
//  unsigned char dst_bits;
//  int mode = DIRECT_MODE_RECV_PARITY;
//
//  DirectModeTransceive(mode, pbuf, numbytes, numbits,
//                       dst, &dst_len, &dst_bits);
//
//}

/* Helper functions for ISO 14443-A frames to be sent or received in Direct Mode */

/* Calculates odd parity for one byte */
static inline unsigned char parity_byte(unsigned char byte)
{
  byte ^= byte >> 1; /* b0 = b0 ^ b1 */
  byte ^= byte >> 1; /* b0 = b0 ^ b1 ^ b2 */
  byte ^= byte >> 1; /* ... b3 */
  byte ^= byte >> 1; /* ... b4 */
  byte ^= byte >> 1; /* ... b5 */
  byte ^= byte >> 1; /* ... b6 */
  byte ^= byte >> 1; /* ... b7 */
  return (byte & 1)^1;
}

/* Checks that data has correct (odd) parity */
int CheckParity(const parity_data_t * const data, const size_t_rfid len)
{
	int i;

  int failed = 0;
  for(i=0; i<len; i++)
    failed |= (data[i] >> 8) ^ parity_byte(data[i] & 0xff);  
  return !failed;
}

/* Sets data to correct (odd) parity ; */  
void CalculateParity(parity_data_t * const data, const size_t_rfid len)
{
	int i;
  for(i=0; i<len; i++)
    data[i] = (data[i] & 0xff) | ( parity_byte(data[i] & 0xff) << 8 );
}

/* Calculate CRC-A and return it */
static uint16_t calculate_crc(const parity_data_t * const data, const size_t_rfid len)
{
	int i, j;

  uint16_t crc = 0x6363;
  for(i=0; i<len; i++) {
    uint8_t byte = data[i] & 0xff;
    for(j=0; j<8; j++) {
      char bit = (crc ^ byte) & 1;
      byte >>= 1;
      crc >>= 1;
      if(bit) crc ^= 0x8408;
    }
  }
  return crc;
}

/* Check that data has correct CRC in last two bytes */
int CheckCRC(const parity_data_t * const data, const size_t_rfid len)
{
  if(len < 2) return 0;
  uint16_t crc = calculate_crc(data, len-2);
  return ( (crc & 0xff) == (data[len-2] & 0xff) ) &&
    ( ((crc >> 8) & 0xff) == (data[len-1] & 0xff) );
}

/* Append two bytes of CRC to the data. Note that the buffer pointed to by
 *  data MUST have room for an additional two bytes, beyond the size indicated
 *  by len. Returns new length. */
size_t_rfid CalculateCRC(parity_data_t * const data, const size_t_rfid len)
{
  uint16_t crc = calculate_crc(data, len);
  data[len] = crc & 0xff;
  data[len+1] = (crc >> 8) & 0xff;
  CalculateParity(data+len, 2);
  return len +2;
}



//*****************************************************************************
//
//! Mifare_SDM_config - Configures MCU and TRF7970 for Special Direct Mode (SDM)
//!
//! This function is called from the DirectModeTransceive, just before using SDM
//! to transmit out the next packet.
//!
//! \return None.
//
//*****************************************************************************
void Mifare_SDM_config(void)
{
	SDM_TXENABLE_OFF;
	SDM_DATA_OFF;						// Step 1  (SLOA214 App Note)
	SDM_PORT_SET;

	buf[2] = TRF797x_IRQ_MASK;			// Step 2
	buf[3] = 0x3E;
	SpiWriteSingle(&buf[2], 2);

	buf[0] = TRF797x_ISO_CONTROL;		// Step 3
	buf[1] = 0x88;
	SpiWriteSingle(buf, 2);

	buf[0] = TRF797x_STATUS_CONTROL;	// Step 4
	buf[1] = 0x21;
	SpiWriteSingle(buf, 2);

	buf[0] = STOP_DECODERS;				// Step 5
	SpiDirectCommand(buf);

	buf[0] = TRF797x_IRQ_STATUS;		// Step 6
	SpiReadSingle(buf, 1);

	IRQ_OFF;
}

//*****************************************************************************
//
//! Mifare_SDM_Enter - Enters TRF7970A's Special Direct Mode (SDM)
//!
//! This function is called from the DirectModeTransceive, just before using SDM
//! to transmit out the next packet.  If configures the TRF7970 just to enter SDM.
//!
//! \return None.
//
//*****************************************************************************
void Mifare_SDM_Enter(void)
{
	buf[0] = TRF797x_SPECIAL_FUNCTION;	//Step 7
	buf[1] = 0x08;
	SpiWriteSingle(buf, 2);

	g_ui8DirectMode = 1;  // Keeps Slave Select Low

	buf[0] = TRF797x_STATUS_CONTROL;	// Step 8
	buf[1] = 0x61;
	SpiWriteSingle(buf, 2);

	UCB0TXBUF = 0x00;  					// Step 9
	while(UCB0STAT & UCBUSY);

	Trf797x_timeout_ms(40);
}

//*****************************************************************************
//
//! Mifare_SDM_Exit - Exit TRF7970A's Special Direct Mode (SDM)
//!
//! This function is called from DirectModeTransceive, just after using SDM
//! to transmit out the packet.  If configures the TRF7970 back to SPI mode.
//!
//! \return None.
//
//*****************************************************************************
void Mifare_SDM_Exit(void)
{
	SS_HIGH;							// Step 13
	g_ui8DirectMode = 0;

	buf[0] = TRF797x_SPECIAL_FUNCTION;
	buf[1] = 0x00;
	SpiWriteSingle(buf, 2);				// Step 14 & 15
}

//*****************************************************************************
//
//! Mifare_DM1_Exit - Exit Direct Mode 1 (DM1)
//!
//! This function is called from the DirectModeTransceive. It should be called
//! after the the response packet has been received or a timeout occured.
//!
//! \return None.
//
//*****************************************************************************
void Mifare_DM1_Exit(void)
{
	g_ui8DirectMode = 0;
	SS_HIGH;										// Step 21

	SpiUsciExtClkSet();

	buf[0] = TRF797x_SPECIAL_FUNCTION;
	buf[1] = 0x00;
	SpiWriteSingle(buf, 2);							// Step 22

	buf[0] = TRF797x_IRQ_STATUS;
	SpiReadSingle(buf, 1);       					// Step 23

	buf[0] = TRF797x_ISO_CONTROL;
	buf[1] = 0x88;
	SpiWriteSingle(buf, 2);

	__enable_interrupt();
}

//*****************************************************************************
//
//! Mifare_SDM_Transmit - Transmits Data using TRF7970A's Special Direct Mode (SDM)
//!
//! \param pui16TxData is a pointer to the beginning of the TX data
//!
//! \param ui8ByteCount is the length of the Tx Packet
//!
//! \param bSDMTxParity is the parity flag for Transmitting
//!
//! This function is called from DirectModeTransceive. It takes in the TX data,
//! length, and parity information and transmits the packet out to the tag.
//!
//! \return None.
//
//*****************************************************************************
void Mifare_SDM_Transmit(unsigned int *pui16TxData, unsigned short ui8ByteCount, unsigned char bSDMTxParity){
    uint8_t i, j, ui8ByteWidth;

    SDM_TXENABLE_ON;  														// Step 10

    if(bSDMTxParity){  // Parity bits?
        ui8ByteWidth = 9;
    }
    else{
        ui8ByteWidth = 8;
    }

	for(i=0; i<ui8ByteCount; i++){		//Byte Count
		for(j=0; j<ui8ByteWidth; j++){	//Bit Count
			while(SDM_BITCLK && (!IRQ) && (!g_ui8TimeoutFlag));  			// wait for Falling edge

			if(pui16TxData[i] & (0x01 << j)){  // next bit '0' or'1'?
				while((!SDM_BITCLK) && (!IRQ) && (!g_ui8TimeoutFlag));		// wait for Rising edge
				SDM_DATA_ON;			// "1"								// Step 11
			}
			else{
				while((!SDM_BITCLK)  && (!IRQ) && (!g_ui8TimeoutFlag));		// wait for Rising edge
				SDM_DATA_OFF;			// "0"
			}
		}
	}

    /************** EOF Sequence ************/
    if((!IRQ) ){
        while(SDM_BITCLK && (!g_ui8TimeoutFlag));
        while(!SDM_BITCLK && (!g_ui8TimeoutFlag));

        SDM_TXENABLE_OFF;													// Step 12
        while(SDM_BITCLK && (!g_ui8TimeoutFlag));
        while(!SDM_BITCLK && (!g_ui8TimeoutFlag));
    }
}

//*****************************************************************************
//
//! Mifare_DM1_Enter - Enter Direct Mode 1 (DM1) for receiving
//!
//! This function is called from the DirectModeTransceive. It should be called as
//! fast as possible after completely a transmit to be listen when the response
//! starts to be received.
//!
//! \return None.
//
//*****************************************************************************
void Mifare_DM1_Enter(void)
{
	SDM_PORT_SET;

	buf[0] = TRF797x_ISO_CONTROL;
	buf[1] = 0x48;
	SpiWriteSingle(buf, 2);				// Step 16

	g_ui8DirectMode = 1;

	buf[0] = TRF797x_STATUS_CONTROL;
	buf[1] = 0x61;
	SpiWriteSingle(buf, 2);				// Step 17

	UCB0TXBUF = 0x00; // 8 dummy clocks    Step 18
	while(UCB0STAT & UCBUSY);
}

//*****************************************************************************
//
//! Mifare_DM1_Recieve - Builds rx packet as each bit is received
//!
//! \param pui16RxData is a pointer to where the receive packet is built and stored
//!
//! \param pui8ByteCount is a pointer for passing the received byte count
//!
//! \param pui8BitCount is a pointer for passing the received bit count
//!
//! \param bParityFlag indicates if rx with parity (needed for correctly building bytes)
//!
//! This function is called from the DirectModeTransceive. It is called after Mifare_DM1_Enter.
//! This funciton takes the rx bits, stores them into bytes, and passes this and their length
//! back up for decryption.  The Parity_Flag = 1 indicates rx with parity, meaning each byte
//! should be 9 bits wide.  IRQ goes high, to signal the end of rx.
//!
//! \return None.
//
//*****************************************************************************
void Mifare_DM1_Recieve(unsigned int *pui16RxData, unsigned short *pui8ByteCount, unsigned char *pui8BitCount, unsigned char Parity_flag){  // polling method

	unsigned char ui8TempByteCount = 0;
	unsigned char ui8TempBitCount = 0;
	unsigned char ui8ByteWidth = 0;

#ifdef DM1_RX_DEBUGGING_SIGNAL
	SDM_DATA_OFF;
#endif  /* DM1_RX_DEBUGGING_SIGNAL */

	MISO_DATA_SETUP; 	 // DATA signal

	ui8TempByteCount = 0;
	ui8TempBitCount = 0;
	pui16RxData[ui8TempByteCount] = 0; // initialize buffer as used

	if(Parity_flag){
		ui8ByteWidth = 9;
	}
	else{
		ui8ByteWidth = 8;
	}

	/*
	 * In Direct Mode 1 Receiving, RX_Data (MISO) is updated on the falling Bit_Clk edge (I/O_5), and latched on the rising Bit_Clk edge.
	 * To make this decode functionality work @ 8MHz DCO, I only check for the Bit_Clk falling edge, then sample the Data.
	 * This allows us to latch proper data, and gives us the best time flexibility.
	 *
	 * Currently, interrupts are disabled in Direct mode.
	 */

	while( !(SDM_BITCLK || g_ui8TimeoutFlag) );  // wait for first rising edge: Start of RX

	do{

#ifdef DM1_RX_DEBUGGING_SIGNAL
		SDM_DATA_ON;
#endif  /* DM1_RX_DEBUGGING_SIGNAL */

		if(DM1_DATA_BIT){  	// If RX_data == "1", OR in "1".  (Buffer is pre-cleared to all 0x00.)
			pui16RxData[ui8TempByteCount] |= (0x01 << ui8TempBitCount);			// STEP 19
		}

		if(IRQ){  			// need to break free before incrementing bit_count
			break;
		}

		ui8TempBitCount++;

		if(ui8TempBitCount == ui8ByteWidth){
			ui8TempByteCount++;
			ui8TempBitCount = 0;
			pui16RxData[ui8TempByteCount] = 0; // PRE-clear next Buffer byte
		}

#ifdef DM1_RX_DEBUGGING_SIGNAL
		SDM_DATA_OFF;
#endif  /* DM1_RX_DEBUGGING_SIGNAL */

		while(SDM_BITCLK){ 				//wait for falling edge
			if((IRQ) || (g_ui8TimeoutFlag)){
				break;
			}
		}

	}while((!IRQ) && (!g_ui8TimeoutFlag)); // IRQ will go high when receive is complete;

	*pui8ByteCount = ui8TempByteCount;
	*pui8BitCount = ui8TempBitCount;

	MISO_SPI_SETUP;

}
