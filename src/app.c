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
int          score           = 0;
int          nb_lines_clear  = 0;
int          nb_blocks       = 0;
int          block_stat[7]   = { 0 };
char         block_str[7]    = { 'I', 'J', 'L', 'O', 'S', 'T', 'Z' };
unsigned int game_timer      = 0;
char         menu_str[2][50] = { " Start ", " Quit " };
int          menu_entries    = 2;
int          menu_select     = 0;
//
SDL_Event ev;

//
void play_keyboard_event();
void pause_keyboard_event();
void gameover_keyboard_event();
void draw_title(int *l);

void draw_info();

/*-----------------------------------------------------------------------------*/
void play() {
    // Initialisation
    int i          = 0;
    score          = 0;
    nb_lines_clear = 0;
    nb_blocks      = 0;
    bool quit      = false;
    init_timer(&t1);
    init_timer(&t0);
    game_timer = 0;
    for(i = 0; i < 7; i++) block_stat[i] = 0;
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
    }
}

/*-----------------------------------------------------------------------*/
void draw_info() {
    int  i = 0;
    int  l = 1;
    char buffer[50];
    draw_title(&l);
    sprintf(buffer, "Timer   : %.2i:%.2i.%i ", (game_timer / 100) / 60, (game_timer / 100) % 60, game_timer % 100);
    draw_info_text(buffer, &l);
    sprintf(buffer, "Score   : %i", score);
    draw_info_text(buffer, &l);
    sprintf(buffer, "Lines   : %i", nb_lines_clear);
    draw_info_text(buffer, &l);
    sprintf(buffer, "Blocks  : %i", nb_blocks);
    draw_info_text(buffer, &l);
    for(i = 0; i < 7; i++) {
        sprintf(buffer, "%c Block : %.2f %%", block_str[i],
                (nb_blocks != 0) ? 100 * (double)block_stat[i] / (double)nb_blocks : 0);
        draw_info_text(buffer, &l);
    }

    draw_info_text("------------------------------", &l);
    switch(game_status) {
        case GS_PLAY:
            draw_info_text("Status  : Playing", &l);
            break;
        case GS_PAUSE:
        case GS_GAMEOVER:
            sprintf(buffer, "Status  : %s",
                    (SDL_GetTicks() % 300 > 150) ? ((game_status == GS_PAUSE) ? "Pause" : "Game Over") : "");
            draw_info_text(buffer, &l);
            draw_info_text("------------------------------", &l);
            draw_info_text("[M]enu - [R]estart - [Q]uit", &l);
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
                    block_stat[g.current.type]++;
                    printf("%i\n", block_stat[g.current.type]);
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
void menu() {
    // Initialisation
    int  i    = 0;
    int  l    = 0;
    bool quit = false;
    while(!quit) {
        // Affichage
        l = 0;
        clear_screen();
        draw_grid(50);
        draw_field(g);
        draw_title(&l);
        for(i = 0; i < menu_entries; i++) {
            if(i == menu_select)
                draw_info_text((SDL_GetTicks() % 300 > 150) ? menu_str[i] : "", &l);
            else
                draw_info_text(menu_str[i], &l);
        }
        draw_info_text("------------------------------", &l);
        draw_info_text("Left/Right : Move", &l);
        draw_info_text("X/C        : Rotate", &l);
        draw_info_text("Down       : Soft Drop", &l);
        draw_info_text("Up         : Hard Drop", &l);
        draw_info_text("Space      : Hold", &l);
        draw_info_text("ESC        : Pause", &l);
        draw_info_text("------------------------------", &l);
        SDL_RenderPresent(renderer);

        // SDL_Event
        while(SDL_PollEvent(&ev)) {
            if(ev.type == SDL_KEYDOWN) {
                switch(ev.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        game_status = GS_QUIT;
                        break;
                    case SDLK_UP:
                        menu_select = (menu_select == 0) ? 0 : menu_select - 1;
                        break;
                    case SDLK_DOWN:
                        menu_select = (menu_select == menu_entries) ? menu_entries : menu_select + 1;
                        break;
                    case SDLK_RETURN:
                        switch(menu_select) {
                            case 0:
                                game_status = GS_PLAY;
                                break;
                            case 1:
                                game_status = GS_QUIT;
                                break;
                        }
                        quit = true;
                        break;
                }
            }
            if(ev.type == SDL_QUIT) {
                game_status = GS_QUIT;
                quit        = true;
            }
        }
    }
}

/*-----------------------------------------------------------------------*/
void draw_title(int *l) {
    draw_info_text("------------------------------", l);
    draw_info_text("            TETRIS            ", l);
    draw_info_text("------------------------------", l);
}
