//////////////////////////////////////////////////////////
//
// Slot component - takes care of a single toaster slot
//
//////////////////////////////////////////////////////////

// Can be compiled with:
// zcc +zx -vn -startup=0 -clib=sdcc_iy -D_TEST_SLOT slot.c slot_monitor.c game.c -o slot -create-app

#include <arch/zx.h>

#include <stdio.h>
#include <stdlib.h>
#include <sound.h>

#include "slot_monitor.h"
#include "util.h"
#include "game.h"

void slot_func(GameComponent* input, GameParameters* params) {
  // What does a slot do?
  // Unpack state
  int breadStateOut = 0;
  SlotState* state = (SlotState*)(input->ptr);

  //Check for a message
  int myAddress = 100 + state->slotNumber;
  if (params->messageAddress == 100 + state->slotNumber) {
    //Someone sent bread!
    //Take it off the GameParameters object
    params->messageAddress = 0;
    if (state->bread) {
      //Already have bread in the slot - send straight back to the bread collector (this will be bad toast)
      params->messageAddress = 20;
    } else {
      //Bread into the slot - let's TOAST!
      state->bread = (BreadState*)params->message;
      params->message = NULL;
      bit_beepfx(BEEPFX_CLANG);
    }
  } else if (params->messageAddress == 200 + state->slotNumber) {
    params->messageAddress = 0;
    //This is a pop-up message
    if (state->bread) {
      params->messageAddress = 20; // Bread collector
      params->message = (void*)state->bread;
      params->messageSourceAddress = (void *)state;
      state->bread = NULL;
      state->power = 0;
      bit_fx(BFX_EXPLOSION);
    }
  }

  // Now do the toasting thing
  if (state->bread) {
    breadStateOut = 1;
    state->power = 200; // Slot is on
    state->temperature += ((state->power - state->temperature) / 10); //temperature may be rising (or steadyish close to 200)
    state->bread->temperature += ((state->temperature - state->bread->temperature) / 10); //bread temperature also likely to be rising
    if (state->bread->moisture > 0) {
      //Dry it out before toastuing can commence
      state->bread->moisture -= (state->bread->temperature / 10);
    } else {
      //Can't toast at low temperatures!
      state->bread->toastedness += MAX(0, ((state->bread->temperature - 90) / 10));
    }
    params->maxToast = MAX(state->bread->toastedness, params->maxToast);
  } else {
    //Cooling - make zero the ambient temp
    state->temperature += ((0 - state->temperature) / 10);
  }

  state->slotMon->draw_slot(state->slotMon, state);

}

#ifdef _TEST_SLOT

int main() {

  int i;
  SlotMonitor *testMon = get_slot_monitor(3, 5);

  SlotState s1state = {
    1, //int       slot_number; // Identifier of this slot
    0, //int       temperature;
    0, //int       power;   //Current power level
    testMon->xBase, //int       x_coord; //screen x-coord
    testMon->yBase, //int       y_coord; //screen y-coord
    (BreadState*) NULL, //bread;
    (SlotMonitor *)testMon
  };

  GameComponent comp = {
    &s1state,
    &slot_func,
    NULL
  };

  GameParameters params = {
    0,
    0,
    0,
    0,
    100,
    0,
    NULL
  };

  BreadState brd = {
    0,
    100,
    0
  };


  s1state.power = 0;
  for (i = 0; i < 50; ++i) {
    slot_func(&comp, &params);
    s1state.power += 1;
  }

  s1state.bread = &brd;
  for (i = 0; i < 100; ++i) {
    slot_func(&comp, &params);
  }

  return 1;
 
}
#endif