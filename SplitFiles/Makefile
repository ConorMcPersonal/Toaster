game_CODE.bin: *.c *.h
	z88dk.zcc +zx -startup=1 -clib=sdcc_iy -D_TEST_GAME  attributes_text.c numbers.c face.c slot.c slot_monitor.c game.c control.c music.c util.c bread.c customer.c base.c -o game -create-app
