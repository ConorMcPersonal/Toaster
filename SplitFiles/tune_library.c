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