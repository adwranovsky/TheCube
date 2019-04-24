/*
 * My custom main funcion for running tests. Only runs if the ALEX_TEST build
 * configuration is selected in Code Composer.
 */

#include "main.h"

void framebuffer_test(void);

void main(void) {
    sys_init();
    start_cube();

    framebuffer_test();

    while (1) {
    }
}

void framebuffer_test(void) {
    int i;
    for (i = 1; i <= 75; i++) {
        framebuffer[i] = i;
    }
}
