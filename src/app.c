#include <SDL2/SDL.h>
#include <stdbool.h>
//
#include "app.h"
#include "gui/gui_draw.h"
#include "gui/gui_text.h"
#include "gui/init_video.h"
#include "tetris.h"
//
int game_status = GS_MENU;

//
int play_tetris(t_tetris_game *ptr_g) {
  bool quit = false;
  int result = GS_PLAY;
  SDL_Event ev;
  while (!quit) {
    while (SDL_PollEvent(&ev)) {
      // Gestion des touches du clavier
      if (ev.type == SDL_KEYDOWN) {
        switch (ev.key.keysym.sym) {
        case SDLK_ESCAPE:
          quit = true;
          result = GS_MENU;
          break;
        }
      }
      if (ev.type == SDL_QUIT) {
        quit = true;
        result = GS_QUIT;
      }
    }
    // affichage
    clear_screen();
    draw_game(*ptr_g);
    SDL_RenderPresent(renderer);
  }
  return result;
}

// GS_MENU ???
int menu() { return GS_PLAY; }
