/*
 * test_fastpwm.c
 *
 * Created: 2/16/2022 10:15:24 PM
 * Author : vklad
 */ 

#include "common.h"

#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "coords.h"
#include "snescontrol.h"
#include "usart_config.h"
#include "ws2812b.h"
#include "tetris.h"
#include "eememcfg.h"
#include "randomcfg.h"
#include "demo.h"

#define TCNT 60000
#define MAXSPEED 6
#define SCORE_LEVEL 150

uint16_t EEMEM lastScore;
uint32_t led_matrix[PIXELS] = {}; 
volatile int8_t brightness = 0x2; // Max brightness is 0
uint16_t currScore = 0;
volatile uint16_t seed = 0;
volatile uint16_t score = 0;
volatile uint8_t lines = 0;
volatile uint8_t count_speed = 0;
volatile uint8_t count = 0;
volatile uint8_t curr_pixel = 0;
volatile uint32_t curr_led_color = 0;
volatile uint16_t snes_btns = 0;
volatile uint8_t random_number = 0;
volatile uint8_t Event = 0x00;
volatile MoveDirection moveDirection = moveDown;
volatile uint8_t level = 0;
volatile uint8_t test = 0;
volatile int8_t start_test = 0;
volatile int8_t start_demo = 0;
volatile uint8_t frames = 0;

void fillColor(uint32_t);
void screen();
void refresh();
void showScore();
void showLevel();

uint8_t getRandomIndex();

Shape_t current_shape = {
    .X = 3,
    .Y = 0,
    .coords = {
            {.x = -1, .y =-1},
            {.x = -1, .y =-1},
            {.x = -1, .y =-1},
            {.x = -1, .y =-1}
        },
    .direction = UP,
    .show = &showShape,
    .hide = &hideShape,
    .move = &moveShape,
    .put = &putShapeDown,
    .init = &initShape
};

ISR(TIMER0_OVF_vect) {
    if (count > 23) {
        curr_pixel++;
        count = 0;
        curr_led_color = led_matrix[curr_pixel];
    }
    OCR0B = ((uint32_t)1 << (23 - count)) & curr_led_color ? BIT_HIGH : BIT_LOW;
    count++;
    if ((count >= 23) && (curr_pixel >= (PIXELS))) {
        TCCR0B = 0;
        _delay_us(RESET_DELAY);
        TCCR1B |= _BV(CS10) | _BV(CS11);
    }
}

