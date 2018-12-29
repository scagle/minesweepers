#ifndef LOGIC_H
#define LOGIC_H

typedef enum Inputs
{
//   0     1     2 
    UP,  DOWN, LEFT, RIGHT, REVEAL, PLACEFLAG
} Input;

typedef enum Tiles
{
     NONE  , // 0
     ONE   , // 1
     TWO   , // 2
     THREE , // 3
     FOUR  , // 4
     FIVE  , // 5
     SIX   , // 6
     SEVEN , // 7
     EIGHT , // 8
     BOMB    // 9
} Tile;

typedef enum Overlays
{
    FOG   , // 0
    CLEAR , // 1
    FLAG    // 2
} Overlay;

void      randomize     (                              );
Tile      get_adj_bombs ( int x        , int y         );
void      reveal        ( int x        , int y         );
void      handle_input  ( Input action , int x , int y );
void      print_game    (                              );
Tile     *get_tiles     (                              );
Overlay  *get_overlays  (                              );

#endif
