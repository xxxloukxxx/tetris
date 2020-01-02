#ifndef _GUI_DRAW_H_
#define _GUI_DRAW_H_

#include "..\tetris.h"
#include <SDL2/SDL.h>

#define SQUARE_SIZE 28

int block_c[9];

/******************************************************************************
 * Déclaration des fonctions
 ******************************************************************************/
void draw_field(t_tetris_game g);
void draw_block(t_block t, int alpha);
void draw_next_blocks(t_tetris_game g);
void draw_hold_block(t_tetris_game g);
void draw_ghost_block(t_tetris_game g, int alpha);
void draw_grid(int alpha);

#endif
