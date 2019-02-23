#ifndef LOGIC_H
#define LOGIC_H

#include <SDL.h>
#include "global.h"

Board create_board    (                                    );
void  board_init      ( Board *board                       );
int   update          (SDL_Event event, Board *board       );
Tile  get_adj_mines   ( Board board  , int x        , int y);
void  reveal          ( Board *board , int x        , int y);
int   handle_input    ( Input action , Board *board        );
int   get_cursor_y    (                                    );
int   get_cursor_x    (                                    );
void  print_game      ( Board board                        );

#endif
