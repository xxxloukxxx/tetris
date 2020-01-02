#ifndef _TETRIS_H_
#define _TETRIS_H_
/*
 * includes
 */
#include <stdbool.h>
#include <stdlib.h>

/*
 * Taille du field
 */
#define XSIZE 10
#define YSIZE 20

/*
 * Les types t_block et t_tetris_game
 */
typedef struct {
  int x, y;
  unsigned int type, rotate;
} t_block;

typedef struct {
  int f[XSIZE][YSIZE];
  t_block current;
  int next[14];
  bool hold_ok;
  int hold_block;
} t_tetris_game;

/*
 * Les tetrominos et leurs rotations : https://tetris.fandom.com/wiki/SRS
 */
int blocks[7][4];
/*
 * Table de tests pour les rotations : https://tetris.fandom.com/wiki/SRS
 */
int SRS_test_table[8][5][2];
int SRS_I_test_table[8][5][2];

/*
 * Un define bien pratique : vrai si x \in [a,b]
 */
#ifndef in
#define in(x, a, b) ((x >= a) && (x <= b))
#endif

/*
 * Les fonctions
 */
void t_tetris_game_init(t_tetris_game *g);
bool move_block(t_tetris_game *g, char dir);
bool rotate_block(t_tetris_game *g, bool clockwise);
bool drop_block(t_tetris_game *g);
bool hold(t_tetris_game *g);
int check_lines(t_tetris_game *g);
//
bool block_xy(t_block b, int x, int y);
bool block_valid(t_tetris_game g, t_block b);
void generate_next_blocks(t_tetris_game *g);
void update_current_block(t_tetris_game *g);
//

#endif
