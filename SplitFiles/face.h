#ifndef _FACE_H
#define _FACE_H

enum mood {
    ANGRY, SAD, NEUTRAL, HAPPY
};

//draw a number at (x, y)
void screenFace(const unsigned int x, const unsigned int y, mood face);

#endif
