#include <input.h>
#include <stdio.h>
#include <stdlib.h>
#include <arch/zx.h>

#include "control.h"
#include "game.h"
#include "util.h"
#include "slot_monitor.h"

// Compile with:
// zcc +zx -vn -startup=1 -clib=sdcc_iy -D_TEST_CONTROL control.c -o control -create-app

char* buffer_getcommand(const char c) {
    char* retVal = "          ";
    switch(c) {
        case 0:
            break;
        case 'B':
            retVal = "BROWN BREAD";
            break;
        case 'W':
            retVal = "WHITE BREAD";
            break;
        case 'T':
            retVal = "TOAST IT!";
            break;
        case 'P':
            retVal = "POP SLOT";
            break;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            sprintf(retVal, "SLOT %c", c);
            break;
        default:
            retVal = "YOU WOT MATE?";
    }
    return retVal;
}

//show the stack on the screen
void buffer_restack(const ControlBuffer* const buff) {
    int i;
    char* command;
    const int buffer_index = buff->bufferIndex;
    // first clean the buffer
    for (i = 2; i <= CONTROL_BUFFER_SIZE; i++) {
        printf(PRINTAT"%c%c""            \0", 21, i);
    }
    // now write out stack
    for (i = 2; i <= buffer_index; i++) {
        command = buffer_getcommand((buff->buffer[i - 1]));
        printf(PRINTAT"%c%c""%s\0", 21, i, command);
    }
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
    if (buff->bufferIndex == 1) {
        return 0;
    }
    buff->bufferIndex = buff->bufferIndex - 1;
    c = (buff->buffer)[buff->bufferIndex];
    (buff->buffer)[buff->bufferIndex] = 0;
    return c;
}

void initialise_control_buffer(ControlBuffer *buff) {
    buff->bufferIndex = 1;
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
            //Bad format - maybe indicate somehow?
            ;
        }
    } else if (c == 'T') {
        //Push some bread to a slot
        d = buffer_pop(ctrlBuff);
        e = buffer_pop(ctrlBuff);
        if (d >= '0' && d <= '9' && (e == 'W' || e == 'B'))  {
            //Valid format
            params->messageAddress = 100 + d - '0';
            BreadState* new_slice = malloc(sizeof(struct BreadStateStruct));
            new_slice->temperature = 0;
            new_slice->moisture = ((e == 'B')? 75: 50) + rand()%50; //Brown takes longer
            new_slice->toastedness = 0;
            params->message = new_slice;
            params->messageSourceAddress = (void *)ctrlBuff;
        } else {
            //Bad format - maybe indicate somehow?
            ;
        }
    } else {
        //It's a bust
        buffer_push(c, ctrlBuff);
    }
    buffer_restack(ctrlBuff);
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
        if (c == 'D' || c == 12) {
            buffer_pop(ctrlBuff);
        }
        else if (c == 'E') {
            // Execute any valid command at head of stack

            // For now just empty buffer
            //ctrlBuff->bufferIndex = 0;
            //(ctrlBuff->buffer)[0] = 0;
            execute_command(ctrlBuff, params);
        } else {
            buffer_push(c, ctrlBuff);
        }
        buffer_restack(ctrlBuff);
    }
    ctrlBuff->lastCharSeen = c;
//  printf(PRINTAT "\x01\x17" "%-32s", ctrlBuff->buffer);
 
}

#ifdef _TEST_CONTROL

int main() {
    unsigned char c = 0;
    GameParameters params = {
      0,
      0,
      0,
      0,
      100,
      0,
      NULL
    };
    ControlBuffer buff = {
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
