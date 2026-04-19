#ifndef BOARD_H__
#define BOARD_H__

#include <stdbool.h>

#include "config.h"
#include "piece.h"
#include "moves.h"

typedef enum { MOVE_OK, MOVE_PROMOTE, MOVE_WHITE_IN_CHECK, MOVE_BLACK_IN_CHECK } MoveResult;

typedef struct {
	bool kingSide;
	bool queenSide;
} CastleValid;

typedef struct {
	int state[BOARD_CELLS][BOARD_CELLS];
	PieceColour turn;
	Point enPassantPawn;
	CastleValid canCastleWhite;
	CastleValid canCastleBlack;
} Board;

Board board_init();
Board board_init_game();

MoveResult board_register_move(Board* b, Move move);
void board_next_turn(Board* b);
void board_pawn_promote(Board* b, Point from, PieceType type);
Point board_find_king(const Board* b, PieceColour colour);

bool board_move_valid(const Board* b, Move move);
bool board_is_en_passant(const Board* b, Move move);

bool board_can_castle(const Board* b, PieceType side);
void board_invalidate_castle(Board* b, PieceType side);
bool board_castle_move_ok(const Board* b, Move move);

bool board_in_check(const Board* b, Point king);
bool board_no_moves(const Board* b, PieceColour colourToCheck);

bool board_blocked_pawn(const Board* b, Move move);
bool board_blocked_rook(const Board* b, Move move);
bool board_blocked_bishop(const Board* b, Move move);

bool board_can_capture(const Board* b, Move move);

bool board_mouse_over(float x, float y);
Point board_mouse_coords(float x, float y);
bool board_select_valid(const Board* b, Point clicked);

#endif //BOARD_H__