/*
 * Author: Alex Wranovsky
 */
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
void adc_start_sampling(volatile uint32_t *buffer, size_t length);
int adc_done_sampling(void);

// DSP utilities
#define FFT_SIZE 1024
extern volatile int32_t buffer1[FFT_SIZE];
extern volatile int32_t buffer2[FFT_SIZE];
union Chromagraph {
    struct {
        uint32_t Aflat;
        uint32_t A;
        uint32_t Bflat;
        uint32_t B;
        uint32_t C;
        uint32_t Dflat;
        uint32_t D;
        uint32_t Eflat;
        uint32_t E;
        uint32_t F;
        uint32_t Gflat;
        uint32_t G;
    } note;
    uint32_t array[12];
};
void cfft(int32_t *time_domain_buffer, int32_t *freq_domain_buffer);
void print_time_domain(int32_t *time_domain_buffer);
void print_freq_domain(int32_t *freq_domain_buffer);

// utility functions
const char *itoa(int32_t num, int base);

// ISRs
__interrupt void sci_rx_isr(void);
__interrupt void sci_tx_isr(void);
__interrupt void cpu_timer0_isr(void);
__interrupt void cpu_timer1_isr(void);
__interrupt void adc_int1_isr(void);
