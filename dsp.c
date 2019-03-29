#include <fft.h>
#include <fft_hamming_Q31.h>
#include "main.h"

/*
 * The FFT requires that the input buffer must be in bit-reverse order. To do
 * this correctly, the input buffer address must be a multiple of 2*FFT_SIZE.
 * The main application swaps which buffer it uses as the input buffer, so that
 * it can overlay ADC sampling with chromagraph computation. This means both
 * buffers need to be aligned. Alignment is accomplished with the #pragma
 * DATA_ALIGN() directive.
 */
#pragma DATA_ALIGN(buffer1, 2*FFT_SIZE)
volatile int32_t buffer1[FFT_SIZE];
#pragma DATA_ALIGN(buffer2, 2*FFT_SIZE)
volatile int32_t buffer2[FFT_SIZE];

const int32_t fft_win[FFT_SIZE/2] = HAMMING1024;
CFFT32 cfft_control = CFFT32_1024P_DEFAULTS;

/*
 * Computes the complex frequencies from the ADC samples. ONLY use buffer1 and
 * buffer2 which are defined above as inputs. Other arrays will NOT be aligned
 * or sized correctly.
 */
void cfft(int32_t *time_domain_buffer, int32_t *freq_domain_buffer) {
    // Do initialization if this is the first call to this function
    static int need_init = 1;
    if (need_init) {
        need_init = 0;
        cfft_control.winptr = (int32_t *)fft_win; // I don't like the cast, but the example does it
        cfft_control.init(&cfft_control);
    }

    // Perform bit reversal necessary for complex FFT
    CFFT32_brev(time_domain_buffer, time_domain_buffer, FFT_SIZE/2);
    CFFT32_brev(time_domain_buffer+1, time_domain_buffer+1, FFT_SIZE/2);

    // Set the buffers, and calculate the FFT
    cfft_control.ipcbptr = freq_domain_buffer;
    cfft_control.magptr = time_domain_buffer;
    cfft_control.calc(&cfft_control);
    cfft_control.mag(&cfft_control);
}

void print_time_domain(int32_t *time_domain_buffer) {
    sci_send_string("START TIME DOMAIN\n\r");
    size_t index;
    for (index = 0; index < FFT_SIZE; index+=2) {
        sci_send_string(itoa(buffer1[index], 10));
        sci_send_string("\n\r");
    }
    sci_send_string("END TIME DOMAIN\n\r");
}

void print_freq_domain(int32_t *freq_domain_buffer) {
    sci_send_string("START FREQ DOMAIN\n\r");
    size_t index;
    for (index = 0; index < FFT_SIZE; index++) {
        sci_send_string(itoa(index, 10));
        sci_send_string(", ");
        sci_send_string(itoa(freq_domain_buffer[index], 10));
        sci_send_string("\n\r");
    }
    sci_send_string("END FREQ DOMAIN\n\r");
}

uint32_t gen_chromagraph_and_detect_beat(const int32_t *frequencies, union Chromagraph *chr) {
    return 0;
}
