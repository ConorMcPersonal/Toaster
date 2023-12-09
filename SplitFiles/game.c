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
#include "attributes_text.h"

// Compile with:
// zcc +zx -vn -startup=1 -clib=sdcc_iy -D_TEST_GAME slot.c slot_monitor.c game.c control.c music.c util.c bread.c customer.c base.c -o game -create-app

static int hour;
static int min;
static int game_day = 1;
extern int g_mustRedraw;

void update_clock() {
  int start_x = 1;
  int start_y = 1;
  min = (min + 1)%60;
  if (min == 0) {
    hour++;
  }
  screenTime(start_x, start_y, hour, min);
}

void draw_tick_line(const unsigned int tick)
{
  static const int increment = MAX_TICKS / 256;
  static const int timediv = MAX_TICKS / 240 ;   /* assumes clock runs from 7am to 11am - 240 minutes */

  const int ticks_left = MAX_TICKS - tick;
  unsigned char point, previous_point;
  static int tick_progress = MAX_TICKS;
  previous_point = (ticks_left + 1) / increment;
  point = ticks_left / increment;
  if (previous_point != point) {
    // zero out lower bit
    int old_byte = *zx_pxy2saddr(previous_point, 191);
    *zx_pxy2saddr(previous_point, 191) = (old_byte << 1);
    int next_progress = ticks_left / timediv;
    if (next_progress != tick_progress) {
      update_clock();
      tick_progress = next_progress;
    }
  }
}

int wait_for_a_new_key() {
    unsigned char entry = in_inkey();
    unsigned char keyp = entry;
    while (keyp == 0 || keyp == entry) {
      keyp = in_inkey();
    }
    in_wait_nokey();
    return (int)keyp;
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
  long last_score = (long)input->ptr;
  if (params->score != last_score) {
    int show_score = score_to_display(params->score);
    screenNumber(21, 1, show_score);
    input->ptr = (void *)params->score;
  }
  params->ticks += 1;
  if (params->ticks < MAX_TICKS) {
    draw_tick_line(params->ticks);
  } else {
    params->hotelOpen = 0;
  }
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
    zx_border(params->ticks & 7);
  }
  params->maxToast = 0; //Reset for next loop
}

void game_do_day(const unsigned int day)
{
  int i, j, emo;
  /* Fill screen with faces */
  for (i = 1; i < 24; i+=2) {
    for (j = 1; j < 32; j+=2) {
     emo = rand() & 3; 
     screenEmotion(j, i, emo);
    }
  }
  /* clear centre of screen */
  for (i = 15; i < 18; i++) {
    for (j = 11; j < 14; j++) {
      screenClear(i,j);
    }
  }
  draw_number(16, 12, day);
  for (i = 0; i < 20000; i++) {;}
  g_mustRedraw = 1;
  hour = 6;
  min = 59;
}

