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
// InitLCD - This file initializes the Gpio as used by LCD display and displays stupid looking hand drawn images
//Author - Michael
//CS_SCD = GPIO6
// SDA_LCD = GPIO32
// SCL_LCD = GPIO33
// A0_LCD = GPIO7
//
volatile uint16_t button_pushed = 0;
uint16_t LCD_data; // 16 bits of data to send to LCD display, lower 8 are command/data and bit 9 is A0
volatile uint16_t curr_display = 1; // number of current display, start at 1 so that first button interrupt correctly moves to next display
uint16_t *LCD_data_array;

// LCD DISPLAY IMAGES:
// go to  http://dot2pic.com/  and select 128 columns, 32 rows, and monochromatic 8 pixels per byte vertical
// paste resulting array below
uint16_t LCD_data_array1[]={0x00,0x00,0x1e,0x0f,0x00,0x00,0x00,0x03,0x07,0x0f,0x00,0x00,0x00,0x00,0x00,0x1e,0x13,0x0f,0x07,0x07,0x07,0x07,0x07,0x07,0x03,0x07,0x07,0x01,0x03,0x03,0x03,0x02,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x03,0x00,0x07,0x00,0x00,0x07,0x07,0x05,0x03,0x1f,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x1e,0x1f,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x07,0x03,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x0f,0x08,0x03,0x0f,0x1f,0x04,0x00,0x00,0x00,0x00,0x70,0x1c,0x06,0x02,0x02,0x02,0x82,0x0c,0x00,0x00,0x00,0x00,0x00,0x00,0x7f,0xff,0xff,0xff,0xe3,0xc3,0x81,0x01,0x81,0x81,0x81,0x81,0x80,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xfd,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x2f,0xfe,0x7f,0x7c,0xbc,0xa0,0x00,0x00,0xfb,0xdf,0xcf,0xe0,0xf8,0xde,0x3f,0x06,0x01,0x00,0x00,0x00,0x00,0xbf,0xb7,0xff,0x00,0x00,0x00,0x00,0x00,0x01,0x3e,0xfb,0x5f,0x40,0x00,0x00,0x01,0x03,0x0f,0x1c,0x3e,0x70,0x40,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xe0,0x30,0x88,0xe8,0xfc,0x2f,0x3d,0x0e,0x0d,0x07,0x0f,0x01,0x06,0x0c,0x0b,0x27,0xfb,0x7c,0xdc,0xe0,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xff,0xff,0xff,0xc0,0xc0,0xc0,0x80,0xc0,0xc0,0xc0,0x80,0xc0,0xc0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xbf,0xbe,0xfd,0x01,0x00,0x00,0x00,0x00,0x07,0xdf,0xff,0x7f,0xf8,0x00,0x00,0x00,0x00,0x02,0xff,0xeb,0xff,0x00,0x00,0x00,0x80,0xe0,0xf8,0x7f,0x0f,0x01,0x00,0xff,0xfa,0xab,0x00,0x00,0x00,0x00,0x00,0xe0,0x5f,0xff,0xff,0x0d,0x1f,0x3f,0xf7,0xa3,0x41,0x09,0x05,0x01,0x02,0x00,0x02,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xc0,0xf7,0xff,0xff,0xff,0xc0,0x80,0x00,0x01,0x01,0x01,0x01,0x03,0x0f,0x07,0x01,0x01,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xfc,0xf8,0xfc,0xfc,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xc0,0xf0,0x7e,0x1d,0x1f,0x1f,0x3e,0xfc,0xf0,0xe0,0x00,0x00,0x00,0x00,0x00,0x00,0xf0,0xbc,0xfc,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xe0,0xdc,0xdf,0xf8,0x70,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0xf8,0xff,0xc9,0x00,0x80,0x80,0x00,0x40,0xc0,0xc0,0xa0,0xe0,0xe0,0xe0,0xa0,0x70,0xd8,0x08,0x04,0x0c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x94,0xfe,0xfe,0x2f,0x48,0x00,0x00,0x00,0x00,0x98,0xb8,0xf0,0xf0,0xf0,0xb0,0x98,0x00};
uint16_t LCD_data_array2[]={0x00,0x00,0x18,0x18,0x18,0x1f,0x1f,0x18,0x18,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x07,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x07,0x00,0x04,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xf8,0xf0,0x00,0x00,0xe0,0xb0,0x90,0x90,0xb0,0xe0,0x00,0x00,0x00,0xf0,0x00,0x00,0x00,0x00,0x20,0xf0,0x90,0x90,0xf0,0x38,0x08,0x00,0x00,0xf8,0x00,0x00,0xf8,0x00,0x01,0x03,0x66,0x3c,0x18,0x30,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x02,0x00,0x00,0x00,0x00,0x07,0x08,0x0c,0x04,0x06,0x02,0x03,0x81,0xc1,0x61,0x19,0x0c,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0f,0x18,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0e,0x03,0x00,0x00,0x00,0x00,0x1e,0x03,0x00,0x00,0xc0,0x60,0x1c,0x3f,0x21,0x23,0x32,0x1e,0x00,0x40,0x38,0x0e,0x00,0xe0,0x30,0x10,0x10,0x10,0x10,0x10,0xc0,0x40,0x20,0x20,0x20,0x20,0xc0,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xe0,0x18,0x0c,0x04,0x02,0x02,0x03,0x01,0x03,0x02,0x06,0x04,0x0c,0x18,0x10,0x80,0x60,0x30,0x10,0x10,0x30,0xc0,0xc0,0x60,0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xfe,0x86,0x86,0x84,0x8c,0x4c,0x78,0x00,0x78,0x08,0x08,0x78,0x1c,0x04,0x00,0x00,0x70,0xc8,0x48,0x68,0xf8,0x00,0x00,0x00,0x70,0xf8,0xac,0xa4,0x64,0x00,0x00,0x00,0x00,0x00,0x00};
//uint16_t LCD_data_array3[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x07,0x07,0x1e,0x1e,0x1c,0x1c,0x3c,0x3c,0x38,0x38,0x38,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x1f,0x3f,0x1f,0x0e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x1f,0x1f,0x1f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x07,0x07,0x07,0x0f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x04,0x08,0x30,0x21,0x6f,0x51,0x61,0x63,0x42,0x43,0x41,0x21,0x22,0x22,0x14,0x18,0x0f,0x09,0x0f,0x06,0x04,0x02,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0x81,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0x00,0x01,0x03,0x03,0x03,0x03,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xe1,0xf1,0xf0,0xf1,0xf1,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0x6c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1f,0x30,0xc7,0x1e,0x33,0xe6,0xa4,0x7f,0x2b,0x2c,0x28,0x30,0x30,0x31,0x30,0xfa,0x0d,0x5f,0x51,0xe7,0xb8,0xe0,0x6c,0x01,0x00,0x03,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xe0,0xf8,0xfc,0xff,0x3f,0x0f,0x07,0x03,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xf8,0xc0,0xe0,0xf0,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xee,0xf9,0x80,0xf8,0x0c,0x84,0x82,0xe7,0xea,0x3a,0x3e,0x3b,0x2b,0x2c,0x3c,0x31,0xe0,0x71,0x8f,0x01,0x83,0x46,0x2d,0xf8,0x78,0xdf,0xf0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xc0,0xe0,0xe0,0xe0,0xe0,0xf0,0xf0,0xf0,0xf0,0xf0,0xc0,0xc0,0xc0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xf0,0xf0,0xe0,0xe0,0x00,0x00,0x00,0x00,0xf0,0xf0,0xf0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xfc,0xfe,0xfc,0xc0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xf8,0xf8,0xf0,0xf0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xfc,0xfe,0xfe,0xfc,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x40,0x20,0x30,0x10,0x10,0xd0,0x70,0x30,0x18,0x18,0x88,0x88,0x88,0x08,0x18,0x18,0x10,0xf0,0x70,0x90,0x10,0x10,0x30,0xe0,0x00};

