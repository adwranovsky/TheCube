/*
 * My custom main funcion for running tests. Only runs if the ALEX_TEST build
 * configuration is selected in Code Composer.
 */

#include "main.h"
#include "F2802x_Device.h"
#include "f2802x_examples.h"

void init_framebuffer(uint16_t value);
void index_table_check(void);
void framebuffer_check(void);
void layer_assignment_check(void);
void check_all_leds(void);

void main(void) {
    sys_init();

    init_framebuffer(255);

    start_cube();

    // wait for a second
    ConfigCpuTimer(&CpuTimer0, 60, 500000);
    CpuTimer0.InterruptCount = 0;
    CpuTimer0Regs.TCR.all = 0x4001;
    while (CpuTimer0.InterruptCount < 2);

    init_framebuffer(0);

    // Turn all the corners on so that I know I2C is running
    SET_LED(0, 0, 0, G, 255);
    SET_LED(0, 0, 4, G, 255);
    SET_LED(0, 4, 0, G, 255);
    SET_LED(0, 4, 4, G, 255);
    SET_LED(4, 0, 0, G, 255);
    SET_LED(4, 0, 4, G, 255);
    SET_LED(4, 4, 0, G, 255);
    SET_LED(4, 4, 4, G, 255);

    while (1) {
        adc_start_sampling(sample_buffer, LENGTH(sample_buffer));
        while (!adc_done_sampling());
        bit_reversal(sample_buffer, fft_comp_buffer);
        rfft(fft_comp_buffer);

        uint16_t value = detect_beat(fft_comp_buffer) > 10 ? 255 : 10;

        SET_LED(0, 2, 4, B, value);
        SET_LED(0, 2, 4, R, value);
    }
}

void init_framebuffer(uint16_t value) { //turns on all leds with power up to 255
    int i;

    for (i = 0; i < LENGTH(framebuffer); i++) {
        framebuffer[i] = value;
    }
}

// Go through all LEDs one by one, switching on a button press. This is to
// verify that the index table is set up correctly.
void index_table_check(void) {
    int row, column;
    int i;
    const int layer = 4;

    for (row = 0; row < 5; row++) {
        for (column = 0; column < 5; column++) {
            const enum Color colors[3] = {R, G, B};
            for (i = 0; i < LENGTH(colors); i++) {
                int16_t value = 0, step = 1;

                while (!button_pushed) {
                    strobe(layer, row, column, colors[i], &value, &step);
                }
                button_pushed = 0;

                SET_LED(row, column, layer, colors[i], 0);
            }
        }
    }
}

// Go through all LEDs one by one, switching on a button press. This is to
// assist in making the LED index table.
void framebuffer_check(void) {
    int i;
    button_pushed = 0;

    for (i = 75*4; i < LENGTH(framebuffer); i++) {
        //sci_send_string("Framebuffer index: ");
        //sci_send_string(itoa(i, 0, 10));
        //sci_send_string("\r\n");

        framebuffer[i] = 128;
        while (!button_pushed);
        button_pushed = 0;
        framebuffer[i] = 0;
    }
}

// Lights up a couple LEDs in a single layer, moving to the next layer on a
// button press. This is to assist in creating the assignments in the
// enable_layer() function in util.c
void layer_assignment_check(void) {
    int layer;
    for (layer = 0; layer < 5; layer++) {
        int i;
        for (i = layer*75; i < (layer+1)*75; i++)
            framebuffer[i] = 64;

        while (!button_pushed);
        button_pushed = 0;

        for (i = layer*75; i < (layer+1)*75; i++)
            framebuffer[i] = 0;
    }
}

// Strobes through all LEDs
void check_all_leds(void) {
    int i;
    int num_cycles = 10;

    while (!vsync);
    vsync = 0;

    for (i = 0; i < LENGTH(framebuffer); i++) {
        int j;

        framebuffer[i] = 128;

        // Wait for a few refresh cycles
        for (j = 0; j < num_cycles; j++) {
            while (!vsync);
            vsync = 0;
        }

        framebuffer[i] = 0;
    }
}
