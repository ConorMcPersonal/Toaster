#include <input.h>
#include <stdio.h>
#include <stdlib.h>
#include <arch/zx.h>
#include <stdbool.h>

#include "control.h"
#include "game.h"
#include "util.h"
#include "slot_monitor.h"
#include "music.h"

// Compile with:
// zcc +zx -vn -startup=1 -clib=sdcc_iy -D_TEST_CONTROL control.c -o control -create-app

/*bool fetch_bread(const char c, BreadBin* bin)
{
    int i;
    bool bread_found = false;
    for (i = 0; i < MAX_ORDER_LIST; i++) {
        if (breadBin[i] == c) {
            bread_found = true;
            reorderBreadBin(i, bin);
            break;
        }
    }
    return bread_found;
}*/

const char* buffer_getcommand(const unsigned char c, GameParameters* params, char* retVal) {
    BreadType* breadType = get_bread_type(params->breadBin, c);
    if (breadType != NULL) {
        return breadType->desc;
    } else 
        if (c >='1' && c <= '9') {
            sprintf((char *)retVal, "slot %c", c);
    } else {
        switch(c) {
            case 'T':
                return "Toast it!";
            case 'P':
                return "Pop it!";
            default:
                return "You wot??";
        }
    }
    return retVal;
}

//show the bread on the screen
/*void bread_restack(BreadBin* bin)
{
    int i;
    for (i =0; i < MAX_ORDER_LIST; i++) {
        printf(PRINTAT"%c%c" "         ", ORDER_X_COORD, i + ORDER_Y_COORD);
        printf(PRINTAT"%c%c" "%s", ORDER_X_COORD, i + ORDER_Y_COORD, get_bread_type(bin, breadBin[i])->desc);
    }
}*/

//show the stack on the screen
void buffer_restack(ControlBuffer* buff, GameParameters* params) {
    int i;
    char* command;
    char* retVal = "         ";
    const int bufferIndex = buff->bufferIndex;
    int prevBufferIndex = buff->prevBufferIndex;
    // Write out commands
    if (prevBufferIndex < bufferIndex) {
        for (i = prevBufferIndex; i < bufferIndex; ++i) {
            command = buffer_getcommand((buff->buffer[i]), params, retVal);
            printf(PRINTAT"%c%c""%-12s", CONTROL_X_COORD, i + CONTROL_Y_COORD, command);
        }
    } else if (prevBufferIndex > bufferIndex) {
        // Ensure the rest is cleared
        for (i = prevBufferIndex; i >= bufferIndex; --i) {
            printf(PRINTAT"%c%c""%-12s", CONTROL_X_COORD, i + CONTROL_Y_COORD, " ");
        }
    }
    buff->prevBufferIndex = bufferIndex;
}

int buffer_push(unsigned char c, ControlBuffer* buff) {
    if (buff->bufferIndex > CONTROL_BUFFER_SIZE - 2) {
        return 0;
    }
    (buff->buffer)[buff->bufferIndex] = c;
    buff->bufferIndex = buff->bufferIndex + 1;
    (buff->buffer)[buff->bufferIndex] = 0;
    return 1;
}

unsigned char buffer_pop(ControlBuffer* buff) {
    unsigned char c;
    if (buff->bufferIndex == 0) {
        return 0;
    }
    buff->bufferIndex = buff->bufferIndex - 1;
    c = (buff->buffer)[buff->bufferIndex];
    (buff->buffer)[buff->bufferIndex] = 0;
    return c;
}

void initialise_control_buffer(ControlBuffer *buff) {
    buff->bufferIndex = 0;
    buff->prevBufferIndex = 0;
    buff->lastCharSeen = 0;
    buff->buffer = (unsigned char*)malloc(CONTROL_BUFFER_SIZE * sizeof(unsigned char));
}

