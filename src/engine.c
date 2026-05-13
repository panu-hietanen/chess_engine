#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "engine.h"

#include "utils.h"
#include "board.h"

Engine* engine_create(const char* path)
{
	Engine* engine = calloc(1, sizeof(Engine));
	
	char ws[256];
	sprintf(ws, "%s%s", path, ".csv");

    if (!load_weights(ws, engine->ws, engine->shapes, &engine->n)) { engine_destroy(engine); return NULL; }

	return engine;
}

void engine_destroy(Engine *engine)
{
    if (!engine) return;
    for (int i = 0; i < MAX_LAYERS; ++i)
    {
        free(engine->ws[i]);
		free(engine->shapes[i]);
    }
    free(engine);
}

void engine_search(const Board* b, MoveArray* arr)
{
	for (int i = 0; i < BOARD_CELLS; ++i)
	{
		for (int j = 0; j < BOARD_CELLS; ++j)
		{
			int piece = b->state[i][j];
			if (piece < 0) continue;
			if (get_piece_colour(piece) != b->turn) continue;

			Point from = point_make(i, j);
			switch (get_piece_type(piece))
			{
			case PIECE_PAWN:
				board_search_pawn(b, arr, from);
				break;
			case PIECE_ROOK:
				board_search_rook(b, arr, from);
				break;
			case PIECE_KNIGHT:
				board_search_knight(b, arr, from);
				break;
			case PIECE_BISHOP:
				board_search_bishop(b, arr, from);
				break;
			case PIECE_QUEEN:
				board_search_queen(b, arr, from);
				break;
			case PIECE_KING:
				board_search_king(b, arr, from);
				break;
			}
		}
	}
}

int board_evaluate(const Board* b)
{
	int sum = 0;
	for (int i = 0; i < BOARD_CELLS; ++i)
	{
		for (int j = 0; j < BOARD_CELLS; ++j)
		{
			int piece = b->state[i][j];
			if (piece < 0) continue;
			if (get_piece_colour(piece) == PIECE_WHITE)
			{
				sum += piece_value(get_piece_type(piece));
			}
			else
			{
				sum -= piece_value(get_piece_type(piece));
			}
		}
	}
	return sum;
}

void board_to_features(const Board *b, float* features)
{
	for (int i = 0; i < FEATURES; ++i) features[i] = 0.0f;
	for (int i = 0; i < BOARD_CELLS; ++i)
	{
		for (int j = 0; j < BOARD_CELLS; ++j)
		{
			int piece = b->state[i][j];
			if (piece < 0) continue;
			int feature_idx = piece * 64 + (i + j * 8);
			features[feature_idx] = 1.0f;
		}
	}
	int base = PIECE_FEATURES;
	features[base] = (b->turn == PIECE_WHITE) ? 1.0f : 0.0f;
	features[base + 1] = (b->canCastleWhite.kingSide) ? 1.0f : 0.0f;
	features[base + 2] = (b->canCastleWhite.queenSide) ? 1.0f : 0.0f;
	features[base + 3] = (b->canCastleBlack.kingSide) ? 1.0f : 0.0f;
	features[base + 4] = (b->canCastleBlack.queenSide) ? 1.0f : 0.0f;
	if (b->enPassantPawn.x >= 0 && b->enPassantPawn.y >= 0)
	{
		features[base + 5 + b->enPassantPawn.x] = 1.0f;
	}
}

float engine_forward(const Engine *engine, float *features)
{
    if (!engine || !features || engine->n < 2) return 0.0f;

    float *prev = features;
    int prev_dim = FEATURES;
    bool owns_prev = false;

    for (int t = 0; t + 1 < engine->n; t += 2)
    {
        float *W = engine->ws[t];
        float *B = engine->ws[t + 1];

        if (!W || !B || !engine->shapes[t] || !engine->shapes[t + 1])
        {
            if (owns_prev) free(prev);
            return 0.0f;
        }

        int in_dim  = engine->shapes[t][0];
        int out_dim = engine->shapes[t][1];

        if (in_dim != prev_dim || out_dim <= 0)
        {
            if (owns_prev) free(prev);
            return 0.0f;
        }

        float *curr = malloc((size_t)out_dim * sizeof(float));
        if (!curr)
        {
            if (owns_prev) free(prev);
            return 0.0f;
        }

        for (int o = 0; o < out_dim; ++o)
        {
            float sum = B[o];
            for (int i = 0; i < in_dim; ++i)
            {
                sum += prev[i] * W[i * out_dim + o];
            }

            bool is_last_pair = (t + 2 >= engine->n);
            curr[o] = (is_last_pair || sum > 0.0f) ? sum : 0.0f;
        }

        if (owns_prev) free(prev);
        prev = curr;
        prev_dim = out_dim;
        owns_prev = true;
    }

    float y = (prev_dim > 0) ? prev[0] : 0.0f;
    if (owns_prev) free(prev);

    return 1.0f / (1.0f + expf(-y));
}

int piece_value(PieceType piece)
{
	switch (piece)
	{
	case PIECE_PAWN:
		return PAWN_VALUE;
	case PIECE_ROOK:
		return ROOK_VALUE;
	case PIECE_BISHOP:
		return BISHOP_VALUE;
	case PIECE_KNIGHT:
		return KNIGHT_VALUE;
	case PIECE_QUEEN:
		return QUEEN_VALUE;
	case PIECE_KING:
		return 0;
	}
}