#include <SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "logic.h"

#define CURSOR_X_START 0
#define CURSOR_Y_START 0

int cursor_x = CURSOR_X_START;
int cursor_y = CURSOR_Y_START;
int end = 0;
int first_tile = 1; // flag to check if user lost on first turn

Board create_board()
{
    Board board;
    return board;
}

void board_init(Board *board)
{
    srand(time(NULL));
    int x, y;
    // Wipe board
    for (x = 0; x < GAME_SIZE; x++)
    {
        for (y = 0; y < GAME_SIZE; y++)
        {
            board->tiles   [x][y] = NONE;
            board->overlays[x][y] = FOG;
        }
    }

    // Lay Mines 
    int mine, pos;
    for (mine = 0; mine < NUM_MINES; mine++)
    {
        bool success = false;
        while (!success)
        {
            pos = rand() % (GAME_SIZE * GAME_SIZE);
            int new_x = pos / GAME_SIZE;
            int new_y = pos % GAME_SIZE;
            if (board->tiles[new_x][new_y] != MINE)
            {
                board->tiles[new_x][new_y] = MINE;
                success = true;
            }
        }
    }

    // Increment adjacent board.tiles
    //   (go through each empty tile, and assign it the #of_adjacent_mines)
    for (x = 0; x < GAME_SIZE; x++)
    {
        for (y = 0; y < GAME_SIZE; y++)
        {
            if (board->tiles[x][y] == NONE)
            {
                board->tiles[x][y] = get_adj_mines(*board, x, y); // [0-8], 0=NONE
            }
        }
    }
}

int update(SDL_Event event, Board *board)
{
    if (end == 0)
    {
        switch (event.key.keysym.sym)
        {
            // Movement
            case SDLK_w:                 // Move Up
                end = handle_input(UP, board);
                break;
            case SDLK_a:                 // Move Left
                end = handle_input(LEFT, board);
                break;
            case SDLK_s:                 // Move Down
                end = handle_input(DOWN, board);
                break;
            case SDLK_d:                 // Move Right
                end = handle_input(RIGHT, board);
                break;
            // Actions
            case SDLK_p:                 // Reveal
                end = handle_input(REVEAL, board);
                break;
            case SDLK_o:                 // Toggle Flag
                end = handle_input(TOGGLEFLAG, board);
                break;
            //case SDLK_p:                 // Print board (debugging)
            //    print_game(*board);
            //    break;
        }
    }
    if (event.key.keysym.sym == SDLK_r)
    {
        end = 0; // Reset ended game
        first_tile = 1; // Reset first tile flag
        *board = create_board();
        board_init(board);
    }

    return end;
}

