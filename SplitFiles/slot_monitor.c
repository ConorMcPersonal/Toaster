//////////////////////////////////////////////////////////
//
// Slot monitor component - does to screen updates for a slot
//
//////////////////////////////////////////////////////////

// Can be compiled with:
// zcc +zx -vn -startup=1 -clib=sdcc_iy -D_TEST_SLOT_MONITOR slot_monitor.c -o slot_monitor -create-app

#include <arch/zx.h>

#include <stdio.h>
#include <stdlib.h>
#include <input.h>
#include <string.h>
#include <stdbool.h>

#include "slot_monitor.h"
#include "util.h"

#define SLOT_RANGE 64
#define SLOT_VALUE_MAX 256

const char topLabel[] = {
    0x70, //0b01110000
    0x20, //0b00100000
    0x20, //0b00100000
    0x20, //0b00100000
    0x0c, //0b00001100
    0x0A, //0b00001010
    0x0c, //0b00001100
    0x08  //0b00001000
    };

const char bottomLabel[] = {
    0x6C, //0b01101100
    0x54, //0b01010100
    0x44, //0b01000100
    0x00, //0b00000000
    0x0E, //0b00001110
    0x04, //0b00000100
    0x04, //0b00000100
    0x04  //0b00000100
    };

void plot_tickmarks(SlotMonitor* slotMon) {
    int i;
    //Get row above the toastedness bar
    unsigned int y          = (slotMon->yBase - 1) * 8 + 3 * 4 - 1;
    unsigned int step_value = SLOT_VALUE_MAX / SLOT_RANGE;
    unsigned int x          = (slotMon->xBase - 1) * 8 + 100 / step_value;
    *zx_pxy2saddr(x,y) |= zx_px2bitmask(x);
    //Move to row below
    y+= 4;
    *zx_pxy2saddr(x,y) |= zx_px2bitmask(x);

    //Now plot the labels
    y = (slotMon->yBase - 1) * 8;
    x = (slotMon->xBase - 1) * 8 - 8;
    *zx_cxy2aaddr(slotMon->xBase - 2, slotMon->yBase - 1) = PAPER_CYAN + INK_BLUE;
    for (i = 0; i < 8; ++i) {
        *zx_pxy2saddr(x, y) = topLabel[i];
        ++y;
    }
    *zx_cxy2aaddr(slotMon->xBase - 2, slotMon->yBase) = PAPER_YELLOW + INK_BLACK;
    for (i = 0; i < 8; ++i) {
        *zx_pxy2saddr(x, y) = bottomLabel[i];
        ++y;
    }

}

int plot_value(SlotMonitor* slotMon, const int value, const unsigned int task,
    const int range, const int max, const int old_value)
{
    if (value != old_value) {
        bool increment = (value > old_value);
        if (value <= max && old_value >= 0) {
            const unsigned int offset = (slotMon->yBase - 1) * 8 + task * 4;
            int i, j, x;
            const unsigned int step_value = max / range;
            x = (slotMon->xBase - 1) * 8 + old_value / step_value;
            if (increment) {
                for (i = old_value; i < value; i+= step_value) {
                    for (j = 0; j < 3; j++) {
                        if (x > (slotMon->xBase - 1) * 8) { // avoid negative charting
                            *zx_pxy2saddr(x, offset + j) |= zx_px2bitmask(x);
                        }
                    }
                    x++;
                }
            }
            else {
                for (i = old_value; i >= value; i-= step_value) {
                    for (j = 0; j < 3; j++) {
                        unsigned char mask_out = ~zx_px2bitmask(x);
                        *zx_pxy2saddr(x, offset + j) &= mask_out;
                    }
                    x--;
                }
            }
        }
    }
    return value;
}


void draw_slot(SlotMonitor* slotMon, SlotState* slot) {
    // Removed - only works for a one slot setup
    slotMon->slot_old_temperature = plot_value(slotMon, slot->temperature, 0, SLOT_RANGE,
        SLOT_VALUE_MAX, slotMon->slot_old_temperature);
    slotMon->slot_old_power = plot_value(slotMon, slot->power, 1, SLOT_RANGE,
        SLOT_VALUE_MAX, slotMon->slot_old_power);
    if (slot->bread) {
        slotMon->bread_old_moisture = plot_value(slotMon, slot->bread->moisture, 2, SLOT_RANGE,
            SLOT_VALUE_MAX, slotMon->bread_old_moisture);
        slotMon->bread_old_toastedness = plot_value(slotMon, slot->bread->toastedness, 3, SLOT_RANGE,
            SLOT_VALUE_MAX, slotMon->bread_old_toastedness);
    } else if (slotMon->bread_old_moisture != 0 || slotMon->bread_old_toastedness != 0) {
        //no bread so wipe it - once
        plot_value(slotMon, 0, 2, SLOT_RANGE, SLOT_VALUE_MAX, SLOT_VALUE_MAX);
        plot_value(slotMon, 0, 3, SLOT_RANGE, SLOT_VALUE_MAX, SLOT_VALUE_MAX);
        slotMon->bread_old_toastedness = 0;
        slotMon->bread_old_moisture = 0;
    }
}

