/*
 * cube_displays.c
 *
 *  Created on: Apr 27, 2019
 *      Author: mike nards
 */

#include "main.h"
#include "F2802x_Device.h"
#include "f2802x_examples.h"

volatile uint16_t rampV;
void check_all_layers(void);

//Alex function to strobe LEDs
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

void default_pattern(uint16_t beat){
    static int16_t values[5] = {254*0/4, 254*1/4, 254*2/4, 254*3/4, 254*4/4};
    static int16_t steps[5] = {1, 1, 1, 1, 1};
    strobe(0, 0, 0, R, &values[0], &steps[0]);
    strobe(0, 1, 1, R, &values[1], &steps[1]);
    strobe(0, 2, 2, R, &values[2], &steps[2]);
    strobe(0, 3, 3, R, &values[3], &steps[3]);
    strobe(0, 4, 4, R, &values[4], &steps[4]);
}
void mike_pattern_1(uint16_t beat){
   // SET_LED(row, column, layer, color, value)
   uint16_t r;
   uint16_t c;
   uint16_t l;
   uint16_t add_or_sub;
   while (!vsync);
   vsync = 0;
   for(r = 0; r < 5; r ++){
       for( c = 0; c < 5; c ++){
           for ( l = 0; l < 5; l++ ){
               // determine if bulbs dimming on brightening
               if(rampV == 250){
                   add_or_sub = 0;
               }
               if(rampV == 0){
                   add_or_sub = 1;
               }
               //determine voltage
               if( add_or_sub == 1){
                   rampV = rampV + 5;
               }
               else{
                   rampV = rampV - 5;
               }
               // determine color (semirandom)
              if(( r * c * l) >= 90){
                  SET_LED(r,c,l,G,rampV);
                  SET_LED(r,c,l,R,0);
                  SET_LED(r,c,l,B,0);
               }
              else if ((r * c * l) <= 50){
                  SET_LED(r,c,l,B,rampV);
                  SET_LED(r,c,l,R,0);
                  SET_LED(r,c,l,G,0);
               }
              else{
                  SET_LED(r,c,l,R,rampV);
                  SET_LED(r,c,l,G,0);
                  SET_LED(r,c,l,B,0);
               }
           }
       }
   }

}

//color change based on beat detection
void mike_pattern_2(uint16_t beat){
    uint16_t r;
    uint16_t c;
    uint16_t l;
    char color;
    char color2;
    char color3;
    color = R;
    color2 = G;
    color3 = B;
    while (!vsync);
    vsync = 0;
    for(r = 0; r < 5; r ++){
          for( c = 0; c < 5; c ++){
              for ( l = 0; l < 5; l++ ){
                  if(beat > 0){
                      if(color == R){
                          color = G;
                          color2 = B;
                          color3 = R;
                      }
                      else if(color == G){
                          color = B;
                          color2 = R;
                          color3 = G;
                      }
                      else{
                          color = R;
                          color2 = G;
                          color3 = B;
                      }
                  }
                  SET_LED(r,c,l,color,200);
                  SET_LED(r,c,l,color2,0);
                  SET_LED(r,c,l,color3,0);
              }
          }
    }
}
void rehaan_pattern_1(uint16_t beat){
    int i;
    for (i = 0; i < LENGTH(framebuffer); i++) {
        framebuffer[i] = 50;
    }
}
void alex_pattern_1(uint16_t beat){

}
void alex_pattern_2(uint16_t beat){

}

void check_all_layers(void) {
    int i;
    int num_cycles = 1;
    for (i = 0; i < LENGTH(framebuffer); i++) {
        int j;

        for (j = 0; j < num_cycles; j++) {
            while (!vsync);
            vsync = 0;
        }
        framebuffer[i] = 128;

        for (j = 0; j < num_cycles; j++) {
            while (!vsync);
            vsync = 0;
        }
        framebuffer[i] = 0;
    }
}



