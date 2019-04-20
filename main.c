/*
 * Author: Alex Wranovsky
 */
#include <string.h>
#include <fft.h>
#include "F2802x_Device.h"
#include "f2802x_examples.h"
#include "main.h"
#include "spi.h"


void main(void)
 {
    // Do all peripheral and CPU initialization. This function must be called
    // before other code is run.
    sys_init();
    LCD_init2();
    LCD_display(1);

    // michael doing a SPI test
    //SPI_write_16(0x1234);

    // alex doing an I2C test
    start_cube();

    // Begin application code
    while (1) {
        // Pick the demo to do based on the input character
       // char c = sci_get_char();

        // Start the CPU timer, which triggers the ADC interrupt at regular intervals
        CpuTimer1Regs.TCR.bit.TSS = 0;

        char c = 'd';
        switch (c) {

        // FFT demo. Used by the f28027_fft_display.py script.
        case 's':
            // Tell the ADC to fill the buffer
            adc_start_sampling(sample_buffer, LENGTH(sample_buffer));
            while (!adc_done_sampling());

            // Do the bit reversal
            bit_reversal(sample_buffer, fft_comp_buffer);

            // Perform FFT
            rfft(fft_comp_buffer);

            // Print out waveform and FFT for debug
            print_time_domain((int32_t *)sample_buffer); // discard volatile qualifier
            print_freq_domain(fft_comp_buffer, 1000);

            break;

        // FFT performance test. Samples and computes as fast as possible, printing '$' on every loop.
        case 't':
            adc_start_sampling(sample_buffer, LENGTH(sample_buffer));
            while(1) {
                while (!adc_done_sampling());
                bit_reversal(sample_buffer, fft_comp_buffer);
                adc_start_sampling(sample_buffer, LENGTH(sample_buffer));
                rfft(fft_comp_buffer);
                sci_send_char('$');
            }

        // Beat detector test. Blinks the LED when a beat is detected.
        case 'b':
            // First disable CpuTimer0 so that we can use the LED
            CpuTimer0Regs.TCR.bit.TSS = 1;

            // Turn off the LED to start
            GpioDataRegs.GPASET.bit.GPIO0 = 1;

            // loop: sample audio -> fft -> detect beat
            adc_start_sampling(sample_buffer, LENGTH(sample_buffer));
            while (1) {
                while(!adc_done_sampling());

                bit_reversal(sample_buffer, fft_comp_buffer);
                adc_start_sampling(sample_buffer, LENGTH(sample_buffer));
                rfft(fft_comp_buffer);

                uint32_t beat = detect_beat(fft_comp_buffer);
                //sci_send_string(itoa(beat, 0, 10));
                //sci_send_string("\r\n");
                if (beat > 10) {
                    GpioDataRegs.GPACLEAR.bit.GPIO0 = 1;
                } else {
                    GpioDataRegs.GPASET.bit.GPIO0 = 1;
                }
            }
        //DAC test
        case 'd':
            // DAC_test_freq();
            adc_start_sampling(sample_buffer, LENGTH(sample_buffer));
            while(1) {
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
                while (!adc_done_sampling());
                bit_reversal(sample_buffer, fft_comp_buffer);
                adc_start_sampling(sample_buffer, LENGTH(sample_buffer));
                rfft(fft_comp_buffer);
                 DAC_send(); //this gets called as soon as buffer is populated with raw sample data so that dac can start pumping it out
                // sci_send_char('$');
                uint32_t beat = detect_beat(fft_comp_buffer);
                //sci_send_string(itoa(beat, 0, 10));
                //sci_send_string("\r\n");
                if (beat > 10) {
                    GpioDataRegs.GPACLEAR.bit.GPIO0 = 1;
                } else {
                    GpioDataRegs.GPASET.bit.GPIO0 = 1;
                }
            }

        //LCD test
        case '1':
            curr_display = 1;
            //insert everything under this somewhere in main loop when ready, also does DAC stuff
            while (1){
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
            }
        }
    }
}