int main_game( int hiScore )
{
    int retScore;
    int happyEnding = 0;

    start_frame_count();
    attractor();
    //Clear screen
    zx_cls(PAPER_WHITE);
    printf(PRINTAT"\x01\x01%s", HAPPY_CUSTOMER);
    printf(PRINTAT "\x01\x01" \
      "Welcome to the " "\x10\x32H\x10\x34o\x10\x36t\x10\x35" "e\x10\x31l " "\x10\x32" "E" \
      "\x10\x34" "x" "\x10\x36" "c" "\x10\x35" "e" "\x10\x31" "s\x10\x33s\x10\x30.");
    printf(PRINTAT "\x01\x03" "We loved your job application to");
    printf(PRINTAT "\x01\x04" "be our new breakfast Toast Host,");
    printf(PRINTAT "\x01\x05" "but now stuff gets really tasty.");
    printf(PRINTAT "\x01\x07" "\x12\x31\x10\x32" "Can you stand the heat?\x12\x30\x10\x30");
    printf(PRINTAT "\x05\x0B" "Press any key to start ");

    printf(PRINTAT "\x01\x13" "High score: %d", hiScore);
    
    MusicPlayer *mplayer = get_music_player(3);
    mplayer->add_music(mplayer, TUNE_TIMING, 2);//Keep speed constant-ish
    mplayer->add_music(mplayer, TUNE_RICKROLL, 1);
    int rando = 0;
    while (in_inkey() == 0) {
      mplayer->play(mplayer);
      rando += 1;
    }
    while (in_inkey() != 0) {
      //mplayer->play(mplayer);
      rando += 17;
    }

    //int rando = wait_for_a_key(NULL, NULL);
    // get a random seed based on frame count
    srand(rando);
    game_day = 0; // We ended up with two variables with slightly different names :-(

    //Now the parameters - we do this only once per game
    BreadBin* newBreadBin = get_bread_bin();
    GameParameters* params =  get_game_parameters();
    params->breadBin = newBreadBin;
    params->minReputation = params->reputation - 10;

    while (params->gameOverFlag == 0) {
        params->ticks = 0;
        // We're still open!
        params->hotelOpen = 1;

        game_day += 1;
        //Actually increase the number of customers
        params->maxCustomers = MIN(params->maxCustomers + 1, CUSTOMER_MAX);
        play_game(params);
        // Put a message up on screen#
        if (params->gameOverFlag == 0) {
          zx_cls(PAPER_WHITE);
          printf(PRINTAT "\x01\x01"
                 //12345678901234567890123456789012
                  "Shift over: time for the lunch\n"
                  "crew to take over for Day %d", game_day);
          printf(PRINTAT "\x01\x03"
                  "Your current score is: %d\n", score_to_display(params->score));
          //Increase min rep required to survive
          if (params->reputation < params->minReputation + 500) {
            // Game over!
            if (params->reputation < (game_day - 2) * 500) {
              /* Angry face! */
              screenEmotion(18, 1, 0);
              printf(PRINTAT "\x01\x08"
                  "Unfortunately the truth is\n"
                  "that you and toast are not\n"
                  "a good partnership.\n"
                  "Let me me spell it out in\n"
                  "the simplest of terms.\n"
                  "\x12\x31\x10\x32"
                  "YOU ARE FIRED. GOODBYE.\n"
                  "\x12\x30\x10\x30");
              params->gameOverFlag = 1;
            } else {
              params->minReputation = params->reputation - 100;
              printf(PRINTAT "\x01\x05"
                  "Have you ever made a slice\n"
                  "of toast in your life?\n");
              printf(PRINTAT "\x01\x08" "      \x12\x31\x10\x32" 
                  "You stink at this!\x12\x30\x10\x30\n");
              printf(PRINTAT "\x01\x0A"
                  "But it's hard to get decent\n"
                  "staff on minimum wage so you\n"
                  "have one more chance and...\n\n");
            }
          } else {
            params->minReputation += 500;
            printf(PRINTAT "\x01\x08"
                  "You have done so well that...\n");
          }
        }

        if (params->gameOverFlag == 0) {
          if (params ->maxCustomers < CUSTOMER_MAX) {
            printf("\x10\x34We're opening up to more\n"
                  "customers than ever before.\x10\x30\n");
          } else {
            printf("\x10\x34We'll just keep going.")
          }
          in_wait_nokey();
          printf("\nPress any key to start Day %d\n", game_day + 1);
          //printf("%d %d %d         ", params->gameOverFlag, params->reputation, params->minReputation);
          wait_for_a_new_key();
        }
    }

    if (params->messageAddress == 999) {
      printf(PRINTAT"\x01\x0C" "%s", (char *)params->message);
    } else if (params->messageAddress == 998) {
      printf(PRINTAT"\x01\x0A" "%s", (char *)params->message);
      happyEnding = 1;
    }
 
    printf(PRINTAT"\x01\x01%s", HAPPY_CUSTOMER);
    retScore = score_to_display(params->score);
    printf(PRINTAT"\x01\x12" "   Final score %d ", retScore);
    if (happyEnding == 0) {
      bit_beepfx(BEEPFX_AWW);
    } else {
      bit_beepfx(BEEPFX_YEAH);
    }

    printf(PRINTAT"\x01\x13" "   Press SPACE to restart");
    in_wait_nokey();
    while (wait_for_a_new_key() != 32) {
      ;
    }
    free (newBreadBin);
    free (params);
    return retScore; 
}

