#ifndef PIECE_H__
#define PIECE_H__

#include <stddef.h>
#include <stdbool.h>

typedef enum {
	PIECE_PAWN,
	PIECE_ROOK,
	PIECE_KNIGHT,
	PIECE_BISHOP,
	PIECE_QUEEN,
	PIECE_KING,
} PieceType;

typedef enum { PIECE_WHITE, PIECE_BLACK } PieceColour;

typedef struct {
	int x, y;
} Point;

Point point_invalid();

bool move_valid_pawn(Point from, Point to, PieceColour colour);
bool move_valid_pawn_capture(Point from, Point to, PieceColour colour);
bool move_valid_rook(Point from, Point to);
bool move_valid_knight(Point from, Point to);
bool move_valid_bishop(Point from, Point to);
bool move_valid_queen(Point from, Point to);
bool move_valid_king(Point from, Point to);

#endif //PIECE_H__