#include "gui.h"
#include "init_video.h"
#include "tetris.h"
#include <SDL2/SDL.h>

/*
 *
 */
void draw_rect(int x, int y, int w, int h, Uint8 c);
/*
 *
 */
Uint8 block_color[8][3] = {{0x00, 0xFF, 0xFF}, {0x00, 0x00, 0xFF},
                           {0xFF, 0xAA, 0x00}, {0xFF, 0xFF, 0x00},
                           {0x00, 0xFF, 0x00}, {0x99, 0x00, 0xFF},
                           {0xFF, 0x00, 0x00}, {0xFF, 0xFF, 0xFF}};
/*
 *
 */
void draw_field(t_tetris_game g) {
  int x, y, c = 0;
  t_block t = g.current;
  t_block ghost = t;
  /* Position de départ de la fenetre */
  int xd = (SCREEN_WIDTH - XSIZE * SQUARE_W) / 2,
      yd = (SCREEN_HEIGHT - YSIZE * SQUARE_W) / 2;

  /* Le contour blanc */
  for (c = 1; c < 5; c++) {
    draw_rect(xd - c, yd - c, (XSIZE * SQUARE_W) + 2 * c,
              (YSIZE * SQUARE_W) + 2 * c, 7);
  }

  /* La grille */
  SDL_SetRenderDrawColor(renderer, 50, 50, 50, 100);
  for (x = 0; x < XSIZE + 1; x++) {
    SDL_RenderDrawLine(renderer, xd + x * SQUARE_W, yd, xd + x * SQUARE_W,
                       yd + YSIZE * SQUARE_W - 1);
  }
  for (y = 0; y < YSIZE + 1; y++) {
    SDL_RenderDrawLine(renderer, xd, yd + y * SQUARE_W,
                       xd + XSIZE * SQUARE_W - 1, yd + y * SQUARE_W);
  }

  /* Le contenu */
  for (y = 0; y < YSIZE; y++) {
    for (x = 0; x < XSIZE; x++) {
      /* On récupère le contenu du field */
      c = g.f[x][y];
      /* Si on doit afficher le block, on l'affiche */
      if (in(x - t.x, 0, 3) && in(y - t.y, 0, 3) &&
          (block_xy(t, x - t.x, y - t.y)))
        c = t.type;
      /* Si le block à afficher est différent de -1*/
      if (c != -1)
        draw_square(xd + x * SQUARE_W, yd + y * SQUARE_W, c);
    }
  }

  /* Pour le block fantôme */
  while (block_valid(g, ghost)) {
    ghost.y++;
  }
  ghost.y--;
  if (ghost.y >= 0) {
    for (y = 0; y < 4; y++) {
      for (x = 0; x < 4; x++) {
        if (block_xy(ghost, x, y))
          draw_square_ext(xd + (x + ghost.x) * SQUARE_W,
                          yd + (y + ghost.y) * SQUARE_W, ghost.type, 50);
      }
    }
  }

  /* Draw next block */
  for (x = 0; x < 7; x++)
    draw_next_block(g, xd + XSIZE * SQUARE_W + 10, yd + x * SQUARE_W * 2, x);
}

void draw_next_block(t_tetris_game g, int x, int y, int i) {
  int xx, yy;
  t_block t;
  t.type = g.next[i];
  t.rotate = 0;
  for (yy = 0; yy < 4; yy++) {
    for (xx = 0; xx < 4; xx++) {
      if (block_xy(t, xx, yy))
        draw_little_square(x + (xx)*SQUARE_W / 2, y + (yy)*SQUARE_W / 2,
                           t.type);
    }
  }
}

void draw_square(int x, int y, Uint8 c) { draw_square_ext(x, y, c, 255); }

void draw_square_ext(int x, int y, Uint8 c, Uint8 a) {
  SDL_Rect r;
  r.x = x;
  r.y = y;
  r.w = SQUARE_W;
  r.h = SQUARE_W;
  SDL_SetRenderDrawColor(renderer, block_color[c][0], block_color[c][1],
                         block_color[c][2], a);
  SDL_RenderFillRect(renderer, &r);
}

void draw_little_square(int x, int y, Uint8 c) {
  SDL_Rect r;
  r.x = x;
  r.y = y;
  r.w = SQUARE_W / 2;
  r.h = SQUARE_W / 2;
  SDL_SetRenderDrawColor(renderer, block_color[c][0], block_color[c][1],
                         block_color[c][2], 255);
  SDL_RenderFillRect(renderer, &r);
}

void draw_rect(int x, int y, int w, int h, Uint8 c) {
  SDL_Rect r;
  r.x = x;
  r.y = y;
  r.w = w;
  r.h = h;
  SDL_SetRenderDrawColor(renderer, block_color[c][0], block_color[c][1],
                         block_color[c][2], 255);
  SDL_RenderDrawRect(renderer, &r);
}

void clear_screen() {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);
}