ISR(TIMER1_OVF_vect) {
    
    TCCR1B = 0;
	
    /* Read controller */
    read_btns(&snes_btns);
    
    switch (snes_btns) {
        case BTN_B:
		    _delay_ms(100);
		    if (Event & _BV(DEMO)) {
			    start_demo--;
			    if (start_demo <= 0) {
				    Event &= ~_BV(DEMO);
				    Event &= ~_BV(PAUSE);
				    start_demo = 0;
				    clear();
			    }
			} else {
				start_demo++;
				if (start_demo >= 5) {
					Event |= _BV(DEMO);
					Event |= _BV(PAUSE);
					start_demo = 5;
					frames = 0;
					clear();
				}
		    }
		    break;
        case BTN_Y:
            Event ^= _BV(PAUSE);
            _delay_ms(200);
            break;
        case BTN_X:
		    break;
        case BTN_A:
		    _delay_ms(100);
			frames = 0;
			if (Event & _BV(TEST_SCREEN)) {
				start_test--;
				if (start_test <= 0) {
					Event &= ~_BV(TEST_SCREEN);
					Event &= ~_BV(PAUSE);
					start_test = 0;
					clear(); refresh();
				}
			} else {
				start_test++;
				if (start_test >= 5) {
					Event |= _BV(TEST_SCREEN);
					Event |= _BV(PAUSE);
					start_test = 5;
				}
			}
			break;
        case BTN_LEFT:
            seed+=1;
            moveDirection = moveLeft;
            break;
        case BTN_RIGHT:
            seed+=2;
            moveDirection = moveRight;
            break;
        case BTN_UP:
		    // Increase Level
		    if (Event & _BV(GAME_STOP)) {
			    _delay_ms(100);
			    level++;
			    if (level>6) level = 6;
			    printCh(10, 0, red); // print L
			    printCh(ONE(level), 1, red);
			    printCh(11, 2, off); // print space
			    screen();
			    refresh();
			    return;
		    }
            seed+=3;
            moveDirection = moveRotate;
            _delay_ms(30);
            break;
        case BTN_DOWN:
			// Decrease Level
			if (Event & _BV(GAME_STOP)) {
				_delay_ms(100);
				level--;
				if (level>6) level = 0; // cause unsigned
				printCh(10, 0, red); // print L
				printCh(ONE(level), 1, red);
				printCh(11, 2, off); // print space
				screen();
				refresh();
				return;
			}
            seed+=4;
            moveDirection = putDown;
            break;
        case BTN_R:
            brightness++;
            if (brightness > 6) brightness = 6;
            break;
        case BTN_L:
            brightness--;
            if (brightness < 0) brightness = 0;
            break;      
        case BTN_SELECT:
            break;
        case BTN_START:
            if ((Event & _BV(GAME_STOP)) | (Event & _BV(PAUSE))) { 
                Event |= _BV(START_GAME) | _BV(START_NEW_SHAPE);
                Event &= ~_BV(GAME_STOP);
                Event &= ~_BV(PAUSE);
				Event &= ~ _BV(GAME_OVER);
                moveDirection = moveDown;
                init_screen(off);
                score = 0; frames = 0;
            }
            break;
        default:
            break;
    }
    
    TCNT1 = TCNT;
    
    if (Event & _BV(START_GAME)) {
        Event |= _BV(START_NEW_SHAPE);
        Event &= ~_BV(START_GAME);
    }
	
	if (Event & _BV(TEST_SCREEN)) {
		 if (count_speed >= (MAXSPEED-level)) {
			 count_speed = 0;
			 if (test == 0) {
				 clear();
				 refresh();
				 test = 1;
			 } else {
				 test = 0;
				 fillColor(WEAKGREEN);
				 refresh();
			 }
		 } else {
			 count_speed++;
			 TCCR1B |= _BV(CS10) | _BV(CS11);
		 }
		 return;
	}
	
	if (Event & _BV(DEMO)) {
		showDemo2(&frames);
		refresh();
		return;
	}
	
	if (Event & _BV(GAME_OVER)) {
		if (frames>19) {
			Event &= ~_BV(GAME_OVER);
			Event |= _BV(GAME_STOP);
			if (score > currScore) {
				if (score > 999) score = 999;
				eeprom_update_word(&lastScore, score);
			}
			currScore = score;
			showScore();
		} else {
		    animationCleanUpField(&frames);
		}
		level = 0;
		screen();
		refresh();
		return;
	}
            
    if (!(Event & _BV(GAME_STOP)) && !(Event & _BV(PAUSE))) {
                
        if (Event & _BV(STOP_SHAPE)) {
            lines = checkGameField();
            seed += lines;
            switch (lines)
            {
                case 1:
                score += 4 * (level + 1);
                break;
                case 2:
                score += 8 * (level + 1);
                break;
                case 3:
                score += 16 * (level + 1);
                break;
                case 4:
                score += 32 * (level + 1);
                break;
                default:
                ;;
            }
            if ( score >= (SCORE_LEVEL*(1 + level)) )
            {
                level++;
            }
            Event &= ~_BV(STOP_SHAPE);
            Event |= _BV(START_NEW_SHAPE);
        }
                
        if (Event & _BV(START_NEW_SHAPE))
        {
            random_number = getRandomIndex();
            // start a new shape
            current_shape.X = 3;
            current_shape.Y = -2;
            current_shape.direction = UP;
            current_shape.descr = &shapesDescr[random_number];
            current_shape.init(&current_shape);
            current_shape.show(&current_shape);
            Event &= ~_BV(START_NEW_SHAPE);
                
        } else {
                    
            if (moveDirection == putDown) {
                current_shape.put(&current_shape, &Event);
                moveDirection = moveDown;
            } else if (moveDirection != moveDown) {
                current_shape.move(&current_shape, &Event, moveDirection);
                moveDirection = moveDown;
            }
                    
            if (count_speed >= (MAXSPEED-level)) {
                count_speed = 0;
                current_shape.move(&current_shape, &Event, moveDirection);
            }

        }
        
        screen();
        refresh();
        count_speed++;
        if (count_speed >= MAXSPEED) count_speed = MAXSPEED;
        
    } else {
        TCCR1B |= _BV(CS10) | _BV(CS11);
    }
    
}

