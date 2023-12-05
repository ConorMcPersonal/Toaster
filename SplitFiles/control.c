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


void execute_command_instant(unsigned char c, GameParameters* params) {
    if (c >= '0' && c <= '9') {
        //Pop that slot
        params->messageAddress = 200 + c - '0';
    } else if (get_bread_type(params->breadBin, c) != NULL) {
        //Toast it - somewhere
        params->messageAddress = 100;
        BreadState* new_slice = get_bread(params->breadBin, c);
        params->message = new_slice;
    }
}


void command_entry_func_instant(GameComponent* input, GameParameters* params) {
    unsigned char c;
    static unsigned char lastCharSeen = 0;
    c = in_inkey();
    if (c >= 'a' && c <= 'z') {
        c -= 32;
    }
    if (c && c != lastCharSeen) {
        params->effect = TUNE_EFFECT_TICK;
        // Execute immediately
        execute_command_instant(c, params);
    }
    lastCharSeen = c;
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
