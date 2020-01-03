#include "gui_text.h"
#include "..\init_video.h"
#include "..\tetris.h"
#include "font/font8x8.h"
#include <SDL2/SDL.h>

void draw_text(char *str, int x, int y) {
  draw_text_ext(str, x, y, font8x8_basic);
}

void draw_text_ext(char *str, int x, int y, char t[][8]) {
  int idx_point = 0, len = 0, i = 0, xx, yy;
  bool set;
  while (str[len] != '\0')
    len++;
  SDL_Point points[len * 64];
  while (str[i] != '\0') {
    for (xx = 0; xx < 8; xx++) {
      for (yy = 0; yy < 8; yy++) {
        set = t[(unsigned int)str[i]][yy] & 1 << xx;
        if (set) {
          points[idx_point].x = 8 * i + x + xx;
          points[idx_point].y = y + yy;
          idx_point++;
        }
      }
    }
    i++;
  }
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderDrawPoints(renderer, points, idx_point);
}
