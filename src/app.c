#include <SDL2/SDL.h>
#include <stdbool.h>
//
#include "app.h"
#include "gui/gui_draw.h"
#include "gui/gui_text.h"
#include "gui/init_video.h"
#include "tetris.h"
#include "timer.h"
//
int game_status = GS_MENU;
int score = 0;
int nb_lines_clear = 0;

//
#define GAME_PLAY 0
#define GAME_PAUSE 1
#define GAME_GAMEOVER 2

int play_tetris(t_tetris_game *ptr_g) {
  int return_state = GS_PLAY;
  int lines = 0;
  bool fix_block = false;
  int state = GAME_PLAY;
  SDL_Event ev;
  //
  init_timer(&t0);
  score = 0;
  nb_lines_clear = 0;
  //
  while (return_state == GS_PLAY) {
    ///*** START GESTION EVENT ***///
    while (SDL_PollEvent(&ev)) {
      // Gestion des touches du clavier
      if (ev.type == SDL_KEYDOWN) {
        switch (state) {
        case GAME_PLAY:
          switch (ev.key.keysym.sym) {
          case SDLK_ESCAPE:
            state = GAME_PAUSE;
            break;
          case SDLK_UP:
            fix_block = true;
            break;
          case SDLK_DOWN:
            init_timer(&t0);
            move_block(ptr_g, 'd');
            break;
          case SDLK_LEFT:
            move_block(ptr_g, 'l');
            break;
          case SDLK_RIGHT:
            move_block(ptr_g, 'r');
            break;
          case SDLK_c:
            rotate_block(ptr_g, true);
            break;
          case SDLK_x:
            rotate_block(ptr_g, false);
            break;
          case SDLK_SPACE:
            hold(ptr_g);
            break;
          }
          break;
        case GAME_PAUSE:
          switch (ev.key.keysym.sym) {
          case SDLK_ESCAPE:
            init_timer(&t0);
            state = GAME_PLAY;
            break;
          case SDLK_r:
            return GS_PLAY;
            break;
          case SDLK_m:
            return_state = GS_MENU;
            break;
          case SDLK_q:
            return_state = GS_QUIT;
            break;
          }
          break;
        case GAME_GAMEOVER:
          switch (ev.key.keysym.sym) {
          case SDLK_r:
            return GS_PLAY;
            break;
          case SDLK_m:
            return_state = GS_MENU;
            break;
          case SDLK_q:
            return_state = GS_QUIT;
            break;
          }
          break;
        }
      }

      if (ev.type == SDL_QUIT) {
        return_state = GS_QUIT;
      }
    }
    ///*** END GESTION EVENT ***///

    if (state == GAME_PLAY) {
      // Toute les ...ms on fait descendre le block si possible  !
      if (pulse(&t0, 750)) {
        fix_block = !move_block(ptr_g, 'd');
      }
      // si le block est drop en bas
      if (fix_block) {
        drop_block(ptr_g);
        lines = check_lines(ptr_g);
        score = update_score(score, lines);
        nb_lines_clear += lines;
        update_current_block(ptr_g);
        //  gameover ?
        if (!block_valid(*ptr_g, ptr_g->current)) {
          state = GAME_GAMEOVER;
        }
        init_timer(&t0);
        fix_block = false;
      }
    }

    ////////////////////////////////////////////////
    // affichage
    ///////////////////////////////////////////////
    clear_screen();
    draw_game(*ptr_g);
    draw_score(score, nb_lines_clear);
    // Affichage message game over
    switch (state) {
    case GAME_GAMEOVER:
      draw_text("-- Game Over --", 16, 4 * 16);
      draw_text("[M] for Menu", 16, 5 * 16);
      draw_text("[R] for Restart", 16, 6 * 16);
      draw_text("[Q] for Quit", 16, 7 * 16);
      break;
    case GAME_PAUSE:
      if ((SDL_GetTicks() % 300) > 150)
        draw_text("-- Pause --", 16, 4 * 16);
      draw_text("[M] for Menu", 16, 5 * 16);
      draw_text("[R] for Restart", 16, 6 * 16);
      draw_text("[Q] for Quit", 16, 7 * 16);
      break;
    }

    SDL_RenderPresent(renderer);
  }

  // return Game status
  return return_state;
}

// GS_MENU ???
int menu() { return GS_PLAY; }
