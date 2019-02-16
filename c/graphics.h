#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL.h>
#include "global.h"

#define GRAPHICS_WIDTH  640
#define GRAPHICS_HEIGHT 480

SDL_Window* graphics_init();
void graphics_close(SDL_Window*);

#endif
