#ifndef UI_H__
#define UI_H__

#include <stdbool.h>

#include "piece.h"

typedef enum {
	STATE_DEFAULT,
	STATE_PIECE_MOVING,
	STATE_PROMOTION_SELECTION,
	STATE_WHITE_WON,
	STATE_BLACK_WON,
	STATE_STALEMATE
} GameState;

bool restart_mouse_over(float x, float y, GameState state);

bool promote_mouse_over(float x, float y);
PieceType promote_mouse_selection(float x, float y);

#endif //UI_H__