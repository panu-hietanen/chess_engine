#include "moves.h"

Point point_invalid()
{
	return (Point) { .x = -1, .y = -1 };
}

Move move_make(Point from, Point to)
{
	return (Move) { .from = from, .to = to };
}

MoveArray move_array_init()
{
	return (MoveArray) { .len = 0 };
}
