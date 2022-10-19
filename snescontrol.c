/*
 * snescontrol.c
 *
 * Created: 3/15/2022 8:53:20 PM
 *  Author: vklad
 */ 

#include "snescontrol.h"

void
init_snes_control() {
    DIR_SNES_REG |= _BV(PIN_CLK) | _BV(PIN_LATCH); // Clock, latch output
    DIR_SNES_REG &= ~_BV(PIN_DATA); // Data input
    CLK_HIGH;
    LATCH_LOW;
}

void
read_btns(uint16_t *regbtn) {
    
    uint8_t c = 0;
    
    LATCH_HIGH;
    _delay_us(12);
    LATCH_LOW;
    _delay_us(6);
    
    *regbtn = 0x0000;
    for(c=0; c<16; c++) {
        CLK_LOW;
        if (READ_DATA)
            *regbtn &= ~(1 << c);
        else
            *regbtn |= (1 << c);
        _delay_us(6);
        CLK_HIGH;
        _delay_us(6);
    }
    
}