Tile get_adj_mines(Board board, int x, int y)
{
    // My basic/inefficient algorithm for finding all adjacent mines to a tile
    //   (This is how the numbers on the tiles get generated)
    int mines = 0;
    if (x == 0)               // Left Case
    {
        if (y == 0)           // Ignore Left & Top
        {
            if (board.tiles[x+1][y  ] == MINE) mines++;
            if (board.tiles[x  ][y+1] == MINE) mines++;
            if (board.tiles[x+1][y+1] == MINE) mines++;
        }
        else 
        if (y == GAME_SIZE-1) // Ignore Left & Bottom
        {
            if (board.tiles[x+1][y  ] == MINE) mines++;
            if (board.tiles[x  ][y-1] == MINE) mines++;
            if (board.tiles[x+1][y-1] == MINE) mines++;
        }
        else                  // Ignore Left
        {
            if (board.tiles[x+1][y-1] == MINE) mines++;
            if (board.tiles[x+1][y  ] == MINE) mines++;
            if (board.tiles[x+1][y+1] == MINE) mines++;
            if (board.tiles[x  ][y-1] == MINE) mines++;
            if (board.tiles[x  ][y+1] == MINE) mines++;
        }
    }
    else
    if (x == GAME_SIZE-1)     // Right Case
    {
        if (y == 0)           // Ignore Right & Top
        {
            if (board.tiles[x-1][y  ] == MINE) mines++;
            if (board.tiles[x  ][y+1] == MINE) mines++;
            if (board.tiles[x-1][y+1] == MINE) mines++;
        }
        else 
        if (y == GAME_SIZE-1) // Ignore Right & Bottom
        {
            if (board.tiles[x-1][y  ] == MINE) mines++;
            if (board.tiles[x  ][y-1] == MINE) mines++;
            if (board.tiles[x-1][y-1] == MINE) mines++;
        }
        else                  // Ignore Right
        {
            if (board.tiles[x-1][y-1] == MINE) mines++;
            if (board.tiles[x-1][y  ] == MINE) mines++;
            if (board.tiles[x-1][y+1] == MINE) mines++;
            if (board.tiles[x  ][y-1] == MINE) mines++;
            if (board.tiles[x  ][y+1] == MINE) mines++;
        }
    }
    else                      // In-Between Case
    {
        if (y == 0)           // Ignore Top
        {
            if (board.tiles[x-1][y+1] == MINE) mines++;
            if (board.tiles[x  ][y+1] == MINE) mines++;
            if (board.tiles[x+1][y+1] == MINE) mines++;
            if (board.tiles[x-1][y  ] == MINE) mines++;
            if (board.tiles[x+1][y  ] == MINE) mines++;
        }
        else
        if (y == GAME_SIZE-1) // Ignore Bottom
        {
            if (board.tiles[x-1][y-1] == MINE) mines++;
            if (board.tiles[x  ][y-1] == MINE) mines++;
            if (board.tiles[x+1][y-1] == MINE) mines++;
            if (board.tiles[x-1][y  ] == MINE) mines++;
            if (board.tiles[x+1][y  ] == MINE) mines++;
        }
        else                  // Ignore NOTHING! :)
        {
            if (board.tiles[x-1][y-1] == MINE) mines++;
            if (board.tiles[x  ][y-1] == MINE) mines++;
            if (board.tiles[x+1][y-1] == MINE) mines++;
            if (board.tiles[x-1][y  ] == MINE) mines++;
            if (board.tiles[x  ][y  ] == MINE) mines++;
            if (board.tiles[x+1][y  ] == MINE) mines++;
            if (board.tiles[x-1][y+1] == MINE) mines++;
            if (board.tiles[x  ][y+1] == MINE) mines++;
            if (board.tiles[x+1][y+1] == MINE) mines++;
        }
    }
    Tile result = (Tile)mines;
    return result;
}

