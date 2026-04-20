#include <stdlib.h>
#include <math.h>

#include "board.h"
#include "utils.h"

Board board_init()
{
	CastleValid castle = { .kingSide = true,.queenSide = true };
	Board b = (Board){ 
		.turn = PIECE_WHITE,
		.enPassantPawn = point_invalid(),
		.canCastleWhite = castle,
		.canCastleBlack = castle
	};
	for (int i = 0; i < BOARD_CELLS; ++i)
	{
		for (int j = 0; j < BOARD_CELLS; ++j)
		{
			b.state[i][j] = -1;
		}
	}
	return b;
}

Board board_init_game()
{
	Board b = board_init();

	for (int i = 0; i < BOARD_CELLS; ++i)
	{
		b.state[i][1] = PIECE_PAWN;
	}
	for (int i = 0; i < BOARD_CELLS; ++i)
	{
		b.state[i][6] = PIECE_PAWN + NUM_PIECE_TYPES;
	}
	b.state[0][0] = PIECE_ROOK;
	b.state[1][0] = PIECE_KNIGHT;
	b.state[2][0] = PIECE_BISHOP;
	b.state[3][0] = PIECE_QUEEN;
	b.state[4][0] = PIECE_KING;
	b.state[5][0] = PIECE_BISHOP;
	b.state[6][0] = PIECE_KNIGHT;
	b.state[7][0] = PIECE_ROOK;

	b.state[0][7] = PIECE_ROOK   + NUM_PIECE_TYPES;
	b.state[1][7] = PIECE_KNIGHT + NUM_PIECE_TYPES;
	b.state[2][7] = PIECE_BISHOP + NUM_PIECE_TYPES;
	b.state[3][7] = PIECE_QUEEN  + NUM_PIECE_TYPES;
	b.state[4][7] = PIECE_KING   + NUM_PIECE_TYPES;
	b.state[5][7] = PIECE_BISHOP + NUM_PIECE_TYPES;
	b.state[6][7] = PIECE_KNIGHT + NUM_PIECE_TYPES;
	b.state[7][7] = PIECE_ROOK   + NUM_PIECE_TYPES;
	return b;
}

