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


struct control_buffer_struct {
    int  bufferIndex;
    int  prevBufferIndex;
    unsigned char lastCharSeen;
    unsigned char* buffer;
};
typedef struct control_buffer_struct ControlBuffer;

int buffer_push(unsigned char c, ControlBuffer* buff);

unsigned char buffer_pop(ControlBuffer* buff);

void initialise_control_buffer(ControlBuffer *buff);

void command_entry_func(GameComponent* input, GameParameters* params);

void command_entry_func_instant(GameComponent* input, GameParameters* params);

//void bread_restack(BreadBin*);

#endif
