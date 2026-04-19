#ifndef MOVES_H__
#define MOVES_H__

#include "config.h"
#include "piece.h"

typedef struct {
	Point from, to;
} Move;

typedef struct {
	Move data[MAX_MOVE_ARR_SIZE];
	int len;
} MoveArray;

MoveArray move_array_init();

#endif //MOVES_H__