SlotMonitor* get_slot_monitor(unsigned char x, unsigned char y, int slotIndex) {
    long screenOffset;
    long attrOffset;
    int i;

    //Spectrum screen pixel arithmetic - three sections of 8 lines of 32 (8x8) characters.
    //All first rows, then all second rows and so on...
    screenOffset = 32 * 8 * 8 * ((y - 1) / 8) + 32 * ((y - 1) % 8) + x - 1;
    //Attributes a bit easier
    attrOffset = (y - 1) * 32 + x - 1;

    SlotMonitor* slotMon = malloc(sizeof(SlotMonitor));
    slotMon->xBase = x;
    slotMon->yBase = y;
    slotMon->startPixels =   (void *)(SCREENSTART + screenOffset);
    slotMon->startAttributes = (void *)(ATTRSTART + attrOffset);
    slotMon->draw_slot = &draw_slot;
    slotMon->slot_old_temperature = 0;
    slotMon->slot_old_power = 0;
    slotMon->bread_old_moisture = 0;
    slotMon->bread_old_toastedness = 0;

    //Set up attributes - Blue x2, Magenta x2, Red x2, Black x2 - all with white paper
    //const unsigned char* attrs = (unsigned char*)"\x79\x79\x7B\x7B\x7A\x7A\x78\x78"; //BRIGHT
    //const unsigned char* attrs = (unsigned char*)"\x39\x39\x3B\x3B\x3A\x3A\x38\x38"; //DIMMER
    // zx_pxy2aaddr(x,y) returns char*
    for (i = 0; i < 2; i++) {
        int x1 = x - 1;
        int y1 = i + y - 1;
        *zx_cxy2aaddr(x1++, y1) = PAPER_WHITE + INK_BLUE;
        *zx_cxy2aaddr(x1++, y1) = PAPER_WHITE + INK_BLUE;
        *zx_cxy2aaddr(x1++, y1) = PAPER_WHITE + INK_MAGENTA;
        *zx_cxy2aaddr(x1++, y1) = PAPER_WHITE + INK_MAGENTA;
        *zx_cxy2aaddr(x1++, y1) = PAPER_WHITE + INK_RED;
        *zx_cxy2aaddr(x1++, y1) = PAPER_WHITE + INK_RED;
        *zx_cxy2aaddr(x1++, y1) = PAPER_WHITE + INK_BLACK;
        *zx_cxy2aaddr(x1, y1) =   PAPER_WHITE + INK_BLACK;
    }
    //Set up tick marks at 'perfect toast' level
    plot_tickmarks(slotMon);
    printf(PRINTAT"%c%c""Slot %d", x, y - 1, slotIndex);
    return slotMon;
}


#ifdef _TEST_SLOT_MONITOR

int main_slot_monitor()
{
  int i = 0, j;
  const char *alphabet = "0123456789ABCDEF";
  unsigned char *attr = (unsigned char *)ATTRSTART;
  SlotMonitor* slotMon;
  //Clear screen
  zx_cls(PAPER_WHITE);

  for (i = 0; i < -256; ++i){
//    printf("%c", alphabet[i % 16]);
    *attr = i;
    attr++;
  }
  printf("\n");
  slotMon = get_slot_monitor(3, 12, 1);//, 12l);
  BreadState brd = {
    175,
    50,
    150,
    150
  };
  SlotState state = {
    1,
    100,
    200,
    15,
    12,
    41,
    0,
    &brd
  };
  for (j = 0; j< 5; ++j) {  
    for (i = state.temperature; i > 0; --i) {
        slotMon->draw_slot(slotMon, &state);
        (state.temperature)--;
        (state.power) -= 2;
        (brd.moisture) += 1;
        (brd.toastedness) -=1;
    } 
    for (; i < 200; ++i) {
        slotMon->draw_slot(slotMon, &state);
        ++(state.temperature);
        (state.power) += 2;
        (brd.moisture) -= 1;
        (brd.toastedness) +=1;
    } 
  }
  return i;
}


int main() {
    return main_slot_monitor();
}

#endif