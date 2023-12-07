#include <arch/zx.h>
#include <stdlib.h>
#include <input.h>

#include "attributes_text.h"

// Compile with zcc +zx -startup=1 -clib=sdcc_iy -D_TEST_ATTRSCREEN attributes_text.c -o attr -create-app

const char t5[] = {
    0xF8, //0b11111000
    0x20, //0b00100000
    0x20, //0b00100000
    0x20, //0b00100000
    0x20  //0b00100000
    };

const char o5[] = {
    0x70, //0b01110000
    0x88, //0b10001000
    0x88, //0b10001000
    0x88, //0b10001000
    0x70  //0b01110000
    };

const char a5[] = {
    0x20, //0b00100000
    0x50, //0b01010000
    0x88, //0b10001000
    0xF8, //0b11111000
    0x88  //0b10001000
    };

const char s5[] = {
    0x70, //0b01110000
    0x80, //0b10000000
    0x60, //0b01100000
    0x10, //0b00010000
    0xE0  //0b11100000
    };

const char h5[] = {
    0x88, //0b10001000
    0x88, //0b10001000
    0xF8, //0b11111000
    0x88, //0b10001000
    0x88  //0b10001000
    };

const char t3[] = {
    0xE0, //### //07
    0x40, //.#. //02
    0x40, //.#. //02
    0x40, //.#. //02
    0x40  //.#. //02
    };

const char o3[] = {
    0x40, //.#. //02
    0xA0, //#.# //05
    0xA0, //#.# //05
    0xA0, //#.# //05
    0x40  //.#. //02
    };

const char a3[] = {
    0x40, //.#. //02
    0xA0, //#.# //05
    0xE0, //### //07
    0xA0, //#.# //05
    0xA0  //#.# //05
    };

const char s3[] = {
    0x60, //.## //03
    0x80, //#.. //04
    0x60, //.## //03
    0x20, //..# //01
    0xC0  //##. //06
};

const char h3[] = {
    0xA0, //#.# //05
    0xA0, //#.# //05
    0xE0, //### //07
    0xA0, //#.# //05
    0xA0  //#.# //05
};

AttrFont font3 = {
    3,
    5,
    (const char *)&t3,
    (const char *)&o3,
    (const char *)&a3,
    (const char *)&s3,
    (const char *)&h3
};

AttrFont font5 = {
    5,
    5,
    (const char *)&t5,
    (const char *)&o5,
    (const char *)&a5,
    (const char *)&s5,
    (const char *)&h5
};

void attr_write(AttrFont* font, char letter, int x, int y, char colour) {
    char* draw;
    char row;
    int i, j;
    switch (letter) {
        case 't':
            draw = (char *)font->t;
            break;
        case 'o':
            draw = (char *)font->o;
            break;
        case 'a':
            draw = (char *)font->a;
            break;
        case 's':
            draw = (char *)font->s;
            break;
        case 'h':
        default:
            draw = (char *)font->h;
    }
    for (j = 0; j < font->height; ++j) {
        row = *draw;
        if (y + j < 24) {
            for (i = 0; i < font->width; ++i) {
                if (x + i < 32) {
                    if ((row & 0x80) == 0x80) {
                        *zx_cxy2aaddr(x + i, y + j) |= (colour |= FLASH);
                    }
                    row = row<<1;
                }
            }
            ++draw;
        }
    }
}

char random_colour(int ink) {
    int randcol = rand() % 6 + 1;
    if (ink) {
        return randcol;
    } else {
        return randcol * 8;
    }
}

int write_thin(char letter, int x, int y, char colour) {
    attr_write(&font3, letter, x, y, colour);
    return (&font3)->width;
}

int write_fat(char letter, int x, int y, char colour) {
    attr_write(&font5, letter, x, y, colour);
    return (&font5)->width;
}

void write_block(int x1, int y1, int x2, int y2) {
    int i, j;
    for (i = x1; i <= x2; ++i) {
        for (j = y1; j <= y2; ++j) {
            *zx_cxy2aaddr(i, j) = INK_BLACK | PAPER_BLACK;
        }
    }
    i = x1;
    i += write_thin('t', i, y1 + 1, random_colour(1));
    i += write_thin('o', i, y1 + 2, random_colour(1)) + 1;
    i += write_thin('a', i, y1    , random_colour(1)) + 1;
    i += write_thin('s', i, y1 + 2, random_colour(1));
    i += write_thin('t', i, y1 + 1, random_colour(1));
    i = x1 + 1;
    i += write_thin('h', i, y1    , random_colour(0)) + 1;
    i += write_thin('o', i, y1 + 2, random_colour(0)) + 1;
    i += write_thin('s', i, y1 + 1, random_colour(0)) + 1;
    i += write_thin('t', i, y1 + 2, random_colour(0)) + 1;

}

#ifdef _TEST_ATTRSCREEN

int wait_for_a_new_key() {
    unsigned char entry = in_inkey();
    unsigned char keyp = entry;
    while (keyp == 0 || keyp == entry) {
      keyp = in_inkey();
    }
    in_wait_nokey();
    return (int)keyp;
}

int main() {
    AttrFont* use_font = &font5;
    zx_border(INK_RED);
    zx_cls(PAPER_WHITE | INK_BLACK);

    write_block(15, 16, 31, 22);
    wait_for_a_new_key();

    while (1) {
        zx_border(INK_BLACK);
        zx_cls(PAPER_BLACK | INK_BLACK);

        attr_write(use_font, 't', 1, 7, random_colour(1));
        attr_write(use_font, 'o', 7, 8, random_colour(1));
        attr_write(use_font, 'a', 13, 7, random_colour(1));
        attr_write(use_font, 's', 19, 6, random_colour(1));
        attr_write(use_font, 't', 25, 7, random_colour(1));

        attr_write(use_font, 'h', 3, 12, random_colour(0));
        attr_write(use_font, 'o', 10, 10, random_colour(0));
        attr_write(use_font, 's', 17, 13, random_colour(0));
        attr_write(use_font, 't', 23, 11, random_colour(0));
        wait_for_a_new_key();
    }
    return 0;
}

#endif
