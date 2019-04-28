/*
 * demo_main.c
 *
 * Authors: Alex, Michael, and Rehaan
 *
 * This file is only compiled if the demo build config is selected. This
 * contains the main code for the final demonstration.
 */

#include "main.h"
#include "F2802x_Device.h"
#include "f2802x_examples.h"


typedef void (*PatternFunc)(uint16_t beat);

void init_framebuffer(uint16_t value); //turns on all LEDs initially to get rid of LED GHOST PENIS
//moved to Display Patterns section below

void main(void) {
    uint16_t beat = 0;
    sys_init();
    LCD_init2();
    LCD_display(curr_display);
    init_framebuffer(100);
    start_cube();
    
    // wait for a second
    ConfigCpuTimer(&CpuTimer0, 60, 500000);
    CpuTimer0.InterruptCount = 0;
    CpuTimer0Regs.TCR.all = 0x4001;
    while (CpuTimer0.InterruptCount < 2);

    init_framebuffer(0);



    PatternFunc all_patterns[] = {default_pattern, mike_pattern_1, mike_pattern_2, rehaan_pattern_1, alex_pattern_1, alex_pattern_2}; //add other patterns
    // Start the CPU timer, which triggers the ADC interrupt at regular intervals
    //CpuTimer1Regs.TCR.bit.TSS = 0;
    //adc_start_sampling(sample_buffer, LENGTH(sample_buffer));
    while (1) {

  //      SET_LED(1,1,1,G,0);

        //update LED pattern
        all_patterns[curr_display - 1](beat); // since curr_display starts at 1

        //change LCD screen and audio effect, ack button push
        if(button_pushed != 0){
            if(curr_display == 6){
                curr_display = 0;
            }
            curr_display++;
            stop_cube();
            LCD_display(curr_display);
            start_cube();
            button_pushed = 0;
        }

        //detect beat with FFT
        //while (!adc_done_sampling());
        //bit_reversal(sample_buffer, fft_comp_buffer);
        //adc_start_sampling(sample_buffer, LENGTH(sample_buffer));
        //rfft(fft_comp_buffer);
        //DAC_send(); //this gets called as soon as buffer is populated with raw sample data so that dac can start pumping it out
        //beat = detect_beat(fft_comp_buffer);
    }
}


void init_framebuffer(uint16_t value) {
    int i;

    for (i = 0; i < LENGTH(framebuffer); i++) {
        framebuffer[i] = value;
    }
}

