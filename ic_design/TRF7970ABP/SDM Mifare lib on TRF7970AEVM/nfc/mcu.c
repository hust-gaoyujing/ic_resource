/*
 * File Name: mcu.c
 *
 * Description: Contains general microcontroller level functions
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

#include "trf797x.h"
#include "mcu.h"


//===========================================================================
//	Name:  McuCounterSet
//	Usage: Set up the counter for Timer A
//
//===========================================================================

void 
McuCounterSet(void)
{
	TACTL |= TACLR;
	TACTL &= ~TACLR;				// reset the timerA
	TACTL |= TASSEL0 + TAIE;	// ACLK (@ 3KHz), interrupt enable, timer stoped

	TAR = 0x0000;
	TACCTL0 |= CCIE;				// compare interrupt enable
}

//===========================================================================
//	Name:  McuDelayMillisecond
//	Usage: Built in function to delay for approximately one millisecond
// 		   DELAY_1ms must be calibrated based on the clock speed of the MCU
//
//===========================================================================

void 
McuDelayMillisecond(u32_t n_ms)
{
    while (n_ms--)
    {
    	__delay_cycles(DELAY_1ms);		// clock speed in Hz divined by 1000
    }
}

//===========================================================================
//	Name:  McuOscSel
//	Usage: Calibrate the Oscillator
//
//===========================================================================

void 
McuOscSel(u08_t mode)
{
    // select DCO to 8MHz

    if (CALBC1_8MHZ != 0xFF)
    {
        // Follow recommended flow. First, clear all DCOx and MODx bits.
        // Then apply new RSELx values. Finally, apply new DCOx and MODx bit
        // values.
        DCOCTL = 0x00;
        BCSCTL1 = CALBC1_8MHZ;
        DCOCTL = CALDCO_8MHZ;
    }

    // Disable XT1 pins
    P2SEL &= ~(BIT6 + BIT7);

    // Disable XT1 high frequency mode, ACLK = 12kHz/4 = 3kHz
    BCSCTL1 &= ~XTS;
    BCSCTL1 |= DIVA_2;

    // Set XT1 to VLO
    BCSCTL3 |= LFXT1S_2;

	return;
}



void CounterSet(void)
{
	TACTL |= TACLR;
	TACTL &= ~TACLR;			//reset the timerA
	TACTL |= TASSEL1 + ID1 + ID0;		//SMCLK, div 8, interrupt enable, timer stoped; Mekre: Changed to SMCLK
	//TASSEL__ACLK;
	TAR = 0x0000;
	TACCTL0 |= CCIE;			//compare interrupt enable
}//CounterSet()

/*
 =======================================================================================================================
 =======================================================================================================================
 */


/*
 =======================================================================================================================
 =======================================================================================================================
 */

//////////#pragma vector=TIMER2_A0_VECTOR
//////////__interrupt void TimerA2handler(void)
//////////{
//////////    irq_flagx= 0x03;
//////////    stopCounter;
//////////
//////////    __low_power_mode_off_on_exit();
//////////}

//#pragma vector = TIMERA0_VECTOR // TIMER1_A0_VECTOR
//__interrupt void TimerAhandler(void)
//{
//	stopCounter;
//	g_ui8TimeOutFlag = 1;
//
//	//unsigned char Register;
//          //clkOFF;
////          unsigned char Register[2];
////
////	__low_power_mode_off_on_exit();
////
////////////////////         kputchar('V');
////
////
////	//Register = IRQStatus;		//IRQ status register address
////	//IRQ_CLR;				//PORT2 interrupt flag clear
////	//ReadSingle(&Register, 1);	//function call for single address read
////					//IRQ status register has to be read
////        Register[0] = TRF797x_IRQ_STATUS;	/* IRQ status register address */
////        Register[1] = TRF797x_IRQ_MASK;		//Dummy read
////	//ReadSingle(Register, 2);	/* function call for single address read */
////        //ReadCont(Register, 1);
////        SpiReadCont(Register, 2);
////       // ReadSingle(Register, 1);
////
////
////
////        *Register = *Register & 0xF7;	//set the parity flag to 0
//
//
////	//if(Register == 0x00)
////        if(*Register == 0x00 || *Register == 0x80) //added code
////          	//if(RXTXstate > 1)
////                //  	i_reg = 0xFF;
////        	//else
////			i_reg = 0x00;
////	else
////		i_reg = 0x00;
////
////    irq_flag = 0x03;
//
//
//}//TimerAhandler
