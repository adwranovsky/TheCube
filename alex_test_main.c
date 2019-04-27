/*
 * My custom main funcion for running tests. Only runs if the ALEX_TEST build
 * configuration is selected in Code Composer.
 */

#include "main.h"
#include "F2802x_Device.h"
#include "f2802x_examples.h"

void init_framebuffer(void);
void strobe(uint16_t layer, uint16_t row, uint16_t column,
    enum Color color, int16_t *value, int16_t *step);

void main(void) {
    sys_init();

    // wait for a second
    //ConfigCpuTimer(&CpuTimer0, 60, 500000);
    //CpuTimer0.InterruptCount = 0;
    //CpuTimer0Regs.TCR.all = 0x4001;
    //while (CpuTimer0.InterruptCount < 2);

    init_framebuffer();

    start_cube();

    //led_driver_test();

    int16_t values[5] = {254*0/4, 254*1/4, 254*2/4, 254*3/4, 254*4/4};
    int16_t steps[5] = {1, 1, 1, 1, 1};
    while (1) {
        strobe(0, 0, 0, R, &values[0], &steps[0]);
        strobe(0, 1, 1, R, &values[1], &steps[1]);
        strobe(0, 2, 2, R, &values[2], &steps[2]);
        strobe(0, 3, 3, R, &values[3], &steps[3]);
        strobe(0, 4, 4, R, &values[4], &steps[4]);
    }
}

void strobe(
    uint16_t layer,
    uint16_t row,
    uint16_t column,
    enum Color color,
    int16_t *value,
    int16_t *step
) {
    // Wait for the entire cube to write out
    while (!vsync);
    vsync = 0;

    SET_LED(row, column, layer, color, *value);

    *value += *step;

    if (*value == 255) {
        *step = -1;
    } else if (*value == 0) {
        *step = 1;
    }
}

void init_framebuffer(void) {
    int i;

    for (i = 0; i < LENGTH(framebuffer); i++) {
        framebuffer[i] = 0;
    }
}

