#include <stdio.h>
#include "graphics.h"
#include "global.h"

#define BORDER_V 50 // Top/Bottom border in pixels
#define BORDER_H 50 // Left/Right border in pixels

unsigned int GRID_WIDTH  = (SCREEN_WIDTH  - BORDER_H*2) / GAME_SIZE;  // Width of each tile
unsigned int GRID_HEIGHT = (SCREEN_HEIGHT - BORDER_V*2) / GAME_SIZE;  // Height of each tile

SDL_Window   *window          = NULL     ; // Window that contains everything
SDL_Renderer *renderer        = NULL     ; // Renderer that updates surfaces/shows them
SDL_Texture  *texture         = NULL     ; // Hardware acclerated drawing on surfaces 

TextObject    header                     ; // Header containing title/score/face
TextObject    footer                     ; // Footer containing instructions
TextObject    gameover                   ; // Game over text
TextObject    won                        ; // Game over text
TextObject    tile_objects[10]           ; // Tile text objects
SDL_Rect      grid[GAME_SIZE][GAME_SIZE] ; // Graphical representation of board

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
    // Initializing colors
    const SDL_Color BLACK      = {0   , 0   , 0  };
    const SDL_Color RED        = {255 , 0   , 0  };
    const SDL_Color GREEN      = {0   , 255 , 0  };
    const SDL_Color BLUE       = {0   , 0   , 255};
    const SDL_Color PURPLE     = {128 , 0   , 128};
    const SDL_Color MAROON     = {128 , 0   , 0  };
    const SDL_Color TURQUOISE  = { 64 , 224 , 208};
    const SDL_Color GRAY       = {128 , 128 , 128};
    SDL_Color HEADER_COLOR  = BLACK; 
    SDL_Color FOOTER_COLOR  = BLACK; 
    SDL_Color TILE_COLORS[]  = {BLACK, BLUE, GREEN, RED, PURPLE, MAROON, TURQUOISE, BLACK, GRAY, BLACK};


    // Initialize SDL Components 
    screenSurface = SDL_GetWindowSurface(window); //Get window surface
    if (screenSurface == NULL)
    {
        printf("Screen Surface couldn't be created!\nSDL_ERROR: %s\n", SDL_GetError());
        return 1;
    }
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
    TTF_Font* header_font   = TTF_OpenFont("../fonts/OpenSans/OpenSans-Bold.ttf", 50); //this opens a font style and sets a size
    TTF_Font* footer_font   = TTF_OpenFont("../fonts/OpenSans/OpenSans-Bold.ttf", 24); //this opens a font style and sets a size
    TTF_Font*   tile_font   = TTF_OpenFont("../fonts/OpenSans/OpenSans-Bold.ttf", 50); //this opens a font style and sets a size
    TTF_Font* gameover_font = TTF_OpenFont("../fonts/OpenSans/OpenSans-Bold.ttf", 50); //this opens a font style and sets a size

    if (header_font == NULL || footer_font == NULL || tile_font == NULL || gameover_font == NULL)
    {
        printf("TTF_Font Error : %s\n", TTF_GetError());
        printf("***NOTE: program assumes present working directory is '<project_root>/c/build/')\n");
        return 1;
    }

    // Initialize header text object
    snprintf(header.text, sizeof(header.text), "%s", "Minesweeper");
    header.font    = header_font;
    header.color   = HEADER_COLOR; // Defined in graphics.h
    header.surface = TTF_RenderText_Solid(header.font, header.text, header.color);
    header.message = SDL_CreateTextureFromSurface(renderer, header.surface);

    // Initialize footer text object
    snprintf(footer.text, sizeof(footer.text), "%s", "'w', 'a', 's', 'd' = move cursor | 'p' = reveal tile | 'o' = place flag | 'r' = restart game");
    footer.font    = footer_font;
    footer.color   = FOOTER_COLOR; // Defined in graphics.h
    footer.surface = TTF_RenderText_Solid(footer.font, footer.text, footer.color);
    footer.message = SDL_CreateTextureFromSurface(renderer, footer.surface);

    snprintf(gameover.text, sizeof(gameover.text), "%s", "GAME OVER!");
    gameover.font    = header_font;
    gameover.color   = RED; // Defined in graphics.h
    gameover.surface = TTF_RenderText_Solid(gameover.font, gameover.text, gameover.color);
    gameover.message = SDL_CreateTextureFromSurface(renderer, gameover.surface);

    snprintf(won.text, sizeof(won.text), "%s", "YOU WON!");
    won.font    = header_font;
    won.color   = GREEN; // Defined in graphics.h
    won.surface = TTF_RenderText_Solid(won.font, won.text, won.color);
    won.message = SDL_CreateTextureFromSurface(renderer, won.surface);

    for (int i = 0; i < 10; i++)
    {
        // tile #0 is included just in case a debugging situation arises (normally it's invisible/blank)
        // tile #9 is a mine so "M"
        char str[2];  // length is 2, because <one-digit-number> + \0 zero terminator
        snprintf(str, sizeof(str), "%d", i);
        if (i != 9)
            snprintf(str, sizeof(str), "%d", i);
        else
            snprintf(str, sizeof(str), "%s", "M");

        snprintf(tile_objects[i].text, sizeof(tile_objects[i].text), "%s", str);
        tile_objects[i].font    = tile_font; 
        tile_objects[i].color   = TILE_COLORS[i];
        tile_objects[i].surface = TTF_RenderText_Solid(tile_font, str, TILE_COLORS[i]);
        tile_objects[i].message = SDL_CreateTextureFromSurface(renderer, tile_objects[i].surface);
    }

    return 0;
}

void render(Board board, int cursor_x, int cursor_y, int end)
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
                    TextObject to = tile_objects[value];
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
    SDL_Rect header_box = {BORDER_H+SCREEN_WIDTH/3, 0, (SCREEN_WIDTH  - BORDER_H*2)/4, BORDER_V};
    SDL_RenderCopy(renderer, header.message, NULL, &header_box); 

    SDL_Rect footer_box = {BORDER_H, (SCREEN_HEIGHT - BORDER_V), (SCREEN_WIDTH  - BORDER_H*2), BORDER_V};
    SDL_RenderCopy(renderer, footer.message, NULL, &footer_box); 

    if (end == 1) // if bad end
    {
        SDL_Rect gameover_box = {SCREEN_WIDTH/4, SCREEN_HEIGHT/3, SCREEN_WIDTH/2, SCREEN_HEIGHT/3};
        SDL_RenderCopy(renderer, gameover.message, NULL, &gameover_box); 
    }
    else if (end == 2) // if good end
    {
        SDL_Rect won_box = {SCREEN_WIDTH/4, SCREEN_HEIGHT/3, SCREEN_WIDTH/2, SCREEN_HEIGHT/3};
        SDL_RenderCopy(renderer, won.message, NULL, &won_box); 
    }
    // Update screen
    SDL_RenderPresent(renderer);
}

void graphics_close(SDL_Window *window)
{
    TTF_CloseFont(header.font);
    TTF_CloseFont(footer.font);
    TTF_CloseFont(tile_objects[0].font);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

