#include <input.h>
#include <stdio.h>
#include <stdlib.h>
#include <arch/zx.h>
#include <sound.h>

#include "game.h"
#include "util.h"
#include "music.h"
//#ifdef _TEST_MUSIC
#include "tune_library.c"
//#endif

// Can be compiled with:
// zcc +zx -vn -startup=1 -clib=sdcc_iy -D_TEST_MUSIC music.c -o music -create-app

Note* step_music(Music* music, Note* note) {
    Note* retVal = NULL;
    if (music->subCount > 0) {
        //Keep counting down
        music->subCount = music->subCount - 1;

        if ((music->buffer[music->bufferIndex]) == 1 || note->time != 0) {
            //Pause, or at least do not update the note
            return NULL;
        }
        note->time  = music->buffer[music->bufferIndex + 2];
        note->pitch = music->buffer[music->bufferIndex + 3];
        retVal = note;
    }
    if (music->subCount == 0) {
        //Time to move on
        music->bufferIndex = music->bufferIndex + music->nextInc;
        if (music->buffer[music->bufferIndex] == 0) {
            //Loop back to the start
            music->bufferIndex = music->startIndex;
        }
        if (music->bufferIndex >= 0) {
            //Set up next command
            if (music->buffer[music->bufferIndex] == 1) {
                //A pause
                music->subCount = music->buffer[music->bufferIndex + 1];
                music->nextInc = 2;
            } else if (music->buffer[music->bufferIndex] == 2) {
                //Play a note
                music->subCount = music->buffer[music->bufferIndex + 1];
                music->nextInc = 4;
            }
        }
    }
    return retVal;
}

void step_player(MusicPlayer* player) {
    int i;
    Note* next_note;
    Note* play_note;
    play_note = NULL;
    for (i = 0; i < player->voiceCount; ++i) {
        if ((player->tunes + i)->buffer != NULL) {
            next_note = step_music((player->tunes + i), player->note);
            if (play_note == NULL) {
                play_note = next_note;
            }
        }
        if ((player->tunes + i)->bufferIndex < 0) {
            //This sound is played out - remove it
            (player->tunes + i)->buffer = NULL;
        }
    }
    if (play_note != NULL) {
        bit_beep(play_note->time, play_note->pitch);
        play_note->time = 0;
    }
}

//Add a tune to a voice.  If it's voice 0 it'll be played once - the rest all loop
void add_music(MusicPlayer* player, unsigned int* tune, unsigned char index) {
    if (index >= player->voiceCount) {
        bit_beep(1000, 1000);
        return;
    }
    Music* thisTune = player->tunes + index;
    thisTune->buffer = tune;
    thisTune->startIndex = (index == 0)? -1: 0;
    thisTune->bufferIndex = 0;
    thisTune->nextInc = 0;
    thisTune->subCount = 0;
    return;
}

void add_music_if_different(MusicPlayer* player, unsigned int* tune, unsigned char index) {
    if (index >= player->voiceCount) {
        return;
    }
    if ((player->tunes + index)->buffer != tune) {
        player->add_music(player, tune, index);
    }
}

//Constructor for a Music Player
MusicPlayer* get_music_player(unsigned char voiceCount) {
    MusicPlayer* player = (MusicPlayer*)malloc(sizeof(MusicPlayer));
    Music* tunesArray = (Music*)malloc(voiceCount * sizeof(Music));
    Note* theNote = (Note*)malloc(sizeof(Note));
    theNote->time = 0;
    for (int i = 0; i < voiceCount; ++i) {
        //Initialise
        tunesArray[i].bufferIndex = 0;
        tunesArray[i].nextInc = 0;
        tunesArray[i].startIndex = 0;
        tunesArray[i].subCount = 0;
        tunesArray[i].buffer = NULL;
    }
    player->add_music = &add_music;
    player->note = theNote;
    player->tunes = tunesArray;
    player->voiceCount = voiceCount;
    player->play = &step_player;
    player->add_music_if_different = &add_music_if_different;
    return player;
}

int music_main()
{
    int i, j;
    unsigned char *rando = (unsigned char *)23672;
    unsigned int* effects[] = {TUNE_EFFECT_SHORT_BEEP, TUNE_EFFECT_UP, TUNE_EFFECT_DOWN, TUNE_EFFECT_BEEP, NULL};
    int effectIndex = 0;

    MusicPlayer *mPlayer = get_music_player(3);
    mPlayer->add_music(mPlayer, TUNE_DRUM, 2);
    mPlayer->add_music(mPlayer, TUNE_TOAST, 1);
    i = 0;
    j = 0;
    //Clear screen
    zx_cls(PAPER_WHITE);
    while (1) {
        mPlayer->play(mPlayer);
        //for (j = 0; j < 1; ++j) {
            printf(PRINTAT"\x05\x05""%d %d %d %d", j, *rando, *(rando + 1), *(rando + 2));
        //}
        j += 1;
        if ((j % 500) == 0) {
            mPlayer->add_music(mPlayer, effects[effectIndex], 0);
            effectIndex += 1;
            if (effects[effectIndex] == NULL) {
                effectIndex = 0;
            }
            //if ((j % 1000) == 0) {
            //    mPlayer->add_music(mPlayer, TUNE_EFFECT_DOWN, 0);
            //} else {
            //    mPlayer->add_music(mPlayer, TUNE_EFFECT_UP, 0);
            //}
        }
    }
    return 0;
}


#ifdef _TEST_MUSIC
int main()
{
    return music_main();
} 
#endif 