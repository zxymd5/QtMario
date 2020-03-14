#ifndef SPARKLE_H
#define SPARKLE_H

#include "Sprite.h"

class Sparkle : public Sprite
{
public:
    Sparkle(int x, int y, float xa, float ya, int xPic, int yPic, int timeSpan);
    virtual void move();
    int life;
    int xPicStart;
};

#endif // SPARKLE_H
