/*
 * cube_displays.c
 *
 *  Created on: Apr 27, 2019
 *      Author: mike nards
 */

#include "main.h"
#include "F2802x_Device.h"
#include "f2802x_examples.h"

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

void default_pattern(void){
    int16_t values[5] = {254*0/4, 254*1/4, 254*2/4, 254*3/4, 254*4/4};
    int16_t steps[5] = {1, 1, 1, 1, 1};
    strobe(0, 0, 0, R, &values[0], &steps[0]);
    strobe(0, 1, 1, R, &values[1], &steps[1]);
    strobe(0, 2, 2, R, &values[2], &steps[2]);
    strobe(0, 3, 3, R, &values[3], &steps[3]);
    strobe(0, 4, 4, R, &values[4], &steps[4]);
}
void mike_pattern_1(void){
   // SET_LED(row, column, layer, color, value)
   uint16_t r;
   uint16_t c;
   uint16_t l;
   uint16_t v;
   uint16_t add_or_sub;
   char color;
   v = 100;
   while (!vsync);
   vsync = 0;
   for(r = 0; r < 5; r ++){
       for( c = 0; c < 5; c ++){
           for ( l = 0; l < 5; l++ ){
               // determine if bulbs dimming on brightening
               if(v == 250){
                   add_or_sub = 0;
               }
               if(v == 0){
                   add_or_sub = 1;
               }
               //determine voltage
               if( add_or_sub == 1){
                   v = v + 5;
               }
               else{
                   v = v - 5;
               }
               // determine color (semirandom)
              if( r * c * l >= 90){
                  color = R;
               }
              else if (r * c * l <= 50){
                   color = G;
               }
              else{
                   color = B;
               }

               SET_LED(r,c,l,color,v);
           }
       }
   }

}
void mike_pattern_2(void){

}
void rehaan_pattern_1(void){

}
void alex_pattern_1(void){

}
void alex_pattern_2(void){

}





