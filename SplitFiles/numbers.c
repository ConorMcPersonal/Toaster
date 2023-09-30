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

const char oneData[] = {
 0x0, 0x0,
 0xf, 0xc0,
 0x1f, 0xc0,
 0x13, 0xc0,
 0x3, 0xc0,
 0x3, 0xc0,
 0x3, 0xc0,
 0x3, 0xc0,
 0x3, 0xc0,
 0x3, 0xc0,
 0x3, 0xc0,
 0x3, 0xc0,
 0x3, 0xc0,
 0x3, 0xc0,
 0xf, 0xf0,
 0x0, 0x0
};

const char twoData[] = {
 0x0, 0x0,
 0x1f, 0xc0,
 0x1f, 0xc0,
 0x30, 0xc0,
 0x30, 0xc0,
 0x30, 0xc0,
 0x0, 0xc0,
 0x1, 0xc0,
 0x3, 0x80,
 0x7, 0x0,
 0xe, 0x0,
 0x1c, 0x0,
 0x30, 0x0,
 0x3f, 0xc0,
 0x3f, 0xc0,
 0x0, 0x0
};

const char threeData[] = {
 0x0, 0x0,
 0x7, 0x80,
 0x1f, 0xf0,
 0x1c, 0x38,
 0x0, 0x38,
 0x0, 0x70,
 0x1, 0xc0,
 0x3, 0x80,
 0x0, 0xf0,
 0x0, 0x70,
 0x0, 0x70,
 0x0, 0x70,
 0x10, 0x70,
 0x1f, 0xc0,
 0x1f, 0xc0,
 0x0, 0x0,
};

const char fourData[] = {
 0x0, 0x0,
 0x7f, 0xf0,
 0x7f, 0xf0,
 0x7f, 0xf0,
 0x70, 0x70,
 0x70, 0x70,
 0x70, 0x70,
 0x70, 0x70,
 0x70, 0x70,
 0x70, 0x70,
 0x7f, 0xfe,
 0x7f, 0xfe,
 0x7f, 0xfe,
 0x0, 0x70,
 0x0, 0x70,
 0x0, 0x0
};

const char fiveData[] = {
 0x0, 0x0,
 0x1f, 0xf8,
 0x1f, 0xf8,
 0x18, 0x0,
 0x18, 0x0,
 0x1f, 0xe0,
 0xf, 0xf0,
 0x0, 0x18,
 0x0, 0x18,
 0x0, 0x18,
 0x0, 0x18,
 0x0, 0x18,
 0x18, 0x18,
 0x1f, 0xf8,
 0x1f, 0xf0,
 0x0, 0x0
};

const char sixData[] = {
 0x0, 0x0,
 0x1f, 0xf0,
 0x1f, 0xf8,
 0x18, 0x0,
 0x18, 0x0,
 0x18, 0x0,
 0x1f, 0xf8,
 0x1f, 0xf8,
 0x18, 0x18,
 0x18, 0x18,
 0x18, 0x18,
 0x18, 0x18,
 0x18, 0x18,
 0x1f, 0xf8,
 0x1f, 0xf8,
 0x0, 0x0
};

const char sevenData[] = {
 0x0, 0x0,
 0x1f, 0xf0,
 0x1f, 0xf0,
 0x0, 0x30,
 0x0, 0x30,
 0x0, 0x60,
 0x0, 0xc0,
 0x1f, 0xf0,
 0x1f, 0xf0,
 0x3, 0x0,
 0x7, 0x0,
 0xe, 0x0,
 0x18, 0x0,
 0x18, 0x0,
 0x18, 0x0,
 0x0, 0x0
};

const char eightData[] = {
 0x0, 0x0,
 0xf, 0xf8,
 0xf, 0xf8,
 0xc, 0x18,
 0xc, 0x18,
 0xc, 0x18,
 0x6, 0x30,
 0x3, 0xe0,
 0x3, 0xe0,
 0x6, 0x30,
 0xc, 0x18,
 0xc, 0x18,
 0xc, 0x18,
 0xf, 0xf8,
 0xf, 0xf8,
 0x0, 0x0
};

