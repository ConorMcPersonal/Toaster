#ifndef _DEFINE_UTIL_H
#define _DEFINE_UTIL_H


#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define PRINTAT "\x16"
#define INK     "\x10"
#define PAPER   "\x11"
#define FLASHON  "\x12\x31"
#define FLASHOFF "\x12\x30"

#define SCREENSTART 0x4000
#define ATTRSTART   0x5800

// frame counter
void start_frame_count();
extern unsigned int G_frames;
extern unsigned int G_frames_local;

#endif