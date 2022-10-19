/*
 * tetris.h
 *
 * Created: 4/2/2022 10:49:32 PM
 *  Author: vklad
 */ 


#ifndef TETRIS_H_
#define TETRIS_H_

#include "common.h"

#define ALLOW 1
#define DENY 0

#define ROWS 20
#define COLS 10

#define off 0
#define cyan 1
#define blue 2
#define orange 3
#define yellow 4
#define green 5
#define purple 6
#define red 7
#define weakred 8

/* Shapes
  
  
  0-5 shape bits
  6,7 reserved
  8,9 columns, 0 - one column etc.
  10,11 rows, 0 - one row etc.
  12-15 - reserved
  
  0bx1000110xx111010  T - up
  0bx1001001xx101110  T - right
  0bx1000110xx010111  T - down
  0bx1001001xx011101  T - left
  
  0bxxxx0110xx011110  S - up
  0bxxxx1001xx101101  S - right
  0bxxxx0110xx110011  S - down
  0bxxxx1001xx011110  S - left
  
  0bxxxx0101xxxx1111  O - up/down/left/right
  
  0bxxxx1100xxxx1111  I - up/down
  0bxxxx0011xxxx1111  I - left/right
 
*/

#define TUP    0b0001011000111010
#define TRIGHT 0b0001100100101110
#define TDOWN  0b0001011000010111
#define TLEFT  0b0001100100011101

#define SUP    0b0000011000110011
#define SRIGHT 0b0000100100011110
#define SDOWN  0b0000011000110011
#define SLEFT  0b0000100100011110

#define ZUP    0b0000011000011110
#define ZRIGHT 0b0000100100101101
#define ZDOWN  0b0000011000011110
#define ZLEFT  0b0000100100101101

#define JUP    0b0000011000001111
#define JRIGHT 0b0000110100110101
#define JDOWN  0b0000011000111100
#define JLEFT  0b0000110100101011

#define LUP    0b0000011000100111
#define LRIGHT 0b0000110100010111
#define LDOWN  0b0000011000111001
#define LLEFT  0b0000110100111010

#define OUP    0b0000010100001111
#define ORIGHT 0b0000010100001111
#define ODOWN  0b0000010100001111
#define OLEFT  0b0000010100001111

#define IUP    0b0000110000001111
#define IRIGHT 0b0000001100001111
#define IDOWN  0b0000110000001111
#define ILEFT  0b0000001100001111


typedef struct {
    uint16_t up;
    uint16_t right;
    uint16_t down;
    uint16_t left;
    uint8_t color;  
} ShapeDescr_t;

static ShapeDescr_t shapesDescr[] = {
    {.up = TUP, .right = TRIGHT, .down = TDOWN, .left = TLEFT, .color = purple },
    {.up = SUP, .right = SRIGHT, .down = SDOWN, .left = SLEFT, .color = green },
    {.up = ZUP, .right = ZRIGHT, .down = ZDOWN, .left = ZLEFT, .color = red },
    {.up = JUP, .right = JRIGHT, .down = JDOWN, .left = JLEFT, .color = blue },
    {.up = OUP, .right = ORIGHT, .down = ODOWN, .left = OLEFT, .color = yellow },
    {.up = LUP, .right = LRIGHT, .down = LDOWN, .left = LLEFT, .color = orange },
    {.up = IUP, .right = IRIGHT, .down = IDOWN, .left = ILEFT, .color = cyan },
};

/* Event bits */
#define STOP_SHAPE 0
#define START_NEW_SHAPE 1
#define GAME_OVER 2
#define GAME_STOP 3
#define PAUSE 4
#define START_GAME 5
#define DEMO 6
#define TEST_SCREEN 7

#define SHAPE_MAX_DIRECTION 3     // The number of shape direction: 0 -> UP, 1 -> RIGHT, 2 -> DOWN, 3 -> LEFT

uint8_t matrix[COLS][ROWS];
    
typedef struct Shape_s Shape_t;

typedef enum {
    UP, RIGHT, DOWN, LEFT
} Direction;

typedef enum {
    moveDown, moveLeft, moveRight, moveRotate, putDown
} MoveDirection;

typedef struct {
    int8_t x;
    int8_t y;
} Coords;

struct Shape_s {
    int8_t X;
    int8_t Y;
    Coords coords[4];
    ShapeDescr_t *descr;
    const uint8_t color;
    Direction direction;
    void (* show) (Shape_t*);
    void (* hide) (Shape_t*);
    int8_t (* move) (Shape_t*, uint8_t*, MoveDirection);
    void (* put) (Shape_t*, uint8_t*);
    void (* init) (Shape_t*);
};

void showShape(Shape_t *shape);
void hideShape(Shape_t *shape);
int8_t moveShape(Shape_t *shape, uint8_t*, MoveDirection);
void rotateClockwise(Shape_t *shape, uint8_t*);
void putShapeDown(Shape_t *shape, uint8_t*);
void initShape(Shape_t *shape);
uint16_t* getShapeDirection(Shape_t *shape);
uint8_t checkGameField(void);
void animationCleanUpField(uint8_t*);
uint8_t setShapeCoords(Shape_t*);

void printCh(uint8_t ch, uint8_t place, uint8_t color);

#endif /* TETRIS_H_ */