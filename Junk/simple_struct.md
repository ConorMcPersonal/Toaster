# Simple Struct

Just a code test looking at the idea of having a struct template as a basic 'block' of code for Toaster.  Currently called GameCOmponent.

Game runs constantly on a main loop. In that loop the first 'block' gets called.  It is passed a set of parameters (the global game state).
Each block can do whatever it likes, manipulate the screen, read the keyboard, play sound, update its own state.
It can also update the paraneters it has been passed.  Once its work is completed, it calls the next block in the sequence
passing the (possibly updated) paraneters to it.

Because of C typing each strict needs to have the same member variables, signatures etc.  Each one will need, at a minimum:

* a void pointer to any malloc'd state.  That state is likely to also be a struct so the pointer will need to be cast to the right type to be used
* a 'constructor' function pointer to do any startup (eg malloc somehwere to striore the state): void <func>(GameComponent* input)
 constructor
* a game function, called once per loop void <func>(GameComponent* input, GameParameters* params)

These features are not in the sample as yet!

As an example, a toaster slot could be one type of GameComponent.  It would store state about the type of bread in the slot, whether it is off or on, temperature etc.  It would also know what part of the screen to use for display (to keep it distinct from other slots if there are any).

Compiler command line:
zcc +zx -vn -startup=1 -clib=sdcc_iy simple_struct.c -o simple_struct -create-app

Output:
simple_struct.tap -- can be loaded into pretty much any Spectrum emulator