void
InitLCD(void)
{
    EALLOW;

    GpioCtrlRegs.GPAPUD.bit.GPIO6 = 0;   // Enable pull-up on GPIO6

    GpioCtrlRegs.GPADIR.all |= 0x00C0; //GPIO 6 and 7 are outputs
    GpioCtrlRegs.GPBDIR.all |= 0x0003; //GPIO 32 and 33 are outputs

    GpioIntRegs.GPIOXINT1SEL.all = 0x0005; //Button on GPIO5 is interrupt
    XIntruptRegs.XINT1CR.all = 0x0009; // Set interrupt to FALLING edge (button is pushed) and enable
    //possibly simulate button debouncing with "6 sample qualification"?
    //GpioCtrlRegs.GPAQSEL1.all = 0x0800;    // GPIO5 - 6 samples


    EDIS;
}

void
LCD_init2(void){
    //initialize display
    LCD_data = 0x00A0; // ADC Select is normal
    LCDTimerStart();
    LCDWait();

    LCD_data = 0x00AE; // turn display OFF
    LCDTimerStart();
    LCDWait();

    LCD_data = 0x00C0; // COM output direction is normal
    LCDTimerStart();
    LCDWait();

    LCD_data = 0x00A2; //LCD drive voltage ration bias at 1/9th
    LCDTimerStart();
    LCDWait();

    LCD_data = 0x002F;  // set all internal voltage regulation on
    LCDTimerStart();
    LCDWait();

    LCD_data = 0x0021; // voltage regulator internal resistor ratio set to 3.5 (goes from 3 to 6.5)
    LCDTimerStart();
    LCDWait();

    LCD_data = 0x0081; //electronic volume mode set, next command will set screen brightness
    LCDTimerStart();
    LCDWait();

    LCD_data = 0x0020; // screen brightness set to reasonable value (0x003F to 0x0000)
    LCDTimerStart();
    LCDWait();

    LCD_data = 0x00AF; // turn display on
    LCDTimerStart();
    LCDWait();
}

