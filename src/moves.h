#ifndef MOVES_H__
#define MOVES_H__

#include "config.h"

typedef struct {
	int x, y;
} Point;

Point point_invalid();

typedef struct {
	Point from, to;
} Move;

Move move_make(Point from, Point to);

typedef struct {
	Move data[MAX_MOVE_ARR_SIZE];
	int len;
} MoveArray;

MoveArray move_array_init();

#endif //MOVES_H__
