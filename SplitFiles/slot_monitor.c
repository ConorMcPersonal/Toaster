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

int dirty_graphics = 0;

// This function writes the bytes for the line into buffer
void value_to_line(unsigned char* buffer, int byte_count, int value, int range, int min) {
    //How many bits to set
    int bitsToSet = (int)(MIN((long)range, MAX(0, (long)value - (long)min))
                     * ((long)byte_count * 8) / (long)range);
    int i;
    for (i = 0; i < byte_count; ++i) {
        if (bitsToSet >= (i + 1) * 8) {
            buffer[i] = 0xFF;
        } else if (bitsToSet <= i * 8) {
            buffer[i] = 0x00;
        } else {
            buffer[i] = 0xFF << (8 - (bitsToSet - i * 8));
        }
    }
    return;
}

int line_to_mem(void* targetAddr, void* source, long size, long increment) {
    int i;
    int offset = 0;
    unsigned char *hitHere = targetAddr;
    for (i = 0; i < 3; ++i) {
        memcpy(hitHere + offset, source, size);
        offset += increment;
    }
    return offset;
}

void draw_value(const unsigned int slot, const int value, const unsigned int task,
    const int range, const int max)
{
    if (value <= max) {
        const unsigned int offset = slot * 4 * 8 + task * 3;
        int i, j, x;
        const unsigned int step_value = max / range;
        x = 16;
        for (i = 0; i < value; i+= step_value) {
            for (j = 0; j < 2; j++) {
                *zx_pxy2saddr(x, offset + j) |= zx_px2bitmask(x);
            }
            x++;
        }
        for (; i < max; i+= step_value) {
            for (j = 0; j < 2; j++) {
               unsigned char pByte = *zx_pxy2saddr(x, offset + j);
               if (pByte != 0) {
                    unsigned char mask_out = ~zx_px2bitmask(x);
                    *zx_pxy2saddr(x, offset + j) &= mask_out;
                }
                //*zx_pxy2saddr(x, offset + j) ^= zx_px2bitmask(x);
            }
            x++;
        }
    }
}

void draw_slot(SlotMonitor* slotMon, SlotState* slot) {
    draw_value(slot->slotNumber, slot->temperature, 0, 64, 256);
    draw_value(slot->slotNumber, slot->power, 1, 64, 256);
    draw_value(slot->slotNumber, slot->bread->moisture, 2, 64, 256);
    draw_value(slot->slotNumber, slot->bread->toastedness, 3, 64, 256);

/*  unsigned char line[8]; 
    int lineInc = 0;
    // Draw lines for Slot temp/power, Bread moisture/toastedness
    value_to_line(&(line[0]), 7, slot->temperature, 200, 0);
    lineInc += line_to_mem(slotMon->startPixels + lineInc, &line, 7, 256);

    lineInc += 256;
    value_to_line(&(line[0]), 7, slot->power, 200, 0);
    lineInc += line_to_mem(slotMon->startPixels + lineInc, &line, 7, 256);

    //First pair done - on to bread
    lineInc = 32;
    if (slot->bread) {
        value_to_line(&(line[0]), 7, slot->bread->moisture, 200, 0);
    } else {
        value_to_line(&(line[0]), 7, 0, 200, 0);
    }
    lineInc += line_to_mem(slotMon->startPixels + lineInc, &line, 7, 256);

    lineInc += 256;
    if (slot->bread) {
        value_to_line(&(line[0]), 7, slot->bread->toastedness, 200, 0);
    } else {
        value_to_line(&(line[0]), 7, 0, 200, 0);
    }
    lineInc += line_to_mem(slotMon->startPixels + lineInc, &line, 7, 256);
*/
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

    //printf("%d ", screenOffset);
    //printf("%d ", attrOffset);
    //printf("%d ", x);
    //printf("%d\n", y);
    SlotMonitor* slotMon = malloc(sizeof(SlotMonitor));
    slotMon->xBase = x;
    slotMon->yBase = y;
    slotMon->startPixels =   (void *)(SCREENSTART + screenOffset);
    slotMon->startAttributes = (void *)(ATTRSTART + attrOffset);
    slotMon->draw_slot = &draw_slot;
    //printf("%x   ", slotMon->startPixels);
    //printf("%x\n", slotMon->startAttributes);

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