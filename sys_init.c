/*
 * Author: Alex Wranovsky
 *
 * Lots of this is taken from the TI examples.
 */

#include <string.h>
#include <DSP28x_Project.h>
#include <f2802x_globalprototypes.h>
#include <gpio.h>
#include "main.h"

void sys_init(void) {
    //
    // WARNING: Always ensure you call memcpy before running any functions from
    // RAM InitSysCtrl includes a call to a RAM based function and without a
    // call to memcpy first, the processor will go "into the weeds"
    //
#ifdef _FLASH
    memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (size_t)&RamfuncsLoadSize);
    InitFlash();
#endif

    //
    // Step 1. Initialize System Control:
    // PLL, WatchDog, enable Peripheral Clocks
    // This example function is found in the f2802x_SysCtrl.c file.
    //
    InitSysCtrl();

    //
    // Step 2. Initialize GPIO:
    // This example function is found in the f2802x_Gpio.c file and
    // illustrates how to set the GPIO to it's default state.
    //
    InitGpio();

    //
    // Step 3. Clear all interrupts and initialize PIE vector table:
    // Disable CPU interrupts
    //
    DINT;

    //
    // Initialize the PIE control registers to their default state.
    // The default state is all PIE interrupts disabled and flags
    // are cleared.
    // This function is found in the f2802x_PieCtrl.c file.
    //
    InitPieCtrl();

    //
    // Disable CPU interrupts and clear all CPU interrupt flags
    //
    IER = 0x0000;
    IFR = 0x0000;

    //
    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    // This will populate the entire table, even if the interrupt
    // is not used in this example.  This is useful for debug purposes.
    // The shell ISR routines are found in f2802x_DefaultIsr.c.
    // This function is found in f2802x_PieVect.c.
    //
    InitPieVectTable();

    //
    // Interrupts that are used in this example are re-mapped to
    // ISR functions found within this file.
    //
    EALLOW;    // This is needed to write to EALLOW protected registers
    PieVectTable.TINT0     = &cpu_timer0_isr;
    PieVectTable.TINT1     = &cpu_timer1_isr;
    PieVectTable.TINT2     = &cpu_timer2_isr;
    PieVectTable.SCIRXINTA = &sci_rx_isr;
    PieVectTable.SCITXINTA = &sci_tx_isr;
    PieVectTable.ADCINT1   = &adc_int1_isr;
    EDIS;      // This is needed to disable write to EALLOW protected registers

    //
    // Step 4. Initialize the Device Peripherals.
    //
    InitCpuTimers();
    InitAdc();
    InitSci();
    InitSpi();
    InitSpiGpio();
    InitSpiFifos();
    InitLCD();
    //
    // Configure CPU-Timer 0 to interrupt every 500 milliseconds:
    // 60MHz CPU Freq, 500 millisecond Period (in uSeconds)
    //
    ConfigCpuTimer(&CpuTimer0, 60, 500000);

    //
    // Configure CPU-Timer 1 to interrupt every 25 microseconds:
    // 60MHz CPU Freq, 25 microsecond Period
    //
    // This Timer is used to trigger the ADC
    //
    ConfigCpuTimer(&CpuTimer1, 60, 25);

    //
    // Configure CPU-Timer 2 to interrupt every 1 microseconds (1000 nano):
    //
    // This Timer is used to mimic SPI clock for LCD screen
    //
    ConfigCpuTimer(&CpuTimer2, 60, 1);

    //
    // To ensure precise timing, use write-only instructions to write to the
    // entire register. Therefore, if any of the configuration bits are changed
    // in ConfigCpuTimer and InitCpuTimers (in f2802x_CpuTimers.h), the
    // below settings must also be updated.
    //

    //
    // Use write-only instruction to set TSS bit = 0
    //
    CpuTimer0Regs.TCR.all = 0x4001;
    //CpuTimer1Regs.TCR.all = 0x4001; // adc code will start and stop timer 1
                                     // LCD code will start and stop timer 2
    //
    // Step 5. User specific code, enable interrupts
    //

    //
    // Configure GPIO0 as a GPIO output pin
    //
    EALLOW;
    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO0 = 1;
    EDIS;

    //
    // Enable CPU interrupts
    //
    IER |= M_INT1;  // CPU timer 0 and ADC interrupt 1 and XINT1 come in on INT1
    IER |= M_INT9;  // SCI interrupts come in on INT9
    IER |= M_INT13; // CPU timer 1 comes in on INT13
    IER |= M_INT14; // CPU timer 2 comes in on INT14


    //
    // Enable PIE interrupts
    //
    PieCtrlRegs.PIECTRL.bit.ENPIE  = 1; // Enable the PIE block
    PieCtrlRegs.PIEIER1.bit.INTx1  = 1; // Enable group 1 interrupt 1 (ADCINT1)
    PieCtrlRegs.PIEIER1.bit.INTx4  = 1; // enable group 1 interrupt 4 (GPIO XINT1)
    PieCtrlRegs.PIEIER1.bit.INTx7  = 1; // Enable group 1 interrupt 7
    PieCtrlRegs.PIEIER9.bit.INTx1  = 1; // Enable group 9 interrupt 1 (SCARXINTA)
    PieCtrlRegs.PIEIER9.bit.INTx2  = 1; // Enable group 9 interrupt 2 (SCATXINTA)


    //
    // Enable global Interrupts and higher priority real-time debug events
    //
    EINT;           // Enable Global interrupt INTM
    ERTM;           // Enable Global realtime interrupt DBGM
}
