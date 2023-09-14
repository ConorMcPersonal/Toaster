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

struct BreadStateStruct {
  int               temperature;  
  int               moisture;
  int               toastedness;
  unsigned char     thermalMass;
  unsigned int      thermalAggregation;
  BreadType*        type; // keep reference to underlying BreadType, for letter, desc
};

typedef struct BreadStateStruct BreadState;


struct BreadBinStruct {
    int probTotal;
    BreadType** breadTypes;                    //will be an array of 26 pointers 
                                               //       where (BreadTypeStruct.letter - 'A') is the index
};

BreadType*   rand_bread_type(BreadBin*);               //get a random BreadType
BreadState*  get_bread(BreadBin*, unsigned char);      //get a specific BreadState
void         add_bread(BreadBin*, BreadType*);         //add a bread type to the bread bin
BreadType*   get_type(BreadBin*, unsigned char);       //ensure the requested bread type exists

BreadBin*   get_bread_bin(void);
BreadType*  create_bread_type(
    unsigned char     letter, 
    char*             desc,
    int               thermalMass,
    int               massVariability,  //Plus or minus this amount
    int               moisture,         //This moisture
    int               moistureVariability,  //Plus or minus...
    int               cost,
    int               callProb         //Actually an integer, higher number = more likely
);

#endif