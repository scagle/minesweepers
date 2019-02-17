#include <stdio.h>
#include "graphics.h"
#include "global.h"

#define BORDER_V    50 // Top/Bottom border in pixels
#define BORDER_H    50 // Left/Right border in pixels
#define GRID_WIDTH  (SCREEN_WIDTH - BORDER_H*2)  / GAME_SIZE 
#define GRID_HEIGHT (SCREEN_HEIGHT - BORDER_V*2) / GAME_SIZE

SDL_Window   *window   = NULL;            // Window that contains everything
SDL_Renderer *renderer = NULL;              
SDL_Texture  *texture  = NULL;            // Hardware acclerated drawing
SDL_Rect      grid[GAME_SIZE][GAME_SIZE]; // Visual representation of board

void graphics_init()
{    
    window = NULL;
    SDL_Surface* screenSurface = NULL;
    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }
    else
    {
        window = SDL_CreateWindow("C-Minesweeper", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL)
        {
            printf("Window couldn't be created!\nSDL_ERROR: %s\n", SDL_GetError());
        }
        else
        {
            //Get window surface
            screenSurface = SDL_GetWindowSurface(window);
        
            //Fill the surface white
            SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
 
            //Update the surface
            SDL_UpdateWindowSurface(window);
        }
    }
    for (int x = 0; x < GAME_SIZE; x++) 
    {
        for (int y = 0; y < GAME_SIZE; y++) 
        {
            grid[x][y] = (SDL_Rect){BORDER_V + (x*GRID_WIDTH), BORDER_H + (y*GRID_HEIGHT), GRID_WIDTH , GRID_HEIGHT};
        }
    }
}

void render(Board board, int cursor_x, int cursor_y)
{

}

void graphics_close(SDL_Window *window)
{
    SDL_DestroyWindow(window);
    SDL_Quit();
}
