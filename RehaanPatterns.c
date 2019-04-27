/*
 * RehaanPatterns.c
 *
 *  File that contains the different patterns for the cube for the visual effects.
 *
 *  Created on: Apr 22, 2019
 *      Author: CMPE_STUDENT
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "F2802x_Device.h"
#include <fft.h>

static uint32_t last_time = 0;

enum CubeStateType {SET_STATE, WAIT_STATE};
static enum CubeStateType my_state;
static enum CubeStateType my_state1;
static uint16_t current_layer = 0;



void patterns(uint32_t beat, int32_t fft_comp_buffer[FFT_SIZE], int pattern) {

    //ask michael how to connect pattern selection to buttons and LCD
    //initial basic tests to see if cube works
    if(pattern == 1) {
        for(int l = 0; l < 5; l++) {
            for(int c = 0; c < 5; c++) {
                for(int r = 0; r < 5; r++) {
                    SET_LED(r, c, l, R, 150);
                }
            }
        }
    }

    if(pattern == 2) {
        for(int l = 0; l < 5; l++) {
            for(int c = 0; c < 5; c++) {
                for(int r = 0; r < 5; r++) {
                    if(r%2 == 0)
                        SET_LED(r, c, l, B, 150);
                    else
                        SET_LED(r, c, l, G, 150);
                }
            }
        }
    }

    // link LCD screen choices to the pattern choosing switch statement
    // char pick_pattern_from_screen =

    //real patterns


    switch (my_state) {
    case SET_STATE:
        for(int c = 0; c < 5; c++) {
            for(int r = 0; r < 5; r++) {
                SET_LED(r, c, current_layer, R, 150);
            }
        }
        last_time = CpuTimer1.InterruptCount;
        my_state = WAIT_STATE;
        break;
    case WAIT_STATE:
        if (CpuTimer1.InterruptCount - last_time > 2000) {
            for(int c = 0; c < 5; c++) {
                for(int r = 0; r < 5; r++) {
                    SET_LED(r, c, current_layer, R, 0);
                }
            }
            if (current_layer < 5) {
                current_layer++;
            } else {
                current_layer = 0;
            }
            my_state = SET_STATE;
        }
        break;
    }
}

/*
void rain_pattern(int iterations) {
    int i, j;
    int rand_x, rand_y, rand_int;

    for(i = 0; i < iterations; i++) {
        rand_int = rand() % 5;

        for(j = 0; j < rand_int; j++) {
            rand_x = rand() % 5;
            rand_y = rand() % 5;
            SET_LED(rand_x, rand_y, 4, B, 200);
        }
        //logic to decrease the layer by 1 till curr_layer = 0.
        current_layer = 4;

    }
}
*/




