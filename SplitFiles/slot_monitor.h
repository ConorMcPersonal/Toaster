#include game.h

struct BreadStateStruct {
  int      temperature;
  int      moisture;
  int      toastedness;
};
typedef struct BreadStateStruct BreadState;

struct SlotStateStruct {
  int       slot_number; // Identifier of this slot
  int       temperature;
  int       power;   //Current power level
  int       x_coord; //screen x-coord
  int       y_coord; //screen y-coord
  BreadState* bread;
};
typedef struct SlotStateStruct SlotState;

// 2.) typedef of the structure
// Still no details on the members.
typedef struct SlotMonitorStruct  SlotMonitor;

struct SlotMonitorStruct {
  unsigned char xBase;            // X coordinate on 32x24 screen, top corner is 1, 1
  unsigned char yBase;            // Y coordinate
  unsigned char *startPixels;     //where to start drawing pixels
  unsigned char *startAttributes; //where to write the attributes updates
  void *draw_slot(SlotMonitor*, SlotState*); //function to draw the monitor
};