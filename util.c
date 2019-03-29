/*
 * Author: Alex Wranovsky
 */

#include "main.h"

// Converts a 32 bit signed number into an ascii string
// NOT REENTRANT
const char *itoa(int32_t num, int base) {
    if (base > 16 || base < 2)
        return "Base must be between 2 and 16.";

    // Treat 0 specially
    if (num == 0)
        return "0";

    // Long enough to fit all 32 bit signed integers in any base from 2 to 16
    static char string[34];

    // The function only works on unsigned numbers, so negate and remember if necessary
    int is_negative = 0;
    if (num < 0) {
        is_negative = 1;
        num = -num;
    }

    // Initialize the buffer
    size_t index = LENGTH(string) - 1;
    string[index--] = '\0';

    // Convert num to a string
    while (num) {
        string[index--] = "0123456789abcdef"[num % base];
        num /= base;
    }

    // Insert the negative sign if necessary
    if (is_negative)
        string[index--] = '-';

    return string + index + 1;
}
