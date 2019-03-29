/*
 * Author: Alex Wranovsky
 */

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
            for (i = 0; i < FFT_SIZE; i++) {
                size_t j;
                // Generate 64 samples of a high signal
                for (j = i + 64; i < j; i++) {
                    buffer1[i] = 0x7FFFFFFF;
                }
                // Generate 64 samples of a low signal
                for (j = i + 64; i < j; i++) {
                    buffer1[i] = 0x00000000;
                }
            }

            // It's ok to throw away the volatile qualifier here, since the ADC isn't sampling
            cfft((int32_t *)buffer1, (int32_t *)buffer2);
            print_time_domain((int32_t *)buffer1);
            print_freq_domain((int32_t *)buffer2);
        }
    }
}
