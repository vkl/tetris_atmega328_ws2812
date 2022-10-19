/*
 * demo.c
 *
 * Created: 5/6/2022 6:02:13 PM
 *  Author: vklad
 */ 


#include "demo.h"

void showDemo(uint8_t* pos) {
	uint8_t f = 1;
	for(uint8_t i=0; i<PIXELS; i++) {
		if ((*pos == i) && (f == 1)) {
		    led_matrix[i] = PURPLE; ++*pos; f = 0; if (*pos >= 200) (*pos) = 0;
		} else {
		    led_matrix[i] = OFF;
		}
	}
}

void showDemo2(uint8_t* pos) {
	for(uint8_t i=199; i>0; i--) {
		led_matrix[i] = led_matrix[i-1];
	}
	if (*pos == RAINBOW_COLORS) (*pos) = 0;
	led_matrix[0] = rainbow[*pos];
	++*pos;
}

