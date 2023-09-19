#include <stdio.h>
#include <stdlib.h>
#include <arch/zx.h>
#include <stdbool.h>

#include "numbers.h"
#include "util.h"

/* Decalations for numbers code that doesn't need to be shared */

struct pairStruct {
    int x;
    int y;
};

typedef struct pairStruct pair;

pair* screenBlank(pair* st);
pair* screenOne(pair* st);
pair* screenTwo(pair* st);
pair* screenThree(pair* st);
pair* screenFour(pair* st);
pair* screenFive(pair* st);
pair* screenSix(pair* st);
pair* screenSeven(pair* st);
pair* screenEight(pair* st);
pair* screenNine(pair* st);
pair* screenZero(pair* st);
pair* screenMinus(pair* st);

#define BUFFERLEN 7

void screenNumber(const unsigned int x, const unsigned int y, int numberIn)
{
    char buffer[BUFFERLEN];
    int i;
    int correction;
    int numLength;
    pair here = {x - 1, y - 1};
    itoa(numberIn, buffer, 10);
    for (numLength = 0; numLength < BUFFERLEN; numLength++) {
        if (buffer[numLength] == '\0') {
            break;
        }
    }
    correction = (BUFFERLEN - 1) - numLength;
    for (i = 0; i < correction; i++) {
        //blank out leading spaces
        screenBlank(&here);
    }
    for (i = 0; i < numLength; i++) {
        switch(buffer[i]) {
            case '0':
                screenZero(&here);
                break;
            case '1':
                screenOne(&here);
                break;
            case '2':
                screenTwo(&here);
                break;
            case '3':
                screenThree(&here);
                break;
            case '4':
                screenFour(&here);
                break;
            case '5':
                screenFive(&here);
                break;
            case '6':
                screenSix(&here);
                break;
            case '7':
                screenSeven(&here);
                break;
            case '8':
                screenEight(&here);
                break;
            case '9':
                screenNine(&here);
                break;
            case '-':
                screenMinus(&here);
                break;
            default:
                // we are done
                i = BUFFERLEN;
                break;
        }
    }
}

pair* screenBlank(pair* pairIn)
{
    int i;
    int x = pairIn->x;
    int px = x * 8;
    int y = pairIn->y;
    int py = y * 8;
    for (i = 0; i < 16; i++) {
        *zx_pxy2saddr(px + 8, py) = 0x0;
        *zx_pxy2saddr(px, py++) = 0x0;
    }
    pairIn->x = x + 2;
    return pairIn;
}

pair* screenOne(pair* pairIn)
{
    int x = pairIn->x;
    int px = x * 8;
    int y = pairIn->y;
    int py = y * 8;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0xE0;
    *zx_pxy2saddr(px, py++) = 0x0F;
    *zx_pxy2saddr(px + 8, py) = 0xE0;
    *zx_pxy2saddr(px, py++) = 0x13;
    *zx_pxy2saddr(px + 8, py) = 0xA0;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0xE0;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0xE0;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0xE0;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0xA0;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0xA0;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0xA0;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0xE0;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0xE0;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0xE0;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0xF8;
    *zx_pxy2saddr(px, py++) = 0x01;
    *zx_pxy2saddr(px + 8, py) = 0xFC;
    *zx_pxy2saddr(px, py++) = 0x1F;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x0;
    pairIn->x = x + 2;
    return pairIn;
}

pair* screenTwo(pair* pairIn)
{
   int x = pairIn->x;
    int px = x * 8;
    int y = pairIn->y;
    int py = y * 8;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0xf8;
    *zx_pxy2saddr(px, py++) = 0x3f;
    *zx_pxy2saddr(px + 8, py) = 0xe8;
    *zx_pxy2saddr(px, py++) = 0x35;
    *zx_pxy2saddr(px + 8, py) = 0xf8;
    *zx_pxy2saddr(px, py++) = 0x9f;
    *zx_pxy2saddr(px + 8, py) = 0x38;
    *zx_pxy2saddr(px, py++) = 0x60;
    *zx_pxy2saddr(px + 8, py) = 0x70;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0xf0;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0xc0;
    *zx_pxy2saddr(px, py++) = 0x3;
    *zx_pxy2saddr(px + 8, py) = 0x80;
    *zx_pxy2saddr(px, py++) = 0x3;
    *zx_pxy2saddr(px + 8, py) = 0x80;
    *zx_pxy2saddr(px, py++) = 0x2;
    *zx_pxy2saddr(px + 8, py) = 0x80;
    *zx_pxy2saddr(px, py++) = 0x3;
    *zx_pxy2saddr(px + 8, py) = 0xc;
    *zx_pxy2saddr(px, py++) = 0x7;
    *zx_pxy2saddr(px + 8, py) = 0xc;
    *zx_pxy2saddr(px, py++) = 0x1f;
    *zx_pxy2saddr(px + 8, py) = 0xfc;
    *zx_pxy2saddr(px, py++) = 0x2b;
    *zx_pxy2saddr(px + 8, py) = 0xfc;
    *zx_pxy2saddr(px, py++) = 0x3f;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x0;
    pairIn->x = x + 2;
    return pairIn;
}

