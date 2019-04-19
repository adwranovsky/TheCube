//###########################################################################
//
// FILE:    F2802x_I2C.c
//
// TITLE:    F2802x I2C Initialization & Support Functions.
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
#include "F2802x_Device.h"     // Headerfile Include File
#include "f2802x_examples.h"   // Examples Include File
#include "main.h"

//
// InitI2C - This function initializes the I2C to a known state.
//
void
InitI2C(void)
{
    //
    // Initialize I2C-A
    //

    // Force the module into reset using IRS bit in I2CMDR. IRS must remain zero while reconfiguring.
    I2caRegs.I2CMDR.bit.IRS = 0;

    // Set the I2C SCL frequency to 400 KHz
    I2caRegs.I2CPSC.bit.IPSC = 4;  // init I2C module clock, must be between 7-12 MHz (I2CPSC)
    I2caRegs.I2CCLKL         = 10; // init I2C SCL low time
    I2caRegs.I2CCLKH         = 10; // init I2C SCL high time

    // Configure the TX FIFO
    I2caRegs.I2CFFTX.bit.I2CFFEN = 1; // Enable the I2C FIFOs
    I2caRegs.I2CFFTX.bit.TXFFIL  = 0; // Interrupt when the FIFO has 0 bytes left
    I2caRegs.I2CFFTX.bit.TXFFRST = 1; // Pull the TX FIFO out of reset

    // Configure the RX FIFO
    I2caRegs.I2CFFRX.bit.RXFFIL  = 1; // Interrupt when the FIFO has received 1 byte
    I2caRegs.I2CFFRX.bit.RXFFRST = 1; // Pull the RX FIFO out of reset

    // Enable interrupts
    I2caRegs.I2CIER.bit.ARDY      = 1; // Interrupt on Register-access-ready
    I2caRegs.I2CIER.bit.NACK      = 1; // Interrupt on NACK
    I2caRegs.I2CIER.bit.SCD       = 1; // Interrupt on stop condition
    I2caRegs.I2CFFTX.bit.TXFFIENA = 1; // Interrupt on TX FIFO
    I2caRegs.I2CFFRX.bit.RXFFIENA = 1; // Interrupt on RX FIFO

    // Make sure FIFO interrupts are cleared before exiting reset
    I2caRegs.I2CFFTX.bit.TXFFINTCLR = 1;
    I2caRegs.I2CFFRX.bit.RXFFINTCLR = 1;

    // Take the I2C module out of reset
    I2caRegs.I2CMDR.bit.IRS = 1;
}

//
// InitI2CGpio - This function initializes GPIO pins to function as I2C pins
//
// Each GPIO pin can be configured as a GPIO pin or up to 3 different
// peripheral functional pins. By default all pins come up as GPIO
// inputs after reset.
//
// Caution:
// Only one GPIO pin should be enabled for SDAA operation.
// Only one GPIO pin shoudl be enabled for SCLA operation.
// Comment out other unwanted lines.
//
void
InitI2CGpio()
{
    EALLOW;

    //
    // Enable internal pull-up for the selected pins
    // Pull-ups can be enabled or disabled disabled by the user.
    // This will enable the pullups for the specified pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPAPUD.bit.GPIO28 = 0;    // Enable pull-up for GPIO28 (SDAA)
    GpioCtrlRegs.GPAPUD.bit.GPIO29 = 0;    // Enable pull-up for GPIO29 (SCLA)

    //GpioCtrlRegs.GPBPUD.bit.GPIO32 = 0;   // Enable pull-up for GPIO32 (SDAA)
    //GpioCtrlRegs.GPBPUD.bit.GPIO33 = 0;   // Enable pull-up for GPIO33 (SCLA)

    //
    // Set qualification for selected pins to asynch only
    // This will select asynch (no qualification) for the selected pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPAQSEL2.bit.GPIO28 = 3;  // Asynch input GPIO28 (SDAA)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO29 = 3;  // Asynch input GPIO29 (SCLA)

    //GpioCtrlRegs.GPBQSEL1.bit.GPIO32 = 3;  // Asynch input GPIO32 (SDAA)
    //GpioCtrlRegs.GPBQSEL1.bit.GPIO33 = 3;  // Asynch input GPIO33 (SCLA)

    //
    // Configure I2C pins using GPIO regs
    // This specifies which of the possible GPIO pins will be I2C
    // functional pins. Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 2;   // Configure GPIO28 for SDAA
    GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 2;   // Configure GPIO29 for SCLA

    //GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 1;   // Configure GPIO32 for SDAA
    //GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 1;   // Configure GPIO33 for SCLA

    EDIS;
}

/*
 * LED Driver Workflow:
 *
 *
 */

// I2C defines
#define REPEAT_MODE_START 0x26b0
#define REPEAT_MODE_STOP  0x0eb0
#define FIFO_DEPTH        4

// LED driver register addresses
#define SHUTDOWN_REG      0x00
#define DUTY_CYCLE_0_REG  0x05
#define UPDATE_REG        0x25
#define LED_CTRL_0_REG    0x2A
#define GLOBAL_CTRL_REG   0x4A
#define OUTPUT_FREQ_REG   0x4B
#define RESET_REG         0x4F

static const uint16_t driver_addrs = {0x3c|0, 0x3c|3, 0x3c|1};
static struct {
    uint16_t current_led = 0;
} Cube_State;

void start_cube(void) {
    // Set the slave address to the first LED driver
    I2caRegs.I2CSAR.bit.SAR = driver_addrs[0];

    // Load the driver register address into the FIFO
    I2caRegs.I2CDXR.all = DUTY_CYCLE_0_REG;

    // Load the first few LED values into the FIFO
    while (I2caRegs.I2CFFTX.bit.I2CFFST != FIFO_DEPTH) {
        I2caRegs.I2CDXR.all = framebuffer[Cube_State.current_led++];
    }

    // Start a repeated transaction to get the snowball rolling
    I2caRegs.I2CMDR.all = REPEAT_MODE_START;
}

__interrupt void i2c_isr(void) {
    uint16_t i2c_int_status = I2caRegs.I2CSTR.all;

    switch (I2caRegs.I2CISRC.bit.INTCODE) {
        case I2C_NACK_ISRC: {
            break;
        }
        case I2C_ARDY_ISRC: {
            break;
        }
        case I2C_TX_ISRC: {
            break;
        }
        case I2C_SCD_ISRC: {
            break;
        }
        default:
            while (1); // Unhandled interrupt type, spin loop for debug. The I2C interrupts were probably configured wrong
    }

    // Acknowledge the PIE interrupt so we can receive more on this group
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP8;
}

//
// End of file
//

