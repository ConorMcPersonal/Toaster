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
    char faceData[] = { 0, 1, 0xe0, 0xe, 0x10, 0x08, 0x20, 0x44, 0x44, 0x4,
        0x40, 0x4, 0x40, 0x2, 0x80, 0x44, 0x44, 0x84, 0x43, 0x4, 0x40, 0x8, 
        0x20, 0x10, 0x10, 0xe0, 0xe, 0x0, 0x1, 0x0, 0x0};
    cheeky_face(x, y, (char *)&faceData);
}

void OLDhappyFace(const int x, const int y)
{
    int px = (x - 1) * 8;
    int py = (y - 1) * 8;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x1;
    *zx_pxy2saddr(px + 8, py) = 0xe0;
    *zx_pxy2saddr(px, py++) = 0xe;
    *zx_pxy2saddr(px + 8, py) = 0x10;
    *zx_pxy2saddr(px, py++) = 0x10;
    *zx_pxy2saddr(px + 8, py) = 0x08;
    *zx_pxy2saddr(px, py++) = 0x20;
    *zx_pxy2saddr(px + 8, py) = 0x44;
    *zx_pxy2saddr(px, py++) = 0x44;
    *zx_pxy2saddr(px + 8, py) = 0x4;
    *zx_pxy2saddr(px, py++) = 0x40;
    *zx_pxy2saddr(px + 8, py) = 0x4;
    *zx_pxy2saddr(px, py++) = 0x40;
    *zx_pxy2saddr(px + 8, py) = 0x2;
    *zx_pxy2saddr(px, py++) = 0x80;
    *zx_pxy2saddr(px + 8, py) = 0x44;
    *zx_pxy2saddr(px, py++) = 0x44;
    *zx_pxy2saddr(px + 8, py) = 0x84;
    *zx_pxy2saddr(px, py++) = 0x43;
    *zx_pxy2saddr(px + 8, py) = 0x4;
    *zx_pxy2saddr(px, py++) = 0x40;
    *zx_pxy2saddr(px + 8, py) = 0x8;
    *zx_pxy2saddr(px, py++) = 0x20;
    *zx_pxy2saddr(px + 8, py) = 0x10;
    *zx_pxy2saddr(px, py++) = 0x10;
    *zx_pxy2saddr(px + 8, py) = 0xe0;
    *zx_pxy2saddr(px, py++) = 0xe;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x1;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x0;
}

void OLD_neutralFace(const int x, const int y)
{
    int px = (x - 1) * 8;
    int py = (y - 1) * 8;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x1;
    *zx_pxy2saddr(px + 8, py) = 0xe0;
    *zx_pxy2saddr(px, py++) = 0xe;
    *zx_pxy2saddr(px + 8, py) = 0x10;
    *zx_pxy2saddr(px, py++) = 0x10;
    *zx_pxy2saddr(px + 8, py) = 0x08;
    *zx_pxy2saddr(px, py++) = 0x20;
    *zx_pxy2saddr(px + 8, py) = 0x44;
    *zx_pxy2saddr(px, py++) = 0x44;
    *zx_pxy2saddr(px + 8, py) = 0x4;
    *zx_pxy2saddr(px, py++) = 0x40;
    *zx_pxy2saddr(px + 8, py) = 0x4;
    *zx_pxy2saddr(px, py++) = 0x40;
    *zx_pxy2saddr(px + 8, py) = 0x2;
    *zx_pxy2saddr(px, py++) = 0x80;
    *zx_pxy2saddr(px + 8, py) = 0x4;
    *zx_pxy2saddr(px, py++) = 0x40;
    *zx_pxy2saddr(px + 8, py) = 0x84;
    *zx_pxy2saddr(px, py++) = 0x43;
    *zx_pxy2saddr(px + 8, py) = 0x4;
    *zx_pxy2saddr(px, py++) = 0x40;
    *zx_pxy2saddr(px + 8, py) = 0x8;
    *zx_pxy2saddr(px, py++) = 0x20;
    *zx_pxy2saddr(px + 8, py) = 0x10;
    *zx_pxy2saddr(px, py++) = 0x10;
    *zx_pxy2saddr(px + 8, py) = 0xe0;
    *zx_pxy2saddr(px, py++) = 0xe;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x1;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x0;
}

void sadFace(const int x, const int y)
{
    char faceData[] = { 0x0, 0x1, 0xe0, 0xe, 0x10, 0x10, 0x8, 0x20, 0x44,
        0x44, 0x4, 0x40, 0x4, 0x40, 0x2, 0x80, 0x4, 0x40, 0x84, 0x43, 0x44, 
        0x44, 0x8, 0x20, 0x10, 0x10, 0xe0, 0xe, 0x0, 0x1, 0x0, 0x0 };
    cheeky_face(x, y, (char *)&faceData);
}

