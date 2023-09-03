#ifndef _DEFINE_MUSIC_H
#define _DEFINE_MUSIC_H


struct Music_Player_Struct;
struct Music_Struct;
struct Note_Struct;

typedef struct Music_Player_Struct  MusicPlayer;
typedef struct Music_Struct  Music;
typedef struct Note_Struct   Note;

typedef Note* (*MusicInc)(Music*, Note*, char);
typedef void  (*MusicAdd)(MusicPlayer*, unsigned int*, unsigned char);
typedef void  (*MusicStep)(MusicPlayer*);

struct Music_Struct {
  unsigned int *     buffer;      //The music expressed as a string of integers, zero-terminated (will loop round)
  int                bufferIndex; // Current position in the buffer - signed because -ve means this is done
  unsigned int       subCount;    // If the current position has a count on it, count them here
  unsigned int       nextInc;     //How many spaces to move the buffer index next time it moves
  int                startIndex;  //Zero if this should loop, -ve if it's a "one and done"
};

struct Note_Struct {
    unsigned int time;  //How long to play
    unsigned int pitch; //Frequency in Hertz
};

struct Music_Player_Struct {
    unsigned char voiceCount; //How many "voices" (independent tunes, in priority order)
    Music*        tunes;      //Array of "tunes" and effects
    void          add_music(MusicPlayer*, unsigned int*, unsigned char);  //Add a tune for a given "voice"
    void          add_music_if_different(MusicPlayer*, unsigned int*, unsigned char);  //Add a tune for a given "voice"
    void          play     (MusicPlayer*);       //Called repeatedly, makes sounds come out of the speaker
    Note *        note;       //The note to be played
};

Note* step_music(Music* music, Note* G_note);
void step_tunes(Music** tunes, int numTunes, Note* G_note);
MusicPlayer* get_music_player(unsigned char);

/*********************************************
 * 
 *     Tunes library
 * 
*********************************************/
extern unsigned int* TUNE_DRUM;
extern unsigned int* TUNE_TOAST;
extern unsigned int* TUNE_EFFECT;
#endif