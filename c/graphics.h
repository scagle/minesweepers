#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL.h>
#include <SDL_ttf.h>
#include "global.h"

#define GRAPHICS_WIDTH  640
#define GRAPHICS_HEIGHT 480

int         graphics_init  (                                                );
void        graphics_close (                                                );
void        render         (Board board, int cursor_x, int cursor_y, int end);

typedef struct
{
    char         text[128] ;
    TTF_Font*    font      ;
    SDL_Color    color     ;
    SDL_Surface* surface   ;
    SDL_Texture* message   ;
} TextObject;


#endif