const char nineData[] = {
 0x0, 0x0,
 0x1f, 0xe0,
 0x3f, 0xf0,
 0x30, 0x30,
 0x30, 0x70,
 0x38, 0x70,
 0x1f, 0xf0,
 0x7, 0xb0,
 0x0, 0x30,
 0x0, 0x30,
 0x30, 0x30,
 0x30, 0x30,
 0x33, 0xe0,
 0x3f, 0xe0,
 0xe, 0x0,
 0x0, 0x0
};

const char minusData[] = {
 0x0, 0x0,
 0x0, 0x0,
 0x0, 0x0,
 0x0, 0x0,
 0x0, 0x0,
 0x0, 0x0,
 0x0, 0x0,
 0x1f, 0xf8,
 0x1f, 0xf8,
 0x0, 0x0,
 0x0, 0x0,
 0x0, 0x0,
 0x0, 0x0,
 0x0, 0x0,
 0x0, 0x0,
 0x0, 0x0
};

const char zeroData[] = {
 0x0, 0x0,
 0x1f, 0x78,
 0x38, 0x1c,
 0x60, 0xe,
 0x60, 0x16,
 0x40, 0x22,
 0x40, 0x42,
 0x0, 0x82,
 0x41, 0x0,
 0x42, 0x2,
 0x44, 0x2,
 0x68, 0x6,
 0x70, 0x6,
 0x38, 0x1c,
 0x1e, 0xf8,
 0x0, 0x0
};

/* minus is indexed at 10*/
const char* numGraphics[] = {zeroData, oneData, twoData, threeData, fourData, fiveData, sixData,
    sevenData, eightData, nineData, minusData};

pair* screenBlank(pair* st);

pair* draw_numeral(pair* pairIn, const char c) 
{
    int px = (pairIn->x) * 8;
    int py = (pairIn->y) * 8;
    const char* pix = numGraphics[c];
    char* leftPtr = zx_pxy2saddr(px, py);
    for (int j = 0; j < 2; ++j) {
        for (int i = 0; i < 8; ++i) {
            *(leftPtr) = *pix++;
            *(leftPtr + 1) = *pix++;
            leftPtr += 256;
        }
        if (j == 0) {
            leftPtr = zx_pxy2saddr(px, py + 8);
        }
    }
    pairIn->x += 2;
    return pairIn;
}

void draw_number(const unsigned int x, const unsigned int y, const char c)
{
    pair drawAt = {x, y};
    draw_numeral(&drawAt, c);
}

#define BUFFERLEN 7

void screenTime(const unsigned int x, const unsigned int y, const int hour, const int min)
{
    pair here = {x - 1, y - 1};
    if (hour < 10) {
        draw_numeral(&here, 0);
    }
    switch (hour) {
        case 7:
        case 8:
        case 9:
            draw_numeral(&here, hour);
            break;
        case 10:
            draw_numeral(&here, 1);
            draw_numeral(&here, 0);
            break;
        default:
            draw_numeral(&here, 1);
            draw_numeral(&here, 1);
    }

    draw_numeral(&here, 10);

    switch (min) {
        case 0:
            draw_numeral(&here, 0);
            draw_numeral(&here, 0);
            break;
        case 15:
            draw_numeral(&here, 1);
            draw_numeral(&here, 5);
            break;
        case 30:
            draw_numeral(&here, 3);
            draw_numeral(&here, 0);
            break;
        default:
            draw_numeral(&here, 4);
            draw_numeral(&here, 5);
    }
}

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
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                draw_numeral(&here, buffer[i]);
                break;
            case '-':
                draw_numeral(&here, 10);
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

void screenClear(const unsigned int x, const unsigned int y)
{
    pair clearAt = {x, y};
    screenBlank(&clearAt);
}
