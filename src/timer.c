#include <SDL2/SDL.h>
#include <stdbool.h>

int t0 = 0;

void init_timer(int *t) { (*t) = SDL_GetTicks(); }
bool pulse(int *t, int period) {
  int t1 = SDL_GetTicks();
  int gap = t1 - (*t);
  if (gap > period) {
    while (gap > period)
      gap -= period;
    (*t) = t1 - gap;
    return true;
  }
  return false;
}
