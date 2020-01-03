#ifndef _APP_H_
#define _APP_H_
//
#include "tetris.h"
//
#define GS_MENU 1
#define GS_PLAY 2
#define GS_QUIT 3

int game_status;
int score;
int nb_lines_clear;

int play_tetris(t_tetris_game *ptr_g);
int menu();
#endif
