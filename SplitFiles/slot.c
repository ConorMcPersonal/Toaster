//////////////////////////////////////////////////////////
//
// Slot component - takes care of a single toaster slot
//
//////////////////////////////////////////////////////////

// Can be compiled with:
// zcc +zx -vn -startup=1 -clib=sdcc_iy -D_TEST_SLOT slot.c slot_monitor.c music.c -o slot -create-app

#include <arch/zx.h>

#include <stdio.h>
#include <stdlib.h>
#include <sound.h>
#include <stdbool.h>

#include "slot_monitor.h"
#include "util.h"
#include "game.h"
#include "music.h"

#define TMAX 200

void slot_func(GameComponent* input, GameParameters* params) {
  // What does a slot do?
  // Unpack state
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
      state->power = TMAX;
      params->message = NULL;
      //bit_beepfx(BEEPFX_CLANG);
      params->effect = TUNE_EFFECT_DOWN; 
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
      params->effect = TUNE_EFFECT_UP; 
      //bit_fx(BFX_EXPLOSION);
    }
  }

  // Now do the toasting thing
  // Update the tick-wise integration of observed temperature differences
  state->thermalAggregation += (state->power - state->temperature);
  // Calculate new temperature from that
  state->temperature = state->thermalAggregation / state->thermalMass;
#ifdef _TEST_SLOT
    printf(PRINTAT"\x02\x0B""%d %d %d", state->thermalAggregation, state->temperature, state->power);
#endif

  if (state->bread) {
    state->bread->thermalAggregation += (state->temperature - state->bread->temperature); //bread temperature also likely to be rising
    state->bread->temperature = state->bread->thermalAggregation / state->bread->thermalMass;
    if (state->bread->moisture > 0) {
      //Dry it out before toasting can commence
      state->bread->moisture -= MIN(state->bread->moisture, (state->bread->temperature / 10));
    } else {
      //Can't toast at low temperatures!
      state->bread->toastedness += MAX(0, ((state->bread->temperature - 90) / 10));
    }
#ifdef _TEST_SLOT
    printf(PRINTAT"\x02\x0C""%d %d %d %d", state->bread->moisture, state->bread->temperature, state->bread->toastedness, state->bread->thermalAggregation);
#endif
    params->maxToast = MAX(state->bread->toastedness, params->maxToast);
  }
  state->slotMon->draw_slot(state->slotMon, state);


}

SlotState* get_slot(unsigned char x, unsigned char y, int slotIndex, int thermalMass) {
    SlotState* slot = malloc(sizeof(SlotState));
    slot->slotNumber = slotIndex;
    slot->bread = NULL;
    slot->power = 0;
    slot->temperature = 0;
    slot->thermalAggregation = 0;
    slot->thermalMass = thermalMass;
    slot->xCoord = x;
    slot->yCoord = y;
    slot->slotMon = get_slot_monitor(x, y, slotIndex);
    return slot;
}

int slot_main() {

  int i;

  SlotState* s1state = get_slot(3, 5, 1, 41);

  GameComponent comp = {
    s1state,
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
    0,
    (unsigned char)164,   //thermalMass - high for bread
    0                     //thermalAggregation
  };


  s1state->power = 0;
  for (i = 0; i < 200; ++i) {
    slot_func(&comp, &params);
    s1state->power += 1;
  }

  s1state->bread = &brd;
  for (i = 0; i < 500; ++i) {
    slot_func(&comp, &params);
  }

  return 1;
 
}

#ifdef _TEST_SLOT
  int main() {
    return slot_main();
  }
#endif