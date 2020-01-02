#include "tetris.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Les tetrominos et leurs rotations : https://tetris.fandom.com/wiki/SRS
 */
int blocks[7][4] = {
    {0x0F00, 0x2222, 0x00F0, 0x4444}, {0x8E00, 0x6440, 0x0E20, 0x44C0},
    {0x2E00, 0x4460, 0x0E80, 0xC440}, {0x6600, 0x6600, 0x6600, 0x6600},
    {0x6C00, 0x4620, 0x06C0, 0x8C40}, {0x4E00, 0x4640, 0x0E40, 0x4C40},
    {0xC600, 0x2640, 0x0C60, 0x4C80}};

/*
 * Table de tests pour les rotations : https://tetris.fandom.com/wiki/SRS
 */
int SRS_test_table[8][5][2] = {
    {{0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2}},  /* 0 >> 1 */
    {{0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2}},      /* 1 >> 2 */
    {{0, 0}, {1, 0}, {1, 1}, {0, -2}, {1, -2}},     /* 2 >> 3 */
    {{0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2}},   /* 3 >> 0 */
    {{0, 0}, {1, 0}, {1, 1}, {0, -2}, {1, -2}},     /* 0 >> 3 */
    {{0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2}},      /* 1 >> 0 */
    {{0, 0}, {-1, 0}, {-1, -1}, {0, -2}, {-1, -2}}, /* 2 >> 1 */
    {{0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2}}    /* 3 >> 2 */
};

int SRS_I_test_table[8][5][2] = {
    {{0, 0}, {-2, 0}, {1, 0}, {-2, -1}, {1, 2}}, /* 0 >> 1 */
    {{0, 0}, {-1, 0}, {2, 0}, {-1, 2}, {2, -1}}, /* 1 >> 2 */
    {{0, 0}, {2, 0}, {-1, 0}, {2, 1}, {-1, -2}}, /* 2 >> 3 */
    {{0, 0}, {1, 0}, {-2, 0}, {1, -2}, {-2, 1}}, /* 3 >> 0 */
    {{0, 0}, {-1, 0}, {2, 0}, {-1, 2}, {2, -1}}, /* 0 >> 3 */
    {{0, 0}, {2, 0}, {-1, 0}, {2, 1}, {-1, -2}}, /* 1 >> 0 */
    {{0, 0}, {1, 0}, {-2, 0}, {1, -2}, {-2, 1}}, /* 2 >> 1 */
    {{0, 0}, {-2, 0}, {1, 0}, {-2, -1}, {1, 2}}  /* 3 >> 2 */
};

/*
 * Les fonctions
 */
int check_lines(t_tetris_game *g) {
  int x, y, yy;
  int nb_lines_clear = 0;
  bool line_clear = true;
  for (y = 0; y < YSIZE; y++) {
    line_clear = true;
    for (x = 0; x < XSIZE; x++) {
      if (g->f[x][y] == -1)
        line_clear = false;
    }
    if (line_clear) {
      nb_lines_clear++;
      for (yy = y; yy > 0; yy--)
        for (x = 0; x < XSIZE; x++)
          g->f[x][yy] = g->f[x][yy - 1];
      for (x = 0; x < XSIZE; x++)
        g->f[x][0] = -1;
    }
  }
  return nb_lines_clear;
}

bool rotate_block(t_tetris_game *g, bool clockwise) {
  t_block t = g->current;
  int test_index = t.rotate + ((!clockwise) ? 4 : 0);
  int test_number;
  for (test_number = 0; test_number < 5; test_number++) {
    t = g->current;
    if (t.type > 0) {
      t.x += SRS_test_table[test_index][test_number][0];
      t.y -= SRS_test_table[test_index][test_number][1];
    } else {
      t.x += SRS_I_test_table[test_index][test_number][0];
      t.y -= SRS_I_test_table[test_index][test_number][1];
    }
    t.rotate = (t.rotate + ((clockwise) ? 1 : -1)) % 4;
    if (block_valid(*g, t)) {
      g->current = t;
      return true;
    }
  }
  return false;
}

bool move_block(t_tetris_game *g, char dir) {
  t_block t = g->current;
  switch (dir) {
  case 'l':
    t.x--;
    break;
  case 'r':
    t.x++;
    break;
  case 'd':
    t.y++;
    break;
  default:
    return false;
    break;
  }
  if (block_valid(*g, t)) {
    g->current = t;
    return true;
  }
  return false;
}

bool drop_block(t_tetris_game *g) {
  int x, y;
  t_block b = g->current;
  if (!block_valid((*g), b))
    return false;
  while (block_valid((*g), b)) {
    b.y++;
  }
  b.y--;
  for (y = 0; y < 4; y++) {
    for (x = 0; x < 4; x++) {
      if (block_xy(b, x, y))
        g->f[b.x + x][b.y + y] = b.type;
    }
  }
  return true;
}

void t_tetris_game_init(t_tetris_game *g) {
  int x, y, i;
  for (x = 0; x < XSIZE; x++)
    for (y = 0; y < YSIZE; y++)
      g->f[x][y] = -1;
  generate_next_blocks(g);
  for (i = 0; i < 7; i++) {
    g->next[i] = g->next[i + 7];
  }
  generate_next_blocks(g);
  update_current_block(g);
}

void generate_next_blocks(t_tetris_game *g) {
  int tab[7] = {0, 1, 2, 3, 4, 5, 6};
  int i, i1, i2, temp;
  for (i = 0; i < 400; i++) {
    i1 = rand() % 7;
    i2 = rand() % 7;
    temp = tab[i1];
    tab[i1] = tab[i2];
    tab[i2] = temp;
  }
  for (i = 0; i < 7; i++) {
    (*g).next[i + 7] = tab[i];
  }
}

void update_current_block(t_tetris_game *g) {
  int i;
  g->current.type = g->next[0];
  g->current.rotate = 0;
  g->current.x = -1 + (XSIZE / 2);
  g->current.y = 0;
  for (i = 0; i < 13; i++) {
    g->next[i] = g->next[i + 1];
  }
  g->next[13] = -1;
  if (g->next[7] == -1)
    generate_next_blocks(g);
}

bool block_xy(t_block b, int x, int y) {
  int tmp;
  if (!in(x, 0, 3) || !in(y, 0, 3))
    return false;
  tmp = blocks[b.type][b.rotate];
  return (((tmp << (x + 4 * y)) & 0x8000) > 0);
}

bool block_valid(t_tetris_game g, t_block b) {
  int x, y;
  for (x = 0; x < 4; x++)
    for (y = 0; y < 4; y++)
      if (block_xy(b, x, y)) {
        if (!in(b.x + x, 0, XSIZE - 1) || !in(b.y + y, 0, YSIZE - 1))
          return false;
        if (g.f[b.x + x][b.y + y] != (-1))
          return false;
      }
  return true;
}
