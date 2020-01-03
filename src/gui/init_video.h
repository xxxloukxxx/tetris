#ifndef _INIT_VIDEO_H_
#define _INIT_VIDEO_H_
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define WINDOWS_TITLE "Prout"
#define WINDOWS_FLAGS SDL_WINDOW_SHOWN /* || SDL_WINDOW_FULLSCREEN */

SDL_Window *window;
SDL_Renderer *renderer;

bool init_SDL2();
int quit_SDL2();

#endif
