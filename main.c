/*
 * Author: Alex Wranovsky
 */

#include <fft.h>
#include <string.h>
#include "main.h"

void main(void)
{
    // Do all peripheral and CPU initialization. This function must be called
    // before other code is run.
    sys_init();

    // Begin application code
    while (1) {
        char c = sci_get_char();
        switch (c) {
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
        case 't':
            // continually compute, and send a character after every time the fft is computed
            adc_start_sampling(sample_buffer, LENGTH(sample_buffer));
            while(1) {
                while (!adc_done_sampling());
                bit_reversal(sample_buffer, fft_comp_buffer);
                adc_start_sampling(sample_buffer, LENGTH(sample_buffer));
                rfft(fft_comp_buffer);
                sci_send_char('$');
            }
        }
    }
}
