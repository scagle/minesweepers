#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "logic.h"

#define CURSOR_X_START 5
#define CURSOR_Y_START 5

int cursor_x = CURSOR_X_START;
int cursor_y = CURSOR_Y_START;

Board create_board()
{
    Board board;
    board.tiles[GAME_SIZE][GAME_SIZE]; // Tile information 
    board.overlays[GAME_SIZE][GAME_SIZE]; // Overlay information 
    return board;
}

void make_board(Board *board)
{
    cursor_x = CURSOR_X_START;
    cursor_y = CURSOR_Y_START;
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
            if (y == 0)             // Ignore Right & Top
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

void handle_input(Input action, Board *board)
{
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
            reveal(board, cursor_y, cursor_x);
            break;
        case PLACEFLAG:
            if (board->overlays[cursor_y][cursor_x] == FOG)
                board->overlays[cursor_y][cursor_x] = FLAG;
            break;
    }
}

//*****************************************************************************
//*                                 Utilities                                 *
//*****************************************************************************
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
