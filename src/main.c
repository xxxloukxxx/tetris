// Includes
#include "gui.h"
#include "gui/gui_text.h"
#include "init_video.h"
#include "tetris.h"
// Standard include
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/*
 * Code principal
 */
int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;
  /* Initialisation */
  /* ... Video / SDL2 */
  init_SDL2();
  SDL_Event event;
  /* Des paramètres de jeu */
  t_tetris_game g;
  srand(time(NULL));
  t_tetris_game_init(&g);

  /* boucle principale */
  Uint8 quit = 0;
  while (!quit) {

    /* gestion des événements */
    while (SDL_PollEvent(&event)) {
      /* Si on quit */
      switch (event.type) {
      case SDL_QUIT:
        quit = 1;
        break;
      /* Keyboard event */
      case SDL_KEYDOWN:
        switch (event.key.keysym.scancode) {
        case SDL_SCANCODE_ESCAPE:
          quit = 1;
          break;
        case SDL_SCANCODE_SPACE:
          rotate_block(&g, true);
          break;
        case SDL_SCANCODE_C:
          rotate_block(&g, false);
          break;
        case SDL_SCANCODE_X:
          hold(&g);
          break;
        case SDL_SCANCODE_LEFT:
          move_block(&g, 'l');
          break;
        case SDL_SCANCODE_RIGHT:
          move_block(&g, 'r');
          break;
        case SDL_SCANCODE_DOWN:
          move_block(&g, 'd');
          break;
        case SDL_SCANCODE_UP:
          drop_block(&g);
          check_lines(&g);
          update_current_block(&g);
          break;
        default:
          break;
        }
        break;
      }
      /* Affichage */
      clear_screen();
      draw_game(g);
      SDL_RenderPresent(renderer);
    }
  }

  /* et on s'en va */
  quit_SDL2();
  return EXIT_SUCCESS;
}
