/*
 * Author: Alex Wranovsky
 */
#include <stddef.h>
#include <stdint.h>

// Use to get the length of a static array only
#define LENGTH(x) ((sizeof x)/(sizeof x[0]))

// Sci functions
void sci_get_buf(char *buf, size_t length);
void sci_send_buf(const char *buf, size_t length); // This function isn't working
char sci_get_char(void);
void sci_send_char(char c);
void sci_send_string(const char *string);

// ADC functions
void adc_start_sampling(volatile void *buffer, size_t length);
int adc_done_sampling(void);

// utility functions
const char *itoa(int32_t num, int base);

// ISRs
__interrupt void sci_rx_isr(void);
__interrupt void sci_tx_isr(void);
__interrupt void cpu_timer0_isr(void);
__interrupt void cpu_timer1_isr(void);
__interrupt void adc_int1_isr(void);
