//###########################################################################
//
// Michael just trying to get GPIO pins set up for LCD display
//
//###########################################################################

//
// Included Files
//
#include "F2802x_Device.h"     // Headerfile Include File
#include "f2802x_examples.h"   // Examples Include File
#include "main.h"              // functions in this file in main.h

//
// InitLCD - This function initializes the Gpio as used by LCD display
//Author - Michael
//CS_SCD = GPIO6
// SDA_LCD = GPIO32
// SCL_LCD = GPIO33
// A0_LCD = GPIO7
//
uint16_t LCD_data;
void
InitLCD(void)
{
    EALLOW;

    GpioCtrlRegs.GPAPUD.bit.GPIO6 = 0;   // Enable pull-up on GPIO6

    GpioCtrlRegs.GPADIR.all |= 0x00C0; //GPIO 6 and 7 are outputs
    GpioCtrlRegs.GPBDIR.all |= 0x0003; //GPIO 32 and 33 are outputs

    GpioIntRegs.GPIOXINT1SEL.all = 0x0005; //Button on GPIO5 is interrupt

    
    //possibly simulate button debouncing with "6 sample qualification"?
    //GpioCtrlRegs.GPAQSEL1.all = 0x0800;    // GPIO5 - 6 samples


    EDIS;
}

void
LCD_init2(void){
    //initialize display
    LCD_data = 0x0005;
    LCDTimerStart();
    LCDWait();

    LCD_data = 0x00AE;
    LCDTimerStart();
    LCDWait();

    LCD_data = 0x00C0;
    LCDTimerStart();
    LCDWait();

    LCD_data = 0x00A2;
    LCDTimerStart();
    LCDWait();

    LCD_data = 0x002F;
    LCDTimerStart();
    LCDWait();

    LCD_data = 0x0021;
    LCDTimerStart();
    LCDWait();

    LCD_data = 0x0081;
    LCDTimerStart();
    LCDWait();

    LCD_data = 0x003F;
    LCDTimerStart();
    LCDWait();

}

void
LCD_display1(void){
    LCD_data = 0x003F;
    LCDTimerStart();
    LCDWait();
}

void
LCDWait(void){
    uint16_t x = 0;
    uint16_t cycWait = 130;
    while(x < cycWait) //wait for all data to send
    {
        x++;
    }
}

void
LCDTimerStart(void)
{
    CpuTimer2.InterruptCount = 0;
    CpuTimer2Regs.TCR.bit.TSS = 0;
}
void
LCDTimerStop(void){
    CpuTimer2Regs.TCR.bit.TSS = 1;
}

