/*
 * demo.h
 *
 * Created: 5/6/2022 6:01:59 PM
 *  Author: vklad
 */ 


#ifndef DEMO_H_
#define DEMO_H_

#include "common.h"
#include "ws2812b.h"

extern uint32_t led_matrix[PIXELS];


void showDemo(uint8_t*);
void showDemo2(uint8_t*);



#endif /* DEMO_H_ */