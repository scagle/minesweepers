#include <SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "global.h"   // Contains global settings/structs/enums
#include "logic.h"    // Controls minesweeper logic
#include "graphics.h" // Controls graphics

// Function definitions
Board board_init(void);

int main(int argc, char* args[])
{
    // TODO: Incorporate Rendering of SDL
    // Graphics Initializations
    SDL_Window *window = graphics_init();

    // Board Initialization
    Board board = board_init();

    // Main Loop
    int x = 0, y = 0;
    bool quit = false;
    SDL_Event event;
    while (!quit)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)          // If window closed
            {
                quit = true;
            }
            else if (event.type == SDL_KEYDOWN)  // If key pressed
            {
                switch (event.key.keysym.sym)
                {
                    // Movement
                    case SDLK_w:                 // Print board
                        handle_input(UP, &board);
                        break;
                    case SDLK_a:                 // Print board
                        handle_input(LEFT, &board);
                        break;
                    case SDLK_s:                 // Print board
                        handle_input(DOWN, &board);
                        break;
                    case SDLK_d:                 // Print board
                        handle_input(RIGHT, &board);
                        break;
                    // Actions
                    case SDLK_f:                 // Print board
                        handle_input(REVEAL, &board);
                        break;
                    // Extras 
                    case SDLK_r:                 // Restart
                        board = board_init();
                        break;
                    case SDLK_p:                 // Print board
                        print_game(board);
                        break;
                    case SDLK_q:                 // Close window
                        quit = true;
                        break;
                }
            }
        }
    }

    // Wrap things up
    graphics_close(window);
    return 0;
}

Board board_init()
{
    Board board = create_board();
    make_board(&board);

    return board;
}
