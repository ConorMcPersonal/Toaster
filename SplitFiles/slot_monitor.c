//////////////////////////////////////////////////////////
//
// Slot monitor component - does to screen updates for a slot
//
//////////////////////////////////////////////////////////

#include <arch/zx.h>

#include <stdio.h>
#include <stdlib.h>
#include <input.h>

#include slot_monitor.h
#include util.h

SlotMonitor* get_slot_monitor(int x, int y) {
    long screenOffset = 32 * 8 * 8 * ((y - 1) / 8) + 32 * 8 * ((y -1) % 8) + x - 1;
    long attrOffset = (y - 1) * 32 + x - 1;
    SlotMonitor slotMon = {
        (unsigned char)x,
        (unsigned char)y,
        void *(SCREENSTART + screenOffset),
        void *(ATTRSTART   + attrOffset),
    };
    //Set up attributes
    unsigned char* attrs = '\x39\x39\x3E\x3E\x3A\x3A\x38\x38';
    memcpy(slotMon.startAttributes, attrs, 7);
    memcpy(slotMon.startAttributes + 32, attrs, 7);
    return *slotMon;
}

void value_to_line(unsigned char* buffer, int byte_count, int value, int range, int min) {
    //How many bits to set
    int bitsToSet = int(MIN((long)range, MAX(0, (long)value - (long)min))
                     * ((long)byte_count * 8) / (long)range);
    int i;
    for (i = 1; i <= byte_count; ++i) {
        if (bitsToSet >= i * 8) {
            buffer[i] = \xFF;
        } elif (bitsToSet <= (i - 1) * 8) {
            buffer[i] = \x00;
        } else {
            buffer[i] = !(bitsToSet - (i * 8))
        }
    }
    return;
}

void draw_slot(SlotMonitor* slotMon, SlotState* slot) {
    unsigned char[] line[8]; 
    int lineInc = 0;
    int i;
    // Draw lines for Slot temp/power, Bread moisture/toastedness
    value_to_line(&line, 7, slot->temperature, 200, 0)
    for (i = 0; i < 3; ++i) {
        memcpy(slotMon->startPixels + lineInc, &line, 7);
        lineInc += 256;
    }
    lineInc += 256;
    value_to_line(&line, 7, slot->power, 200, 0)
    for (i = 0; i < 3; ++i) {
        memcpy(slotMon->startPixels + lineInc, &line, 7);
        lineInc += 256;
    }
    //First pair done
    lineInc = 32;
    value_to_line(&line, 7, slot->bread->moisture, 200, 0)
    for (i = 0; i < 3; ++i) {
        memcpy(slotMon->startPixels + lineInc, &line, 7);
        lineInc += 256;
    }
    lineInc += 256;
    value_to_line(&line, 7, slot->bread->toastedness, 200, 0)
    for (i = 0; i < 3; ++i) {
        memcpy(slotMon->startPixels + lineInc, &line, 7);
        lineInc += 256;
    }
}

