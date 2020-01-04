#ifndef _APP_H_
#define _APP_H_
//
#include "tetris.h"
//
#define GS_MENU         0x10
#define GS_PLAY_RESTART 0x11
#define GS_PLAY         0x20
#define GS_PAUSE        0x21
#define GS_GAMEOVER     0x22
#define GS_QUIT         0x40

int           game_status;
int           score;
int           nb_lines_clear;
t_tetris_game g;

int play();
// int play_tetris(t_tetris_game *ptr_g);
int menu();
#endif
