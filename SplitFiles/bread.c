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

BreadType*  rand_bread_type(BreadBin* bin) {
    int i;
    rand_choice = rand() % bin->probTotal;
    for (int i = 0; i < BREADBINSIZE; ++i) {
        if (bin->breadTypes[i] != NULL) {
            if (rand_choice > bin->breadTypes[i]->callProb) {
                rand_choice -= bin->breadTypes[i]->callProb;
            } else {
                //Found our bread type
                return bin->breadTypes[i];
            }
        }
    }
}

BreadBin* get_bread_bin() {
    BreadType** types = malloc(sizeof(BreadType*) * BREADBINSIZE);
    BreadBin* bin = malloc(sizeof(BreadBin));
    bin->probTotal = 0;
    bin->breadTypes = types;

}