//***************************************************************
//------------<02.Dec.2010 by Peter Reiser>----------------------
//***************************************************************

#ifndef _MSP430F2370_H_
#define _MSP430F2370_H_

//================================================================

#include <MSP430x23x0.h> 					// prozessor spezific header
//#include <stdio.h>							// standard input/output header
#include "types.h"

//===============================================================

#define ENABLE_INTERRUPTS _EINT()
#define TRIGGER		0						// if TRIGGER 1 trigger-point at LED 5

//=====MCU constants=============================================

#define TRF_WRITE 		P4OUT				//port4 is connected to the
#define TRF_READ  		P4IN				//TRF796x IO port.
#define TRF_DIR_IN		P4DIR = 0x00
#define TRF_DIR_OUT		P4DIR = 0xFF
#define TRF_FUNC		P4SEL = 0x00

#define ENABLE_SET		P1DIR |= BIT0		// P1.0 ist switched in output direction
#define	TRF_ENABLE		P1OUT |= BIT0		// EN pin on the TRF796x
#define TRF_DISABLE		P1OUT &= ~BIT0

//---- PIN operations -------------------------------------------

#define CLK_P_OUT_SET 	P3DIR |= BIT3		// DATA_CLK on P3.3 (P3.3/UCB0CLK used in GPIO Mode for Parallel operation)
#define CLK_ON			P3OUT |= BIT3
#define CLK_OFF			P3OUT &= ~BIT3

#define MOD_SET			P2DIR |= BIT0;
#define MOD_ON			P2OUT |= BIT0
#define MOD_OFF			P2OUT &= ~BIT0;

#define IRQ_PIN_SET		P2DIR &= ~BIT1;	
#define IRQ_PIN			BIT1
#define IRQ_PORT		P2IN
#define IRQ_ON			P2IE |= BIT1		// IRQ on P2.1
#define IRQ_OFF			P2IE &= ~BIT1		// IRQ on P2.1
#define IRQ_EDGE_SET	P2IES &= ~BIT1		// rising edge interrupt
#define IRQ_CLR			P2IFG = 0x00
#define IRQ_REQ_REG		P2IFG

#define LED_PORT_SET	P1DIR |= 0xFC;
#define LED_ALL_OFF		P1OUT &= ~0xFC;
#define LED_ALL_ON		P1OUT |= 0xFC;
#define LED_POWER_ON	P1OUT |= BIT7;
#define LED_POWER_OFF	P1OUT &= ~BIT7;
#define LED_14443A_ON	P1OUT |= BIT6;
#define LED_14443A_OFF	P1OUT &= ~BIT6;
#define LED_14443B_ON	P1OUT |= BIT5;
#define LED_14443B_OFF	P1OUT &= ~BIT5;
#define LED_15693_ON	P1OUT |= BIT4;
#define LED_15693_OFF	P1OUT &= ~BIT4;
#define LED_OPEN1_ON	P1OUT |= BIT3;
#define LED_OPEN1_OFF	P1OUT &= ~BIT3;
#define LED_OPEN2_ON 	P1OUT |= BIT2;
#define LED_OPEN2_OFF 	P1OUT &= ~BIT2;

// #define SPIMODE				0				// This is set to Vcc for parallel mode regardless of the jumper at GND or VCC)
// #define SPIMODE				1				// This is set to Vcc for SPI mode regardless of the jumper at GND or VCC)
#define SPIMODE					P2IN & BIT3		// This is set to Vcc for SPI mode and GND for Parallel Mode using a separate jumper
#define SLAVE_SELECT_PORT_SET	P3DIR |= BIT0;
#define SLAVE_SELECT_HIGH		P3OUT |= BIT0;
#define SLAVE_SELECT_LOW		P3OUT &= ~BIT0;

#define OOK_DIR_IN				P2DIR &= ~BIT2;
#define OOK_DIR_OUT				P2DIR |= BIT2
#define OOK_OFF					P2OUT &= ~BIT2
#define OOK_ON					P2OUT |= BIT2

//-----Counter-timer constants-----------------------------------
#define COUNT_VALUE	TACCR0					//counter register
// #define START_COUNTER	TACTL |= MC0 + MC1	//start counter in up/down mode
#define START_COUNTER	TACTL |=  MC1		//start counter in up mode

#define STOP_COUNTER	TACTL &= ~(MC0 + MC1)	//stops the counter

//---------------------------------------------------------------

#define COUNT_1ms		847
#define COUNT_60ms		0xC684

//---------------------------------------------------------------

#define DELAY_1ms		6780

//===============================================================

void Msp430f23x0DelayMillisecond(u32_t n_ms);
void Msp430f23x0OscSel(u08_t mode);
void Msp430f23x0CounterSet(void);

//===============================================================

#endif
