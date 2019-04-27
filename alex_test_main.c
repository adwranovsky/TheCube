/*
 * My custom main funcion for running tests. Only runs if the ALEX_TEST build
 * configuration is selected in Code Composer.
 */

#include "main.h"
#include "F2802x_Device.h"
#include "f2802x_examples.h"

void framebuffer_test(void);
void color_test(void);
extern void alex_test(void);

void main(void) {
    sys_init();

    // wait for a second
    //ConfigCpuTimer(&CpuTimer0, 60, 500000);
    //CpuTimer0.InterruptCount = 0;
    //CpuTimer0Regs.TCR.all = 0x4001;
    //while (CpuTimer0.InterruptCount < 2);

    framebuffer_test();

    start_cube();
    //alex_test();

    //led_driver_test();

    while (!vsync);

    while (1) {
    }
}

void framebuffer_test(void) {
    int i;

    for (i = 0; i < 75; i++) {
        framebuffer[i] = i+1;
    }
}
