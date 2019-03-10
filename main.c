/*
 * C2000Ware includes
 */
#include <DSP28x_Project.h>
#include <f2802x_globalprototypes.h>
#include <gpio.h>

/*
 * Forward declarations
 */
__interrupt void cpu_timer0_isr(void);
__interrupt void sci_rx_isr(void);
__interrupt void sci_tx_isr(void);

/**
 * main.c
 */
int main(void)
{
    //
    // WARNING: Always ensure you call memcpy before running any functions from
    // RAM InitSysCtrl includes a call to a RAM based function and without a
    // call to memcpy first, the processor will go "into the weeds"
    //
#ifdef _FLASH
    memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (size_t)&RamfuncsLoadSize);
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
    PieVectTable.TINT0 = &cpu_timer0_isr;
    PieVectTable.SCIRXINTA = &sci_rx_isr;
    PieVectTable.SCITXINTA = &sci_tx_isr;
    EDIS;      // This is needed to disable write to EALLOW protected registers

    //
    // Step 4. Initialize the Device Peripherals.
    //
    InitCpuTimers();
    InitAdc();
    InitSci();

    //
    // Configure CPU-Timer 0 to interrupt every 500 milliseconds:
    // 60MHz CPU Freq, 50 millisecond Period (in uSeconds)
    //
    ConfigCpuTimer(&CpuTimer0, 60, 500000);

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
    IER |= M_INT1; // CPU timer 0 comes in on INT1
    IER |= M_INT9; // SCI interrupts come in on INT9

    //
    // Enable PIE interrupts
    //
    PieCtrlRegs.PIECTRL.bit.ENPIE = 1; // Enable the PIE block
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1; // Enable group 1 interrupt 7
    PieCtrlRegs.PIEIER9.bit.INTx1 = 1; // Enable group 9 interrupt 1 (SCARXINTA)
    PieCtrlRegs.PIEIER9.bit.INTx2 = 1; // Enable group 9 interrupt 2 (SCATXINTA)

    //
    // Enable global Interrupts and higher priority real-time debug events
    //
    EINT;           // Enable Global interrupt INTM
    ERTM;           // Enable Global realtime interrupt DBGM

    //
    // Step 6. IDLE loop. Just sit and loop forever (optional)
    //
    while (1) {
        sci_send_char(sci_get_char());
    }

    return 0;
}

/*
 * cpu_timer0_isr
 *
 * Toggles GPIO0
 */
__interrupt void cpu_timer0_isr(void)
{
    CpuTimer0.InterruptCount++;

    //
    // Toggle GPIO0 once per 500 milliseconds
    //
    GpioDataRegs.GPATOGGLE.bit.GPIO0 = 1;

    //
    // Acknowledge this interrupt to receive more interrupts from group 1
    //
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}
