//###########################################################################
//
// FILE:    F2802x_Adc.c
//
// TITLE:   F2802x ADC Initialization & Support Functions.
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
// Defines
//
#define ADC_usDELAY  1000L


static struct {
    volatile size_t index;
    size_t length;
    volatile int32_t *data;
    volatile uint16_t run_flag;
} Sample_Buffer;


//
// InitAdc - This function initializes ADC to a known state.
// NOTE: ADC INIT IS DIFFERENT ON 2802x DEVICES COMPARED TO OTHER 28X DEVICES
//
void InitAdc(void)
{
    extern void DSP28x_usDelay(Uint32 Count);

    //
    //                               IMPORTANT
    // The Device_cal function, which copies the ADC calibration values from TI
    // reserved OTP into the ADCREFSEL and ADCOFFTRIM registers, occurs
    // automatically in the Boot ROM. If the boot ROM code is bypassed during
    // the debug process, the following function MUST be called for the ADC to
    // function according to specification. The clocks to the ADC MUST be
    // enabled before calling this function.
    // See the device data manual and/or the ADC Reference
    // Manual for more information.
    //
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1;
    (*Device_cal)();
    EDIS;

    //
    // To powerup the ADC the ADCENCLK bit should be set first to enable
    // clocks, followed by powering up the bandgap, reference circuitry, and
    // ADC core. Before the first conversion is performed a 5ms delay must be
    // observed after power up to give all analog circuits time to power up and
    // settle
    //

    //
    // Please note that for the delay function below to operate correctly the
    // CPU_RATE define statement in the f2802x_examples.h file must
    // contain the correct CPU clock period in nanoseconds.
    //
    EALLOW;
    AdcRegs.ADCCTL1.bit.ADCBGPWD  = 1;      // Power ADC BG
    AdcRegs.ADCCTL1.bit.ADCREFPWD = 1;      // Power reference
    AdcRegs.ADCCTL1.bit.ADCPWDN   = 1;      // Power ADC
    AdcRegs.ADCCTL1.bit.ADCENABLE = 1;      // Enable ADC
    AdcRegs.ADCCTL1.bit.ADCREFSEL = 0;      // Select internal BG
    EDIS;

    DELAY_US(ADC_usDELAY);         // Delay before converting ADC channels

    //
    // Set up ADC for audio sampling. Only SoC 0 is needed, since a sample rate
    // of 40 KHz should be slow enough to not need to overlay conversions.
    //
    EALLOW;
    AdcRegs.ADCCTL1.bit.INTPULSEPOS   = 1; // ADC interrupt signaled as result is latched into the result register
    AdcRegs.ADCCTL2.bit.ADCNONOVERLAP = 1; // ADC samples can't overlap
    AdcRegs.ADCCTL2.bit.CLKDIV2EN     = 0; // ADC clock equals system clock rate (60 MHz)

    AdcRegs.INTSEL1N2.bit.INT1CONT    = 0; // ADC interrupt pulse not generated until user clears interrput flag in ADCINTFLG
    AdcRegs.INTSEL1N2.bit.INT1E       = 1; // ADC interrupt is enabled
    AdcRegs.INTSEL1N2.bit.INT1SEL     = 0; // End of conversion 0 triggers interrupt 1

    AdcRegs.ADCSOC0CTL.bit.TRIGSEL    = 2; // Start of conversion 0 will be triggered by TINT1 (CPU timer 1)
    AdcRegs.ADCSOC0CTL.bit.CHSEL      = 0; // SOC0 will convert ADCINA0
    AdcRegs.ADCSOC0CTL.bit.ACQPS      = 6; // SOC0 will sample for 6+1 clock cycles
    EDIS;

}

//
// InitAdcAio -
//
void
InitAdcAio()
{
    EALLOW;

    //
    // Configure ADC pins using AIO regs
    // This specifies which of the possible AIO pins will be Analog input pins.
    // NOTE: AIO1, 3, 5, 7-9, 11, 13, 15 are analog inputs in all AIOMUX1
    // configurations. Comment out other unwanted lines.
    //

    //
    // Configure AIO2 for A2 (analog input) operation
    //
    GpioCtrlRegs.AIOMUX1.bit.AIO2 = 2;

    //
    // Configure AIO4 for A4 (analog input) operation
    //
    GpioCtrlRegs.AIOMUX1.bit.AIO4 = 2;

    //
    // Configure AIO6 for A6 (analog input) operation
    //
    GpioCtrlRegs.AIOMUX1.bit.AIO6 = 2;

    //
    // Configure AIO10 for B2 (analog input) operation
    //
    GpioCtrlRegs.AIOMUX1.bit.AIO10 = 2;

    //
    // Configure AIO12 for B4 (analog input) operation
    //
    GpioCtrlRegs.AIOMUX1.bit.AIO12 = 2;

    //
    // Configure AIO14 for B6 (analog input) operation
    //
    GpioCtrlRegs.AIOMUX1.bit.AIO14 = 2;

    EDIS;
}

