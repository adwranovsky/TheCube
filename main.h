/*
 * Author: Alex Wranovsky
 */
#ifndef MAIN_H_
#define MAIN_H_

#include <stddef.h>
#include <stdint.h>

// Use to get the length of a static array only
#define LENGTH(x) ((sizeof x)/(sizeof x[0]))

// Function for doing all CPU and peripheral initialization
void sys_init(void);

// Sci functions
void sci_get_buf(char *buf, size_t length);
void sci_send_buf(const char *buf, size_t length); // This function isn't working
char sci_get_char(void);
void sci_send_char(char c);
void sci_send_string(const char *string);

// ADC functions
void adc_start_sampling(volatile int32_t *buffer, size_t length);
int adc_done_sampling(void);

// SPI functions
void InitSpiFifos(void);
void SPI_write_16(const uint16_t data);

// DSP utilities
#define FFT_SIZE 64
extern int32_t fft_comp_buffer[FFT_SIZE+2];
extern volatile int32_t sample_buffer[FFT_SIZE];
void rfft(int32_t *fft_comp_buffer);
void bit_reversal(volatile int32_t *sample_buffer, int32_t *fft_comp_buffer);
void print_time_domain(int32_t *sample_buffer);
void print_freq_domain(int32_t *fft_comp_buffer, int32_t sample_rate);
uint32_t detect_beat(const int32_t *frequencies);

// LED cube interface
#define SET_LED(row, column, layer, color, value)\
    framebuffer[led_index_table[(row)*3 + (column)*5 + (layer)*25]] = (value)
enum Color {R=0, G=1, B=2};
extern uint16_t framebuffer[5*5*5*3];
extern const uint16_t led_index_table[5*5*5*3];

// utility functions
const char *itoa(int32_t num, int is_signed, int base);

// ISRs
__interrupt void sci_rx_isr(void);
__interrupt void sci_tx_isr(void);
__interrupt void cpu_timer0_isr(void);
__interrupt void cpu_timer1_isr(void);
__interrupt void adc_int1_isr(void);
__interrupt void i2c_isr(void);

#endif