void OLDsadFace(const int x, const int y)
{
    int px = (x - 1) * 8;
    int py = (y - 1) * 8;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x1;
    *zx_pxy2saddr(px + 8, py) = 0xe0;
    *zx_pxy2saddr(px, py++) = 0xe;
    *zx_pxy2saddr(px + 8, py) = 0x10;
    *zx_pxy2saddr(px, py++) = 0x10;
    *zx_pxy2saddr(px + 8, py) = 0x8;
    *zx_pxy2saddr(px, py++) = 0x20;
    *zx_pxy2saddr(px + 8, py) = 0x44;
    *zx_pxy2saddr(px, py++) = 0x44;
    *zx_pxy2saddr(px + 8, py) = 0x4;
    *zx_pxy2saddr(px, py++) = 0x40;
    *zx_pxy2saddr(px + 8, py) = 0x4;
    *zx_pxy2saddr(px, py++) = 0x40;
    *zx_pxy2saddr(px + 8, py) = 0x2;
    *zx_pxy2saddr(px, py++) = 0x80;
    *zx_pxy2saddr(px + 8, py) = 0x4;
    *zx_pxy2saddr(px, py++) = 0x40;
    *zx_pxy2saddr(px + 8, py) = 0x84;
    *zx_pxy2saddr(px, py++) = 0x43;
    *zx_pxy2saddr(px + 8, py) = 0x44;
    *zx_pxy2saddr(px, py++) = 0x44;
    *zx_pxy2saddr(px + 8, py) = 0x8;
    *zx_pxy2saddr(px, py++) = 0x20;
    *zx_pxy2saddr(px + 8, py) = 0x10;
    *zx_pxy2saddr(px, py++) = 0x10;
    *zx_pxy2saddr(px + 8, py) = 0xe0;
    *zx_pxy2saddr(px, py++) = 0xe;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x1;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x0;
}

void angryFace(const int x, const int y)
{
    char faceData[] = {  0x0, 0x1, 0xe0, 0xe, 0x10, 0x10, 0x8, 0x20, 0x44, 0x44, 0x40, 
        0x4, 0x84, 0x42, 0x04, 0x40, 0x84, 0x43, 0x44, 0x44, 0x44, 0x44, 0x88, 0x23, 
        0x10, 0x10, 0xe0, 0xe, 0x0, 0x1, 0x0, 0x0 };
    cheeky_face(x, y, (char *)&faceData);
}

void OLDangryFace(const int x, const int y)
{
    int px = (x - 1) * 8;
    int py = (y - 1) * 8;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x1;
    *zx_pxy2saddr(px + 8, py) = 0xe0;
    *zx_pxy2saddr(px, py++) = 0xe;
    *zx_pxy2saddr(px + 8, py) = 0x10;
    *zx_pxy2saddr(px, py++) = 0x10;
    *zx_pxy2saddr(px + 8, py) = 0x8;
    *zx_pxy2saddr(px, py++) = 0x20;
    *zx_pxy2saddr(px + 8, py) = 0x44;
    *zx_pxy2saddr(px, py++) = 0x44;
    *zx_pxy2saddr(px + 8, py) = 0x40;
    *zx_pxy2saddr(px, py++) = 0x4;
    *zx_pxy2saddr(px + 8, py) = 0x84;
    *zx_pxy2saddr(px, py++) = 0x42;
    *zx_pxy2saddr(px + 8, py) = 0x04;
    *zx_pxy2saddr(px, py++) = 0x40;
    *zx_pxy2saddr(px + 8, py) = 0x84;
    *zx_pxy2saddr(px, py++) = 0x43;
    *zx_pxy2saddr(px + 8, py) = 0x44;
    *zx_pxy2saddr(px, py++) = 0x44;
    *zx_pxy2saddr(px + 8, py) = 0x44;
    *zx_pxy2saddr(px, py++) = 0x44;
    *zx_pxy2saddr(px + 8, py) = 0x88;
    *zx_pxy2saddr(px, py++) = 0x23;
    *zx_pxy2saddr(px + 8, py) = 0x10;
    *zx_pxy2saddr(px, py++) = 0x10;
    *zx_pxy2saddr(px + 8, py) = 0xe0;
    *zx_pxy2saddr(px, py++) = 0xe;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x1;
    *zx_pxy2saddr(px + 8, py) = 0x0;
    *zx_pxy2saddr(px, py++) = 0x0;
}



void neutralFace(const int x, const int y)
{
    char faceData[] = { 0x0, 0x1, 0xe0, 0xe, 0x10, 0x10, 0x08, 0x20, 0x44, 0x44, 0x4, 0x40, 0x4, 0x40, 0x2, 0x80
                            , 0x4, 0x40, 0x84, 0x43, 0x4, 0x40, 0x8, 0x20, 0x10, 0x10, 0xe0, 0xe, 0x0, 0x1, 0x0, 0x0};
    cheeky_face(x, y, (char *)&faceData);
}

enum emotion {
    ANGRY, SAD, NEUTRAL, HAPPY, START
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