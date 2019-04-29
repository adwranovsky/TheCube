//###########################################################################
//
// FILE:   F2802x_Spi.c
//
// TITLE:  F2802x SPI Initialization & Support Functions.
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
#include "spi.h"               // bitmask stuff
#include "main.h"



#define SPI_BRR        3 // clock is at 3.7 MHz --->>> may need to slow down to match rate of data from ADC (80 KHz)

volatile uint16_t readyForMore = 0; // SPI TX FIFO is ready for new data

//
// InitSPI - This function initializes the SPI(s) to a known state.
//
void
InitSpi(void)
{
    // Initialize SPI-A

    // Set reset low before configuration changes
    // Clock polarity (0 == rising, 1 == falling)
    // 16-bit character
    // Enable loop-back <---- we disabled
    SpiaRegs.SPICCR.bit.SPISWRESET = 0;
    SpiaRegs.SPICCR.bit.CLKPOLARITY = 0;
    SpiaRegs.SPICCR.bit.SPICHAR = (16 - 1);
    SpiaRegs.SPICCR.bit.SPILBK = 0;

    // Enable master (0 == slave, 1 == master)
    // Enable transmission (Talk)
    // Clock phase (0 == normal, 1 == delayed)
    // SPI interrupts are disabled
    SpiaRegs.SPICTL.bit.MASTER_SLAVE = 1;
    SpiaRegs.SPICTL.bit.TALK = 1;
    SpiaRegs.SPICTL.bit.CLK_PHASE = 0;
    SpiaRegs.SPICTL.bit.SPIINTENA = 1; //leave disabled, described as: SPIINT/SPIRXINT Transmit interrupt/ Receive Interrupt in non FIFO mode (referred to as SPIINT) Receive interrupt in FIFO mode

    // Set the baud rate
    SpiaRegs.SPIBRR = SPI_BRR;

    // Set FREE bit
    // Halting on a breakpoint will not halt the SPI
    //SpiaRegs.SPIPRI.bit.FREE = 1;




    //TX_BITS = 0x4061;  //reset and enable interrupts and FIFOs(interrupts when no words left in TX buffer) POSSIBLY ONE OR THE OTHER WITH SPICTL register
    SpiaRegs.SPIFFTX.all = 0x8000;
   // RX_BITS = 0x604F;
    SpiaRegs.SPIFFRX.all = 0x0000;
    // Release the SPI from reset
    SpiaRegs.SPICCR.bit.SPISWRESET = 1;
}

//
// InitSpiGpio - This function initializes GPIO pins to function as SPI pins
//
// Each GPIO pin can be configured as a GPIO pin or up to 3 different
// peripheral functional pins. By default all pins come up as GPIO
// inputs after reset.
//
// Caution:
// For each SPI peripheral
// Only one GPIO pin should be enabled for SPISOMO operation.
// Only one GPIO pin should be enabled for SPISOMI operation.
// Only one GPIO pin should be enabled for SPICLKA operation.
// Only one GPIO pin should be enabled for SPISTEA operation.
// Comment out other unwanted lines.
//
void
InitSpiGpio()
{
    InitSpiaGpio();
}

//
// InitSpiaGpio - 
//
void
InitSpiaGpio()
{
    EALLOW;
    
    //
    // Enable internal pull-up for the selected pins
    // Pull-ups can be enabled or disabled by the user.
    // This will enable the pullups for the specified pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPAPUD.bit.GPIO16 = 0;   // Enable pull-up on GPIO16(SPISIMOA)
//    GpioCtrlRegs.GPAPUD.bit.GPIO17 = 0;   // Enable pull-up on GPIO17(SPISOMIA)     <----possibly comment
    GpioCtrlRegs.GPAPUD.bit.GPIO18 = 0;   // Enable pull-up on GPIO18(SPICLKA)
    GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;   // Enable pull-up on GPIO19(SPISTEA)

    //
    // Set qualification for selected pins to asynch only
    // This will select asynch (no qualification) for the selected pins.
    // Comment out other unwanted lines.
    //
 //   GpioCtrlRegs.GPAQSEL2.bit.GPIO16 = 3; // Asynch input GPIO16 (SPISIMOA)
 //   GpioCtrlRegs.GPAQSEL2.bit.GPIO17 = 3; // Asynch input GPIO17 (SPISOMIA)
 //   GpioCtrlRegs.GPAQSEL2.bit.GPIO18 = 3; // Asynch input GPIO18 (SPICLKA)
 //   GpioCtrlRegs.GPAQSEL2.bit.GPIO19 = 3; // Asynch input GPIO19 (SPISTEA)

    //
    // Configure SPI-A pins using GPIO regs
    // This specifies which of the possible GPIO pins will be SPI functional
    // pins. Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 1;    // Configure GPIO16 as SPISIMOA
 //   GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 1;    // Configure GPIO17 as SPISOMIA   <----possibly comment
    GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 1;    // Configure GPIO18 as SPICLKA
    GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 1;    // Configure GPIO19 as SPISTEA

    EDIS;
}

//
//will write 16 bits to SPI TX buffer
//
void SPI_write_16(const uint16_t data)
{
 //   SpiaRegs.SPIDAT = data;
    SpiaRegs.SPITXBUF = data;
}

//
//Data to DAC is two dont-cares, followed by two bits for mode, followed by 8
// bits of data, and finally 4 more dont cares.
// Given 8 bits of data, writes data to SPI in DAC-friendly format.
// Author - Michael
//
void DAC_write(const uint16_t data)
{
    uint16_t DACdata = 0x0000;

    //functions will handle audio distortions based on button presses
    if(curr_display == 1){ // half freq
         DACdata = (data / 8) << 4;
         SPI_write_16(DACdata);
    }
    else if(curr_display == 2){ // decrease resolution
        DACdata = data;
        SPI_write_16(DACdata);
    }
    else if(curr_display == 3){ // decrease resolution
          //  DACdata = (data << 4) & 0x0F00;
            DACdata = (data /16) << 4;                                    //  -------------
            SPI_write_16(DACdata);
        }
    else if(curr_display == 4){ // decrease resolution
          //  DACdata = (data << 4) & 0x0F00;
            DACdata = (data / 32) << 5;
            SPI_write_16(DACdata);
        }
    else if(curr_display == 5){ // decrease resolution
          //  DACdata = (data << 4) & 0x0F00;
            DACdata = data && 0x0F10;
            SPI_write_16(DACdata);
        }

    else if(curr_display == 6){ //double freq
        DACdata = data - 600;
      //  DACdata = ((data * 2) << 4) & 0x0FF0; //need to keep data framed
        SPI_write_16(DACdata);
    }
    else{
        DACdata = data << 4;
        SPI_write_16(DACdata);
    }
}


//
//prototype function to send a single ADC audio samples to DAC, called from adc interrupt
//Author - Michael
//
void DAC_send_2(uint16_t ADC_data)
{
    if (readyForMore == 0){
        readyForMore = 1;  //NOT ready for more
        DAC_write(ADC_data);
    }
}


__interrupt void spi_isr(void){
    SpiaRegs.SPICCR.bit.SPISWRESET = 0; //clears SPI INT FLAG
    //SpiaRegs.SPIFFTX.bit.TXFFINTCLR = 1; //clears TX FIFO interrupt
    SpiaRegs.SPICCR.bit.SPISWRESET = 1;  //reenables SPI transmit capabilities
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP6;
    readyForMore = 0;
}
//
// End of file
//

