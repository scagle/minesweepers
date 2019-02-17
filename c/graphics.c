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

int graphics_init()
{    
    window = NULL;
    SDL_Surface* screenSurface = NULL;
    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    else
    {
        window = SDL_CreateWindow("C-Minesweeper", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL)
        {
            printf("Window couldn't be created!\nSDL_ERROR: %s\n", SDL_GetError());
            return 1;
        }
        else
        {
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if (renderer == NULL)
            {
                printf("Renderer couldn't be created!\nSDL_ERROR: %s\n", SDL_GetError());
                return 1;
            }
        }
    }
    //Get window surface
    screenSurface = SDL_GetWindowSurface(window);

    //Fill the surface white
    SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));

    //Update the surface
    SDL_UpdateWindowSurface(window);
    for (int x = 0; x < GAME_SIZE; x++) 
    {
        for (int y = 0; y < GAME_SIZE; y++) 
        {
            grid[x][y] = (SDL_Rect){BORDER_V + (x*GRID_WIDTH), BORDER_H + (y*GRID_HEIGHT), GRID_WIDTH , GRID_HEIGHT};
        }
    }
    return 0;
}

void render(Board board, int cursor_x, int cursor_y)
{
    // Clear Screen 
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);

    // Render red filled quad
    SDL_Rect fillRect = {SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);        
    SDL_RenderFillRect(renderer, &fillRect);

    // Render green outlined quad
    SDL_Rect outlineRect = {SCREEN_WIDTH / 6, SCREEN_HEIGHT / 6, SCREEN_WIDTH * 2 / 3, SCREEN_HEIGHT * 2 / 3};
    SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);        
    SDL_RenderDrawRect( renderer, &outlineRect );

    // Draw blue horizontal line
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);        
    SDL_RenderDrawLine(renderer, 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2);

    // Draw vertical line of yellow dots
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0x00, 0xFF);
    for (int i = 0; i < SCREEN_HEIGHT; i += 4)
    {
        SDL_RenderDrawPoint(renderer, SCREEN_WIDTH / 2, i);
    }

    // Update screen
    SDL_RenderPresent(renderer);
}

void graphics_close(SDL_Window *window)
{
    SDL_DestroyWindow(window);
    SDL_Quit();
}

