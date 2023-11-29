#ifndef _DEFINE_GAME_H
#define _DEFINE_GAME_H

#include "bread.h"
#include "base.h"

struct dispatcher_struct {
  int last_time_int;
};
typedef struct dispatcher_struct DispatcherState;

//what orders are waiting
#define MAX_ORDER_LIST 7

//When does the smake alarm go off?
#define BURNT_TOAST 200 

int play_game( GameParameters* params );

#endif
