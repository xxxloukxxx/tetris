#ifndef _GUI_H_
#define _GUI_H_
#include "tetris.h"
#include <SDL2/SDL.h>

#define SQUARE_W 24

Uint8 block_color[8][3]; /* le dernier est blanc */

void draw_square(int x, int y, Uint8 c);
void draw_square_ext(int x, int y, Uint8 c, Uint8 a);
void clear_screen();
void draw_field(t_tetris_game g, t_block t);

#endif
