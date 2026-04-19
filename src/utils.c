#include <string.h>
#include <stdio.h>

#include "utils.h"

void get_piece_path(char* name, PieceColour pColour, PieceType pType)
{
	char colour[6];
	if (pColour == PIECE_WHITE)
	{
		strcpy(colour, "white");
	}
	else
	{
		strcpy(colour, "black");
	}

	char piece[11];
	switch (pType)
	{
	case PIECE_PAWN:
		strcpy(piece, "pawn.png");
		break;
	case PIECE_ROOK:
		strcpy(piece, "rook.png");
		break;
	case PIECE_BISHOP:
		strcpy(piece, "bishop.png");
		break;
	case PIECE_KNIGHT:
		strcpy(piece, "knight.png");
		break;
	case PIECE_QUEEN:
		strcpy(piece, "queen.png");
		break;
	case PIECE_KING:
		strcpy(piece, "king.png");
		break;
	}
	sprintf(name, "%s%s-%s", RESOURCES_PATH, colour, piece);
}

PieceColour get_piece_colour(int p)
{
	return (PieceColour)(p < NUM_PIECE_TYPES ? PIECE_WHITE : PIECE_BLACK);
}

PieceType get_piece_type(int p)
{
	return (PieceType)(p % NUM_PIECE_TYPES);
}

int get_piece_id(PieceType type, PieceColour colour)
{
	return type + colour * NUM_PIECE_TYPES;
}

void game_restart(Board* b, Point* selected, GameState* state)
{
	*b = board_init_game();
	*selected = point_invalid();
	*state = STATE_DEFAULT;
}
