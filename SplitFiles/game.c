#include <input.h>
#include <stdio.h>
#include <stdlib.h>
#include <arch/zx.h>
#include <sound.h>

#include "game.h"
#include "util.h"
#include "slot_monitor.h"
#include "control.h"
#include "music.h"
#include "bread.h"
#include "customer.h"
#include "numbers.h"
#include "face.h"

// Compile with:
// zcc +zx -vn -startup=1 -clib=sdcc_iy -D_TEST_GAME slot.c slot_monitor.c game.c control.c music.c util.c bread.c customer.c -o game -create-app

static int hour = 6;
static int min = 45;

void update_clock() {
  int start_x = 1;
  int start_y = 1;
  min = (min + 15)%60;
  if (min == 0) {
    hour++;
  }
  screenTime(start_x, start_y, hour, min);
}

void draw_tick_line(const unsigned int tick)
{
  const int increment = MAX_TICKS / 256;
  const int ticks_left = MAX_TICKS - tick;
  unsigned char point, previous_point;
  static int tick_progress = MAX_TICKS;
  previous_point = (ticks_left + 1) / increment;
  point = ticks_left / increment;
  if (previous_point != point) {
    // zero out lower bit
    int old_byte = *zx_pxy2saddr(previous_point, 191);
    *zx_pxy2saddr(previous_point, 191) = (old_byte << 1);
    int next_progress = ticks_left / TICK_DIV;
    if (next_progress != tick_progress) {
      update_clock();
      tick_progress = next_progress;
    }
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
}

void tick_func(GameComponent* input, GameParameters* params) { //Now a scoreboard too
  int last_score = (int)input->ptr;
  if (params->score != last_score) {
    screenNumber(21, 1, params->score);
  //  printf(PRINTAT"\x19\x01""%6d", params->score);
    input->ptr = (void *)params->score;
  }
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
}

void smoke_alarm_func(GameComponent* input, GameParameters* params) {
  input;
  if (params->maxToast > BURNT_TOAST) {
    params->effect = TUNE_EFFECT;
    zx_border(params->ticks % 8);
  }
  params->maxToast = 0; //Reset for next loop
}

void game_do_day(const unsigned int day)
{
  int i, j, emo;
  emo = 0;
  /* Fill screen with faces */
  for (i = 1; i < 24; i+=2) {
    for (j = 1; j < 32; j+=2) {
     screenEmotion(j, i, emo);
     emo = (emo + 1) % 4; 
    }
  }
  /* clear centre of screen */
  screenClear(16, 12);
  screenClear(17, 12);
  screenClear(16, 13);
  screenClear(17, 13);
  draw_number(16, 12, day);
  wait_for_a_key(NULL, NULL);
}

int main_game()
{
    start_frame_count();
    int i, j;
    //Clear screen
    zx_cls(PAPER_WHITE);
    printf(PRINTAT "\x01\x01" \
      "Welcome to the " "\x10\x32H\x10\x34o\x10\x36t\x10\x35" "e\x10\x31l " "\x10\x32" "E" \
      "\x10\x34" "x" "\x10\x36" "c" "\x10\x35" "e" "\x10\x31" "s\x10\x33s\x10\x30.");
    printf(PRINTAT "\x01\x03" "We loved your job application to");
    printf(PRINTAT "\x01\x04" "be our new breakfast Toast Host,");
    printf(PRINTAT "\x01\x05" "but now stuff gets really tasty.");
    printf(PRINTAT "\x01\x07" "\x12\x31\x10\x32" "Can you stand the heat?\x12\x30\x10\x30");
    printf(PRINTAT "\x05\x0B" "Press any key to start");

    int rando = wait_for_a_key(NULL, NULL);
    // get a random seed based on frame count
    srand(rando);
    zx_cls(PAPER_WHITE);
    game_do_day(1);
    zx_cls(PAPER_WHITE);
    for (i = 0; i < 2; i++) {
      for (j = 0; j < 32; j++) {
        *zx_cxy2aaddr(j, i) = INK_YELLOW | PAPER_BLACK;
      }
    }
    for (j = 0; j < 32; j++) {
      *zx_cxy2aaddr(j, 23) = INK_RED | PAPER_BLACK;
    }

    BreadBin* newBreadBin = get_bread_bin();
  
  // *******************************************************
  // Music set-up
  // *******************************************************

    //Three-voice music player
    MusicPlayer* music_player = get_music_player(2);
    //music_player->add_music(music_player, TUNE_DRUM, 2);
    music_player->add_music(music_player, TUNE_RICKROLL, 1);
    music_player->add_music(music_player, TUNE_EFFECT_BEEP, 0);
    // ****************************************************
    //  Music set-up ends
    // *******************************************************

    Customer root = {0, NULL, NULL};
    CustomerBase base = {&root, 0};

    GameComponent collector = {
        &base,
        &customer_func,
        NULL
    };

    GameComponent smokeAlarm = {
      (void*)NULL, //ptr
      &smoke_alarm_func, //func
      &collector //next
    };

    SlotState* s1state = get_slot(3, 20, 4, 41);
    GameComponent slot1 = {
      (void*)s1state, //ptr
      &slot_func, //func
      &smokeAlarm //next
    };

    SlotState* s2state = get_slot(3, 15, 3, 41);
    GameComponent slot2 = {
      (void*)s2state, //ptr
      &slot_func, //func
      &slot1 //next
    };

    SlotState* s3state = get_slot(3, 10, 2, 41);
    GameComponent slot3 = {
      (void*)s3state, //ptr
      &slot_func, //func
      &slot2 //next
    };

    SlotState* s4state = get_slot(3, 5, 1, 41);
    GameComponent slot4 = {
      (void*)s4state, //ptr
      &slot_func, //func
      &slot3 //next
    };

    ControlBuffer buff = {
        0,
        0,
        0,
        NULL
    };
    initialise_control_buffer(&buff);
    GameComponent ctrl = {
        &buff,
        &command_entry_func_instant,
        &slot4
    };


    GameComponent ticker = {
                            (void *)SCREENSTART, //ptr = last_score - needs to be different to opening score
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
                                0, //message_address
                                (void*)NULL,//.message 
                                (void*)NULL,//.messageSourceAddress = 
                                0, // effect
                                newBreadBin, //breadBin
                                1000
                            };

    // draw starting time line
    for (i = 0; i < 256; i++) {
      *zx_pxy2saddr(i, 191) |= zx_px2bitmask(i);
    }
    
    printf(PRINTAT"%c%c""%-12s", 18, 12, "Commands");
    printf(PRINTAT"%c%c""%-12s", 18, 3,  "Order Queue");
    //bread_restack(newBreadBin);


    for (i = 0; i < MAX_TICKS; i++) {
      int last_frame_count = G_frames;

      GameComponent* comp = &ticker;
      //ticker.func(comp, &params);
      while (comp) {
        comp->func(comp, &params);
        comp = comp->next;
        music_player->play(music_player);
      }
      //Check for a new sound effect
      if (params.effect != NULL) {
        music_player->add_music_if_different(music_player, params.effect, 0);
        params.effect = NULL;
      }
      //Min one frame per loop
      while (G_frames == last_frame_count) {}
      if (params.gameOverFlag) {
        i = MAX_TICKS + 1;
      }
    }
    if (i == MAX_TICKS) {
      screenTime(1, 1, 11, 0);
    }
    screenNumber(21, 1, params.score);
    printf(PRINTAT "\x01\x18" "Final score %d ", (params.score));
    if (params.messageAddress == 999) {
      printf(PRINTAT "\x01\x0C" "%s", (char *)params.message);
    }
    bit_beepfx(BEEPFX_AWW);
    //we malloc this so free it
    free(buff.buffer);
    while (1) {}
    return params.score;
} 


#ifdef _TEST_GAME
int main()
{
    return main_game();
} 
#endif
