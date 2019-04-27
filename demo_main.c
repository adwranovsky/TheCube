/*
 * demo_main.c
 *
 * Authors: Alex, Michael, and Rehaan
 *
 * This file is only compiled if the demo build config is selected. This
 * contains the main code for the final demonstration.
 */

#include "main.h"
#include "F2802x_Device.h"
#include "f2802x_examples.h"

typedef void (*PatternFunc)(void);

void init_framebuffer(void);
void default_pattern(void);

void main(void) {
    sys_init();
    LCD_init2();
    LCD_display(1);
    init_framebuffer();
    start_cube();

    PatternFunc current_pattern = default_pattern;
    
    while (1) {
        /*
         * Psuedo-code: Mike please fill out
         *
         *  if (button pressed) {
         *      current_pattern = new_pattern;
         *      change LCD display
         *  }
         */

        current_pattern();
    }
}

void init_framebuffer(void) {
    int i;

    for (i = 0; i < LENGTH(framebuffer); i++) {
        framebuffer[i] = 0;
    }
}

void default_pattern(void) {
    // do nothing
}
