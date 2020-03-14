#ifndef MUSHROOM_H
#define MUSHROOM_H

#include "Sprite.h"

class Mushroom : public Sprite
{
public:
    Mushroom(int x, int y);
    void move();
    void bumpCheck(int xTile, int yTile);
    void collideCheck();

    static float GROUND_INERTIA;
    static float AIR_INERTIA;
    int facing;
    bool avoidCliffs = false;

private:
    bool move(float xa, float ya);
    bool isBlocking(float _x, float _y, float xa, float ya);
    float runTime;
    bool onGround = false;
    bool mayJump = false;
    int jumpTime = 0;
    float xJumpSpeed = 0;
    float yJumpSpeed = 0;
    int width = 8;
    int height = 48;
    int life;
};

#endif // MUSHROOM_H