//
// AdcoffsetSelfCal- This function re-calibrates the ADC zero offset error by
// converting the VREFLO reference with the ADC and modifying the ADCOFFTRIM
// register. VREFLO is sampled by the ADC using an internal MUX select which
// connects VREFLO to A5 without sacrificing an external ADC pin. This function
// calls two other functions:
//   - AdcChanSelect(channel) – selects the ADC channel to convert
//   - AdcConversion() – initiates several ADC conversions and returns the
//     average
//
void
AdcOffsetSelfCal()
{
    Uint16 AdcConvMean;
    EALLOW;
    AdcRegs.ADCCTL1.bit.ADCREFSEL = 0;      // Select internal reference mode

    //
    // Select VREFLO internal connection on B5
    //
    AdcRegs.ADCCTL1.bit.VREFLOCONV = 1;

    AdcChanSelect(13);                      // Select channel B5 for all SOC

    //
    // Apply artificial offset (+80) to account for a negative offset that may
    // reside in the ADC core
    //
    AdcRegs.ADCOFFTRIM.bit.OFFTRIM = 80;

    AdcConvMean = AdcConversion();          // Capture ADC conversion on VREFLO

    //
    // Set offtrim register with new value (i.e remove artical offset (+80) and
    // create a two's compliment of the offset error)
    //
    AdcRegs.ADCOFFTRIM.bit.OFFTRIM = 80 - AdcConvMean;

    //
    // Select external ADCIN5 input pin on B5
    //
    AdcRegs.ADCCTL1.bit.VREFLOCONV = 0;
    EDIS;
}

//
// AdcChanSelect- This function selects the ADC channel to convert by setting
// all SOC channel selects to a single channel.
//                              IMPORTANT
// This function will overwrite previous SOC channel select settings.
// Recommend saving the previous settings.
//
void
AdcChanSelect(Uint16 ch_no)
{
    AdcRegs.ADCSOC0CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC1CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC2CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC3CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC4CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC5CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC6CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC7CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC8CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC9CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC10CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC11CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC12CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC13CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC14CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC15CTL.bit.CHSEL= ch_no;
}

