/*
 * Author: Alex Wranovsky
 */
#ifndef MAIN_H_
#define MAIN_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

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
void SPI_write_16(const uint16_t data);
void DAC_write(const uint16_t data);
void DAC_send(void);
void DAC_test_freq(void);
extern volatile uint16_t readyForMore;
void DAC_send_2(uint16_t ADC_data);


// DSP utilities
#define FFT_SIZE 32
extern int32_t fft_comp_buffer[FFT_SIZE+2];
extern volatile int32_t sample_buffer[FFT_SIZE];
void rfft(int32_t *fft_comp_buffer);
void bit_reversal(volatile int32_t *sample_buffer, int32_t *fft_comp_buffer);
void print_time_domain(int32_t *sample_buffer);
void print_freq_domain(int32_t *fft_comp_buffer, int32_t sample_rate);
uint32_t detect_beat(const int32_t *frequencies);
int32_t strongest_freq(const int32_t frequencies[FFT_SIZE+2]);
uint32_t get_volume(const int32_t frequencies[FFT_SIZE+2]);

// LCD functions and variables
void LCDTimerStart(void);
void LCDTimerStop(void);
void LCDMainAction(void);
void LCDWait(void);
void LCD_init2(void);
void LCD_display(uint16_t display_number);
extern volatile uint16_t button_pushed;
extern volatile uint16_t curr_display;

// LED cube interface
// row 0 - 4
// column 0 - 4
// layer 0 - 4
// color R G or B
// value = 0 - 255
#define SET_LED(row, column, layer, color, value)\
    (framebuffer[led_index_table[(color) + (row)*3 + (column)*3*5] + (layer)*3*5*5] = (value))

// Like SET_LED(), but sets R, G, and B simultaneously
#define SET_PIXEL(row, column, layer, r, g, b) (SET_LED(row, column, layer, R, r),\
    SET_LED(row, column, layer, G, g), SET_LED(row, column, layer, B, b)) 

// Works like SET_LED(), but instead returns the value
#define GET_LED(row, column, layer, color)\
    (framebuffer[led_index_table[(color) + (row)*3 + (column)*3*5] + (layer)*3*5*5])

// Put the color values of the pixel into r, g, and b
#define GET_PIXEL(row, column, layer, r, g, b) ( (r = GET_LED(row, column, layer, R)),\
    (g = GET_LED(row, column, layer, G)), (b = GET_LED(row, column, layer, B)) )

enum Color {R=0, G=1, B=2};
extern uint16_t framebuffer[5*5*5*3];
extern const uint16_t led_index_table[5*5*5*3];
extern volatile uint16_t vsync;
extern const uint16_t device_addrs[3];
void i2c_write(uint16_t slave_addr, uint16_t reg_addr, uint16_t reg_val);
void start_cube(void);
void stop_cube(void);
void led_driver_test(void);
void enable_layer(int16_t layer_num);
void strobe(uint16_t layer, uint16_t row, uint16_t column, enum Color color,
    int16_t *value, int16_t *step);

// utility functions and macros
#define ABS(x) ( (x) < 0 ? -(x) : (x) )
const char *itoa(int32_t num, int is_signed, int base);
void color_picker(int32_t index, int16_t scale, uint16_t *r, uint16_t *g, uint16_t *b);

// display functions
void default_pattern(uint16_t beat);
void mike_pattern_1(uint16_t beat);
void mike_pattern_2(uint16_t beat);
void rehaan_pattern_1(uint16_t beat);
void alex_pattern_1(uint16_t beat);
void alex_pattern_2(uint16_t beat);


// ISRs
__interrupt void sci_rx_isr(void);
__interrupt void sci_tx_isr(void);
__interrupt void cpu_timer0_isr(void);
__interrupt void cpu_timer1_isr(void);
__interrupt void cpu_timer2_isr(void);
__interrupt void adc_int1_isr(void);
__interrupt void gpio_xint1_isr(void);
__interrupt void spi_isr(void);
__interrupt void i2c_isr1(void);
__interrupt void i2c_isr2(void);

#endif
