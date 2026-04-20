#include "moves.h"

Point point_invalid()
{
	return (Point) { .x = -1, .y = -1 };
}

Point point_make(int x, int y)
{
	return (Point) { .x = x, .y = y };
}

Move move_make(Point from, Point to)
{
	return (Move) { .from = from, .to = to };
}

MoveArray move_array_init()
{
	return (MoveArray) { .len = 0 };
}

void move_array_push(MoveArray* arr, Move move)
{
	if (arr->len == MAX_MOVE_ARR_SIZE) return;
	arr->data[arr->len++] = move;
}

MoveArray move_array_clear()
{
	return (MoveArray) {};
}
