#include <stdio.h>
#include "graphics.h"
#include "global.h"

#define BORDER_V 50 // Top/Bottom border in pixels
#define BORDER_H 50 // Left/Right border in pixels

unsigned int GRID_WIDTH  = (SCREEN_WIDTH  - BORDER_H*2) / GAME_SIZE;  // Width of each tile
unsigned int GRID_HEIGHT = (SCREEN_HEIGHT - BORDER_V*2) / GAME_SIZE;  // Height of each tile

SDL_Window   *window          = NULL     ; // Window that contains everything
SDL_Renderer *renderer        = NULL     ;
SDL_Texture  *texture         = NULL     ; // Hardware acclerated drawing
SDL_Rect      grid[GAME_SIZE][GAME_SIZE] ; // Visual representation of board

TTF_Font*     font            = NULL     ; // Font initialized in graphics_init()
TTF_Font*     directions_font = NULL     ; // Font initialized in graphics_init()
SDL_Color     dir_color                  ; // Directions color
SDL_Surface  *dir_surface     = NULL     ; // Directions surface object
SDL_Texture  *dir_message     = NULL     ; // Directions texture object

TileObject tile_objects[10];

int graphics_init()
{    
    // Initialize SDL
    window = NULL;
    SDL_Surface* screenSurface = NULL;
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

    // Initialize SDL Components 
    screenSurface = SDL_GetWindowSurface(window); //Get window surface
    SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF)); //Fill the surface white
    SDL_UpdateWindowSurface(window); //Update the surface
    for (int x = 0; x < GAME_SIZE; x++) 
    {
        for (int y = 0; y < GAME_SIZE; y++) 
        {
            grid[y][x] = (SDL_Rect){BORDER_V + (x*GRID_WIDTH), BORDER_H + (y*GRID_HEIGHT), GRID_WIDTH , GRID_HEIGHT};
        }
    }

    //Initialize SDL_ttf
    if (TTF_Init() == -1)
    {
        printf("SDL_ttf could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    // Initialize SDL_ttf Components
    TTF_Font* font = TTF_OpenFont("./fonts/OpenSans/OpenSans-Bold.ttf", 24); //this opens a font style and sets a size
    TTF_Font* dir_font = TTF_OpenFont("./fonts/OpenSans/OpenSans-Bold.ttf", 50); //this opens a font style and sets a size
    if (font == NULL)
        printf("TTF_Font Error : %s\n", TTF_GetError());

    SDL_Color font_color = {0, 0, 0};

    for (char i = 0; i < 10; i++)
    {
        char str[2];
        sprintf(str, "%d", i);
        SDL_Surface* surface = TTF_RenderText_Solid(font, str, font_color);

        tile_objects[i].text            = i; 
        tile_objects[i].tile_font       = font; 
        tile_objects[i].tile_font_color = font_color;
        tile_objects[i].font_surface    = TTF_RenderText_Solid(font, str, font_color);
        tile_objects[i].message         = SDL_CreateTextureFromSurface(renderer, surface);
    }
    char directions[] = "'w', 'a', 's', 'd' = move cursor | 'p' = reveal tile | 'o' = place flag | 'r' = restart game";
    dir_color = font_color;
    dir_surface    = TTF_RenderText_Solid(dir_font, directions, font_color);
    dir_message    = SDL_CreateTextureFromSurface(renderer, dir_surface);

    return 0;
}

void render(Board board, int cursor_x, int cursor_y)
{
    // Clear Screen 
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);
    
    for (int x = 0; x < GAME_SIZE; x++)
    {
        for (int y = 0; y < GAME_SIZE; y++)
        {
            if (board.overlays[y][x] == FOG) // Draw Fog
            {
                if (x == cursor_x && y == cursor_y)
                    SDL_SetRenderDrawColor(renderer, 0xFF, 0x7F, 0x7F, 0xFF); // Darker Red
                else
                    SDL_SetRenderDrawColor(renderer, 0xFF, 0xC0, 0xCB, 0xFF); // Pink
                SDL_RenderFillRect(renderer, &grid[y][x]);
            }
            else if (board.overlays[y][x] == FLAG) // Draw Fog
            {
                if (x == cursor_x && y == cursor_y)
                    SDL_SetRenderDrawColor(renderer, 0x00, 0xAA, 0x00, 0xFF); // Darker Green 
                else
                    SDL_SetRenderDrawColor(renderer, 0x70, 0xFF, 0x70, 0xFF); // Green 
                SDL_RenderFillRect(renderer, &grid[y][x]);
            }
            else                             // Draw Clear or Number
            {
                if (x == cursor_x && y == cursor_y)
                    SDL_SetRenderDrawColor(renderer, 0xBF, 0xBF, 0xBF, 0xFF); // Gray
                else if (board.tiles[y][x] == MINE)
                    SDL_SetRenderDrawColor(renderer, 0xFF, 0xA5, 0x00, 0xFF); // Orange
                else
                    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF); // White
                SDL_RenderFillRect(renderer, &grid[y][x]);
                char value = board.tiles[y][x];
                if (value != NONE)
                {
                    TileObject to = tile_objects[value];
                    // Adjust font dimensions (shrink horizontally a bit)
                    SDL_Rect box = grid[y][x];
                    SDL_Rect font_box = {box.x+box.w/4, box.y, box.w/2, box.h};
                    // Render Font
                    SDL_RenderCopy(renderer, to.message, NULL, &font_box); 
                }
            }
            
            // Draw Border
            SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF); // Red
            SDL_RenderDrawRect(renderer, &grid[y][x]);

        }
    }
    SDL_Rect dir_box = {BORDER_H, (SCREEN_HEIGHT - BORDER_V), (SCREEN_WIDTH  - BORDER_H*2), BORDER_V};
    SDL_RenderCopy(renderer, dir_message, NULL, &dir_box); 

    // Update screen
    SDL_RenderPresent(renderer);
}

void graphics_close(SDL_Window *window)
{
    TTF_CloseFont(font);
    TTF_CloseFont(directions_font);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