int play_game( GameParameters* params )
{
    int i, j;
    zx_cls(PAPER_WHITE);
    game_do_day(game_day);
  
    zx_cls(PAPER_WHITE);
    for (i = 0; i < 2; i++) {
      for (j = 0; j < 32; j++) {
        *zx_cxy2aaddr(j, i) = INK_YELLOW | PAPER_BLACK;
      }
    }
    for (j = 0; j < 32; j++) {
      *zx_cxy2aaddr(j, 23) = INK_RED | PAPER_BLACK;
    }

  
  // *******************************************************
  // Music set-up
  // *******************************************************

    //Three-voice music player
    MusicPlayer* music_player = get_music_player(2);
    //music_player->add_music(music_player, TUNE_TIMING, 2);//Keep speed constant-ish
    music_player->add_music(music_player, TUNE_DRUM, 1);
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

    
    GameComponent ctrl = {
        NULL,
        &command_entry_func_instant,
        &slot4
    };


    GameComponent ticker = {
                            (void *)SCREENSTART, //ptr = last_score - needs to be different to opening score
                            &tick_func, //func
                            &ctrl //next
                            };
  //}

    // draw starting time line
    for (i = 0; i < 256; i++) {
      *zx_pxy2saddr(i, 191) |= zx_px2bitmask(i);
    }
    
    printf(PRINTAT"%c%c""%-12s", 18, 3,  "Order Queue");
    write_block(15, 16, 31, 22);

    for (i = 0;
         params->gameOverFlag == 0 && 
                  (i < MAX_TICKS ||  base.customerCount > 0); //Existing customers should be served (or leave)
         i++) { 
      //Ensure we can't get an overrun on frames
      G_frames_local = 0;
      int last_frame_count = G_frames_local;

      GameComponent* comp = &ticker;
      while (comp) {
        comp->func(comp, params);
        comp = comp->next;
        music_player->play(music_player);
      }
      //Check for a new sound effect
      if (params->effect != NULL) {
        music_player->add_music_if_different(music_player, params->effect, 0);
        params->effect = NULL;
      }
      //Min one frame per loop
      while (G_frames_local < last_frame_count + params->wait_frames) {}
      if (params->score < 0 || params->reputation < params->minReputation) {
          /* Angry face! */
          screenEmotion(18, 1, 0);
          params->gameOverFlag = 1;
          params->messageAddress = 999;
          params->message = INK"\x32"PAPER"\x36"FLASHON
          " You have ruined the reputation \n"
          "      of the Hotel Excess!      \n"
          "        YOU ARE FIRED!!         \n"
          HAPPY_CUSTOMER FLASHOFF " ";
      } else if (params->score >= params->winningScore) {
          /* Angry face! */
          screenEmotion(18, 1, 3);
          params->gameOverFlag = 1;
          params->messageAddress = 998;
          params->message = INK"\x31"PAPER"\x35"FLASHON
          "You are the greatest toast host \n"
          " and far too good for the Hotel \n"
          " Excess. You've been poached by \n"
          "         Happy Eater.           \n"
          "   CONGRATULATIONS!! YOU WON!   \n"
          HAPPY_CUSTOMER FLASHOFF " ";
      }
    }
    if (params->gameOverFlag == 0) {
      screenTime(1, 1, 11, 0);
    }
    screenNumber(21, 1, score_to_display(params->score));

    //we malloc this so free it
    free(music_player);
    if (s1state->bread) {
      free(s1state->bread);
    }
    free(s1state);
    if (s2state->bread) {
      free(s2state->bread);
    }
    free(s2state);
    if (s3state->bread) {
      free(s3state->bread);
    }
    free(s3state);
    if (s4state->bread) {
      free(s4state->bread);
    }
    free(s4state);

    if (0 == params->gameOverFlag) {
      return 0;
    } else {
      return 1;
    }
} 


#ifdef _TEST_GAME
int main()
{
    int hi = 0;
    while (1) {
      int thisScore = main_game(hi);
      if (thisScore > hi) {
        hi = thisScore;
      }
    }
} 
#endif
