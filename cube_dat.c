#include <stdint.h>

/*
 * This value gets set to 1 every time the full framebuffer is written out.
 * This is so that the application can sync up with the driver's refresh rate.
 */
volatile uint16_t vsync = 0;

/*
 * The framebuffer holds the brightness data for all of the LEDs. Only the
 * lowest 8 bits of each word are used. The values are arranged in a way to
 * make I2C transations faster, so use the provided macros to set a particular
 * LED from user code.
 */
uint16_t framebuffer[5*5*5*3];

/*
 * Since the cube isn't plugged into the drivers in any particular order, we
 * need to record where each row, column, and color actually connects, and map
 * that to a place in the framebuffer. The led index table does this job. Use
 * the SET_LED() macro, defined in main.h, as an interface to the framebuffer.
 */
const uint16_t led_index_table[5*5*5*3] = {
    //R, G, B
    0, 1, 2,    // Row 0 Column 0
    3, 4, 5,    // Row 0 Column 1
    6, 7, 8,    // Row 0 Column 2
    9, 10, 11,  // Row 0 Column 4
    12, 13, 14, // Row 0 Column 5
    15, 16, 17, // Row 1 Column 0
    18, 19, 20, // Row 1 Column 1
    21, 22, 23, // Row 1 Column 2
    24, 25, 26, // Row 1 Column 4
    27, 28, 29, // Row 1 Column 5
    30, 31, 32, // Row 2 Column 0
    33, 34, 35, // Row 2 Column 1
    36, 37, 38, // Row 2 Column 2
    39, 40, 41, // Row 2 Column 4
    42, 43, 44, // Row 2 Column 5
    45, 46, 47, // Row 3 Column 0
    48, 49, 50, // Row 3 Column 1
    51, 52, 53, // Row 3 Column 2
    54, 55, 56, // Row 3 Column 4
    57, 58, 59, // Row 3 Column 5
    60, 61, 62, // Row 4 Column 0
    63, 64, 65, // Row 4 Column 1
    66, 67, 68, // Row 4 Column 2
    69, 70, 71, // Row 4 Column 4
    72, 73, 74  // Row 4 Column 5
};
