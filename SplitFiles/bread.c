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
#include <input.h>
#include <string.h>
#include <stdbool.h>

#include "bread.h"
#include "util.h"


// create a breadbin of randomly chosen breads
BreadBin* get_bread_bin() {
    int i;
    BreadBin* bin = malloc(sizeof(BreadBinStruct));
    for (i = 0; i < BREADBINSIZE; i++) {
        // Should we test for NULL here? Game is broken if we get NULL
        // so not bother for now
        bin->breadTypes[i] = createBreadType();
    }
}

// free the resources
void clean_bread_bin(BreadBin* bin) {
    int i;
    for (i = 0; i < BREADBINSIZE; i++) {
        if (NULL != bin->breadTypes[i]) {
            free(bin->breadTypes[i]);
            breadTypes[i] = NULL;
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
    switch (true) {
        case (rando_bread < 35):
            return getWhiteBread();
        case (rando_bread < 65):
            return getBrownBread();
        case (rando_bread < 85):
            return getBagelBread();
        default:
            return getCiabbataBread();
    }
}


// bread type constructors - change these to change what different
// types of bread might be like
BreadType* getWhiteBread(void)
{
    //driest bread, quickest to toast
    BreadType* slice = malloc(sizeof(BreadTypeStruct));
    slice->desc = "White";
    slice->letter_desc = "W";
    slice->moisture = 64 + rand()%48;
    slice->old_moisture = slice->moisture;
    slice->temperature = 0;
    slice->old_temperature = 0;
    slice->toastedness = 0;
    slice->old_toastedness = 0;
    slice->thermalMass = 64 + rand()%16;
}

BreadType* getBrownBread(void)
{
    //more moist than white
    BreadType* slice = malloc(sizeof(BreadTypeStruct));
    slice->desc = "Brown";
    slice->letter_desc = "B";
    slice->moisture = 80 + rand()%64;
    slice->old_moisture = slice->moisture;
    slice->temperature = 0;
    slice->old_temperature = 0;
    slice->toastedness = 0;
    slice->old_toastedness = 0;
    slice->thermalMass = 80 + rand()%32;
}

BreadType* getBagelBread(void)
{
    //bit random but more moist
    BreadType* slice = malloc(sizeof(BreadTypeStruct));
    slice->desc = "baGel";
    slice->letter_desc = "G";
    slice->moisture = 64 + rand()%192;
    slice->old_moisture = slice->moisture;
    slice->temperature = 0;
    slice->old_temperature = 0;
    slice->toastedness = 0;
    slice->old_toastedness = 0;
    slice->thermalMass = 160 + rand()%48;
}

BreadType* getCiabattaBread(void)
{
    //dry but thicker
    BreadType* slice = malloc(sizeof(BreadTypeStruct));
    slice->desc = "Ciabatta";
    slice->letter_desc = "C";
    slice->moisture = 48 + rand()%48;
    slice->old_moisture = slice->moisture;
    slice->temperature = 0;
    slice->old_temperature = 0;
    slice->toastedness = 0;
    slice->old_toastedness = 0;
    slice->thermalMass = 128 + rand()%128;
}

//check if the specified bread type can be seen and thus is available
bool IsBreadTypeVisible(BreadBin* bin, char bT)
{
    int i;
    bool retVal = false;
    for (i = 0; i < VISIBLEBIN; i++) {
        if (bin->breadTypes[i]->letter_desc == bt) {
            retVal = true;
            break;
        }
    }
    return retVal;
}