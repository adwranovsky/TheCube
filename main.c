/*
 * Author: Alex Wranovsky
 */

#include <fft.h>
#include "main.h"

void main(void)
{
    // Do all peripheral and CPU initialization. This function must be called
    // before other code is run.
    sys_init();

    // Begin application code
    while (1) {
        if (sci_get_char() == 's') {
            //adc_start_sampling(buffer1, LENGTH(buffer1));
            //while (!adc_done_sampling());

            // Generate a test square wave
            size_t i;
            for (i = 0; i < FFT_SIZE*2; i++) {
                size_t j;
                // Generate 32 samples of a high signal
                for (j = i + 32; i < j; i++) {
                    sample_buffer[i++] = 0x7FFFFFFF;
                    sample_buffer[i] = 0x00000000; // complex part
                }
                // Generate 32 samples of a low signal
                for (j = i + 32; i < j; i++) {
                    sample_buffer[i++] = 0x00000000;
                    sample_buffer[i] = 0x00000000; // complex part
                }
            }

            // Do the bit reversal
            bit_reversal(sample_buffer, fft_comp_buffer);

            cfft(fft_comp_buffer);
            print_time_domain((int32_t *)sample_buffer); // discard volatile qualifier
            print_freq_domain(fft_comp_buffer, 40000);
        }
    }
}
