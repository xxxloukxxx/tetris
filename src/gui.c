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
void draw_field(t_tetris_game g, t_block t) {
  int x, y, c = 0;
  t_block ghost = t;
  int xd = (SCREEN_WIDTH - XSIZE * SQUARE_W) / 2,
      yd = (SCREEN_HEIGHT - YSIZE * SQUARE_W) / 2;
  for (c = 1; c < 5; c++) {
    draw_rect(xd - c, yd - c, (XSIZE * SQUARE_W) + 2 * c,
              (YSIZE * SQUARE_W) + 2 * c, 7);
  }
  for (y = 0; y < YSIZE; y++) {
    for (x = 0; x < XSIZE; x++) {
      c = g.f[x][y];
      if (in(x - t.x, 0, 3) && in(y - t.y, 0, 3) &&
          (block_xy(t, x - t.x, y - t.y)))
        c = t.type;
      if (c != -1)
        draw_square(xd + x * SQUARE_W, yd + y * SQUARE_W, c);
    }
  }
  while (block_valid(g, ghost)) {
    ghost.y++;
  }
  ghost.y--;
  for (y = 0; y < 4; y++) {
    for (x = 0; x < 4; x++) {
      if (block_xy(ghost, x, y))
        draw_square_ext(xd + (x + ghost.x) * SQUARE_W,
                        yd + (y + ghost.y) * SQUARE_W, ghost.type, 100);
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
