#include <input.h>
#include <stdio.h>
#include <stdlib.h>
#include <arch/zx.h>

#include "game.h"
#include "util.h"
#include "slot_monitor.h"
#include "control.h"

// Compile with:
// zcc +zx -vn -startup=1 -clib=sdcc_iy -D_TEST_GAME slot.c slot_monitor.c game.c -o game -create-app

void wait_key(GameComponent* input, GameParameters* params) {
    unsigned char c;
    in_wait_key();
    c = in_inkey();
    in_wait_nokey();

    printf(PRINTAT"\x01\x12""Key pressed is %c (0x%02X)\n", c, c);
}

void tick_func(GameComponent* input, GameParameters* params) {
  input;
  params->ticks += 1;
  printf(PRINTAT"\x01\x01" "Time  %d          ", params->ticks);
}

void toast_collector_func(GameComponent* input, GameParameters* params) {
  input;
  if (params->messageAddress == 20) {
    //Someone sent toast!
     BreadState* bread = (BreadState*)params->message;
    params->score += 100 - abs(bread->toastedness - 100); //100 toastedness gets 100 points
    (params->slices) += 1;
    params->messageAddress = 0;
    params->message = NULL;
    //Give back memory
    free(bread);
  }
  printf(PRINTAT"\x01\x14" "Score %d Slices %d       ", params->score, params->slices);
}

void smoke_alarm_func(GameComponent* input, GameParameters* params) {
  input;
  if (params->maxToast > 200) {
    zx_border(params->ticks % 8);
  }
  params->maxToast = 0; //Reset for next loop
}

void dispatcher_func(GameComponent* input, GameParameters* params) {
  DispatcherState* state = (DispatcherState*)input->ptr;
  if (params->slices != state->last_time_int) {
    if (params->messageAddress == 0) {//No message on the bus at this time
      BreadState* new_slice = malloc(sizeof(struct BreadStateStruct));
      new_slice->temperature = 0;
      new_slice->moisture = 50 + rand()%50;
      new_slice->toastedness = 0;
      params->messageAddress = 101;
      params->message = (void*)new_slice;
    }
  }
  state->last_time_int = params->slices;
}

void popper_func(GameComponent* input, GameParameters* params) {
  int p_down;
  if (params->messageAddress == 0) {
    DispatcherState* state = (DispatcherState*)input->ptr;

    p_down = in_key_pressed( IN_KEY_SCANCODE_p );
    if (p_down) {
      if (state->last_time_int) {
        //Nothing to do
      } else {
        //New P action
        params->messageAddress = 201;
      }
    }
    state->last_time_int = p_down;
  }
}

void send_toast_func(GameComponent* input, GameParameters* params) {
  int t_down;
  if (params->messageAddress == 0) {
    DispatcherState* state = (DispatcherState*)input->ptr;

    t_down = in_key_pressed( IN_KEY_SCANCODE_t );
    if (t_down) {
      if (state->last_time_int) {
        //Nothing to do
      } else {
        //New T action
        BreadState* new_slice = malloc(sizeof(struct BreadStateStruct));
        new_slice->temperature = 0;
        new_slice->moisture = 50 + rand()%50;
        new_slice->toastedness = 0;
        params->messageAddress = 101;
        params->message = (void*)new_slice;
      }
    }
    state->last_time_int = t_down;
  }
}

#ifdef _TEST_GAME
int main()
{
  int i;
  //Clear screen
  zx_cls(PAPER_WHITE);
  // Initialize the "game" - do the loop backwards
  GameComponent collector = {
    (void*)NULL, //ptr
    &toast_collector_func,  //func
    (GameComponent *)NULL //next - this is end of the line
  };

  GameComponent smokeAlarm = {
    (void*)NULL, //ptr
    &smoke_alarm_func, //func
    &collector //next
  };

  SlotState s1state = {
    1, //int       slot_number; // Identifier of this slot
    0, //int       temperature;
    0, //int       power;   //Current power level
    3, //int       x_coord; //screen x-coord
    5, //int       y_coord; //screen y-coord
    (BreadState*) NULL, //bread
    get_slot_monitor(3, 5) //slot monitor
  };
  GameComponent slot1 = {
    (void*)&s1state, //ptr
    &slot_func, //func
    &smokeAlarm //next
  };
  
  DispatcherState dispState = {-1}; //.last_time_int
  GameComponent dispatcher = {
    (void*)&dispState, //ptr
    &send_toast_func, //func
    &slot1 //next
  };

  DispatcherState popState = {-1}; //.last_time_int
  GameComponent popper = { 
    (void*)&popState, // ptr
    &popper_func, //func
    &dispatcher //next
  };

   GameComponent ticker = {
                          (void*)NULL, //ptr
                          &tick_func, //func
                          &popper //next
                          };

  //Now the parameters
  GameParameters params =  { 0, //.ticks = 
                              0,//.score =
                              0,//.slices = 
                              0,//.game_over_flag = 
                              0,//.max_toast = 
                              0,//.message_address = 
                              (void*)NULL//.message = 
                           };


  for (i = 0; i < 1000; i++) {
    GameComponent* comp = &ticker;
    //ticker.func(comp, &params);
    while (comp) {
      comp->func(comp, &params);
      //wait_key(comp, &params);
      //printf(PRINTAT "\x01\x02" "MsgAddr = %d    ", params.message_address);
      //WaitKey(comp, &params);
      comp = comp->next;
    }
  }
  printf(PRINTAT "\x01\x0B" "Final score %d ", (params.slices * params.score));
  return params.score;
} 
#endif
