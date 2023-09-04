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

#include "slot_monitor.h"
#include "util.h"

const unsigned char full[]  = "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF";
const unsigned char empty[] = "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";

void draw_bar(char* startAddr, int byteCount, int value, int range, int min) {
    // How many bits need to be filled?
    int bitsToSet = (MIN(range, MAX(0, value - min))
                     * (byteCount * 8)) / range;
    int fullBytes = MIN(bitsToSet / 8, byteCount - 1);
    int emptyBytes = byteCount - 1 - fullBytes;
    unsigned char extra = 0xFF << (8 - (bitsToSet - fullBytes * 8));
    //printf(PRINTAT"\x05\x0B""%d %d %d %d    ", bitsToSet, fullBytes, emptyBytes, extra);
    memcpy(startAddr, &full, fullBytes);
    *(startAddr + fullBytes) = extra;
    memcpy(startAddr + fullBytes + 1, &empty, emptyBytes);
    //Now copy down
    for (int i = 1; i < 3; ++i) {
        memcpy(startAddr + 256 * i, startAddr, byteCount);
    }
}

void draw_slot(SlotMonitor* slotMon, SlotState* slot) {
    // Draw lines for Slot temp/power, Bread moisture/toastedness
    draw_bar(zx_pxy2saddr(slotMon->xBase, slotMon->yBase), 8, slot->temperature, 200, 0);
    draw_bar(zx_pxy2saddr(slotMon->xBase, slotMon->yBase + 4), 8, slot->power, 200, 0);

    //First pair done - on to bread
    if (slot->bread) {
        draw_bar(zx_pxy2saddr(slotMon->xBase, slotMon->yBase + 8), 8, slot->bread->moisture, 200, 0);
        draw_bar(zx_pxy2saddr(slotMon->xBase, slotMon->yBase + 12), 8, slot->bread->toastedness, 200, 0);
    } else {
        draw_bar(zx_pxy2saddr(slotMon->xBase, slotMon->yBase + 8), 8, 0, 200, 0);
        draw_bar(zx_pxy2saddr(slotMon->xBase, slotMon->yBase + 12), 8, 0, 200, 0);
    }
}

SlotMonitor* get_slot_monitor(unsigned char x, unsigned char y, int slotIndex) {
    //Use x and y directly once only
    printf(PRINTAT"%c%c""Slot %d", x, y - 1, slotIndex);

    SlotMonitor* slotMon = malloc(sizeof(SlotMonitor));
    slotMon->xBase = (x - 1) * 8;
    slotMon->yBase = (y - 1) * 8;
    slotMon->draw_slot = &draw_slot;

    const unsigned char* attrs = (unsigned char*)"\x39\x39\x3B\x3B\x3A\x3A\x38\x38"; //DIMMER
    memcpy(zx_cxy2aaddr(x - 1, y - 1), attrs, 8);
    memcpy(zx_cxy2aaddr(x - 1, y    ), attrs, 8); //Next row down
    return slotMon;
}

int main_slot_monitor()
{
  zx_cls(PAPER_WHITE);
  char* startAddr = (char*)SCREENSTART + 256;
  for (int j = 0; j < 10; ++j) {
    int i;
    for (i = 0; i < 250; ++i) {
        draw_bar(startAddr, 16, i, 200, 0);
        //for (int j = 0; j < 100; ++j) {}
    } 
    for (i; i > -10; --i) {
        draw_bar(startAddr, 16, i, 200, 0);
        //for (int j = 0; j < 100; ++j) {}
    } 
  }

  int i = 0;
  const char *alphabet = "0123456789ABCDEF";
  unsigned char *attr = (unsigned char *)ATTRSTART;
  SlotMonitor* slotMon;
  //Clear screen
  zx_cls(PAPER_WHITE);

  for (i = 0; i < 256; ++i){
    printf("%c", alphabet[i % 16]);
    *attr = i;
    attr++;
  }
  printf("\n");
  slotMon = get_slot_monitor(3, 17, 1);//, 12l);
  BreadState brd = {
    175,
    50,
    150
  };
  SlotState state = {
    0,
    100,
    200,
    15,
    12,
    &brd
  };
  for (i = state.temperature; i > 0; --i) {
    slotMon->draw_slot(slotMon, &state);
    (state.temperature)--;
    (state.power) -= 2;
    (brd.moisture) += 1;
    (brd.toastedness) -=1;
  } 
  return i;
}

#ifdef _TEST_SLOT_MONITOR

int main() {
    return main_slot_monitor();
}

#endif