#ifndef _DEFINE_MUSIC_H
#define _DEFINE_MUSIC_H


struct Music_Struct;
struct Note_Struct;

typedef struct Music_Struct  Music;
typedef struct Note_Struct   Note;

typedef Note* (*MusicInc)(Music*, Note*);

struct Music_Struct {
  unsigned int *     buffer;      //The music expressed as a string of integers, zero-terminated (will loop round)
  unsigned int       bufferIndex; // Current position in the buffer
  unsigned int       subCount;    // If the current position has a count on it, count them here
  unsigned int       nextInc;     //How many spaces to move the buffer index next time it moves
  MusicInc  incBuffer;   // Called every loop
};

struct Note_Struct {
    unsigned int time;  //How long to play
    unsigned int pitch; //Frequency in Hertz
};

Note* step_music(Music* music, Note* G_note);
void step_tunes(Music** tunes, int numTunes, Note* G_note);

#endif