//
// AdcConversion - This function initiates several ADC conversions and returns
// the average. It uses ADCINT1 and ADCINT2 to "ping-pong" between SOC0-7 and
// SOC8-15 and is referred to as "ping-pong" sampling.
//                              IMPORTANT
// This function will overwrite previous ADC settings. Recommend saving
// previous settings.
//
Uint16 AdcConversion(void)
{
    Uint16 index, SampleSize, Mean, ACQPS_Value;
    Uint32 Sum;

    index       = 0;      // initialize index to 0

    //
    // set sample size to 256
    // (**NOTE: Sample size must be multiples of 2^x where is an integer >= 4)
    //
    SampleSize  = 256;

    Sum         = 0;     // set sum to 0
    Mean        = 999;   // initialize mean to known value

    //
    // Set the ADC sample window to the desired value
    // (Sample window = ACQPS + 1)
    //
    ACQPS_Value = 6;
    AdcRegs.ADCSOC0CTL.bit.ACQPS  = ACQPS_Value;
    AdcRegs.ADCSOC1CTL.bit.ACQPS  = ACQPS_Value;
    AdcRegs.ADCSOC2CTL.bit.ACQPS  = ACQPS_Value;
    AdcRegs.ADCSOC3CTL.bit.ACQPS  = ACQPS_Value;
    AdcRegs.ADCSOC4CTL.bit.ACQPS  = ACQPS_Value;
    AdcRegs.ADCSOC5CTL.bit.ACQPS  = ACQPS_Value;
    AdcRegs.ADCSOC6CTL.bit.ACQPS  = ACQPS_Value;
    AdcRegs.ADCSOC7CTL.bit.ACQPS  = ACQPS_Value;
    AdcRegs.ADCSOC8CTL.bit.ACQPS  = ACQPS_Value;
    AdcRegs.ADCSOC9CTL.bit.ACQPS  = ACQPS_Value;
    AdcRegs.ADCSOC10CTL.bit.ACQPS = ACQPS_Value;
    AdcRegs.ADCSOC11CTL.bit.ACQPS = ACQPS_Value;
    AdcRegs.ADCSOC12CTL.bit.ACQPS = ACQPS_Value;
    AdcRegs.ADCSOC13CTL.bit.ACQPS = ACQPS_Value;
    AdcRegs.ADCSOC14CTL.bit.ACQPS = ACQPS_Value;
    AdcRegs.ADCSOC15CTL.bit.ACQPS = ACQPS_Value;

    //
    // Enable ping-pong sampling
    //

    //
    // Enabled ADCINT1 and ADCINT2
    //
    AdcRegs.INTSEL1N2.bit.INT1E = 1;
    AdcRegs.INTSEL1N2.bit.INT2E = 1;

    //
    // Disable continuous sampling for ADCINT1 and ADCINT2
    //
    AdcRegs.INTSEL1N2.bit.INT1CONT = 0;
    AdcRegs.INTSEL1N2.bit.INT2CONT = 0;

    //
    // ADCINTs trigger at end of conversion
    //
    AdcRegs.ADCCTL1.bit.INTPULSEPOS = 1;

    //
    // Setup ADCINT1 and ADCINT2 trigger source
    //
    AdcRegs.INTSEL1N2.bit.INT1SEL = 6;      // EOC6 triggers ADCINT1
    AdcRegs.INTSEL1N2.bit.INT2SEL = 14;     // EOC14 triggers ADCINT2

    //
    // Setup each SOC's ADCINT trigger source
    //
    AdcRegs.ADCINTSOCSEL1.bit.SOC0  = 2;    // ADCINT2 starts SOC0-7
    AdcRegs.ADCINTSOCSEL1.bit.SOC1  = 2;
    AdcRegs.ADCINTSOCSEL1.bit.SOC2  = 2;
    AdcRegs.ADCINTSOCSEL1.bit.SOC3  = 2;
    AdcRegs.ADCINTSOCSEL1.bit.SOC4  = 2;
    AdcRegs.ADCINTSOCSEL1.bit.SOC5  = 2;
    AdcRegs.ADCINTSOCSEL1.bit.SOC6  = 2;
    AdcRegs.ADCINTSOCSEL1.bit.SOC7  = 2;
    AdcRegs.ADCINTSOCSEL2.bit.SOC8  = 1;    // ADCINT1 starts SOC8-15
    AdcRegs.ADCINTSOCSEL2.bit.SOC9  = 1;
    AdcRegs.ADCINTSOCSEL2.bit.SOC10 = 1;
    AdcRegs.ADCINTSOCSEL2.bit.SOC11 = 1;
    AdcRegs.ADCINTSOCSEL2.bit.SOC12 = 1;
    AdcRegs.ADCINTSOCSEL2.bit.SOC13 = 1;
    AdcRegs.ADCINTSOCSEL2.bit.SOC14 = 1;
    AdcRegs.ADCINTSOCSEL2.bit.SOC15 = 1;

    DELAY_US(ADC_usDELAY);        // Delay before converting ADC channels

    //
    // ADC Conversion
    //

    //
    // Force Start SOC0-7 to begin ping-pong sampling
    //
    AdcRegs.ADCSOCFRC1.all = 0x00FF;

    while( index < SampleSize )
    {
        //
        // Wait for ADCINT1 to trigger, then add ADCRESULT0-7 registers to sum
        //
        while (AdcRegs.ADCINTFLG.bit.ADCINT1 == 0)
        {

        }

        //
        // Must clear ADCINT1 flag since INT1CONT = 0
        //
        AdcRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;

        Sum += AdcResult.ADCRESULT0;
        Sum += AdcResult.ADCRESULT1;
        Sum += AdcResult.ADCRESULT2;
        Sum += AdcResult.ADCRESULT3;
        Sum += AdcResult.ADCRESULT4;
        Sum += AdcResult.ADCRESULT5;
        Sum += AdcResult.ADCRESULT6;

        //
        // Wait for SOC9 conversion to start, which gives time for SOC7
        // conversion result
        //
        while( AdcRegs.ADCSOCFLG1.bit.SOC9 == 1 )
        {

        }
        Sum += AdcResult.ADCRESULT7;

        //
        // Wait for ADCINT2 to trigger, then add ADCRESULT8-15 registers to sum
        //
        while (AdcRegs.ADCINTFLG.bit.ADCINT2 == 0)
        {

        }

        //
        // Must clear ADCINT2 flag since INT2CONT = 0
        //
        AdcRegs.ADCINTFLGCLR.bit.ADCINT2 = 1;

        Sum += AdcResult.ADCRESULT8;
        Sum += AdcResult.ADCRESULT9;
        Sum += AdcResult.ADCRESULT10;
        Sum += AdcResult.ADCRESULT11;
        Sum += AdcResult.ADCRESULT12;
        Sum += AdcResult.ADCRESULT13;
        Sum += AdcResult.ADCRESULT14;

        //
        // Wait for SOC1 conversion to start, which gives time for SOC15
        // conversion result
        //
        while( AdcRegs.ADCSOCFLG1.bit.SOC1 == 1 )
        {

        }
        Sum += AdcResult.ADCRESULT15;

        index+=16;

    }

    //
    // Disable ADCINT1 and ADCINT2 to STOP the ping-pong sampling
    //
    AdcRegs.INTSEL1N2.bit.INT1E = 0;
    AdcRegs.INTSEL1N2.bit.INT2E = 0;

    while(AdcRegs.ADCSOCFLG1.all != 0)
    {
        //
        // Wait for any pending SOCs to complete
        //
    }

    //
    // Clear any pending interrupts
    //
    AdcRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;
    AdcRegs.ADCINTFLGCLR.bit.ADCINT2 = 1;
    AdcRegs.ADCINTOVFCLR.bit.ADCINT1 = 1;
    AdcRegs.ADCINTOVFCLR.bit.ADCINT2 = 1;

    //
    // reset RR pointer to 32, so that next SOC is SOC0
    //
    AdcRegs.SOCPRICTL.bit.SOCPRIORITY = 1;
    while( AdcRegs.SOCPRICTL.bit.SOCPRIORITY != 1 );
    {

    }
    AdcRegs.SOCPRICTL.bit.SOCPRIORITY = 0;
    while( AdcRegs.SOCPRICTL.bit.SOCPRIORITY != 0 );
    {

    }

    Mean = Sum / SampleSize;    // Calculate average ADC sample value

    return Mean;                // return the average
}