void
LCD_display(uint16_t display_number){

    int i = 0;
    int k = 0;
    int LCD_data_count = 0;

    LCD_data = 0x0040; // start at line 0
    LCDTimerStart();
    LCDWait();
    LCD_data = 0x00B3; // start at page 4
    LCDTimerStart();
    LCDWait();
    LCD_data = 0x0010; // start at column 0 upper bits <------------ Use Column reset command (ADC Select?) or don't use this probably
    LCDTimerStart();
    LCDWait();
    LCD_data = 0x0000; // start at column 0 lower bits <------------ Use Column reset command (ADC Select?) or don't use this probably
    LCDTimerStart();
    LCDWait();

    //grab next image array from storage (get alex's help)
   switch(display_number){
   case 1:
       LCD_data_array = LCD_data_array1;//   <--------- use mem location here
       break;
   case 2:
       LCD_data_array = LCD_data_array2;
       break;
   case 3:
       LCD_data_array = LCD_data_array2;   //<------ TURN BACK TO 3
       break;
   }

    // double loop populates RAM with graphic data
   for (i = 0; i < 4; i++){  // 4 pages
       for (k = 0; k < 128; k++){ //128 columns
           LCD_data = LCD_data_array[LCD_data_count] | 0x0100; //append 1 to 8-bit data value as A0
           LCDTimerStart();
           LCDWait();
           LCD_data_count++;
       }
       if(i == 0){
           LCD_data = 0x00B2;
       }
       else if(i == 1){
           LCD_data = 0x00B1;
       }
       else if(i == 2){
           LCD_data = 0x00B0;
       }
       LCDTimerStart();
       LCDWait();
       LCD_data = 0x0010; // reset to column 0 upper bits
       LCDTimerStart();
       LCDWait();
       LCD_data = 0x0000; // reset to column 0 lower bits
       LCDTimerStart();
       LCDWait();
   }

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


__interrupt void gpio_xint1_isr(void) {
    button_pushed = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}
//
// End of file
//
