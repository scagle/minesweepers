#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "logic.h"
#include "global.h"

int cursor_x = 5;
int cursor_y = 5;

Board create_board()
{
    Board board;
    board.tiles[GAME_SIZE][GAME_SIZE]; // Tile information 
    board.overlays[GAME_SIZE][GAME_SIZE]; // Overlay information 
    return board;
}

void randomize_board(Board board)
{
    srand(time(NULL));
    int x, y;
    // Wipe board
    for (x = 0; x < GAME_SIZE; x++)
    {
        for (y = 0; y < GAME_SIZE; y++)
        {
            board.tiles   [x][y] = NONE;
            board.overlays[x][y] = CLEAR;
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
            if (board.tiles[new_x][new_y] != MINE)
            {
                board.tiles[new_x][new_y] = MINE;
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
            if (board.tiles[x][y] == NONE)
            {
                board.tiles[x][y] = get_adj_mines(board, x, y); // [0-8], 0=NONE
            }
        }
    }
}

Tile get_adj_mines(Board board, int x, int y)
{
    // My basic/inefficient algorithm for finding all adjacent mines to a tile
    int mines = 0;
    if (x == 0)                 // Left Case
    {
        if (y == 0)             // Ignore Left & Top
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
        else                    // Ignore Left
        {
            if (board.tiles[x+1][y-1] == MINE) mines++;
            if (board.tiles[x+1][y  ] == MINE) mines++;
            if (board.tiles[x+1][y+1] == MINE) mines++;
            if (board.tiles[x  ][y-1] == MINE) mines++;
            if (board.tiles[x  ][y+1] == MINE) mines++;
        }
    }
    else
    if (x == GAME_SIZE-1)      // Right Case
    {
        if (y == 0)             // Ignore Right & Top
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
        else                    // Ignore Right
        {
            if (board.tiles[x-1][y-1] == MINE) mines++;
            if (board.tiles[x-1][y  ] == MINE) mines++;
            if (board.tiles[x-1][y+1] == MINE) mines++;
            if (board.tiles[x  ][y-1] == MINE) mines++;
            if (board.tiles[x  ][y+1] == MINE) mines++;
        }
    }
    else                        // In-Between Case
    {
        if (y == 0)             // Ignore Top
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
        else                    // Ignore NOTHING! :)
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

void reveal(Board board, int x, int y)
{
    // My inefficient, overly recursive method of revealing tiles
    //  that is piggy-backing off of find_adj_mines's basic/inefficient algorithm
    board.overlays[x][y] = CLEAR;
    if (board.tiles[x][y] == NONE) // If blank, keep checking for adjacent non-mines
    {
        if (x == 0)                 // Left Case
        {
            if (y == 0)             // Ignore Left & Top
            {
                if (board.overlays[x+1][y  ] == FOG && board.tiles[x+1][y  ] < 9) reveal(board, x+1, y  ); 
                if (board.overlays[x  ][y+1] == FOG && board.tiles[x  ][y+1] < 9) reveal(board, x  , y+1);
                if (board.overlays[x+1][y+1] == FOG && board.tiles[x+1][y+1] < 9) reveal(board, x+1, y+1);
            }
            else 
            if (y == GAME_SIZE-1) // Ignore Left & Bottom
            {
                if (board.overlays[x+1][y  ] == FOG && board.tiles[x+1][y  ] < 9) reveal(board, x+1, y  );
                if (board.overlays[x  ][y-1] == FOG && board.tiles[x  ][y-1] < 9) reveal(board, x  , y-1);
                if (board.overlays[x+1][y-1] == FOG && board.tiles[x+1][y-1] < 9) reveal(board, x+1, y-1);
            }
            else                    // Ignore Left
            {
                if (board.overlays[x+1][y-1] == FOG && board.tiles[x+1][y-1] < 9) reveal(board, x+1, y-1);
                if (board.overlays[x+1][y  ] == FOG && board.tiles[x+1][y  ] < 9) reveal(board, x+1, y  );
                if (board.overlays[x+1][y+1] == FOG && board.tiles[x+1][y+1] < 9) reveal(board, x+1, y+1);
                if (board.overlays[x  ][y-1] == FOG && board.tiles[x  ][y-1] < 9) reveal(board, x  , y-1);
                if (board.overlays[x  ][y+1] == FOG && board.tiles[x  ][y+1] < 9) reveal(board, x  , y+1);
            }
        }
        else
        if (x == GAME_SIZE-1)      // Right Case
        {
            if (y == 0)             // Ignore Right & Top
            {
                if (board.overlays[x-1][y  ] == FOG && board.tiles[x-1][y  ] < 9) reveal(board, x-1, y  );
                if (board.overlays[x  ][y+1] == FOG && board.tiles[x  ][y+1] < 9) reveal(board, x  , y+1);
                if (board.overlays[x-1][y+1] == FOG && board.tiles[x-1][y+1] < 9) reveal(board, x-1, y+1);
            }
            else 
            if (y == GAME_SIZE-1) // Ignore Right & Bottom
            {
                if (board.overlays[x-1][y  ] == FOG && board.tiles[x-1][y  ] < 9) reveal(board, x-1, y  );
                if (board.overlays[x  ][y-1] == FOG && board.tiles[x  ][y-1] < 9) reveal(board, x  , y-1);
                if (board.overlays[x-1][y-1] == FOG && board.tiles[x-1][y-1] < 9) reveal(board, x-1, y-1);
            }
            else                    // Ignore Right
            {
                if (board.overlays[x-1][y-1] == FOG && board.tiles[x-1][y-1] < 9) reveal(board, x-1, y-1);
                if (board.overlays[x-1][y  ] == FOG && board.tiles[x-1][y  ] < 9) reveal(board, x-1, y  );
                if (board.overlays[x-1][y+1] == FOG && board.tiles[x-1][y+1] < 9) reveal(board, x-1, y+1);
                if (board.overlays[x  ][y-1] == FOG && board.tiles[x  ][y-1] < 9) reveal(board, x  , y-1);
                if (board.overlays[x  ][y+1] == FOG && board.tiles[x  ][y+1] < 9) reveal(board, x  , y+1);
            }
        }
        else                        // In-Between Case
        {
            if (y == 0)             // Ignore Top
            {
                if (board.overlays[x-1][y+1] == FOG && board.tiles[x-1][y+1] < 9) reveal(board, x-1, y+1);
                if (board.overlays[x  ][y+1] == FOG && board.tiles[x  ][y+1] < 9) reveal(board, x  , y+1);
                if (board.overlays[x+1][y+1] == FOG && board.tiles[x+1][y+1] < 9) reveal(board, x+1, y+1);
                if (board.overlays[x-1][y  ] == FOG && board.tiles[x-1][y  ] < 9) reveal(board, x-1, y  );
                if (board.overlays[x+1][y  ] == FOG && board.tiles[x+1][y  ] < 9) reveal(board, x+1, y  );
            }
            else
            if (y == GAME_SIZE-1) // Ignore Bottom
            {
                if (board.overlays[x-1][y-1] == FOG && board.tiles[x-1][y-1] < 9) reveal(board, x-1, y-1);
                if (board.overlays[x  ][y-1] == FOG && board.tiles[x  ][y-1] < 9) reveal(board, x  , y-1);
                if (board.overlays[x+1][y-1] == FOG && board.tiles[x+1][y-1] < 9) reveal(board, x+1, y-1);
                if (board.overlays[x-1][y  ] == FOG && board.tiles[x-1][y  ] < 9) reveal(board, x-1, y  );
                if (board.overlays[x+1][y  ] == FOG && board.tiles[x+1][y  ] < 9) reveal(board, x+1, y  );
            }
            else                    // Ignore NOTHING! :)
            {
                if (board.overlays[x-1][y-1] == FOG && board.tiles[x-1][y-1] < 9) reveal(board, x-1, y-1);
                if (board.overlays[x  ][y-1] == FOG && board.tiles[x  ][y-1] < 9) reveal(board, x  , y-1);
                if (board.overlays[x+1][y-1] == FOG && board.tiles[x+1][y-1] < 9) reveal(board, x+1, y-1);
                if (board.overlays[x-1][y  ] == FOG && board.tiles[x-1][y  ] < 9) reveal(board, x-1, y  );
                if (board.overlays[x  ][y  ] == FOG && board.tiles[x  ][y  ] < 9) reveal(board, x  , y  );
                if (board.overlays[x+1][y  ] == FOG && board.tiles[x+1][y  ] < 9) reveal(board, x+1, y  );
                if (board.overlays[x-1][y+1] == FOG && board.tiles[x-1][y+1] < 9) reveal(board, x-1, y+1);
                if (board.overlays[x  ][y+1] == FOG && board.tiles[x  ][y+1] < 9) reveal(board, x  , y+1);
                if (board.overlays[x+1][y+1] == FOG && board.tiles[x+1][y+1] < 9) reveal(board, x+1, y+1);
            }
        }
    }
}

void handle_input(Input action, Board board, int x, int y)
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
            reveal(board, x, y);
            break;
        case PLACEFLAG:
            if (board.overlays[x][y] == FOG)
                board.overlays[x][y] = FLAG;
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
                    result = '?';
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
