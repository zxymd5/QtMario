#ifndef FLOWERENEMY_H
#define FLOWERENEMY_H

#include "Enemy.h"

class FlowerEnemy : public Enemy
{
public:
    FlowerEnemy(int x, int y);
    void move();

private:
    long long llTick;
    int yStart;
    int jumpTime = 0;
};

#endif // FLOWERENEMY_H
