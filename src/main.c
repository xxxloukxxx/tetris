#include "gui.h"
#include "init_video.h"
#include "tetris.h"
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  init_SDL2();
  SDL_Event event;

  t_tetris_game g;
  srand(time(NULL));
  t_tetris_game_init(&g);

  Uint8 quit = 0;
  while (!quit) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        quit = 1;
        break;
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
          g.current.type = rand() % 7;
          g.current.x = -1 + (XSIZE / 2);
          g.current.y = 0;
          break;
        default:
          break;
        }
        break;
      }
      clear_screen();
      draw_field(g, g.current);
      SDL_RenderPresent(renderer);
    }
    // drop_block(&g);
    // move_block(&g, 'l');
  }

  quit_SDL2();
  return EXIT_SUCCESS;
}