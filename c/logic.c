#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "logic.h"

#define GAME_WIDTH   40
#define GAME_HEIGHT  40
#define NUM_BOMBS    50

Tile    tiles   [GAME_WIDTH][GAME_HEIGHT]; // Tile information 
Overlay overlays[GAME_WIDTH][GAME_HEIGHT]; // Overlay information 

int cursor_x = 5;
int cursor_y = 5;


void randomize()
{
    srand(time(NULL));
    int x, y;
    // Wipe board
    for (x = 0; x < GAME_WIDTH; x++)
    {
        for (y = 0; y < GAME_HEIGHT; y++)
        {
            tiles   [x][y] = NONE;
            overlays[x][y] =  FOG;
        }
    }

    // Lay Bombs 
    int bomb, pos;
    for (bomb = 0; bomb < NUM_BOMBS; bomb++)
    {
        bool success = false;
        while (!success)
        {
            pos = rand() % (GAME_WIDTH * GAME_HEIGHT);
            int new_x = pos / GAME_WIDTH;
            int new_y = pos % GAME_WIDTH;
            if (tiles[new_x][new_y] != BOMB)
            {
                tiles[new_x][new_y] = BOMB;
                success = true;
            }
        }
    }

    // Increment adjacent tiles
    //   (go through each empty tile, and assign it the #of_adjacent_bombs)
    for (x = 0; x < GAME_WIDTH; x++)
    {
        for (y = 0; y < GAME_HEIGHT; y++)
        {
            if (tiles[x][y] == NONE)
            {
                tiles[x][y] = get_adj_bombs(x, y); // [0-8], 0=NONE
            }
        }
    }
}

Tile get_adj_bombs(int x, int y)
{
    // My basic/inefficient algorithm for finding all adjacent bombs to a tile
    int bombs = 0;
    if (x == 0)                 // Left Case
    {
        if (y == 0)             // Ignore Left & Top
        {
            if (tiles[x+1][y  ] == BOMB) bombs++;
            if (tiles[x  ][y+1] == BOMB) bombs++;
            if (tiles[x+1][y+1] == BOMB) bombs++;
        }
        else 
        if (y == GAME_HEIGHT-1) // Ignore Left & Bottom
        {
            if (tiles[x+1][y  ] == BOMB) bombs++;
            if (tiles[x  ][y-1] == BOMB) bombs++;
            if (tiles[x+1][y-1] == BOMB) bombs++;
        }
        else                    // Ignore Left
        {
            if (tiles[x+1][y-1] == BOMB) bombs++;
            if (tiles[x+1][y  ] == BOMB) bombs++;
            if (tiles[x+1][y+1] == BOMB) bombs++;
            if (tiles[x  ][y-1] == BOMB) bombs++;
            if (tiles[x  ][y+1] == BOMB) bombs++;
        }
    }
    else
    if (x == GAME_WIDTH-1)      // Right Case
    {
        if (y == 0)             // Ignore Right & Top
        {
            if (tiles[x-1][y  ] == BOMB) bombs++;
            if (tiles[x  ][y+1] == BOMB) bombs++;
            if (tiles[x-1][y+1] == BOMB) bombs++;
        }
        else 
        if (y == GAME_HEIGHT-1) // Ignore Right & Bottom
        {
            if (tiles[x-1][y  ] == BOMB) bombs++;
            if (tiles[x  ][y-1] == BOMB) bombs++;
            if (tiles[x-1][y-1] == BOMB) bombs++;
        }
        else                    // Ignore Right
        {
            if (tiles[x-1][y-1] == BOMB) bombs++;
            if (tiles[x-1][y  ] == BOMB) bombs++;
            if (tiles[x-1][y+1] == BOMB) bombs++;
            if (tiles[x  ][y-1] == BOMB) bombs++;
            if (tiles[x  ][y+1] == BOMB) bombs++;
        }
    }
    else                        // In-Between Case
    {
        if (y == 0)             // Ignore Top
        {
            if (tiles[x-1][y+1] == BOMB) bombs++;
            if (tiles[x  ][y+1] == BOMB) bombs++;
            if (tiles[x+1][y+1] == BOMB) bombs++;
            if (tiles[x-1][y  ] == BOMB) bombs++;
            if (tiles[x+1][y  ] == BOMB) bombs++;
        }
        else
        if (y == GAME_HEIGHT-1) // Ignore Bottom
        {
            if (tiles[x-1][y-1] == BOMB) bombs++;
            if (tiles[x  ][y-1] == BOMB) bombs++;
            if (tiles[x+1][y-1] == BOMB) bombs++;
            if (tiles[x-1][y  ] == BOMB) bombs++;
            if (tiles[x+1][y  ] == BOMB) bombs++;
        }
        else                    // Ignore NOTHING! :)
        {
            if (tiles[x-1][y-1] == BOMB) bombs++;
            if (tiles[x  ][y-1] == BOMB) bombs++;
            if (tiles[x+1][y-1] == BOMB) bombs++;
            if (tiles[x-1][y  ] == BOMB) bombs++;
            if (tiles[x  ][y  ] == BOMB) bombs++;
            if (tiles[x+1][y  ] == BOMB) bombs++;
            if (tiles[x-1][y+1] == BOMB) bombs++;
            if (tiles[x  ][y+1] == BOMB) bombs++;
            if (tiles[x+1][y+1] == BOMB) bombs++;
        }
    }
    Tile result = (Tile)bombs;
    return result;
}

