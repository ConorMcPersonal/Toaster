#ifndef _DEFINE_SLOT_MONITOR_H
#define _DEFINE_SLOT_MONITOR_H

#include "game.h"
#include <stdbool.h>

struct BreadStateStruct {
  int               temperature;  
  int               moisture;
  int               toastedness;
  unsigned char     thermalMass;
  unsigned int      thermalAggregation;
};
typedef struct BreadStateStruct BreadState;

// 1.) Forward declaration: Here is the name of the structure
// but member-details are omitted.
struct SlotMonitorStruct;

// 2.) typedef of the structure
// Still no details on the members.
typedef struct SlotMonitorStruct  SlotMonitor;

struct SlotStateStruct {
  int                slotNumber; // Identifier of this slot
  int                temperature;
  int                power;   //Current power level 
  int                xCoord; //screen x-coord
  int                yCoord; //screen y-coord
  unsigned char      thermalMass;
  unsigned int       thermalAggregation;
  BreadState*        bread;
  SlotMonitor*       slotMon;
};
typedef struct SlotStateStruct SlotState;

struct SlotMonitorStruct {
  unsigned char xBase;            // X coordinate on 32x24 screen, top corner is 1, 1
  unsigned char yBase;            // Y coordinate
  unsigned char *startPixels;     //where to start drawing pixels
  unsigned char *startAttributes; //where to write the attributes updates
  int           slot_old_temperature;
  int           slot_old_power;
  int           bread_old_moisture;
  int           bread_old_toastedness;
  void draw_slot(SlotMonitor*, SlotState*); //function to draw the monitor
};

SlotMonitor* get_slot_monitor(unsigned char x, unsigned char y, int slotIndex);

void slot_func(GameComponent* input, GameParameters* params);

#endif