#include "gui_text.h"
#include "..\init_video.h"
#include "..\tetris.h"
#include "font/font8x8.h"
#include <SDL2/SDL.h>

void draw_text(char *str, int x, int y) {
  int i = 0, xx, yy;
  bool set;
  // unsigned int c = (unsigned int)str[i];
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  while (str[i] != '\0') {
    for (xx = 0; xx < 8; xx++) {
      for (yy = 0; yy < 8; yy++) {
        // c = (unsigned int)str[i];
        set = font8x8_basic[(unsigned int)str[i]][yy] & 1 << xx;
        if (set)
          SDL_RenderDrawPoint(renderer, 8 * i + x + xx, y + yy);
      }
    }
    i++;
  }
}
