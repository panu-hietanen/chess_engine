#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "board.h"
#include "moves.h"
#include "piece.h"
#include "utils.h"
#include "engine.h"

int main(void)
{
	Board b = board_init_game();
	GameState state = STATE_DEFAULT;

	Engine* engine = engine_create(WEIGHTS_PATH);

	print_board(&b);

	char line[16];
	while (state == STATE_DEFAULT || state == STATE_PIECE_MOVING || state == STATE_PROMOTION_SELECTION)
	{
		printf("%s to move: ", b.turn == PIECE_WHITE ? "White" : "Black");
		Move m;
		Point from, to;
		if (b.turn == PIECE_WHITE)
		{
			if (!fgets(line, sizeof(line), stdin))
				break;

			if (strlen(line) < 5 || !parse_square(line, &from) || !parse_square(line + 3, &to))
			{
				printf("Enter move as 'e2 e4'\n");
				continue;
			}

			m = move_make(from, to);
			if (!board_move_valid(&b, m))
			{
				printf("Invalid move.\n");
				continue;
			}

		}
		else
		{
			m = engine_best_move(engine, &b);
			char from_char = 'a' + m.from.x;
			char to_char = 'a' + m.to.x;
			printf(
				"Black played %c%d to %c%d\n", 
				from_char, m.from.y, to_char, m.to.y
			);
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
			if (board_no_moves(&b, PIECE_WHITE))
				state = STATE_BLACK_WON;
			else
				board_next_turn(&b);
			break;
		case MOVE_BLACK_IN_CHECK:
			if (board_no_moves(&b, PIECE_BLACK))
				state = STATE_WHITE_WON;
			else
				board_next_turn(&b);
			break;
		case MOVE_PROMOTE:
			bool promoted = false;
			PieceType promotion_piece;
			if (b.turn == PIECE_WHITE)
			{
				while (!promoted)
				{
					printf("Enter piece to promote to (q/r/b/n): ");
					if (!fgets(line, sizeof(line), stdin))
						break;
					if (strlen(line) < 1 || !parse_promote(line, &promotion_piece))
					{
						printf("Enter valid piece.\n");
						continue;
					}
					promoted = true;
				}
				board_pawn_promote(&b, to, promotion_piece);
			}
			else
			{
				promotion_piece = PIECE_QUEEN;
				board_pawn_promote(&b, m.to, promotion_piece);
			}
			board_next_turn(&b);
			break;
		}
		// float features[FEATURES];
		// board_to_features(&b, features);
		// float y = engine_forward(engine, features);
		// y = (y - 0.5f) * 200.0f;
		// printf("Estimated score: %f\n", y);
		print_board(&b);
	}

	if (state == STATE_WHITE_WON) printf("White wins by checkmate!\n");
	else if (state == STATE_BLACK_WON) printf("Black wins by checkmate!\n");
	else if (state == STATE_STALEMATE) printf("Stalemate!\n");

	engine_destroy(engine);
	return 0;
}
