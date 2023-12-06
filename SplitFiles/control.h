#include "game.h"
#include "bread.h"

#ifndef _DEFINE_CONTROL_H
#define _DEFINE_CONTROL_H

#define CONTROL_X_COORD 18
#define CONTROL_Y_COORD 13
#define ORDER_X_COORD 18
#define ORDER_Y_COORD 4

// max stack depth
static const unsigned int CONTROL_BUFFER_SIZE = 10;
// length game runs for
#define MAX_TICKS (4096)
static const unsigned int TICK_DIV = 4096 / 16;

void command_entry_func_instant(GameComponent* input, GameParameters* params);

#endif
