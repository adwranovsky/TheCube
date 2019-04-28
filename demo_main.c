/*
 * demo_main.c
 *
 * Authors: Alex, Michael, and Rehaan
 *
 * This file is only compiled if the "Demo" build configuration is selected.
 * This contains the main code for the final demonstration.
 */

#include "main.h"
#include "F2802x_Device.h"
#include "f2802x_examples.h"


typedef void (*PatternFunc)(void);

void init_framebuffer(void);
//moved to Display Patterns section below

void main(void) {
    sys_init();
    LCD_init2();
    LCD_display(1);
    init_framebuffer();
    start_cube();
    
    PatternFunc all_patterns[] = {default_pattern, mike_pattern_1, mike_pattern_2, rehaan_pattern_1, alex_pattern_1, alex_pattern_2}; //add other patterns
    //PatternFunc current_pattern = default_pattern;
    // Start the CPU timer, which triggers the ADC interrupt at regular intervals
//    CpuTimer1Regs.TCR.bit.TSS = 0;
 //   adc_start_sampling(sample_buffer, LENGTH(sample_buffer));
    while (1) {
        all_patterns[curr_display - 1](); // since curr_display starts at 1
        if(button_pushed != 0){
            if(curr_display == 6){
                curr_display = 0;
            }
            curr_display++;
            if(curr_display < 4){
                LCD_display(curr_display);
            }
            button_pushed = 0;
        }
    //    while (!adc_done_sampling());
   //     bit_reversal(sample_buffer, fft_comp_buffer);
    //    adc_start_sampling(sample_buffer, LENGTH(sample_buffer));
    //    rfft(fft_comp_buffer);
   //     DAC_send(); //this gets called as soon as buffer is populated with raw sample data so that dac can start pumping it out
    //    uint32_t beat = detect_beat(fft_comp_buffer);
    }
}


void init_framebuffer(void) {
    int i;

    for (i = 0; i < LENGTH(framebuffer); i++) {
        framebuffer[i] = 0;
    }
}

