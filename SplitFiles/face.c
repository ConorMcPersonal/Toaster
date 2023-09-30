#include <arch/zx.h>
#include <stdio.h>
#include "util.h"
#include "face.h"

void cheeky_face(const int x, const int y, char* pix) 
{
    int px = (x - 1) * 8;
    int py = (y - 1) * 8;
    char* leftPtr = zx_pxy2saddr(px, py);
    for (int j = 0; j < 2; ++j) {
        for (int i = 0; i < 8; ++i) {
            //Why right before left?
            *(leftPtr + 1) = *pix++;
            *leftPtr = *pix++;
            leftPtr += 256;
        }
        if (j == 0) {
            leftPtr = zx_pxy2saddr(px, py + 8);
        }
    }
}

void happyFace(const int x, const int y)
{
    char faceData[] = { 0, 1, 0xe0, 0xe, 0x10, 0x10, 0x08, 0x20, 0x44, 0x44, 0x4,
        0x40, 0x4, 0x40, 0x2, 0x80, 0x44, 0x44, 0x84, 0x43, 0x4, 0x40, 0x8, 
        0x20, 0x10, 0x10, 0xe0, 0xe, 0x0, 0x1, 0x0, 0x0};
    cheeky_face(x, y, (char *)&faceData);
}

void sadFace(const int x, const int y)
{
    char faceData[] = { 0x0, 0x1, 0xe0, 0xe, 0x10, 0x10, 0x8, 0x20, 0x44,
        0x44, 0x4, 0x40, 0x4, 0x40, 0x2, 0x80, 0x4, 0x40, 0x84, 0x43, 0x44, 
        0x44, 0x8, 0x20, 0x10, 0x10, 0xe0, 0xe, 0x0, 0x1, 0x0, 0x0 };
    cheeky_face(x, y, (char *)&faceData);
}


void angryFace(const int x, const int y)
{
    char faceData[] = {  0x0, 0x1, 0xe0, 0xe, 0x10, 0x10, 0x8, 0x20, 0x44, 0x44, 0x40, 
        0x4, 0x84, 0x42, 0x04, 0x40, 0x84, 0x43, 0x44, 0x44, 0x44, 0x44, 0x88, 0x23, 
        0x10, 0x10, 0xe0, 0xe, 0x0, 0x1, 0x0, 0x0 };
    cheeky_face(x, y, (char *)&faceData);
}

void neutralFace(const int x, const int y)
{
    char faceData[] = { 0x0, 0x1, 0xe0, 0xe, 0x10, 0x10, 0x08, 0x20, 0x44, 0x44, 0x4, 0x40, 0x4, 0x40, 0x2, 0x80
                            , 0x4, 0x40, 0x84, 0x43, 0x4, 0x40, 0x8, 0x20, 0x10, 0x10, 0xe0, 0xe, 0x0, 0x1, 0x0, 0x0};
    cheeky_face(x, y, (char *)&faceData);
}

enum emotion {
    ANGRY = 0, SAD = 1, NEUTRAL = 2, HAPPY = 3, START = 4
};

void screenFace(const unsigned int x, const unsigned int y, const int reputation)
{
    static enum emotion lastEmo = START;
    if (reputation < 0 && lastEmo != ANGRY) {
        angryFace(x, y);
        lastEmo = ANGRY;
    }
    else if (lastEmo != SAD && reputation < 1000 && reputation >= 0) {
        sadFace(x, y);
        lastEmo = SAD;
    }
    else if (lastEmo != NEUTRAL && reputation >= 1000 && reputation < 1200) {
        neutralFace(x, y);
        lastEmo = NEUTRAL;
    }
    else if (lastEmo != HAPPY && reputation >= 1200) {
        happyFace(x, y);
        lastEmo = HAPPY;
    }
}

void screenEmotion(const unsigned int x, const unsigned int y, const unsigned int emotion)
{
    switch (emotion) {
        case 0:
            angryFace(x, y);
            break;
        case 1:
            sadFace(x, y);
            break;
        case 2:
            neutralFace(x, y);
            break;
        case 3:
        default:
            happyFace(x, y);
    }
}