/*
 * Audio sampling control flow:
 * 1. Set up sample buffer
 * 2. Enable timer interrupt
 * 3. Timer interrupt triggers an SoC
 * 4. When SoC is complete, and ADC interrupt is triggered
 * 5. ADC interrupt places sample in buffer
 * 6. Once the buffer is full, ADC interrupt turns off timer and signals to the main application
 */
void adc_start_sampling(volatile int32_t *buffer, size_t length) {
    // Since the ADC interrupt stuffs a 0 valued imaginary component after each
    // real-valued sample, the length must be even
    if (length % 2)
        return;

    // Set the ISR state info
    Sample_Buffer.data = buffer;
    Sample_Buffer.length = length;
    Sample_Buffer.index = 0;
    Sample_Buffer.run_flag = 1;
}

int adc_done_sampling(void) {
    return Sample_Buffer.length == Sample_Buffer.index;
}

/*
 * The ADC is set up to sample at 40 KHz. It then averages every 40 samples together to achieve an effective sample rate of 1 KHz
 */
__interrupt void adc_int1_isr(void) {
    // sample buffer for averaging the last several samples together
    static uint16_t buffer[40];
    static uint16_t index = 0;
    uint16_t DAC_data;

    // Get the converted sample from SOC0
    buffer[index++] = AdcResult.ADCRESULT0;
    DAC_data = AdcResult.ADCRESULT0 << 4;
    DAC_send_2(DAC_data);
    if ((Sample_Buffer.run_flag != 0) &&( index == LENGTH(buffer))) {
        index = 0;

        // Average all 40 samples together
        uint32_t result = 0;
        uint16_t i;
        for (i = 0; i < LENGTH(buffer); i++) {
            result += buffer[i]; // unlikely to overflow, since these are 12 bit ADC samples, going into a 32 bit wide integer
        }
        result /= LENGTH(buffer);

        //Low pass filter test
        //DAC_data = result << 4;
        //DAC_send_2(DAC_data);

        // Place the averaged 12-bit samples in the buffer. Put the most
        // significant bit of the sample in bit 16 of the result. We do this
        // because the FFT library uses a Q30 fixed point number representation,
        // which places the decimal point between bits 31 and 30. Shifting over the
        // sample allows us to take advantage of more bits.
        Sample_Buffer.data[Sample_Buffer.index++] = result << 15;

        // If done, turn off CPU timer 1. CPU timer 1 triggers the start of an ADC
        // sample, so turning it off stops ADC sampling.
        if (Sample_Buffer.index == Sample_Buffer.length) {
            Sample_Buffer.run_flag = 0;
          //  CpuTimer1Regs.TCR.bit.TSS = 1; Dont turn off timer, just tell
        }
    }

    AdcRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;    // clear interrupt
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;  // acknowledge this interrupt with the PIE
}




//
// End of file
//

