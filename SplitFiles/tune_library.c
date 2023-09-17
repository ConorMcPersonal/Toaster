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