//
// handles main LCD screen commands based on number of interrupts this cycle
//
void
LCDMainAction(void){
    uint32_t count = CpuTimer2.InterruptCount;
    switch (count) {
            //cases based on how many interrupts have occurred
            //data flows as follows:
            // 1. drop CS and set A0 and SI to first values, clock low
            // 2. clock goes high
            // 3. clock goes low, change SI
            // 4. clock goes high
            // repeat until all 8 values passed
            // 17. set clock low, CS high, disable timer
            case 1:
                GpioDataRegs.GPACLEAR.bit.GPIO6 = 1; //CS
                GpioDataRegs.GPACLEAR.bit.GPIO7 = 1; // A0 = 0 until 8th active edge of clk MAYBE UNNECCESSARY HERE
                if(((LCD_data >> 7) & 0x0001) == 1) {
                    GpioDataRegs.GPBSET.bit.GPIO32 = 1;
                }
                else {
                    GpioDataRegs.GPBCLEAR.bit.GPIO32 = 1;
                }
                GpioDataRegs.GPBCLEAR.bit.GPIO33 = 1; //CLK
                break;
            case 2:
                GpioDataRegs.GPBTOGGLE.bit.GPIO33 = 1; //CLK
                break;
            case 3:
                GpioDataRegs.GPBTOGGLE.bit.GPIO33 = 1; //CLK
                if(((LCD_data >> 6) & 0x0001) == 1){
                     GpioDataRegs.GPBSET.bit.GPIO32 = 1;
                }
                else {
                     GpioDataRegs.GPBCLEAR.bit.GPIO32 = 1;
                }
                break;
            case 4:
                GpioDataRegs.GPBTOGGLE.bit.GPIO33 = 1; //CLK
                break;
            case 5:
                GpioDataRegs.GPBTOGGLE.bit.GPIO33 = 1; //CLK
                if(((LCD_data >> 5) & 0x0001) == 1){
                     GpioDataRegs.GPBSET.bit.GPIO32 = 1;
                }
                else {
                     GpioDataRegs.GPBCLEAR.bit.GPIO32 = 1;
                }
                break;
            case 6:
                GpioDataRegs.GPBTOGGLE.bit.GPIO33 = 1; //CLK
                break;
            case 7:
                GpioDataRegs.GPBTOGGLE.bit.GPIO33 = 1; //CLK
                if(((LCD_data >> 4) & 0x0001) == 1){
                     GpioDataRegs.GPBSET.bit.GPIO32 = 1;
                }
                else {
                     GpioDataRegs.GPBCLEAR.bit.GPIO32 = 1;
                }
                break;
            case 8:
                GpioDataRegs.GPBTOGGLE.bit.GPIO33 = 1; //CLK
                break;
            case 9:
                GpioDataRegs.GPBTOGGLE.bit.GPIO33 = 1; //CLK
                if(((LCD_data >> 3) & 0x0001) == 1){
                     GpioDataRegs.GPBSET.bit.GPIO32 = 1;
                }
                else {
                     GpioDataRegs.GPBCLEAR.bit.GPIO32 = 1;
                }
                break;
            case 10:
                GpioDataRegs.GPBTOGGLE.bit.GPIO33 = 1; //CLK
                break;
            case 11:
                GpioDataRegs.GPBTOGGLE.bit.GPIO33 = 1; //CLK
                if(((LCD_data >> 2) & 0x0001) == 1){
                     GpioDataRegs.GPBSET.bit.GPIO32 = 1;
                }
                else {
                     GpioDataRegs.GPBCLEAR.bit.GPIO32 = 1;
                }
                break;
            case 12:
                GpioDataRegs.GPBTOGGLE.bit.GPIO33 = 1; //CLK
                break;
            case 13:
                GpioDataRegs.GPBTOGGLE.bit.GPIO33 = 1; //CLK
                if(((LCD_data >> 1) & 0x0001) == 1){
                     GpioDataRegs.GPBSET.bit.GPIO32 = 1;
                }
                else {
                     GpioDataRegs.GPBCLEAR.bit.GPIO32 = 1;
                }
                break;
            case 14:
                GpioDataRegs.GPBTOGGLE.bit.GPIO33 = 1; //CLK
                break;
            case 15:
                GpioDataRegs.GPBTOGGLE.bit.GPIO33 = 1; //CLK
                if(((LCD_data >> 8) & 0x0001) == 1){
                    GpioDataRegs.GPASET.bit.GPIO7 = 1; //set A0
                }
                else{
                    GpioDataRegs.GPACLEAR.bit.GPIO7 = 1; //clear A0
                }
                if(((LCD_data >> 0) & 0x0001) == 1){
                     GpioDataRegs.GPBSET.bit.GPIO32 = 1;
                }
                else {
                     GpioDataRegs.GPBCLEAR.bit.GPIO32 = 1;
                }
                break;
            case 16:
                GpioDataRegs.GPBTOGGLE.bit.GPIO33 = 1; //CLK
                break;
            case 17:
                GpioDataRegs.GPBCLEAR.bit.GPIO33 = 1; //CLK
                GpioDataRegs.GPASET.bit.GPIO6 = 1; //CS
                GpioDataRegs.GPACLEAR.bit.GPIO7 = 1; // clear AO
                GpioDataRegs.GPBCLEAR.bit.GPIO32 = 1; // set low so it looks pretty
                LCDTimerStop(); //stop timer
                break;
    }


}
//
// End of file
//

