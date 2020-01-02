#include "gui.h"
#include "gui/gui_draw.h"
#include "init_video.h"
#include "tetris.h"
#include <SDL2/SDL.h>

/* Dessine le jeu */
void draw_game(t_tetris_game g) {
  draw_grid(50);
  draw_field(g);
  draw_block(g.current, 255);
  draw_ghost_block(g, 100);
  draw_next_blocks(g);
}

/* Efface l'écran */
void clear_screen() {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);
}
