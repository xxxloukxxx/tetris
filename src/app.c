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
// int game_status    = GS_MENU;
int          score          = 0;
int          nb_lines_clear = 0;
int          nb_blocks      = 0;
unsigned int game_timer     = 0;

//
SDL_Event ev;

//
void play_keyboard_event();
void pause_keyboard_event();
void gameover_keyboard_event();

void draw_info();

/*-----------------------------------------------------------------------------*/
int play() {
    // Initialisation
    score          = 0;
    nb_lines_clear = 0;
    nb_blocks      = 0;
    bool quit      = false;
    init_timer(&t1);
    init_timer(&t0);
    game_timer = 0;
    /*------------------------------------*/
    while(!quit) {            // boucle principale
        switch(game_status) { // Gestion Keyboard
            case GS_PLAY:
                if(pulse(&t0, 750)) {                    // Toutes les ... ms, faire descendre le block
                    if(!move_block(&g, 'd')) {           // et si c'est pas possible
                        ev.type           = SDL_KEYDOWN; // Forcer un hard_drop
                        ev.key.keysym.sym = SDLK_UP;     //
                        SDL_PushEvent(&ev);              //
                    }
                }
                if(pulse(&t1, 10)) game_timer++;
                // Regardons si on a perdu
                if(is_gameover(&g)) game_status = GS_GAMEOVER;
                play_keyboard_event();
                break;
            case GS_PAUSE:
                pause_keyboard_event();
                break;
            case GS_GAMEOVER:
                gameover_keyboard_event();
                break;
        }

        // Affichage
        clear_screen();
        draw_game(g);
        draw_info();
        SDL_RenderPresent(renderer);

        // Est-ce que on quit ?
        if(game_status == GS_QUIT) quit = true;
        if(game_status == GS_PLAY_RESTART) quit = true;
        if(game_status == GS_MENU) quit = true;
        ///
    }
    return game_status;
}

/*-----------------------------------------------------------------------*/
void draw_info() {
    int  l = 1;
    char buffer[50];
    draw_info_text("------------------------------", &l);
    draw_info_text("            TETRIS            ", &l);
    draw_info_text("------------------------------", &l);
    sprintf(buffer, "Timer   : %.2i:%.2i.%i ", (game_timer / 100) / 60, (game_timer / 100) % 60, game_timer % 100);
    draw_info_text(buffer, &l);
    sprintf(buffer, "Score   : %i", score);
    draw_info_text(buffer, &l);
    sprintf(buffer, "Lines   : %i", nb_lines_clear);
    draw_info_text(buffer, &l);
    sprintf(buffer, "Blocks  : %i", nb_blocks);
    draw_info_text(buffer, &l);
    draw_info_text("------------------------------", &l);
    switch(game_status) {
        case GS_PLAY:
            draw_info_text("Status  : Playing", &l);
            break;
        case GS_GAMEOVER:
            draw_info_text("Status  : Game Over", &l);
            draw_info_text("[M] for Menu", &l);
            draw_info_text("[R] for Restart", &l);
            draw_info_text("[Q] for Quit", &l);
            break;
        case GS_PAUSE:
            draw_info_text("Status  : Pause", &l);
            draw_info_text("[M] for Menu", &l);
            draw_info_text("[R] for Restart", &l);
            draw_info_text("[Q] for Quit", &l);
            break;
    }
    draw_info_text("------------------------------", &l);
}

/*-----------------------------------------------------------------------*/
void gameover_keyboard_event() {
    while(SDL_PollEvent(&ev)) {
        if(ev.type == SDL_KEYDOWN) {
            switch(ev.key.keysym.sym) {
                case SDLK_r:
                    game_status = GS_PLAY_RESTART;
                    break;
                case SDLK_m:
                    game_status = GS_MENU;
                    break;
                case SDLK_q:
                    game_status = GS_QUIT;
                    break;
            }
        }
        if(ev.type == SDL_QUIT) game_status = GS_QUIT;
    }
}

/*-----------------------------------------------------------------------*/
void pause_keyboard_event() {
    while(SDL_PollEvent(&ev)) {
        if(ev.type == SDL_KEYDOWN) {
            switch(ev.key.keysym.sym) {
                case SDLK_ESCAPE:
                    init_timer(&t0);
                    game_status = GS_PLAY;
                    break;
                case SDLK_r:
                    game_status = GS_PLAY_RESTART;
                    break;
                case SDLK_m:
                    game_status = GS_MENU;
                    break;
                case SDLK_q:
                    game_status = GS_QUIT;
                    break;
            }
        }
        if(ev.type == SDL_QUIT) game_status = GS_QUIT;
    }
}

/*-----------------------------------------------------------------------*/
void play_keyboard_event() {
    int lines_clear; // on check_lines
    while(SDL_PollEvent(&ev)) {
        if(ev.type == SDL_KEYDOWN) {
            switch(ev.key.keysym.sym) {
                case SDLK_ESCAPE:
                    game_status = GS_PAUSE;
                    break;
                case SDLK_UP:
                    drop_block(&g);
                    lines_clear    = check_lines(&g);
                    score          = update_score(score, lines_clear);
                    nb_lines_clear = nb_lines_clear + lines_clear;
                    nb_blocks++;
                    update_current_block(&g);
                    break;
                case SDLK_DOWN:
                    init_timer(&t0);
                    move_block(&g, 'd');
                    break;
                case SDLK_LEFT:
                    move_block(&g, 'l');
                    break;
                case SDLK_RIGHT:
                    move_block(&g, 'r');
                    break;
                case SDLK_c:
                    rotate_block(&g, true);
                    break;
                case SDLK_x:
                    rotate_block(&g, false);
                    break;
                case SDLK_SPACE:
                    init_timer(&t0);
                    hold(&g);
                    break;
            }
        }
        if(ev.type == SDL_QUIT) game_status = GS_QUIT;
    }
}

/*-----------------------------------------------------------------------*/
int menu() {
    return GS_PLAY;
}
