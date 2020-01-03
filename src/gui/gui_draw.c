#include "gui_draw.h"
#include "..\init_video.h"
#include "..\tetris.h"
#include <SDL2/SDL.h>

#define RED(x) ((x & 0xFF0000) >> 16)
#define GREEN(x) ((x & 0x00FF00) >> 8)
#define BLUE(x) (x & 0x0000FF)
#define FIELD_X ((SCREEN_WIDTH - (XSIZE * SQUARE_SIZE)) / 2)
#define FIELD_Y ((SCREEN_HEIGHT - (YSIZE * SQUARE_SIZE)) / 2)
#define NEXT_X (XSIZE * SQUARE_SIZE + FIELD_X)
#define NEXT_Y (FIELD_Y)

/*
 *Les couleurs des blocks
 */
int block_c[9] = {0x00FFFF, 0x0000FF, 0xFFAA00, 0xFFFF00, 0x00FF00,
                  0x9900FF, 0xFF0000, 0xFFFFFF, 0x000000};
#define C_WHITE 7
#define C_BLACK 8

/******************************************************************************
 * Déclaration des fonctions propre à gui_draw
 ******************************************************************************/
/* x et y : coordonnées de la "case" a remplir dans le field */
void draw_field_squar(int x, int y, int c);
void draw_field_squar_ext(int x, int y, int c, int alpha);

/* x et y en pixel */
void draw_squar(int x, int y, int w, int c, int alpha);
void draw_rect(int x, int y, int w, int h, int c, int alpha, bool fill);

/* pour les next block */
void draw_mini_block(int type, int x, int y, int size);

/******************************************************************************
 * Définitions des fonctions
 ******************************************************************************/
/* Dessine un block */
void draw_block(t_block t, int alpha) {
  int x, y;
  for (y = 0; y < 4; y++)
    for (x = 0; x < 4; x++)
      if (block_xy(t, x, y))
        draw_field_squar_ext(t.x + x, t.y + y, t.type, alpha);
}

void draw_next_blocks(t_tetris_game g) {
  int i, size = SQUARE_SIZE / 2, dec = 0, n = 4;
  draw_rect(NEXT_X, NEXT_Y, 5 * size, n * 5 * size, C_WHITE, 100, true);
  draw_rect(NEXT_X, NEXT_Y - 1, 5 * size, n * 5 * size, C_WHITE, 255, false);
  for (i = 0; i < n; i++) {
    dec = (g.next[i] == 0 || g.next[i] == 3) ? -size / 2 : 0;
    draw_mini_block(g.next[i], NEXT_X + size + dec,
                    NEXT_Y + size + 5 * i * size, size);
  }
}

void draw_hold_block(t_tetris_game g) {
  int size = SQUARE_SIZE / 2, dec = 0, decy = 0;
  draw_rect(FIELD_X - 5 * size, FIELD_Y, 5 * size, 5 * size, C_WHITE, 100,
            true);
  draw_rect(FIELD_X - 5 * size - 1, FIELD_Y - 1, 5 * size + 1, 5 * size + 1,
            C_WHITE, 255, false);
  // draw_rect(NEXT_X, NEXT_Y - 1, 5 * size, n * 5 * size, C_WHITE, 255, false);
  dec = (g.hold_block == 0 || g.hold_block == 3) ? -size / 2 : 0;
  decy = (g.hold_block != 0) ? size / 2 : 0;
  draw_mini_block(g.hold_block, FIELD_X - 4 * size + dec, FIELD_Y + size + decy,
                  size);
}

/* Dessine un mini block en x y */
void draw_mini_block(int type, int x, int y, int size) {
  int xx, yy;
  if (type >= 0) {
    t_block t = {.type = type, .rotate = 0};
    for (yy = 0; yy < 4; yy++)
      for (xx = 0; xx < 4; xx++)
        if (block_xy(t, xx, yy))
          draw_squar(x + xx * size, y + yy * size, size, type, 255);
  }
}

/* dessine le block "ghost" */
void draw_ghost_block(t_tetris_game g, int alpha) {
  t_block ghost = g.current;
  while (block_valid(g, ghost)) {
    ghost.y++;
  }
  ghost.y--;
  draw_block(ghost, alpha);
}

/* Dessine le field */
void draw_field(t_tetris_game g) {
  int x, y;
  draw_rect(FIELD_X - 1, FIELD_Y - 1, 2 + XSIZE * SQUARE_SIZE,
            2 + YSIZE * SQUARE_SIZE, C_WHITE, 255, false);
  for (y = 0; y < YSIZE; y++)
    for (x = 0; x < XSIZE; x++)
      draw_field_squar(x, y, g.f[x][y]);
}

/* Dessine le quadrillage */
void draw_grid(int alpha) {
  int x, y;
  for (y = 0; y < YSIZE; y++) {
    for (x = 0; x < XSIZE; x++) {
      draw_field_squar_ext(x, y, C_BLACK, alpha); // carré noir avec l'alpha
      draw_rect(FIELD_X + x * SQUARE_SIZE, FIELD_Y + y * SQUARE_SIZE,
                SQUARE_SIZE, SQUARE_SIZE, C_WHITE, alpha, false);
    }
  }
}

/* draw_field_squar et  draw_field_squar_ext */
void draw_field_squar(int x, int y, int c) {
  draw_field_squar_ext(x, y, c, 255);
}

void draw_field_squar_ext(int x, int y, int c, int alpha) {
  if (c != -1 && x >= 0 && x < XSIZE && y >= 0 && y < YSIZE)
    draw_squar(FIELD_X + x * SQUARE_SIZE, FIELD_Y + y * SQUARE_SIZE,
               SQUARE_SIZE, c, alpha);
}

/* draw_squar : dessine un carré ... ou un mouton ! */
void draw_squar(int x, int y, int w, int c, int alpha) {
  draw_rect(x, y, w, w, c, alpha, true);
}

/* draw_rect : dessine un rectangle vide ou plein*/
void draw_rect(int x, int y, int w, int h, int c, int alpha, bool fill) {
  SDL_Rect r = {.x = x, .y = y, .w = w, .h = h};
  if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
    SDL_SetRenderDrawColor(renderer, RED(block_c[c]), GREEN(block_c[c]),
                           BLUE(block_c[c]), alpha);
    if (!fill)
      SDL_RenderDrawRect(renderer, &r);
    else
      SDL_RenderFillRect(renderer, &r);
  }
}
