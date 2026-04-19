#include <stdlib.h>

#include "config.h"
#include "piece.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// BALLARRAY
///////////////////////////////////////////////////////////////////////////////////////////////////

Point point_invalid()
{
	return (Point) { .x = -1, .y = -1 };
}

bool move_valid_pawn(Point from, Point to, PieceColour colour)
{
	bool firstMove = false;
	switch (colour) {
	case PIECE_WHITE:
		firstMove = from.y == 1 && from.x == to.x && to.y == 2 + from.y;
		return (firstMove || from.x == to.x && to.y == 1 + from.y);
	case PIECE_BLACK:
		firstMove = from.y == 6 && from.x == to.x && to.y == from.y - 2;
		return (firstMove || from.x == to.x && to.y == from.y - 1);
	}
}

bool move_valid_pawn_capture(Point from, Point to, PieceColour colour)
{
	switch (colour) {
	case PIECE_WHITE:
		return (abs(from.x - to.x) == 1 && to.y - from.y == 1);
	case PIECE_BLACK:
		return (abs(from.x - to.x) == 1 && from.y - to.y == 1);
	}
}

bool move_valid_rook(Point from, Point to)
{
	return (from.x == to.x || from.y == to.y);
}

bool move_valid_knight(Point from, Point to)
{
	return ((abs(from.x - to.x) == 2 && abs(from.y - to.y) == 1 ||
		abs(from.x - to.x) == 1 && abs(from.y - to.y) == 2));
}

bool move_valid_bishop(Point from, Point to)
{
	return (abs(from.x - to.x) == abs(from.y - to.y));
}

bool move_valid_queen(Point from, Point to)
{
	return move_valid_rook(from, to) || move_valid_bishop(from, to);
}

bool move_valid_king(Point from, Point to)
{
	return abs(from.x - to.x) <= 1 && abs(from.y - to.y) <= 1;
}