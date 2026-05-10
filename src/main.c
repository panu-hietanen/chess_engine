#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "board.h"
#include "moves.h"
#include "piece.h"
#include "utils.h"

int main(void)
{
	Board b = board_init_game();
	GameState state = STATE_DEFAULT;

	print_board(&b);

	char line[16];
	while (state == STATE_DEFAULT || state == STATE_PIECE_MOVING || state == STATE_PROMOTION_SELECTION)
	{
		printf("%s to move: ", b.turn == PIECE_WHITE ? "White" : "Black");
		if (!fgets(line, sizeof(line), stdin)) break;

		Point from, to;
		if (strlen(line) < 5 || !parse_square(line, &from) || !parse_square(line + 3, &to))
		{
			printf("Enter move as 'e2 e4'\n");
			continue;
		}

		Move m = move_make(from, to);
		if (!board_move_valid(&b, m))
		{
			printf("Invalid move.\n");
			continue;
		}

		MoveResult result = board_register_move(&b, m);
		switch (result)
		{
		case MOVE_OK:
			board_next_turn(&b);
			if (board_no_moves(&b, b.turn))
				state = STATE_STALEMATE;
			break;
		case MOVE_WHITE_IN_CHECK:
			if (board_no_moves(&b, PIECE_WHITE)) state = STATE_BLACK_WON;
			else board_next_turn(&b);
			break;
		case MOVE_BLACK_IN_CHECK:
			if (board_no_moves(&b, PIECE_BLACK)) state = STATE_WHITE_WON;
			else board_next_turn(&b);
			break;
		case MOVE_PROMOTE:
			bool promoted = false;
			PieceType promotion_piece;
			while (!promoted)
			{
				printf("Enter piece to promote to (q/r/b/n): ");
				if (!fgets(line, sizeof(line), stdin)) break;
				if (strlen(line) < 1 || !parse_promote(line, &promotion_piece))
				{
					printf("Enter valid piece.\n");
					continue;
				}
				promoted = true;
			}
			board_pawn_promote(&b, to, promotion_piece);
			board_next_turn(&b);
			break;
		}

		print_board(&b);
	}

	if (state == STATE_WHITE_WON) printf("White wins by checkmate!\n");
	else if (state == STATE_BLACK_WON) printf("Black wins by checkmate!\n");
	else if (state == STATE_STALEMATE) printf("Stalemate!\n");

	return 0;
}