void execute_command(ControlBuffer *ctrlBuff, GameParameters* params) {
    unsigned char c, d, e; 
    c = buffer_pop(ctrlBuff);
    if (c == 'P') {
        //Pop some toast
        d = buffer_pop(ctrlBuff);
        if (d >= '0' && d <= '9') {
            //Valid formatting, try to pop that slot
            params->messageAddress = 200 + d - '0';
            params->messageSourceAddress = (void *)ctrlBuff;
        } else {
            // bad format - report error
            buffer_push('x', ctrlBuff);
        }
    } else if (c == 'T') {
        //Push some bread to a slot
        d = buffer_pop(ctrlBuff);
        e = buffer_pop(ctrlBuff);
        if (get_bread_type(params->breadBin, e) != NULL && d > '0' && d <= '9') {
            //Valid format
            params->messageAddress = 100 + d - '0';
            BreadState* new_slice = get_bread(params->breadBin, e);
            params->message = new_slice;
            params->messageSourceAddress = (void *)ctrlBuff;
        } else {
           // restack with error message too
           buffer_push(e, ctrlBuff);
           buffer_push(d, ctrlBuff);
           buffer_push('x', ctrlBuff);
        }
    } else {
        //It's a bust
        buffer_push(c, ctrlBuff);
    }
}

void execute_command_instant(ControlBuffer *ctrlBuff, GameParameters* params) {
    unsigned char c; 
    c = buffer_pop(ctrlBuff);
    if (c >= '0' && c <= '9') {
        //Pop that slot
        params->messageAddress = 200 + c - '0';
        params->messageSourceAddress = (void *)ctrlBuff;
    } else if (get_bread_type(params->breadBin, c) != NULL) {
        //Toast it - somewhere
        params->messageAddress = 100;
        BreadState* new_slice = get_bread(params->breadBin, c);
        params->message = new_slice;
        params->messageSourceAddress = (void *)ctrlBuff;
    }
}

void command_entry_func(GameComponent* input, GameParameters* params) {
    unsigned char c;
    ControlBuffer *ctrlBuff = (ControlBuffer*)input->ptr;
    //Check on uncaught message
    if (params->messageAddress) {
        if (params->messageSourceAddress == ctrlBuff) {
            // This message has come back to here uncaught - remove it
            if (params->messageAddress >= 100 && params->messageAddress < 200) {
                BreadState* brd = (BreadState*)params->message;
                params->score -= brd->type->cost;
                free(brd);
            }
            params->message = NULL;
            params->messageAddress = 0;
            params->messageSourceAddress = NULL;
        }
    }

    c = in_inkey();
    if (c >= 'a' && c <= 'z') {
        c -= 32;
    }
    if (c && c != ctrlBuff->lastCharSeen) {
        params->effect = TUNE_EFFECT_TICK;
        if (c == 'D' || c == 12) {
            buffer_pop(ctrlBuff);
        }
        else if (c == 'E') {
            // Execute any valid command at head of stack
            execute_command(ctrlBuff, params);
        } else {
            buffer_push(c, ctrlBuff); 
        }
        buffer_restack(ctrlBuff, params);
    }
    ctrlBuff->lastCharSeen = c;
}

void command_entry_func_instant(GameComponent* input, GameParameters* params) {
    unsigned char c;
    ControlBuffer *ctrlBuff = (ControlBuffer*)input->ptr;
    //Check on uncaught message
    if (params->messageAddress) {
        if (params->messageSourceAddress == ctrlBuff) {
            // This message has come back to here uncaught - remove it
            if (params->messageAddress >= 100 && params->messageAddress < 200) {
                BreadState* brd = (BreadState*)params->message;
                params->score -= brd->type->cost;
                free(brd);
            }
            params->message = NULL;
            params->messageAddress = 0;
            params->messageSourceAddress = NULL;
        }
    }

    c = in_inkey();
    if (c >= 'a' && c <= 'z') {
        c -= 32;
    }
    if (c && c != ctrlBuff->lastCharSeen) {
        params->effect = TUNE_EFFECT_TICK;
        // Execute immediately
        buffer_push(c, ctrlBuff); 
        execute_command_instant(ctrlBuff, params);
    }
    ctrlBuff->lastCharSeen = c;
}

#ifdef _TEST_CONTROL

int main() {
    unsigned char c = 0;
    BreadBin* bin = get_bread_bin();
    GameParameters params = {
      0,
      0,
      0,
      0,
      100,
      0,
      NULL,
      bin,
      1000
    };
    ControlBuffer buff = {
        0,
        0,
        0,
        NULL
    };
    initialise_control_buffer(&buff);
    GameComponent gc = {
        &buff,
        &command_entry_func,
        NULL
    };

    while (c != 32) {
        c = in_inkey();
        command_entry_func(&gc, &params);
        printf(PRINTAT "\x05\x05" "%d   ",c);
    }
    return 0;
}

#endif
