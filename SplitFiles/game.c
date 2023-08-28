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

int wait_for_a_key(GameComponent* input, GameParameters* params) {
    unsigned char c;
    int rando = 0;
    while (in_inkey() == 0) {
      rando++;
    }
    c = in_inkey();
    in_wait_nokey();
    return rando;

    //printf(PRINTAT"\x01\x12""Key pressed is %c (0x%02X)\n", c, c);
}

void tick_func(GameComponent* input, GameParameters* params) {
  input;
  params->ticks += 1;
  draw_tick_line(params->ticks);
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

int main_game()
{
    int i;
    //Clear screen
    zx_cls(PAPER_WHITE);
    printf(PRINTAT "\x01\x01" \
      "Welcome to the " "\x10\x32H\x10\x34o\x10\x36t\x10\x35" "e\x10\x31l " "\x10\x32" "E" \
      "\x10\x34" "x" "\x10\x36" "c" "\x10\x35" "e" "\x10\x31" "s\x10\x33s\x10\x30.");
    printf(PRINTAT "\x01\x03" "We loved your job application to");
    printf(PRINTAT "\x01\x04" "be our new breakfast Toast Host,");
    printf(PRINTAT "\x01\x05" "but now stuff gets really tasty.");
    printf(PRINTAT "\x01\x07" "\x12\x31\x10\x32" "Can you take the heat?\x12\x30\x10\x30");
    printf(PRINTAT "\x05\x0B" "Press any key to start");
    int rando = wait_for_a_key(NULL, NULL);
    // get a random seed based on frame count
    srand(rando);
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
      get_slot_monitor(3, 5, 1) //slot monitor
    };
    GameComponent slot1 = {
      (void*)&s1state, //ptr
      &slot_func, //func
      &smokeAlarm //next
    };

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
    
    printf(PRINTAT"%c%c""%-12s", 18, 12, "Commands");
    printf(PRINTAT"%c%c""%-12s", 18, 3,  "Order Queue");

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


#ifdef _TEST_GAME
int main()
{
    return main_game();
} 
#endif
