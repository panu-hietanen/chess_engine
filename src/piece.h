#ifndef PIECE_H__
#define PIECE_H__

#include <stddef.h>
#include <stdbool.h>

#include "moves.h"

typedef enum {
	PIECE_PAWN,
	PIECE_ROOK,
	PIECE_KNIGHT,
	PIECE_BISHOP,
	PIECE_QUEEN,
	PIECE_KING,
} PieceType;

typedef enum { PIECE_WHITE, PIECE_BLACK } PieceColour;

bool move_valid_pawn(Move move, PieceColour colour);
bool move_valid_pawn_capture(Move move, PieceColour colour);
bool move_valid_rook(Move move);
bool move_valid_knight(Move move);
bool move_valid_bishop(Move move);
bool move_valid_queen(Move move);
bool move_valid_king(Move move);

#endif //PIECE_H__