pair* screenThree(pair* pairIn)
{
   int x = pairIn->x;
    int px = x * 8;
    int y = pairIn->y;
    int py = y * 8;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0xc0;
    *zx_pxy2saddr(px, py++) = 0x1f;
    *zx_pxy2saddr(px + 8, py) = 0xe0;
    *zx_pxy2saddr(px, py++) = 0x3f;
    *zx_pxy2saddr(px + 8, py) = 0x38;
    *zx_pxy2saddr(px, py++) = 0x30;
    *zx_pxy2saddr(px + 8, py) = 0x38;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0x38;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0xc0;
    *zx_pxy2saddr(px, py++) = 0x7;
    *zx_pxy2saddr(px + 8, py) = 0xc0;
    *zx_pxy2saddr(px, py++) = 0x7;
    *zx_pxy2saddr(px + 8, py) = 0x38;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0x38;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0x38;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0x30;
    *zx_pxy2saddr(px, py++) = 0x30;
    *zx_pxy2saddr(px + 8, py) = 0x20;
    *zx_pxy2saddr(px, py++) = 0x30;
    *zx_pxy2saddr(px + 8, py) = 0xc0;
    *zx_pxy2saddr(px, py++) = 0x3f;
    *zx_pxy2saddr(px + 8, py) = 0xc0;
    *zx_pxy2saddr(px, py++) = 0xf;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x0;
    pairIn->x = x + 2;
    return pairIn;
}

pair* screenFour(pair* pairIn)
{
    int x = pairIn->x;
    int px = x * 8;
    int y = pairIn->y;
    int py = y * 8;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0xe0;
    *zx_pxy2saddr(px, py++) = 0x3f;
    *zx_pxy2saddr(px + 8, py) = 0xe0;
    *zx_pxy2saddr(px, py++) = 0x3f;
    *zx_pxy2saddr(px + 8, py) = 0xe0;
    *zx_pxy2saddr(px, py++) = 0x3f;
    *zx_pxy2saddr(px + 8, py) = 0xe0;
    *zx_pxy2saddr(px, py++) = 0x38;
    *zx_pxy2saddr(px + 8, py) = 0xe0;
    *zx_pxy2saddr(px, py++) = 0x38;
    *zx_pxy2saddr(px + 8, py) = 0xe0;
    *zx_pxy2saddr(px, py++) = 0x38;
    *zx_pxy2saddr(px + 8, py) = 0xe0;
    *zx_pxy2saddr(px, py++) = 0x38;
    *zx_pxy2saddr(px + 8, py) = 0xe0;
    *zx_pxy2saddr(px, py++) = 0x38;
    *zx_pxy2saddr(px + 8, py) = 0xe0;
    *zx_pxy2saddr(px, py++) = 0x38;
    *zx_pxy2saddr(px + 8, py) = 0xfc;
    *zx_pxy2saddr(px, py++) = 0x3f;
    *zx_pxy2saddr(px + 8, py) = 0xfc;
    *zx_pxy2saddr(px, py++) = 0x3f;
    *zx_pxy2saddr(px + 8, py) = 0xfc;
    *zx_pxy2saddr(px, py++) = 0x3f;
    *zx_pxy2saddr(px + 8, py) = 0xe0;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0xe0;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x0;
    pairIn->x = x + 2;
    return pairIn;
}

