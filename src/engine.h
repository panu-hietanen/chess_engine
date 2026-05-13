#ifndef ENGINE_H__
#define ENGINE_H__

#include "board.h"
#include "moves.h"
#include "piece.h"
#include "config.h"

typedef struct {
	float* ws[MAX_LAYERS];
	float* ms[MAX_LAYERS];
	float* vs[MAX_LAYERS];
    int* shapes[MAX_LAYERS];
	int n;
} Engine;

Engine* engine_create(const char* path);
void engine_destroy(Engine* engine);

void engine_search(const Board* b, MoveArray* arr);

int board_evaluate(const Board* b);

int piece_value(PieceType piece);

#endif //ENGINE_H__