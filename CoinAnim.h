#ifndef COINANIM_H
#define COINANIM_H

#include "Sprite.h"

class CoinAnim : public Sprite
{
public:
    CoinAnim(int xTile, int yTile);
    void move();
private:
    int life = 10;
};

#endif // COINANIM_H
