#include <fft.h>
#include <fft_hamming_Q31.h>
#include "main.h"

/*
 * The FFT requires that the computation buffer be in bit-reverse order. To do
 * this properly, the buffer's address must be a multple of 2*FFT_SIZE. Use
 * RFFT32_brev() to do the actual bit reversing.
 */
#pragma DATA_SECTION(fft_comp_buffer, "FFTcomp")
int32_t fft_comp_buffer[FFT_SIZE+2];

/*
 * Put samples from the ADC in sample_buffer, and use fft_output_buffer for the
 * FFT output.
 */
#pragma DATA_SECTION(sample_buffer, "FFTsamples")
volatile int32_t sample_buffer[FFT_SIZE];

// Make sure to have FFT defaults match FFT_SIZE
RFFT32 rfft_control = RFFT32_128P_DEFAULTS;

/*
 * Wrapper around the DSP library bit reversal function
 */
void bit_reversal(volatile int32_t *sample_buffer, int32_t *fft_comp_buffer) {
    // Clean up computation buffer
    int32_t i;
    for (i=0; i<FFT_SIZE+2; i++) {
        fft_comp_buffer[i] = 0;
    }

    // Perform bit reversal necessary for real FFT. Discard volatile qualifier
    RFFT32_brev((int32_t *)sample_buffer, fft_comp_buffer, FFT_SIZE);
}

/*
 * Computes the complex frequencies from the ADC samples. ONLY use a properly
 * aligned buffer for time_domain_buffer.
 */
void rfft(int32_t *fft_comp_buffer) {
    // Set the buffer, and calculate the FFT
    rfft_control.ipcbptr = fft_comp_buffer;
    rfft_control.magptr = fft_comp_buffer;
    rfft_control.init(&rfft_control);

    rfft_control.calc(&rfft_control);
    rfft_control.split(&rfft_control);
    rfft_control.mag(&rfft_control);
}

void print_time_domain(int32_t *sample_buffer) {
    sci_send_string("START TIME DOMAIN\n\r");
    size_t index;
    for (index = 0; index < FFT_SIZE; index++) {
        sci_send_string(itoa(index/2, 0, 10));
        sci_send_string(", ");
        sci_send_string(itoa(sample_buffer[index], 0, 10));
        sci_send_string("\n\r");
    }
    sci_send_string("END TIME DOMAIN\n\r");
}

void print_freq_domain(int32_t *fft_comp_buffer, int32_t sample_rate) {
    sci_send_string("START FREQ DOMAIN\n\r");
    size_t index;
    for (index = 0; index < FFT_SIZE/2; index++) {
        sci_send_string(itoa((index*sample_rate)/FFT_SIZE, 0, 10));
        sci_send_string(", ");
        sci_send_string(itoa(fft_comp_buffer[index], 0, 10));
        sci_send_string("\n\r");
    }
    sci_send_string("END FREQ DOMAIN\n\r");
}

uint32_t gen_chromagraph_and_detect_beat(const int32_t *frequencies, union Chromagraph *chr) {
    return 0;
}
