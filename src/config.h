#ifndef CONFIG_H__
#define CONFIG_H__

#define CELL_SIZE             80
#define BOARD_CELLS			  8
#define BOARD_PX		      (CELL_SIZE * BOARD_CELLS)
#define BOARD_COLOUR_LIGHT    GetColor(0xb58762ff)
#define BOARD_COLOUR_DARK     GetColor(0xefd9b5ff)
#define BACKGROUND_COLOUR	  BROWN
#define MOVE_IND_RADIUS       10

#define MAX_MOVE_ARR_SIZE	  256

#define PIECE_W				  80
#define PIECE_H				  80
#define NUM_PIECE_TYPES		  6

#define SCREEN_W			  1280
#define SCREEN_H			  720

#define BOARD_OFFSET_X		  ((SCREEN_W - BOARD_PX) / 2)
#define BOARD_OFFSET_Y		  ((SCREEN_H - BOARD_PX) / 2)

#define WON_W				  1000
#define WON_H				  600
#define WON_OFFSET_X		  ((SCREEN_W - WON_W) / 2)
#define WON_OFFSET_Y		  ((SCREEN_H - WON_H) / 2)

#define RESTART_W			  200
#define RESTART_H			  80
#define RESTART_OFFSET_X	  ((BOARD_OFFSET_X) / 2 - 100)
#define RESTART_OFFSET_Y	  (BOARD_OFFSET_Y)

#define PROMOTE_W			  (3 * PIECE_W / 4)
#define PROMOTE_H			  (3 * PIECE_H / 4)
#define PROMOTE_BOX_W		  (PROMOTE_W * 4)
#define PROMOTE_OFFSET_X	  (BOARD_OFFSET_X + BOARD_PX + 50)
#define PROMOTE_OFFSET_Y	  (BOARD_OFFSET_Y)

#define RESOURCES_PATH		  "C:/Dev/C/chess_v1/resources/"
#define MAX_PATH_LENGTH		  48
#define MAX_PIECE_NAME_LENGTH 16


#endif //CONFIG_H__