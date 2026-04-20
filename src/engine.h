#ifndef ENGINE_H__
#define ENGINE_H__

#include "board.h"
#include "moves.h"
#include "piece.h"

void engine_search(const Board* b, MoveArray* arr);

int piece_value(PieceType piece);

#endif //ENGINE_H__