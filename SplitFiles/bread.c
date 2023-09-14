//////////////////////////////////////////////////////////
//
// Bread components - including types and bread bin
//
//////////////////////////////////////////////////////////

// Can be compiled with:
// zcc +zx -vn -startup=1 -clib=sdcc_iy -D_TEST_BREAD bread.c -o bread -create-app

#include <arch/zx.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <string.h>

#include "bread.h"
#include "util.h"


// create a breadbin of randomly chosen breads
BreadBin* get_bread_bin(void) {
    int i;
    BreadBin* bin = malloc(sizeof(BreadBin));
    for (i = 0; i < BREADBINSIZE; i++) {
        // Should we test for NULL here? Game is broken if we get NULL
        // so not bother for now
        bin->breadTypes[i] = create_bread_type();
    }
    return bin;
}

// free the resources
void clean_bread_bin(BreadBin* bin) {
    int i;
    for (i = 0; i < BREADBINSIZE; i++) {
        if (NULL != bin->breadTypes[i]) {
            free(bin->breadTypes[i]);
            bin->breadTypes[i] = NULL;
        }
    }
    free(bin);
    bin = NULL;
}

// Create the bread types - fiddle in here to change bread distribution
BreadType* create_bread_type(void)
{
    // distribution: 
    //          35% white
    //          30% brown
    //          20% bagel
    //          15% ciabbatta
    int rando_bread = rand()%100;
    BreadType* slice = malloc(sizeof(BreadType));
    if (rando_bread < 35) {
            return getWhiteBread(slice);
    }
    else if (rando_bread < 65) {
            return getBrownBread(slice);
    }
    else if (rando_bread < 85) {
            return getBagelBread(slice);
    }
    return getCiabattaBread(slice);
}


// bread type constructors - change these to change what different
// types of bread might be like
BreadType* getWhiteBread(BreadType* slice)
{
    //driest bread, quickest to toast
    strcpy(slice->desc, "White\0");
    memcpy(slice->letter_desc, "W", 1);
    slice->moisture = 64 + rand()%48;
    slice->old_moisture = slice->moisture;
    slice->temperature = 0;
    slice->old_temperature = 0;
    slice->toastedness = 0;
    slice->old_toastedness = 0;
    slice->thermalMass = 64 + rand()%16;
    return slice;
}

BreadType* getBrownBread(BreadType* slice)
{
    //more moist than white
    strcpy(slice->desc, "Brown\0");
    memcpy(slice->letter_desc, "B", 1);
    slice->moisture = 80 + rand()%64;
    slice->old_moisture = slice->moisture;
    slice->temperature = 0;
    slice->old_temperature = 0;
    slice->toastedness = 0;
    slice->old_toastedness = 0;
    slice->thermalMass = 80 + rand()%32;
    return slice;
}

BreadType* getBagelBread(BreadType* slice)
{
    //bit random but more moist
    strcpy(slice->desc, "baGel\0");
    memcpy(slice->letter_desc, "G", 1);
    slice->moisture = 64 + rand()%192;
    slice->old_moisture = slice->moisture;
    slice->temperature = 0;
    slice->old_temperature = 0;
    slice->toastedness = 0;
    slice->old_toastedness = 0;
    slice->thermalMass = 160 + rand()%48;
    return slice;
}

BreadType* getCiabattaBread(BreadType* slice)
{
    //dry but thicker
    strcpy(slice->desc, "Ciabatta\0");
    memcpy(slice->letter_desc, "C", 1);
    slice->moisture = 48 + rand()%48;
    slice->old_moisture = slice->moisture;
    slice->temperature = 0;
    slice->old_temperature = 0;
    slice->toastedness = 0;
    slice->old_toastedness = 0;
    slice->thermalMass = 128 + rand()%128;
    return slice;
}

//check if the specified bread type can be seen and thus is available
bool IsBreadTypeVisible(BreadBin* bin, char bT)
{
    int i;
    bool retVal = false;
    for (i = 0; i < VISIBLEBIN; i++) {
        if ((bin->breadTypes[i]->letter_desc)[0] == bT) {
            retVal = true;
            break;
        }
    }
    return retVal;
}