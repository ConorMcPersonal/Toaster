#include <input.h>
#include <stdio.h>
#include <stdlib.h>
#include <arch/zx.h>

#include "control.h"
#include "game.h"
#include "util.h"
#include "slot_monitor.h"
#include "music.h"

// Compile with:
// zcc +zx -vn -startup=1 -clib=sdcc_iy -D_TEST_CONTROL control.c -o control -create-app

char* buffer_getcommand(const char c) {
    const char* retVal = "         ";
    switch(c) {
        case 'B':
            retVal = "Brown";
            break;
        case 'W':
            retVal = "White";
            break;
        case 'T':
            retVal = "Toast it!";
            break;
        case 'P':
            retVal = "Pop it!";
            break;
        case 'G':
            retVal = "baGel";
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
            sprintf((char *)retVal, "slot %c", c);
            break;
        default:
            retVal = "you wot??";
    }
    return (char *)retVal;
}

//show the stack on the screen
void buffer_restack(const ControlBuffer* const buff) {
    int i;
    char* command;
    const int buffer_index = buff->bufferIndex;
    // Write out commands
    printf(PRINTAT"%c%c""%-12s", 18, 12, "Commands");
    for (i = 0; i < buffer_index; ++i) {
        command = buffer_getcommand((buff->buffer[i]));
        printf(PRINTAT"%c%c""%-12s", 18, i + 13, command);
    }
    // Ensure the rest is cleared
    for (; i <= CONTROL_BUFFER_SIZE; i++) {
        printf(PRINTAT"%c%c""%-12s", 18, i + 13, " ");
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
        if (d >= '0' && d <= '9' && (e == 'W' || e == 'B' || e == 'G'))  {
            //Valid format
            params->messageAddress = 100 + d - '0';
            BreadState* new_slice = malloc(sizeof(struct BreadStateStruct));
            new_slice->temperature = 0;
            switch(e) {
                case 'W':
                // white bread is driest
                new_slice->moisture = 50 + rand()%50;
                break;
                case 'B':
                // brown takes longer
                new_slice->moisture = 75 + rand()%60;
                break;
                case 'G':
                // Bagel is a gamble
                new_slice->moisture = 50 + rand()%150;
                break;
            }
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
        params->effect = TUNE_EFFECT_TICK;
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
    //printf(PRINTAT "\x01\x17" "%-32s", ctrlBuff->buffer);
 
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
