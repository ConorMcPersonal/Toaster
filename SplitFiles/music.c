#include <input.h>
#include <stdio.h>
#include <stdlib.h>
#include <arch/zx.h>
#include <sound.h>

#include "game.h"
#include "util.h"
#include "music.h"

// Can be compiled with:
// zcc +zx -vn -startup=1 -clib=sdcc_iy -D_TEST_MUSIC music.c -o music -create-app

Note* step_music(Music* music) {
    Note* retVal = NULL;
    if (music->subCount > 0) {
        //Keep counting down
        music->subCount = music->subCount - 1;

        if ((music->buffer[music->bufferIndex]) == 1 || G_Note.time != 0) {
            //Pause, or at least do not update the note
            retVal = NULL;
            return retVal;
        }
        G_Note.time  = music->buffer[music->bufferIndex + 2];
        G_Note.pitch = music->buffer[music->bufferIndex + 3];
        retVal = &G_Note;
    }
    if (music->subCount == 0) {
        //Time to move on
        music->bufferIndex = music->bufferIndex + music->nextInc;
        if (music->buffer[music->bufferIndex] == 0) {
            //Loop back to the start
            music->bufferIndex = 0;
        }
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
    return retVal;
}

void step_tunes(Music** tunes, int numTunes) {
    int i, have_note;
    Note* next_note;
    Note* play_note;
    have_note = 0;
    play_note = NULL;
    for (i = 0; i < numTunes; ++i) {
        //try play_note = play_note || tunes[i]->incBuffer(tunes[i]);
        next_note = tunes[i]->incBuffer(tunes[i]);
        if (have_note == 0 && next_note != NULL) {
            play_note = next_note;
            have_note = 1;
        }
    }
    if (have_note != 0) {
        printf(PRINTAT"\x05\x15""%d %-10d", tunes[0]->bufferIndex, tunes[1]->bufferIndex);
        //printf(PRINTAT"\x05\x16""%u %-10u", play_note->time, play_note->pitch);
        bit_beep(play_note->time, play_note->pitch);
        play_note->time = 0;
    }

}

int music_main()
{
    int i, j;
    unsigned char *rando = (unsigned char *)23672;

    unsigned int drum_inst[] = {1, 34,
                2, 1, 2, 4000,
                1, 55,
                2, 1, 2, 4000,
                1, 55,
                2, 1, 2, 4000,
                1, 55,
                2, 1, 2, 4000,
                1, 55,
                2, 1, 2, 4000,
                1, 34,
                2, 1, 2, 4000,
                0
    };
    unsigned int toast_inst[] = {1, 294,
                1, 294,
                1, 294,
                1, 294,
                2, 4, 10, 7500,
                1, 286,
                2, 4, 10, 7500,
                0
    };

    Music drum_track = {
        (unsigned int *)&drum_inst,
        0,
        0,
        0,
        &step_music
    };

    Music toast_track = {
        (unsigned int *)&toast_inst,
        0,
        0,
        0,
        &step_music
    };

    i = 0;
    j = 0;
    Music* tunes[] = {&toast_track, &drum_track};
    //Clear screen
    zx_cls(PAPER_WHITE);
    while (1) {
        step_tunes(tunes, 2);
        //for (j = 0; j < 1; ++j) {
            printf(PRINTAT"\x05\x05""%d %d %d %d", j, *rando, *(rando + 1), *(rando + 2));
        //}
        j += 1;
    }
    return 0;
}


#ifdef _TEST_MUSIC
int main()
{
    return music_main();
} 
#endif 