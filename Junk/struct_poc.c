  
#include <arch/zx.h>

#include <stdio.h>
#include <stdlib.h>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define PRINTAT "\x16"

struct GameParameters_struct {
  int       ticks; // How many times has main loop run
  int       score; // What's the score
  int       slices; //How many slices toasted?
  int       game_over_flag; // Zero if game not over
  int       max_toast; // Who has most toastedness - is anything on fire?
  int       message_address;  //Who or what is the message being sent to?
  void*     message; //Pointer to a message structure of some kind - e.g. a command issued to a component
};
typedef struct GameParameters_struct GameParameters;

// 1.) Forward declaration: Here is the name of the structure
// but member-details are omitted.
struct GameComponent_struct;

// 2.) typedef of the structure
// Still no details on the members.
typedef struct GameComponent_struct  GameComponent;

// 3.) The parameter to the function is listed, using the definition
// from step 2.)  (note: Still no details on the members yet)
typedef void (*GameComponentFunc)(GameComponent* input, GameParameters* params);

// 4.) Now we actually detail the members of the structure
struct GameComponent_struct {
  void*                      ptr; //Pointer to any state information for this object - will need to be cast before use
  GameComponentFunc          func;
  GameComponent*             next;
};

struct bread_state_struct {
  int      temperature;
  int      moisture;
  int      toastedness;
};
typedef struct bread_state_struct BreadState;

struct dispatcher_struct {
  int last_time_int;
};
typedef struct dispatcher_struct DispatcherState;

struct slot_state_struct {
  int       slot_number; // Identifier of this slot
  int       temperature;
  int       power;   //Current power level
  int       x_coord; //screen x-coord
  int       y_coord; //screen y-coord
  BreadState* bread;
};
typedef struct slot_state_struct SlotState;

void SlotFunc(GameComponent* input, GameParameters* params) {
  // What does a slot do?
  // Unpack state
  SlotState* state = (SlotState*)input->ptr;

  //Check for a message
  int my_address = 100 + state->slot_number;
  if (params->message_address == 100 + state->slot_number) {
    //Someone sent bread!
    //Take it off the GameParameters object
    params->message_address = 0;
    if (state->bread) {
      //Already have bread in the slot - send straight back to the bread collector (this will be bad toast)
      params->message_address = 20;
    } else {
      //Bread into the slot - let's TOAST!
      state->bread = (BreadState*)params->message;
      params->message = NULL;
    }
  } else if (params->message_address == 200 + state->slot_number) {
    params->message_address = 0;
    //This is a pop-up message
    if (state->bread) {
      params->message_address = 20; // Bread collector
      params->message = (void*)state->bread;
      state->bread = NULL;
    }
  }

  // Now do the toasting thing
  if (state->bread) {
    state->power = 200; // Slot is on
    state->temperature += (state->power - state->temperature) / 10; //temperature may be rising (or steadyish close to 200)
    state->bread->temperature += (state->temperature - state->bread->temperature) / 10; //bread temperature also likely to be rising
    if (state->bread->moisture > 0) {
      //Dry it out before toastuing can commence
      state->bread->moisture -= state->bread->temperature / 10;
    } else {
      //Can't toast at low temperatures!
      state->bread->toastedness += MAX(0, (state->bread->temperature - 90) / 10);
    }
    params->max_toast = MAX(state->bread->toastedness, params->max_toast);
  } else {
    //Cooling - make zero the ambient temp
    state->temperature += (0 - state->temperature) / 10;
  }

  // Now output my state to screen!
  //printf(PRINTAT"%c%c                    ", (char)state->x_coord, (char)state->y_coord); //Clear it
  printf(PRINTAT"%c%cT: %d, P: %d         ", (char)state->x_coord, (char)state->y_coord, state->temperature, state->power);

}

void TickFunc(GameComponent* input, GameParameters* params) {
  params->ticks += 1;
  printf(PRINTAT"\x01\x01" "Time  %d          ", params->ticks);
}

void ToastCollectorFunc(GameComponent* input, GameParameters* params) {
  if (params->message_address == 20) {
    //Someone sent toast!
    BreadState* bread = (BreadState*)params->message;
    params->score += 100 - abs(bread->toastedness - 100); //100 toastedness gets 100 points
    (params->slices) += 1;
    params->message_address = 0;
    params->message = NULL;
    //Give back memory
    free(bread);
  }
  printf(PRINTAT"\x01\x14" "Score %d Slices %d       ", params->score, params->slices);
}

void SmokeAlarmFunc(GameComponent* input, GameParameters* params) {
  if (params->max_toast > 200) {
    zx_border(params->ticks % 8);
  }
}

void NoOpFunc(GameComponent* input, GameParameters* params) {
  printf(PRINTAT"\x01\x12" "Address of ptr %d        ", input->func);
  return;
}

void DispatcherFunc(GameComponent* input, GameParameters* params) {
  DispatcherState* state = (DispatcherState*)input->ptr;
  if (params->slices != state->last_time_int) {
    if (params->message_address == 0) {
      BreadState* new_slice = malloc(sizeof(struct bread_state_struct));
      new_slice->temperature = 0;
      new_slice->moisture = 50 + rand()%50;
      new_slice->toastedness = 0;
      params->message_address = 101;
      params->message = (void*)new_slice;
    }
  }
}

void RandomPopperFunc(GameComponent* input, GameParameters* params) {
  if (params->message_address == 0) {
    DispatcherState* state = (DispatcherState*)input->ptr;
    //Min 20 ticks, then a 1-in 100 chance after that - beware int wraparound
    if ((params->ticks - 20 > state->last_time_int) || (params->ticks < state->last_time_int)) {
      if (rand()%100 == 51) {
        params->message_address = 201;
      }
    }
  }
}


int main()
{
  int i;
  // Initialize the "game" - do the loop backwards
  GameComponent collector = {.func = &ToastCollectorFunc,  //Should this be &, or *, or nothing?
                            .ptr = NULL, 
                            .next = NULL};

  GameComponent smokeAlarm = {.func = &SmokeAlarmFunc,
                              .ptr = NULL,
                              .next = &collector};

  SlotState s1state = {.slot_number = 1,
                        .bread = NULL,
                        .power = 0,
                        .temperature = 0,
                        .x_coord = 3,
                        .y_coord = 5};
  GameComponent slot1 = {.func = &SlotFunc,
                          .ptr = (void*)&s1state,
                          .next = &smokeAlarm};
  
  DispatcherState dispstate = {.last_time_int = -1};
  GameComponent dispatcher = { .func = DispatcherFunc,
                              .ptr = (void*)&dispstate,
                              .next = &slot1};

  DispatcherState popstate = {.last_time_int = -1};
  GameComponent popper = { .func = RandomPopperFunc,
                              .ptr = (void*)&popstate,
                              .next = &dispatcher};

  GameComponent ticker = {.func = TickFunc,
                            .ptr = NULL, 
                            .next = &popper
                            };

  //Now the parameters
  GameParameters params =  { .ticks = 0,
                              .score = 0,
                              .slices = 0,
                              .game_over_flag = 0,
                              .max_toast = 0,
                              .message_address = 0,
                              .message = NULL};

  for (i = 0; i < 1000; i++) {
    GameComponent* comp = &ticker;
    while (comp) {
      //This line crashes the Spectrum - have tried many permutations
      //(comp->func)(comp, &params);
      NoOpFunc(comp, &params);
      comp = comp->next;
    }
    printf(PRINTAT"\x01\x05" "Time  %d          ", i);
  }
  return params.score;
} 