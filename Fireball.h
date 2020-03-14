#ifndef FIREBALL_H
#define FIREBALL_H

#include "Sprite.h"

class Fireball : public Sprite
{
public:
    Fireball(float x, float y, int facing);

    void move();
    bool isBlocking(float _x, float _y, float xa, float ya);
    void die();
    bool move(float xa, float ya);

    static float GROUND_INERTIA;
    static float AIR_INERTIA;
    int facing;
    bool avoidCliffs = false;
    int anim;
    bool dead = false;
    int height = 48;
private:
    float runTime;
    bool onGround = false;
    int width = 8;
    int deadTime = 0;
};

#endif // FIREBALL_H
