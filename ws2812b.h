/*
 * ws2812b.h
 *
 * Created: 3/19/2022 2:51:31 PM
 *  Author: vklad
 */ 


#ifndef WS2812B_H_
#define WS2812B_H_

/* WS2812B color  */
/* Green Red Blue */
#define OFF     0x000000
#define CYAN    0xFF00FF
#define BLUE    0x0000FF
#define ORANGE  0x80FF00
#define YELLOW  0x64FF00
#define GREEN   0xFF0000
#define PURPLE  0x20B0F0
#define RED     0x00FF00
#define WEAKGREEN 0x770000

#define PERIOD 20 /* Timer 0 OCR0A */
#define BIT_HIGH 14
#define BIT_LOW 6

#define RESET_DELAY 200 /* delay us */

#define PIXELS 200
#define RAINBOW_COLORS 12

const static uint32_t color[8] = {
    OFF, CYAN, BLUE, ORANGE, YELLOW, GREEN, PURPLE, RED
};

const static uint32_t rainbow[] = {
	0xFF7F00, 0xFF0000, 0xFF007F, 0xFF00FF,
	0x7F00FF, 0x0000FF, 0x007FFF, 0x00FFFF,
	0x00FF7F, 0x00FF00, 0x7FFF00, 0xFFFF00
};



#endif /* WS2812B_H_ */