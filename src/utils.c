#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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

void print_board(const Board* b)
{
	static const char symbols[] = "PRNBQK";
	for (int y = BOARD_CELLS - 1; y >= 0; --y)
	{
		printf("%d ", y + 1);
		for (int x = 0; x < BOARD_CELLS; ++x)
		{
			int p = b->state[x][y];
			if (p < 0) { printf(". "); continue; }
			char sym = symbols[get_piece_type(p)];
			if (get_piece_colour(p) == PIECE_BLACK) sym += 32;
			printf("%c ", sym);
		}
		printf("\n");
	}
	printf("  a b c d e f g h\n\n");
}

bool parse_square(const char* s, Point* p)
{
	if (s[0] < 'a' || s[0] > 'h') return false;
	if (s[1] < '1' || s[1] > '8') return false;
	p->x = s[0] - 'a';
	p->y = s[1] - '1';
	return true;
}

bool parse_promote(const char *s, PieceType* piece)
{
	char p = s[0];
	switch (p)
	{
	case 'q':
	case 'Q':
		*piece = PIECE_QUEEN;
		return true;
	case 'r':
	case 'R':
		*piece = PIECE_ROOK;
		return true;
	case 'n':
	case 'N':
		*piece = PIECE_KNIGHT;
		return true;
	case 'b':
	case 'B':
		*piece = PIECE_BISHOP;
		return true;
	}
    return false;
}

bool load_weights(const char *path, float *weights[MAX_LAYERS], int *shape[MAX_LAYERS], int *count)
{
    FILE* fptr = fopen(path, "r");
    if (!fptr) return false;

    int used = 0;
    for (;;)
    {
        int ndim;
        int rc = fscanf(fptr, " %d,", &ndim);
        if (rc == EOF) break;
        if (rc != 1 || ndim <= 0 || ndim > 2 || used >= MAX_LAYERS) goto error;

        int elements = 1;
		int* curr_shape = malloc(sizeof(int) * 2);
        for (int i = 0; i < ndim; ++i)
        {
            int dim = 0;
            if (fscanf(fptr, "%d,", &dim) != 1 || dim <= 0) goto error;
            elements *= dim;
			if (shape != NULL)
			{
				curr_shape[i] = dim;
			}
		}

        float *layer = malloc((size_t)elements * sizeof(float));
        if (!layer) goto error;

        for (int i = 0; i < elements; ++i)
        {
            const char *fmt = (i + 1 < elements) ? "%f," : "%f";
            if (fscanf(fptr, fmt, &layer[i]) != 1) { free(layer); goto error; }
        }

		shape[used] = curr_shape;
        weights[used++] = layer;
    }

    fclose(fptr);
	if (count != NULL) *count = used;
	return true;

error:
    for (int i = 0; i < used; ++i) free(weights[i]);
    fclose(fptr);
    return false;
}