pair* screenFive(pair* pairIn)
{
    int x = pairIn->x;
    int px = x * 8;
    int y = pairIn->y;
    int py = y * 8;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0xf8;
    *zx_pxy2saddr(px, py++) = 0x1f;
    *zx_pxy2saddr(px + 8, py) = 0xfc;
    *zx_pxy2saddr(px, py++) = 0x1f;
    *zx_pxy2saddr(px + 8, py) = 0x4;
    *zx_pxy2saddr(px, py++) = 0x18;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x18;
    *zx_pxy2saddr(px + 8, py) = 0xf0;
    *zx_pxy2saddr(px, py++) = 0xf;
    *zx_pxy2saddr(px + 8, py) = 0xf8;
    *zx_pxy2saddr(px, py++) = 0xf;
    *zx_pxy2saddr(px + 8, py) = 0x38;
    *zx_pxy2saddr(px, py++) = 0x80;
    *zx_pxy2saddr(px + 8, py) = 0x28;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0x28;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0x38;
    *zx_pxy2saddr(px, py++) = 0x10;
    *zx_pxy2saddr(px + 8, py) = 0x38;
    *zx_pxy2saddr(px, py++) = 0x18;
    *zx_pxy2saddr(px + 8, py) = 0xf0;
    *zx_pxy2saddr(px, py++) = 0x1f;
    *zx_pxy2saddr(px + 8, py) = 0xb0;
    *zx_pxy2saddr(px, py++) = 0x14;
    *zx_pxy2saddr(px + 8, py) = 0xe0;
    *zx_pxy2saddr(px, py++) = 0x1f;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x0;
    pairIn->x = x + 2;
    return pairIn;
}

pair* screenSix(pair* pairIn)
{
    int x = pairIn->x;
    int px = x * 8;
    int y = pairIn->y;
    int py = y * 8;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0xf8;
    *zx_pxy2saddr(px, py++) = 0x1f;
    *zx_pxy2saddr(px + 8, py) = 0xfc;
    *zx_pxy2saddr(px, py++) = 0x1f;
    *zx_pxy2saddr(px + 8, py) = 0xc;
    *zx_pxy2saddr(px, py++) = 0x1c;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x14;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x1c;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x1c;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x16;
    *zx_pxy2saddr(px + 8, py) = 0xf8;
    *zx_pxy2saddr(px, py++) = 0x1f;
    *zx_pxy2saddr(px + 8, py) = 0xf8;
    *zx_pxy2saddr(px, py++) = 0x10;
    *zx_pxy2saddr(px + 8, py) = 0x38;
    *zx_pxy2saddr(px, py++) = 0x10;
    *zx_pxy2saddr(px + 8, py) = 0x38;
    *zx_pxy2saddr(px, py++) = 0x10;
    *zx_pxy2saddr(px + 8, py) = 0x18;
    *zx_pxy2saddr(px, py++) = 0x10;
    *zx_pxy2saddr(px + 8, py) = 0x98;
    *zx_pxy2saddr(px, py++) = 0x1f;
    *zx_pxy2saddr(px + 8, py) = 0xf8;
    *zx_pxy2saddr(px, py++) = 0x1f;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x0;
    pairIn->x = x + 2;
    return pairIn;
}

pair* screenSeven(pair* pairIn)
{
    int x = pairIn->x;
    int px = x * 8;
    int y = pairIn->y;
    int py = y * 8;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0xfc;
    *zx_pxy2saddr(px, py++) = 0xc;
    *zx_pxy2saddr(px + 8, py) = 0xfc;
    *zx_pxy2saddr(px, py++) = 0x7;
    *zx_pxy2saddr(px + 8, py) = 0x18;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0x30;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0x60;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0xc0;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0x80;
    *zx_pxy2saddr(px, py++) = 0x1;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x3;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x6;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x6;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x6;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x6;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x6;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x6;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x0;
    pairIn->x = x + 2;
    return pairIn;
}

pair* screenEight(pair* pairIn)
{
    int x = pairIn->x;
    int px = x * 8;
    int y = pairIn->y;
    int py = y * 8;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0xf0;
    *zx_pxy2saddr(px, py++) = 0xf;
    *zx_pxy2saddr(px + 8, py) = 0x8;
    *zx_pxy2saddr(px, py++) = 0x10;
    *zx_pxy2saddr(px + 8, py) = 0x8;
    *zx_pxy2saddr(px, py++) = 0x10;
    *zx_pxy2saddr(px + 8, py) = 0x8;
    *zx_pxy2saddr(px, py++) = 0x10;
    *zx_pxy2saddr(px + 8, py) = 0x8;
    *zx_pxy2saddr(px, py++) = 0x10;
    *zx_pxy2saddr(px + 8, py) = 0x10;
    *zx_pxy2saddr(px, py++) = 0x8;
    *zx_pxy2saddr(px + 8, py) = 0xc0;
    *zx_pxy2saddr(px, py++) = 0x3;
    *zx_pxy2saddr(px + 8, py) = 0xc0;
    *zx_pxy2saddr(px, py++) = 0x3;
    *zx_pxy2saddr(px + 8, py) = 0x10;
    *zx_pxy2saddr(px, py++) = 0x8;
    *zx_pxy2saddr(px + 8, py) = 0x8;
    *zx_pxy2saddr(px, py++) = 0x10;
    *zx_pxy2saddr(px + 8, py) = 0x8;
    *zx_pxy2saddr(px, py++) = 0x10;
    *zx_pxy2saddr(px + 8, py) = 0x8;
    *zx_pxy2saddr(px, py++) = 0x10;
    *zx_pxy2saddr(px + 8, py) = 0x8;
    *zx_pxy2saddr(px, py++) = 0x10;
    *zx_pxy2saddr(px + 8, py) = 0xf0;
    *zx_pxy2saddr(px, py++) = 0xf;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x0;
    pairIn->x = x + 2;
    return pairIn;
}