MoveResult board_register_move(Board* b, Move move)
{
	Point from = move.from;
	Point to = move.to;
	int piece = b->state[from.x][from.y];
	int capturedPiece = b->state[to.x][to.y];

	if (board_is_en_passant(b, move)) b->state[b->enPassantPawn.x][b->enPassantPawn.y] = -1;
	b->state[from.x][from.y] = -1;
	b->state[to.x][to.y] = piece;

	b->enPassantPawn = point_invalid();
	PieceColour colourToCheck = (b->turn == PIECE_WHITE) ? PIECE_BLACK : PIECE_WHITE;

	PieceType type = get_piece_type(piece);
	PieceType capturedType = get_piece_type(capturedPiece);

	if (type == PIECE_PAWN)
	{
		if (to.y == 0 || to.y == 7)
			return MOVE_PROMOTE;
		if (from.y == 1 && to.y == 3) b->enPassantPawn = to;
		else if (from.y == 6 && to.y == 4) b->enPassantPawn = to;
	}
	if (type == PIECE_KING)
	{
		if (from.x == 4 && to.x == 2) 
		{
			int rookID = b->state[0][from.y];
			b->state[0][from.y] = -1;
			b->state[3][from.y] = rookID;
		}
		else if (from.x == 4 && to.x == 6) 
		{
			int rookID = b->state[7][from.y];
			b->state[7][from.y] = -1;
			b->state[5][from.y] = rookID;
		}
		board_invalidate_castle(b, PIECE_KING);
		board_invalidate_castle(b, PIECE_QUEEN);
	}
	if (type == PIECE_ROOK)
	{
		if (from.x == 0 && board_can_castle(b, PIECE_QUEEN)) board_invalidate_castle(b, PIECE_QUEEN);
		if (from.x == 7 && board_can_castle(b, PIECE_KING)) board_invalidate_castle(b, PIECE_KING);
	}
	if (capturedType == PIECE_ROOK)
	{
		board_next_turn(b);
		if (to.x == 0 && board_can_castle(b, PIECE_QUEEN))
		{
			board_invalidate_castle(b, PIECE_QUEEN);
		}
		if (to.x == 7 && board_can_castle(b, PIECE_KING))
		{
			board_invalidate_castle(b, PIECE_KING);
		}
		board_next_turn(b);
	}
	Point king = board_find_king(b, colourToCheck);
	if (board_in_check(b, king))
		return (colourToCheck == PIECE_WHITE) ? MOVE_WHITE_IN_CHECK : MOVE_BLACK_IN_CHECK;

	return MOVE_OK;
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

			Point from = (Point){ .x = i, .y = j };
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

void board_next_turn(Board* b)
{
	b->turn = (b->turn == PIECE_WHITE) ? PIECE_BLACK : PIECE_WHITE;
}

void board_pawn_promote(Board* b, Point at, PieceType type)
{
	b->state[at.x][at.y] = get_piece_id(type, b->turn);
}

Point board_find_king(const Board* b, PieceColour colour)
{
	for (int j = 0; j < BOARD_CELLS; ++j)
	{
		for (int i = 0; i < BOARD_CELLS; ++i)
		{
			int piece = b->state[i][j];
			PieceColour c = get_piece_colour(piece);
			PieceType t = get_piece_type(piece);

			if (t == PIECE_KING && c == colour)
			{
				return (Point) { .x = i, .y = j };
			}
		}
	}
	return point_invalid();
}

void board_search_pawn(const Board* b, MoveArray* arr, Point from)
{
	int step = (b->turn == PIECE_WHITE) ? 1 : -1;
	int startRank = (b->turn == PIECE_WHITE) ? 1 : 6;

	Point one = point_make(from.x, from.y + step);
	Move m = move_make(from, one);
	if (one.y >= 0 && one.y < BOARD_CELLS && board_move_valid(b, m))
		move_array_push(arr, m);

	if (from.y == startRank)
	{
		Point two = point_make(from.x, from.y + 2 * step);
		m = move_make(from, two);
		if (board_move_valid(b, m))
			move_array_push(arr, m);
	}

	int captures[2] = { from.x - 1, from.x + 1 };
	for (int i = 0; i < 2; ++i)
	{
		if (captures[i] < 0 || captures[i] >= BOARD_CELLS) continue;
		Point cap = point_make(captures[i], from.y + step);
		m = move_make(from, cap);
		if (board_move_valid(b, m))
			move_array_push(arr, m);
	}
}

void board_search_rook(const Board* b, MoveArray* arr, Point from)
{
	int stepx[4] = { -1, 1, 0, 0 };
	int stepy[4] = { 0, 0, 1, -1 };

	for (int i = 0; i < 4; ++i)
	{
		Point at = from;
		at.x += stepx[i];
		at.y += stepy[i];
		while (at.x < BOARD_CELLS && at.x >= 0 &&
			at.y < BOARD_CELLS && at.y >= 0)
		{
			Move m = move_make(from, at);
			bool occupied = b->state[at.x][at.y] >= 0;
			if (board_move_valid(b, m))
				move_array_push(arr, m);
			if (occupied) break;
			at.x += stepx[i];
			at.y += stepy[i];
		}
	}
}

void board_search_knight(const Board* b, MoveArray* arr, Point from)
{
	for (int i = -2; i <= 2; ++i)
	{
		if (from.x + i >= BOARD_CELLS || from.x + i < 0) continue;
		for (int j = -2; j <= 2; ++j)
		{
			if (abs(i) == abs(j) || i == 0 || j == 0) continue;
			if (from.y + j >= BOARD_CELLS || from.y + j < 0) continue;
			Point at = point_make(from.x + i, from.y + j);
			Move m = move_make(from, at);
			if (board_move_valid(b, m))
				move_array_push(arr, m);
		}
	}
}

void board_search_bishop(const Board* b, MoveArray* arr, Point from)
{
	int stepx[4] = { -1, 1, -1, 1 };
	int stepy[4] = { -1, -1, 1, 1 };
	for (int i = 0; i < 4; ++i)
	{
		Point at = from;
		at.x += stepx[i];
		at.y += stepy[i];
		while (at.x < BOARD_CELLS && at.x >= 0 &&
			at.y < BOARD_CELLS && at.y >= 0)
		{
			Move m = move_make(from, at);
			bool occupied = b->state[at.x][at.y] >= 0;
			if (board_move_valid(b, m))
				move_array_push(arr, m);
			if (occupied) break;
			at.x += stepx[i];
			at.y += stepy[i];
		}
	}
}

void board_search_queen(const Board* b, MoveArray* arr, Point from)
{
	int rookStepx[4] = { -1, 1, 0, 0 };
	int rookStepy[4] = { 0, 0, 1, -1 };

	for (int i = 0; i < 4; ++i)
	{
		Point at = from;
		at.x += rookStepx[i];
		at.y += rookStepy[i];
		while (at.x < BOARD_CELLS && at.x >= 0 &&
			at.y < BOARD_CELLS && at.y >= 0)
		{
			Move m = move_make(from, at);
			bool occupied = b->state[at.x][at.y] >= 0;
			if (board_move_valid(b, m))
				move_array_push(arr, m);
			if (occupied) break;
			at.x += rookStepx[i];
			at.y += rookStepy[i];
		}
	}

	int bishopStepx[4] = { -1, 1, -1, 1 };
	int bishopStepy[4] = { -1, -1, 1, 1 };
	for (int i = 0; i < 4; ++i)
	{
		Point at = from;
		at.x += bishopStepx[i];
		at.y += bishopStepy[i];
		while (at.x < BOARD_CELLS && at.x >= 0 &&
			at.y < BOARD_CELLS && at.y >= 0)
		{
			Move m = move_make(from, at);
			bool occupied = b->state[at.x][at.y] >= 0;
			if (board_move_valid(b, m))
				move_array_push(arr, m);
			if (occupied) break;
			at.x += bishopStepx[i];
			at.y += bishopStepy[i];
		}
	}
}

void board_search_king(const Board* b, MoveArray* arr, Point from)
{
	for (int i = -1; i <= 1; ++i)
	{
		if (from.x + i >= BOARD_CELLS || from.x + i < 0) continue;
		for (int j = -1; j <= 1; ++j)
		{
			if (i == 0 && j == 0) continue;
			Point to = point_make(from.x + i, from.y + j);
			Move m = move_make(from, to);
			if (to.y >= 0 && to.y < BOARD_CELLS && board_move_valid(b, m))
				move_array_push(arr, m);
		}
	}
	Point castleTargets[2] = { point_make(2, from.y), point_make(6, from.y) };
	for (int i = 0; i < 2; i++) {
		Move m = move_make(from, castleTargets[i]);
		if (board_move_valid(b, m))
			move_array_push(arr, m);
	}
}


bool board_move_valid(const Board* b, Move move)
{
	Point from = move.from;
	Point to = move.to;
	if (from.x == to.x && from.y == to.y) return false;

	int piece = b->state[from.x][from.y];
	if (piece < 0) return false;

	PieceColour colour = get_piece_colour(piece);
	PieceType type = get_piece_type(piece);

	switch (type)
	{
	case PIECE_PAWN:
		bool canMove = move_valid_pawn(move_make(from, to), b->turn);
		bool canCapture = move_valid_pawn_capture(move_make(from, to), b->turn);

		if (!canMove && !canCapture) return false;
		if (!canCapture && board_blocked_pawn(b, move)) return false;
		if (!canMove && b->state[to.x][to.y] < 0 && !board_is_en_passant(b, move)) return false;
		break;
	case PIECE_ROOK:
		if (!move_valid_rook(move)) return false;
		if (board_blocked_rook(b, move)) return false;
		break;
	case PIECE_KNIGHT:
		if (!move_valid_knight(move)) return false;
		break;
	case PIECE_BISHOP:
		if (!move_valid_bishop(move)) return false;
		if (board_blocked_bishop(b, move)) return false;
		break;
	case PIECE_QUEEN:
		if (!move_valid_queen(move)) return false;
		if (move_valid_rook(move) && board_blocked_rook(b, move)) return false;
		if (move_valid_bishop(move) && board_blocked_bishop(b, move)) return false;
		break;
	case PIECE_KING:
		if (!move_valid_king(move) && 
			!board_can_castle(b, PIECE_KING) && 
			!board_can_castle(b, PIECE_QUEEN)) return false;
		if (!move_valid_king(move) && !board_castle_move_ok(b, move)) return false;
		
		break;
	}

	if (!board_can_capture(b, move) && b->state[to.x][to.y] >= 0) return false;

	if (get_piece_type(b->state[to.x][to.y]) != PIECE_KING)
	{
		Board b_copy = *b;
		board_next_turn(&b_copy);
		board_register_move(&b_copy, move);
		Point king = board_find_king(&b_copy, b->turn);
		if (board_in_check(&b_copy, king)) return false;
	}

	return true;
}

bool board_is_en_passant(const Board* b, Move move)
{
	Point from = move.from;
	Point to = move.to;
	int step = (get_piece_colour(b->state[from.x][from.y]) == PIECE_WHITE) ? -1 : 1;
	if ((b->enPassantPawn.x >= 0 && b->enPassantPawn.y >= 0) &&
		(to.x == b->enPassantPawn.x && to.y + step == b->enPassantPawn.y)) return true;
	return false;
}

bool board_can_castle(const Board* b, PieceType side)
{
	CastleValid castle = (b->turn == PIECE_WHITE) ? b->canCastleWhite : b->canCastleBlack;
	return (side == PIECE_KING) ? castle.kingSide : castle.queenSide;
}

void board_invalidate_castle(Board* b, PieceType side)
{
	switch (b->turn)
	{
	case PIECE_WHITE:
		if (side == PIECE_KING)
		{
			b->canCastleWhite.kingSide = false;
		}
		else
		{
			b->canCastleWhite.queenSide = false;
		}
		break;
	case PIECE_BLACK:
		if (side == PIECE_KING)
		{
			b->canCastleBlack.kingSide = false;
		}
		else
		{
			b->canCastleBlack.queenSide = false;
		}
		break;
	}
}

bool board_castle_move_ok(const Board* b, Move move)
{
	Point from = move.from;
	Point to = move.to;
	if (to.x != 2 && to.x != 6) return false;
	if (from.x != 4) return false;
	if (from.y != to.y) return false;
	if (to.x == 2 && b->state[1][from.y] >= 0) return false;

	if (to.x == 2 && !board_can_castle(b, PIECE_QUEEN)) return false;
	if (to.x == 6 && !board_can_castle(b, PIECE_KING)) return false;
	
	if (board_in_check(b, from)) return false;

	int step = (to.x > from.x) ? 1 : -1;
	for (int x = from.x + step; step > 0 ? x <= to.x : x >= to.x; x += step)
	{
		if (b->state[x][from.y] >= 0) return false;
		PieceColour colourToCheck = get_piece_colour(b->state[from.x][from.y]);
		Board b_copy = *b;
		b_copy.turn = colourToCheck;
		Point newPos = { .x = x, .y = from.y };
		board_register_move(&b_copy, move_make(from, newPos));
		if (board_in_check(&b_copy, newPos)) return false;
	}
	return true;
}

bool board_in_check(const Board* b, Point king)
{
	int kingx = king.x;
	int kingy = king.y;

	if (kingx < 0 || kingy < 0) return false;
	if (get_piece_type(b->state[kingx][kingy]) != PIECE_KING) return false;
	PieceColour colourToCheck = get_piece_colour(b->state[kingx][kingy]);
	Board b_copy = *b;
	b_copy.turn = (colourToCheck == PIECE_WHITE) ? PIECE_BLACK : PIECE_WHITE;

	for (int i = 0; i < BOARD_CELLS; ++i)
	{
		for (int j = 0; j < BOARD_CELLS; ++j)
		{
			int piece = b->state[i][j];
			if (piece < 0) continue;
			PieceColour c = get_piece_colour(piece);
			PieceType t = get_piece_type(piece);
			if (get_piece_colour(piece) == colourToCheck) continue;

			Point from = { .x = i, .y = j};
			if (board_move_valid(&b_copy, move_make(from, king))) return true;
		}
	}

	return false;
}

bool board_no_moves(const Board* b, PieceColour colourToCheck)
{
	Board b_copy = *b;
	b_copy.turn = colourToCheck;
	MoveArray arr = move_array_init();
	engine_search(&b_copy, &arr);
	return arr.len == 0;
}

bool board_blocked_pawn(const Board* b, Move move)
{
	Point from = move.from;
	Point to = move.to;
	bool firstMove = false;
	switch (b->turn) {
	case PIECE_WHITE:
		firstMove = abs(to.y - from.y) == 2 && b->state[from.x][from.y + 2] >= 0;
		if (b->state[from.x][from.y + 1] >= 0) return true;
		if (firstMove) return true;
		break;
	case PIECE_BLACK:
		firstMove = abs(to.y - from.y) == 2 && b->state[from.x][from.y - 2] >= 0;
		if (b->state[from.x][from.y - 1] >= 0) return true;
		if (firstMove) return true;
		break;
	}
	return false;
}

bool board_blocked_rook(const Board* b, Move move)
{
	Point from = move.from;
	Point to = move.to;
	if (from.x == to.x)
	{
		int step = (to.y > from.y) ? 1 : -1;
		for (int y = from.y + step; y != to.y; y += step)
			if (b->state[from.x][y] >= 0) return true;
	}
	else
	{
		int step = (to.x > from.x) ? 1 : -1;
		for (int x = from.x + step; x != to.x; x += step)
			if (b->state[x][from.y] >= 0) return true;
	}
	return false;
}

bool board_blocked_bishop(const Board* b, Move move)
{
	Point from = move.from;
	Point to = move.to;
	int xstep = (to.x > from.x) ? 1 : -1;
	int ystep = (to.y > from.y) ? 1 : -1;

	int x = from.x + xstep;
	int y = from.y + ystep;

	while (x != to.x)
	{
		if (b->state[x][y] >= 0) return true;
		x += xstep;
		y += ystep;
	}
	return false;
}

bool board_can_capture(const Board* b, Move move)
{
	return get_piece_colour(b->state[move.to.x][move.to.y]) != b->turn;
}

bool board_mouse_over(float x, float y)
{
	if (x > BOARD_OFFSET_X && x < BOARD_OFFSET_X + BOARD_PX &&
		y > BOARD_OFFSET_Y && y < BOARD_OFFSET_Y + BOARD_PX) 
	{
		return true;
	}
	return false;
}

Point board_mouse_coords(float x, float y)
{
	int i, j;

	i = (int)(x - BOARD_OFFSET_X) / CELL_SIZE;
	j = BOARD_CELLS - 1 - (int)(y - BOARD_OFFSET_Y) / CELL_SIZE;

	return point_make(i, j);
}

bool board_select_valid(const Board* b, Point clicked)
{
	int x = clicked.x;
	int y = clicked.y;

	if (x < 0 || y < 0) return false;

	if (b->state[x][y] < 0) return false;

	int piece = b->state[x][y];
	if (get_piece_colour(piece) != b->turn) return false;

	return true;
}


