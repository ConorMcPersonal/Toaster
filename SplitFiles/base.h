#ifndef _DEFINE_BASE_H
#define _DEFINE_BASE_H

// 1.) Forward declaration: Here is the name of the structure
// but member-details are omitted.
struct BreadBinStruct;

// 2.) typedef of the structure
// Still no details on the members.
typedef struct BreadBinStruct  BreadBin;

struct BreadTypeStruct {
    int               thermalMass;      //This mass
    int               massVariability;  //Plus or minus this amount
    int               moisture;         //This moisture
    int               moistureVariability;  //Plus or minus...
    int               cost;
    int               callProb;         //Actually an integer, higher number = more likely
    char*             desc;
    unsigned char     letter;
};
typedef struct BreadTypeStruct BreadType;

struct BreadBinStruct {
    int probTotal;
    BreadType** breadTypes;                    //will be an array of 26 pointers 
                                               //       where (BreadTypeStruct.letter - 'A') is the index
};

struct GameParameters_Struct;

typedef struct GameParameters_Struct GameParameters;

// Forward declaration
struct GameComponent_Struct;

// typedef of the structure
typedef struct GameComponent_struct  GameComponent;

// 3.) The parameter to the function is listed, using the definition
// from step 2.)  (note: Still no details on the members yet)
//typedef void (*GameComponentFunc)(GameComponent* input, GameParameters* params);
typedef void (*GameComponentFunc)(GameComponent*, GameParameters*);

struct GameParameters_Struct {
  long      score; // What's the score
  int       ticks; // How many times has main loop run
  int       slices; //How many slices toasted?
  int       gameOverFlag; // Zero if game not over
  int       maxToast; // Who has most toastedness - is anything on fire?
  int       messageAddress;  //Who or what is the message being sent to?
  void*     message; //Pointer to a message structure of some kind - e.g. a command issued to a component
  void*     messageSourceAddress; //Who emitted the message?  Set to GameComponent->ptr of the source
  unsigned int* effect; //Sound effect to be played
  BreadBin*  breadBin;
  int        reputation; //Starts at 1000 - can't drop below minReputation or you're in trouble
  int        hotelOpen;  // Are we open for business?
  int        maxCustomers; //How many customers are we allowing now?
  int        minReputation; //How bad do you have to be to get the sack?
  int        wait_frames;   //Minimum frames per loop
  int        toast_ticks;   //How often toast is updated
};


// 4.) Now we actually detail the members of the structure
struct GameComponent_struct {
  void*                      ptr; //Pointer to any state information for this object - will need to be cast before use
  GameComponentFunc          func;
  GameComponent*             next;
};

GameParameters* get_game_parameters(void);
int score_to_display(long score);

#endif