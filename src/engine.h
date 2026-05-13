#ifndef ENGINE_H__
#define ENGINE_H__

#include "board.h"
#include "moves.h"
#include "piece.h"
#include "config.h"

typedef struct {
	float* ws[MAX_LAYERS];
    int* shapes[MAX_LAYERS];
	int n;
} Engine;

Engine* engine_create(const char* path);
void engine_destroy(Engine* engine);

void engine_search(const Board* b, MoveArray* arr);

int board_evaluate(const Board* b);
void board_to_features(const Board* b, float* features);
float engine_forward(const Engine* engine, float* features);

int piece_value(PieceType piece);

#endif //ENGINE_H__