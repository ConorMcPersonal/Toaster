//////////////////////////////////////////////////////////
//
// Slot monitor component - does to screen updates for a slot
//
//////////////////////////////////////////////////////////

// Can be compiled with:
// zcc +zx -vn -startup=0 -clib=sdcc_iy -D_TEST_SLOT_MONITOR slot_monitor.c -o slot_monitor -create-app

#include <arch/zx.h>

#include <stdio.h>
#include <stdlib.h>
#include <input.h>
#include <string.h>
#include <stdbool.h>

#include "slot_monitor.h"
#include "util.h"


int plot_value(const unsigned int slot, const int value, const unsigned int task,
    const int range, const int max, bool increment, const int old_value)
{
    if (value <= max) {
        const unsigned int offset = slot * 4 * 8 + task * 4;
        int i, j, x;
        const unsigned int step_value = max / range;
        if (increment) {
            x = 16 + old_value / step_value;
            for (i = old_value; i < value; i+= step_value) {
                for (j = 0; j < 3; j++) {
                    if (x > 16) { // avoid negative charting
                        *zx_pxy2saddr(x, offset + j) |= zx_px2bitmask(x);
                    }
                }
                x++;
            }
        }
        else {
            x = 16 + value / step_value;
            for (i = value; i < old_value; i+= step_value) {
                for (j = 0; j < 3; j++) {
                    unsigned char mask_out = ~zx_px2bitmask(x);
                    *zx_pxy2saddr(x, offset + j) &= mask_out;
                }
                x++;
            }
        }
    }
    return value;
}

void draw_slot(SlotMonitor* slotMon, SlotState* slot, bool increment) {
    static bool wipe_out = false;
    slot->old_temperature = plot_value(slot->slotNumber, slot->temperature, 0, 64,
        256, increment, slot->old_temperature);
    slot->old_power = plot_value(slot->slotNumber, slot->power, 1, 64,
        256, increment, slot->old_power);
    if (slot->bread) {
        wipe_out = false;
        slot->bread->old_moisture = plot_value(slot->slotNumber, slot->bread->moisture, 2, 64,
            256, increment, slot->bread->old_moisture);
        slot->bread->old_toastedness = plot_value(slot->slotNumber, slot->bread->toastedness, 3, 64,
            256, increment, slot->bread->old_toastedness);
    } else if (wipe_out == false){
        //no bread so wipe it - once
        wipe_out = true;
        plot_value(slot->slotNumber, 0, 2, 64, 256, false, 256);
        plot_value(slot->slotNumber, 0, 3, 64, 256, false, 256);
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
        *zx_cxy2aaddr(x1, y1) = PAPER_WHITE + INK_BLACK;
    }
    printf(PRINTAT"%c%c""Slot %d", x, y - 1, slotIndex);
    return slotMon;
}

int main_slot_monitor()
{
  int i = 0;
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
    175,
    50,
    50,
    150,
    150
  };
  SlotState state = {
    0,
    100,
    100,
    200,
    200,
    15,
    12,
    &brd
  };
  for (i = state.temperature; i > 0; --i) {
 //   slotMon->draw_slot(slotMon, &state, false);
 //   (state.temperature)--;
 //   (state.power) -= 2;
 //   (brd.moisture) += 1;
 //   (brd.toastedness) -=1;
  } 
  return i;
}

#ifdef _TEST_SLOT_MONITOR

int main() {
    return main_slot_monitor();
}

#endif