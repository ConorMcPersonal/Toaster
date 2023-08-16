#ifndef _DEFINE_CONTROL_H
#define _DEFINE_CONTROL_H

struct control_buffer_struct {
    int  bufferIndex;
    unsigned char lastCharSeen;
    unsigned char* buffer;
};
typedef struct control_buffer_struct ControlBuffer;

int buffer_push(unsigned char c, ControlBuffer* buff);

unsigned char buffer_pop(ControlBuffer* buff);

void initialise_control_buffer(ControlBuffer *buff);

#endif
