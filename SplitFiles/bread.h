#ifndef _DEFINE_BREAD_H
#define _DEFINE_BREAD_H

#define BREADBINSIZE 26

// 1.) Forward declaration: Here is the name of the structure
// but member-details are omitted.
struct BreadBinStruct;

// 2.) typedef of the structure
// Still no details on the members.
typedef struct BreadBinStruct  BreadBin;

struct BreadTypeStruct {
  unsigned int      temperature;
  unsigned int      old_temperature;
  unsigned int      moisture;
  unsigned int      old_moisture;
  unsigned int      toastedness;
  unsigned int      old_toastedness;
  unsigned int      thermalMass;
  unsigned int      thermalAggregation;
  char*             desc;
  char              letter_desc;
};

typedef struct BreadTypeStruct BreadType;
/*
struct BreadTypeStruct {
    unsigned char     letter;
    int               thermalMass;      //This mass
    int               massVariability;  //Plus or minus this amount
    int               moisture;         //This moisture
    int               moistureVariability;  //Plus or minus...
    int               cost;
    int               callProb;         //Actually an integer, higher number = more likely
    char*             desc;
};
typedef struct BreadTypeStruct BreadType;
*/

struct BreadBinStruct {
    BreadType* breadTypes[BREADBINSIZE];        //will be an array of 26 pointers 
};
typedef struct BreadBinStruct BreadBin;

// Breadbin creator
BreadBin*   get_bread_bin(void);
// Bread factory pattern
BreadType*  create_bread_type(void);

/* Bread creators*/
BreadType* getWhiteBread(void);
BreadType* getBrownBread(void);
BreadType* getBagelBread(void);
BreadType* getCiabbataBread(void);

#define VISIBLEBIN 5
bool IsBreadTypeVisible(BreadBin* bin, char bT);



#endif