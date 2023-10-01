#include "tune_library.h"
//Tunes and sound effects are defined here - this file should be included
//in the file that contains "main" - will need to use #ifdef to ensure that is the case
//All TUNE_ variables should be declared as extern in music.h

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
unsigned int* TUNE_DRUM = (unsigned int*)drum_inst;

unsigned int toast_inst[] = {1, 294,
            1, 294,
            1, 294,
            1, 294,
            2, 4, 10, 7500,
            1, 286,
            2, 4, 10, 7500,
            0
};
unsigned int* TUNE_TOAST = (unsigned int*)toast_inst;

unsigned int test_effect_inst[] = {
      2, 4, 10, 1500
    , 2, 4, 10, 7500
    , 2, 4, 10, 4500
    , 2, 4, 10, 1500
    , 2, 4, 10, 7500
    , 2, 4, 10, 1500
    , 2, 4, 10, 7500
    , 2, 4, 10, 1500
    , 2, 4, 10, 7500
    , 0
};
unsigned int* TUNE_EFFECT = (unsigned int*)test_effect_inst;

unsigned int test_effect_up[] = {
      2, 4, 10, 1500
    , 2, 4, 10, 2000
    , 2, 4, 19, 2500
    , 2, 4, 10, 3000
    , 2, 4, 10, 3500
    , 2, 4, 10, 4000
    , 2, 4, 10, 4500
    , 2, 4, 10, 5000
    , 2, 4, 10, 5500
    , 2, 4, 10, 6000
    , 2, 4, 10, 6500
    , 0
};

unsigned int test_effect_up2[] = {
      2, 5, 6, 1500
    , 2, 5, 6, 2000
    , 2, 5, 6, 2500
    , 2, 5, 6, 3000
    , 2, 5, 6, 3500
    , 2, 5, 6, 4000
    , 2, 5, 6, 4500
    , 2, 5, 6, 5000
    , 2, 5, 6, 5500
    , 2, 5, 6, 6000
    , 2, 5, 6, 6500
    , 0
};
unsigned int* TUNE_EFFECT_UP = (unsigned int*)test_effect_up2;

unsigned int test_effect_down[] = {
      2, 4, 10, 6500
    , 2, 4, 10, 6000
    , 2, 4, 19, 5500
    , 2, 4, 10, 5000
    , 2, 4, 10, 4500
    , 2, 4, 10, 4000
    , 2, 4, 10, 3500
    , 2, 4, 10, 3000
    , 2, 4, 10, 2500
    , 2, 4, 10, 2000
    , 2, 4, 10, 1500
    , 0
};
unsigned int* TUNE_EFFECT_DOWN = (unsigned int*)test_effect_down;

unsigned int test_effect_beep[] = {
    2, 10, 10, 3500
    , 0
};
unsigned int* TUNE_EFFECT_BEEP = (unsigned int*)test_effect_beep;


unsigned int test_effect_short_beep[] = {
    2, 3, 10, 3500
    , 0
};
unsigned int* TUNE_EFFECT_SHORT_BEEP = (unsigned int*)test_effect_short_beep;


unsigned int test_effect_tick[] = {
    2, 1, 2, 4000,
    0
};
unsigned int* TUNE_EFFECT_TICK = (unsigned int*)test_effect_tick;

unsigned int timing_test[] = {
        2, __BR, __C0, //A breve at 16Hz - very little sound will come out!
        0
};
unsigned int* TUNE_TIMING = (unsigned int*)timing_test;

