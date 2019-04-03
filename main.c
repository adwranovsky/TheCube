/*
 * Author: Alex Wranovsky
 */

#include <fft.h>
#include "main.h"
#include "spi.h"
void main(void)
{
    // Do all peripheral and CPU initialization. This function must be called
    // before other code is run.
    sys_init();

    //michael doing a SPI test
   // SPI_write_16(0x1234);

    // Begin application code
    while (1) {
        if (sci_get_char() == 's') {
            //adc_start_sampling(buffer1, LENGTH(buffer1));
            //while (!adc_done_sampling());

            // Generate a test square wave
            size_t i;
            int32_t val1 = 0x7fffffff;
            int32_t val2 = 0;
            for (i = 0; i < FFT_SIZE*2; i++) {
                if( (i>>1)%8 == 0 ) {
                    int32_t tmp = val1;
                    val1 = val2;
                    val2 = tmp;
                }
                sample_buffer[i++] = val1; // real component
                sample_buffer[i] = 0;      // complex component
            }

            // Do the bit reversal
            bit_reversal(sample_buffer, fft_comp_buffer);

            cfft(fft_comp_buffer);
            print_time_domain((int32_t *)sample_buffer); // discard volatile qualifier
            print_freq_domain(fft_comp_buffer, 40000);
        }
    }
}
