#ifndef _DEFINE_BREAD_H
#define _DEFINE_BREAD_H


#include <stdbool.h>
#define BREADBINSIZE 26

// 1.) Forward declaration: Here is the name of the structure
// but member-details are omitted.
struct BreadBinStruct;

struct BreadTypeStruct {
  unsigned int      temperature;
  unsigned int      old_temperature;
  unsigned int      moisture;
  unsigned int      old_moisture;
  unsigned int      toastedness;
  unsigned int      old_toastedness;
  unsigned int      thermalMass;
  unsigned int      thermalAggregation;
  char              desc[10];
  char              letter_desc[1];
};

typedef struct BreadTypeStruct BreadType;

struct BreadBinStruct {
    BreadType* breadTypes[BREADBINSIZE];        //will be an array of 26 pointers 
};
typedef struct BreadBinStruct BreadBin;

// Breadbin creator
BreadBin*   get_bread_bin(void);
// Bread factory pattern
BreadType*  create_bread_type(void);

/* Bread creators*/
BreadType* getWhiteBread(BreadType* slice);
BreadType* getBrownBread(BreadType* slice);
BreadType* getBagelBread(BreadType* slice);
BreadType* getCiabattaBread(BreadType* slice);

#define VISIBLEBIN 5

bool IsBreadTypeVisible(BreadBin* bin, unsigned char bT);

void reorderBreadBin(BreadBin* bin, const int slot);


#endif