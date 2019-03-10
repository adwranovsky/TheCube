//###########################################################################
//
// FILE:    F2802x_Sci.c
//
// TITLE:    F2802x SCI Initialization & Support Functions.
//
//###########################################################################
// $TI Release: F2802x Support Library v3.02.00.00 $
// $Release Date: Thu Oct 18 15:45:37 CDT 2018 $
// $Copyright:
// Copyright (C) 2009-2018 Texas Instruments Incorporated - http://www.ti.com/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//   Redistributions of source code must retain the above copyright
//   notice, this list of conditions and the following disclaimer.
//
//   Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the
//   documentation and/or other materials provided with the
//   distribution.
//
//   Neither the name of Texas Instruments Incorporated nor the names of
//   its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// $
//###########################################################################

//
// Included Files
//
#include <stddef.h>
#include "F2802x_Device.h"     // Headerfile Include File
#include "f2802x_examples.h"   // Examples Include File

static struct {
    volatile int busy;
    char *next;
    char *end;
} Rx_Isr_State = {
    .busy = 0,
    .next = NULL,
    .end = NULL
};

static struct {
    volatile int busy;
    char *next;
    char *end;
} Tx_Isr_State = {
    .busy = 0,
    .next = NULL,
    .end = NULL
};

//
// InitSci - This function initializes the SCI(s) to a known state.
//
void
InitSci(void)
{
    //
    // Initialize SCI-A
    //
    InitSciaGpio();

    // hold the peripheral in reset while configuring
    SciaRegs.SCICTL1.bit.SWRESET = 0;

    // Configure the peripheral for uart operation
    SciaRegs.SCICCR.bit.STOPBITS = 0;
    SciaRegs.SCICCR.bit.PARITY = 0;
    SciaRegs.SCICCR.bit.PARITYENA = 0;
    SciaRegs.SCICCR.bit.LOOPBKENA = 0;
    SciaRegs.SCICCR.bit.ADDRIDLE_MODE = 0;
    SciaRegs.SCICCR.bit.SCICHAR = 7;

    SciaRegs.SCICTL1.bit.RXERRINTENA = 0;
    SciaRegs.SCICTL1.bit.TXWAKE = 0;
    SciaRegs.SCICTL1.bit.SLEEP = 0;

    // Program a baud rate of 9600
    SciaRegs.SCIHBAUD = (0xFF00 & 195) >> 8;
    SciaRegs.SCILBAUD = 0xFF & 195;

    // Disable interrupts while idling
    SciaRegs.SCICTL2.bit.RXBKINTENA = 0;
    SciaRegs.SCICTL2.bit.TXINTENA = 0;

    // Disable fifo mode
    SciaRegs.SCIFFTX.bit.SCIFFENA = 0;

    // During a breakpoint, halt the peripheral
    SciaRegs.SCIPRI.bit.FREE = 0;
    SciaRegs.SCIPRI.bit.SOFT = 0;

    // enable the transmitter and receiver
    SciaRegs.SCICTL1.bit.TXENA = 1;
    SciaRegs.SCICTL1.bit.RXENA = 1;

    // take the peripheral out of reset
    SciaRegs.SCICTL1.bit.SWRESET = 1;
}

void
InitSciaGpio()
{
    EALLOW;

    //
    // Enable internal pull-up for the selected pins
    // Disable internal pull-up for the selected output pins
    // to reduce power consumption.
    // Pull-ups can be enabled or disabled disabled by the user.
    //

    //
    // Enable pull-up for GPIO28 (SCIRXDA)
    //
    GpioCtrlRegs.GPAPUD.bit.GPIO28 = 0;

    //
    // Enable pull-up for GPIO19 (SCIRXDA)
    //
    //GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;

    //
    // Enable pull-up for GPIO7  (SCIRXDA)
    //
    //GpioCtrlRegs.GPAPUD.bit.GPIO7 = 0;

    //
    // Disable pull-up for GPIO29 (SCITXDA)
    //
    GpioCtrlRegs.GPAPUD.bit.GPIO29 = 1;

    //
    // Disable pull-up for GPIO18 (SCITXDA)
    //
    //GpioCtrlRegs.GPAPUD.bit.GPIO18 = 1;

    //
    // Disable pull-up for GPIO12 (SCITXDA)
    //
    //GpioCtrlRegs.GPAPUD.bit.GPIO12 = 1;

    //
    // Set qualification for selected pins to asynch only
    // Inputs are synchronized to SYSCLKOUT by default.
    // This will select asynch (no qualification) for the selected pins.
    //
    GpioCtrlRegs.GPAQSEL2.bit.GPIO28 = 3;  // Asynch input GPIO28 (SCIRXDA)
    //GpioCtrlRegs.GPAQSEL2.bit.GPIO19 = 3;  // Asynch input GPIO19 (SCIRXDA)
    //GpioCtrlRegs.GPAQSEL1.bit.GPIO7 = 3;   // Asynch input GPIO7 (SCIRXDA)

    //
    // Configure SCI-A pins using GPIO regs
    // This specifies which of the possible GPIO pins will be SCI functional
    // pins.
    //
    GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 1;   // Configure GPIO28 for SCIRXDA
    //GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 2;   // Configure GPIO19 for SCIRXDA
    //GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 2;    // Configure GPIO7  for SCIRXDA

    GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 1;   // Configure GPIO29 for SCITXDA
    //GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 2;   // Configure GPIO18 for SCITXDA
    //GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 2;   // Configure GPIO12 for SCITXDA

    EDIS;
}

char sci_get_char(void) {
    // If a data is waiting in the receive register, grab it and return
    if (SciaRegs.SCIRXST.bit.RXRDY) {
        return SciaRegs.SCIRXBUF.bit.RXDT;
    }

    // Otherwise, wait for a character
    volatile char c;
    Rx_Isr_State.next = &c;
    Rx_Isr_State.end  = &c + 1;
    Rx_Isr_State.busy = 1;

    SciaRegs.SCICTL2.bit.RXBKINTENA = 1; // enable rx interrupt
    while (Rx_Isr_State.busy) {};
    SciaRegs.SCICTL2.bit.RXBKINTENA = 0; // disable rx interrupt

    return c;
}

void sci_send_char(char c) {
    // No interrupts are used when sending a single char
    while (!SciaRegs.SCICTL2.bit.TXRDY) {};
    SciaRegs.SCITXBUF = c;
    while (!SciaRegs.SCICTL2.bit.TXEMPTY) {};
}

void sci_get_buf(char *buf, size_t length) {
}

void sci_send_buf(const char *buf, size_t length) {
}

__interrupt void sci_rx_isr(void) {
    // Read the RX buffer register to clear the interrupt
    char c = SciaRegs.SCIRXBUF.bit.RXDT;

    // Check if we're actively trying to receive something
    if (Rx_Isr_State.busy) {
        // Put the received character in the buffer, and increment to the next position
        *Rx_Isr_State.next = c;
        Rx_Isr_State.next++;

        // If this was the last character, signal to the main application
        if (Rx_Isr_State.next == Rx_Isr_State.end) {
            Rx_Isr_State.busy = 0;
        }
    }

    // Acknowledge PIE group interrupt
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}

__interrupt void sci_tx_isr(void) {
    SciaRegs.SCITXBUF = *Tx_Isr_State.next;
    Tx_Isr_State.next++;
    if (Tx_Isr_State.next == Tx_Isr_State.end) {
        Tx_Isr_State.busy = 0;
    }
    // Acknowledge PIE group interrupt
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}

//
// End of file
//

