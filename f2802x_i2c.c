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
void InitI2C(void) {
    //
    // Initialize I2C-A
    //
    InitI2CGpio();

    EALLOW;

    // Force the module into reset using IRS bit in I2CMDR. IRS must remain zero while reconfiguring.
    I2caRegs.I2CMDR.bit.IRS = 0;

    // Set the I2C SCL frequency to 400 KHz
    I2caRegs.I2CPSC.bit.IPSC = 4;  // init I2C module clock, must be between 7-12 MHz (I2CPSC)
    I2caRegs.I2CCLKL         = 20; // init I2C SCL low time
    I2caRegs.I2CCLKH         = 20; // init I2C SCL high time


    // Take the I2C module out of reset
    I2caRegs.I2CMDR.bit.IRS = 1;

    // Configure the TX FIFO
    I2caRegs.I2CFFTX.all = 0x6000;   // Enable FIFO mode and TXFIFO, and interrupt when the FIFO is empty

    // Configure the RX FIFO
    I2caRegs.I2CFFRX.all = 0x2040;   // Enable RXFIFO, clear RXFFINT,

    EDIS;
}

static void disable_i2c_interrupts(void) {
    I2caRegs.I2CIER.bit.ARDY      = 0; // Interrupt on Register-access-ready
    I2caRegs.I2CIER.bit.NACK      = 0; // Interrupt on NACK
    I2caRegs.I2CIER.bit.SCD       = 0; // Interrupt on stop condition
    I2caRegs.I2CFFTX.bit.TXFFIENA = 0; // Interrupt on TX FIFO
    I2caRegs.I2CFFRX.bit.RXFFIENA = 0; // Interrupt on RX FIFO
}
static void enable_i2c_interrupts(void) {
    // Make sure FIFO interrupts are cleared before reenabling them
    I2caRegs.I2CFFTX.bit.TXFFINTCLR = 1;
    I2caRegs.I2CFFRX.bit.RXFFINTCLR = 1;

    //I2caRegs.I2CIER.bit.ARDY      = 1; // Interrupt on Register-access-ready
    I2caRegs.I2CIER.bit.NACK      = 1; // Interrupt on NACK
    I2caRegs.I2CIER.bit.SCD       = 1; // Interrupt on stop condition
    I2caRegs.I2CFFTX.bit.TXFFIENA = 1; // Interrupt on TX FIFO
    //I2caRegs.I2CFFRX.bit.RXFFIENA = 1; // Interrupt on RX FIFO
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
void InitI2CGpio() {
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
 * LED Cube Workflow:
 *
 * The application writer interfaces with the cube using the SET_LED() macro
 * found in main.h. This macro writes to the framebuffer, which stores the
 * brightness of every LED in the cube. The I2C driver reads from this buffer
 * and writes the values out to the LED drivers, as well as handling the layer
 * strobe. To start the I2C driver, the application must call start_cube(), but
 * after that everything is handled within the I2C interrupt.
 */

// I2C defines
#define REPEAT_MODE_START 0x26a0
#define REPEAT_MODE_STOP  0x0ea0
#define FIFO_DEPTH        4

// LED driver register addresses
#define SHUTDOWN_REG      0x00
#define DUTY_CYCLE_1_REG  0x05
#define UPDATE_REG        0x25
#define LED_CTRL_1_REG    0x2A
#define GLOBAL_CTRL_REG   0x4A
#define OUTPUT_FREQ_REG   0x4B
#define RESET_REG         0x4F

// The number of channels used on each driver
#define NUM_CHANNELS      25

static const uint16_t device_addrs[] = {0x3c|0, 0x3c|3, 0x3c|1};
static struct {
    enum {BEGIN_WRITE, WRITE, UPDATE} mode;
    uint16_t current_device; // index into device_addrs
    uint16_t *next_data;     // stores the next value to write to the fifo
    uint16_t *end;           // once next_data gets to this address, we're done writing
} I2c_State;

/*
 * Write two values to a single I2C slave device. This function spin loops
 * instead of using interrupts, and is used for testing and LED driver
 * initialization.
 */
static void i2c_write(uint16_t slave_addr, uint16_t reg_addr, uint16_t reg_val) {
    disable_i2c_interrupts();

    // Wait for a stop condition to be cleared before doing any operations
    while (I2caRegs.I2CMDR.bit.STP);

    // Set the slave address
    I2caRegs.I2CSAR = slave_addr;

    // Load the data into the FIFO
    I2caRegs.I2CDXR = reg_addr;
    I2caRegs.I2CDXR = reg_val;

    // Signal to the I2C module that it should start the transaction
    I2caRegs.I2CMDR.all = REPEAT_MODE_START;

    // Wait for the FIFO to empty itself
    while (!I2caRegs.I2CFFTX.bit.TXFFINT);
    I2caRegs.I2CFFTX.bit.TXFFINTCLR = 1;

    // Signal to the I2C module that it should stop the transaction
    I2caRegs.I2CMDR.all = REPEAT_MODE_STOP;
}

/*
 * Turns on every LED driver channel just a little bit, and blinks channel 2 of
 * driver 3. Uses interrupt free operation. Pins that aren't connected won't
 * turn on, and ones that are shorted to ground will light up very brightly.
 */
void led_driver_test(void) {
    int i, j;

    // for all the drivers
    for (i = 0; i < 3; i++) {
        // Turn on the driver
        i2c_write(device_addrs[i], SHUTDOWN_REG, 1);

        // Enable the appropriate channels
        for (j = 0; j < NUM_CHANNELS; j++) {
            i2c_write(device_addrs[i], LED_CTRL_1_REG + j, 1);
        }

        // Turn on each channel just a wee bit so that shorts to ground are obvious
        for (j = 0; j < NUM_CHANNELS; j++) {
            i2c_write(device_addrs[i], DUTY_CYCLE_1_REG + j, 2);
        }

        // Commit the changes to the LED driver
        i2c_write(device_addrs[i], UPDATE_REG, 0);
    }

    // fade one LED in and out
    uint16_t value = 1;
    uint16_t direction = 1;
    while (1) {
        // write a value to driver 2 channel 2
        i2c_write(device_addrs[2], DUTY_CYCLE_1_REG + 1, value);

        // commit the channel 2 value to the output
        i2c_write(device_addrs[2], UPDATE_REG, 0);

        // find the next value
        if (direction) {
            if (value == 0x80) {
                direction = 0;
                value >>= 1;
            } else {
                value <<= 1;
            }
        } else {
            if (value == 0x01) {
                direction = 1;
                value <<= 1;
            } else {
                value >>= 1;
            }
        }

        // wait for a bit
        ConfigCpuTimer(&CpuTimer0, 60, 25000);
        CpuTimer0.InterruptCount = 0;
        CpuTimer0Regs.TCR.all = 0x4001;
        while (CpuTimer0.InterruptCount < 1);
    }
}

void start_cube(void) {
    int i, j;

    // Initialize each LED driver using no-interrupt mode
    for (i = 0; i < LENGTH(device_addrs); i++) {
        // Turn it on
        i2c_write(device_addrs[i], SHUTDOWN_REG, 1);

        // Enable the appropriate channels
        for (j = 0; j < NUM_CHANNELS; j++) {
            i2c_write(device_addrs[i], LED_CTRL_1_REG + j, 1);
        }

        // Commit configuration changes
        i2c_write(device_addrs[i], UPDATE_REG, 0);
    }

    // Start in write mode, and on the first device.
    I2c_State.mode = WRITE;
    I2c_State.current_device = 0;
    I2c_State.next_data = framebuffer;
    I2c_State.end = I2c_State.next_data + NUM_CHANNELS;

    // Wait for any old transactions to complete
    while (I2caRegs.I2CMDR.bit.STP);

    // Set the next slave address
    I2caRegs.I2CSAR = device_addrs[I2c_State.current_device];

    // Load the FIFO with the register address we want to write to, which is
    // the first duty cycle register
    I2caRegs.I2CDXR = DUTY_CYCLE_1_REG;

    // Load the first few data values into the FIFO
    while (I2caRegs.I2CFFTX.bit.TXFFST != FIFO_DEPTH) {
        I2caRegs.I2CDXR = *(I2c_State.next_data++);
    }

    // Start a repeated transaction to get the snowball rolling
    enable_i2c_interrupts();
    I2caRegs.I2CMDR.all = REPEAT_MODE_START;
}

/*
 * Handles normal I2C interrupts
 */
__interrupt void i2c_isr1(void) {
    uint16_t i2c_int_status = I2caRegs.I2CSTR.all;

    switch (I2caRegs.I2CISRC.bit.INTCODE) {
        case I2C_NACK_ISRC: {
            // Slave device sent a NACK, stop for debug.
            __asm("     ESTOP0");
        }
        case I2C_SCD_ISRC: {
            // This I2C driver should never generate a stop condition while
            // interrupts are enabled. Stop here for debug.
            __asm("     ESTOP0");
            break;
        }
        default:
            // Unhandled interrupt type, stop for debug. The I2C interrupts were
            // probably configured wrong.
            __asm("     ESTOP0");
    }

    // Acknowledge the PIE interrupt so we can receive more on this group
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP8;
}

/*
 * Handles I2C FIFO interrupts
 */
__interrupt void i2c_isr2(void) {
    if (I2caRegs.I2CFFTX.bit.TXFFINT) {
        switch (I2c_State.mode) {
            case BEGIN_WRITE:
                // Set up for the next state
                I2c_State.mode = WRITE;
                I2c_State.end += NUM_CHANNELS;

                // Set the slave address to the next device
                I2caRegs.I2CSAR = device_addrs[I2c_State.current_device];

                // Load the FIFO with the register address we want to write to, which is
                // the first duty cycle register
                I2caRegs.I2CDXR = DUTY_CYCLE_1_REG;

                // Load the first few data values into the FIFO
                while (I2caRegs.I2CFFTX.bit.TXFFST != FIFO_DEPTH) {
                    I2caRegs.I2CDXR = *(I2c_State.next_data++);
                }

                // Start the new transaction
                I2caRegs.I2CMDR.all = REPEAT_MODE_START;

                break;

            case WRITE:
                // Load the FIFO until it is full or we run out of data
                while (I2caRegs.I2CFFTX.bit.TXFFST != FIFO_DEPTH) {
                    // Put the next value into the FIFO
                    I2caRegs.I2CDXR = *(I2c_State.next_data++);

                    // Check if we're out of data to load
                    if (I2c_State.next_data == I2c_State.end) {
                        // Switch to the next driver
                        I2c_State.current_device++;
                        I2c_State.mode = BEGIN_WRITE;

                        // If this was the last driver, switch instead to the update state
                        if (I2c_State.current_device == LENGTH(device_addrs)) {
                            I2c_State.mode = UPDATE;
                            I2c_State.current_device = 0;
                        }

                        break;
                    }
                }
                break;

            case UPDATE:
                // Write to the update register on the next driver
                I2caRegs.I2CSAR = device_addrs[I2c_State.current_device];
                I2caRegs.I2CDXR = UPDATE_REG;
                I2caRegs.I2CDXR = 0;
                I2caRegs.I2CMDR.all = REPEAT_MODE_START;

                I2c_State.current_device++;

                // If this was the last driver, begin writing new values
                if (I2c_State.current_device == LENGTH(device_addrs)) {
                    I2c_State.mode = BEGIN_WRITE;
                    I2c_State.current_device = 0;

                    // If this was the last layer, reset the frame pointer back
                    // to the beginning, and signal to the main application
                    // that we've completed a strobe of the entire cube.
                    //if (framebuffer + LENGTH(framebuffer) == I2c_State.next_data) {
                    { // Only use the first layer for now
                        I2c_State.next_data = framebuffer;
                        vsync = 1;
                    }
                }
                break;
        }

        // clear the TX fifo interrupt
        I2caRegs.I2CFFTX.bit.TXFFINTCLR = 1;
    } else {
        // Unhandled interrupt type, stop for debug. The I2C interrupts were
        // probably configured wrong.
        __asm("     ESTOP0");
    }

    // Acknowledge the PIE interrupt so we can receive more on this group
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP8;
}
