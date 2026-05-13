#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "engine.h"

#include "utils.h"
#include "board.h"

Engine* engine_create(const char* path)
{
	Engine* engine = malloc(sizeof(Engine));
	
	char ws[256];
	char ms[256];
	char vs[256];
	sprintf(ws, "%s%s", path, ".csv");
	sprintf(ms, "%s%s", path, "_m.csv");
	sprintf(vs, "%s%s", path, "_v.csv");

    if (!load_weights(ws, engine->ws, engine->shapes, &engine->n)) { engine_destroy(engine); return NULL; }
    if (!load_weights(ms, engine->ms, NULL, NULL)) { engine_destroy(engine); return NULL; }
    if (!load_weights(vs, engine->vs, NULL, NULL)) { engine_destroy(engine); return NULL; }

	return engine;
}

void engine_destroy(Engine *engine)
{
    if (!engine) return;
    for (int i = 0; i < MAX_LAYERS; ++i)
    {
        free(engine->ws[i]);
        free(engine->ms[i]);
        free(engine->vs[i]);
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
	for (int i = 0; i < BOARD_CELLS; ++i)
	{
		for (int j = 0; j < BOARD_CELLS; ++j)
		{
			int piece = b->state[i][j];
			if (piece < 0) continue;
			int feature_idx = piece * 64 + (i + j * 8);
		}
	}
	int base = PIECE_FEATURES;
	features[base] = (b->turn == PIECE_WHITE) ? 1.0f : 0.0f;
	features[base + 1] = (b->canCastleWhite.kingSide) ? 1.0f : 0.0f;
	features[base + 2] = (b->canCastleWhite.queenSide) ? 1.0f : 0.0f;
	features[base + 3] = (b->canCastleBlack.kingSide) ? 1.0f : 0.0f;
	features[base + 4] = (b->canCastleBlack.queenSide) ? 1.0f : 0.0f;
	if (b->enPassantPawn.x > 0 && b->enPassantPawn.y > 0)
	{
		features[base + b->enPassantPawn.x] = 1.0f;
	}
}

float engine_forward(const Engine *engine, float *features)
{
	return 0.0f;
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