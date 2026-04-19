#include "render.h"
#include "config.h"
#include "utils.h"

#include "piece.h"

void create_texture(Texture2D* t, PieceColour pColour, PieceType pType, int w, int h)
{
	char name[MAX_PATH_LENGTH];
	get_piece_path(name, pColour, pType);

	Image im = LoadImage(name);
	ImageResize(&im, w, h);
	
	*t = LoadTextureFromImage(im);
	UnloadImage(im);
}

void board_draw()
{
	for (int i = 0; i < BOARD_CELLS; ++i)
	{
		for (int j = 0; j < BOARD_CELLS; ++j)
		{
			Color c = ((i + j) % 2 == 0) ? BOARD_COLOUR_LIGHT : BOARD_COLOUR_DARK;
			DrawRectangle(
				BOARD_OFFSET_X + j * CELL_SIZE,
				BOARD_OFFSET_Y + i * CELL_SIZE,
				CELL_SIZE, CELL_SIZE,
				c
			);
		}
	}
}

void board_draw_piece(const Board* b, const PieceTextures* pt, const Point p)
{
	if (p.x < 0 || p.y < 0) return;
	int piece = b->state[p.x][p.y];
	if (piece < 0) return;

	const PieceColour colour = get_piece_colour(piece);
	const PieceType type = get_piece_type(piece);

	const int sx = BOARD_OFFSET_X + p.x * CELL_SIZE;
	const int sy = BOARD_OFFSET_Y + (BOARD_CELLS - 1 - p.y) * CELL_SIZE;
	DrawTexture(pt->data[colour][type], sx, sy, WHITE);
}

void board_state_draw(const Board* b, const PieceTextures* pt)
{
	for (int i = 0; i < BOARD_CELLS; ++i)
	{
		for (int j = 0; j < BOARD_CELLS; ++j)
		{
			board_draw_piece(b, pt, (Point) { .x = i, .y = j });
		}
	}
}

void board_draw_highlight(const Board* b, const PieceTextures* pt, Point clicked)
{
	if (board_select_valid(b, clicked))
	{
		int x = clicked.x;
		int y = clicked.y;
		DrawRectangle(
			BOARD_OFFSET_X + x * CELL_SIZE,
			SCREEN_H - BOARD_OFFSET_Y - (y + 1) * CELL_SIZE,
			CELL_SIZE, CELL_SIZE,
			MAROON
		);
		board_draw_piece(b, pt, clicked);
	}
	Point king = board_find_king(b, b->turn);
	if (board_in_check(b, king))
	{
		int x = king.x;
		int y = king.y;
		DrawRectangle(
			BOARD_OFFSET_X + x * CELL_SIZE,
			SCREEN_H - BOARD_OFFSET_Y - (y + 1) * CELL_SIZE,
			CELL_SIZE, CELL_SIZE,
			SKYBLUE
		);
		board_draw_piece(b, pt, king);
	}
}

void board_draw_moves(const Board* b, Point clicked)
{
	int piece = b->state[clicked.x][clicked.y];

	for (int i = 0; i < BOARD_CELLS; ++i)
	{
		for (int j = 0; j < BOARD_CELLS; ++j)
		{
			Point to = { .x = i, .y = j };
			
			if (!board_move_valid(b, move_make(clicked, to))) continue;

			DrawCircle(
				BOARD_OFFSET_X + i * CELL_SIZE + CELL_SIZE / 2,
				SCREEN_H - BOARD_OFFSET_Y - j * CELL_SIZE - CELL_SIZE / 2,
				MOVE_IND_RADIUS, BROWN
			);
		}
	}
}

void ui_draw(const Board* b, const PieceTextures* pt, GameState state)
{
	const double recSide = 80;
	Color turnColour = (b->turn == PIECE_WHITE) ? WHITE : BLACK;
	DrawRectangle((BOARD_OFFSET_X - recSide) / 2, SCREEN_H / 2, recSide, recSide, turnColour);
	DrawText("Turn:", (BOARD_OFFSET_X - 3 * recSide / 4) / 2, SCREEN_H / 2 - 26, 19, BLACK);

	if (state == STATE_PROMOTION_SELECTION)
	{
		DrawRectangleLines(PROMOTE_OFFSET_X, PROMOTE_OFFSET_Y, PROMOTE_BOX_W, PROMOTE_H, WHITE);
		int x = PROMOTE_OFFSET_X + 0 * PROMOTE_W;
		DrawTexture(pt->data[b->turn][PIECE_QUEEN], x, PROMOTE_OFFSET_Y, WHITE);
		x = PROMOTE_OFFSET_X + 1 * PROMOTE_W;
		DrawTexture(pt->data[b->turn][PIECE_ROOK], x, PROMOTE_OFFSET_Y, WHITE);
		x = PROMOTE_OFFSET_X + 2 * PROMOTE_W;
		DrawTexture(pt->data[b->turn][PIECE_BISHOP], x, PROMOTE_OFFSET_Y, WHITE);
		x = PROMOTE_OFFSET_X + 3 * PROMOTE_W;
		DrawTexture(pt->data[b->turn][PIECE_KNIGHT], x, PROMOTE_OFFSET_Y, WHITE);
	}

	int restartx, restarty;
	Color restartColour = WHITE;
	if (state == STATE_WHITE_WON || state == STATE_BLACK_WON)
	{
		Rectangle rec = { .x = WON_OFFSET_X, .y = WON_OFFSET_Y, .width = WON_W, .height = WON_H };
		DrawRectangleRec(rec, WHITE);
		DrawRectangleLinesEx(rec, 10, BLACK);
		const char* winText = (state == STATE_WHITE_WON)
			? "White won by checkmate!"
			: "Black won by checkmate!";
		int textWidth = MeasureText(winText, 50);
		DrawText(winText, WON_OFFSET_X + (WON_W - textWidth)/ 2, WON_OFFSET_Y + 50, 50, BLACK);
		restartx = (SCREEN_W - RESTART_W) / 2;
		restarty = SCREEN_H / 2 + 100;
		restartColour = BLACK;
	}
	else
	{
		restartx = RESTART_OFFSET_X;
		restarty = RESTART_OFFSET_Y;
	}
	const char* msg = "RESTART";
	Vector2 msgSize = MeasureTextEx(GetFontDefault(), msg, 30, 0);
	int msgWidth = MeasureText(msg, 30);
	int msgHeight = msgSize.y;
	DrawRectangleLines(restartx, restarty, RESTART_W, RESTART_H, restartColour);
	DrawText(msg, restartx + (RESTART_W - msgWidth) / 2, restarty + (RESTART_H - msgHeight) / 2, 30, BLACK);
}

void piece_textures_load(PieceTextures* pt, int w, int h)
{
	for (PieceColour colour = 0; colour < 2; ++colour)
	{
		for (PieceType type = 0; type < 6; ++type)
		{
			create_texture(&pt->data[colour][type], colour, type, w, h);
		}
	}
}

void piece_textures_unload(PieceTextures* pt)
{
	for (int colour = 0; colour < 2; ++colour)
	{
		for (int type = 0; type < 6; ++type)
		{
			UnloadTexture(pt->data[colour][type]);
		}
	}
}
