/*
 * Author: Alex Wranovsky
 */

#include "F2802x_Device.h"
#include "f2802x_examples.h"

#include "main.h"

// Converts a 32 bit number into an ascii string
// NOT REENTRANT
const char *itoa(int32_t num, int is_signed, int base) {
    if (base > 16 || base < 2)
        return "Base must be between 2 and 16.";

    // Treat 0 specially
    if (num == 0)
        return "0";

    // Long enough to fit all 32 bit signed integers in any base from 2 to 16
    static char string[34];

    // The function only works on unsigned numbers, so negate and remember if necessary
    int is_negative = 0;
    uint32_t num_unsigned = num;
    if (is_signed && num < 0) {
        is_negative = 1;
        num_unsigned = -num;
    }

    // Initialize the buffer
    size_t index = LENGTH(string) - 1;
    string[index--] = '\0';

    // Convert num to a string
    while (num_unsigned) {
        string[index--] = "0123456789abcdef"[num_unsigned % base];
        num_unsigned /= base;
    }

    // Insert the negative sign if necessary
    if (is_negative)
        string[index--] = '-';

    return string + index + 1;
}

void enable_layer(int16_t layer_num) {
    // Turn off all layers first
    GpioDataRegs.GPACLEAR.all = 0x001F;

    // Turn on the selected layer
    switch (layer_num) {
        case 0:
            GpioDataRegs.GPASET.all = 0x0002;
            break;
        case 1:
            GpioDataRegs.GPASET.all = 0x0001;
            break;
        case 2:
            GpioDataRegs.GPASET.all = 0x0010;
            break;
        case 3:
            GpioDataRegs.GPASET.all = 0x0004;
            break;
        case 4:
            GpioDataRegs.GPASET.all = 0x0008;
            break;
    }
}
