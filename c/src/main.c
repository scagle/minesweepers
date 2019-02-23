#include <SDL.h>
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
    int error = graphics_init();
    if (error)
    {
        printf("Initialization failed!\n");
        return 1;
    }

    // Board Initialization
    Board board = create_board();
    board_init(&board);

    // Main Loop
    int cursor_x = get_cursor_x();
    int cursor_y = get_cursor_y();
    int quit = 0; // quit application
    int  end = 0; // 0 = game is on, 1 = game over (lost), 2 = game over (won)
    SDL_Event event;

    render(board, cursor_x, cursor_y, end);
    while (!quit)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)          // If window closed
            {
                quit = 1;
            }
            else if (event.type == SDL_KEYDOWN)  // If key pressed
            {
                switch (event.key.keysym.sym)
                {
                    // System events
                    case SDLK_q:
                        quit = 1;
                        break;
                    // Board events
                    default:
                        end = update(event, &board);
                }
                cursor_x = get_cursor_x();
                cursor_y = get_cursor_y();
                render(board, cursor_x, cursor_y, end);
            }
        }
               
    }

    // Wrap things up
    graphics_close();
    return 0;
}

