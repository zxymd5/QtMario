#ifndef FIREFLOWER_H
#define FIREFLOWER_H

#include "Sprite.h"

class FireFlower : public Sprite
{
public:
    FireFlower(int x, int y);
    void move();
    void collideCheck();
    int width = 8;
    int height = 48;
    int facing;
    bool avoidCliffs = false;
private:
    int life;
};

#endif // FIREFLOWER_H
