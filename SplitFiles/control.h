#include "game.h"

#ifndef _DEFINE_CONTROL_H
#define _DEFINE_CONTROL_H

// max stack depth
static const unsigned int CONTROL_BUFFER_SIZE = 10;
// length game runs for
static const unsigned int MAX_TICKS = 4096;


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

void bread_restack();

#endif
