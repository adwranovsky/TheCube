#include <fft.h>
#include <fft_hamming_Q31.h>
#include "main.h"

/*
 * The FFT requires that the computation buffer be in bit-reverse order. To do
 * this properly, the buffer's address must be a multple of 2*FFT_SIZE. Use
 * CFFT32_brev() to do the actual bit reversing.
 */
#pragma DATA_SECTION(fft_comp_buffer, "FFTcomp")
int32_t fft_comp_buffer[FFT_SIZE*2];

/*
 * Put samples from the ADC in sample_buffer, and use fft_output_buffer for the
 * FFT output.
 */
#pragma DATA_SECTION(sample_buffer, "FFTsamples")
volatile int32_t sample_buffer[FFT_SIZE*2];

// Make sure to have CFFT defaults match FFT_SIZE
CFFT32 cfft_control = CFFT32_256P_DEFAULTS;

/*
 * Wrapper around the DSP library bit reversal function
 */
void bit_reversal(volatile int32_t *sample_buffer, int32_t *fft_comp_buffer) {
    // Perform bit reversal necessary for complex FFT. Discard volatile qualifier
    CFFT32_brev((int32_t *)sample_buffer, fft_comp_buffer, FFT_SIZE/2);
    CFFT32_brev((int32_t *)sample_buffer+1, fft_comp_buffer+1, FFT_SIZE/2);
}

/*
 * Computes the complex frequencies from the ADC samples. ONLY use a properly
 * aligned buffer for time_domain_buffer.
 */
void cfft(int32_t *fft_comp_buffer) {
    // Set the buffers, and calculate the FFT
    cfft_control.ipcbptr = fft_comp_buffer;
    cfft_control.magptr = fft_comp_buffer;
    cfft_control.init(&cfft_control);
    cfft_control.calc(&cfft_control);
    cfft_control.mag(&cfft_control);
}

void print_time_domain(int32_t *sample_buffer) {
    sci_send_string("START TIME DOMAIN\n\r");
    size_t index;
    for (index = 0; index < FFT_SIZE*2; index+=2) {
        sci_send_string(itoa(index, 10));
        sci_send_string(", ");
        sci_send_string(itoa(sample_buffer[index], 10));
        sci_send_string("\n\r");
    }
    sci_send_string("END TIME DOMAIN\n\r");
}

void print_freq_domain(int32_t *fft_comp_buffer, int32_t sample_rate) {
    sci_send_string("START FREQ DOMAIN\n\r");
    size_t index;
    for (index = 0; index < FFT_SIZE/2; index++) {
        sci_send_string(itoa((index*sample_rate)/FFT_SIZE, 10));
        sci_send_string(", ");
        sci_send_string(itoa(fft_comp_buffer[index], 10));
        sci_send_string("\n\r");
    }
    sci_send_string("END FREQ DOMAIN\n\r");
}

uint32_t gen_chromagraph_and_detect_beat(const int32_t *frequencies, union Chromagraph *chr) {
    return 0;
}