void reveal(Board *board, int x, int y)
{
    // My recursive method of revealing tiles
    //   (recursive because we want to lift "fog" from all clear tiles nearby)
    if (board->overlays[x][y] == FLAG)
        return;  // Don't allow us to reveal a flagged tile

    board->overlays[x][y] = CLEAR;
    if (board->tiles[x][y] == NONE) // If blank, keep checking for adjacent non-mines
    {
        if (x == 0)                 // Left Case
        {
            if (y == 0)             // Ignore Left & Top
            {
                if (board->overlays[x+1][y  ] == FOG && board->tiles[x+1][y  ] < 9) reveal(board, x+1, y  ); 
                if (board->overlays[x  ][y+1] == FOG && board->tiles[x  ][y+1] < 9) reveal(board, x  , y+1);
                if (board->overlays[x+1][y+1] == FOG && board->tiles[x+1][y+1] < 9) reveal(board, x+1, y+1);
            }
            else 
                if (y == GAME_SIZE-1)   // Ignore Left & Bottom
                {
                    if (board->overlays[x+1][y  ] == FOG && board->tiles[x+1][y  ] < 9) reveal(board, x+1, y  );
                    if (board->overlays[x  ][y-1] == FOG && board->tiles[x  ][y-1] < 9) reveal(board, x  , y-1);
                    if (board->overlays[x+1][y-1] == FOG && board->tiles[x+1][y-1] < 9) reveal(board, x+1, y-1);
                }
                else                    // Ignore Left
                {
                    if (board->overlays[x+1][y-1] == FOG && board->tiles[x+1][y-1] < 9) reveal(board, x+1, y-1);
                    if (board->overlays[x+1][y  ] == FOG && board->tiles[x+1][y  ] < 9) reveal(board, x+1, y  );
                    if (board->overlays[x+1][y+1] == FOG && board->tiles[x+1][y+1] < 9) reveal(board, x+1, y+1);
                    if (board->overlays[x  ][y-1] == FOG && board->tiles[x  ][y-1] < 9) reveal(board, x  , y-1);
                    if (board->overlays[x  ][y+1] == FOG && board->tiles[x  ][y+1] < 9) reveal(board, x  , y+1);
                }
        }
        else
            if (x == GAME_SIZE-1)       // Right Case
            {
                if( y == 0 )             // Ignore Right & Top
                {
                    if (board->overlays[x-1][y  ] == FOG && board->tiles[x-1][y  ] < 9) reveal(board, x-1, y  );
                    if (board->overlays[x  ][y+1] == FOG && board->tiles[x  ][y+1] < 9) reveal(board, x  , y+1);
                    if (board->overlays[x-1][y+1] == FOG && board->tiles[x-1][y+1] < 9) reveal(board, x-1, y+1);
                }
                else 
                    if (y == GAME_SIZE-1)   // Ignore Right & Bottom
                    {
                        if (board->overlays[x-1][y  ] == FOG && board->tiles[x-1][y  ] < 9) reveal(board, x-1, y  );
                        if (board->overlays[x  ][y-1] == FOG && board->tiles[x  ][y-1] < 9) reveal(board, x  , y-1);
                        if (board->overlays[x-1][y-1] == FOG && board->tiles[x-1][y-1] < 9) reveal(board, x-1, y-1);
                    }
                    else                    // Ignore Right
                    {
                        if (board->overlays[x-1][y-1] == FOG && board->tiles[x-1][y-1] < 9) reveal(board, x-1, y-1);
                        if (board->overlays[x-1][y  ] == FOG && board->tiles[x-1][y  ] < 9) reveal(board, x-1, y  );
                        if (board->overlays[x-1][y+1] == FOG && board->tiles[x-1][y+1] < 9) reveal(board, x-1, y+1);
                        if (board->overlays[x  ][y-1] == FOG && board->tiles[x  ][y-1] < 9) reveal(board, x  , y-1);
                        if (board->overlays[x  ][y+1] == FOG && board->tiles[x  ][y+1] < 9) reveal(board, x  , y+1);
                    }
            }
            else                        // In-Between Case
            {
                if (y == 0)             // Ignore Top
                {
                    if (board->overlays[x-1][y+1] == FOG && board->tiles[x-1][y+1] < 9) reveal(board, x-1, y+1);
                    if (board->overlays[x  ][y+1] == FOG && board->tiles[x  ][y+1] < 9) reveal(board, x  , y+1);
                    if (board->overlays[x+1][y+1] == FOG && board->tiles[x+1][y+1] < 9) reveal(board, x+1, y+1);
                    if (board->overlays[x-1][y  ] == FOG && board->tiles[x-1][y  ] < 9) reveal(board, x-1, y  );
                    if (board->overlays[x+1][y  ] == FOG && board->tiles[x+1][y  ] < 9) reveal(board, x+1, y  );
                }
                else
                    if (y == GAME_SIZE-1)   // Ignore Bottom
                    {
                        if (board->overlays[x-1][y-1] == FOG && board->tiles[x-1][y-1] < 9) reveal(board, x-1, y-1);
                        if (board->overlays[x  ][y-1] == FOG && board->tiles[x  ][y-1] < 9) reveal(board, x  , y-1);
                        if (board->overlays[x+1][y-1] == FOG && board->tiles[x+1][y-1] < 9) reveal(board, x+1, y-1);
                        if (board->overlays[x-1][y  ] == FOG && board->tiles[x-1][y  ] < 9) reveal(board, x-1, y  );
                        if (board->overlays[x+1][y  ] == FOG && board->tiles[x+1][y  ] < 9) reveal(board, x+1, y  );
                    }
                    else                    // Ignore NOTHING! :)
            {
                if (board->overlays[x-1][y-1] == FOG && board->tiles[x-1][y-1] < 9) reveal(board, x-1, y-1);
                if (board->overlays[x  ][y-1] == FOG && board->tiles[x  ][y-1] < 9) reveal(board, x  , y-1);
                if (board->overlays[x+1][y-1] == FOG && board->tiles[x+1][y-1] < 9) reveal(board, x+1, y-1);
                if (board->overlays[x-1][y  ] == FOG && board->tiles[x-1][y  ] < 9) reveal(board, x-1, y  );
                if (board->overlays[x  ][y  ] == FOG && board->tiles[x  ][y  ] < 9) reveal(board, x  , y  );
                if (board->overlays[x+1][y  ] == FOG && board->tiles[x+1][y  ] < 9) reveal(board, x+1, y  );
                if (board->overlays[x-1][y+1] == FOG && board->tiles[x-1][y+1] < 9) reveal(board, x-1, y+1);
                if (board->overlays[x  ][y+1] == FOG && board->tiles[x  ][y+1] < 9) reveal(board, x  , y+1);
                if (board->overlays[x+1][y+1] == FOG && board->tiles[x+1][y+1] < 9) reveal(board, x+1, y+1);
            }
        }
    }
}

void reveal_all(Board *board) // Reveals all tiles in event of game-over
{
    for (int x = 0; x < GAME_SIZE; x++)
        for (int y = 0; y < GAME_SIZE; y++)
            board->overlays[x][y] = CLEAR;
}

