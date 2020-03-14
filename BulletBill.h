#ifndef BULLETBILL_H
#define BULLETBILL_H

#include "Sprite.h"

class BulletBill : public Sprite
{
public:
    BulletBill(float x, float y, int dir);
    void move();
    void collideCheck();

    bool shellCollideCheck(Shell *shell);

    bool fireballCollideCheck(Fireball *fireball);

    int width = 8;
    int height = 48;
    int facing;
    bool avoidCliffs = false;
    int anim = 0;
    bool dead = false;
private:
    bool move(float xa, float ya);
    int deadTime = 0;
};

#endif // BULLETBILL_H
