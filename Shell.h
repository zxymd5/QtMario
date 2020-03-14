#ifndef SHELL_H
#define SHELL_H

#include "Sprite.h"

class Shell : public Sprite
{
public:
    Shell(float x, float y, int type);
    void move();
    void bumpCheck(int xTile, int yTile);
    void collideCheck();
    bool shellCollideCheck(Shell *shell);
    bool fireballCollideCheck(Fireball *fireball);
    void die();
    void release(Mario *mario);
    static float GROUND_INERTIA;
    static float AIR_INERTIA;
    int width = 8;
    int height = 24;
    int facing;
    bool avoidCliffs = false;
    int anim = 0;
    bool dead = false;
    bool carried = false;
private:
    bool move(float xa, float ya);
    bool isBlocking(float _x, float _y, float xa, float ya);
    bool onGround = false;
    int deadTime = 0;
    float runTime = 0.0f;
};

#endif // SHELL_H
