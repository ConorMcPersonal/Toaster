#ifndef _DEFINE_GAME_H
#define _DEFINE_GAME_H

struct GameParameters_Struct;

typedef struct GameParameters_Struct GameParameters;

// 1.) Forward declaration: Here is the name of the structure
// but member-details are omitted.
struct GameComponent_Struct;

// 2.) typedef of the structure
// Still no details on the members.
typedef struct GameComponent_struct  GameComponent;

// 3.) The parameter to the function is listed, using the definition
// from step 2.)  (note: Still no details on the members yet)
//typedef void (*GameComponentFunc)(GameComponent* input, GameParameters* params);
typedef void (*GameComponentFunc)(GameComponent*, GameParameters*);

struct GameParameters_Struct {
  int       ticks; // How many times has main loop run
  int       score; // What's the score
  int       slices; //How many slices toasted?
  int       gameOverFlag; // Zero if game not over
  int       maxToast; // Who has most toastedness - is anything on fire?
  int       messageAddress;  //Who or what is the message being sent to?
  void*     message; //Pointer to a message structure of some kind - e.g. a command issued to a component
};


// 4.) Now we actually detail the members of the structure
struct GameComponent_struct {
  void*                      ptr; //Pointer to any state information for this object - will need to be cast before use
  GameComponentFunc          func;
  GameComponent*             next;
};

struct dispatcher_struct {
  int last_time_int;
};
typedef struct dispatcher_struct DispatcherState;

#endif