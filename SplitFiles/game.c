#include <input.h>
#include <stdio.h>
#include <stdlib.h>
#include <arch/zx.h>
#include <sound.h>

#include "game.h"
#include "util.h"
#include "slot_monitor.h"
#include "control.h"

// Compile with:
// zcc +zx -vn -startup=1 -clib=sdcc_iy -D_TEST_GAME slot.c slot_monitor.c game.c control.c -o game -create-app

void draw_tick_line(const unsigned int tick)
{
  const int increment = MAX_TICKS / 256;
  const int ticks_left = MAX_TICKS - tick;
  unsigned char point, previous_point;
  previous_point = (ticks_left + 1) / increment;
  point = ticks_left / increment;
  if (previous_point != point) {
    // zero out lower bit
    int old_byte = *zx_pxy2saddr(previous_point, 191);
    *zx_pxy2saddr(previous_point, 191) = (old_byte << 1);
  }
}

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
  draw_tick_line(params->ticks);
  //printf(PRINTAT"\x01\x01" "Time  %d          ", params->ticks);
 
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
    params->messageSourceAddress = NULL;
    //Give back memory
    free(bread);
  }
//  printf(PRINTAT"\x01\x14" "Score %d Slices %d       ", params->score, params->slices);
}

void smoke_alarm_func(GameComponent* input, GameParameters* params) {
  input;
  if (params->maxToast > 200) {
    if (params->ticks % 2 == 0) {
      bit_fx(BFX_BEEP);
    }
    zx_border(params->ticks % 8);
  }
  params->maxToast = 0; //Reset for next loop
}

#ifdef _TEST_GAME
int main()
{
  int i;
  //Clear screen
  zx_cls(PAPER_WHITE);
  bit_fx(BFX_KLAXON);

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
  
  /*
  if (0) {
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
  } else {
    */
    ControlBuffer buff = {
        0,
        0,
        NULL
    };
    initialise_control_buffer(&buff);
    GameComponent ctrl = {
        &buff,
        &command_entry_func,
        &slot1
    };


    GameComponent ticker = {
                            (void*)NULL, //ptr
                            &tick_func, //func
                            &ctrl //next
                            };
  //}

  //Now the parameters
  GameParameters params =  { 0, //.ticks = 
                              0,//.score =
                              0,//.slices = 
                              0,//.game_over_flag = 
                              0,//.max_toast = 
                              0,//.message_address = 
                              (void*)NULL//.message = 
                           };

  // draw starting time line
  for (i = 0; i < 256; i++) {
    *zx_pxy2saddr(i, 191) |= zx_px2bitmask(i);
  }

  for (i = 0; i < MAX_TICKS; i++) {
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
  bit_beepfx(BEEPFX_AWW);
  //we malloc this so free it
  free(buff.buffer);
  return params.score;
} 
#endif