void relocate_mine(Board *board, int cursor_x, int cursor_y) 
{
    // Handles situation if dead on first tile
    // Will do what official minesweeper does by simply moving the bomb somewhere else
    int escape = 0;
    for (int x = 0; x < GAME_SIZE; x++)
    {
        for (int y = 0; y < GAME_SIZE; y++)
        {
            if (board->tiles[x][y] != 9) // if not mine
            {
                board->tiles[x][y] = 9;  // add mine at free spot
                board->tiles[cursor_x][cursor_y] = 0; // remove mine at cursor
                escape = 1;
                break;
            }
        }
        if (escape) // break out of both loops if flag is set
            break;
    }
    // Reset all non-bomb tiles to zero, so they can be re-numbered
    for (int x = 0; x < GAME_SIZE; x++)
        for (int y = 0; y < GAME_SIZE; y++)
            if (board->tiles[x][y] != 9)
                board->tiles[x][y] = 0;  // add mine at free spot

    // Reset all numbering on tiles
    //   (go through each empty tile, and assign it the #of_adjacent_mines)
    for (int x = 0; x < GAME_SIZE; x++)
    {
        for (int y = 0; y < GAME_SIZE; y++)
        {
            if (board->tiles[x][y] == NONE)
            {
                board->tiles[x][y] = get_adj_mines(*board, x, y); // [0-8], 0=NONE
            }
        }
    }
}

int handle_input(Input action, Board *board)
{
    int result = 0; // play on
    switch (action)
    {
        case UP:
            if (cursor_y > 0)
                cursor_y--;
            break;
        case DOWN:
            if (cursor_y < GAME_SIZE-1)
                cursor_y++;
            break;
        case LEFT:
            if (cursor_x > 0)
                cursor_x--;
            break;
        case RIGHT:
            if (cursor_x < GAME_SIZE-1)
                cursor_x++;
            break;
        case REVEAL:
            // Checking if lost
            if (board->tiles[cursor_y][cursor_x] == MINE)
            {
                if (first_tile)        // If gameover on very first tile, then cut player some slack
                {
                    printf("RELOCATING!\n");
                    relocate_mine(board, cursor_y, cursor_x); 
                }
                else                   // Otherwise... you lost
                {
                    reveal_all(board); // Reveal all tiles on the board
                    result = 1;        // MINE HAS BEEN HIT! END GAME!
                    break;
                }
            }
            // Reveal what's underneath the tile
            reveal(board, cursor_y, cursor_x);
            first_tile = 0; // First tile is unearthed, therefor gameover possible

            // Checking if won
            int won = 1; // Proof by contradiction flag
            for (int x = 0; x < GAME_SIZE; x++)
            {
                for (int y = 0; y < GAME_SIZE; y++)
                {
                    // If there are fogged up non-bomb tiles then we haven't won
                    if (board->overlays[x][y] == FOG && board->tiles[x][y] != MINE)
                    {
                        won = 0; 
                        break;
                    }
                    // If there are false positive flags, then we haven't won
                    else if (board->overlays[x][y] == FLAG && board->tiles[x][y] != MINE)
                    {
                        won = 0; 
                        break;
                    }
                }
                if (!won) // break out of both loops if flag is set
                    break;
            }
            if (won)
                result = 2;

            break;

        case TOGGLEFLAG:
            if (board->overlays[cursor_y][cursor_x] == FOG)
                board->overlays[cursor_y][cursor_x] = FLAG;
            else if (board->overlays[cursor_y][cursor_x] == FLAG)
                board->overlays[cursor_y][cursor_x] = FOG;
            break;
    }
    return result; // 0 = play on, 1 = lost, 2 = won
}

//*****************************************************************************
//*                                 Utilities                                 *
//*****************************************************************************
int get_cursor_x()
{ return cursor_x; }
int get_cursor_y()
{ return cursor_y; }

// Print out the gameboard to console for debugging
void print_game(Board board)
{
    int x, y;
    for (x = 0; x < GAME_SIZE; x++)
    {
        for (y = 0; y < GAME_SIZE; y++)
        {
            char result;
            switch (board.overlays[x][y])
            {
                case FOG: 
                    result = '#';
                    break;
                case CLEAR: 
                    result = board.tiles[x][y] + '0';  // Convert to char
                    if (result == '0') 
                        result = ' '; // only exception: clear = space
                    break;
                case FLAG: 
                    result = 'F';
                    break;
                }
            printf(" %c", result);
        }
        printf("\n");
    }
}
