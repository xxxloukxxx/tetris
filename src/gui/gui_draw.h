#ifndef _GUI_DRAW_H_
#define _GUI_DRAW_H_

#include "..\tetris.h"

// Taille des carrés
#define SQUARE_SIZE 32

#define FIELD_X (((SCREEN_WIDTH / 2) - (XSIZE * SQUARE_SIZE)) / 2)
#define FIELD_Y ((SCREEN_HEIGHT - (YSIZE * SQUARE_SIZE)) / 2)
#define INFO_X  (SCREEN_WIDTH / 2)

void draw_game(t_tetris_game g);
void draw_score(int *draw_line, int score, int nb_lines_clear);
void draw_info_text(char *str, int *l);
void clear_screen();

#endif
