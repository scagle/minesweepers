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

int main(int argc, char* args[])
{
    // TODO: Incorporate Rendering of SDL
    // Graphics Initializations
    graphics_init();

    // Board Initialization
    Board board = create_board();
    board_init(&board);

    // Main Loop
    int cursor_x = 0;
    int cursor_y = 0;
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
                    // System events
                    case SDLK_q:
                        quit = true;
                        break;
                    // Board events
                    default:
                        update(event, &board);
                }
                cursor_x = get_cursor_x();
                cursor_y = get_cursor_y();
                render(board, cursor_x, cursor_y);
            }
        }
               
    }

    // Wrap things up
    graphics_close();
    return 0;
}

