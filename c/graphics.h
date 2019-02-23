#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL.h>
#include <SDL_ttf.h>
#include "global.h"

#define GRAPHICS_WIDTH  640
#define GRAPHICS_HEIGHT 480

int         graphics_init  (                                        );
void        graphics_close (                                        );
void        render         (Board board, int cursor_x, int cursor_y );

typedef struct
{
    char         text            ;
    TTF_Font*    tile_font       ;
    SDL_Color    tile_font_color ;
    SDL_Surface* font_surface    ;
    SDL_Texture* message         ;
} TileObject;


#endif
