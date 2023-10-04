#ifndef _DEFINE_BREAD_H
#define _DEFINE_BREAD_H

#define BREADBINSIZE 26

#include "base.h" //BreadBin and BreadType defined here

struct BreadStateStruct {
  int               temperature;  
  int               moisture;
  int               toastedness;
  unsigned char     thermalMass;
  unsigned int      thermalAggregation;
  BreadType*        type; // keep reference to underlying BreadType, for letter, desc
};

typedef struct BreadStateStruct BreadState;

BreadType*   rand_bread_type(BreadBin*);               //get a random BreadType
BreadState*  get_bread(BreadBin*, unsigned char);      //get a specific BreadState
void         add_bread(BreadBin*, BreadType*);         //add a bread type to the bread bin
BreadType*   get_bread_type(BreadBin*, unsigned char);       //ensure the requested bread type exists
void         print_all_bread_types(BreadBin*);

BreadBin*   get_bread_bin(void);
BreadType*  create_bread_type(
    unsigned char     letter, 
    const char*       desc,
    int               thermalMass,
    int               massVariability,  //Plus or minus this amount
    int               moisture,         //This moisture
    int               moistureVariability,  //Plus or minus...
    int               cost,
    int               callProb         //Actually an integer, higher number = more likely
);

#endif