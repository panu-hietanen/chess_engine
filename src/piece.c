#include <stdlib.h>

#include "config.h"
#include "piece.h"

bool move_valid_pawn(Move move, PieceColour colour)
{
	Point from = move.from;
	Point to = move.to;
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

bool move_valid_pawn_capture(Move move, PieceColour colour)
{
	switch (colour) {
	case PIECE_WHITE:
		return (abs(move.from.x - move.to.x) == 1 && move.to.y - move.from.y == 1);
	case PIECE_BLACK:
		return (abs(move.from.x - move.to.x) == 1 && move.from.y - move.to.y == 1);
	}
}

bool move_valid_rook(Move move)
{
	return (move.from.x == move.to.x || move.from.y == move.to.y);
}

bool move_valid_knight(Move move)
{
	return ((abs(move.from.x - move.to.x) == 2 && abs(move.from.y - move.to.y) == 1 ||
		abs(move.from.x - move.to.x) == 1 && abs(move.from.y - move.to.y) == 2));
}

bool move_valid_bishop(Move move)
{
	Point from = move.from;
	Point to = move.to;
	return (abs(move.from.x - move.to.x) == abs(move.from.y - move.to.y));
}

bool move_valid_queen(Move move)
{
	return move_valid_rook(move) || move_valid_bishop(move);
}

bool move_valid_king(Move move)
{
	return abs(move.from.x - move.to.x) <= 1 && abs(move.from.y - move.to.y) <= 1;
}