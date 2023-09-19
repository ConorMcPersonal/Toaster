#include <arch/zx.h>
#include <stdio.h>
#include "util.h"
#include "face.h"

void happyFace(const int x, const int y)
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

void neutralFace(const int x, const int y)
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