void reveal(int x, int y)
{
    // My inefficient, overly recursive method of revealing tiles
    //  that is piggy-backing off of find_adj_bombs's basic/inefficient algorithm
    overlays[x][y] = CLEAR;
    if (tiles[x][y] == NONE) // If blank, keep checking for adjacent non-bombs
    {
        if (x == 0)                 // Left Case
        {
            if (y == 0)             // Ignore Left & Top
            {
                if (overlays[x+1][y  ] == FOG && tiles[x+1][y  ] < 9) reveal(x+1, y  ); 
                if (overlays[x  ][y+1] == FOG && tiles[x  ][y+1] < 9) reveal(x  , y+1);
                if (overlays[x+1][y+1] == FOG && tiles[x+1][y+1] < 9) reveal(x+1, y+1);
            }
            else 
            if (y == GAME_HEIGHT-1) // Ignore Left & Bottom
            {
                if (overlays[x+1][y  ] == FOG && tiles[x+1][y  ] < 9) reveal(x+1, y  );
                if (overlays[x  ][y-1] == FOG && tiles[x  ][y-1] < 9) reveal(x  , y-1);
                if (overlays[x+1][y-1] == FOG && tiles[x+1][y-1] < 9) reveal(x+1, y-1);
            }
            else                    // Ignore Left
            {
                if (overlays[x+1][y-1] == FOG && tiles[x+1][y-1] < 9) reveal(x+1, y-1);
                if (overlays[x+1][y  ] == FOG && tiles[x+1][y  ] < 9) reveal(x+1, y  );
                if (overlays[x+1][y+1] == FOG && tiles[x+1][y+1] < 9) reveal(x+1, y+1);
                if (overlays[x  ][y-1] == FOG && tiles[x  ][y-1] < 9) reveal(x  , y-1);
                if (overlays[x  ][y+1] == FOG && tiles[x  ][y+1] < 9) reveal(x  , y+1);
            }
        }
        else
        if (x == GAME_WIDTH-1)      // Right Case
        {
            if (y == 0)             // Ignore Right & Top
            {
                if (overlays[x-1][y  ] == FOG && tiles[x-1][y  ] < 9) reveal(x-1, y  );
                if (overlays[x  ][y+1] == FOG && tiles[x  ][y+1] < 9) reveal(x  , y+1);
                if (overlays[x-1][y+1] == FOG && tiles[x-1][y+1] < 9) reveal(x-1, y+1);
            }
            else 
            if (y == GAME_HEIGHT-1) // Ignore Right & Bottom
            {
                if (overlays[x-1][y  ] == FOG && tiles[x-1][y  ] < 9) reveal(x-1, y  );
                if (overlays[x  ][y-1] == FOG && tiles[x  ][y-1] < 9) reveal(x  , y-1);
                if (overlays[x-1][y-1] == FOG && tiles[x-1][y-1] < 9) reveal(x-1, y-1);
            }
            else                    // Ignore Right
            {
                if (overlays[x-1][y-1] == FOG && tiles[x-1][y-1] < 9) reveal(x-1, y-1);
                if (overlays[x-1][y  ] == FOG && tiles[x-1][y  ] < 9) reveal(x-1, y  );
                if (overlays[x-1][y+1] == FOG && tiles[x-1][y+1] < 9) reveal(x-1, y+1);
                if (overlays[x  ][y-1] == FOG && tiles[x  ][y-1] < 9) reveal(x  , y-1);
                if (overlays[x  ][y+1] == FOG && tiles[x  ][y+1] < 9) reveal(x  , y+1);
            }
        }
        else                        // In-Between Case
        {
            if (y == 0)             // Ignore Top
            {
                if (overlays[x-1][y+1] == FOG && tiles[x-1][y+1] < 9) reveal(x-1, y+1);
                if (overlays[x  ][y+1] == FOG && tiles[x  ][y+1] < 9) reveal(x  , y+1);
                if (overlays[x+1][y+1] == FOG && tiles[x+1][y+1] < 9) reveal(x+1, y+1);
                if (overlays[x-1][y  ] == FOG && tiles[x-1][y  ] < 9) reveal(x-1, y  );
                if (overlays[x+1][y  ] == FOG && tiles[x+1][y  ] < 9) reveal(x+1, y  );
            }
            else
            if (y == GAME_HEIGHT-1) // Ignore Bottom
            {
                if (overlays[x-1][y-1] == FOG && tiles[x-1][y-1] < 9) reveal(x-1, y-1);
                if (overlays[x  ][y-1] == FOG && tiles[x  ][y-1] < 9) reveal(x  , y-1);
                if (overlays[x+1][y-1] == FOG && tiles[x+1][y-1] < 9) reveal(x+1, y-1);
                if (overlays[x-1][y  ] == FOG && tiles[x-1][y  ] < 9) reveal(x-1, y  );
                if (overlays[x+1][y  ] == FOG && tiles[x+1][y  ] < 9) reveal(x+1, y  );
            }
            else                    // Ignore NOTHING! :)
            {
                if (overlays[x-1][y-1] == FOG && tiles[x-1][y-1] < 9) reveal(x-1, y-1);
                if (overlays[x  ][y-1] == FOG && tiles[x  ][y-1] < 9) reveal(x  , y-1);
                if (overlays[x+1][y-1] == FOG && tiles[x+1][y-1] < 9) reveal(x+1, y-1);
                if (overlays[x-1][y  ] == FOG && tiles[x-1][y  ] < 9) reveal(x-1, y  );
                if (overlays[x  ][y  ] == FOG && tiles[x  ][y  ] < 9) reveal(x  , y  );
                if (overlays[x+1][y  ] == FOG && tiles[x+1][y  ] < 9) reveal(x+1, y  );
                if (overlays[x-1][y+1] == FOG && tiles[x-1][y+1] < 9) reveal(x-1, y+1);
                if (overlays[x  ][y+1] == FOG && tiles[x  ][y+1] < 9) reveal(x  , y+1);
                if (overlays[x+1][y+1] == FOG && tiles[x+1][y+1] < 9) reveal(x+1, y+1);
            }
        }
    }
}

void handle_input(Input action, int x, int y)
{
    switch (action)
    {
        case UP:
            if (cursor_y > 0)
                cursor_y--;
            break;
        case DOWN:
            if (cursor_y < GAME_HEIGHT-1)
                cursor_y++;
            break;
        case LEFT:
            if (cursor_x > 0)
                cursor_x--;
            break;
        case RIGHT:
            if (cursor_x < GAME_WIDTH-1)
                cursor_x++;
            break;
        case REVEAL:
            reveal(x, y);
            break;
        case PLACEFLAG:
            if (overlays[x][y] == FOG)
                overlays[x][y] = FLAG;
            break;
    }
}

//*****************************************************************************
//*                                 Utilities                                 *
//*****************************************************************************
// Print out the gameboard to console for debugging
void print_game()
{
    int x, y;
    for (x = 0; x < GAME_WIDTH; x++)
    {
        for (y = 0; y < GAME_HEIGHT; y++)
        {
            char result;
            switch (overlays[x][y])
            {
                case FOG: 
                    result = '?';
                    break;
                case CLEAR: 
                    result = tiles[x][y] + '0';  // Convert to char
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
