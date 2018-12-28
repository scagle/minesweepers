#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "logic.h"
#include "graphics.h"

#define GAME_WIDTH  40
#define GAME_HEIGHT 40

void board_init    ( );
void graphics_init ( );
void main_loop     ( );

//*****************************************************************************
//*                              Functions                                    *
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

void main_loop()
{
    int cursor_x = 5;
    int cursor_y = 5;
    handle_input(REVEAL, cursor_x, cursor_y);
    print_game();
}

//*****************************************************************************
//*                             Initializations                               *
//*****************************************************************************
void board_init()
{
    randomize();
}

void graphics_init()
{
}

