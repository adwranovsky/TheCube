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
    // using 9 as indices I couldn't resolve, since 9 didn't light up
    //R, G, B
    2, 3, 4,    // Row 0 Column 0
    9, 0, 1,    // Row 0 Column 1
    6, 7, 8,    // Row 0 Column 2
    13, 14, 5,  // Row 0 Column 3
    10, 11, 12, // Row 0 Column 4
    17, 18, 19, // Row 1 Column 0
    24, 15, 16, // Row 1 Column 1
    21, 22, 23, // Row 1 Column 2
    26, 25, 20, // Row 1 Column 3
    29, 28, 27, // Row 1 Column 4
    42, 43, 44, // Row 2 Column 0
    35, 40, 9,  // Row 2 Column 1
    36, 9, 9,   // Row 2 Column 2
    33, 34, 9,  // Row 2 Column 3
    30, 31, 32, // Row 2 Column 4
    57, 58, 59, // Row 3 Column 0
    54, 55, 56, // Row 3 Column 1
    51, 52, 53, // Row 3 Column 2
    48, 49, 50, // Row 3 Column 3
    45, 46, 47, // Row 3 Column 4
    67, 68, 69, // Row 4 Column 0
    74, 65, 66, // Row 4 Column 1
    71, 72, 73, // Row 4 Column 2
    63, 64, 70, // Row 4 Column 3
    60, 61, 62  // Row 4 Column 4
};
