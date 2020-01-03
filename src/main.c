#include "app.h"
#include "gui/gui_draw.h"
#include "gui/init_video.h"
#include "tetris.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 * GO Et c'est parti
 */
int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;
  /* Initialisation SDL2 */
  init_SDL2();

  /*  Initialisation des paramètres du t_tetris_game */
  srand(time(NULL));
  t_tetris_game g;

  /* Go */
  while (game_status != GS_QUIT) {
    switch (game_status) {
    case GS_MENU:
      game_status = menu();
      break;
    case GS_PLAY:
      t_tetris_game_init(&g);
      game_status = play_tetris(&g);
      break;
    }
  }

  /* et on s'en va */
  quit_SDL2();
  return EXIT_SUCCESS;
}
