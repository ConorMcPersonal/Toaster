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

#define BUFFERLEN 10

void screenNumber(const unsigned int x, const unsigned int y, int numberIn)
{
    char buffer[10];
    int i;
    pair here = {x - 1, y - 1};
    if (numberIn < 0) {
        screenMinus(&here);
        numberIn = abs(numberIn);
    }
    itoa(numberIn, buffer, 10);
    for (i = 0; i < BUFFERLEN; i++) {
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
            default:
                // we are done
                i = BUFFERLEN;
                break;
        }
    }
}

pair* screenOne(pair* pairIn)
{
    int x = pairIn->x;
    int px = x * 8;
    int y = pairIn->y;
    int py = y * 8;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x30;
    *zx_pxy2saddr(px, py++) = 0x10;
    *zx_pxy2saddr(px, py++) = 0x10;
    *zx_pxy2saddr(px, py++) = 0x10;
    *zx_pxy2saddr(px, py++) = 0x10;
    *zx_pxy2saddr(px, py++) = 0x38;
    *zx_pxy2saddr(px, py) = 0x0;
    pairIn->x = x + 1;
    return pairIn;
}

pair* screenTwo(pair* pairIn)
{
    int x = pairIn->x;
    int px = x * 8;
    int y = pairIn->y;
    int py = y * 8;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x38;
    *zx_pxy2saddr(px, py++) = 0x28;
    *zx_pxy2saddr(px, py++) = 0x10;
    *zx_pxy2saddr(px, py++) = 0x10;
    *zx_pxy2saddr(px, py++) = 0x20;
    *zx_pxy2saddr(px, py++) = 0x38;
    *zx_pxy2saddr(px, py) = 0x0;
    pairIn->x = x + 1;
    return pairIn;
}

pair* screenThree(pair* pairIn)
{
    int x = pairIn->x;
    int px = x * 8;
    int y = pairIn->y;
    int py = y * 8;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x18;
    *zx_pxy2saddr(px, py++) = 0x28;
    *zx_pxy2saddr(px, py++) = 0x18;
    *zx_pxy2saddr(px, py++) = 0x8;
    *zx_pxy2saddr(px, py++) = 0x8;
    *zx_pxy2saddr(px, py++) = 0x38;
    *zx_pxy2saddr(px, py) = 0x0;
    pairIn->x = x + 1;
    return pairIn;
}

pair* screenFour(pair* pairIn)
{
    int x = pairIn->x;
    int px = x * 8;
    int y = pairIn->y;
    int py = y * 8;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x20;
    *zx_pxy2saddr(px, py++) = 0x20;
    *zx_pxy2saddr(px, py++) = 0x28;
    *zx_pxy2saddr(px, py++) = 0x28;
    *zx_pxy2saddr(px, py++) = 0x3C;
    *zx_pxy2saddr(px, py++) = 0x08;
    *zx_pxy2saddr(px, py) = 0x0;
    pairIn->x = x + 1;
    return pairIn;
}

pair* screenFive(pair* pairIn)
{
    int x = pairIn->x;
    int px = x * 8;
    int y = pairIn->y;
    int py = y * 8;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x3C;
    *zx_pxy2saddr(px, py++) = 0x20;
    *zx_pxy2saddr(px, py++) = 0x3C;
    *zx_pxy2saddr(px, py++) = 0x04;
    *zx_pxy2saddr(px, py++) = 0x04;
    *zx_pxy2saddr(px, py++) = 0x3C;
    *zx_pxy2saddr(px, py) = 0x0;
    pairIn->x = x + 1;
    return pairIn;
}

pair* screenSix(pair* pairIn)
{
    int x = pairIn->x;
    int px = x * 8;
    int y = pairIn->y;
    int py = y * 8;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x3C;
    *zx_pxy2saddr(px, py++) = 0x20;
    *zx_pxy2saddr(px, py++) = 0x20;
    *zx_pxy2saddr(px, py++) = 0x3C;
    *zx_pxy2saddr(px, py++) = 0x34;
    *zx_pxy2saddr(px, py++) = 0x3C;
    *zx_pxy2saddr(px, py) = 0x0;
    pairIn->x = x + 1;
    return pairIn;
}

pair* screenSeven(pair* pairIn)
{
    int x = pairIn->x;
    int px = x * 8;
    int y = pairIn->y;
    int py = y * 8;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x3E;
    *zx_pxy2saddr(px, py++) = 0x04;
    *zx_pxy2saddr(px, py++) = 0x1E;
    *zx_pxy2saddr(px, py++) = 0x08;
    *zx_pxy2saddr(px, py++) = 0x18;
    *zx_pxy2saddr(px, py++) = 0x10;
    *zx_pxy2saddr(px, py) = 0x0;
    pairIn->x = x + 1;
    return pairIn;
}

pair* screenEight(pair* pairIn)
{
    int x = pairIn->x;
    int px = x * 8;
    int y = pairIn->y;
    int py = y * 8;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x38;
    *zx_pxy2saddr(px, py++) = 0x28;
    *zx_pxy2saddr(px, py++) = 0x10;
    *zx_pxy2saddr(px, py++) = 0x28;
    *zx_pxy2saddr(px, py++) = 0x28;
    *zx_pxy2saddr(px, py++) = 0x10;
    *zx_pxy2saddr(px, py) = 0x0;
    pairIn->x = x + 1;
    return pairIn;
}

pair* screenNine(pair* pairIn)
{
    int x = pairIn->x;
    int px = x * 8;
    int y = pairIn->y;
    int py = y * 8;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x38;
    *zx_pxy2saddr(px, py++) = 0x28;
    *zx_pxy2saddr(px, py++) = 0x18;
    *zx_pxy2saddr(px, py++) = 0x8;
    *zx_pxy2saddr(px, py++) = 0x8;
    *zx_pxy2saddr(px, py++) = 0x8;
    *zx_pxy2saddr(px, py) = 0x0;
    pairIn->x = x + 1;
    return pairIn;
}

pair* screenMinus(pair* pairIn)
{
    int x = pairIn->x;
    int px = x * 8;
    int y = pairIn->y;
    int py = y * 8;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x78;
    *zx_pxy2saddr(px, py++) = 0x78;
    *zx_pxy2saddr(px, py++) = 0x78;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px, py) = 0x0;
    pairIn->x = x + 1;
    return pairIn;
}


pair* screenZero(pair* pairIn)
{
    int x = pairIn->x;
    int px = x * 8;
    int y = pairIn->y;
    int py = y * 8;
    *zx_pxy2saddr(px, py++) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x3C;
    *zx_pxy2saddr(px, py++) = 0x24;
    *zx_pxy2saddr(px, py++) = 0x24;
    *zx_pxy2saddr(px, py++) = 0x24;
    *zx_pxy2saddr(px, py++) = 0x24;
    *zx_pxy2saddr(px, py++) = 0x3C;
    *zx_pxy2saddr(px, py) = 0x0;
    pairIn->x = x + 1;
    return pairIn;

}
