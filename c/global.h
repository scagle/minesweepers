#ifndef GLOBAL_H
#define GLOBAL_H

#define GAME_SIZE       30 // How many squares on game board? ( x + y  )
#define NUM_MINES        5 // How many mines on game board?   ( total  )
#define SCREEN_WIDTH  1400 // How many pixels on screen?      ( width  )
#define SCREEN_HEIGHT 1400 // How many pixels on screen?      ( height )

// Green, Pink, Red, Orange

typedef enum Inputs
{
    UP        , // 0
    DOWN      , // 1
    LEFT      , // 2
    RIGHT     , // 3
    REVEAL    , // 4
    TOGGLEFLAG  // 5
} Input;

typedef enum Tiles
{
     NONE     , // 0
     ONE      , // 1
     TWO      , // 2
     THREE    , // 3
     FOUR     , // 4
     FIVE     , // 5
     SIX      , // 6
     SEVEN    , // 7
     EIGHT    , // 8
     MINE       // 9
} Tile;

typedef enum Overlays
{
    FOG       , // 0
    CLEAR     , // 1
    FLAG        // 2
} Overlay;

typedef struct 
{
    Tile    tiles   [GAME_SIZE][GAME_SIZE]; // Tile information 
    Overlay overlays[GAME_SIZE][GAME_SIZE]; // Overlay information 
} Board;

#endif
