/*
 * snescontrol.h
 *
 * Created: 3/15/2022 8:52:36 PM
 *  Author: vklad
 */ 


#ifndef SNESCONTROL_H_
#define SNESCONTROL_H_

#include "common.h"

#include <util/delay.h>


#define INPUT_SNES_REG  PIND
#define OUTPUT_SNES_REG PORTD
#define DIR_SNES_REG DDRD
#define PIN_LATCH  PORTD2
#define PIN_CLK    PORTD3
#define PIN_DATA   PIND4  

#define LATCH_HIGH (OUTPUT_SNES_REG |= _BV(PIN_LATCH))
#define LATCH_LOW  (OUTPUT_SNES_REG &= ~_BV(PIN_LATCH))
#define CLK_HIGH   (OUTPUT_SNES_REG |= _BV(PIN_CLK))
#define CLK_LOW   (OUTPUT_SNES_REG &= ~_BV(PIN_CLK))
#define READ_DATA (INPUT_SNES_REG & _BV(PIN_DATA))

#define BTN_B         0b0000000000000001
#define BTN_Y         0b0000000000000010
#define BTN_SELECT    0b0000000000000100
#define BTN_START     0b0000000000001000
#define BTN_UP        0b0000000000010000
#define BTN_DOWN      0b0000000000100000
#define BTN_LEFT      0b0000000001000000
#define BTN_RIGHT     0b0000000010000000
#define BTN_A         0b0000000100000000
#define BTN_X         0b0000001000000000
#define BTN_L         0b0000010000000000
#define BTN_R         0b0000100000000000

void init_snes_control();
void read_btns(uint16_t*);


#endif /* SNESCONTROL_H_ */