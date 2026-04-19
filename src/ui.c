#include "ui.h"
#include "config.h"

bool restart_mouse_over(float x, float y, GameState state)
{
	int restartx = RESTART_OFFSET_X;
	int restarty = RESTART_OFFSET_Y;
	if (state == STATE_WHITE_WON || state == STATE_BLACK_WON)
	{
		restartx = (SCREEN_W - RESTART_W) / 2;
		restarty = SCREEN_H / 2 + 100;
	}
	if (x > restartx && x < restartx + RESTART_W &&
		y > restarty && y < restarty + RESTART_H)
	{
		return true;
	}
	return false;
}

bool promote_mouse_over(float x, float y)
{
	if (x > PROMOTE_OFFSET_X && x < PROMOTE_OFFSET_X + PROMOTE_BOX_W &&
		y > PROMOTE_OFFSET_Y && y < PROMOTE_OFFSET_Y + PROMOTE_H)
	{
		return true;
	}
	return false;
}

PieceType promote_mouse_selection(float x, float y)
{
	float xdiff = x - PROMOTE_OFFSET_X;

	int idx = (int)xdiff / (int)(PROMOTE_W);

	switch (idx)
	{
	case 0:
		return PIECE_QUEEN;
	case 1:
		return PIECE_ROOK;
	case 2:
		return PIECE_BISHOP;
	case 3:
		return PIECE_KNIGHT;
	}
	return -1;
}
