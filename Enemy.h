#ifndef MARIO_ENEMY_H
#define MARIO_ENEMY_H

#include "Sprite.h"
class LevelScene;

class Enemy : public Sprite
{
public:
    static const int ENEMY_RED_KOOPA = 0;
    static const int ENEMY_GREEN_KOOPA = 1;
    static const int ENEMY_GOOMBA = 2;
    static const int ENEMY_SPIKY = 3;
    static const int ENEMY_FLOWER = 4;
    static float GROUND_INERTIA;
    static float AIR_INERTIA;

    Enemy(int x, int y, int dir, int type, bool winged);

    void move();
    bool move(float xa, float ya);
    void bumpCheck(int xTile, int yTile);
    void collideCheck();
    bool shellCollideCheck(Shell* shell);
    void render(QPainter &painter);
    bool fireballCollideCheck(Fireball *fireball);

    int width = 8;
    int height = 48;
    int facing;
    int deadTime = 0;
    bool flyDeath = false;
    bool avoidCliffs = true;
    bool winged = true;
    int wingTime = 0;
    bool noFireballDeath;

private:
    bool isBlocking(float _x, float _y, float xa, float ya);

    float runTime;
    bool onGround = false;
    bool mayJump = false;
    int jumpTime = 0;
    float xJumpSpeed;
    float yJumpSpeed;
    int type;
};

#endif
