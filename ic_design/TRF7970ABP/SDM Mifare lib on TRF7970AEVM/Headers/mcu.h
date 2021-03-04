/*
 * File Name: mcu.h
 *
 * Description: Header file for all functions for mcu.h
 *
 * Copyright (C) 2014 Texas Instruments Incorporated - http://www.ti.com/
 *
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
*/

#ifndef MCU_H_
#define MCU_H_

//===============================================================

#include "msp430.h" 		// Processor specific header
#include "types.h"

//=====MCU constants=============================================

#define TRF_ENABLE_SET	P1DIR |= BIT0		// P2.2 is switched in output direction
#define	TRF_ENABLE		P1OUT |= BIT0		// EN pin on the TRF7970A
#define TRF_DISABLE		P1OUT &= ~BIT0

// IRQ on P2.1
#define IRQ				(IRQ_PORT & IRQ_PIN)
#define IRQ_PIN_SET		P2DIR &= ~BIT1;
#define IRQ_PIN			BIT1
#define IRQ_PORT		P2IN
#define IRQ_ON			P2IE |= BIT1
#define IRQ_OFF			P2IE &= ~BIT1
#define IRQ_EDGE_SET	P2IES &= ~BIT1		// Rising edge interrupt
#define IRQ_CLR			P2IFG = 0x00
#define IRQ_REQ_REG		P2IFG

//#define LED_PORT_SET	P2DIR |= 0x38;
#define LED_ALL_OFF		//P2OUT &= ~0x38;

//#define LED_14443A_ON	//P2OUT |= BIT4;
//#define LED_14443A_OFF	//P2OUT &= ~BIT4;
//#define LED_14443B_ON	//P2OUT |= BIT3;
//#define LED_14443B_OFF	//P2OUT &= ~BIT3;
//#define LED_15693_ON	//P2OUT |= BIT5;
//#define LED_15693_OFF	//P2OUT &= ~BIT5;

#define LED_PORT_SET	P1DIR |= 0xFC;

#define LED_POWER_ON	P1OUT |= BIT7;
#define LED_POWER_OFF	P1OUT &= ~BIT7;

#define LEDtypeAON		P1OUT |= BIT6;
#define LEDtypeAOFF		P1OUT &= ~BIT6;

#define LEDAuth1OFF		P1OUT &= ~BIT5;
#define LEDAuth1ON		P1OUT |= BIT5;

#define LEDAuth2ON		P1OUT |= BIT4;
#define LEDAuth2OFF		P1OUT &= ~BIT4;

#define LEDAuth3ON		P1OUT |= BIT3;
#define LEDAuth3OFF		P1OUT &= ~BIT3;

#define LED_MFC_READ_BLOCK_ON		P1OUT |= BIT2;
#define LED_MFC_READ_BLOCK_OFF		P1OUT &= ~BIT2;

#define SLAVE_SELECT_PORT_SET	P3DIR |= BIT0;
#define SLAVE_SELECT_HIGH		P3OUT |= BIT0;
#define SLAVE_SELECT_LOW		P3OUT &= ~BIT0;

//********* SDM************************************************
#define SDM_TXENABLE	(BIT2)	// port 4.2
#define SDM_DATA		(BIT3)	// port 4.3
#define SDM_BITCLK_BIT 	(BIT5)  // port 4.5


#define SDM_PORT_SET  	P4DIR |= SDM_TXENABLE; 		\
						P4DIR &= ~SDM_BITCLK_BIT; 	\
						P4DIR |= SDM_DATA;	//		\
//						P4IES &= ~SDM_BITCLK_BIT;	\
//						P4REN |= SDM_BITCLK_BIT;

#define MOD_DIR_OUT	 P2DIR |= BIT0
#define MOD_OFF		 P2OUT &= ~BIT0
#define MOD_ON		 P2OUT |= BIT0

//#define SDM_PORT_DIR    P4DIR
//#define SDM_PORT_OUT    P4OUT
//#define SDM_PORT_IN   	P4IN

#define SDM_TXENABLE_ON    	P4OUT |= SDM_TXENABLE
#define SDM_TXENABLE_OFF   	P4OUT &= ~SDM_TXENABLE
#define SDM_DATA_ON    		P4OUT |= SDM_DATA
#define SDM_DATA_OFF   		P4OUT &= ~SDM_DATA
#define SDM_BITCLK    		(P4IN & SDM_BITCLK_BIT)

#define MISO_DATA_SETUP		SPI_PORT_SEL1 &= ~MISO_PIN;  \
                      	    /*SPI_PORT_SEL2 &= ~MISO_PIN;*/  \
							SPI_PORT_DIR &= ~MISO_PIN;

#define MISO_SPI_SETUP		SPI_PORT_SEL1 |= MISO_PIN;	\
                      	  	/*SPI_PORT_SEL2 |= MISO_PIN;*/	\
							SPI_PORT_DIR &= ~MISO_PIN;

#define DM1_DATA_BIT		(SPI_PORT_IN & MISO_PIN)
//******************************************************************

//===============================================================

#define SCLK_PIN		BIT3
#define MISO_PIN		BIT2
#define MOSI_PIN		BIT1

#define SPI_PORT_SEL1	P3SEL
#define SPI_PORT_SEL2	//  Not needed for F2370
#define SPI_PORT_DIR	P3DIR
#define SPI_PORT_OUT	P3OUT
#define SPI_PORT_IN		P3IN

#define SS_PIN			BIT0

#define SS_PORT_SEL		P3SEL
#define SS_PORT_DIR		P3DIR
#define SS_PORT_OUT		P3OUT

#define SS_PORT_SET  SS_PORT_DIR |= SS_PIN; SS_PORT_SEL &= ~SS_PIN;
#define SS_HIGH      SS_PORT_OUT |= SS_PIN;
#define SS_LOW       SS_PORT_OUT &= ~SS_PIN;


//-----Counter-timer constants-----------------------------------
#define COUNTER_VALUE	TACCR0					//counter register
#define START_COUNTER	TACTL |=  MC1			//start counter in up mode
#define STOP_COUNTER	TACTL &= ~(MC0 + MC1)	//stops the counter
#define RESET_COUNTER   TACTL |= TACLR	    	//Resets and stops counter.

#define COUNT_1ms		3		// Used for Timer Counter
//#define COUNT_1ms		1058		// Used for Timer Counter

//===============================================================

#define countValue		TACCR0			//counter register
#define startCounter	TACTL |=  MC1	//start counter in up mode
#define stopCounter		TACTL &= ~(MC0 + MC1)	//stops the counter
#define MS_COUNT		3	// Used for SDM Timeout
//===============================================================

#define DELAY_1ms		8000	// Used for McuDelayMillisecond

//===============================================================

void McuCounterSet(void);
void McuDelayMillisecond(u32_t n_ms);
void McuOscSel(u08_t mode);
void CounterSet(void);

//===============================================================

#endif
