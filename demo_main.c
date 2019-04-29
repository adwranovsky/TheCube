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


typedef void (*PatternFunc)(uint16_t beat);


//moved to Display Patterns section below

void main(void) {
    uint16_t beat = 0;
    sys_init();
    LCD_init2();
    LCD_display(curr_display);
    start_cube();
    PatternFunc all_patterns[] = {default_pattern, mike_pattern_1, mike_pattern_2, rehaan_pattern_1, alex_pattern_1, alex_pattern_2}; //add other patterns
    // Start the CPU timer, which triggers the ADC interrupt at regular intervals
    CpuTimer1Regs.TCR.bit.TSS = 0;
    adc_start_sampling(sample_buffer, LENGTH(sample_buffer));
    while (1) {

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
        if (adc_done_sampling()){
            bit_reversal(sample_buffer, fft_comp_buffer);
            adc_start_sampling(sample_buffer, LENGTH(sample_buffer));
            rfft(fft_comp_buffer);
            CpuTimer1Regs.TCR.bit.TSS = 0;
            beat = detect_beat(fft_comp_buffer);//update LED pattern
            all_patterns[curr_display - 1](beat); // since curr_display starts at 1
        }
    }
}

