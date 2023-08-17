game_CODE.bin: *.c *.h
	z88dk.zcc +zx -startup=1 -clib=sdcc_iy -D_TEST_GAME slot.c slot_monitor.c game.c control.c -o game -create-app
