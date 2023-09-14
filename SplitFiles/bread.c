//////////////////////////////////////////////////////////
//
// Bread components - including types and bread bin
//
//////////////////////////////////////////////////////////

// Can be compiled with:
// zcc +zx -vn -startup=1 -clib=sdcc_iy -D_TEST_BREAD bread.c util.c -o bread -create-app

#include <arch/zx.h>

#include <stdio.h>
#include <stdlib.h>
#include <input.h>
#include <string.h>
#include <stdbool.h>

#include "bread.h"
#include "util.h"

BreadType* rand_bread_type(BreadBin* bin) {
    int i;
    int rand_choice;
    rand_choice = rand() % bin->probTotal;
    for (i = 0; i < BREADBINSIZE; ++i) {
        if (bin->breadTypes[i] != NULL) {
            if (rand_choice > bin->breadTypes[i]->callProb) {
                rand_choice -= bin->breadTypes[i]->callProb;
            } else {
                //Found our bread type
                return bin->breadTypes[i];
            }
        }
    }
    //This should never be reached
    printf("Error in rand_bread_type");
    return rand_bread_type(bin);
}

BreadState* get_bread(BreadBin* bin, unsigned char typeLetter) {
    int index = typeLetter - (unsigned char)'A';
    BreadType* type = *(bin->breadTypes + index);

;

    if (type == NULL) {
        return (BreadState *)NULL;
    }
    BreadState* newBread = malloc(sizeof(BreadState));
    newBread->temperature = 0;
    newBread->moisture = type->moisture + (rand() % type->moistureVariability);
    newBread->toastedness = 0;
    newBread->thermalMass = type->thermalMass + (rand() % type->massVariability);
    newBread->thermalAggregation = 0;
    newBread->type = type;
    return newBread;
}

void add_bread(BreadBin* bin, BreadType* newType) {
    //Set up in the bin
    int index = newType->letter - (unsigned char)'A';
    bin->breadTypes[index] = newType;
    bin->probTotal += newType->callProb;
}

BreadBin* get_bread_bin() {
    BreadType** types = malloc(sizeof(BreadType*) * BREADBINSIZE);
    BreadBin* bin = malloc(sizeof(BreadBin));
    bin->probTotal = 0;
    bin->breadTypes = types;

    /*******************************************************
    ***** This is where we establish the Bread types
    ***** Once established they are only ever referenced
    ***** through the BreadBin methods
    ********************************************************/
    add_bread(bin, 
        create_bread_type(
            'W', //letter
            "White", //desc
            62, //thermal Mass
            1, //variability
            64, //moisture
            48, // moisture variability
            50, // cost
            64  //callProb
        )
    );

    add_bread(bin, 
        create_bread_type(
            'B', //letter
            "Brown", //desc
            82, //thermal Mass
            10, //variability
            96, //moisture
            64, // moisture variability
            75, // cost
            32  //callProb
        )
    );

    add_bread(bin, 
        create_bread_type(
            'G', //letter
            "baGel", //desc
            150, //thermal Mass
            28, //variability
            64, //moisture
            192, // moisture variability
            125, // cost
            16  //callProb
        )
    );

    add_bread(bin, 
        create_bread_type(
            'C', //letter
            "Ciabatta", //desc
            180, //thermal Mass
            40, //variability
            32, //moisture
            12, // moisture variability
            100, // cost
            12  //callProb
        )
   );
   return bin;
}


BreadType*  create_bread_type(
    unsigned char     letter, 
    char*             desc,
    int               thermalMass,
    int               massVariability,  //Plus or minus this amount
    int               moisture,         //This moisture
    int               moistureVariability,  //Plus or minus...
    int               cost,
    int               callProb         //Actually an integer, higher number = more likely
) {
        BreadType* newType = malloc(sizeof(BreadType));
        newType->letter = letter;
        newType->thermalMass = thermalMass;
        newType->massVariability = massVariability;
        newType->moisture = moisture;
        newType->moistureVariability = moistureVariability;
        newType->cost = cost;
        newType->callProb = callProb;
        newType->desc = desc;
        return newType;
    }

int main() {
    start_frame_count();
    //Set up the bin
    BreadBin* bin = get_bread_bin();
    BreadType* type;
    BreadState* bread;
    int startFrame, endFrame;
    while (1) {
        startFrame = G_frames;
        endFrame = startFrame + 50;
        type = rand_bread_type(bin);
        printf(PRINTAT"\x05\x05""%s      ", type->desc);
        bread = get_bread(bin, type->letter);
        printf(PRINTAT"\x05\x06""%s %d %d           ", bread->type->desc, bread->thermalMass, bread->moisture);
        while (G_frames != endFrame) {
            printf(PRINTAT"\x05\x07"" %d %d           ", G_frames, endFrame);
        }
    }

}