#ifndef UTILS_H__
#define UTILS_H__

#include "config.h"
#include "piece.h"
#include "ui.h"
#include "board.h"

void get_piece_path(char* name, PieceColour pColour, PieceType pType);
PieceColour get_piece_colour(int p);
PieceType get_piece_type(int p);
int get_piece_id(PieceType type, PieceColour colour);

void game_restart(Board* b, Point* selected, GameState* state);

#endif //UTILS_H__