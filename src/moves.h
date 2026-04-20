#ifndef MOVES_H__
#define MOVES_H__

#include "config.h"

typedef struct {
	int x, y;
} Point;

Point point_invalid();
Point point_make(int x, int y);

typedef struct {
	Point from, to;
} Move;

Move move_make(Point from, Point to);

typedef struct {
	Move data[MAX_MOVE_ARR_SIZE];
	int len;
} MoveArray;

MoveArray move_array_init();
void move_array_push(MoveArray* arr, Move move);
MoveArray move_array_clear();

#endif //MOVES_H__
