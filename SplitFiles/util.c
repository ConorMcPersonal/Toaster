// sdcc compile with newlib
// zcc +zx -vn -startup=0 -clib=sdcc_iy -D_TEST_UTIL util.c -o util -create-app
//
// sdcc compile with newlib and SO3 to enable aggressive rules to get rid of some facepalms in generated code
// zcc +zx -startup=1 -clib=sdcc_iy -SO3 frames_sdcc.c -o frames_sdcc -create-app
//
// sdcc compile with max optimization = much slower compile times
// zcc +zx -startup=1 -clib=sdcc_iy -SO3 --max-allocs-per-node200000 frames_sdcc.c -o frames_sdcc -create-app
//
// sccz80 compile with newlib
// zcc +zx -startup=1 -clib=new frames_sdcc.c -o frames_sdcc -create-app
//
// There is a new startup=30 that is lighter weight as it uses the rom to print
// There is no scanf stream when this startup is used.
// zcc +zx -startup=30 -clib=sdcc_iy frames_sdcc.c -o frames_sdcc -create-app

// Pragmas modify the crt's startup and initialization.
// The pragmas are stored in file "zcc_opt.def" as the program is compiled
// and the crt will read that file to learn of options different from the defaults.
// Judicious use of pragmas can eliminate features you're not using and reduce binary size.

#pragma output CRT_ORG_CODE = 0x8184   // move org just above the im2 table and jump (see below)
//#pragma printf = "%u"                  // let's cut out the printf converters you're not using

// In large programs it's usually better to put pragmas in a standalone file like "zpragma.inc"
// and then add that to the compile line with "-zpragma-include:zpragma.inc"

#include <stdio.h>
#include <string.h>        // memset
#include <im2.h>           // im2 macros and functions
#include <intrinsic.h>
#include <z80.h>           // bpoke, etc
#include "util.h"

// frame counter

unsigned int G_frames = 0;
unsigned int G_frames_local = 0;

// There are a lot of ways to define an interrupt service routine
// This one is being done with a macro.  What makes a regular function
// different from an ISR is that the ISR must preserve cpu registers
// it is using and enable interrupts before returning with the special
// instruction "reti".  This macro creates a function called "isr" that
// does these things.  The 8080 in its name indicates it only saves
// the main registers af,bc,de,hl.  If you have code in there that modifies
// other registers (including c library code) you can use the IM2_DEFINE_ISR
// macro instead as that will save all of the z80's registers.  This
// is also a place where you can put ay music, etc.

IM2_DEFINE_ISR_8080(isr)
{
   // update the clock
   ++G_frames;
   ++G_frames_local;
}

void start_frame_count() {
    // set up im2 routine
    // newlib has interrupts disabled at this point
    
    im2_init((void*)0x8000);           // set z80's I register to point at 0x8000, set im2 mode
    memset((void*)0x8000, 0x81, 257);  // make a 257-byte im2 table at 0x8000 containg 0x81 bytes

    // On interrupt the z80 will jump to address 0x8181

    z80_bpoke(0x8181, 0xc3);                // z80 JP instruction
    z80_wpoke(0x8182, (unsigned int)isr);   // to the isr routine
    
    // Now enable interrupts
    
    intrinsic_ei();  // inlining ei would disturb the optimizer
}

void util_main()
{
    unsigned int j = 0;
    unsigned char i = 0;
    
    start_frame_count();

    for (i=0; i<10; i++)
    {
        printf("%u\n", G_frames);
        
        for (j=0; j<10000; j++);
    }
         
         // Returning to basic will crash because the org has moved to 0x8184
         // and the basic loader that z88dk is providing is doing "CLEAR 0x8183".
         // This is not low enough because we are putting stuff at 0x8000 and with
         // that clear address, that stuff is overwriting some of basic's stack.
         // So a return with crash because basic's stack has been contaminated.
         // You can write your own loader to fix this problem if you want to return
         // to basic or you can place the im2 table at a higher address but I and a lot
         // of people like placing the im2 table below the program.
         
         while (1) ;
}

#ifdef _TEST_UTIL
void main() {
    util_main();
}

#endif
