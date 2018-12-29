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

void         board_init    (            );
bool         graphics_init (            );
bool         main_loop     (            );
bool         loadMedia     (            );
SDL_Surface* loadSurface   ( char *path );
void         close         (            );

SDL_Window*  g_window          = NULL;
SDL_Surface* g_screen_surface  = NULL;
SDL_Surface* g_current_surface = NULL;
SDL_Rect     tile_rects[GAME_WIDTH][GAME_HEIGHT];

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

//*****************************************************************************
//*                             Initializations                               *
//*****************************************************************************
void board_init()
{
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
			g_screen_surface = SDL_GetWindowSurface( g_window );
		}
	}

	return success;
}

//*****************************************************************************
//*                              Functions                                    *
//*****************************************************************************
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
		if(!loadMedia())           //Load media
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			bool quit = false;     // Main loop flag
			SDL_Event e;           // Event handler

			while(!quit)                           // While application is running
			{
				while(SDL_PollEvent(&e) != 0)    // Handle events on queue
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

				//Apply the current image
				SDL_BlitSurface(g_current_surface, NULL, g_screen_surface, NULL);
			
				//Update the surface
				SDL_UpdateWindowSurface( g_window );
			}
		}
	}

	//Free resources and close SDL
    print_game();
	close();

	return 0;
}

bool loadMedia()
{
	//Loading success flag
    //TODO: Incorporate pictures
	bool success = true;
	g_current_surface = loadSurface( "../images/smile.bmp" );
	return success;
}

SDL_Surface* loadSurface( char *path )
{
	//Load image at specified path
	SDL_Surface* loaded_surface = SDL_LoadBMP( path );
	if( loaded_surface == NULL )
	{
		printf( "Unable to load image %s! SDL Error: %s\n", path, SDL_GetError() );
	}

	return loaded_surface;
}

void close()
{
	//Deallocate surfaces
	SDL_FreeSurface(g_current_surface);
	SDL_FreeSurface(g_screen_surface);
	g_current_surface = NULL;
	g_screen_surface = NULL;

	//Destroy window
	SDL_DestroyWindow(g_window);
	g_window = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}
