#ifndef LOGIC_H
#define LOGIC_H

#include "global.h"

Board     create_board    (                                             );
void      make_board      ( Board *board                                );
Tile      get_adj_mines   ( Board board  , int x        , int y         );
void      reveal          ( Board *board , int x        , int y         );
void      handle_input    ( Input action , Board *board                 );
void      print_game      ( Board board                                 );

#endif
