#include <SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "logic.h"

#define GAME_WIDTH    40
#define GAME_HEIGHT   40
#define SCREEN_WIDTH  600
#define SCREEN_HEIGHT 480

void         board_init    (                     );
bool         graphics_init (                     );
bool         main_loop     (                     );
void         draw_board    ( SDL_Surface *screen );
//[[bool         loadMedia     (                     );
//[[SDL_Surface* loadSurface   ( char *path          );
void         close         (                     );

SDL_Window*  g_window          = NULL;
SDL_Surface* screen            = NULL;    // Visual Board
SDL_Surface* canvas       = NULL;    // Next Visual Board (for blit)
SDL_Rect     board[GAME_WIDTH][GAME_HEIGHT];

//*****************************************************************************
//*                                 Main                                      *
//*****************************************************************************

int main(int argc, char* args[])
{
    // initializations
    board_init();
    graphics_init();

    // start game
    main_loop();

    return 0;
}

bool main_loop()
{
    int cursor_x = 5;
    int cursor_y = 5;
    if( !graphics_init() )
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {
//[[        if(!loadMedia())           //Load media
//[[        {
//[[            printf( "Failed to load media!\n" );
//[[        }
//[[        else
//[[        {   
            bool quit = false;     // Main loop flag
            SDL_Event e;           // Event handler

            while(!quit)                           // While application is running
            {
                while(SDL_PollEvent(&e) != 0)      // Handle events on queue
                {
                    if(e.type == SDL_QUIT)         // User requests quit
                    {
                        printf("Goodbye!");
                        quit = true;
                    }
                    else if(e.type == SDL_KEYDOWN) // User presses a key
                    {
                        switch(e.key.keysym.sym)   // Select surfaces based on key press
                        {
                            case SDLK_w: handle_input(UP    , cursor_x , cursor_y);
                                         break;
                            case SDLK_a: handle_input(LEFT  , cursor_x , cursor_y); 
                                         break;
                            case SDLK_s: handle_input(DOWN  , cursor_x , cursor_y); 
                                         break;
                            case SDLK_d: handle_input(RIGHT , cursor_x , cursor_y); 
                                         break;
                            case SDLK_q: quit = true;
                                         break;
                            default:     break;
                        }
                    }
                }

                draw_board(canvas);           // Draw the board

                //Apply the current image
                SDL_BlitSurface(screen, NULL, canvas, NULL);
            
                //Update the surface
                SDL_UpdateWindowSurface( g_window );
            }
//[[        }
    }

    //Free resources and close SDL
    //print_game();
    close();

    return 0;
}

//*****************************************************************************
//*                              Functions                                    *
//*****************************************************************************
void draw_board(SDL_Surface *surf)
{
    const SDL_Rect rect = {0, 0, 50, 50};

    SDL_FillRect(surf, &rect, SDL_MapRGB(surf->format, 255, 0, 0));
}

//[[ bool loadMedia()
//[[ {
//[[    //Loading success flag
//[[     //TODO: Incorporate pictures
//[[    bool success = true;
//[[    g_current_surface = loadSurface( "../images/smile.bmp" );
//[[    return success;
//[[ }
//[[ 
//[[ SDL_Surface* loadSurface( char *path )
//[[ {
//[[    //Load image at specified path
//[[    SDL_Surface* loaded_surface = SDL_LoadBMP( path );
//[[    if( loaded_surface == NULL )
//[[    {
//[[        printf( "Unable to load image %s! SDL Error: %s\n", path, SDL_GetError() );
//[[    }
//[[ 
//[[    return loaded_surface;
//[[ }

void close()
{
    //Deallocate surfaces
//[[    SDL_FreeSurface(g_current_surface);
//[[    SDL_FreeSurface(g_screen_surface);
//[[    g_current_surface = NULL;
//[[    g_screen_surface = NULL;
    SDL_FreeSurface(screen);
    SDL_FreeSurface(canvas);
    screen      = NULL;
    canvas = NULL;

    //Destroy window
    SDL_DestroyWindow(g_window);
    g_window = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}

//*****************************************************************************
//*                             Initializations                               *
//*****************************************************************************
void board_init()
{
    int x, y;
    const int TILE_WIDTH  = SCREEN_WIDTH  / GAME_WIDTH ;
    const int TILE_HEIGHT = SCREEN_HEIGHT / GAME_HEIGHT;
    for (x = 0; x < GAME_WIDTH; x++)
    {
        for (y = 0; y < GAME_HEIGHT; y++)
        {
            board[x][y].x = x * TILE_WIDTH  ;
            board[x][y].y = y * TILE_HEIGHT ;
            board[x][y].w =     TILE_WIDTH  ;
            board[x][y].h =     TILE_HEIGHT ;
        }
    }
    randomize();
}

bool graphics_init()
{
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Create window
        g_window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( g_window == NULL )
        {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Get window surface
            screen = SDL_GetWindowSurface( g_window );
            canvas = SDL_CreateRGBSurface(0, 600, 480, 32, 0, 0, 0, 0);
        }
    }

    return success;
}