void
timer_init() {
    cli();
    DDRD |= _BV(DDD5); // ws2812b pin data drive
    
    /* Timer0 Config */
    TCCR0A = 0x00;
    TCCR0B = 0x00;
    TIMSK0 |= _BV(TOIE0); // Timer/Counter0 Overflow Interrupt Enable
    TCCR0A |= (_BV(WGM00) | _BV(WGM01) | _BV(WGM02)); // Fast PWM Mode 7
    TCCR0A |= _BV(COM0B1); // Non-inverting mode
    OCR0A = PERIOD;
    TCNT0 = 0;
    /* Timer0 Config End */
    
    /* Timer1 Config */
    TCCR1A = 0x00;
    TCCR1B = 0x00;
    TIMSK1 |= _BV(TOIE1); // Timer/Counter1 Overflow Interrupt Enable
    //TCCR1B |= _BV(CS12); // Pre-scaler /256
    TCNT1 = TCNT; 
    /* Timer1 Config End */
     
    sei();
}

void 
refresh() {
    curr_pixel = 0;
    count = 0;
    curr_led_color = led_matrix[curr_pixel];
    OCR0B = ((uint32_t)1 << 23) & curr_led_color ? BIT_HIGH : BIT_LOW;
    TCCR0B |= _BV(CS00);
    count++;
}

void 
clear() {
    fillColor(OFF);
}

void
fillColor(uint32_t color) {
	uint8_t p = 0;
	do {
		led_matrix[p] = color;
	} while(++p < PIXELS);
}

/************************************************************************/
/* Read values from tetris matrix and fill led matrix with color codes  */
/************************************************************************/
void
screen() {
    uint8_t n = 0;
    uint8_t x = 0, y = 0;
    uint8_t r, g, b;
    
    for (n=0; n<PIXELS; n++) {
        x = GETX(n); y = GETY(n);
        r = ((color[matrix[x][y]] >> 8) & 0xFF) >> brightness;
        g = ((color[matrix[x][y]] >> 16) & 0xFF) >> brightness;
        b = (color[matrix[x][y]] & 0xFF) >> brightness;
        led_matrix[n] = ((uint32_t)g << 16)|((uint32_t)r << 8)|(uint32_t)b;
    }
}

void 
init_screen(uint8_t color) {
    uint8_t x = 0, y = 0;
    do {
        x = 0;
        do {
            matrix[x][y] = color;
        } while(++x < COLS);
    } while(++y < ROWS);
}

int
main(void) {
    Event |= _BV(GAME_STOP);
    USART_Init();
    init_screen(off);
    timer_init();
    clear();
    init_snes_control();
    
    currScore = eeprom_read_word(&lastScore);
    
    showScore();
	screen();
    refresh();
    
    while (1) 
    {
        ;;
    }
}

uint8_t
getRandomIndex() {
    srand(seed);
    int r = rand();
    if (r <= RANGE0)
        return 0;
    else if ((r > RANGE0) && (r <= RANGE1))
        return 1;
    else if ((r > RANGE1) && (r <= RANGE2))
        return 2;
    else if ((r > RANGE2) && (r <= RANGE3))
        return 3;
    else if ((r > RANGE3) && (r <= RANGE4))
        return 4;
    else if ((r > RANGE4) && (r <= RANGE5))
        return 5;
    else
        return 6;
}

void
showScore() {
    printCh(HUNDRED(currScore), 0, green);
    printCh(TEN(currScore), 1, blue);
    printCh(ONE(currScore), 2, orange);
}

void
showLevel() {
    printCh(ONE(level), 0, green);
}
