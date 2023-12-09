# Packaging

The package will contain a basic loader, an attributes code file and then the game itself.

This is useful:
https://www.z88dk.org/forum/viewtopic.php?p=15821&sid=ff45c2c0dda6baac623d276263cf7e69#p15821

## Building the attributes code file

The attributes cane be built by following the compile instructions in attributes_text.c to build this as a standalone app.  
Then get to the end of the sequence and push buttons until you get an attribute set you are happy with.
At that point interrupt the emulator and save 768 bytes of memory starting at 22528 (0x5800) to screen_attr.code.

That can be converted to a tap file like so:

z88dk-appmake +zx -b screen_attr.code -o screen_attr.tap --org 22528 --noloader --blockname attrs

## Building the game code file

To build the code file we need to know where the program starts.  By default this seems to be 33156, but this can be
read from the original basic loader in the standard .tap file produced by z88dk.  To see this, set Fuse to run without
"tape traps" (in Options/Media) to have it load in real time, then hit Ctrl-Space once the first element is loaded.

Assuming 33156 is correct, create the tap like so:

z88dk-appmake +zx -b game_CODE.bin -o game_code.tap --org 33156 --noloader --blockname game

## Writing your own loader

The loader need to be written in Spectrum Basic on the emulator and then saved to a tap file.  Suggested:

10  CLEAR 33155: INK 0 : PAPER 0 : BORDER 0

14 REM CLEAR is on X, Clear number should be 1 less than the program start address

15 REM Colon is Ctrl-Z

16 REM Ink is Ctrl-Shift to get to extended mode, then Shift-x.

17 REM Paper is the same but Shift-c.  BORDER is on B30

20 CLS

25 REM CLS is on V 

30 LOAD "" CODE

35 REM Attributes - LOAD is on J, CODE is Extended mode and I

40 LOAD "" CODE : RANDOMIZE USR 33156

45 REM Game - RAND is T, USR is Extended-L

Save this to TAP from Fuse - make sure your tape is clear (Media/Tape/Clear), then SAVE "Loader" LINE 10 (save is on S, line is on ext-ctrl-3), then Media/Write to NewLoader.tap.

# Final Packaging

Pulling that all together is quite easy - in cygwin/mingw/git just:

cat NewLoader.tap screen_attr.tap game_code.tap > ToastHost.tap
