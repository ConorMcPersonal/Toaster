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

BreadType* get_bread_type(BreadBin* bin, unsigned char typeLetter) {
    if (typeLetter < 'A' || typeLetter > 'Z') {
        return NULL;
    }
    int index = typeLetter - (unsigned char)'A';
    return bin->breadTypes[index];
}

BreadState* get_bread(BreadBin* bin, unsigned char typeLetter) {
    //int index = typeLetter - (unsigned char)'A';
    //BreadType* type = *(bin->breadTypes + index);
    BreadType* type = get_bread_type(bin, typeLetter);

    if (type == NULL) {
        return (BreadState *)NULL;
    }
    BreadState* newBread = malloc(sizeof(BreadState));
    newBread->temperature = 0;
    newBread->moisture = type->moisture + (rand() & type->moistureVariability);
    newBread->toastedness = 0;
    newBread->thermalMass = type->thermalMass + (rand() & type->massVariability);
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
    for (int i = 0; i < BREADBINSIZE; ++i) {
        types[i] = NULL;
    }
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
            7, //variability
            64, //moisture
            63, // moisture variability
            50, // cost
            64  //callProb
        )
    );

    add_bread(bin, 
        create_bread_type(
            'M', //letter
            "Melba", //desc
            31, //thermal Mass
            7, //variability
            32, //moisture
            15, // moisture variability
            50, // cost
            32  //callProb
        )
    );

    add_bread(bin, 
        create_bread_type(
            'B', //letter
            "Brown", //desc
            82, //thermal Mass
            15, //variability
            96, //moisture
            63, // moisture variability
            75, // cost
            32  //callProb
        )
    );

    add_bread(bin, 
        create_bread_type(
            'G', //letter
            "baGel", //desc
            150, //thermal Mass
            31, //variability
            64, //moisture
            255, // moisture variability
            125, // cost
            16  //callProb
        )
    );

    add_bread(bin, 
        create_bread_type(
            'C', //letter
            "Ciabatta", //desc
            180, //thermal Mass
            31, //variability
            32, //moisture
            15, // moisture variability
            100, // cost
            12  //callProb
        )
   );


    add_bread(bin, 
        create_bread_type(
            'F', //letter
            "gluten Free", //desc
            255, //thermal Mass
            1, //variability
            128, //moisture
            15, // moisture variability
            150, // cost
            8  //callProb
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

void print_all_bread_types(BreadBin* bin) {
    for (int i = 0; i < BREADBINSIZE; ++i) {
        if (bin->breadTypes[i] != NULL) {
            printf("%d %c %s\n", i, bin->breadTypes[i]->letter, bin->breadTypes[i]->desc);
        }
    }
}

int main_bread() {
    start_frame_count();
    //Set up the bin
    BreadBin* bin = get_bread_bin();
    print_all_bread_types(bin);
    BreadType* type;
    BreadType* type2;
    BreadState* bread;
    unsigned int sliceCount = 0;
    unsigned int startFrame, endFrame;
    while (1) {
        startFrame = G_frames;
        endFrame = startFrame + 2;
        type = rand_bread_type(bin);
        printf(PRINTAT"\x05\x05""%s      ", type->desc);
        type2 = get_bread_type(bin, type->letter);
        printf(PRINTAT"\x0B\x05""%s      ", type2->desc);
        bread = get_bread(bin, type->letter);
        ++sliceCount;
        printf(PRINTAT"\x05\x06""%c %-3d %-3d %-15s           ", bread->type->letter, bread->thermalMass, bread->moisture, bread->type->desc);
        printf(PRINTAT"\x05\x07"" %u %u  %u         ", G_frames, endFrame, sliceCount);
        while (G_frames < endFrame && (endFrame - G_frames) < 2) {
            printf(PRINTAT"\x05\x07"" %u %u  %u         ", G_frames, endFrame, sliceCount);
        }
        free(bread);
    }

}


#ifdef _TEST_BREAD

int main() {
    return main_bread();
}
#endif