unsigned int* never_gonna[] = {
    //We're no strnagers to love
    2, __CR, __B4,
    2, __CR, __CS5,
    2, __CR, __D5,
    1, 1,
    2, __CR, __D5,
    2, __CR, __E5,
    2, __CR, __CS5,
    2, __QU, __B4,
    2, __CR, __A4,
    1, 20,

    // You know the rules and so do I
    2, __CR, __B4,
    1, 1,
    2, __CR, __B4,
    2, __CR, __CS5,
    2, __CR, __D5,
    2, __CR, __B4,
    2, __CR, __A4,
    2, __CR, __A5,
    1, 3,
    2, __CR, __A5,
    1, 3,
    2, __MI, __E5,
    1, 20,

    //A full commitment's what I'm thinking of
    2, __CR, __B4,
    1, 1,
    2, __CR, __B4,
    2, __CR, __CS5,
    2, __CR, __D5,
    2, __CR, __B4,
    2, __CR, __D5,
    2, __CR, __E5,
    2, __CR, __CS5,
    2, __CR, __B4,
    2, __QU, __CS5,
    2, __QU, __B4,
    2, __QU, __A4,
    1, 20,

    // You wouldn't get this from any other guy (oh really? cheeky c&nt)
    2, __CR, __B4,
    1, 1,
    2, __CR, __B4,
    2, __CR, __CS5,
    2, __CR, __D5,
    2, __CR, __B4,
    2, __CR, __A4,
    2, __CR, __E5,
    1, 1,
    2, __CR, __E5,
    1, 1,
    2, __CR, __E5,
    2, __CR, __FS5,
    2, __MI, __E5,
    1, 20,

    //I just wanna tell you how I'm feeling
    2, __SB, __D5,
    2, __CR, __E5,
    2, __CR, __FS5,
    2, __CR, __D5,
    2, __CR, __E5,
    1, 1,
    2, __CR, __E5,
    1, 1,
    2, __CR, __E5,
    2, __CR, __FS5,
    2, __CR, __E5,
    2, __CR, __A4,
    1, 20,

    //Gotta make you understand
    2, __CR, __B4,
    2, __CR, __CS5,
    2, __CR, __D5,
    2, __CR, __B4,
    1, 5,
    2, __CR, __E5,
    2, __CR, __FS5,
    2, __CR, __E5,
    1, 20,

    //Never gonna give you up
    2, __QU, __A4,
    2, __QU, __B4,
    2, __QU, __D5,
    2, __QU, __B4,
    2, __CR, __FS5,
    1, 1,
    2, __CR, __FS5,
    2, __CR, __E5,
    1, 20,

    //Never goona let you down
    2, __QU, __A4,
    2, __QU, __B4,
    2, __QU, __D5,
    2, __QU, __B4,
    2, __CR, __E5,
    1, 1,
    2, __CR, __E5,
    2, __QU, __D5,
    2, __QU, __CS5,
    2, __QU, __B4,
    1, 20,

    //Never gonna run around and desert you
    2, __QU, __A4,
    2, __QU, __B4,
    2, __QU, __D5,
    2, __QU, __B4,
    2, __CR, __D5,
    2, __CR, __E5,
    2, __CR, __CS5,
    2, __CR, __A4,
    1, 1,
    2, __CR, __A4,
    2, __CR, __E5,
    2, __CR, __D5,
    1, 20,

    //Never gonna make you cry
    2, __QU, __A4,
    2, __QU, __B4,
    2, __QU, __D5,
    2, __QU, __B4,
    2, __CR, __FS5,
    1, 1,
    2, __CR, __FS5,
    2, __CR, __E5,
    1, 20,

    //Never gonna say goodbye
    2, __QU, __A4,
    2, __QU, __B4,
    2, __QU, __D5,
    2, __QU, __B4,
    2, __CR, __A5,
    1, 1,
    2, __CR, __CS5,
    2, __QU, __D5,
    2, __QU, __CS5,
    2, __QU, __B4,
    1, 20,

    //Nver gonna tell a lie, and hurt you
    2, __QU, __A4,
    2, __QU, __B4,
    2, __QU, __D5,
    2, __QU, __B4,
    2, __CR, __D5,
    2, __CR, __E5,
    2, __CR, __CS5,
    2, __CR, __A4,
    1, 1,
    2, __CR, __A4,
    2, __CR, __E5,
    2, __CR, __D5,
    1, 200,
    
    0
};


unsigned int* TUNE_RICKROLL = (unsigned int*)never_gonna;