pair* screenNine(pair* pairIn)
{
    int x = pairIn->x;
    int px = x * 8;
    int y = pairIn->y;
    int py = y * 8;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0xf0;
    *zx_pxy2saddr(px, py++) = 0x1f;
    *zx_pxy2saddr(px + 8, py) = 0x70;
    *zx_pxy2saddr(px, py++) = 0x1d;
    *zx_pxy2saddr(px + 8, py) = 0x30;
    *zx_pxy2saddr(px, py++) = 0x10;
    *zx_pxy2saddr(px + 8, py) = 0x30;
    *zx_pxy2saddr(px, py++) = 0x10;
    *zx_pxy2saddr(px + 8, py) = 0x70;
    *zx_pxy2saddr(px, py++) = 0x1d;
    *zx_pxy2saddr(px + 8, py) = 0xf0;
    *zx_pxy2saddr(px, py++) = 0x1f;
    *zx_pxy2saddr(px + 8, py) = 0x10;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0x30;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0x70;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0x70;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0x50;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0x50;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0x70;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0x70;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x0;
    pairIn->x = x + 2;
    return pairIn;
}

pair* screenMinus(pair* pairIn)
{
    int x = pairIn->x;
    int px = x * 8;
    int y = pairIn->y;
    int py = y * 8;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0xf0;
    *zx_pxy2saddr(px, py++) = 0xf;
    *zx_pxy2saddr(px + 8, py) = 0xf0;
    *zx_pxy2saddr(px, py++) = 0xf;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x0;
    pairIn->x = x + 2;
    return pairIn;
}


pair* screenZero(pair* pairIn)
{
    int x = pairIn->x;
    int px = x * 8;
    int y = pairIn->y;
    int py = y * 8;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px + 8, py) = 0x78;
    *zx_pxy2saddr(px, py++) = 0x3F;
    *zx_pxy2saddr(px + 8, py) = 0x0C;
    *zx_pxy2saddr(px, py++) = 0x40;
    *zx_pxy2saddr(px + 8, py) = 0x14;
    *zx_pxy2saddr(px, py++) = 0x40;
    *zx_pxy2saddr(px + 8, py) = 0x24;
    *zx_pxy2saddr(px, py++) = 0x40;
    *zx_pxy2saddr(px + 8, py) = 0x44;
    *zx_pxy2saddr(px, py++) = 0x40;
    *zx_pxy2saddr(px + 8, py) = 0x84;
    *zx_pxy2saddr(px, py++) = 0x40;
    *zx_pxy2saddr(px + 8, py) = 0x04;
    *zx_pxy2saddr(px, py++) = 0x01;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x42;
    *zx_pxy2saddr(px + 8, py) = 0x04;
    *zx_pxy2saddr(px, py++) = 0x44;
    *zx_pxy2saddr(px + 8, py) = 0x04;
    *zx_pxy2saddr(px, py++) = 0x48;
    *zx_pxy2saddr(px + 8, py) = 0x04;
    *zx_pxy2saddr(px, py++) = 0x50;
    *zx_pxy2saddr(px + 8, py) = 0x04;
    *zx_pxy2saddr(px, py++) = 0x60;
    *zx_pxy2saddr(px + 8, py) = 0x06;
    *zx_pxy2saddr(px, py++) = 0x60;
    *zx_pxy2saddr(px + 8, py) = 0xF8;
    *zx_pxy2saddr(px, py++) = 0x3E;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x0;
    pairIn->x = x + 2;
    return pairIn;

}
