/*
 * tetris.c
 *
 * Created: 4/2/2022 10:49:44 PM
 *  Author: vklad
 */ 

#include "tetris.h"
#include "font.h"


uint16_t*
getShapeDirection(Shape_t *shape) {
    uint16_t *retval;
    switch (shape->direction) {
        case UP:
        retval = &shape->descr->up;
        break;
    case DOWN:
        retval = &shape->descr->down;
        break;
    case LEFT:
        retval = &shape->descr->left;
        break;
    case RIGHT:
        retval = &shape->descr->right;
        break;
    }
    return retval;
}

void initShape(Shape_t *shape) {
    for(uint8_t i=0; i<4; i++) {
        shape->coords[i].x = -1;
        shape->coords[i].y = -1;
    }
    setShapeCoords(shape);
}

uint8_t setShapeCoords(Shape_t *shape) {
    int8_t row = 0, col = 0, i = 0;
    uint8_t rows, cols;
    int8_t x = 0, y = 0;
    uint16_t *shape_type = getShapeDirection(shape);
    rows = (*shape_type >> 10) & 0b11;
    cols = (*shape_type >> 8) & 0b11;
    
    for (row=0; row<=rows; row++) {
        y = shape->Y + row;
        if (y < 0) continue;
        for (col=cols; col>=0; col--) {
            x = shape->X + (cols - col);
            if ((*shape_type >> (col+(row*(cols+1)))) & 1)
            {
                if ((x < 0) || (x>=COLS) || (y>=ROWS)) {
                    return DENY;
                }
                if (matrix[x][y] > 0) {
                    return DENY;
                }
                shape->coords[i].x = x;
                shape->coords[i].y = y;
                i++;
                if (i >= 4) break;
            }
        }
    }
    return ALLOW;
}

void
drawShape(Shape_t *shape, uint8_t color)
{
    for(uint8_t i=0; i<4; i++) {
        matrix[shape->coords[i].x][shape->coords[i].y] = color;
    }
}

void
showShape(Shape_t *shape)
{
    drawShape(shape, shape->descr->color);
}

void
hideShape(Shape_t *shape)
{
    drawShape(shape, off);
}


int8_t
moveShape(Shape_t *shape, uint8_t *Event, MoveDirection shapeMove)
{
    if ( (*Event & (1 << GAME_OVER)) || (*Event & (1 << PAUSE)) )
    return;
    
    Direction tmpDir = shape->direction;
    Coords tmp[4];
    for(uint8_t i=0; i<4; i++)
        tmp[i] = shape->coords[i];
        
    hideShape(shape);
    
    if (shapeMove == moveDown)
        shape->Y++;
    else if (shapeMove == moveLeft)
        shape->X--;
    else if (shapeMove == moveRight)
        shape->X++;
    else if (shapeMove == moveRotate)
        shape->direction == SHAPE_MAX_DIRECTION ? shape->direction = 0 : shape->direction++;
        
    uint8_t result = setShapeCoords(shape);
    
    if (result == ALLOW) {
        showShape(shape);
    } else {
        if (shapeMove == moveDown)
            shape->Y--;
        else if (shapeMove == moveLeft)
            shape->X++;
        else if (shapeMove == moveRight)
            shape->X--;
        else if (shapeMove == moveRotate)
            shape->direction = tmpDir;
        for(uint8_t i=0; i<4; i++)
            shape->coords[i] = tmp[i];
        showShape(shape);
        if (shapeMove == moveDown) {
            shape->Y < 0 ? (*Event |= (1 << GAME_OVER)) : (*Event |= (1 << STOP_SHAPE));
        }
    }
    return result;
}

void
putShapeDown(Shape_t *shape, uint8_t *Event)
{
    int8_t result = 0;
    if (shape->Y<=0) return;
    do {
        result = moveShape(shape, Event, moveDown);
    } while (result);
}

uint8_t
checkRow(uint8_t y) {
    for(uint8_t x=0; x<COLS; x++) {
        if (matrix[x][y] == 0)
            return 0;
    }
    return 1;
}

void
copyRow(uint8_t y_s, uint8_t y_d) {
    for(uint8_t x=0; x<COLS; x++) {
        matrix[x][y_d] = matrix[x][y_s];
    }
}

void 
cleanRow(uint8_t y) {
	for(uint8_t x=0; x<COLS; x++) {
		matrix[x][y] = 0;
	}
}

uint8_t
checkGameField(void)
{
    int8_t dy = 0, y = 0, x = 0;
    uint8_t fullRows = 0;
    
    y = ROWS-1;
    while (1)
    {
        while ( !checkRow(y) )
        {
            y--;
            if ( y<0 )
            return fullRows;
        }
        
        fullRows++;
        dy = y;
        while ( (dy-1)>=0 )
        {
            copyRow((dy-1), dy);
            dy--;
        }
    }
    
    return fullRows;
}

void
animationCleanUpField(uint8_t* frames) {
	for(int8_t y=(ROWS-1); y>=(*frames); y--) {
		if (y==(*frames)) {
			cleanRow(y);
			break;
		}
		copyRow((y-1), y);
	}
	++*frames;
}

void
printCh(uint8_t ch, uint8_t place, uint8_t color) {
    int8_t i, j;
    //ch -= 0x30;
    for(j=0; j<6; j++) {
        for(i=2; i>=0; i--) {
            if ((font[ch][j] >> i) & 0b1) {
                matrix[3+(2-i)][j+place*7] = color;
            } else {
                matrix[3+(2-i)][j+place*7] = off;
            }
        }
    }
}

